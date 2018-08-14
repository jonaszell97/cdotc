//
// Created by Jonas Zell on 05.12.17.
//

#include "TemplateInstantiator.h"

#include "AST/ASTContext.h"
#include "AST/TypeBuilder.h"
#include "Basic/NestedNameSpecifier.h"
#include "IL/Constants.h"
#include "ILGen/ILGenPass.h"
#include "Message/Diagnostics.h"
#include "Module/Module.h"
#include "SemaPass.h"
#include "Serialization/ModuleFile.h"
#include "Template.h"

#include <sstream>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/ErrorHandling.h>
#include <llvm/ADT/SmallString.h>
#include <llvm/Support/PrettyStackTrace.h>

using namespace cdot::ast;
using namespace cdot::support;
using namespace cdot::diag;
using namespace cdot::sema;

namespace cdot {
namespace {

class DependencyResolver: public TypeBuilder<DependencyResolver> {
   const MultiLevelFinalTemplateArgList &templateArgs;
   Expression *TypeExpr;
   RecordDecl *Self;

public:
   DependencyResolver(SemaPass &SP,
                      const MultiLevelFinalTemplateArgList &templateArgs,
                      StmtOrDecl SOD,
                      RecordDecl *Self,
                      Expression *TypeExpr = nullptr)
      : TypeBuilder(SP, SOD),
        templateArgs(templateArgs), TypeExpr(TypeExpr), Self(Self)
   { }

   DependencyResolver(const DependencyResolver &Other)
      : TypeBuilder(Other.SP, Other.SOD),
        templateArgs(Other.templateArgs), TypeExpr(Other.TypeExpr),
        Self(Other.Self)
   {}

   friend class cdot::InstantiatorImpl;

   QualType visit(const SourceType &Ty)
   {
      TypeExpr = Ty.getTypeExpr();
      return TypeBuilder::visit(Ty.getResolvedType());
   }

   QualType visitBuiltinType(BuiltinType *T)
   {
      if (T->getKind() != Type::Self || !Self)
         return T;

      return SP.getContext().getRecordType(Self);
   }

   QualType visitGenericType(GenericType *T)
   {
      if (auto Arg = templateArgs.getArgForParam(T->getParam())) {
         if (Arg->isType() && !Arg->isVariadic())
            return Arg->getType();
      }

      return T;
   }

   QualType visitDependentSizeArrayType(DependentSizeArrayType *T)
   {
      auto Ident = dyn_cast<IdentifierRefExpr>(T->getSizeExpr()->getExpr());
      if (!Ident || Ident->getKind() != IdentifierKind::TemplateParam)
         return T;

      auto Param = Ident->getTemplateParam();

      // have to lookup via name because the address might change if an
      // outer record is instantiated
      auto *Arg = templateArgs.getNamedArg(Param->getDeclName());
      if (!Arg)
         return T;

      assert(Arg->isValue() && "used type for array element size?");
      assert(isa<il::ConstantInt>(Arg->getValue()) && "invalid array size");

      return Ctx.getArrayType(TypeBuilder::visit(T->getElementType()),
                              cast<il::ConstantInt>(Arg->getValue())
                                 ->getZExtValue());
   }

   QualType visitAssociatedType(AssociatedType *T)
   {
      if (!Self)
         return T;

      auto *AT = SP.LookupSingle<AssociatedTypeDecl>(
         *Self, T->getDecl()->getDeclName());

      if (AT && AT->isImplementation()) {
         SP.ensureDeclared(AT);
         return Ctx.getAssociatedType(AT);
      }

      return T;
   }

//   QualType visitDependentNameType(DependentNameType *T)
//   {
//      return SP.resolveDependencies(T, templateArgs, Self);
//   }

   QualType visitFunctionType(FunctionType *T)
   {
      return SP.resolveDependencies(T, templateArgs, Self);
   }

   QualType visitTupleType(TupleType *T)
   {
      return SP.resolveDependencies(T, templateArgs, Self);
   }

   QualType visitRecordType(RecordType *T)
   {
      return SP.resolveDependencies(T, templateArgs, Self);
   }

   QualType visitDependentRecordType(DependentRecordType *T)
   {
      return SP.resolveDependencies(T, templateArgs, Self);
   }
};

} // anonymous namespace

class InstantiatorImpl: public ASTTypeVisitor<InstantiatorImpl> {
public:
   InstantiatorImpl(SemaPass &SP, MultiLevelFinalTemplateArgList &&templateArgs,
                    StmtOrDecl SOD = StmtOrDecl())
      : ASTTypeVisitor(false),
        SP(SP), Context(SP.getContext()), InstScope(SP, &SP.getDeclContext()),
        templateArgs(move(templateArgs)),
        TypeVisitor(SP, this->templateArgs, SOD, nullptr)
   {
      SourceID = SP.getCompilationUnit().getFileMgr()
                   .getSourceId(SP.getCurrentDecl()->getSourceLoc());
   }

   InstantiatorImpl(SemaPass &SP, RecordDecl *R)
      : ASTTypeVisitor(false),
        SP(SP), Context(SP.getContext()), InstScope(SP, &SP.getDeclContext()),
        InstantiatingRecord(R),
        TypeVisitor(SP, this->templateArgs, R, R)
   {
      SourceID = SP.getCompilationUnit().getFileMgr()
                   .getSourceId(SP.getCurrentDecl()->getSourceLoc());
   }

   InstantiatorImpl(SemaPass &SP,
                    MultiLevelFinalTemplateArgList &&templateArgs,
                    NamedDecl* Template)
      : ASTTypeVisitor(false),
        SP(SP), Context(SP.getContext()),
        InstScope(SP, Template->getDeclContext()),
        templateArgs(move(templateArgs)), Template(Template),
        TypeVisitor(SP, this->templateArgs, Template, nullptr)
   {
      SourceID = SP.getCompilationUnit().getFileMgr()
                   .getSourceId(SP.getCurrentDecl()->getSourceLoc());
   }

   InstantiatorImpl(SemaPass &SP,
                    IdentifierInfo *SubstName,
                    il::Constant *SubstVal)
      : ASTTypeVisitor(false),
        SP(SP), Context(SP.getContext()), InstScope(SP, &SP.getDeclContext()),
        TypeVisitor(SP, this->templateArgs, nullptr, nullptr),
        ValueSubst{ SubstName, SubstVal }
   {
      SourceID = SP.getCompilationUnit().getFileMgr()
                   .getSourceId(SP.getCurrentDecl()->getSourceLoc());
   }

   InstantiatorImpl(InstantiatorImpl &&Other) noexcept
     : ASTTypeVisitor(false),
       SP(Other.SP), Context(Other.Context),
       InstScope(Other.InstScope),
       templateArgs(std::move(Other.templateArgs)),
       Template(Other.Template),
       InstantiatingRecord(Other.InstantiatingRecord),
       TypeVisitor(SP, templateArgs, Other.Template,
                   Other.TypeVisitor.Self, Other.TypeVisitor.TypeExpr),
       DelayedTypeDecls(move(Other.DelayedTypeDecls)),
       VariadicTemplateArgs(move(Other.VariadicTemplateArgs)),
       SourceID(Other.SourceID),
       ValueSubst(Other.ValueSubst),
       SubstContexts(move(Other.SubstContexts))
   {

   }

   friend class cdot::TemplateInstantiator;

   RecordDecl *instantiateRecordDecl(RecordDecl *Decl);
   RecordDecl *instantiateRecordDefinition(RecordDecl *Template,
                                           RecordDecl *Inst);

   MethodDecl *instantiateMethodDecl(MethodDecl *Decl)
   {
      return cast_or_null<MethodDecl>(visit(Decl));
   }

   InitDecl *instantiateInitDecl(InitDecl *Decl)
   {
      return cast_or_null<InitDecl>(visit(Decl));
   }

   DeinitDecl *instantiateDeinitDecl(DeinitDecl *Decl)
   {
      return cast_or_null<DeinitDecl>(visit(Decl));
   }

   FunctionDecl *instantiateFunctionDecl(FunctionDecl *Decl)
   {
      return cast_or_null<FunctionDecl>(visit(Decl));
   }

   AliasDecl *instantiateAliasDecl(AliasDecl *Decl)
   {
      return cast_or_null<AliasDecl>(visit(Decl));
   }

   Statement *instantiateStatement(Statement *Stmt)
   {
      return visit(Stmt);
   }

   Decl *instantiateDecl(Decl *D)
   {
      return visit(D);
   }

   NamedDecl *getTemplate() const { return Template; }
   const MultiLevelFinalTemplateArgList &getTemplateArgs() const
   {
      return templateArgs;
   }

   Decl *ActOnDecl(Decl *D, DeclContext *Ctx = nullptr)
   {
      if (!Ctx)
         Ctx = &SP.getDeclContext();

      SP.ActOnDecl(Ctx, D);
      return D;
   }

private:
   bool canElideInstantiation(Statement *Stmt)
   {
      if (InUnevaluatedScope)
         return false;

      if (auto Decl = dyn_cast<DeclStmt>(Stmt)) {
         return canElideInstantiation(Decl->getDecl());
      }

#  ifndef NDEBUG
   if (isa<DebugStmt>(Stmt))
      return false;
#  endif

      return !Stmt->isDependent();
   }

   bool canElideInstantiation(Expression *E)
   {
      if (InUnevaluatedScope)
         return false;

      if (!E->getExprType() || E->getExprType()->isUnknownAnyType())
         return false;

      if (auto Ident = dyn_cast<IdentifierRefExpr>(E)) {
         if (Ident->getKind() != IdentifierKind::MetaType)
            // property method arguments are always instantiated
            if (auto D = Ident->getNamedDecl())
               return !E->isDependent() && !D->isSynthesized();
      }

      return !E->isDependent();
   }

   bool canElideInstantiation(Decl *D)
   {
      if (InUnevaluatedScope)
         return false;

      if (D->isDependent())
         return false;

      if (isa<NamedDecl>(D)) {
         switch (D->getKind()) {
         case Decl::LocalVarDeclID:
         case Decl::MacroDeclID:
         case Decl::MacroExpansionDeclID:
            return true;
         case Decl::FuncArgDeclID:
            return !cast<FuncArgDecl>(D)->isSelf();
         default:
            return false;
         }
      }

      return false;
   }

   Expression* visit(Expression *expr)
   {
      assert((expr->isSemanticallyChecked() || expr->isContextDependent())
                && "instantiating unchecked expr");

      if (canElideInstantiation(expr))
         return expr;

      Expression *Inst;
      switch (expr->getTypeID()) {
#        define CDOT_EXPR(Name)                                      \
            case AstNode::Name##ID:                                  \
               Inst = visit##Name(static_cast<Name*>(expr)); break;
#        include "AST/AstNode.def"

         default:
            llvm_unreachable("not an expression!");
      }

      Inst->setUnsafe(expr->isUnsafe());
      Inst->setEllipsisLoc(expr->getEllipsisLoc());

      return Inst;
   }

   Statement* visit(Statement *stmt)
   {
      if (auto E = dyn_cast<Expression>(stmt))
         return visit(E);

      assert(stmt->isSemanticallyChecked() && "instantiating unchecked expr");

      if (canElideInstantiation(stmt))
         return stmt;

      Statement *Inst;
      switch (stmt->getTypeID()) {
#        define CDOT_STMT(Name)                                      \
            case AstNode::Name##ID:                                  \
               Inst = visit##Name(static_cast<Name*>(stmt)); break;
#        include "AST/AstNode.def"

         default:
            llvm_unreachable("not a statement!");
      }

      if (Inst)
         Inst->setUnsafe(stmt->isUnsafe());

      return Inst;
   }

   Decl *visit(Decl *D)
   {
      if (auto ND = dyn_cast<NamedDecl>(D))
         return visit(ND);

      assert(D->wasDeclared() && "instantiating unchecked decl");

      if (canElideInstantiation(D))
         return D;

      Decl *Inst;
      switch (D->getKind()) {
#        define CDOT_DECL(Name)                                   \
            case Decl::Name##ID:                                  \
               Inst = visit##Name(static_cast<Name*>(D)); break;
#        include "AST/Decl.def"

      default:
         llvm_unreachable("not a decl!");
      }

      if (Inst)
         Context.setAttributes(Inst, D->getAttributes());

      return Inst;
   }

   NamedDecl *visit(NamedDecl *D)
   {
      assert(D->wasDeclared() && "instantiating unchecked decl");

      if (canElideInstantiation(D))
         return D;

      NamedDecl *Inst;
      switch (D->getKind()) {
#        define CDOT_NAMED_DECL(Name)                             \
            case Decl::Name##ID:                                  \
               Inst = visit##Name(static_cast<Name*>(D)); break;
#        include "AST/Decl.def"

      default:
         llvm_unreachable("not a named decl!");
      }

      if (Inst) {
         Inst->setAccessLoc(D->getAccessLoc());
         Context.setAttributes(Inst, D->getAttributes());
      }

      return Inst;
   }

public:
   void visitType(const SourceType &Ty)
   {
      if (Ty.isResolved()) {
         auto ResolvedTy = Ty.getResolvedType();
         if (!ResolvedTy->isDependentType()) {
            Ty.setTypeExpr(nullptr);
            return;
         }

         if (!(isa<BuiltinType>(*ResolvedTy)
               && ResolvedTy->isUnknownAnyType())) {
            auto Inst = TypeVisitor.visit(Ty);
            if (Inst && !Inst->isDependentType()) {
               Ty.setResolvedType(Inst);
               return;
            }
         }
      }

      if (auto E = Ty.getTypeExpr()) {
         Ty.setTypeExpr(visit(Ty.getTypeExpr()));
         Ty.setResolvedType(QualType());

         return;
      }
   }

   SourceType visit(const SourceType &Ty)
   {
      if (Ty.isResolved()) {
         auto ResolvedTy = Ty.getResolvedType();
         if (!ResolvedTy->isDependentType())
            return SourceType(ResolvedTy);

         if (!(isa<BuiltinType>(*ResolvedTy)
               && ResolvedTy->isUnknownAnyType())) {
            auto Inst = TypeVisitor.visit(Ty);
            if (Inst && !Inst->isDependentType())
               return SourceType(Inst);
         }
      }

      if (auto E = Ty.getTypeExpr()) {
         return SourceType(visit(Ty.getTypeExpr()));
      }

      return SourceType();
   }

   void visitSourceTypes()
   {
      for (auto *D : DelayedTypeDecls) {
         auto *Ctx = D->getLexicalContext();
         if (Ctx)
            Ctx = Ctx->lookThroughExtension();
         else
            Ctx = &SP.getDeclContext();

         SemaPass::DeclScopeRAII DSR(SP, Ctx);

//         if (auto *R = dyn_cast<RecordDecl>(Ctx)) {
//            TypeVisitor.Self = R;
//         }
//         else {
//            TypeVisitor.Self = nullptr;
//         }

         ASTTypeVisitor::visit(D);
      }
   }

private:
   SourceType visitOrAuto(const SourceType &Ty)
   {
      auto Result = visit(Ty);
      if (!Result)
         return SourceType(Context.getAutoType());

      return Result;
   }

   CompoundDecl* visitCompoundDecl(CompoundDecl *D);
   CompoundStmt* visitCompoundStmt(CompoundStmt *node);

   PrecedenceGroupDecl *visitPrecedenceGroupDecl(PrecedenceGroupDecl*)
   {
      llvm_unreachable("precedence groups can't be dependent!");
   }

   OperatorDecl *visitOperatorDecl(OperatorDecl*)
   {
      llvm_unreachable("should not be in a template!");
   }

   NamespaceDecl* visitNamespaceDecl(NamespaceDecl*)
   {
      llvm_unreachable("should not be in a template!");
   }

   MacroDecl *visitMacroDecl(MacroDecl*)
   {
      llvm_unreachable("macros can't be dependent!");
   }

   MacroVariableExpr *visitMacroVariableExpr(MacroVariableExpr*)
   {
      llvm_unreachable("macros can't be dependent!");
   }

   ModuleDecl* visitModuleDecl(ModuleDecl *node)
   {
      llvm_unreachable("module should not be instantiated!");
   }

   MacroExpansionExpr *visitMacroExpansionExpr(MacroExpansionExpr *Expr);
   MacroExpansionStmt *visitMacroExpansionStmt(MacroExpansionStmt *Stmt);
   MacroExpansionDecl *visitMacroExpansionDecl(MacroExpansionDecl *Decl);

   UsingDecl* visitUsingDecl(UsingDecl *node);
   ImportDecl* visitImportDecl(ImportDecl *node);

   UnittestDecl *visitUnittestDecl(UnittestDecl *D);

   LocalVarDecl* visitLocalVarDecl(LocalVarDecl *node);
   GlobalVarDecl* visitGlobalVarDecl(GlobalVarDecl *node);
   DestructuringDecl* visitDestructuringDecl(DestructuringDecl *Decl);

   FunctionDecl* visitFunctionDecl(FunctionDecl *F);

   RecordDecl *visitRecordCommon(RecordDecl *Template);

   TemplateParamDecl *visitTemplateParamDecl(TemplateParamDecl *decl);

   ClassDecl* visitClassDecl(ClassDecl *node);
   StructDecl* visitStructDecl(StructDecl *node);
   ProtocolDecl* visitProtocolDecl(ProtocolDecl *node);
   ExtensionDecl* visitExtensionDecl(ExtensionDecl *Ext,
                                     RecordDecl *RecInst = nullptr);
   EnumDecl* visitEnumDecl(EnumDecl *node);
   UnionDecl* visitUnionDecl(UnionDecl *node);

   EnumCaseDecl* visitEnumCaseDecl(EnumCaseDecl *node);

   FieldDecl* visitFieldDecl(FieldDecl *Decl);
   PropDecl* visitPropDecl(PropDecl *Decl, bool IsFieldAccessor = false);
   SubscriptDecl* visitSubscriptDecl(SubscriptDecl *Decl);
   AssociatedTypeDecl* visitAssociatedTypeDecl(AssociatedTypeDecl *node);

   MethodDecl* visitMethodDecl(MethodDecl *M);
   InitDecl* visitInitDecl(InitDecl *Decl);
   DeinitDecl* visitDeinitDecl(DeinitDecl *Decl);

   FuncArgDecl* visitFuncArgDecl(FuncArgDecl *Decl);

   TypedefDecl* visitTypedefDecl(TypedefDecl *TD);
   AliasDecl* visitAliasDecl(AliasDecl *Alias);

   Expression* visitParenExpr(ParenExpr *Expr);
   Expression* visitIdentifierRefExpr(IdentifierRefExpr *Ident);
   BuiltinIdentExpr* visitBuiltinIdentExpr(BuiltinIdentExpr *node);
   SelfExpr* visitSelfExpr(SelfExpr *node);
   SuperExpr* visitSuperExpr(SuperExpr *node);

   Expression* visitBuiltinExpr(BuiltinExpr *node);

   SubscriptExpr* visitSubscriptExpr(SubscriptExpr *node);
   Expression* visitCallExpr(CallExpr *node);
   Expression* visitAnonymousCallExpr(AnonymousCallExpr *Expr);
   EnumCaseExpr* visitEnumCaseExpr(EnumCaseExpr *node);
   TupleMemberExpr* visitTupleMemberExpr(TupleMemberExpr *node);
   TemplateArgListExpr* visitTemplateArgListExpr(TemplateArgListExpr *Expr);

   ForStmt* visitForStmt(ForStmt *node);
   ForInStmt* visitForInStmt(ForInStmt *node);
   WhileStmt* visitWhileStmt(WhileStmt *node);
   IfStmt* visitIfStmt(IfStmt *node);

   MatchStmt* visitMatchStmt(MatchStmt *node);
   CaseStmt* visitCaseStmt(CaseStmt *node);
   ExpressionPattern* visitExpressionPattern(ExpressionPattern *node);

   IsPattern* visitIsPattern(IsPattern *node);
   CasePattern* visitCasePattern(CasePattern *node);

   ReturnStmt* visitReturnStmt(ReturnStmt *node);

   BreakStmt* visitBreakStmt(BreakStmt *node)
   {
      llvm_unreachable("should not be dependent!");
   }

   ContinueStmt* visitContinueStmt(ContinueStmt *node)
   {
      llvm_unreachable("should not be dependent!");
   }

   DiscardAssignStmt *visitDiscardAssignStmt(DiscardAssignStmt *Stmt);

   IntegerLiteral* visitIntegerLiteral(IntegerLiteral *node);
   FPLiteral* visitFPLiteral(FPLiteral *node);
   BoolLiteral* visitBoolLiteral(BoolLiteral *node);
   CharLiteral* visitCharLiteral(CharLiteral *node);

   DictionaryLiteral* visitDictionaryLiteral(DictionaryLiteral *node);
   ArrayLiteral* visitArrayLiteral(ArrayLiteral *node);

   NoneLiteral* visitNoneLiteral(NoneLiteral *node);
   StringLiteral* visitStringLiteral(StringLiteral *node);

   StringInterpolation* visitStringInterpolation(StringInterpolation *node);

   TupleLiteral* visitTupleLiteral(TupleLiteral *node);

   Expression* visitExprSequence(ExprSequence *node);
   Expression* visitBinaryOperator(BinaryOperator *node);
   Expression *visitAssignExpr(AssignExpr *Expr);
   CastExpr* visitCastExpr(CastExpr *node);
   TypePredicateExpr* visitTypePredicateExpr(TypePredicateExpr *node);
   UnaryOperator* visitUnaryOperator(UnaryOperator *node);

   IfExpr *visitIfExpr(IfExpr *Expr);

   StaticExpr* visitStaticExpr(StaticExpr *node);
   ConstraintExpr* visitConstraintExpr(ConstraintExpr *node);
   TraitsExpr* visitTraitsExpr(TraitsExpr *node);

   DeclStmt *visitDeclStmt(DeclStmt *Stmt);

   StaticAssertStmt* visitStaticAssertStmt(StaticAssertStmt *node);
   StaticPrintStmt* visitStaticPrintStmt(StaticPrintStmt *node);
   Decl* visitStaticIfDecl(StaticIfDecl *node);
   Decl* visitStaticForDecl(StaticForDecl *node);

   Statement* visitStaticIfStmt(StaticIfStmt *node);
   Statement* visitStaticForStmt(StaticForStmt *node);

   AttributedStmt *visitAttributedStmt(AttributedStmt *S);
   AttributedExpr *visitAttributedExpr(AttributedExpr *E);

   DoStmt *visitDoStmt(DoStmt *node);
   TryExpr *visitTryExpr(TryExpr *Expr);
   AwaitExpr *visitAwaitExpr(AwaitExpr *Expr);
   ThrowStmt *visitThrowStmt(ThrowStmt *node);

   LambdaExpr* visitLambdaExpr(LambdaExpr *node);

   Expression* visitImplicitCastExpr(ImplicitCastExpr *node);

   DebugStmt* visitDebugStmt(DebugStmt *node);
   DebugDecl* visitDebugDecl(DebugDecl *D);

   NullStmt* visitNullStmt(NullStmt *node)
   {
      llvm_unreachable("should not be dependent!");
   }

   MixinExpr *visitMixinExpr(MixinExpr *Expr);
   MixinStmt *visitMixinStmt(MixinStmt *Stmt);
   MixinDecl *visitMixinDecl(MixinDecl *Decl);

   FunctionTypeExpr *visitFunctionTypeExpr(FunctionTypeExpr *Expr);
   TupleTypeExpr *visitTupleTypeExpr(TupleTypeExpr *Expr);
   ArrayTypeExpr *visitArrayTypeExpr(ArrayTypeExpr *Expr);
   DeclTypeExpr *visitDeclTypeExpr(DeclTypeExpr *Expr);
   PointerTypeExpr *visitPointerTypeExpr(PointerTypeExpr *Expr);
   ReferenceTypeExpr *visitReferenceTypeExpr(ReferenceTypeExpr *Expr);
   OptionTypeExpr *visitOptionTypeExpr(OptionTypeExpr *Expr);

   NestedNameSpecifier *visitNestedNameSpecifier(NestedNameSpecifier *Name);

   /// Reference to the Sema instance.
   SemaPass &SP;

   /// Reference to the AST context.
   ASTContext &Context;
   
   /// Declaration scope for this instantiation.
   SemaPass::DeclScopeRAII InstScope;

   /// The template arguments we are instantiating with.
   MultiLevelFinalTemplateArgList templateArgs;
   
   /// The template we are instantiating.
   NamedDecl* Template = nullptr;

   /// The record currently being instantiated.
   RecordDecl *InstantiatingRecord = nullptr;

   /// The type instantiator.
   DependencyResolver TypeVisitor;

   /// Declarations with types that need delayed instantiation.
   SmallVector<Decl*, 4> DelayedTypeDecls;
   
   /// Map from template parameter names to variadic template arguments.
   llvm::StringMap<ResolvedTemplateArg*> VariadicTemplateArgs;

   /// True iff we're currently in an unevaluated scope.
   bool InUnevaluatedScope = false;

   /// Source ID we're instantiating from.
   unsigned SourceID = fs::InvalidID;

   /// Value substitution in a 'static for'.
   struct {
      IdentifierInfo *Name;
      il::Constant *SubstVal = nullptr;
   } ValueSubst;

   struct UnevalutedScopeRAII {
      UnevalutedScopeRAII(InstantiatorImpl &Inst, bool unevaluated)
         : Inst(Inst), previous(Inst.InUnevaluatedScope)
      {
         Inst.InUnevaluatedScope |= unevaluated;
      }

      ~UnevalutedScopeRAII()
      {
         Inst.InUnevaluatedScope = previous;
      }

   private:
      InstantiatorImpl &Inst;
      bool previous;
   };

   struct SubstContext {
   public:
      SubstContext(TemplateParamDecl *Param,
                   const IdentifierInfo *FuncArg,
                   DeclarationName IdentSubst,
                   const ResolvedTemplateArg *ArgSubst)
         : Param(Param), FuncArg(FuncArg), IdentSubst(IdentSubst),
           ArgSubst(ArgSubst)
      { }

      TemplateParamDecl *Param;

      const IdentifierInfo *FuncArg;
      DeclarationName IdentSubst;

      const ResolvedTemplateArg *ArgSubst;
   };

   struct SubstContextRAII {
      SubstContextRAII(InstantiatorImpl &Inst,
                       TemplateParamDecl *Param,
                       const IdentifierInfo *FuncArg,
                       DeclarationName IdentSubst,
                       const ResolvedTemplateArg *ArgSubst)
         : Inst(Inst)
      {
         Inst.SubstContexts.emplace_back(Param, FuncArg, IdentSubst, ArgSubst);
      }

      ~SubstContextRAII()
      {
         Inst.SubstContexts.pop_back();
      }

   private:
      InstantiatorImpl &Inst;
   };

   std::vector<SubstContext> SubstContexts;

   struct InstantiatingRecordRAII {
      InstantiatingRecordRAII(InstantiatorImpl &Inst, RecordDecl *R)
         : Inst(Inst), Prev(Inst.InstantiatingRecord)
      {
         Inst.InstantiatingRecord = R;
      }

      ~InstantiatingRecordRAII()
      {
         Inst.InstantiatingRecord = Prev;
      }

   private:
      InstantiatorImpl &Inst;
      RecordDecl *Prev;
   };

   DeclarationName getVariadicArgumentSubst(const IdentifierInfo *II)
   {
      auto end_it = SubstContexts.rend();
      for (auto it = SubstContexts.rbegin(); it != end_it; ++it) {
         auto &Subst = *it;
         if (!Subst.FuncArg)
            continue;

         if (Subst.FuncArg == II)
            return Subst.IdentSubst;
      }

      return nullptr;
   }

   const ResolvedTemplateArg *getParameterSubst(DeclarationName name)
   {
      auto end_it = SubstContexts.rend();
      for (auto it = SubstContexts.rbegin(); it != end_it; ++it) {
         auto &Subst = *it;
         if (!Subst.ArgSubst)
            continue;

         if (Subst.Param->getDeclName() == name)
            return Subst.ArgSubst;
      }

      return nullptr;
   }

   const ResolvedTemplateArg *getParameterSubst(TemplateParamDecl *P)
   {
      auto end_it = SubstContexts.rend();
      for (auto it = SubstContexts.rbegin(); it != end_it; ++it) {
         auto &Subst = *it;
         if (!Subst.ArgSubst)
            continue;

         if (Subst.Param->getDeclName() == P->getDeclName())
            return Subst.ArgSubst;
      }

      return nullptr;
   }

   const ResolvedTemplateArg* hasTemplateArg(DeclarationName Name)
   {
      if (auto P = getParameterSubst(Name))
         return P;

      return templateArgs.getNamedArg(Name);
   }

   const ResolvedTemplateArg* hasTemplateArg(TemplateParamDecl *P)
   {
      if (auto Arg = getParameterSubst(P))
         return Arg;

      return templateArgs.getNamedArg(P->getDeclName());
   }

   IfCondition visitIfCondition(const IfCondition &C);

   Expression *makeLiteralExpr(Expression *Expr,
                               QualType Ty,
                               const Variant &Val);

   ASTContext const& getContext() const
   {
      return SP.getCompilationUnit().getContext();
   }

   template<class T, class ...Args>
   T* makeStmt(Statement const* node, Args&& ...args)
   {
      auto ret = SP.makeStmt<T>(std::forward<Args&&>(args)...);
      ret->setSourceLoc(node->getSourceLoc());

      return ret;
   }

   template<class T, class ...Args>
   T* makeDecl(NamedDecl const *node, Args &&...args)
   {
      auto ret = SP.makeStmt<T>(std::forward<Args&&>(args)...);
      ret->setSourceLoc(node->getSourceLoc());

      return ret;
   }

   template<class T, class ...Args>
   T* makeDecl(Decl const *node, Args &&...args)
   {
      auto ret = SP.makeStmt<T>(std::forward<Args&&>(args)...);
      ret->setSourceLoc(node->getSourceLoc());

      return ret;
   }

   template<class T, class ...Args>
   T* makeExpr(Expression const* node, Args&& ...args)
   {
      auto ret = SP.makeStmt<T>(std::forward<Args&&>(args)...);

      ret->setSourceLoc(node->getSourceLoc());
      ret->setIsVariadicArgPackExpansion(node->isVariadicArgPackExpansion());

      return ret;
   }

   template<class T>
   std::vector<T*> cloneVector(const llvm::iterator_range<T**> &vec)
   {
      std::vector<T*> newVec;
      for (const auto &v : vec) {
         auto newStmt = visit(v);
         newVec.push_back(cast<T>(newStmt));
      }

      return newVec;
   }

   template<class T>
   std::vector<T*> cloneVector(const std::vector<T*> &vec)
   {
      std::vector<T*> newVec;
      for (const auto &v : vec) {
         auto newStmt = visit(v);
         newVec.push_back(cast<T>(newStmt));
      }

      return newVec;
   }

   template<class T>
   std::vector<T*> cloneVector(llvm::ArrayRef<T*> vec)
   {
      std::vector<T*> newVec;
      for (const auto &v : vec) {
         auto newStmt = visit(v);
         newVec.push_back(cast<T>(newStmt));
      }

      return newVec;
   }

   template<class T>
   T* copyOrNull(T* sp)
   {
      if (!sp)
         return nullptr;

      return cast<T>(visit(sp));
   }

   template<class T>
   T* clone(T* sp)
   {
      return cast<T>(visit(sp));
   }

   template<class T>
   ASTVector<TemplateParamDecl*> copyTemplateParameters(const T *from)
   {
      auto OriginalParams = from->getTemplateParams();
      ASTVector<TemplateParamDecl*> Params(Context, OriginalParams.size());

      for (auto &P : OriginalParams) {
         if (!hasTemplateArg(P->getDeclName()))
            Params.push_back(clone(P), Context);
      }

      return Params;
   }

public:
   void copyArgListAndFindVariadic(
                              CallableDecl *C,
                              llvm::SmallVectorImpl<FuncArgDecl*> &Variadics);

private:
   template<class Container,
            class Expr = typename
               std::remove_reference<decltype(*Container().front())>::type>
   std::vector<Expr*> copyExprList(const Container &exprList);

   void copyArgumentList(ArrayRef<Expression*> Args,
                         ArrayRef<IdentifierInfo*> Labels,
                         SmallVectorImpl<Expression*> &ResultArgs,
                         SmallVectorImpl<IdentifierInfo*> &ResultLabels);

   void copyParameterList(ArrayRef<SourceType> Args,
                          ArrayRef<FunctionType::ParamInfo> ParamInfo,
                          SmallVectorImpl<SourceType> &ResultArgs,
                          SmallVectorImpl<FunctionType::ParamInfo> &ResultInfo);

   std::vector<SourceType> copyTypeList(llvm::ArrayRef<SourceType> Tys);
   ASTVector<SourceType> copyConformances(llvm::ArrayRef<SourceType> Tys);

   ASTVector<Expression*> astvec(ArrayRef<Expression*> Vec)
   {
      ASTVector<Expression*> Copy(Context, Vec.size());
      Copy.insert(Context, Copy.end(), Vec.begin(), Vec.end());

      return Copy;
   }

   class VariadicParamFinder: public RecursiveASTVisitor<VariadicParamFinder> {
   public:
      VariadicParamFinder(
         llvm::SmallPtrSetImpl<TemplateParamDecl*> &Params,
         llvm::SmallPtrSetImpl<IdentifierRefExpr*> &PackArguments)
         : VariadicParams(Params), PackArguments(PackArguments)
      { }

      bool visitIdentifierRefExpr(IdentifierRefExpr *Expr)
      {
         if (Expr->getKind() == IdentifierKind::TemplateParam) {
            auto Param = Expr->getTemplateParam();
            if (Param->isVariadic()) {
               VariadicParams.insert(Param);
               return false;
            }
         }
         else if (Expr->getKind() == IdentifierKind::FunctionArg) {
            auto Arg = Expr->getFuncArg();
            if (Arg->isVariadicArgPackExpansion()) {
               PackArguments.insert(Expr);
               visit(Arg->getType().getTypeExpr());
            }
         }
         else if (Expr->getKind() == IdentifierKind::Field) {
            auto F = Expr->getFieldDecl();
            if (F->isVariadic()) {
               PackArguments.insert(Expr);
               visit(F->getType().getTypeExpr());
            }
         }

         return true;
      }

   private:
      llvm::SmallPtrSetImpl<TemplateParamDecl*> &VariadicParams;
      llvm::SmallPtrSetImpl<IdentifierRefExpr*> &PackArguments;
   };

   template<class Container>
   using element_type = typename
      std::remove_pointer<typename
                          std::remove_reference<decltype(((Container*)0)->front())>
                             ::type>::type;

   template<class Container, class Element = element_type<Container>>
   bool expandVariadicArgument(Container &exprs,
                               Expression* variadicExpr) {
      if (!variadicExpr->isVariadicArgPackExpansion()) {
         exprs.emplace_back(cast<Element>(visit(variadicExpr)));
         return false;
      }

      llvm::SmallPtrSet<TemplateParamDecl*, 4> VariadicParams;
      llvm::SmallPtrSet<IdentifierRefExpr*, 4> PackArguments;

      VariadicParamFinder Visitor(VariadicParams, PackArguments);
      Visitor.visit(variadicExpr);

      assert(!VariadicParams.empty() && "undiagnosed invalid pack expansion");

      if (VariadicParams.size() > 1) {
         llvm_unreachable("FIXME error message");
      }

      TemplateParamDecl *ParameterPack = *VariadicParams.begin();
      IdentifierRefExpr *PackArgument = nullptr;

      if (!PackArguments.empty())
         PackArgument = *PackArguments.begin();

      auto GivenArg = hasTemplateArg(ParameterPack);
      if (!GivenArg) {
         // current context is still templated
         exprs.emplace_back(cast<Element>(visit(variadicExpr)));
         return false;
      }

      assert(GivenArg->isVariadic() && "invalid passed template argument");

      unsigned numVariadics = (unsigned)GivenArg->getVariadicArgs().size();
      for (unsigned i = 0; i < numVariadics; ++i) {
         DeclarationName IdentSubst;
         const IdentifierInfo *FuncArg = nullptr;

         if (PackArgument) {
            FuncArg = PackArgument->getIdentInfo();
            IdentSubst = Context.getDeclNameTable()
                                .getPackExpansionName(FuncArg, i);
         }

         SubstContextRAII raii(*this, ParameterPack, FuncArg, IdentSubst,
                               &GivenArg->getVariadicArgs()[i]);

         Expression *newExpr = cast<Element>(visit(variadicExpr));
         newExpr->setEllipsisLoc(SourceLocation());

         exprs.emplace_back(newExpr);
      }

      return true;
   }

   bool expandVariadicDecl(FieldDecl *Decl)
   {
      llvm::SmallPtrSet<TemplateParamDecl*, 4> VariadicParams;
      llvm::SmallPtrSet<IdentifierRefExpr*, 4> PackArguments;

      VariadicParamFinder Visitor(VariadicParams, PackArguments);
      Visitor.visit(Decl->getType().getTypeExpr());

      assert(!VariadicParams.empty() && "undiagnosed invalid pack expansion");

      if (VariadicParams.size() > 1) {
         llvm_unreachable("FIXME error message");
      }

      TemplateParamDecl *ParameterPack = *VariadicParams.begin();

      auto GivenArg = hasTemplateArg(ParameterPack);
      if (!GivenArg) {
         // current context is still templated
         visit(Decl);
         return false;
      }

      assert(GivenArg->isVariadic() && "invalid passed template argument");

      unsigned numVariadics = (unsigned)GivenArg->getVariadicArgs().size();
      for (unsigned i = 0; i < numVariadics; ++i) {
         DeclarationName DN =
            Context.getDeclNameTable()
                   .getPackExpansionName(Decl->getDeclName(), i);

         SubstContextRAII raii(*this, ParameterPack,
                               Decl->getIdentifierInfo(), DN,
                               &GivenArg->getVariadicArgs()[i]);

         auto Inst = visit(Decl);
         if (!Inst)
            continue;

         auto Var = cast<VarDecl>(Inst);
         if (auto Ty = Var->getType().getTypeExpr()) {
            assert(Var->getType().getTypeExpr() != Decl->getType().getTypeExpr()
                   && "didn't instantiate dependent type expression!");

            Ty->setEllipsisLoc(SourceLocation());
         }
      }

      return true;
   }

   bool expandVariadicDecl(FuncArgDecl *Decl,
                           llvm::SmallVectorImpl<FuncArgDecl*> &Vec) {
      llvm::SmallPtrSet<TemplateParamDecl*, 4> VariadicParams;
      llvm::SmallPtrSet<IdentifierRefExpr*, 4> PackArguments;

      VariadicParamFinder Visitor(VariadicParams, PackArguments);
      Visitor.visit(Decl->getType().getTypeExpr());

      assert(!VariadicParams.empty() && "undiagnosed invalid pack expansion");

      if (VariadicParams.size() > 1) {
         llvm_unreachable("FIXME error message");
      }

      TemplateParamDecl *ParameterPack = *VariadicParams.begin();

      auto GivenArg = hasTemplateArg(ParameterPack);
      if (!GivenArg) {
         // current context is still templated
         auto Inst = visit(Decl);
         if (Inst)
            Vec.push_back(cast<FuncArgDecl>(Inst));

         return false;
      }

      assert(GivenArg->isVariadic() && "invalid passed template argument");

      unsigned numVariadics = (unsigned)GivenArg->getVariadicArgs().size();
      for (unsigned i = 0; i < numVariadics; ++i) {
         DeclarationName DN =
            Context.getDeclNameTable()
                   .getPackExpansionName(Decl->getDeclName(), i);

         SubstContextRAII raii(*this, ParameterPack,
                               Decl->getIdentifierInfo(), DN,
                               &GivenArg->getVariadicArgs()[i]);

         auto Inst = visit(Decl);
         if (!Inst)
            continue;

         auto Var = cast<FuncArgDecl>(Inst);
         if (auto Ty = Var->getType().getTypeExpr()) {
            assert(Var->getType().getTypeExpr() != Decl->getType().getTypeExpr()
                   && "didn't instantiate dependent type expression!");

            Ty->setEllipsisLoc(SourceLocation());
         }

         Vec.push_back(Var);
      }

      return true;
   }
};

TemplateParamDecl*
InstantiatorImpl::visitTemplateParamDecl(TemplateParamDecl *P)
{
   TemplateParamDecl *Inst;
   if (P->isTypeName()) {
      Inst = TemplateParamDecl::Create(Context, P->getDeclName(),
                                       visitOrAuto(P->getCovariance()),
                                       visitOrAuto(P->getContravariance()),
                                       copyOrNull(P->getDefaultValue()),
                                       P->getIndex(),
                                       P->getTypeNameOrValueLoc(),
                                       P->getNameLoc(), P->getEllipsisLoc(),
                                       P->isUnbounded());
   }
   else {
      Inst = TemplateParamDecl::Create(Context, P->getDeclName(),
                                       visitOrAuto(P->getCovariance()),
                                       copyOrNull(P->getDefaultValue()),
                                       P->getIndex(),
                                       P->getTypeNameOrValueLoc(),
                                       P->getNameLoc(), P->getEllipsisLoc(),
                                       P->isUnbounded());
   }

   return Inst;
}

RecordDecl *InstantiatorImpl::instantiateRecordDecl(RecordDecl *Decl)
{
   AccessSpecifier access = Decl->getAccess();
   DeclarationName Name = Decl->getDeclName();

   if (Decl == Template)
      Name = Context.getDeclNameTable()
                    .getInstantiationName(Decl->getDeclName(),
                                          templateArgs.innermost());

   auto conformances = copyConformances(Decl->getConformanceTypes());
   auto constraints = cloneVector(Decl->getConstraints());
   auto templateParmams = copyTemplateParameters(Decl);

   SourceType enumRawTypeOrExtends;
   bool isAbstract = false;

   if (auto E = dyn_cast<EnumDecl>(Decl)) {
      enumRawTypeOrExtends = visit(E->getRawType());
   }
   else if (auto C = dyn_cast<ClassDecl>(Decl)) {
      isAbstract = C->isAbstract();
      enumRawTypeOrExtends = visit(C->getParentType());
   }

   RecordDecl *Inst;
   switch (Decl->getKind()) {
   case Decl::StructDeclID:
      Inst = StructDecl::Create(Context, access, Decl->getKeywordLoc(),
                                Name, move(conformances),
                                move(templateParmams));
      break;
   case Decl::ClassDeclID:
      Inst = ClassDecl::Create(Context, access, Decl->getKeywordLoc(),
                               Name, move(conformances), move(templateParmams),
                               enumRawTypeOrExtends, isAbstract);
      break;
   case Decl::EnumDeclID:
      Inst = EnumDecl::Create(Context, access, Decl->getKeywordLoc(),
                              Name, move(conformances), move(templateParmams),
                              enumRawTypeOrExtends);
      break;
   case Decl::UnionDeclID:
      Inst = UnionDecl::Create(Context, access, Decl->getKeywordLoc(),
                               Name, move(conformances), move(templateParmams));
      break;
   case Decl::ProtocolDeclID:
      Inst = ProtocolDecl::Create(Context, access, Decl->getKeywordLoc(), Name,
                                  move(conformances), move(templateParmams));
      break;
   default:
      llvm_unreachable("not a record decl!");
   }

   // make sure to set the parent context before calling this
   Inst->setParentCtx(&SP.getCompilationUnit().getGlobalDeclCtx());
   if (auto MF = Decl->getModFile())
      Inst->setModFile(MF->copy());

   Inst->setLastMethodID(Decl->getLastMethodID());

   Context.setConstraints(Inst, constraints);
   return Inst;
}

RecordDecl *InstantiatorImpl::instantiateRecordDefinition(RecordDecl *Template,
                                                          RecordDecl *Inst) {
   InstantiatingRecordRAII IRR(*this, Inst);
   {
      SemaPass::DeclContextRAII declContext(SP, Inst,
                                            SemaPass::SetParentContext);

      // resolve static conditions line because they might contain decls that
      // are needed by others
      for (auto *D : Template->getDecls()) {
         if (D->isSynthesized())
            continue;

         switch (D->getKind()) {
         case Decl::StaticIfDeclID:
         case Decl::StaticForDeclID: {
            (void)visit(D);
            break;
         }
         case Decl::MacroExpansionDeclID:
         case Decl::TemplateParamDeclID:
         default:
            break;
         }
      }

      // associated types and aliases must be visited line because their types
      // might be used in fields / method signatures / etc.
      for (auto *D : Template->getDecls()) {
         if (D->isSynthesized())
            continue;

         switch (D->getKind()) {
         case Decl::TypedefDeclID:
         case Decl::AssociatedTypeDeclID:
         case Decl::AliasDeclID:
         case Decl::UsingDeclID:
         case Decl::ImportDeclID:
         case Decl::ModuleDeclID: {
            (void)visit(D);
            break;
         }
         case Decl::TemplateParamDeclID:
         default:
            break;
         }
      }

      // Now we can instantiate the rest
      for (auto &D : Template->getDecls()) {
         if (D->isSynthesized())
            continue;

         switch (D->getKind()) {
         case Decl::TypedefDeclID:
         case Decl::AssociatedTypeDeclID:
         case Decl::AliasDeclID:
         case Decl::TemplateParamDeclID:
         case Decl::UsingDeclID:
         case Decl::ImportDeclID:
         case Decl::ModuleDeclID:
         case Decl::OperatorDeclID:
         case Decl::PrecedenceGroupDeclID:
         case Decl::StaticIfDeclID:
         case Decl::StaticForDeclID:
         case Decl::MacroExpansionDeclID:
            break;
         default: {
            (void)visit(D);
            break;
         }
         }
      }
   }

   for (auto &E : Template->getExtensions()) {
      auto ExtInst = visitExtensionDecl(E, Inst);
      if (!ExtInst)
         continue;

      SP.registerExtension(ExtInst);
   }

   return Inst;
}

RecordDecl *InstantiatorImpl::visitRecordCommon(RecordDecl *Template)
{
   if (auto *MF = Template->getModFile()) {
      MF->LoadAllDecls(*Template);
   }

   auto Inst = instantiateRecordDecl(Template);

   auto R = instantiateRecordDefinition(Template, Inst);
   if (R) {
      ActOnDecl(R);
   }

   return R;
}

ClassDecl* InstantiatorImpl::visitClassDecl(ClassDecl *node)
{
   return cast<ClassDecl>(visitRecordCommon(node));
}

StructDecl* InstantiatorImpl::visitStructDecl(StructDecl *node)
{
   return cast<StructDecl>(visitRecordCommon(node));
}

EnumDecl* InstantiatorImpl::visitEnumDecl(EnumDecl *node)
{
   return cast<EnumDecl>(visitRecordCommon(node));
}

EnumCaseDecl* InstantiatorImpl::visitEnumCaseDecl(EnumCaseDecl *node)
{
   auto EC = EnumCaseDecl::Create(Context, node->getAccess(),
                                  node->getCaseLoc(), node->getIdentLoc(),
                                  node->getDeclName(),
                                  copyOrNull(node->getRawValExpr()),
                                  cloneVector(node->getArgs()));

   EC->setILValue(node->getILValue());

   ActOnDecl(EC);
   return EC;
}

UnionDecl* InstantiatorImpl::visitUnionDecl(UnionDecl *node)
{
   return cast<UnionDecl>(visitRecordCommon(node));
}

ProtocolDecl* InstantiatorImpl::visitProtocolDecl(ProtocolDecl *node)
{
   return cast<ProtocolDecl>(visitRecordCommon(node));
}

ExtensionDecl*
InstantiatorImpl::visitExtensionDecl(ExtensionDecl *Ext, RecordDecl *RecInst)
{
   if (!SP.ensureDeclared(Ext))
      return nullptr;

   assert(RecInst && "instantiating extension without instantiated record!");

   auto constraints = cloneVector(Ext->getConstraints());

   auto Result = SP.checkConstraints(RecInst, Ext, constraints, RecInst);
   if (Result.getFailedConstraint()) {
      return nullptr;
   }

   // Make sure all declarations are visible.
   if (auto *MF = Ext->getModFile())
      MF->LoadAllDecls(*Ext);

   AccessSpecifier access = Ext->getAccess();
   auto conformances = copyConformances(Ext->getConformanceTypes());

   auto Inst = ExtensionDecl::Create(Context, access, Ext->getExtLoc(),
                                     RecInst, conformances);

   Inst->setName(Context.getDeclNameTable()
                        .getExtensionName(Context.getRecordType(RecInst)));

   RecInst->addExtension(Inst);

   {
      SemaPass::DeclContextRAII declContext(SP, Inst,
                                            SemaPass::SetParentContext);

      for (const auto &namedDecl : Ext->getDecls()) {
         if (isa<TemplateParamDecl>(namedDecl))
            continue;

         if (namedDecl->isSynthesized())
            continue;

         (void) visit(namedDecl);
      }
   }

   Context.setConstraints(Inst, constraints);
   Context.registerInstantiation(Ext, Inst);

   return Inst;
}

PropDecl* InstantiatorImpl::visitPropDecl(PropDecl *Decl, bool IsFieldAccessor)
{
   if (Decl->isSynthesized() && !IsFieldAccessor)
      return nullptr;

   auto Prop = PropDecl::Create(Context, Decl->getAccess(),
                                Decl->getSourceRange(), Decl->getDeclName(),
                                visit(Decl->getType()), Decl->isStatic(),
                                copyOrNull(Decl->getGetterMethod()),
                                copyOrNull(Decl->getSetterMethod()));

   Prop->setPropTemplate(Decl);

   if (!IsFieldAccessor)
      ActOnDecl(Prop);

   return Prop;
}

SubscriptDecl* InstantiatorImpl::visitSubscriptDecl(SubscriptDecl *Decl)
{
   auto SD = SubscriptDecl::Create(Context, Decl->getAccess(),
                                   Decl->getSourceRange(),
                                   visit(Decl->getType()),
                                   copyOrNull(Decl->getGetterMethod()),
                                   copyOrNull(Decl->getSetterMethod()));

   SD->setTemplate(Decl);
   ActOnDecl(SD);

   return SD;
}

MethodDecl* InstantiatorImpl::visitMethodDecl(MethodDecl *M)
{
   bool IsMainTemplate = M == Template;
   bool SkipBody = !IsMainTemplate;

   DeclarationName Name = M->getDeclName();
   if (IsMainTemplate)
      Name = Context.getDeclNameTable()
                    .getInstantiationName(M->getDeclName(),
                                          templateArgs.innermost());

   SmallVector<FuncArgDecl*, 4> Args;
   copyArgListAndFindVariadic(M, Args);

   auto templateParams = copyTemplateParameters(M);

   MethodDecl *Inst;
   if (M->isConversionOp()) {
      Inst = MethodDecl::CreateConversionOp(Context, M->getAccess(),
                                            M->getDefLoc(),
                                            visit(M->getReturnType()),
                                            Args, move(templateParams),
                                            nullptr);
   }
   else {
      Inst = MethodDecl::Create(Context,  M->getAccess(), M->getDefLoc(), Name,
                                visit(M->getReturnType()), Args,
                                move(templateParams), nullptr, M->isStatic());
   }

   if (SkipBody) {
      Inst->setBodyTemplate(M);
   }
   else {
      Statement *Body = M->getBody() ? M->getBody()
                                     : M->getBodyTemplate()->getBody();

      SemaPass::DeclContextRAII declContext(SP, Inst,
                                            SemaPass::SetParentContext);

      Inst->setBody(copyOrNull(Body));
   }

   Inst->setMethodID(M->getMethodID());
   Inst->setFunctionFlags(M->getFunctionFlags());

   VariadicTemplateArgs.clear();

   if (!IsMainTemplate && !Inst->isProperty() && !Inst->isSubscript()) {
      ActOnDecl(Inst);

      auto *Template = M->getRecord();
      auto *RecInst = cast<RecordDecl>(
         SP.getDeclContext().lookThroughExtension());

      if (M == Template->getCopyFn()) {
         RecInst->setCopyFn(Inst);
      }
      if (M == Template->getToStringFn()) {
         RecInst->setToStringFn(Inst);
      }
      if (M == Template->getOperatorEquals()) {
         RecInst->setOperatorEquals(Inst);
      }
      if (M == Template->getHashCodeFn()) {
         RecInst->setHashCodeFn(Inst);
      }
   }

   return Inst;
}

AssociatedTypeDecl*
InstantiatorImpl::visitAssociatedTypeDecl(AssociatedTypeDecl *node)
{
   SourceType ActualType;
   if (node->getDeclName().isStr("Self") && node->isImplementation()) {
      ActualType = Context.getRecordType(InstantiatingRecord);
   }
   else {
      ActualType = visit(node->getActualType());
   }

   auto Inst = AssociatedTypeDecl::Create(Context, node->getSourceLoc(),
                                          node->getProtoSpecInfo(),
                                          node->getDeclName(),
                                          ActualType, node->isImplementation());

   Context.setConstraints(Inst, cloneVector(node->getConstraints()));
   ActOnDecl(Inst);

   return Inst;
}

TypedefDecl* InstantiatorImpl::visitTypedefDecl(TypedefDecl *TD)
{
   bool IsMainTemplate = TD == Template;
   DeclarationName Name = TD->getDeclName();

   auto Inst = TypedefDecl::Create(Context, TD->getAccess(),
                                   TD->getSourceLoc(), Name,
                                   visit(TD->getOriginTy()),
                                   copyTemplateParameters(TD));

   if (!IsMainTemplate)
      ActOnDecl(Inst);

   return Inst;
}

AliasDecl* InstantiatorImpl::visitAliasDecl(AliasDecl *Alias)
{
   SP.ensureVisited(Alias);

   if (Alias->isInvalid())
      return nullptr;

   bool IsMainTemplate = Alias == Template;
   DeclarationName Name = Alias->getDeclName();
   if (IsMainTemplate)
      Name = Context.getDeclNameTable()
                    .getInstantiationName(Alias->getDeclName(),
                                          templateArgs.innermost());

   auto Inst = AliasDecl::Create(Context, Alias->getSourceLoc(),
                                 Alias->getAccess(), Name,
                                 visitOrAuto(Alias->getType()),
                                 copyOrNull(Alias->getAliasExpr()),
                                 copyTemplateParameters(Alias));

   Context.setConstraints(Inst, cloneVector(Alias->getConstraints()));

   if (!IsMainTemplate)
      ActOnDecl(Inst);

   return Inst;
}

FieldDecl* InstantiatorImpl::visitFieldDecl(FieldDecl *Decl)
{
   // Make sure the default value was visited.
   SP.ensureVisited(Decl);

   DeclarationName DeclName = Decl->getDeclName();
   if (auto Subst = getVariadicArgumentSubst(Decl->getIdentifierInfo())) {
      DeclName = Subst;
   }
   else if (Decl->isVariadic()) {
      if (expandVariadicDecl(Decl))
         return nullptr;

      // still in a dependent context, instantiate normally
   }

   auto Inst = FieldDecl::Create(Context, Decl->getAccess(),
                                 Decl->getVarOrLetLoc(), Decl->getColonLoc(),
                                 DeclName, visit(Decl->getType()),
                                 Decl->isStatic(), Decl->isConst(),
                                 copyOrNull(Decl->getDefaultVal()));

   if (auto Acc = Decl->getAccessor()) {
      PropDecl *AccInst = visitPropDecl(Acc, true);

      AccInst->setSynthesized(true);
      Inst->setAccessor(AccInst);
   }

   ActOnDecl(Inst);
   return Inst;
}

InitDecl* InstantiatorImpl::visitInitDecl(InitDecl *Decl)
{
   bool IsMainTemplate = Decl == Template;
   bool SkipBody = !IsMainTemplate;

   RecordDecl *R = InstantiatingRecord;
   if (!R) {
      R = Decl->getRecord();
   }

   DeclarationName Name = Context.getDeclNameTable()
                                 .getConstructorName(Context.getRecordType(R));

   if (IsMainTemplate)
      Name = Context.getDeclNameTable()
                    .getInstantiationName(Name, templateArgs.innermost());

   SmallVector<FuncArgDecl*, 4> Args;
   copyArgListAndFindVariadic(Decl, Args);

   auto templateParams = copyTemplateParameters(Decl);
   auto Inst = InitDecl::Create(Context, Decl->getAccess(),
                                Decl->getSourceLoc(), Args,
                                move(templateParams), nullptr,
                                Name, Decl->isFallible());

   if (SkipBody) {
      // An explicit memberwise initializer does not have a body.
      if (Decl->getBody() || Decl->getLazyFnInfo()) {
         Inst->setBodyTemplate(Decl);
      }
   }
   else {
      Statement *Body = Decl->getBody() ? Decl->getBody()
                                        : Decl->getBodyTemplate()->getBody();

      SemaPass::DeclContextRAII declContext(SP, Inst,
                                            SemaPass::SetParentContext);

      Inst->setBody(copyOrNull(Body));
   }

   Inst->setMethodID(Decl->getMethodID());
   Inst->setFunctionFlags(Decl->getFunctionFlags());

   if (!IsMainTemplate) {
      ActOnDecl(Inst);

      auto *Template = cast<RecordDecl>(
         SP.getDeclContext().lookThroughExtension());

      if (auto *S = dyn_cast<StructDecl>(Template)) {
         if (Decl == S->getMemberwiseInitializer()) {
            Inst->setMemberwiseInitializer(true);
            S->setMemberwiseInitializer(Inst);
         }
      }
   }

   return Inst;
}

DeinitDecl* InstantiatorImpl::visitDeinitDecl(DeinitDecl *Decl)
{
   DeclarationName Name =
      Context.getDeclNameTable()
             .getDestructorName(
                Context.getRecordType(cast<RecordDecl>(&SP.getDeclContext())));

   auto Inst = DeinitDecl::Create(Context, Decl->getSourceLoc(),
                                  nullptr, cloneVector(Decl->getArgs()), Name);

   Inst->setMethodID(Decl->getMethodID());
   Inst->setBodyTemplate(Decl);
   Inst->setFunctionFlags(Decl->getFunctionFlags());

   ActOnDecl(Inst);
   return Inst;
}

FunctionDecl* InstantiatorImpl::visitFunctionDecl(FunctionDecl *F)
{
   bool IsMainTemplate = F == Template;

   DeclarationName Name = F->getDeclName();
   if (IsMainTemplate)
      Name = Context.getDeclNameTable()
                    .getInstantiationName(Name, templateArgs.innermost());

   llvm::SmallVector<FuncArgDecl*, 4> Args;
   copyArgListAndFindVariadic(F, Args);

   auto templateParams = copyTemplateParameters(F);
   auto Inst = FunctionDecl::Create(Context, F->getAccess(), F->getDefLoc(),
                                    Name, Args, visit(F->getReturnType()),
                                    nullptr, move(templateParams));

   {
      SemaPass::DeclContextRAII declContext(SP, Inst,
                                            SemaPass::SetParentContext);

      Inst->setBody(copyOrNull(F->getBody()));
   }

   Inst->setFunctionFlags(F->getFunctionFlags());
   VariadicTemplateArgs.clear();

   if (!IsMainTemplate)
      ActOnDecl(Inst);

   return Inst;
}

AttributedStmt *InstantiatorImpl::visitAttributedStmt(AttributedStmt *S)
{
   llvm::SmallVector<Attr*, 4> Attrs;
   for (auto &A : S->getAttributes())
      Attrs.push_back(A->clone(Context));

   return AttributedStmt::Create(Context, visit(S->getStatement()),
                                 Attrs);
}

AttributedExpr *InstantiatorImpl::visitAttributedExpr(AttributedExpr *E)
{
   llvm::SmallVector<Attr*, 4> Attrs;
   for (auto &A : E->getAttributes())
      Attrs.push_back(A->clone(Context));

   return AttributedExpr::Create(Context, visit(E->getExpr()), Attrs);
}

TupleTypeExpr *InstantiatorImpl::visitTupleTypeExpr(TupleTypeExpr *Expr)
{
   return TupleTypeExpr::Create(Context, Expr->getSourceRange(),
                                copyTypeList(Expr->getContainedTypes()),
                                Expr->isMeta());
}

FunctionTypeExpr*
InstantiatorImpl::visitFunctionTypeExpr(FunctionTypeExpr *Expr)
{
   SmallVector<SourceType, 4> Tys;
   SmallVector<FunctionType::ParamInfo, 4> Inf;

   copyParameterList(Expr->getArgTypes(), Expr->getParamInfo(), Tys, Inf);

   return FunctionTypeExpr::Create(Context, Expr->getSourceRange(),
                                   visit(Expr->getReturnType()),
                                   Tys, Inf, Expr->isMeta(), Expr->isThin(),
                                   Expr->throws(), Expr->isAsync(),
                                   Expr->isUnsafe());
}

ArrayTypeExpr *InstantiatorImpl::visitArrayTypeExpr(ArrayTypeExpr *Expr)
{
   return ArrayTypeExpr::Create(Context, Expr->getSourceRange(),
                                visit(Expr->getElementTy()),
                                clone<StaticExpr>(Expr->getSizeExpr()),
                                Expr->isMeta());
}

DeclTypeExpr *InstantiatorImpl::visitDeclTypeExpr(DeclTypeExpr *Expr)
{
   return DeclTypeExpr::Create(Context, Expr->getSourceRange(),
                               visit(Expr->getTyExpr()),
                               Expr->isMeta());
}

PointerTypeExpr *InstantiatorImpl::visitPointerTypeExpr(PointerTypeExpr *Expr)
{
   return PointerTypeExpr::Create(Context, Expr->getSourceRange(),
                                  visit(Expr->getSubType()),
                                  Expr->isMeta());
}

ReferenceTypeExpr*
InstantiatorImpl::visitReferenceTypeExpr(ReferenceTypeExpr *Expr)
{
   return ReferenceTypeExpr::Create(Context, Expr->getSourceRange(),
                                    visit(Expr->getSubType()),
                                    Expr->isMeta());
}

OptionTypeExpr *InstantiatorImpl::visitOptionTypeExpr(OptionTypeExpr *Expr)
{
   return OptionTypeExpr::Create(Context, Expr->getSourceRange(),
                                 visit(Expr->getSubType()),
                                 Expr->isMeta());
}

Expression* InstantiatorImpl::visitImplicitCastExpr(ImplicitCastExpr *node)
{
   return visit(node->getTarget());
//   return ImplicitCastExpr::Create(Context, visit(node->getTarget()),
//                                   node->getConvSeq().copy());
}

CompoundDecl* InstantiatorImpl::visitCompoundDecl(CompoundDecl *D)
{
   auto Inst = CompoundDecl::Create(Context, D->getLBraceLoc(),
                                    D->isTransparent());

   SemaPass::DeclContextRAII declContextRAII(SP, Inst,
                                             SemaPass::SetParentContext);

   for (auto &decl : D->getDecls())
      visit(decl);

   Inst->setRBraceLoc(D->getRBraceLoc());
   return Inst;
}

CompoundStmt* InstantiatorImpl::visitCompoundStmt(CompoundStmt *node)
{
   SmallVector<Statement*, 8> Stmts;
   for (auto &Stmt : node->getStatements())
      Stmts.push_back(visit(Stmt));

   auto *CS = CompoundStmt::Create(Context, Stmts, node->preservesScope(),
                                   node->getLBraceLoc(), node->getRBraceLoc());

   CS->setScopeID(node->getScopeID());
   CS->setContainsDeclStmt(node->containsDeclStmt());

   return CS;
}

void InstantiatorImpl::copyArgListAndFindVariadic(
                                 CallableDecl *C,
                                 SmallVectorImpl<FuncArgDecl*> &Variadics){
   auto argList = C->getArgs();
   for (auto arg : argList) {
      if (!arg->isVariadicArgPackExpansion()) {
         Variadics.push_back(clone(arg));
      }
      else {
         expandVariadicDecl(arg, Variadics);
      }
   }
}

template<class Container, class Expr> std::vector<Expr*>
InstantiatorImpl::copyExprList(const Container &exprList)
{
   std::vector<Expr*> list;
   for (const auto &expr : exprList)
      expandVariadicArgument(list, expr);

   return list;
}

void
InstantiatorImpl::copyArgumentList(ArrayRef<Expression *> Args,
                                   ArrayRef<cdot::IdentifierInfo *> Labels,
                                   SmallVectorImpl<Expression *> &ResultArgs,
                                   SmallVectorImpl<IdentifierInfo*> &ResultLabels) {
   unsigned i = 0;
   for (const auto &expr : Args) {
      IdentifierInfo *Lbl = nullptr;
      if (i < Labels.size())
         Lbl = Labels[i];

      unsigned PrevSize = (unsigned)ResultArgs.size();
      expandVariadicArgument(ResultArgs, expr);

      unsigned AddedArgs = (unsigned)ResultArgs.size() - PrevSize;
      while (AddedArgs--)
         ResultLabels.push_back(Lbl);

      ++i;
   }

   while (!ResultLabels.empty() && !ResultLabels.back()) {
      ResultLabels.pop_back();
   }
}

void
InstantiatorImpl::copyParameterList(ArrayRef<SourceType> Args,
                                    ArrayRef<FunctionType::ParamInfo> ParamInfo,
                                    SmallVectorImpl<SourceType> &ResultArgs,
                                    SmallVectorImpl<FunctionType::ParamInfo> &ResultInfo) {
   SmallVector<Expression*, 4> Exprs;

   unsigned i = 0;
   for (const auto &Ty : Args) {
      FunctionType::ParamInfo PI;
      if (i < ParamInfo.size())
         PI = ParamInfo[i];

      unsigned PrevSize = (unsigned)ResultArgs.size();

      if (Ty.getTypeExpr() && !canElideInstantiation(Ty.getTypeExpr())) {
         expandVariadicArgument(Exprs, Ty.getTypeExpr());
         for (auto &E : Exprs)
            ResultArgs.emplace_back(E);

         Exprs.clear();
      }
      else {
         ResultArgs.push_back(Ty);
      }

      unsigned AddedArgs = (unsigned)ResultArgs.size() - PrevSize;
      while (AddedArgs--)
         ResultInfo.push_back(PI);

      ++i;
   }
}

std::vector<SourceType>
InstantiatorImpl::copyTypeList(llvm::ArrayRef<SourceType> Tys)
{
   SmallVector<Expression*, 4> Exprs;
   std::vector<SourceType> list;

   for (const auto &Ty : Tys) {
      if (Ty.getTypeExpr() && !canElideInstantiation(Ty.getTypeExpr())) {
         expandVariadicArgument(Exprs, Ty.getTypeExpr());
         for (auto &E : Exprs)
            list.emplace_back(E);

         Exprs.clear();
      }
      else {
         list.push_back(Ty);
      }
   }

   return list;
}

ASTVector<SourceType>
InstantiatorImpl::copyConformances(llvm::ArrayRef<SourceType> Tys)
{
   llvm::SmallVector<Expression*, 4> Exprs;
   ASTVector<SourceType> list(Context, Tys.size());

   for (const auto &Ty : Tys) {
      if (Ty.getTypeExpr() && !canElideInstantiation(Ty.getTypeExpr())) {
         expandVariadicArgument(Exprs, Ty.getTypeExpr());
         for (auto &E : Exprs)
            list.push_back(SourceType(E), Context);

         Exprs.clear();
      }
      else {
         list.push_back(Ty, Context);
      }
   }

   return list;
}

FuncArgDecl*
InstantiatorImpl::visitFuncArgDecl(FuncArgDecl *Decl)
{
   bool IsSubstitution = false;
   DeclarationName DeclName = Decl->getDeclName();
   if (auto Subst = getVariadicArgumentSubst(Decl->getIdentifierInfo())) {
      DeclName = Subst;
      IsSubstitution = true;
   }
   else {
      assert(!Decl->isVariadic() && "should be handled specially!");
   }

   auto *Inst = FuncArgDecl::Create(Context, Decl->getOwnershipLoc(),
                              Decl->getColonLoc(), DeclName, Decl->getLabel(),
                              Decl->getConvention(), visit(Decl->getType()),
                              copyOrNull(Decl->getDefaultVal()),
                              Decl->isVariadicArgPackExpansion()
                                  && !IsSubstitution,
                              Decl->isCstyleVararg(), Decl->isSelf());

   return Inst;
}

IntegerLiteral *InstantiatorImpl::visitIntegerLiteral(IntegerLiteral *node)
{
   return IntegerLiteral::Create(Context, node->getSourceRange(),
                                 node->getType(),
                                 node->getValue(),
                                 node->getSuffix());
}

FPLiteral *InstantiatorImpl::visitFPLiteral(FPLiteral *node)
{
   return FPLiteral::Create(Context, node->getSourceRange(),
                            node->getType(), node->getValue(),
                            node->getSuffix());
}

BoolLiteral *InstantiatorImpl::visitBoolLiteral(BoolLiteral *node)
{
   return BoolLiteral::Create(Context, node->getSourceLoc(), node->getType(),
                              node->getValue());
}

CharLiteral *InstantiatorImpl::visitCharLiteral(CharLiteral *node)
{
   if (node->isWide()) {
      return CharLiteral::Create(Context, node->getSourceRange(),
                                 node->getType(), node->getWide());
   }
   else {
      return CharLiteral::Create(Context, node->getSourceRange(),
                                 node->getType(), node->getNarrow());
   }
}

SelfExpr* InstantiatorImpl::visitSelfExpr(SelfExpr *node)
{
   return SelfExpr::Create(Context, node->getSourceLoc(), node->isUppercase());
}

SuperExpr* InstantiatorImpl::visitSuperExpr(SuperExpr *node)
{
   return SuperExpr::Create(Context, node->getSourceLoc());
}

DebugStmt* InstantiatorImpl::visitDebugStmt(DebugStmt *node)
{
   if (!node->isUnreachable()) {
      int i = 3;
      (void) i;
   }

   return new (Context) DebugStmt(node->getSourceLoc(),
                                  node->isUnreachable());
}

DebugDecl *InstantiatorImpl::visitDebugDecl(DebugDecl *D)
{
   return DebugDecl::Create(Context, D->getLoc());
}

NoneLiteral* InstantiatorImpl::visitNoneLiteral(NoneLiteral *node)
{
   return NoneLiteral::Create(Context, node->getSourceLoc());
}

StringInterpolation*
InstantiatorImpl::visitStringInterpolation(StringInterpolation *node)
{
   return StringInterpolation::Create(Context, node->getSourceRange(),
                                      cloneVector(node->getSegments()));
}

StringLiteral *InstantiatorImpl::visitStringLiteral(StringLiteral *node)
{
   return StringLiteral::Create(Context, node->getSourceRange(),
                                node->getValue().str());
}

TupleLiteral* InstantiatorImpl::visitTupleLiteral(TupleLiteral *node)
{
   return TupleLiteral::Create(Context, node->getSourceRange(),
                               copyExprList(node->getElements()));
}

LocalVarDecl* InstantiatorImpl::visitLocalVarDecl(LocalVarDecl *node)
{
   auto Decl = LocalVarDecl::Create(Context, node->getAccess(),
                                    node->getVarOrLetLoc(), node->getColonLoc(),
                                    node->isConst(), node->getDeclName(),
                                    visitOrAuto(node->getType()),
                                    copyOrNull(node->getValue()));

   Decl->setLexicalContext(&SP.getDeclContext());
   return Decl;
}

GlobalVarDecl* InstantiatorImpl::visitGlobalVarDecl(GlobalVarDecl *node)
{
   auto Decl = GlobalVarDecl::Create(Context, node->getAccess(),
                                     node->getVarOrLetLoc(),
                                     node->getColonLoc(),
                                     node->isConst(), node->getDeclName(),
                                     visitOrAuto(node->getType()),
                                     copyOrNull(node->getValue()));

   ActOnDecl(Decl);
   return Decl;
}

DestructuringDecl*
InstantiatorImpl::visitDestructuringDecl(DestructuringDecl *Decl)
{
   auto *Inst = DestructuringDecl::Create(Context, Decl->getParenRange(),
                                          cloneVector(Decl->getDecls()),
                                          visit(Decl->getType()),
                                          visit(Decl->getValue()));

   ActOnDecl(Inst);
   return Inst;
}

Expression* InstantiatorImpl::visitBuiltinExpr(BuiltinExpr *node)
{
   return BuiltinExpr::Create(Context, node->getExprType());
}

Expression* InstantiatorImpl::visitParenExpr(ParenExpr *Expr)
{
   // variadic tuple
   if (auto ExprSeq = dyn_cast<ExprSequence>(Expr->getParenthesizedExpr())) {
      if (ExprSeq->getFragments().size() == 1
          && ExprSeq->getFragments().front().getExpr()
                    ->isVariadicArgPackExpansion()) {
         llvm::SmallVector<Expression*, 4> tupleElements;
         expandVariadicArgument(tupleElements,
                                ExprSeq->getFragments().front().getExpr());

         return TupleLiteral::Create(Context, Expr->getSourceRange(),
                                     tupleElements);
      }
   }

   return ParenExpr::Create(Context, Expr->getSourceRange(),
                            visit(Expr->getParenthesizedExpr()));
}

Expression* InstantiatorImpl::makeLiteralExpr(Expression *Expr,
                                              QualType valTy,
                                              const Variant &Val) {
   Expression* literal;
   if (valTy->isIntegerType()) {
      switch (valTy->getBitwidth()) {
      case 1:
         literal = BoolLiteral::Create(Context, Expr->getSourceLoc(),
                                       getContext().getBoolTy(),
                                       Val.getZExtValue() != 0);

         break;
      case 8:
         literal = CharLiteral::Create(Context, Expr->getSourceRange(),
                                       getContext().getCharTy(), Val.getChar());

         break;
      default: {
         literal = IntegerLiteral::Create(Context, Expr->getSourceRange(),
                                          valTy, Val.getAPSInt());
         break;
      }
      }
   }
   else if (valTy->isFPType()) {
      literal = FPLiteral::Create(Context, Expr->getSourceRange(), valTy,
                                  Val.getAPFloat());
   }
   else if (valTy->isRecordType() && valTy->getRecord() == SP.getStringDecl()) {
      literal = StringLiteral::Create(Context, Expr->getSourceRange(),
                                      string(Val.getString()));
   }
   else {
      llvm_unreachable("bad variant kind!");
   }

   return literal;
}

NestedNameSpecifier*
InstantiatorImpl::visitNestedNameSpecifier(NestedNameSpecifier *Name)
{
   if (!Name)
      return nullptr;

   switch (Name->getKind()) {
   case NestedNameSpecifier::Type: {
      SourceType ST(Name->getType());
      visit(ST);

      return NestedNameSpecifier::Create(
         Context.getDeclNameTable(), ST ? (QualType)ST : Name->getType(),
         visitNestedNameSpecifier(Name->getPrevious()));
   }
   case NestedNameSpecifier::Identifier:
      return NestedNameSpecifier::Create(
         Context.getDeclNameTable(), Name->getIdentifier(),
         visitNestedNameSpecifier(Name->getPrevious()));
   case NestedNameSpecifier::Namespace:
      return NestedNameSpecifier::Create(
         Context.getDeclNameTable(), Name->getNamespace(),
         visitNestedNameSpecifier(Name->getPrevious()));
   case NestedNameSpecifier::TemplateParam: {
      auto *Arg = hasTemplateArg(Name->getParam()->getDeclName());
      if (Arg && Arg->isType() && !Arg->isVariadic()) {
         return NestedNameSpecifier::Create(Context.getDeclNameTable(),
                                            Arg->getType(),
                                            visitNestedNameSpecifier(
                                               Name->getPrevious()));
      }

      return NestedNameSpecifier::Create(Context.getDeclNameTable(),
                                         Name->getParam(),
                                         visitNestedNameSpecifier(
                                            Name->getPrevious()));
   }
   case NestedNameSpecifier::AssociatedType: {
      return NestedNameSpecifier::Create(Context.getDeclNameTable(),
                                         Name->getAssociatedType(),
                                         visitNestedNameSpecifier(
                                            Name->getPrevious()));
   }
   case NestedNameSpecifier::Module:
      return NestedNameSpecifier::Create(
         Context.getDeclNameTable(), Name->getModule(),
         visitNestedNameSpecifier(Name->getPrevious()));
   }
}

Expression* InstantiatorImpl::visitIdentifierRefExpr(IdentifierRefExpr *Ident)
{
   if (!Ident->getParentExpr()) {
      if (auto Arg = hasTemplateArg(Ident->getDeclName())) {
         if (Arg->isVariadic()) {
            auto Subst = getParameterSubst(Ident->getDeclName());

            assert(Subst && "didn't diagnose unexpanded parameter pack!");

            return new(Context) IdentifierRefExpr(
               Ident->getSourceLoc(), IdentifierKind::MetaType,
               getContext().getMetaType(Subst->getType()));
         }

         if (Arg->isType()) {
            return new(Context) IdentifierRefExpr(
               Ident->getSourceLoc(), IdentifierKind::MetaType,
               getContext().getMetaType(Arg->getType()));
         }

         return StaticExpr::Create(Context, Arg->getParam()->getValueType(),
                                   Ident->getSourceRange(),
                                   Arg->getValue());
      }

      if (ValueSubst.Name == Ident->getIdentInfo()) {
         return StaticExpr::Create(Context, ValueSubst.SubstVal->getType(),
                                   Ident->getSourceRange(),
                                   ValueSubst.SubstVal);
      }
   }

   DeclarationName DeclName;
   DeclContext *Ctx = Ident->getDeclCtx();

   if (Ident->getKind() == IdentifierKind::FunctionArg) {
      if (Ident->getFuncArg()->isVariadicArgPackExpansion()) {
         DeclName = getVariadicArgumentSubst(Ident->getIdentInfo());
      }
   }
   else if (Ident->getKind() == IdentifierKind::Field
            || Ident->getKind() == IdentifierKind::StaticField) {
      if (Ident->getFieldDecl()->isVariadic()) {
         DeclName = getVariadicArgumentSubst(Ident->getIdentInfo());
      }
   }
   else if (Ident->getKind() == IdentifierKind::AssociatedType) {
      auto *AT = cast<AssociatedTypeDecl>(Ident->getNamedDecl());
      if (isa<ProtocolDecl>(AT->getDeclContext())) {
         Ctx = nullptr;
      }
   }

   if (!DeclName)
      DeclName = Ident->getDeclName();

   // only copy the context if it is not (in) a template, if it is it has to be
   // re-evaluated
   if (auto ND = dyn_cast_or_null<NamedDecl>(Ctx)) {
      if (ND->inDependentContext()) {
         Ctx = nullptr;
      }
   }

   auto Inst = new(Context) IdentifierRefExpr(Ident->getSourceLoc(), DeclName,
                                              Ctx);

   Inst->setSelf(Ident->isSelf());
   Inst->setNameSpec(visitNestedNameSpecifier(Ident->getNameSpec()));
   Inst->setParentExpr(copyOrNull(Ident->getParentExpr()));
   Inst->setIsPointerAccess(Ident->isPointerAccess());
   Inst->setAllowNamespaceRef(Ident->allowNamespaceRef());
   Inst->setLeadingDot(Ident->hasLeadingDot());

   return Inst;
}

BuiltinIdentExpr*InstantiatorImpl::visitBuiltinIdentExpr(BuiltinIdentExpr *node)
{
   return BuiltinIdentExpr::Create(Context, node->getSourceLoc(),
                                   node->getIdentifierKind());
}

EnumCaseExpr* InstantiatorImpl::visitEnumCaseExpr(EnumCaseExpr *node)
{
   auto Inst = new(Context) EnumCaseExpr(node->getSourceLoc(),
                                         node->getIdentInfo(),
                                         astvec(copyExprList(node->getArgs())));

   if (!node->isTypeDependent()) {
      Inst->setCase(node->getCase());
   }

   return Inst;
}

Expression* InstantiatorImpl::visitCallExpr(CallExpr *node)
{
   DeclContext *Ctx = node->getContext();
   if (auto ND = dyn_cast_or_null<NamedDecl>(Ctx)) {
      if (ND->inDependentContext()) {
         Ctx = nullptr;
      }
   }

   SmallVector<Expression*, 4> ArgVec;
   ArgVec.reserve(node->getArgs().size());

   SmallVector<IdentifierInfo*, 4> Labels;
   Labels.reserve(node->getLabels().size());

   copyArgumentList(node->getArgs(), node->getLabels(), ArgVec, Labels);

   auto call = CallExpr::Create(Context,
                                node->getIdentLoc(), node->getParenRange(),
                                copyOrNull(node->getParentExpr()),
                                astvec(ArgVec), Labels,
                                node->getDeclName(), node->isDotInit(),
                                node->isDotDeinit());

   call->setContext(Ctx);
   call->setIncludesSelf(node->includesSelf());
   call->setDirectCall(node->isDirectCall());
   call->setIsPointerAccess(node->isPointerAccess());
   call->setLeadingDot(node->hasLeadingDot());

//   if (!node->isTypeDependent()) {
//      assert(node->isValueDependent() && "instantiating non-dependent "
//                                         "expression!");
//
//      call->setKind(node->getKind());
//      call->setExprType(node->getExprType());
//
//      switch (node->getKind()) {
//      case CallKind::Builtin:
//         call->setBuiltinKind(node->getBuiltinKind());
//         break;
//      case CallKind::NamedFunctionCall:
//      case CallKind::MethodCall:
//      case CallKind::StaticMethodCall:
//      case CallKind::InitializerCall:
//      case CallKind::CallOperator:
//         call->setFunc(node->getFunc());
//         break;
//      case CallKind::UnionInitializer:
//         call->setUnion(node->getUnion());
//         break;
//      default:
//         break;
//      }
//   }

   return call;
}

Expression *InstantiatorImpl::visitAnonymousCallExpr(AnonymousCallExpr *Expr)
{
   SmallVector<Expression*, 4> ArgVec;
   ArgVec.reserve(Expr->getArgs().size());

   SmallVector<IdentifierInfo*, 4> Labels;
   Labels.reserve(Expr->getLabels().size());

   copyArgumentList(Expr->getArgs(), Expr->getLabels(), ArgVec, Labels);

   return AnonymousCallExpr::Create(Context, Expr->getParenRange(),
                                    visit(Expr->getParentExpr()),
                                    ArgVec, Labels);
}

SubscriptExpr *InstantiatorImpl::visitSubscriptExpr(SubscriptExpr *node)
{
   return SubscriptExpr::Create(Context, node->getSourceRange(),
                                copyOrNull(node->getParentExpr()),
                                copyExprList(node->getIndices()));
}

TupleMemberExpr* InstantiatorImpl::visitTupleMemberExpr(TupleMemberExpr *node)
{
   return new (Context) TupleMemberExpr(node->getSourceLoc(),
                                        copyOrNull(node->getParentExpr()),
                                        node->getIndex(),
                                        node->isPointerAccess());
}

TemplateArgListExpr*
InstantiatorImpl::visitTemplateArgListExpr(TemplateArgListExpr *Expr)
{
   return TemplateArgListExpr::Create(Context, Expr->getSourceRange(),
                                      visit(Expr->getParentExpr()),
                                      copyExprList(Expr->getExprs()));
}

ReturnStmt* InstantiatorImpl::visitReturnStmt(ReturnStmt *node)
{
   if (auto Val = node->getReturnValue()) {
      return ReturnStmt::Create(Context, node->getSourceLoc(), visit(Val));
   }

   return ReturnStmt::Create(Context, node->getSourceLoc());
}

DiscardAssignStmt*
InstantiatorImpl::visitDiscardAssignStmt(DiscardAssignStmt *Stmt)
{
   return DiscardAssignStmt::Create(Context, Stmt->getUnderscoreLoc(),
                                    Stmt->getEqualsLoc(),
                                    visit(Stmt->getRHS()));
}

UnaryOperator *InstantiatorImpl::visitUnaryOperator(UnaryOperator *node)
{
   return UnaryOperator::Create(Context,
                                node->getSourceRange().getStart(),
                                node->getKind(), node->getFunctionType(),
                                visit(node->getTarget()), node->isPrefix());
}

Expression *InstantiatorImpl::visitBinaryOperator(BinaryOperator *node)
{
   if (auto Ident = dyn_cast<IdentifierRefExpr>(node->getRhs())) {
      if (Ident->getIdent() == "...") {
         llvm::SmallVector<Expression* , 4> exprs;
         expandVariadicArgument(exprs, node->getLhs());

         if (exprs.empty()) {
            SP.diagnose(node, err_generic_error,
                  "left hand side of fold expression must contain at least "
                  "one parameter pack");

            goto end;
         }

         auto numExprs = exprs.size();
         if (numExprs == 1) {
            return exprs.front();
         }
         else {
            size_t i = 2;
            auto binOp =
               BinaryOperator::Create(Context,
                                      node->getSourceRange().getStart(),
                                      node->getKind(), node->getFunctionType(),
                                      exprs[0], exprs[1]);

            while (i < numExprs) {
               binOp = BinaryOperator::Create(Context,
                                              node->getSourceRange().getStart(),
                                              node->getKind(),
                                              node->getFunctionType(),
                                              binOp, exprs[i]);

               ++i;
            }

            return binOp;
         }
      }
   }

   end:
   return BinaryOperator::Create(Context,
                                 node->getSourceRange().getStart(),
                                 node->getKind(), node->getFunctionType(),
                                 visit(node->getLhs()), visit(node->getRhs()));
}

Expression* InstantiatorImpl::visitAssignExpr(AssignExpr *Expr)
{
   return AssignExpr::Create(Context, Expr->getEqualsLoc(),
                             visit(Expr->getLhs()), visit(Expr->getRhs()),
                             Expr->isInitialization());
}

CastExpr* InstantiatorImpl::visitCastExpr(CastExpr *Expr)
{
   return CastExpr::Create(Context, Expr->getAsLoc(),
                           Expr->getStrength(), visit(Expr->getTarget()),
                           visit(Expr->getTargetType()));
}

TypePredicateExpr*
InstantiatorImpl::visitTypePredicateExpr(TypePredicateExpr *Expr)
{
   return TypePredicateExpr::Create(Context, Expr->getIsLoc(),
                                    Expr->getSourceRange(),
                                    visit(Expr->getLHS()),
                                    clone(Expr->getRHS()),
                                    Expr->isNegated());
}

Expression* InstantiatorImpl::visitExprSequence(ExprSequence *node)
{
   std::vector<SequenceElement> fragments;
   for (const auto &El : node->getFragments()) {
      switch (El.getKind()) {
         case SequenceElement::EF_Operator:
            fragments.emplace_back(El.getOperatorKind(), El.getLoc());
            break;
         case SequenceElement::EF_PossibleOperator: {
            auto maybeIdent = new(Context) IdentifierRefExpr(El.getLoc(),
                                                             El.getOp());

            auto expr = visitIdentifierRefExpr(maybeIdent);

            auto ident = dyn_cast<IdentifierRefExpr>(expr);
            if (!ident || ident->getIdentInfo() != El.getOp())
               fragments.emplace_back(expr);
            else
               fragments.emplace_back(El.getOp(), El.getLoc());

            break;
         }
         case SequenceElement::EF_Expression:
            fragments.emplace_back(visit(El.getExpr()));
            break;
      }
   }

   return ExprSequence::Create(Context, fragments);
}

IfExpr* InstantiatorImpl::visitIfExpr(IfExpr *node)
{
   return IfExpr::Create(Context, node->getIfLoc(),
                         visit(node->getCond()), visit(node->getTrueVal()),
                         visit(node->getFalseVal()));
}

LambdaExpr* InstantiatorImpl::visitLambdaExpr(LambdaExpr *node)
{
   auto *E = LambdaExpr::Create(Context, node->getParenRange(),
                                node->getArrowLoc(),
                                visitOrAuto(node->getReturnType()),
                                cloneVector(node->getArgs()),
                                visit(node->getBody()));

   auto *II = &Context.getIdentifiers().get("__anonymous_lambda");
   auto Fun = FunctionDecl::Create(Context, AccessSpecifier::Private,
                                   E->getSourceLoc(), II, E->getArgs(),
                                   E->getReturnType(), E->getBody(), {});

   Fun->setIsLambda(true);
   Fun->setExternC(true);
   Fun->setDeclared(true);
   Fun->setSynthesized(true);
   Fun->setSemanticallyChecked(true);

   E->setFunc(Fun);
   SP.ActOnFunctionDecl(Fun);

   return E;
}

DictionaryLiteral*
InstantiatorImpl::visitDictionaryLiteral(DictionaryLiteral *node)
{
   return DictionaryLiteral::Create(Context, node->getSourceRange(),
                                    cloneVector(node->getKeys()),
                                    cloneVector(node->getValues()));
}

ArrayLiteral* InstantiatorImpl::visitArrayLiteral(ArrayLiteral *node)
{
   return ArrayLiteral::Create(Context, node->getSourceRange(),
                               cloneVector(node->getValues()));
}

IfCondition InstantiatorImpl::visitIfCondition(const IfCondition &C)
{
   switch (C.K) {
   case IfCondition::Expression:
      return IfCondition(visit(C.ExprData.Expr));
   case IfCondition::Binding:
      return IfCondition(clone(C.BindingData.Decl), C.BindingData.ConvSeq);
   case IfCondition::Pattern:
      return IfCondition(clone(C.PatternData.Pattern),
                         copyOrNull(C.PatternData.Expr));
   }
}

IfStmt* InstantiatorImpl::visitIfStmt(IfStmt *node)
{
   SmallVector<IfCondition, 2> Conditions;
   for (auto &C : node->getConditions()) {
      Conditions.push_back(visitIfCondition(C));
   }

   return IfStmt::Create(Context, node->getSourceLoc(),
                         Conditions, visit(node->getIfBranch()),
                         copyOrNull(node->getElseBranch()),
                         node->getLabel());
}

WhileStmt* InstantiatorImpl::visitWhileStmt(WhileStmt *node)
{
   SmallVector<IfCondition, 2> Conditions;
   for (auto &C : node->getConditions()) {
      Conditions.push_back(visitIfCondition(C));
   }

   return WhileStmt::Create(Context, node->getSourceLoc(), Conditions,
                            visit(node->getBody()), node->getLabel(),
                            node->isAtLeastOnce());
}

ForStmt* InstantiatorImpl::visitForStmt(ForStmt *node)
{
   return ForStmt::Create(Context, node->getSourceLoc(),
                          copyOrNull(node->getInitialization()),
                          copyOrNull(node->getTermination()),
                          copyOrNull(node->getIncrement()),
                          copyOrNull(node->getBody()),
                          node->getLabel());
}

ForInStmt* InstantiatorImpl::visitForInStmt(ForInStmt *node)
{
   return ForInStmt::Create(Context, node->getSourceLoc(),
                            copyOrNull(node->getDecl()),
                            visit(node->getRangeExpr()),
                            visit(node->getBody()),
                            node->getLabel());
}

MatchStmt* InstantiatorImpl::visitMatchStmt(MatchStmt *node)
{
   return MatchStmt::Create(Context, node->getMatchLoc(),
                            node->getBraceRange(),
                            visit(node->getSwitchValue()),
                            cloneVector(node->getCases()),
                            node->getLabel());
}

CaseStmt* InstantiatorImpl::visitCaseStmt(CaseStmt *node)
{
   return CaseStmt::Create(Context, node->getSourceLoc(),
                           copyOrNull(node->getPattern()),
                           copyOrNull(node->getBody()));
}

ExpressionPattern*
InstantiatorImpl::visitExpressionPattern(ExpressionPattern *node)
{
   return ExpressionPattern::Create(Context, node->getSourceLoc(),
                                    visit(node->getExpr()));
}

IsPattern* InstantiatorImpl::visitIsPattern(IsPattern *node)
{
   return IsPattern::Create(Context, node->getSourceRange(),
                            visit(node->getIsType()));
}

CasePattern* InstantiatorImpl::visitCasePattern(CasePattern *node)
{
   SmallVector<IfCondition, 4> Args;
   for (auto &Arg : node->getArgs()) {
      Args.push_back(visitIfCondition(Arg));
   }

   auto *Inst = CasePattern::Create(Context, node->getSourceRange(),
                                    node->getKind(),
                                    copyOrNull(node->getParentExpr()),
                                    node->getCaseNameIdent(), Args);

   Inst->setLeadingDot(node->hasLeadingDot());
   return Inst;
}

DoStmt* InstantiatorImpl::visitDoStmt(DoStmt *node)
{
   SmallVector<CatchBlock, 2> catchBlocks;
   catchBlocks.reserve(node->getCatchBlocks().size());

   for (const auto &CB : node->getCatchBlocks()) {
      catchBlocks.emplace_back(visitLocalVarDecl(CB.varDecl),
                               visit(CB.Body));
   }

   return new(Context) DoStmt(node->getSourceRange(), visit(node->getBody()),
                              catchBlocks, node->getLabel());
}

TryExpr *InstantiatorImpl::visitTryExpr(TryExpr *Expr)
{
   return TryExpr::Create(Context, Expr->getSourceLoc(), Expr->getKind(),
                          visit(Expr->getExpr()));
}

AwaitExpr* InstantiatorImpl::visitAwaitExpr(AwaitExpr *Expr)
{
   return AwaitExpr::Create(Context, Expr->getAwaitLoc(),
                            visit(Expr->getExpr()));
}

ThrowStmt* InstantiatorImpl::visitThrowStmt(ThrowStmt *node)
{
   return new(Context) ThrowStmt(node->getSourceLoc(),
                                 visit(node->getThrownVal()));
}

UsingDecl* InstantiatorImpl::visitUsingDecl(UsingDecl *node)
{
   return UsingDecl::Create(Context, node->getSourceRange(),
                            node->getAccess(), node->getDeclName(),
                            node->getNestedImportName(),
                            node->isWildcardImport());
}

ImportDecl* InstantiatorImpl::visitImportDecl(ImportDecl *node)
{
   return ImportDecl::Create(Context, node->getSourceRange(),
                             node->getAccess(),
                             node->getQualifiedImportName(),
                             node->getNamedImports(),
                             node->isWildcardImport());
}

UnittestDecl *InstantiatorImpl::visitUnittestDecl(UnittestDecl *D)
{
   return UnittestDecl::Create(Context, D->getKeywordLoc(), D->getBraceRange(),
                               D->getName(), visit(D->getBody()));
}

DeclStmt *InstantiatorImpl::visitDeclStmt(DeclStmt *Stmt)
{
   return DeclStmt::Create(Context, visit(Stmt->getDecl()));
}

StaticExpr *InstantiatorImpl::visitStaticExpr(StaticExpr *node)
{
   return StaticExpr::Create(Context, visit(node->getExpr()));
}

ConstraintExpr* InstantiatorImpl::visitConstraintExpr(ConstraintExpr *node)
{
   if (node->getKind() == ConstraintExpr::Type)
      return ConstraintExpr::Create(Context, node->getSourceLoc(),
                                    visit(node->getTypeConstraint()));

   return ConstraintExpr::Create(Context, node->getSourceLoc(),
                                 node->getKind());
}

TraitsExpr* InstantiatorImpl::visitTraitsExpr(TraitsExpr *node)
{
   std::vector<TraitsArgument> args;
   for (const auto &arg : node->getArgs()) {
      switch (arg.getKind()) {
         case TraitsArgument::Expr:
            args.emplace_back(visit(arg.getExpr()));
            break;
         case TraitsArgument::Stmt:
            args.emplace_back(visit(arg.getStmt()));
            break;
         case TraitsArgument::Type:
            args.emplace_back(visit(arg.getType()));
            break;
         case TraitsArgument::String:
            args.emplace_back(string(arg.getStr()));
            break;
      }
   }

   return TraitsExpr::Create(Context, node->getTraitsLoc(),
                             node->getParenRange(), node->getKind(), args);
}

MacroExpansionExpr*
InstantiatorImpl::visitMacroExpansionExpr(MacroExpansionExpr *Expr)
{
   return MacroExpansionExpr::Create(Context, Expr->getSourceRange(),
                                     Expr->getMacroName(), Expr->getDelim(),
                                     Expr->getTokens(),
                                     copyOrNull(Expr->getParentExpr()));
}

MacroExpansionStmt*
InstantiatorImpl::visitMacroExpansionStmt(MacroExpansionStmt *Stmt)
{
   return MacroExpansionStmt::Create(Context, Stmt->getSourceRange(),
                                     Stmt->getMacroName(),
                                     copyOrNull(Stmt->getParentExpr()),
                                     Stmt->getDelim(), Stmt->getTokens());
}

MacroExpansionDecl*
InstantiatorImpl::visitMacroExpansionDecl(MacroExpansionDecl *Decl)
{
   return MacroExpansionDecl::Create(Context, Decl->getSourceRange(),
                                     Decl->getMacroName(),
                                     copyOrNull(Decl->getParentExpr()),
                                     Decl->getDelim(), Decl->getTokens());
}

MixinExpr* InstantiatorImpl::visitMixinExpr(MixinExpr *Expr)
{
   return MixinExpr::Create(Context, Expr->getSourceRange(),
                            visit(Expr->getMixinExpr()));
}

MixinStmt* InstantiatorImpl::visitMixinStmt(MixinStmt *Stmt)
{
   return MixinStmt::Create(Context, Stmt->getSourceRange(),
                            visit(Stmt->getMixinExpr()));
}

MixinDecl* InstantiatorImpl::visitMixinDecl(MixinDecl *Decl)
{
   auto Inst = MixinDecl::Create(Context, Decl->getMixinLoc(),
                                 Decl->getParenRange(),
                                 visit(Decl->getMixinExpr()));

   ActOnDecl(Inst);
   return Inst;
}

Decl* InstantiatorImpl::visitStaticIfDecl(StaticIfDecl *node)
{
   // instantiating a static if or for requires some more care since none of
   // the branches will have been Sema-checked if the condition is dependent,
   // however instantiation relies on everything being checked. If the
   // condition is dependent, mark the instantiation as dependent for now and
   // let Sema handle the instantiation once it determine which branch to take.
   if (!node->getCondition()->isDependent()) {
      if (cast<il::ConstantInt>(node->getCondition()->getEvaluatedExpr())
            ->getBoolValue()) {
         return visit(node->getIfDecl());
      }

      return visit(node->getElseDecl());
   }

   StaticIfDecl *Template = node;
   if (auto PrevTemplate = node->getTemplate())
      Template = PrevTemplate;

   auto Decl = StaticIfDecl::Create(Context, node->getStaticLoc(),
                                    node->getRBRaceLoc(),
                                    clone(node->getCondition()),
                                    Template);

   ActOnDecl(Decl);
   return Decl;
}

Decl* InstantiatorImpl::visitStaticForDecl(StaticForDecl *node)
{
   auto Decl = StaticForDecl::Create(Context, node->getStaticLoc(),
                                     node->getRBRaceLoc(),
                                     node->getElementName(),
                                     clone(node->getRange()),
                                     copyOrNull(node->getBodyDecl()));

   ActOnDecl(Decl);
   return Decl;
}

Statement* InstantiatorImpl::visitStaticIfStmt(StaticIfStmt *node)
{
   if (!node->getCondition()->isDependent()) {
      if (cast<il::ConstantInt>(node->getCondition()->getEvaluatedExpr())
         ->getBoolValue()) {
         return visit(node->getIfBranch());
      }

      return visit(node->getElseBranch());
   }

   StaticIfStmt *Template = node;
   if (auto PrevTemplate = node->getTemplate())
      Template = PrevTemplate;

   return StaticIfStmt::Create(Context, node->getStaticLoc(), node->getIfLoc(),
                               visitStaticExpr(node->getCondition()), Template);
}

Statement* InstantiatorImpl::visitStaticForStmt(StaticForStmt *node)
{
   return StaticForStmt::Create(Context, node->getStaticLoc(),
                                node->getForLoc(), node->getElementName(),
                                visitStaticExpr(node->getRange()),
                                visit(node->getBody()));
}

StaticAssertStmt*InstantiatorImpl::visitStaticAssertStmt(StaticAssertStmt *node)
{
   auto Decl = StaticAssertStmt::Create(Context, node->getStaticAssertLoc(),
                                        node->getParenRange(),
                                        visitStaticExpr(node->getExpr()),
                                        node->getMessage());

   ActOnDecl(Decl);
   return Decl;
}

StaticPrintStmt* InstantiatorImpl::visitStaticPrintStmt(StaticPrintStmt *node)
{
   auto Decl = StaticPrintStmt::Create(Context, node->getStaticPrintLoc(),
                                       node->getParenRange(),
                                       visit(node->getExpr()));

   ActOnDecl(Decl);
   return Decl;
}

namespace {

class InstPrettyStackTraceEntry: public llvm::PrettyStackTraceEntry {
   InstantiatorImpl &Instantiator;

public:
   InstPrettyStackTraceEntry(InstantiatorImpl &Instantiator)
      : Instantiator(Instantiator)
   { }

   void print(llvm::raw_ostream &OS) const override
   {
      OS << "while instantiating "
         << Instantiator.getTemplate()->getDeclName()
         << " with template arguments "
         << Instantiator.getTemplateArgs()
         << "\n";
   }
};

} // anonymous namespace

static FinalTemplateArgumentList *MakeList(SemaPass &SP,
                                           const TemplateArgList &list) {
   return FinalTemplateArgumentList::Create(SP.getContext(), list);
}

template<class T>
T *lookupExternalInstantiation(StringRef MangledName, ModuleDecl *Mod)
{
   if (auto MF = Mod->getModFile()) {
      auto Inst = MF->LookupInstantiation(MangledName);
      if (Inst)
         return cast<T>(Inst);
   }

   for (auto *Import : Mod->getImportedModules()) {
      auto *BaseMod = Import->getBaseModule()->getDecl();
      if (BaseMod == Mod)
         continue;

      if (auto Inst = lookupExternalInstantiation<T>(MangledName, BaseMod))
         return cast<T>(Inst);
   }

   return nullptr;
}

template<class T>
T *lookupExternalInstantiation(StringRef MangledName, SemaPass &SP)
{
   auto *Mod = SP.getDeclContext().getDeclModule();
   return lookupExternalInstantiation<T>(
      MangledName, Mod->getBaseModule()->getModule()->getDecl());
}

bool TemplateInstantiator::checkInstantiationDepth(NamedDecl *Inst,
                                                   NamedDecl *CurDecl,
                                                   SourceLocation POI) {
   auto It = InstantiationDepthMap.find(CurDecl);
   if (It == InstantiationDepthMap.end()) {
      InstantiationDepthMap[Inst] = 0;
      return false;
   }

   if (It->getSecond() >= 512) {
      SemaPass::DeclScopeRAII DSR(SP, cast<DeclContext>(CurDecl));
      SP.diagnose(Inst, err_generic_error,
                  "maximum template instantiation depth (512) exceeded",
                  POI);

      return true;
   }

   InstantiationDepthMap[Inst] = It->getSecond() + 1;
   return false;
}

unsigned TemplateInstantiator::getInstantiationDepth(NamedDecl *Decl)
{
   auto It = InstantiationDepthMap.find(Decl);
   if (It == InstantiationDepthMap.end()) {
      return 0;
   }

   return It->getSecond();
}

void TemplateInstantiator::setInstantiationDepth(NamedDecl *Decl,
                                                 unsigned Depth) {
   InstantiationDepthMap[Decl] = Depth;
}

bool checkTemplateArgRecursion(SemaPass &SP, RecordDecl *Inst,
                               sema::FinalTemplateArgumentList *templateArgs) {
   for (auto &Arg : *templateArgs) {
      if (!Arg.isType())
         continue;

      if (Arg.getType() == SP.getContext().getRecordType(Inst)) {
         SP.diagnose(err_generic_error,
                     "template appears within its own template arguments",
                     Inst->getSourceLoc());

         return true;
      }
   }

   return false;
}

RecordInstResult
TemplateInstantiator::InstantiateRecord(StmtOrDecl POI,
                                        RecordDecl *rec,
                                        const sema::TemplateArgList
                                                         &templateArgs,
                                        bool *isNew) {
   if (templateArgs.isStillDependent())
      return RecordInstResult();

   return InstantiateRecord(POI, rec, MakeList(SP, templateArgs), isNew);
}

RecordInstResult
TemplateInstantiator::InstantiateRecord(StmtOrDecl POI,
                                        RecordDecl *Template,
                                        TemplateArgs *templateArgs,
                                        bool *isNew) {
   if (templateArgs->isStillDependent())
      return RecordInstResult();

   SP.ensureDeclared(Template);

   if (Template->isInvalid())
      return RecordInstResult();

   assert(!Template->isInstantiation() && "only instantiate base template!");

   // Check if we are already instantiating a record.
   bool AlreadyInstantiating = InstantiationDepth > 0;

   InstantiationDepthRAII IDR(*this);

   // Check if this instantiation exists within our current compilation.
   void *insertPos;
   if (auto R = SP.getContext().getRecordTemplateInstantiation(Template,
                                                               *templateArgs,
                                                               insertPos)) {
      if (isNew) *isNew = false;
      return R;
   }

   // Check if this instantiation exists in an external module.
   if (auto *MF = Template->getModFile())
      MF->LoadAllDecls(*Template);

   std::string MangledName;
   {
      llvm::raw_string_ostream OS(MangledName);
      SP.getMangler().manglePrefix(Template, *templateArgs, OS);
   }

   if (auto Inst = lookupExternalInstantiation<RecordDecl>(MangledName, SP)) {
      Inst->setImportedInstantiation(true);
      return Inst;
   }

   // create this beforehand so we don't accidentally use the template arg
   // list after moving it
   auto instInfo = new (SP.getContext()) InstantiationInfo<RecordDecl> {
      POI.getSourceRange().getStart(), templateArgs, Template
   };

   auto *InstScope = SP.getCurrentDecl();

   // Instantiate the record interface.
   InstantiatorImpl Instantiator(SP, *instInfo->templateArgs, Template);
   InstPrettyStackTraceEntry STE(Instantiator);

   auto Inst = Instantiator.instantiateRecordDecl(Template);
   if (!Inst)
      return RecordInstResult();

   if (checkInstantiationDepth(Inst, InstScope, POI.getSourceLoc()))
      return RecordInstResult();

   Inst->setInstantiationInfo(instInfo);
   Inst->setIsInstantiation(true);
   SP.registerInstantiation(Inst, InstScope);

   // avoid infinite recursion when this template is instantiated within its
   // own definition
   SP.getContext().insertRecordTemplateInstantiation(Inst, insertPos);
   Instantiator.instantiateRecordDefinition(Template, Inst);

   Instantiator.ActOnDecl(Inst);
   SP.registerDelayedInstantiation(Inst, POI);

   // Enqueue the instantiation for declaration and sema checking.
   PendingInstantiations.push(Inst);

   // there might be dependencies between nested instantiations, so we can't
   // calculate record sizes until we are not nested anymore
   if (!AlreadyInstantiating && !SP.encounteredError()) {
      visitPendingInstantiations();
      IDR.pop();
   }

   if (isNew)
      *isNew = true;

   return Inst;
}

void TemplateInstantiator::visitPendingInstantiations()
{
   auto PendingInstantiations = move(this->PendingInstantiations);
   while (!PendingInstantiations.empty()) {
      auto *Next = PendingInstantiations.front();
      PendingInstantiations.pop();

      SP.ensureDeclared(Next);
   }
}

FunctionInstResult
TemplateInstantiator::InstantiateFunction(StmtOrDecl POI, FunctionDecl *F,
                                          const sema::TemplateArgList
                                                               &templateArgs,
                                          bool *isNew) {
   if (templateArgs.isStillDependent())
      return FunctionInstResult();

   return InstantiateFunction(POI, F, MakeList(SP, templateArgs), isNew);
}

FunctionInstResult
TemplateInstantiator::InstantiateFunction(StmtOrDecl POI,
                                          FunctionDecl *Template,
                                          TemplateArgs *templateArgs,
                                          bool *isNew) {
   if (templateArgs->isStillDependent())
      return FunctionInstResult();

   SP.ensureVisited(Template);

   if (Template->isInvalid()) {
      POI.setIsInvalid(true);
      return FunctionInstResult();
   }

   void *insertPos;
   if (auto F = SP.getContext().getFunctionTemplateInstantiation(Template,
                                                                 *templateArgs,
                                                                 insertPos)) {
      if (isNew) *isNew = false;
      return cast<FunctionDecl>(F);
   }

   std::string MangledName;
   {
      llvm::raw_string_ostream OS(MangledName);
      SP.getMangler().manglePrefix(Template, *templateArgs, OS);
   }

   if (auto Inst = lookupExternalInstantiation<FunctionDecl>(MangledName, SP)) {
      Inst->setImportedInstantiation(true);
      return Inst;
   }

   auto *InstScope = SP.getCurrentDecl();

   InstantiatorImpl Instantiator(SP, *templateArgs, Template);
   InstPrettyStackTraceEntry STE(Instantiator);

   auto Inst = Instantiator.instantiateFunctionDecl(Template);
   if (!Inst)
      return FunctionInstResult();

   if (checkInstantiationDepth(Inst, InstScope, POI.getSourceLoc()))
      return FunctionInstResult();

   auto instInfo = new (SP.getContext()) InstantiationInfo<CallableDecl>{
      POI.getSourceRange().getStart(), templateArgs, Template
   };

   Inst->setInstantiationInfo(instInfo);
   Inst->setIsInstantiation(true);
   SP.registerInstantiation(Inst, InstScope);

   SemaPass::DeclScopeRAII raii(SP, Inst->getDeclContext());
   SP.getContext().insertFunctionTemplateInstantiation(Inst, insertPos);

   Instantiator.ActOnDecl(Inst);
   Instantiator.visitSourceTypes();

   SP.declareScoped(Inst);

   SP.registerDelayedInstantiation(Inst, POI);

   if (isNew)
      *isNew = true;

   return Inst;
}

MethodInstResult
TemplateInstantiator::InstantiateMethod(StmtOrDecl POI,
                                        MethodDecl *Template,
                                        const sema::TemplateArgList
                                                               &templateArgs,
                                        bool *isNew) {
   if (templateArgs.isStillDependent() || Template->getRecord()->isUnboundedTemplate())
      return MethodInstResult();

   return InstantiateMethod(POI, Template, MakeList(SP, templateArgs), isNew);
}

MethodInstResult
TemplateInstantiator::InstantiateMethod(StmtOrDecl POI,
                                        MethodDecl *Template,
                                        TemplateArgs *templateArgs,
                                        bool *isNew) {
   if (templateArgs->isStillDependent() || Template->getRecord()->isUnboundedTemplate())
      return MethodInstResult();

   if (Template->isInvalid()) {
      POI.setIsInvalid(true);
      return MethodInstResult();
   }

   // Only instantiate the complete initializer, the base initializer will be
   // synthesized afterwards.
   if (Template->isBaseInitializer()) {
      Template = cast<InitDecl>(Template)->getCompleteInit();
   }

   void *insertPos;
   if (auto M = SP.getContext().getFunctionTemplateInstantiation(Template,
                                                                 *templateArgs,
                                                                 insertPos)) {
      if (isNew) *isNew = false;
      return cast<MethodDecl>(M);
   }

   std::string MangledName;
   {
      llvm::raw_string_ostream OS(MangledName);
      SP.getMangler().manglePrefix(Template, *templateArgs, OS);
   }

   if (auto Inst = lookupExternalInstantiation<MethodDecl>(MangledName, SP)) {
      Inst->setImportedInstantiation(true);
      return Inst;
   }

   if (auto BT = Template->getBodyTemplate()) {
      if (auto Info = BT->getLazyFnInfo()) {
         // if the function is a template, we also need it's body
         Info->loadBody(BT);
      }

      SP.ensureVisited(BT);
   }
   else {
      SP.ensureVisited(Template);
   }

   auto *InstScope = SP.getCurrentDecl();

   MultiLevelFinalTemplateArgList MultiLevelList(*templateArgs);
   if (Template->getRecord()->isInstantiation()) {
      MultiLevelList.addOuterList(Template->getRecord()->getTemplateArgs());
   }

   InstantiatorImpl Instantiator(SP, move(MultiLevelList), Template);
   InstPrettyStackTraceEntry STE(Instantiator);

   MethodDecl* Inst;
   if (auto C = dyn_cast<InitDecl>(Template)) {
      Inst = Instantiator.instantiateInitDecl(C);
   }
   else if (auto D = dyn_cast<DeinitDecl>(Template)) {
      Inst = Instantiator.instantiateDeinitDecl(D);
   }
   else {
      Inst = Instantiator.instantiateMethodDecl(Template);
   }

   if (!Inst)
      return MethodInstResult();

   if (checkInstantiationDepth(Inst, InstScope, POI.getSourceLoc()))
      return MethodInstResult();

   auto instInfo = new (SP.getContext()) InstantiationInfo<CallableDecl>{
      POI.getSourceRange().getStart(), templateArgs, Template
   };

   Inst->setInstantiationInfo(instInfo);
   Inst->setIsInstantiation(true);
   SP.registerInstantiation(Inst, InstScope);

   Instantiator.ActOnDecl(Inst);
   Instantiator.visitSourceTypes();
   SP.getContext().insertFunctionTemplateInstantiation(Inst, insertPos);

   SP.declareScoped(Inst);

   SP.registerDelayedInstantiation(Inst, POI);

   if (isNew)
      *isNew = true;

   return Inst;
}

MethodInstResult
TemplateInstantiator::InstantiateProtocolDefaultImpl(SourceLocation POI,
                                                     RecordDecl *Rec,
                                                     MethodDecl *M) {
   MethodDecl *decl;
   {
      SemaPass::DeclScopeRAII declScopeRAII(SP, Rec);
      InstantiatorImpl Instantiator(SP, Rec);

      decl = Instantiator.instantiateMethodDecl(M);
      decl->setInstantiatedFromProtocolDefaultImpl(true);

      Instantiator.visitSourceTypes();
      if (Rec->isClass()) {
         decl->setMutating(false);
      }
   }

   return decl;
}

FunctionType* TemplateInstantiator::InstantiateFunctionType(
                                          StmtOrDecl SOD,
                                          CallableDecl *Template,
                                          const TemplateArgList &templateArgs) {
   auto FinalList = MakeList(SP, templateArgs);

   SemaPass::DeclScopeRAII declScopeRAII(SP, Template);
   InstantiatorImpl Instantiator(SP, *FinalList, SOD);

   SmallVector<FuncArgDecl*, 4> Args;
   SmallVector<FunctionType::ParamInfo, 4> paramInfo;
   Instantiator.copyArgListAndFindVariadic(Template, Args);

   SmallVector<QualType, 4> Tys;
   for (auto &Arg : Args) {
      if (!SP.visitSourceType(Arg->getType()))
         return nullptr;

      Tys.push_back(Arg->getType());
      paramInfo.emplace_back(Arg->getConvention());
   }

   auto RetInst = Instantiator.visit(Template->getReturnType());
   if (!SP.visitSourceType(RetInst))
      return nullptr;

   return SP.getContext().getFunctionType(RetInst, Tys, paramInfo,
                                          Template->getFunctionType()
                                                  ->getRawFlags());
}

StmtResult
TemplateInstantiator::InstantiateStatement(SourceLocation,
                                           Statement* stmt,
                                           const sema::TemplateArgList
                                                               &templateArgs) {
   auto FinalList = MakeList(SP, templateArgs);

   InstantiatorImpl Instantiator(SP, *FinalList);
   return Instantiator.instantiateStatement(stmt);
}

StmtResult
TemplateInstantiator::InstantiateStatement(SourceLocation instantiatedFrom,
                                           Statement *stmt,
                                           MultiLevelFinalTemplateArgList
                                                               &&templateArgs) {
   InstantiatorImpl Instantiator(SP, move(templateArgs));
   auto *Inst = Instantiator.instantiateStatement(stmt);
   assert(Instantiator.DelayedTypeDecls.empty());

   return Inst;
}

ExprResult TemplateInstantiator::InstantiateTypeExpr(RecordDecl *Rec,
                                                     Expression *E) {
   InstantiatorImpl Instantiator(SP, Rec);
   return cast<Expression>(Instantiator.instantiateStatement(E));
}

DeclResult
TemplateInstantiator::InstantiateDecl(SourceLocation instantiatedFrom,
                                      Decl *D,
                                      MultiLevelFinalTemplateArgList
                                                               &&templateArgs) {
   InstantiatorImpl Instantiator(SP, move(templateArgs), D);
   auto *Inst = Instantiator.instantiateDecl(D);
   Instantiator.visitSourceTypes();

   return Inst;
}

StmtResult
TemplateInstantiator::InstantiateStatement(StmtOrDecl,
                                           Statement *stmt,
                                           IdentifierInfo *SubstName,
                                           il::Constant *SubstVal) {
   InstantiatorImpl Instantiator(SP, SubstName, SubstVal);
   return Instantiator.instantiateStatement(stmt);
}

StmtResult TemplateInstantiator::InstantiateMethodBody(StmtOrDecl POI,
                                                       MethodDecl *Method) {
   SP.ensureVisited(Method->getBodyTemplate());

   if (Method->getBodyTemplate()->isInvalid()) {
      return StmtError();
   }

   MultiLevelFinalTemplateArgList ArgList;
   if (Method->getRecord()->isInstantiation()) {
      ArgList.addOuterList(Method->getRecord()->getTemplateArgs());
   }

   bool IsExternallyLoaded = false;
   if (auto LazyInfo = Method->getBodyTemplate()->getLazyFnInfo()) {
      LazyInfo->loadBody(Method->getBodyTemplate());
      IsExternallyLoaded = true;
   }

   auto *InstScope = SP.getCurrentDecl();

   InstantiatorImpl Instantiator(SP, move(ArgList), Method);
   auto Inst = Instantiator.instantiateStatement(Method->getBodyTemplate()
                                                       ->getBody());

   Method->setBody(Inst);
   Method->setSemanticallyChecked(false);
   Method->setBodyInstantiationLoc(POI.getSourceLoc());

   auto instInfo = new (SP.getContext()) InstantiationInfo<CallableDecl>{
      POI.getSourceLoc(), {}, Method
   };

   Method->setInstantiationInfo(instInfo);
   SP.registerInstantiation(Method, InstScope);

   SP.declareScoped(Method);
   SP.registerDelayedInstantiation(Method, POI);

   if (auto I = dyn_cast<InitDecl>(Method)) {
      if (I->isCompleteInitializer()) {
         SP.registerInstantiation(I->getBaseInit(), InstScope);
      }
      else {
         SP.registerInstantiation(I->getCompleteInit(), InstScope);
      }
   }

   // ILGen would skip this decl since it's external, but we still need to
   // define the body
   if (IsExternallyLoaded) {
      SP.getILGen().registerInstantiatedImportedMethod(Method);
   }

   return Inst;
}

ExprResult
TemplateInstantiator::InstantiateStaticExpr(SourceLocation instantiatedFrom,
                                            Expression* expr,
                                            const sema::TemplateArgList
                                                               &templateArgs) {
   auto Stmt = InstantiateStatement(instantiatedFrom, expr, templateArgs);
   if (!Stmt)
      return ExprError();
   
   return cast<Expression>(Stmt.getValue());
}

AliasInstResult
TemplateInstantiator::InstantiateAlias(AliasDecl *Template,
                                       SourceLocation instantiatedFrom,
                                       TemplateArgs *templateArgs) {
   if (templateArgs->isStillDependent())
      return AliasInstResult();

   void *insertPos;
   if (auto Inst = SP.getContext().getAliasTemplateInstantiation(Template,
                                                                 *templateArgs,
                                                                 insertPos)) {
      return Inst;
   }

   std::string MangledName;
   {
      llvm::raw_string_ostream OS(MangledName);
      SP.getMangler().manglePrefix(Template, *templateArgs, OS);
   }

   if (auto Inst = lookupExternalInstantiation<AliasDecl>(MangledName, SP)) {
      Inst->setImportedInstantiation(true);
      return Inst;
   }

   auto *InstScope = SP.getCurrentDecl();

   auto *InstInfo = new(SP.getContext()) InstantiationInfo<AliasDecl>(
      instantiatedFrom, move(templateArgs), Template
   );

   InstantiatorImpl Instantiator(SP, *InstInfo->templateArgs, Template);
   InstPrettyStackTraceEntry STE(Instantiator);

   auto Inst = Instantiator.instantiateAliasDecl(Template);
   if (!Inst)
      return AliasInstResult();

   if (checkInstantiationDepth(Inst, InstScope, instantiatedFrom))
      return AliasInstResult();

   Inst->setInstantiationInfo(InstInfo);
   Inst->setIsInstantiation(true);
   SP.registerInstantiation(Inst, InstScope);

   Instantiator.ActOnDecl(Inst);
   Instantiator.visitSourceTypes();
   SP.getContext().insertAliasTemplateInstantiation(Inst, insertPos);

   return Inst;
}

} // namespace cdot