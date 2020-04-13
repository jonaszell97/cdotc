#include "TemplateInstantiator.h"

#include "Template.h"
#include "cdotc/AST/ASTContext.h"
#include "cdotc/AST/TypeBuilder.h"
#include "cdotc/Basic/NestedNameSpecifier.h"
#include "cdotc/IL/Constants.h"
#include "cdotc/ILGen/ILGenPass.h"
#include "cdotc/Message/Diagnostics.h"
#include "cdotc/Module/Module.h"
#include "cdotc/Query/QueryContext.h"
#include "cdotc/Sema/SemaPass.h"
#include "cdotc/Serialization/ModuleFile.h"
#include "cdotc/Support/Log.h"

#include <llvm/ADT/SmallString.h>
#include <llvm/Support/raw_ostream.h>

using namespace cdot;
using namespace cdot::ast;
using namespace cdot::support;
using namespace cdot::diag;
using namespace cdot::sema;

namespace cdot {

static constexpr uint16_t GenericMask
    = TypeProperties::ContainsDependentNameType
      | TypeProperties::ContainsDependentSizeArrayType
      | TypeProperties::ContainsUnconstrainedGeneric
      | TypeProperties::ContainsUnknownAny
      | TypeProperties::ContainsTemplateParamType
      | TypeProperties::ContainsTemplate;

static constexpr uint16_t DependentMask
    = GenericMask | TypeProperties::ContainsAssociatedType;

class InstantiatorImpl : public ASTTypeVisitor<InstantiatorImpl>,
                         public TypeBuilder<InstantiatorImpl> {
public:
   InstantiatorImpl(SemaPass& SP, MultiLevelFinalTemplateArgList&& templateArgs,
                    StmtOrDecl SOD = StmtOrDecl(),
                    DeclContext* InstCtx = nullptr, SourceLocation POI = {})
       : ASTTypeVisitor(false), TypeBuilder(SP, SourceRange(POI)), SP(SP),
         Context(SP.getContext()),
         InstScope(SP, InstCtx ? InstCtx : &SP.getDeclContext()),
         templateArgs(move(templateArgs)), POI(POI)
   {
      SourceID = SP.getCompilationUnit().getFileMgr().getSourceId(
          SP.getCurrentDecl()->getSourceLoc());
   }

   InstantiatorImpl(SemaPass& SP, MultiLevelFinalTemplateArgList&& templateArgs,
                    NamedDecl* Template, QualType Self = QualType(),
                    SourceLocation POI = {})
       : ASTTypeVisitor(false), TypeBuilder(SP, SourceRange(POI)), SP(SP),
         Context(SP.getContext()), InstScope(SP, Template->getDeclContext()),
         templateArgs(move(templateArgs)), Template(Template), Self(Self),
         POI(POI)
   {
      SourceID = SP.getCompilationUnit().getFileMgr().getSourceId(
          SP.getCurrentDecl()->getSourceLoc());

      if (Self) {
         InstantiatingRecord = Self->getRecord();
      }
   }

   InstantiatorImpl(SemaPass& SP, IdentifierInfo* SubstName,
                    il::Constant* SubstVal)
       : ASTTypeVisitor(false), TypeBuilder(SP, SourceRange()), SP(SP),
         Context(SP.getContext()),
         InstScope(SP, &SP.getDeclContext()), ValueSubst{SubstName, SubstVal}
   {
      SourceID = SP.getCompilationUnit().getFileMgr().getSourceId(
          SP.getCurrentDecl()->getSourceLoc());
   }

   friend class cdot::TemplateInstantiator;

   RecordDecl* instantiateRecordDecl(RecordDecl* Decl);
   RecordDecl* instantiateRecordDefinition(RecordDecl* Template,
                                           RecordDecl* Inst);

   MethodDecl* instantiateMethodDecl(MethodDecl* Decl)
   {
      return cast_or_null<MethodDecl>(visit(Decl));
   }

   InitDecl* instantiateInitDecl(InitDecl* Decl)
   {
      return cast_or_null<InitDecl>(visit(Decl));
   }

   DeinitDecl* instantiateDeinitDecl(DeinitDecl* Decl)
   {
      return cast_or_null<DeinitDecl>(visit(Decl));
   }

   FunctionDecl* instantiateFunctionDecl(FunctionDecl* Decl)
   {
      return cast_or_null<FunctionDecl>(visit(Decl));
   }

   AliasDecl* instantiateAliasDecl(AliasDecl* Decl)
   {
      return cast_or_null<AliasDecl>(visit(Decl));
   }

   Statement* instantiateStatement(Statement* Stmt) { return visit(Stmt); }

   Decl* instantiateDecl(Decl* D) { return visit(D); }

   NamedDecl* getTemplate() const { return Template; }
   const MultiLevelFinalTemplateArgList& getTemplateArgs() const
   {
      return templateArgs;
   }

   Decl* ActOnDecl(Decl* D, DeclContext* Ctx = nullptr)
   {
      if (!Ctx) {
         Ctx = &SP.getDeclContext();
      }

      SP.ActOnDecl(Ctx, D);
      return D;
   }

private:
   bool canElideInstantiation(Statement* Stmt)
   {
      if (InUnevaluatedScope)
         return false;

      if (auto Decl = dyn_cast<DeclStmt>(Stmt)) {
         return canElideInstantiation(Decl->getDecl());
      }

#ifndef NDEBUG
      if (isa<DebugStmt>(Stmt))
         return false;
#endif

      return !Stmt->needsInstantiation();
   }

   bool canElideInstantiation(Expression* E)
   {
      if (InUnevaluatedScope)
         return false;

      if (!E->getExprType() || E->getExprType()->isUnknownAnyType())
         return false;

      if (E->needsInstantiation())
         return false;

      switch (E->getTypeID()) {
      case Expression::IdentifierRefExprID: {
         auto Ident = cast<IdentifierRefExpr>(E);
         if (Ident->getKind() != IdentifierKind::MetaType) {
            // property method arguments are always instantiated
            if (auto D = Ident->getNamedDecl()) {
               return !D->isSynthesized();
            }
         }

         break;
      }
      case Expression::SelfExprID:
      case Expression::SuperExprID:
         return false;
      default:
         break;
      }

      return true;
   }

   bool canElideInstantiation(Decl* D)
   {
      if (InUnevaluatedScope) {
         return false;
      }

      if (D->containsGenericParam() || D->containsAssociatedType()
          || D->isDependent()) {
         return false;
      }

      if (isa<NamedDecl>(D)) {
         switch (D->getKind()) {
         case Decl::LocalVarDeclID:
         case Decl::MacroDeclID:
         case Decl::MacroExpansionDeclID:
            return true;
         case Decl::FuncArgDeclID:
            return !cast<FuncArgDecl>(D)->getType()->isDependentType()
                   && !cast<FuncArgDecl>(D)->isSelf();
         case Decl::AssociatedTypeDeclID:
            return !cast<AssociatedTypeDecl>(D)
                        ->getActualType()
                        ->isDependentType();
         default:
            return false;
         }
      }

      return false;
   }

   Expression* visit(Expression* expr)
   {
      //      assert((expr->isSemanticallyChecked() ||
      //      expr->isContextDependent())
      //                && "instantiating unchecked expr");

      if (canElideInstantiation(expr))
         return expr;

      Expression* Inst;
      switch (expr->getTypeID()) {
#define CDOT_EXPR(Name)                                                        \
   case AstNode::Name##ID:                                                     \
      Inst = visit##Name(static_cast<Name*>(expr));                            \
      break;
#include "AST/AstNode.def"

      default:
         llvm_unreachable("not an expression!");
      }

      Inst->setUnsafe(expr->isUnsafe());
      Inst->setEllipsisLoc(expr->getEllipsisLoc());

      return Inst;
   }

   Statement* visit(Statement* stmt)
   {
      if (auto E = dyn_cast<Expression>(stmt))
         return visit(E);

      assert(stmt->isSemanticallyChecked() && "instantiating unchecked expr");

      if (canElideInstantiation(stmt))
         return stmt;

      Statement* Inst;
      switch (stmt->getTypeID()) {
#define CDOT_STMT(Name)                                                        \
   case AstNode::Name##ID:                                                     \
      Inst = visit##Name(static_cast<Name*>(stmt));                            \
      break;
#include "AST/AstNode.def"

      default:
         llvm_unreachable("not a statement!");
      }

      if (Inst)
         Inst->setUnsafe(stmt->isUnsafe());

      return Inst;
   }

   Decl* visit(Decl* D)
   {
      if (auto ND = dyn_cast<NamedDecl>(D))
         return visit(ND);

      if (canElideInstantiation(D))
         return D;

      Decl* Inst;
      switch (D->getKind()) {
#define CDOT_DECL(Name)                                                        \
   case Decl::Name##ID:                                                        \
      Inst = visit##Name(static_cast<Name*>(D));                               \
      break;
#include "AST/Decl.def"

      default:
         llvm_unreachable("not a decl!");
      }

      if (Inst)
         Context.setAttributes(Inst, D->getAttributes());

      return Inst;
   }

   NamedDecl* visit(NamedDecl* D)
   {
      if (canElideInstantiation(D))
         return D;

      NamedDecl* Inst;
      switch (D->getKind()) {
#define CDOT_NAMED_DECL(Name)                                                  \
   case Decl::Name##ID:                                                        \
      Inst = visit##Name(static_cast<Name*>(D));                               \
      break;
#include "AST/Decl.def"

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
   SourceType visit(const SourceType& Ty)
   {
      QualType ResolvedTy;
      if (Ty.isResolved()) {
         ResolvedTy = Ty.getResolvedType();
         uint16_t Props = ResolvedTy->properties().getRawProperties();

         while ((Props & DependentMask) != 0) {
            QualType Inst = ResolvedTy;

            if (Inst->containsAssociatedType() && Self) {
               if (SP.QC.SubstAssociatedTypes(Inst, Inst, Self, POI)) {
                  return Ty;
               }
            }

            if ((Props & TypeProperties::ContainsTemplate) != 0) {
               Inst = TypeBuilder::visit(Inst);
            }

            if ((Props & GenericMask) != 0) {
               if (SP.QC.SubstTemplateParamTypes(Inst, Inst, templateArgs,
                                                 POI)) {
                  return Ty;
               }
            }

            // No more changes.
            if (Inst == ResolvedTy) {
               break;
            }

            ResolvedTy = Inst;
            Props = ResolvedTy->properties().getRawProperties();
         }

         if ((ResolvedTy->properties() & DependentMask) == 0) {
            return SourceType(ResolvedTy);
         }
      }

      if (auto E = Ty.getTypeExpr()) {
         return SourceType(visit(Ty.getTypeExpr()));
      }

      if (ResolvedTy && !ResolvedTy->isUnknownAnyType()) {
         return SourceType(ResolvedTy);
      }

      return SourceType();
   }

   QualType visitRecordType(RecordType* T)
   {
      if (!Self) {
         return T;
      }

      auto* Rec = T->getRecord();

      // Find the innermost template.
      RecordDecl* Inst = Self->getRecord();
      while (!Inst->isInstantiation()) {
         Inst = Inst->getRecord();
      }

      auto* Template = Inst->getSpecializedTemplate();

      // Find the current instantiation of the referenced type.
      auto* CurTemplate = Rec;
      SmallVector<DeclarationName, 2> NestedNames;
      while (CurTemplate != Template) {
         NestedNames.push_back(CurTemplate->getDeclName());
         CurTemplate = CurTemplate->getRecord();

         if (!CurTemplate) {
            return T;
         }
      }

      for (auto It = NestedNames.rbegin(); It != NestedNames.rend(); ++It) {
         Inst = SP.QC.LookupSingleAs<RecordDecl>(Inst, *It);
         assert(Inst && "not correctly instantiated!");
      }

      return Context.getRecordType(Inst);
   }

private:
   SourceType visitOrAuto(const SourceType& Ty)
   {
      auto Result = visit(Ty);
      if (!Result)
         return SourceType(Context.getAutoType());

      return Result;
   }

   CompoundDecl* visitCompoundDecl(CompoundDecl* D);
   CompoundStmt* visitCompoundStmt(CompoundStmt* node);

   PrecedenceGroupDecl* visitPrecedenceGroupDecl(PrecedenceGroupDecl*)
   {
      llvm_unreachable("precedence groups can't be dependent!");
   }

   SourceFileDecl* visitSourceFileDecl(SourceFileDecl*)
   {
      llvm_unreachable("should not be in a template!");
   }

   OperatorDecl* visitOperatorDecl(OperatorDecl*)
   {
      llvm_unreachable("should not be in a template!");
   }

   NamespaceDecl* visitNamespaceDecl(NamespaceDecl*)
   {
      llvm_unreachable("should not be in a template!");
   }

   MacroDecl* visitMacroDecl(MacroDecl*)
   {
      llvm_unreachable("macros can't be dependent!");
   }

   MacroVariableExpr* visitMacroVariableExpr(MacroVariableExpr*)
   {
      llvm_unreachable("macros can't be dependent!");
   }

   ModuleDecl* visitModuleDecl(ModuleDecl* node)
   {
      llvm_unreachable("module should not be instantiated!");
   }

   DeclRefExpr* visitDeclRefExpr(DeclRefExpr*)
   {
      llvm_unreachable("should never be dependent!");
   }

   MemberRefExpr* visitMemberRefExpr(MemberRefExpr*)
   {
      llvm_unreachable("should never be dependent!");
   }

   OverloadedDeclRefExpr* visitOverloadedDeclRefExpr(OverloadedDeclRefExpr*)
   {
      llvm_unreachable("should never be dependent!");
   }

   MacroExpansionExpr* visitMacroExpansionExpr(MacroExpansionExpr* Expr);
   MacroExpansionStmt* visitMacroExpansionStmt(MacroExpansionStmt* Stmt);
   MacroExpansionDecl* visitMacroExpansionDecl(MacroExpansionDecl* Decl);

   UsingDecl* visitUsingDecl(UsingDecl* node);
   ImportDecl* visitImportDecl(ImportDecl* node);

   UnittestDecl* visitUnittestDecl(UnittestDecl* D);

   LocalVarDecl* visitLocalVarDecl(LocalVarDecl* node);
   GlobalVarDecl* visitGlobalVarDecl(GlobalVarDecl* node);
   DestructuringDecl* visitDestructuringDecl(DestructuringDecl* Decl);

   FunctionDecl* visitFunctionDecl(FunctionDecl* F);

   RecordDecl* visitRecordCommon(RecordDecl* Template);

   TemplateParamDecl* visitTemplateParamDecl(TemplateParamDecl* decl);

   ClassDecl* visitClassDecl(ClassDecl* node);
   StructDecl* visitStructDecl(StructDecl* node);
   ProtocolDecl* visitProtocolDecl(ProtocolDecl* node);
   ExtensionDecl* visitExtensionDecl(ExtensionDecl* Ext,
                                     RecordDecl* RecInst = nullptr);
   EnumDecl* visitEnumDecl(EnumDecl* node);
   UnionDecl* visitUnionDecl(UnionDecl* node);

   EnumCaseDecl* visitEnumCaseDecl(EnumCaseDecl* Decl);

   FieldDecl* visitFieldDecl(FieldDecl* Decl);
   PropDecl* visitPropDecl(PropDecl* Decl, bool IsFieldAccessor = false);
   SubscriptDecl* visitSubscriptDecl(SubscriptDecl* Decl);
   AssociatedTypeDecl* visitAssociatedTypeDecl(AssociatedTypeDecl* Decl);

   MethodDecl* visitMethodDecl(MethodDecl* M);
   InitDecl* visitInitDecl(InitDecl* Decl);
   DeinitDecl* visitDeinitDecl(DeinitDecl* Decl);

   FuncArgDecl* visitFuncArgDecl(FuncArgDecl* Decl);

   TypedefDecl* visitTypedefDecl(TypedefDecl* TD);
   AliasDecl* visitAliasDecl(AliasDecl* Alias);

   Expression* visitParenExpr(ParenExpr* Expr);
   Expression* visitIdentifierRefExpr(IdentifierRefExpr* Ident);
   BuiltinIdentExpr* visitBuiltinIdentExpr(BuiltinIdentExpr* node);
   SelfExpr* visitSelfExpr(SelfExpr* node);
   SuperExpr* visitSuperExpr(SuperExpr* node);

   Expression* visitBuiltinExpr(BuiltinExpr* node);

   SubscriptExpr* visitSubscriptExpr(SubscriptExpr* node);
   Expression* visitCallExpr(CallExpr* node);
   Expression* visitAnonymousCallExpr(AnonymousCallExpr* Expr);
   EnumCaseExpr* visitEnumCaseExpr(EnumCaseExpr* node);
   TupleMemberExpr* visitTupleMemberExpr(TupleMemberExpr* node);
   TemplateArgListExpr* visitTemplateArgListExpr(TemplateArgListExpr* Expr);

   ForStmt* visitForStmt(ForStmt* node);
   ForInStmt* visitForInStmt(ForInStmt* node);
   WhileStmt* visitWhileStmt(WhileStmt* node);
   IfStmt* visitIfStmt(IfStmt* node);

   MatchStmt* visitMatchStmt(MatchStmt* node);
   CaseStmt* visitCaseStmt(CaseStmt* node);
   ExpressionPattern* visitExpressionPattern(ExpressionPattern* node);

   IsPattern* visitIsPattern(IsPattern* node);
   CasePattern* visitCasePattern(CasePattern* node);

   ReturnStmt* visitReturnStmt(ReturnStmt* node);

   BreakStmt* visitBreakStmt(BreakStmt* node)
   {
      llvm_unreachable("should not be dependent!");
   }

   ContinueStmt* visitContinueStmt(ContinueStmt* node)
   {
      llvm_unreachable("should not be dependent!");
   }

   DiscardAssignStmt* visitDiscardAssignStmt(DiscardAssignStmt* Stmt);

   IntegerLiteral* visitIntegerLiteral(IntegerLiteral* node);
   FPLiteral* visitFPLiteral(FPLiteral* node);
   BoolLiteral* visitBoolLiteral(BoolLiteral* node);
   CharLiteral* visitCharLiteral(CharLiteral* node);

   DictionaryLiteral* visitDictionaryLiteral(DictionaryLiteral* node);
   ArrayLiteral* visitArrayLiteral(ArrayLiteral* node);

   NoneLiteral* visitNoneLiteral(NoneLiteral* node);
   StringLiteral* visitStringLiteral(StringLiteral* node);

   StringInterpolation* visitStringInterpolation(StringInterpolation* node);

   TupleLiteral* visitTupleLiteral(TupleLiteral* node);

   Expression* visitExprSequence(ExprSequence* node);
   Expression* visitBinaryOperator(BinaryOperator* node);
   Expression* visitAssignExpr(AssignExpr* Expr);
   CastExpr* visitCastExpr(CastExpr* node);
   AddrOfExpr* visitAddrOfExpr(AddrOfExpr* Expr);
   TypePredicateExpr* visitTypePredicateExpr(TypePredicateExpr* node);
   UnaryOperator* visitUnaryOperator(UnaryOperator* node);

   IfExpr* visitIfExpr(IfExpr* Expr);

   StaticExpr* visitStaticExpr(StaticExpr* node);
   ConstraintExpr* visitConstraintExpr(ConstraintExpr* node);
   TraitsExpr* visitTraitsExpr(TraitsExpr* node);

   DeclStmt* visitDeclStmt(DeclStmt* Stmt);

   StaticAssertDecl* visitStaticAssertDecl(StaticAssertDecl* node);
   StaticPrintDecl* visitStaticPrintDecl(StaticPrintDecl* node);
   Decl* visitStaticIfDecl(StaticIfDecl* node);
   Decl* visitStaticForDecl(StaticForDecl* node);

   Statement* visitStaticIfStmt(StaticIfStmt* node);
   Statement* visitStaticForStmt(StaticForStmt* node);

   AttributedStmt* visitAttributedStmt(AttributedStmt* S);
   AttributedExpr* visitAttributedExpr(AttributedExpr* E);

   DoStmt* visitDoStmt(DoStmt* node);
   TryExpr* visitTryExpr(TryExpr* Expr);
   AwaitExpr* visitAwaitExpr(AwaitExpr* Expr);
   ThrowStmt* visitThrowStmt(ThrowStmt* node);

   LambdaExpr* visitLambdaExpr(LambdaExpr* node);

   Expression* visitImplicitCastExpr(ImplicitCastExpr* node);

   DebugStmt* visitDebugStmt(DebugStmt* node);
   DebugDecl* visitDebugDecl(DebugDecl* D);

   NullStmt* visitNullStmt(NullStmt* node)
   {
      llvm_unreachable("should not be dependent!");
   }

   MixinExpr* visitMixinExpr(MixinExpr* Expr);
   MixinStmt* visitMixinStmt(MixinStmt* Stmt);
   MixinDecl* visitMixinDecl(MixinDecl* Decl);

   FunctionTypeExpr* visitFunctionTypeExpr(FunctionTypeExpr* Expr);
   TupleTypeExpr* visitTupleTypeExpr(TupleTypeExpr* Expr);
   ArrayTypeExpr* visitArrayTypeExpr(ArrayTypeExpr* Expr);
   DeclTypeExpr* visitDeclTypeExpr(DeclTypeExpr* Expr);
   PointerTypeExpr* visitPointerTypeExpr(PointerTypeExpr* Expr);
   ReferenceTypeExpr* visitReferenceTypeExpr(ReferenceTypeExpr* Expr);
   OptionTypeExpr* visitOptionTypeExpr(OptionTypeExpr* Expr);
   ExistentialTypeExpr* visitExistentialTypeExpr(ExistentialTypeExpr* Expr);

   /// Reference to the Sema instance.
   SemaPass& SP;

   /// Reference to the AST context.
   ASTContext& Context;

   /// Declaration scope for this instantiation.
   SemaPass::DeclScopeRAII InstScope;

   /// The template arguments we are instantiating with.
   MultiLevelFinalTemplateArgList templateArgs;

   /// The template we are instantiating.
   NamedDecl* Template = nullptr;

   /// The 'Self' declaration to use for substituting associated types.
   QualType Self;

   /// The record currently being instantiated.
   RecordDecl* InstantiatingRecord = nullptr;

   /// The location where the instantiation was requested from.
   SourceRange POI;

   /// Declarations with types that need delayed instantiation.
   SmallVector<Decl*, 4> DelayedTypeDecls;

   /// Map from template parameter names to variadic template arguments.
   llvm::StringMap<TemplateArgument*> VariadicTemplateArgs;

   /// True iff we're currently in an unevaluated scope.
   bool InUnevaluatedScope = false;

   /// Source ID we're instantiating from.
   unsigned SourceID = fs::InvalidID;

   /// Value substitution in a 'static for'.
   struct {
      IdentifierInfo* Name;
      il::Constant* SubstVal = nullptr;
   } ValueSubst;

   struct UnevalutedScopeRAII {
      UnevalutedScopeRAII(InstantiatorImpl& Inst, bool unevaluated)
          : Inst(Inst), previous(Inst.InUnevaluatedScope)
      {
         Inst.InUnevaluatedScope |= unevaluated;
      }

      ~UnevalutedScopeRAII() { Inst.InUnevaluatedScope = previous; }

   private:
      InstantiatorImpl& Inst;
      bool previous;
   };

   struct SubstContext {
   public:
      SubstContext(TemplateParamDecl* Param, const IdentifierInfo* FuncArg,
                   DeclarationName IdentSubst, const TemplateArgument* ArgSubst)
          : Param(Param), FuncArg(FuncArg), IdentSubst(IdentSubst),
            ArgSubst(ArgSubst)
      {
      }

      TemplateParamDecl* Param;

      const IdentifierInfo* FuncArg;
      DeclarationName IdentSubst;

      const TemplateArgument* ArgSubst;
   };

   struct SubstContextRAII {
      SubstContextRAII(InstantiatorImpl& Inst, TemplateParamDecl* Param,
                       const IdentifierInfo* FuncArg,
                       DeclarationName IdentSubst,
                       const TemplateArgument* ArgSubst)
          : Inst(Inst)
      {
         Inst.SubstContexts.emplace_back(Param, FuncArg, IdentSubst, ArgSubst);
      }

      ~SubstContextRAII() { Inst.SubstContexts.pop_back(); }

   private:
      InstantiatorImpl& Inst;
   };

   std::vector<SubstContext> SubstContexts;

   struct InstantiatingRecordRAII {
      InstantiatingRecordRAII(InstantiatorImpl& Inst, RecordDecl* R)
          : Inst(Inst), Prev(Inst.InstantiatingRecord)
      {
         Inst.InstantiatingRecord = R;
      }

      ~InstantiatingRecordRAII() { Inst.InstantiatingRecord = Prev; }

   private:
      InstantiatorImpl& Inst;
      RecordDecl* Prev;
   };

   DeclarationName getVariadicArgumentSubst(const IdentifierInfo* II)
   {
      auto end_it = SubstContexts.rend();
      for (auto it = SubstContexts.rbegin(); it != end_it; ++it) {
         auto& Subst = *it;
         if (!Subst.FuncArg)
            continue;

         if (Subst.FuncArg == II)
            return Subst.IdentSubst;
      }

      return nullptr;
   }

   const TemplateArgument* getParameterSubst(DeclarationName name)
   {
      auto end_it = SubstContexts.rend();
      for (auto it = SubstContexts.rbegin(); it != end_it; ++it) {
         auto& Subst = *it;
         if (!Subst.ArgSubst)
            continue;

         if (Subst.Param->getDeclName() == name)
            return Subst.ArgSubst;
      }

      return nullptr;
   }

   const TemplateArgument* getParameterSubst(TemplateParamDecl* P)
   {
      auto end_it = SubstContexts.rend();
      for (auto it = SubstContexts.rbegin(); it != end_it; ++it) {
         auto& Subst = *it;
         if (!Subst.ArgSubst)
            continue;

         if (Subst.Param->getDeclName() == P->getDeclName())
            return Subst.ArgSubst;
      }

      return nullptr;
   }

   const TemplateArgument* hasTemplateArg(DeclarationName Name)
   {
      if (auto P = getParameterSubst(Name))
         return P;

      return templateArgs.getNamedArg(Name);
   }

   const TemplateArgument* hasTemplateArg(TemplateParamDecl* P)
   {
      if (auto Arg = getParameterSubst(P))
         return Arg;

      return templateArgs.getNamedArg(P->getDeclName());
   }

   IfCondition visitIfCondition(const IfCondition& C);

   Expression* makeLiteralExpr(Expression* Expr, QualType Ty,
                               const Variant& Val);

   ASTContext const& getContext() const
   {
      return SP.getCompilationUnit().getContext();
   }

   template<class T, class... Args>
   T* makeStmt(Statement const* node, Args&&... args)
   {
      auto ret = SP.makeStmt<T>(std::forward<Args&&>(args)...);
      ret->setSourceLoc(node->getSourceLoc());

      return ret;
   }

   template<class T, class... Args>
   T* makeDecl(NamedDecl const* node, Args&&... args)
   {
      auto ret = SP.makeStmt<T>(std::forward<Args&&>(args)...);
      ret->setSourceLoc(node->getSourceLoc());

      return ret;
   }

   template<class T, class... Args>
   T* makeDecl(Decl const* node, Args&&... args)
   {
      auto ret = SP.makeStmt<T>(std::forward<Args&&>(args)...);
      ret->setSourceLoc(node->getSourceLoc());

      return ret;
   }

   template<class T, class... Args>
   T* makeExpr(Expression const* node, Args&&... args)
   {
      auto ret = SP.makeStmt<T>(std::forward<Args&&>(args)...);

      ret->setSourceLoc(node->getSourceLoc());
      ret->setIsVariadicArgPackExpansion(node->isVariadicArgPackExpansion());

      return ret;
   }

   template<class T>
   std::vector<T*> cloneVector(const llvm::iterator_range<T**>& vec)
   {
      std::vector<T*> newVec;
      for (const auto& v : vec) {
         auto newStmt = visit(v);
         newVec.push_back(cast<T>(newStmt));
      }

      return newVec;
   }

   template<class T> std::vector<T*> cloneVector(const std::vector<T*>& vec)
   {
      std::vector<T*> newVec;
      for (const auto& v : vec) {
         auto newStmt = visit(v);
         newVec.push_back(cast<T>(newStmt));
      }

      return newVec;
   }

   template<class T> std::vector<T*> cloneVector(llvm::ArrayRef<T*> vec)
   {
      std::vector<T*> newVec;
      for (const auto& v : vec) {
         auto newStmt = visit(v);
         newVec.push_back(cast<T>(newStmt));
      }

      return newVec;
   }

   template<class T> T* copyOrNull(T* sp)
   {
      if (!sp)
         return nullptr;

      return cast<T>(visit(sp));
   }

   template<class T> T* clone(T* sp) { return cast<T>(visit(sp)); }

   template<class T>
   ASTVector<TemplateParamDecl*> copyTemplateParameters(const T* from)
   {
      auto OriginalParams = from->getTemplateParams();
      ASTVector<TemplateParamDecl*> Params(Context, OriginalParams.size());

      for (auto& P : OriginalParams) {
         if (!hasTemplateArg(P)) {
            Params.push_back(P, Context);
         }
      }

      return Params;
   }

public:
   void
   copyArgListAndFindVariadic(CallableDecl* C,
                              llvm::SmallVectorImpl<FuncArgDecl*>& Variadics);

private:
   template<class Container, class Expr = typename std::remove_reference<
                                 decltype(*Container().front())>::type>
   std::vector<Expr*> copyExprList(const Container& exprList);

   void copyArgumentList(ArrayRef<Expression*> Args,
                         ArrayRef<IdentifierInfo*> Labels,
                         SmallVectorImpl<Expression*>& ResultArgs,
                         SmallVectorImpl<IdentifierInfo*>& ResultLabels);

   void copyParameterList(ArrayRef<SourceType> Args,
                          ArrayRef<FunctionType::ParamInfo> ParamInfo,
                          SmallVectorImpl<SourceType>& ResultArgs,
                          SmallVectorImpl<FunctionType::ParamInfo>& ResultInfo);

   std::vector<SourceType> copyTypeList(llvm::ArrayRef<SourceType> Tys);
   ASTVector<SourceType> copyConformances(llvm::ArrayRef<SourceType> Tys);

   ASTVector<Expression*> astvec(ArrayRef<Expression*> Vec)
   {
      ASTVector<Expression*> Copy(Context, Vec.size());
      Copy.insert(Context, Copy.end(), Vec.begin(), Vec.end());

      return Copy;
   }

   class VariadicParamFinder : public RecursiveASTVisitor<VariadicParamFinder> {
   public:
      VariadicParamFinder(llvm::SmallPtrSetImpl<TemplateParamDecl*>& Params,
                          llvm::SmallPtrSetImpl<FuncArgDecl*>& PackArguments)
          : VariadicParams(Params), PackArguments(PackArguments)
      {
      }

      bool visitDeclRefExpr(DeclRefExpr* Expr)
      {
         auto* ND = Expr->getDecl();
         switch (ND->getKind()) {
         case Decl::TemplateParamDeclID: {
            auto* Param = cast<TemplateParamDecl>(ND);
            if (Param->isVariadic()) {
               VariadicParams.insert(Param);
            }

            break;
         }
         case Decl::FuncArgDeclID: {
            auto* Arg = cast<FuncArgDecl>(ND);
            if (Arg->isVariadicArgPackExpansion()) {
               PackArguments.insert(Arg);
               visit(Arg->getType().getTypeExpr());
            }

            break;
         }
         default:
            break;
         }

         return false;
      }

   private:
      llvm::SmallPtrSetImpl<TemplateParamDecl*>& VariadicParams;
      llvm::SmallPtrSetImpl<FuncArgDecl*>& PackArguments;
   };

   template<class Container>
   using element_type =
       typename std::remove_pointer<typename std::remove_reference<decltype(
           ((Container*)0)->front())>::type>::type;

   template<class Container, class Element = element_type<Container>>
   bool expandVariadicArgument(Container& exprs, Expression* variadicExpr)
   {
      if (!variadicExpr->isVariadicArgPackExpansion()) {
         exprs.emplace_back(cast<Element>(visit(variadicExpr)));
         return false;
      }

      SmallPtrSet<TemplateParamDecl*, 4> VariadicParams;
      SmallPtrSet<FuncArgDecl*, 4> PackArguments;

      VariadicParamFinder Visitor(VariadicParams, PackArguments);
      Visitor.visit(variadicExpr);

      assert(!VariadicParams.empty() && "undiagnosed invalid pack expansion");

      if (VariadicParams.size() > 1) {
         llvm_unreachable("FIXME error message");
      }

      TemplateParamDecl* ParameterPack = *VariadicParams.begin();
      FuncArgDecl* PackArgument = nullptr;

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
         const IdentifierInfo* FuncArg = nullptr;

         if (PackArgument) {
            FuncArg = PackArgument->getIdentifierInfo();
            IdentSubst
                = Context.getDeclNameTable().getPackExpansionName(FuncArg, i);
         }

         SubstContextRAII raii(*this, ParameterPack, FuncArg, IdentSubst,
                               &GivenArg->getVariadicArgs()[i]);

         Expression* newExpr = visit(variadicExpr);
         newExpr->setEllipsisLoc(SourceLocation());

         exprs.emplace_back(newExpr);
      }

      return true;
   }

   bool expandVariadicDecl(FieldDecl* Decl)
   {
      llvm::SmallPtrSet<TemplateParamDecl*, 4> VariadicParams;
      llvm::SmallPtrSet<FuncArgDecl*, 4> PackArguments;

      VariadicParamFinder Visitor(VariadicParams, PackArguments);
      Visitor.visit(Decl->getType().getTypeExpr());

      assert(!VariadicParams.empty() && "undiagnosed invalid pack expansion");

      if (VariadicParams.size() > 1) {
         llvm_unreachable("FIXME error message");
      }

      TemplateParamDecl* ParameterPack = *VariadicParams.begin();

      auto GivenArg = hasTemplateArg(ParameterPack);
      if (!GivenArg) {
         // current context is still templated
         visit(Decl);
         return false;
      }

      assert(Decl->getValue() && !Decl->getAccessor());
      assert(GivenArg->isVariadic() && "invalid passed template argument");

      unsigned numVariadics = (unsigned)GivenArg->getVariadicArgs().size();
      for (unsigned i = 0; i < numVariadics; ++i) {
         DeclarationName DN = Context.getDeclNameTable().getPackExpansionName(
             Decl->getDeclName(), i);

         SourceType T(GivenArg->getVariadicArgs()[i].getType());
         auto Inst = FieldDecl::Create(
             Context, Decl->getAccess(), Decl->getVarOrLetLoc(),
             Decl->getColonLoc(), DN, T, Decl->isStatic(), Decl->isConst(),
             nullptr);

         ActOnDecl(Inst);

         if (auto Ty = Inst->getType().getTypeExpr()) {
            assert(Inst->getType().getTypeExpr()
                       != Decl->getType().getTypeExpr()
                   && "didn't instantiate dependent type expression!");

            Ty->setEllipsisLoc(SourceLocation());
         }
      }

      return true;
   }

   bool expandVariadicDecl(FuncArgDecl* Decl,
                           llvm::SmallVectorImpl<FuncArgDecl*>& Vec)
   {
      llvm::SmallPtrSet<TemplateParamDecl*, 4> VariadicParams;
      llvm::SmallPtrSet<FuncArgDecl*, 4> PackArguments;

      VariadicParamFinder Visitor(VariadicParams, PackArguments);
      Visitor.visit(Decl->getType().getTypeExpr());

      assert(!VariadicParams.empty() && "undiagnosed invalid pack expansion");

      if (VariadicParams.size() > 1) {
         llvm_unreachable("FIXME error message");
      }

      TemplateParamDecl* ParameterPack = *VariadicParams.begin();

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
         DeclarationName DN = Context.getDeclNameTable().getPackExpansionName(
             Decl->getDeclName(), i);

         SourceType T(GivenArg->getVariadicArgs()[i].getType());
         auto* Inst = FuncArgDecl::Create(
             Context, Decl->getOwnershipLoc(), Decl->getColonLoc(), DN,
             Decl->getLabel(), Decl->getConvention(), T,
             copyOrNull(Decl->getDefaultVal()), false, Decl->isCstyleVararg(),
             Decl->isSelf());

         if (auto Ty = Inst->getType().getTypeExpr()) {
            assert(Inst->getType().getTypeExpr()
                       != Decl->getType().getTypeExpr()
                   && "didn't instantiate dependent type expression!");

            Ty->setEllipsisLoc(SourceLocation());
         }

         Vec.push_back(Inst);
      }

      return true;
   }
};

} // namespace cdot

TemplateParamDecl*
InstantiatorImpl::visitTemplateParamDecl(TemplateParamDecl* P)
{
   TemplateParamDecl* Inst;
   if (P->isTypeName()) {
      Inst = TemplateParamDecl::Create(
          Context, P->getDeclName(), visitOrAuto(P->getCovariance()),
          visitOrAuto(P->getContravariance()), copyOrNull(P->getDefaultValue()),
          P->getIndex(), P->getTypeNameOrValueLoc(), P->getNameLoc(),
          P->getEllipsisLoc(), P->isUnbounded());
   }
   else {
      Inst = TemplateParamDecl::Create(
          Context, P->getDeclName(), visitOrAuto(P->getCovariance()),
          copyOrNull(P->getDefaultValue()), P->getIndex(),
          P->getTypeNameOrValueLoc(), P->getNameLoc(), P->getEllipsisLoc(),
          P->isUnbounded());
   }

   return Inst;
}

RecordDecl* InstantiatorImpl::instantiateRecordDecl(RecordDecl* Decl)
{
   AccessSpecifier access = Decl->getAccess();
   DeclarationName Name = Decl->getDeclName();

   if (Decl->isTemplate() && Decl == Template) {
      Name = Context.getDeclNameTable().getInstantiationName(
          Decl->getDeclName(), templateArgs.innermost());
   }

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

   RecordDecl* Inst;
   switch (Decl->getKind()) {
   case Decl::StructDeclID:
      Inst = StructDecl::Create(Context, access, Decl->getKeywordLoc(), Name,
                                move(conformances), move(templateParmams));
      break;
   case Decl::ClassDeclID:
      Inst = ClassDecl::Create(Context, access, Decl->getKeywordLoc(), Name,
                               move(conformances), move(templateParmams),
                               enumRawTypeOrExtends, isAbstract);
      break;
   case Decl::EnumDeclID:
      Inst = EnumDecl::Create(Context, access, Decl->getKeywordLoc(), Name,
                              move(conformances), move(templateParmams),
                              enumRawTypeOrExtends);
      break;
   case Decl::UnionDeclID:
      Inst = UnionDecl::Create(Context, access, Decl->getKeywordLoc(), Name,
                               move(conformances), move(templateParmams));
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

   if (auto* E = dyn_cast<EnumDecl>(Decl)) {
      cast<EnumDecl>(Inst)->setMaxAssociatedValues(E->getMaxAssociatedValues());
      cast<EnumDecl>(Inst)->setUnpopulated(E->isUnpopulated());
   }

   return Inst;
}

RecordDecl* InstantiatorImpl::instantiateRecordDefinition(RecordDecl* Template,
                                                          RecordDecl* Inst)
{
   llvm_unreachable("remove this!");
}

RecordDecl* InstantiatorImpl::visitRecordCommon(RecordDecl* Template)
{
   if (auto* MF = Template->getModFile()) {
      MF->LoadAllDecls(*Template);
   }

   auto Inst = instantiateRecordDecl(Template);

   auto R = instantiateRecordDefinition(Template, Inst);
   if (R) {
      ActOnDecl(R);
   }

   return R;
}

ClassDecl* InstantiatorImpl::visitClassDecl(ClassDecl* node)
{
   return cast<ClassDecl>(visitRecordCommon(node));
}

StructDecl* InstantiatorImpl::visitStructDecl(StructDecl* node)
{
   return cast<StructDecl>(visitRecordCommon(node));
}

EnumDecl* InstantiatorImpl::visitEnumDecl(EnumDecl* node)
{
   return cast<EnumDecl>(visitRecordCommon(node));
}

EnumCaseDecl* InstantiatorImpl::visitEnumCaseDecl(EnumCaseDecl* Decl)
{
   auto EC = EnumCaseDecl::Create(
       Context, Decl->getAccess(), Decl->getCaseLoc(), Decl->getIdentLoc(),
       Decl->getDeclName(), copyOrNull(Decl->getRawValExpr()),
       cloneVector(Decl->getArgs()));

   EC->setILValue(Decl->getILValue());
   return EC;
}

UnionDecl* InstantiatorImpl::visitUnionDecl(UnionDecl* node)
{
   return cast<UnionDecl>(visitRecordCommon(node));
}

ProtocolDecl* InstantiatorImpl::visitProtocolDecl(ProtocolDecl* node)
{
   return cast<ProtocolDecl>(visitRecordCommon(node));
}

ExtensionDecl* InstantiatorImpl::visitExtensionDecl(ExtensionDecl* Ext,
                                                    RecordDecl* RecInst)
{
   assert(RecInst && "instantiating extension without instantiated record!");

   auto constraints = cloneVector(Ext->getConstraints());

   auto Result = SP.checkConstraints(RecInst, Ext, constraints, RecInst);
   if (Result.getFailedConstraint()) {
      return nullptr;
   }

   // Make sure all declarations are visible.
   if (auto* MF = Ext->getModFile())
      MF->LoadAllDecls(*Ext);

   AccessSpecifier access = Ext->getAccess();
   auto conformances = copyConformances(Ext->getConformanceTypes());

   auto Inst = ExtensionDecl::Create(Context, access, Ext->getExtLoc(), RecInst,
                                     conformances);

   Inst->setName(Context.getDeclNameTable().getExtensionName(
       Context.getRecordType(RecInst)));

   RecInst->addExtension(Inst);

   {
      SemaPass::DeclContextRAII declContext(SP, Inst,
                                            SemaPass::SetParentContext);

      for (const auto& namedDecl : Ext->getDecls()) {
         if (isa<TemplateParamDecl>(namedDecl))
            continue;

         if (namedDecl->isSynthesized())
            continue;

         (void)visit(namedDecl);
      }
   }

   Context.setConstraints(Inst, constraints);
   Context.registerInstantiation(Ext, Inst);

   return Inst;
}

PropDecl* InstantiatorImpl::visitPropDecl(PropDecl* Decl, bool IsFieldAccessor)
{
   if (Decl->isSynthesized() && !IsFieldAccessor)
      return nullptr;

   auto Prop = PropDecl::Create(
       Context, Decl->getAccess(), Decl->getSourceRange(), Decl->getDeclName(),
       visit(Decl->getType()), Decl->isStatic(), Decl->isReadWrite(),
       copyOrNull(Decl->getGetterMethod()),
       copyOrNull(Decl->getSetterMethod()));

   Prop->setPropTemplate(Decl);
   return Prop;
}

SubscriptDecl* InstantiatorImpl::visitSubscriptDecl(SubscriptDecl* Decl)
{
   auto SD = SubscriptDecl::Create(
       Context, Decl->getAccess(), Decl->getSourceRange(),
       visit(Decl->getType()), copyOrNull(Decl->getGetterMethod()),
       copyOrNull(Decl->getSetterMethod()));

   SD->setTemplate(Decl);
   return SD;
}

MethodDecl* InstantiatorImpl::visitMethodDecl(MethodDecl* M)
{
   bool IsMainTemplate = M == Template;

   DeclarationName Name = M->getDeclName();
   if (M->isTemplate() && IsMainTemplate) {
      Name = Context.getDeclNameTable().getInstantiationName(
          M->getDeclName(), templateArgs.innermost());
   }

   SmallVector<FuncArgDecl*, 4> Args;
   copyArgListAndFindVariadic(M, Args);

   auto templateParams = copyTemplateParameters(M);

   MethodDecl* Inst;
   if (M->isConversionOp()) {
      Inst = MethodDecl::CreateConversionOp(
          Context, M->getAccess(), M->getDefLoc(), visit(M->getReturnType()),
          Args, move(templateParams), nullptr);
   }
   else {
      Inst = MethodDecl::Create(Context, M->getAccess(), M->getDefLoc(), Name,
                                visit(M->getReturnType()), Args,
                                move(templateParams), nullptr, M->isStatic());
   }

   Inst->setBodyTemplate(M);
   Inst->setMethodID(M->getMethodID());
   Inst->setFunctionFlags(M->getFunctionFlags());

   VariadicTemplateArgs.clear();
   return Inst;
}

AssociatedTypeDecl*
InstantiatorImpl::visitAssociatedTypeDecl(AssociatedTypeDecl* Decl)
{
   SourceType ActualType;
   if (Decl->getDeclName().isStr("Self") && Decl->isImplementation()) {
      ActualType = Context.getRecordType(InstantiatingRecord);
   }
   else {
      ActualType = visit(Decl->getActualType());
   }

   auto Inst = AssociatedTypeDecl::Create(Context, Decl->getSourceLoc(),
                                          Decl->getDeclName(), ActualType,
                                          visit(Decl->getCovariance()));

   Context.setConstraints(Inst, cloneVector(Decl->getConstraints()));
   return Inst;
}

TypedefDecl* InstantiatorImpl::visitTypedefDecl(TypedefDecl* TD)
{
   DeclarationName Name = TD->getDeclName();
   auto Inst = TypedefDecl::Create(Context, TD->getAccess(), TD->getSourceLoc(),
                                   Name, visit(TD->getOriginTy()),
                                   copyTemplateParameters(TD));

   return Inst;
}

AliasDecl* InstantiatorImpl::visitAliasDecl(AliasDecl* Alias)
{
   bool IsMainTemplate = Alias == Template;

   DeclarationName Name = Alias->getDeclName();
   if (Alias->isTemplate() && IsMainTemplate)
      Name = Context.getDeclNameTable().getInstantiationName(
          Alias->getDeclName(), templateArgs.innermost());

   auto Inst = AliasDecl::Create(
       Context, Alias->getSourceLoc(), Alias->getAccess(), Name,
       visitOrAuto(Alias->getType()), copyOrNull(Alias->getAliasExpr()),
       copyTemplateParameters(Alias));

   Context.setConstraints(Inst, cloneVector(Alias->getConstraints()));
   return Inst;
}

FieldDecl* InstantiatorImpl::visitFieldDecl(FieldDecl* Decl)
{
   // Make sure the default value was visited.
   if (SP.QC.TypecheckDecl(Decl))
      return nullptr;

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
                                 Decl->isStatic(), Decl->isConst(), nullptr);

   if (Decl->isInUnboundedTemplate() || !Decl->isStatic()) {
      Inst->setValue(copyOrNull(Decl->getDefaultVal()));
   }
   else {
      Inst->setValueTemplate(Decl);
      Inst->setShouldBeSpecialized(true);
   }

   if (auto Acc = Decl->getAccessor()) {
      PropDecl* AccInst = visitPropDecl(Acc, true);

      AccInst->setSynthesized(true);
      Inst->setAccessor(AccInst);
   }

   return Inst;
}

InitDecl* InstantiatorImpl::visitInitDecl(InitDecl* Decl)
{
   bool IsMainTemplate = Decl == Template;

   RecordDecl* R = InstantiatingRecord;
   if (!R) {
      R = Decl->getRecord();
   }

   DeclarationName Name = Context.getDeclNameTable().getConstructorName(
       Context.getRecordType(R));

   if (Decl->isTemplate() && IsMainTemplate)
      Name = Context.getDeclNameTable().getInstantiationName(
          Name, templateArgs.innermost());

   SmallVector<FuncArgDecl*, 4> Args;
   copyArgListAndFindVariadic(Decl, Args);

   auto templateParams = copyTemplateParameters(Decl);
   auto Inst = InitDecl::Create(
       Context, Decl->getAccess(), Decl->getSourceLoc(), Args,
       move(templateParams), nullptr, Name, Decl->isFallible());

   Inst->setBodyTemplate(Decl);
   Inst->setMethodID(Decl->getMethodID());
   Inst->setFunctionFlags(Decl->getFunctionFlags());

   return Inst;
}

DeinitDecl* InstantiatorImpl::visitDeinitDecl(DeinitDecl* Decl)
{
   DeclarationName Name = Context.getDeclNameTable().getDestructorName(
       Context.getRecordType(cast<RecordDecl>(&SP.getDeclContext())));

   auto Inst = DeinitDecl::Create(Context, Decl->getSourceLoc(), nullptr,
                                  cloneVector(Decl->getArgs()), Name);

   Inst->setMethodID(Decl->getMethodID());
   Inst->setBodyTemplate(Decl);
   Inst->setFunctionFlags(Decl->getFunctionFlags());

   return Inst;
}

FunctionDecl* InstantiatorImpl::visitFunctionDecl(FunctionDecl* F)
{
   bool IsMainTemplate = F == Template;

   DeclarationName Name = F->getDeclName();
   if (F->isTemplate() && IsMainTemplate)
      Name = Context.getDeclNameTable().getInstantiationName(
          Name, templateArgs.innermost());

   SmallVector<FuncArgDecl*, 4> Args;
   copyArgListAndFindVariadic(F, Args);

   auto templateParams = copyTemplateParameters(F);
   auto Inst = FunctionDecl::Create(Context, F->getAccess(), F->getDefLoc(),
                                    Name, Args, visit(F->getReturnType()),
                                    nullptr, move(templateParams));

   Inst->setBodyTemplate(F);
   Inst->setFunctionFlags(F->getFunctionFlags());
   VariadicTemplateArgs.clear();

   return Inst;
}

AttributedStmt* InstantiatorImpl::visitAttributedStmt(AttributedStmt* S)
{
   llvm::SmallVector<Attr*, 4> Attrs;
   for (auto& A : S->getAttributes())
      Attrs.push_back(A->clone(Context));

   return AttributedStmt::Create(Context, visit(S->getStatement()), Attrs);
}

AttributedExpr* InstantiatorImpl::visitAttributedExpr(AttributedExpr* E)
{
   llvm::SmallVector<Attr*, 4> Attrs;
   for (auto& A : E->getAttributes())
      Attrs.push_back(A->clone(Context));

   return AttributedExpr::Create(Context, visit(E->getExpr()), Attrs);
}

TupleTypeExpr* InstantiatorImpl::visitTupleTypeExpr(TupleTypeExpr* Expr)
{
   return TupleTypeExpr::Create(Context, Expr->getSourceRange(),
                                copyTypeList(Expr->getContainedTypes()),
                                Expr->isMeta());
}

FunctionTypeExpr*
InstantiatorImpl::visitFunctionTypeExpr(FunctionTypeExpr* Expr)
{
   SmallVector<SourceType, 4> Tys;
   SmallVector<FunctionType::ParamInfo, 4> Inf;

   copyParameterList(Expr->getArgTypes(), Expr->getParamInfo(), Tys, Inf);

   return FunctionTypeExpr::Create(
       Context, Expr->getSourceRange(), visit(Expr->getReturnType()), Tys, Inf,
       Expr->isMeta(), Expr->isThin(), Expr->throws(), Expr->isAsync(),
       Expr->isUnsafe());
}

ArrayTypeExpr* InstantiatorImpl::visitArrayTypeExpr(ArrayTypeExpr* Expr)
{
   return ArrayTypeExpr::Create(
       Context, Expr->getSourceRange(), visit(Expr->getElementTy()),
       clone<StaticExpr>(Expr->getSizeExpr()), Expr->isMeta());
}

DeclTypeExpr* InstantiatorImpl::visitDeclTypeExpr(DeclTypeExpr* Expr)
{
   return DeclTypeExpr::Create(Context, Expr->getSourceRange(),
                               visit(Expr->getTyExpr()), Expr->isMeta());
}

PointerTypeExpr* InstantiatorImpl::visitPointerTypeExpr(PointerTypeExpr* Expr)
{
   return PointerTypeExpr::Create(Context, Expr->getSourceRange(),
                                  visit(Expr->getSubType()), Expr->isMeta());
}

ReferenceTypeExpr*
InstantiatorImpl::visitReferenceTypeExpr(ReferenceTypeExpr* Expr)
{
   return ReferenceTypeExpr::Create(Context, Expr->getSourceRange(),
                                    visit(Expr->getSubType()), Expr->isMeta());
}

OptionTypeExpr* InstantiatorImpl::visitOptionTypeExpr(OptionTypeExpr* Expr)
{
   return OptionTypeExpr::Create(Context, Expr->getSourceRange(),
                                 visit(Expr->getSubType()), Expr->isMeta());
}

ExistentialTypeExpr*
InstantiatorImpl::visitExistentialTypeExpr(ExistentialTypeExpr* Expr)
{
   return ExistentialTypeExpr::Create(Context, Expr->getSourceRange(),
                                      copyTypeList(Expr->getExistentials()),
                                      Expr->isMeta());
}

Expression* InstantiatorImpl::visitImplicitCastExpr(ImplicitCastExpr* node)
{
   return visit(node->getTarget());
   //   return ImplicitCastExpr::Create(Context, visit(node->getTarget()),
   //                                   node->getConvSeq().copy());
}

CompoundDecl* InstantiatorImpl::visitCompoundDecl(CompoundDecl* D)
{
   auto Inst
       = CompoundDecl::Create(Context, D->getLBraceLoc(), D->isTransparent());

   SemaPass::DeclContextRAII declContextRAII(SP, Inst,
                                             SemaPass::SetParentContext);

   for (auto& decl : D->getDecls())
      visit(decl);

   Inst->setRBraceLoc(D->getRBraceLoc());
   return Inst;
}

CompoundStmt* InstantiatorImpl::visitCompoundStmt(CompoundStmt* node)
{
   SmallVector<Statement*, 8> Stmts;
   for (auto& Stmt : node->getStatements())
      Stmts.push_back(visit(Stmt));

   auto* CS = CompoundStmt::Create(Context, Stmts, node->preservesScope(),
                                   node->getLBraceLoc(), node->getRBraceLoc());

   CS->setScopeID(node->getScopeID());
   CS->setContainsDeclStmt(node->containsDeclStmt());

   return CS;
}

void InstantiatorImpl::copyArgListAndFindVariadic(
    CallableDecl* C, SmallVectorImpl<FuncArgDecl*>& Variadics)
{
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

template<class Container, class Expr>
std::vector<Expr*> InstantiatorImpl::copyExprList(const Container& exprList)
{
   std::vector<Expr*> list;
   for (const auto& expr : exprList)
      expandVariadicArgument(list, expr);

   return list;
}

void InstantiatorImpl::copyArgumentList(
    ArrayRef<Expression*> Args, ArrayRef<cdot::IdentifierInfo*> Labels,
    SmallVectorImpl<Expression*>& ResultArgs,
    SmallVectorImpl<IdentifierInfo*>& ResultLabels)
{
   unsigned i = 0;
   for (const auto& expr : Args) {
      IdentifierInfo* Lbl = nullptr;
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

void InstantiatorImpl::copyParameterList(
    ArrayRef<SourceType> Args, ArrayRef<FunctionType::ParamInfo> ParamInfo,
    SmallVectorImpl<SourceType>& ResultArgs,
    SmallVectorImpl<FunctionType::ParamInfo>& ResultInfo)
{
   SmallVector<Expression*, 4> Exprs;

   unsigned i = 0;
   for (const auto& Ty : Args) {
      FunctionType::ParamInfo PI;
      if (i < ParamInfo.size())
         PI = ParamInfo[i];

      unsigned PrevSize = (unsigned)ResultArgs.size();

      if (Ty.getTypeExpr() && !canElideInstantiation(Ty.getTypeExpr())) {
         expandVariadicArgument(Exprs, Ty.getTypeExpr());
         for (auto& E : Exprs)
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

   for (const auto& Ty : Tys) {
      if (Ty.getTypeExpr() && !canElideInstantiation(Ty.getTypeExpr())) {
         expandVariadicArgument(Exprs, Ty.getTypeExpr());
         for (auto& E : Exprs)
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

   for (const auto& Ty : Tys) {
      if (Ty.getTypeExpr() && !canElideInstantiation(Ty.getTypeExpr())) {
         expandVariadicArgument(Exprs, Ty.getTypeExpr());
         for (auto& E : Exprs)
            list.push_back(SourceType(E), Context);

         Exprs.clear();
      }
      else {
         list.push_back(Ty, Context);
      }
   }

   return list;
}

FuncArgDecl* InstantiatorImpl::visitFuncArgDecl(FuncArgDecl* Decl)
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

   auto* Inst = FuncArgDecl::Create(
       Context, Decl->getOwnershipLoc(), Decl->getColonLoc(), DeclName,
       Decl->getLabel(), Decl->getConvention(), visit(Decl->getType()),
       copyOrNull(Decl->getDefaultVal()),
       Decl->isVariadicArgPackExpansion() && !IsSubstitution,
       Decl->isCstyleVararg(), Decl->isSelf());

   return Inst;
}

IntegerLiteral* InstantiatorImpl::visitIntegerLiteral(IntegerLiteral* node)
{
   return IntegerLiteral::Create(Context, node->getSourceRange(),
                                 node->getType(), node->getValue(),
                                 node->getSuffix());
}

FPLiteral* InstantiatorImpl::visitFPLiteral(FPLiteral* node)
{
   return FPLiteral::Create(Context, node->getSourceRange(), node->getType(),
                            node->getValue(), node->getSuffix());
}

BoolLiteral* InstantiatorImpl::visitBoolLiteral(BoolLiteral* node)
{
   return BoolLiteral::Create(Context, node->getSourceLoc(), node->getType(),
                              node->getValue());
}

CharLiteral* InstantiatorImpl::visitCharLiteral(CharLiteral* node)
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

SelfExpr* InstantiatorImpl::visitSelfExpr(SelfExpr* node)
{
   return SelfExpr::Create(Context, node->getSourceLoc(), node->isUppercase());
}

SuperExpr* InstantiatorImpl::visitSuperExpr(SuperExpr* node)
{
   return SuperExpr::Create(Context, node->getSourceLoc());
}

DebugStmt* InstantiatorImpl::visitDebugStmt(DebugStmt* node)
{
   if (!node->isUnreachable()) {
      int i = 3;
      (void)i;
   }

   return new (Context) DebugStmt(node->getSourceLoc(), node->isUnreachable());
}

DebugDecl* InstantiatorImpl::visitDebugDecl(DebugDecl* D)
{
   return DebugDecl::Create(Context, D->getLoc());
}

NoneLiteral* InstantiatorImpl::visitNoneLiteral(NoneLiteral* node)
{
   return NoneLiteral::Create(Context, node->getSourceLoc());
}

StringInterpolation*
InstantiatorImpl::visitStringInterpolation(StringInterpolation* node)
{
   return StringInterpolation::Create(Context, node->getSourceRange(),
                                      cloneVector(node->getSegments()));
}

StringLiteral* InstantiatorImpl::visitStringLiteral(StringLiteral* node)
{
   return StringLiteral::Create(Context, node->getSourceRange(),
                                node->getValue().str());
}

TupleLiteral* InstantiatorImpl::visitTupleLiteral(TupleLiteral* node)
{
   return TupleLiteral::Create(Context, node->getSourceRange(),
                               copyExprList(node->getElements()));
}

LocalVarDecl* InstantiatorImpl::visitLocalVarDecl(LocalVarDecl* node)
{
   SourceType T;
   if (!node->hasInferredType()) {
      T = visit(node->getType());
   }
   else {
      T = SourceType(Context.getAutoType());
   }

   auto Decl = LocalVarDecl::Create(
       Context, node->getAccess(), node->getVarOrLetLoc(), node->getColonLoc(),
       node->isConst(), node->getDeclName(), T, copyOrNull(node->getValue()));

   Decl->setLexicalContext(&SP.getDeclContext());
   return Decl;
}

GlobalVarDecl* InstantiatorImpl::visitGlobalVarDecl(GlobalVarDecl* node)
{
   auto Decl = GlobalVarDecl::Create(
       Context, node->getAccess(), node->getVarOrLetLoc(), node->getColonLoc(),
       node->isConst(), node->getDeclName(), visitOrAuto(node->getType()),
       copyOrNull(node->getValue()));

   return Decl;
}

DestructuringDecl*
InstantiatorImpl::visitDestructuringDecl(DestructuringDecl* Decl)
{
   auto* Inst = DestructuringDecl::Create(
       Context, Decl->getParenRange(), cloneVector(Decl->getDecls()),
       visit(Decl->getType()), visit(Decl->getValue()));

   return Inst;
}

Expression* InstantiatorImpl::visitBuiltinExpr(BuiltinExpr* node)
{
   return BuiltinExpr::Create(Context, visit(node->getType()));
}

Expression* InstantiatorImpl::visitParenExpr(ParenExpr* Expr)
{
   // variadic tuple
   if (auto ExprSeq = dyn_cast<ExprSequence>(Expr->getParenthesizedExpr())) {
      if (ExprSeq->getFragments().size() == 1
          && ExprSeq->getFragments()
                 .front()
                 .getExpr()
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

Expression* InstantiatorImpl::makeLiteralExpr(Expression* Expr, QualType valTy,
                                              const Variant& Val)
{
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

Expression* InstantiatorImpl::visitIdentifierRefExpr(IdentifierRefExpr* Ident)
{
   if (!Ident->getParentExpr()) {
      if (auto Arg = hasTemplateArg(Ident->getDeclName())) {
         if (Arg->isVariadic()) {
            auto Subst = getParameterSubst(Ident->getDeclName());

            assert(Subst && "didn't diagnose unexpanded parameter pack!");

            return new (Context) IdentifierRefExpr(
                Ident->getSourceLoc(), IdentifierKind::MetaType,
                getContext().getMetaType(Subst->getType()));
         }

         if (Arg->isType()) {
            return new (Context) IdentifierRefExpr(
                Ident->getSourceLoc(), IdentifierKind::MetaType,
                getContext().getMetaType(Arg->getType()));
         }

         return StaticExpr::Create(Context, Arg->getParam()->getValueType(),
                                   Ident->getSourceRange(), Arg->getValue());
      }

      if (ValueSubst.Name == Ident->getIdentInfo()) {
         return StaticExpr::Create(Context, ValueSubst.SubstVal->getType(),
                                   Ident->getSourceRange(),
                                   ValueSubst.SubstVal);
      }
   }

   DeclarationName DeclName;
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

   if (!DeclName)
      DeclName = Ident->getDeclName();

   auto Inst = new (Context)
       IdentifierRefExpr(Ident->getSourceLoc(), DeclName, nullptr);

   Inst->setSelf(Ident->isSelf());
   Inst->setParentExpr(copyOrNull(Ident->getParentExpr()));
   Inst->setIsPointerAccess(Ident->isPointerAccess());
   Inst->setAllowNamespaceRef(Ident->allowNamespaceRef());
   Inst->setLeadingDot(Ident->hasLeadingDot());

   auto* NameSpec = Ident->getNameSpec();
   if (NameSpec && Ident->isInTypePosition()) {
      QualType T = Context.getDependentNameType(NameSpec);
      T = visit(SourceType(T));

      if (auto* DN = dyn_cast<DependentNameType>(T)) {
         Ident->setNameSpec(DN->getNameSpecWithLoc());
      }
      else {
         Ident->setNameSpec(NameSpec);
      }
   }

   return Inst;
}

BuiltinIdentExpr*
InstantiatorImpl::visitBuiltinIdentExpr(BuiltinIdentExpr* node)
{
   return BuiltinIdentExpr::Create(Context, node->getSourceLoc(),
                                   node->getIdentifierKind());
}

EnumCaseExpr* InstantiatorImpl::visitEnumCaseExpr(EnumCaseExpr* node)
{
   auto Inst
       = new (Context) EnumCaseExpr(node->getSourceLoc(), node->getIdentInfo(),
                                    astvec(copyExprList(node->getArgs())));

   if (!node->isTypeDependent()) {
      Inst->setCase(node->getCase());
   }

   return Inst;
}

Expression* InstantiatorImpl::visitCallExpr(CallExpr* node)
{
   DeclContext* Ctx = node->getContext();
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

   DeclarationName FuncName = node->getDeclName();
   if (auto Arg = hasTemplateArg(FuncName)) {
      if (Arg->isVariadic()) {
         auto Subst = getParameterSubst(FuncName);
         assert(Subst && "didn't diagnose unexpanded parameter pack!");

         Arg = Subst;
      }

      if (!Arg->isType()) {
         auto* SE = StaticExpr::Create(Context, Arg->getParam()->getValueType(),
                                       node->getSourceRange(), Arg->getValue());

         return AnonymousCallExpr::Create(Context, node->getParenRange(), SE,
                                          ArgVec, Labels);
      }

      QualType Ty = Arg->getType();
      if (true || !Ty->isRecordType()) {
         auto* IE = new (Context)
             IdentifierRefExpr(node->getSourceRange(), IdentifierKind::MetaType,
                               Context.getMetaType(Ty));

         return AnonymousCallExpr::Create(Context, node->getParenRange(), IE,
                                          ArgVec, Labels);
      }

      FuncName = Context.getDeclNameTable().getConstructorName(Ty);
   }

   auto call = CallExpr::Create(
       Context, node->getIdentLoc(), node->getParenRange(),
       copyOrNull(node->getParentExpr()), astvec(ArgVec), Labels, FuncName,
       node->isDotInit(), node->isDotDeinit());

   call->setContext(Ctx);
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

Expression* InstantiatorImpl::visitAnonymousCallExpr(AnonymousCallExpr* Expr)
{
   SmallVector<Expression*, 4> ArgVec;
   ArgVec.reserve(Expr->getArgs().size());

   SmallVector<IdentifierInfo*, 4> Labels;
   Labels.reserve(Expr->getLabels().size());

   copyArgumentList(Expr->getArgs(), Expr->getLabels(), ArgVec, Labels);

   return AnonymousCallExpr::Create(Context, Expr->getParenRange(),
                                    visit(Expr->getParentExpr()), ArgVec,
                                    Labels);
}

SubscriptExpr* InstantiatorImpl::visitSubscriptExpr(SubscriptExpr* node)
{
   return SubscriptExpr::Create(Context, node->getSourceRange(),
                                copyOrNull(node->getParentExpr()),
                                copyExprList(node->getIndices()));
}

TupleMemberExpr* InstantiatorImpl::visitTupleMemberExpr(TupleMemberExpr* node)
{
   return new (Context)
       TupleMemberExpr(node->getSourceLoc(), copyOrNull(node->getParentExpr()),
                       node->getIndex(), node->isPointerAccess());
}

TemplateArgListExpr*
InstantiatorImpl::visitTemplateArgListExpr(TemplateArgListExpr* Expr)
{
   return TemplateArgListExpr::Create(Context, Expr->getSourceRange(),
                                      visit(Expr->getParentExpr()),
                                      copyExprList(Expr->getExprs()));
}

ReturnStmt* InstantiatorImpl::visitReturnStmt(ReturnStmt* node)
{
   if (auto Val = node->getReturnValue()) {
      return ReturnStmt::Create(Context, node->getSourceLoc(), visit(Val));
   }

   return ReturnStmt::Create(Context, node->getSourceLoc());
}

DiscardAssignStmt*
InstantiatorImpl::visitDiscardAssignStmt(DiscardAssignStmt* Stmt)
{
   return DiscardAssignStmt::Create(Context, Stmt->getUnderscoreLoc(),
                                    Stmt->getEqualsLoc(),
                                    visit(Stmt->getRHS()));
}

UnaryOperator* InstantiatorImpl::visitUnaryOperator(UnaryOperator* node)
{
   return UnaryOperator::Create(Context, node->getSourceRange().getStart(),
                                node->getKind(), node->getFunctionType(),
                                visit(node->getTarget()), node->isPrefix());
}

Expression* InstantiatorImpl::visitBinaryOperator(BinaryOperator* node)
{
   if (auto Ident = dyn_cast<IdentifierRefExpr>(node->getRhs())) {
      if (Ident->getIdent() == "...") {
         llvm::SmallVector<Expression*, 4> exprs;
         expandVariadicArgument(exprs, node->getLhs());

         if (exprs.empty()) {
            SP.diagnose(
                node, err_generic_error,
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
            auto binOp = BinaryOperator::Create(
                Context, node->getSourceRange().getStart(), node->getKind(),
                node->getFunctionType(), exprs[0], exprs[1]);

            while (i < numExprs) {
               binOp = BinaryOperator::Create(
                   Context, node->getSourceRange().getStart(), node->getKind(),
                   node->getFunctionType(), binOp, exprs[i]);

               ++i;
            }

            return binOp;
         }
      }
   }

end:
   return BinaryOperator::Create(Context, node->getSourceRange().getStart(),
                                 node->getKind(), node->getFunctionType(),
                                 visit(node->getLhs()), visit(node->getRhs()));
}

Expression* InstantiatorImpl::visitAssignExpr(AssignExpr* Expr)
{
   return AssignExpr::Create(Context, Expr->getEqualsLoc(),
                             visit(Expr->getLhs()), visit(Expr->getRhs()),
                             Expr->isInitialization());
}

CastExpr* InstantiatorImpl::visitCastExpr(CastExpr* Expr)
{
   return CastExpr::Create(Context, Expr->getAsLoc(), Expr->getStrength(),
                           visit(Expr->getTarget()),
                           visit(Expr->getTargetType()));
}

AddrOfExpr* InstantiatorImpl::visitAddrOfExpr(AddrOfExpr* Expr)
{
   return AddrOfExpr::Create(Context, Expr->getAmpLoc(),
                             visit(Expr->getTarget()));
}

TypePredicateExpr*
InstantiatorImpl::visitTypePredicateExpr(TypePredicateExpr* Expr)
{
   return TypePredicateExpr::Create(
       Context, Expr->getIsLoc(), Expr->getSourceRange(), visit(Expr->getLHS()),
       clone(Expr->getRHS()), Expr->isNegated());
}

Expression* InstantiatorImpl::visitExprSequence(ExprSequence* node)
{
   std::vector<SequenceElement> fragments;
   for (const auto& El : node->getFragments()) {
      switch (El.getKind()) {
      case SequenceElement::EF_Operator:
         fragments.emplace_back(El.getOperatorKind(), El.getWhitespace(),
                                El.getLoc());
         break;
      case SequenceElement::EF_PossibleOperator: {
         auto maybeIdent
             = new (Context) IdentifierRefExpr(El.getLoc(), El.getOp());

         auto expr = visitIdentifierRefExpr(maybeIdent);

         auto ident = dyn_cast<IdentifierRefExpr>(expr);
         if (!ident || ident->getIdentInfo() != El.getOp())
            fragments.emplace_back(expr);
         else
            fragments.emplace_back(El.getOp(), El.getWhitespace(), El.getLoc());

         break;
      }
      case SequenceElement::EF_Expression:
         fragments.emplace_back(visit(El.getExpr()));
         break;
      }
   }

   return ExprSequence::Create(Context, fragments);
}

IfExpr* InstantiatorImpl::visitIfExpr(IfExpr* node)
{
   return IfExpr::Create(Context, node->getIfLoc(),
                         visitIfCondition(node->getCond()).ExprData.Expr,
                         visit(node->getTrueVal()), visit(node->getFalseVal()));
}

LambdaExpr* InstantiatorImpl::visitLambdaExpr(LambdaExpr* node)
{
   auto* E = LambdaExpr::Create(
       Context, node->getParenRange(), node->getArrowLoc(),
       visitOrAuto(node->getReturnType()), cloneVector(node->getArgs()),
       visit(node->getBody()));

   auto* II = &Context.getIdentifiers().get("__anonymous_lambda");
   auto Fun = FunctionDecl::Create(Context, AccessSpecifier::Private,
                                   E->getSourceLoc(), II, E->getArgs(),
                                   E->getReturnType(), E->getBody(), {});

   Fun->setIsLambda(true);
   Fun->setExternC(true);
   Fun->setDeclared(true);
   Fun->setSynthesized(true);
   Fun->setSemanticallyChecked(true);

   E->setFunc(Fun);
   ActOnDecl(Fun);

   return E;
}

DictionaryLiteral*
InstantiatorImpl::visitDictionaryLiteral(DictionaryLiteral* node)
{
   return DictionaryLiteral::Create(Context, node->getSourceRange(),
                                    cloneVector(node->getKeys()),
                                    cloneVector(node->getValues()));
}

ArrayLiteral* InstantiatorImpl::visitArrayLiteral(ArrayLiteral* node)
{
   return ArrayLiteral::Create(Context, node->getSourceRange(),
                               cloneVector(node->getValues()));
}

IfCondition InstantiatorImpl::visitIfCondition(const IfCondition& C)
{
   switch (C.K) {
   case IfCondition::Expression:
      return IfCondition(visit(C.ExprData.Expr));
   case IfCondition::Binding:
      return IfCondition(clone(C.BindingData.Decl));
   case IfCondition::Pattern:
      return IfCondition(clone(C.PatternData.Pattern),
                         copyOrNull(C.PatternData.Expr));
   }
}

IfStmt* InstantiatorImpl::visitIfStmt(IfStmt* node)
{
   SmallVector<IfCondition, 2> Conditions;
   for (auto& C : node->getConditions()) {
      Conditions.push_back(visitIfCondition(C));
   }

   return IfStmt::Create(Context, node->getSourceLoc(), Conditions,
                         visit(node->getIfBranch()),
                         copyOrNull(node->getElseBranch()), node->getLabel());
}

WhileStmt* InstantiatorImpl::visitWhileStmt(WhileStmt* node)
{
   SmallVector<IfCondition, 2> Conditions;
   for (auto& C : node->getConditions()) {
      Conditions.push_back(visitIfCondition(C));
   }

   return WhileStmt::Create(Context, node->getSourceLoc(), Conditions,
                            visit(node->getBody()), node->getLabel(),
                            node->isAtLeastOnce());
}

ForStmt* InstantiatorImpl::visitForStmt(ForStmt* node)
{
   return ForStmt::Create(
       Context, node->getSourceLoc(), copyOrNull(node->getInitialization()),
       copyOrNull(node->getTermination()), copyOrNull(node->getIncrement()),
       copyOrNull(node->getBody()), node->getLabel());
}

ForInStmt* InstantiatorImpl::visitForInStmt(ForInStmt* node)
{
   return ForInStmt::Create(
       Context, node->getSourceLoc(), copyOrNull(node->getDecl()),
       visit(node->getRangeExpr()), visit(node->getBody()), node->getLabel());
}

MatchStmt* InstantiatorImpl::visitMatchStmt(MatchStmt* node)
{
   return MatchStmt::Create(Context, node->getMatchLoc(), node->getBraceRange(),
                            visit(node->getSwitchValue()),
                            cloneVector(node->getCases()), node->getLabel());
}

CaseStmt* InstantiatorImpl::visitCaseStmt(CaseStmt* node)
{
   return CaseStmt::Create(Context, node->getSourceLoc(),
                           copyOrNull(node->getPattern()),
                           copyOrNull(node->getBody()));
}

ExpressionPattern*
InstantiatorImpl::visitExpressionPattern(ExpressionPattern* node)
{
   return ExpressionPattern::Create(Context, node->getSourceLoc(),
                                    visit(node->getExpr()));
}

IsPattern* InstantiatorImpl::visitIsPattern(IsPattern* node)
{
   return IsPattern::Create(Context, node->getSourceRange(),
                            visit(node->getIsType()));
}

CasePattern* InstantiatorImpl::visitCasePattern(CasePattern* node)
{
   SmallVector<IfCondition, 4> Args;
   for (auto& Arg : node->getArgs()) {
      Args.push_back(visitIfCondition(Arg));
   }

   auto* Inst = CasePattern::Create(
       Context, node->getSourceRange(), node->getKind(),
       copyOrNull(node->getParentExpr()), node->getCaseNameIdent(), Args);

   Inst->setLeadingDot(node->hasLeadingDot());
   return Inst;
}

DoStmt* InstantiatorImpl::visitDoStmt(DoStmt* node)
{
   SmallVector<CatchBlock, 2> catchBlocks;
   catchBlocks.reserve(node->getCatchBlocks().size());

   for (const auto& CB : node->getCatchBlocks()) {
      catchBlocks.emplace_back(visitLocalVarDecl(CB.varDecl), visit(CB.Body));
   }

   return new (Context) DoStmt(node->getSourceRange(), visit(node->getBody()),
                               catchBlocks, node->getLabel());
}

TryExpr* InstantiatorImpl::visitTryExpr(TryExpr* Expr)
{
   return TryExpr::Create(Context, Expr->getSourceLoc(), Expr->getKind(),
                          visit(Expr->getExpr()));
}

AwaitExpr* InstantiatorImpl::visitAwaitExpr(AwaitExpr* Expr)
{
   return AwaitExpr::Create(Context, Expr->getAwaitLoc(),
                            visit(Expr->getExpr()));
}

ThrowStmt* InstantiatorImpl::visitThrowStmt(ThrowStmt* node)
{
   return new (Context)
       ThrowStmt(node->getSourceLoc(), visit(node->getThrownVal()));
}

UsingDecl* InstantiatorImpl::visitUsingDecl(UsingDecl* node)
{
   return UsingDecl::Create(Context, node->getSourceRange(), node->getAccess(),
                            node->getDeclName(), node->getNestedImportName(),
                            node->isWildcardImport());
}

ImportDecl* InstantiatorImpl::visitImportDecl(ImportDecl* node)
{
   return ImportDecl::Create(Context, node->getSourceRange(), node->getAccess(),
                             node->getQualifiedImportName(),
                             node->getNamedImports(), node->isWildcardImport());
}

UnittestDecl* InstantiatorImpl::visitUnittestDecl(UnittestDecl* D)
{
   return UnittestDecl::Create(Context, D->getKeywordLoc(), D->getBraceRange(),
                               D->getName(), visit(D->getBody()));
}

DeclStmt* InstantiatorImpl::visitDeclStmt(DeclStmt* Stmt)
{
   auto* D = visit(Stmt->getDecl());
   ActOnDecl(D);

   return DeclStmt::Create(Context, D);
}

StaticExpr* InstantiatorImpl::visitStaticExpr(StaticExpr* node)
{
   return StaticExpr::Create(Context, visit(node->getExpr()));
}

ConstraintExpr* InstantiatorImpl::visitConstraintExpr(ConstraintExpr* node)
{
   if (node->getKind() == ConstraintExpr::Type)
      return ConstraintExpr::Create(Context, node->getSourceLoc(),
                                    visit(node->getTypeConstraint()));

   return ConstraintExpr::Create(Context, node->getSourceLoc(),
                                 node->getKind());
}

TraitsExpr* InstantiatorImpl::visitTraitsExpr(TraitsExpr* node)
{
   std::vector<TraitsArgument> args;
   for (const auto& arg : node->getArgs()) {
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
InstantiatorImpl::visitMacroExpansionExpr(MacroExpansionExpr* Expr)
{
   return MacroExpansionExpr::Create(
       Context, Expr->getSourceRange(), Expr->getMacroName(), Expr->getDelim(),
       Expr->getTokens(), copyOrNull(Expr->getParentExpr()));
}

MacroExpansionStmt*
InstantiatorImpl::visitMacroExpansionStmt(MacroExpansionStmt* Stmt)
{
   return MacroExpansionStmt::Create(
       Context, Stmt->getSourceRange(), Stmt->getMacroName(),
       copyOrNull(Stmt->getParentExpr()), Stmt->getDelim(), Stmt->getTokens());
}

MacroExpansionDecl*
InstantiatorImpl::visitMacroExpansionDecl(MacroExpansionDecl* Decl)
{
   return MacroExpansionDecl::Create(
       Context, Decl->getSourceRange(), Decl->getMacroName(),
       copyOrNull(Decl->getParentExpr()), Decl->getDelim(), Decl->getTokens());
}

MixinExpr* InstantiatorImpl::visitMixinExpr(MixinExpr* Expr)
{
   return MixinExpr::Create(Context, Expr->getSourceRange(),
                            visit(Expr->getMixinExpr()));
}

MixinStmt* InstantiatorImpl::visitMixinStmt(MixinStmt* Stmt)
{
   return MixinStmt::Create(Context, Stmt->getSourceRange(),
                            visit(Stmt->getMixinExpr()));
}

MixinDecl* InstantiatorImpl::visitMixinDecl(MixinDecl* Decl)
{
   auto Inst
       = MixinDecl::Create(Context, Decl->getMixinLoc(), Decl->getParenRange(),
                           visit(Decl->getMixinExpr()));

   return Inst;
}

Decl* InstantiatorImpl::visitStaticIfDecl(StaticIfDecl* node)
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

   StaticIfDecl* Template = node;
   if (auto PrevTemplate = node->getTemplate())
      Template = PrevTemplate;

   auto Decl = StaticIfDecl::Create(Context, node->getStaticLoc(),
                                    node->getRBRaceLoc(),
                                    clone(node->getCondition()), Template);

   return Decl;
}

Decl* InstantiatorImpl::visitStaticForDecl(StaticForDecl* node)
{
   auto Decl = StaticForDecl::Create(
       Context, node->getStaticLoc(), node->getRBRaceLoc(),
       node->getElementName(), clone(node->getRange()),
       copyOrNull(node->getBodyDecl()));

   return Decl;
}

Statement* InstantiatorImpl::visitStaticIfStmt(StaticIfStmt* node)
{
   if (!node->getCondition()->isDependent()) {
      if (cast<il::ConstantInt>(node->getCondition()->getEvaluatedExpr())
              ->getBoolValue()) {
         return visit(node->getIfBranch());
      }

      return visit(node->getElseBranch());
   }

   StaticIfStmt* Template = node;
   if (auto PrevTemplate = node->getTemplate())
      Template = PrevTemplate;

   return StaticIfStmt::Create(Context, node->getStaticLoc(), node->getIfLoc(),
                               visitStaticExpr(node->getCondition()), Template);
}

Statement* InstantiatorImpl::visitStaticForStmt(StaticForStmt* node)
{
   return StaticForStmt::Create(
       Context, node->getStaticLoc(), node->getForLoc(), node->getElementName(),
       visitStaticExpr(node->getRange()), visit(node->getBody()));
}

StaticAssertDecl*
InstantiatorImpl::visitStaticAssertDecl(StaticAssertDecl* node)
{
   auto Decl = StaticAssertDecl::Create(
       Context, node->getStaticAssertLoc(), node->getParenRange(),
       visitStaticExpr(node->getExpr()), node->getMessage());

   return Decl;
}

StaticPrintDecl* InstantiatorImpl::visitStaticPrintDecl(StaticPrintDecl* node)
{
   auto Decl
       = StaticPrintDecl::Create(Context, node->getStaticPrintLoc(),
                                 node->getParenRange(), visit(node->getExpr()));

   return Decl;
}

static FinalTemplateArgumentList* MakeList(SemaPass& SP,
                                           const TemplateArgList& list)
{
   return FinalTemplateArgumentList::Create(SP.getContext(), list);
}

template<class T>
T* lookupExternalInstantiation(StringRef MangledName, ModuleDecl* Mod)
{
   if (auto MF = Mod->getModFile()) {
      auto Inst = MF->LookupInstantiation(MangledName);
      if (Inst)
         return cast<T>(Inst);
   }

   for (auto* Import : Mod->getImportedModules()) {
      auto* BaseMod = Import->getBaseModule()->getDecl();
      if (BaseMod == Mod)
         continue;

      if (auto Inst = lookupExternalInstantiation<T>(MangledName, BaseMod))
         return cast<T>(Inst);
   }

   return nullptr;
}

template<class T>
T* lookupExternalInstantiation(StringRef MangledName, SemaPass& SP)
{
   auto* Mod = SP.getDeclContext().getDeclModule();
   return lookupExternalInstantiation<T>(
       MangledName, Mod->getBaseModule()->getModule()->getDecl());
}

bool TemplateInstantiator::checkInstantiationDepth(NamedDecl* Inst,
                                                   NamedDecl* CurDecl,
                                                   SourceLocation POI)
{
   auto It = InstantiationDepthMap.find(CurDecl);
   if (It == InstantiationDepthMap.end()) {
      InstantiationDepthMap[Inst] = 0;
      return false;
   }

   if (It->getSecond() >= 512) {
      SemaPass::DeclScopeRAII DSR(SP, cast<DeclContext>(CurDecl));
      SP.diagnose(Inst, err_generic_error,
                  "maximum template instantiation depth (512) exceeded", POI);

      return true;
   }

   InstantiationDepthMap[Inst] = It->getSecond() + 1;
   return false;
}

unsigned TemplateInstantiator::getInstantiationDepth(NamedDecl* Decl)
{
   auto It = InstantiationDepthMap.find(Decl);
   if (It == InstantiationDepthMap.end()) {
      return 0;
   }

   return It->getSecond();
}

void TemplateInstantiator::setInstantiationDepth(NamedDecl* Decl,
                                                 unsigned Depth)
{
   InstantiationDepthMap[Decl] = Depth;
}

static unsigned getDepth(const TemplateArgument& Arg)
{
   if (!Arg.isType()) {
      return 0;
   }

   if (Arg.isVariadic()) {
      unsigned Max = 0;
      for (auto& VA : Arg.getVariadicArgs()) {
         unsigned Depth = getDepth(VA);
         if (Depth > Max) {
            Max = Depth;
         }
      }

      return Max;
   }

   QualType T = Arg.getType()->removeReference()->removeMetaType();
   if (!T->isRecordType()) {
      return 0;
   }

   auto* R = T->getRecord();
   if (!R->isInstantiation()) {
      return 0;
   }

   return R->getInstantiationInfo()->Depth + 1;
}

static unsigned getInstantiationDepth(NamedDecl* Template,
                                      FinalTemplateArgumentList* TemplateArgs)
{
   unsigned Max = 0;
   for (auto& Arg : *TemplateArgs) {
      unsigned Depth = getDepth(Arg);
      if (Depth > Max) {
         Max = Depth;
      }
   }

   return Max;
}

static bool checkDepth(SemaPass& SP, SourceLocation POI, unsigned Depth)
{
   if (Depth > SP.getCompilationUnit().getOptions().MaxInstantiationDepth) {
      SP.diagnose(err_generic_error,
                  "maximum template instantiation depth exceeded", POI);

      return true;
   }

   return false;
}

static DeclarationName getNameFor(ASTContext& C, NamedDecl* D, RecordDecl* Inst)
{
   auto DN = D->getDeclName();
   switch (DN.getKind()) {
   case DeclarationName::ConstructorName:
   case DeclarationName::BaseConstructorName:
      return C.getDeclNameTable().getConstructorName(
          C.getRecordType(Inst),
          DN.getKind() == DeclarationName::ConstructorName);
   case DeclarationName::DestructorName:
      return C.getDeclNameTable().getDestructorName(C.getRecordType(Inst));
   default:
      return DN;
   }
}

static void updateSpecialFunction(QueryContext& QC, NamedDecl* D,
                                  NamedDecl* MemberInst, RecordDecl* Template,
                                  RecordDecl* Inst,
                                  RecordMetaInfo& TemplateInfo,
                                  RecordMetaInfo& InstInfo)
{
   auto* M = dyn_cast<MethodDecl>(D);
   if (!M)
      return;

   if (isa<DeinitDecl>(M)) {
      Inst->setDeinitializer(cast<DeinitDecl>(MemberInst));
   }
   else if (isa<InitDecl>(M) && M->getArgs().empty() && isa<StructDecl>(Inst)) {
      cast<StructDecl>(Inst)->setParameterlessConstructor(
          cast<InitDecl>(MemberInst));
   }
   else if (M->isMemberwiseInitializer()) {
      cast<StructDecl>(Inst)->setMemberwiseInitializer(
          cast<InitDecl>(MemberInst));
   }
   else if (isa<StructDecl>(Inst)
            && M == cast<StructDecl>(Inst)->getDefaultInitializer()) {
      cast<StructDecl>(Inst)->setDefaultInitializer(
          cast<MethodDecl>(MemberInst));
   }

   if (TemplateInfo.IsImplicitlyCopyable && M == TemplateInfo.CopyFn) {
      InstInfo.IsImplicitlyCopyable = true;
      InstInfo.CopyFn = cast<MethodDecl>(MemberInst);
   }
   else if (TemplateInfo.IsImplicitlyEquatable
            && M == TemplateInfo.OperatorEquals) {
      InstInfo.IsImplicitlyEquatable = true;
      InstInfo.OperatorEquals = cast<MethodDecl>(MemberInst);
   }
   else if (TemplateInfo.IsImplicitlyHashable && M == TemplateInfo.HashCodeFn) {
      InstInfo.IsImplicitlyHashable = true;
      InstInfo.HashCodeFn = cast<MethodDecl>(MemberInst);
   }
   else if (TemplateInfo.IsImplicitlyStringRepresentable
            && M == TemplateInfo.ToStringFn) {
      InstInfo.IsImplicitlyStringRepresentable = true;
      InstInfo.ToStringFn = cast<MethodDecl>(MemberInst);
   }
}

QueryResult PrepareDeclForInstantiationQuery::run()
{
   switch (ND->getKind()) {
   case Decl::StructDeclID:
   case Decl::ClassDeclID:
   case Decl::EnumDeclID:
      if (auto Err = QC.PrepareNameLookup(cast<DeclContext>(ND))) {
         return Query::finish(Err);
      }

      break;
   case Decl::FunctionDeclID:
   case Decl::MethodDeclID:
   case Decl::InitDeclID:
   case Decl::DeinitDeclID:
      if (auto Err = QC.PrepareDeclInterface(ND)) {
         return Query::finish(Err);
      }

      // Make sure default values are instantiated.
      for (auto* Arg : cast<CallableDecl>(ND)->getArgs()) {
         if (auto Err = QC.TypecheckDecl(Arg)) {
            return Query::finish(Err);
         }
      }

      break;
   case Decl::PropDeclID:
      if (auto Err = QC.PrepareDeclInterface(ND)) {
         return Query::finish(Err);
      }

      if (auto* G = cast<PropDecl>(ND)->getGetterMethod()) {
         if (auto Err = QC.PrepareDeclForInstantiation(G)) {
            return Query::finish(Err);
         }
      }

      if (auto* S = cast<PropDecl>(ND)->getSetterMethod()) {
         if (auto Err = QC.PrepareDeclForInstantiation(S)) {
            return Query::finish(Err);
         }
      }

      break;
   case Decl::SubscriptDeclID:
      if (auto Err = QC.PrepareDeclInterface(ND)) {
         return Query::finish(Err);
      }

      if (auto* G = cast<SubscriptDecl>(ND)->getGetterMethod()) {
         if (auto Err = QC.PrepareDeclForInstantiation(G)) {
            return Query::finish(Err);
         }
      }

      if (auto* S = cast<SubscriptDecl>(ND)->getSetterMethod()) {
         if (auto Err = QC.PrepareDeclForInstantiation(S)) {
            return Query::finish(Err);
         }
      }

      break;
   default:
      if (auto Err = QC.TypecheckDecl(ND)) {
         return Query::finish(Err);
      }

      break;
   }

   if (ND->isInvalid()) {
      return fail();
   }

   return finish();
}

QueryResult InstantiateRecordQuery::run()
{
   if (QC.PrepareDeclForInstantiation(Template)) {
      return fail();
   }

   auto& SP = sema();

   // Check if this instantiation exists in an external module.
   if (auto* MF = Template->getModFile()) {
      MF->LoadAllDecls(*Template);
   }

   std::string MangledName;
   {
      llvm::raw_string_ostream OS(MangledName);
      SP.getMangler().manglePrefix(Template, *TemplateArgs, OS);
   }

   if (auto Inst = lookupExternalInstantiation<RecordDecl>(MangledName, SP)) {
      Inst->setImportedInstantiation(true);
      return finish(Inst);
   }

   unsigned Depth = getInstantiationDepth(Template, TemplateArgs);
   if (checkDepth(SP, PointOfInstantiation, Depth)) {
      return fail();
   }

   // Instantiate the interface only for now.
   InstantiatorImpl Instantiator(SP, *TemplateArgs, Template, QualType(),
                                 PointOfInstantiation);

   auto* Inst = Instantiator.instantiateRecordDecl(Template);
   if (!OuterInst) {
      assert(PointOfInstantiation && "no point of instantiation!");

      // Remember the scope in which this instantiation was requested.
      auto* InstScope = SP.getCurrentDecl();
      auto InstInfo = new (QC.Context) InstantiationInfo<RecordDecl>(
          PointOfInstantiation, TemplateArgs, Template, Depth);

      Inst->setInstantiationInfo(InstInfo);
      Inst->setIsInstantiation(true);

      SP.registerInstantiation(Inst, InstScope);
      Instantiator.ActOnDecl(Inst, Template->getDeclContext());
   }
   else {
      // Declaration was visible under the new name.
      OuterInst->removeVisibleDecl(Template, Inst->getDeclName());
      QC.Sema->ActOnDecl(OuterInst, Inst);
   }

   // Make this instantiation usable from here onwards.
   finish(Inst);

   LOG(Instantiations, "instantiated record '", Inst->getFullName(),
       "', requested from ",
       QC.CI.getFileMgr().getSourceLocationAsString(PointOfInstantiation));

   QueryResult::ResultKind K = QueryResult::Success;
   SemaPass::DeclScopeRAII DSR(SP, Inst);

   auto& TemplateInfo = QC.RecordMeta[Template];
   auto& InstInfo = QC.RecordMeta[Inst];

   // Make all declarations visible without instantiating them.
   for (auto* D : Template->getDecls<NamedDecl>()) {
      if (D->isSynthesized()) {
         continue;
      }

      // Don't instantiate template parameters.
      if (isa<TemplateParamDecl>(D)) {
         continue;
      }

      if (OuterInst) {
         NamedDecl* Result;
         if (auto Err = QC.InstantiateTemplateMember(
                 Result, D, Inst, TemplateArgs, PointOfInstantiation)) {
            QueryResult::update(K, Err.K);
         }
      }
      else {
         SP.makeDeclAvailable(*Inst, getNameFor(QC.Context, D, Inst), D);
         updateSpecialFunction(QC, D, D, Template, Inst, TemplateInfo,
                               InstInfo);
      }
   }

   // Check which extensions apply, and make their declarations available too.
   for (auto* Ext : Template->getExtensions()) {
      bool Applies;
      if (QC.CheckTemplateExtensionApplicability(Applies, Inst, Ext)) {
         continue;
      }

      if (!Applies)
         continue;

      // Copy the extensions conformances.
      auto Conformances = Ext->getConformanceTypes();
      Inst->getConformanceTypes().append(SP.Context, Conformances.begin(),
                                         Conformances.end());

      for (auto* D : Ext->getDecls<NamedDecl>()) {
         if (D->isSynthesized()) {
            continue;
         }

         if (OuterInst) {
            NamedDecl* Result;
            if (auto Err = QC.InstantiateTemplateMember(
                    Result, D, Inst, TemplateArgs, PointOfInstantiation)) {
               QueryResult::update(K, Err.K);
            }
         }
         else {
            SP.makeDeclAvailable(*Inst, getNameFor(QC.Context, D, Inst), D);
            updateSpecialFunction(QC, D, D, Template, Inst, TemplateInfo,
                                  InstInfo);
         }
      }
   }

   return finish(Inst);
}

QueryResult InstantiateTemplateMemberQuery::run()
{
   if (QC.PrepareDeclForInstantiation(TemplateMember)) {
      return fail();
   }

   if (auto* C = dyn_cast<CallableDecl>(TemplateMember)) {
      if (C->getBodyTemplate()
          && QC.PrepareDeclForInstantiation(C->getBodyTemplate())) {
         return fail();
      }
   }

   // Instantiate declarations provided by protocol extensions.
   if (isa<ProtocolDecl>(TemplateMember->getRecord())) {
      NamedDecl* Result;
      if (auto Err = QC.InstantiateProtocolDefaultImpl(
              Result, TemplateMember, QC.Context.getRecordType(Inst), false)) {
         return Query::finish(Err);
      }

      Inst->removeVisibleDecl(TemplateMember, Result->getDeclName());
      QC.Sema->ActOnDecl(Inst, Result);

      return finish(Result);
   }

   if (!TemplateArgs) {
      TemplateArgs = &Inst->getTemplateArgs();
   }
   if (!PointOfInstantiation) {
      PointOfInstantiation = Inst->getInstantiatedFrom();
   }

   NamedDecl* MemberInst;
   if (auto* R = dyn_cast<RecordDecl>(TemplateMember)) {
      if (QC.InstantiateRecord(R, R, TemplateArgs, PointOfInstantiation,
                               Inst)) {
         return fail();
      }

      MemberInst = R;

      if (MemberInst == TemplateMember) {
         return finish(MemberInst);
      }
      if (!MemberInst) {
         return fail();
      }
   }
   else {
      InstantiatorImpl Instantiator(
          *QC.Sema, *TemplateArgs, TemplateMember->getRecord(),
          QC.Context.getRecordType(Inst), PointOfInstantiation);

      MemberInst = cast_or_null<NamedDecl>(
          Instantiator.instantiateDecl(TemplateMember));

      if (MemberInst == TemplateMember) {
         return finish(MemberInst);
      }
      if (!MemberInst) {
         return fail();
      }

      // Declaration was visible under the new name.
      Inst->removeVisibleDecl(TemplateMember, MemberInst->getDeclName());
      QC.Sema->ActOnDecl(Inst, MemberInst);
   }

   LOG(Instantiations, "instantiated template member '",
       MemberInst->getFullName(), "', requested from ",
       QC.CI.getFileMgr().getSourceLocationAsString(PointOfInstantiation));

   if (TemplateMember->shouldBeSpecialized()) {
      MemberInst->setShouldBeSpecialized(true);
      cast<CallableDecl>(MemberInst)
          ->setBodyTemplate(
              cast<CallableDecl>(TemplateMember)->getBodyTemplate());
   }

   // Don't do this for inner records.
   if (Inst->isInstantiation()) {
      auto& TemplateInfo = QC.RecordMeta[Inst];
      auto& InstInfo = QC.RecordMeta[Inst];

      updateSpecialFunction(QC, TemplateMember, MemberInst, Inst, Inst,
                            TemplateInfo, InstInfo);
   }

   return finish(MemberInst);
}

QueryResult PrepareInstantiationForNameLookupQuery::run()
{
   llvm_unreachable("unimplemented");
}

QueryResult InstantiateCallableQuery::run()
{
   CallableDecl* Inst;
   if (auto* F = dyn_cast<FunctionDecl>(Template)) {
      FunctionDecl* InstFn;
      if (QC.InstantiateFunction(InstFn, F, TemplateArgs,
                                 PointOfInstantiation)) {
         return fail();
      }

      Inst = InstFn;
   }
   else {
      MethodDecl* InstFn;
      if (QC.InstantiateMethod(InstFn, cast<MethodDecl>(Template), TemplateArgs,
                               PointOfInstantiation)) {
         return fail();
      }

      Inst = InstFn;
   }

   return finish(Inst);
}

QueryResult InstantiateFunctionQuery::run()
{
   if (QC.PrepareDeclForInstantiation(Template)) {
      return fail();
   }

   auto& SP = sema();

   std::string MangledName;
   {
      llvm::raw_string_ostream OS(MangledName);
      SP.getMangler().manglePrefix(Template, *TemplateArgs, OS);
   }

   if (auto Inst = lookupExternalInstantiation<FunctionDecl>(MangledName, SP)) {
      Inst->setImportedInstantiation(true);
      return finish(Inst);
   }

   unsigned Depth = getInstantiationDepth(Template, TemplateArgs);
   if (checkDepth(SP, PointOfInstantiation, Depth)) {
      return fail();
   }

   auto* InstScope = SP.getCurrentDecl();

   InstantiatorImpl Instantiator(SP, *TemplateArgs, Template, QualType(),
                                 PointOfInstantiation);

   auto Inst = Instantiator.instantiateFunctionDecl(Template);
   if (!Inst) {
      return fail();
   }

   // Remember where this template was instantiated.
   auto instInfo = new (SP.getContext()) InstantiationInfo<CallableDecl>(
       PointOfInstantiation, TemplateArgs, Template, Depth);

   Inst->setInstantiationInfo(instInfo);
   Inst->setIsInstantiation(true);
   SP.registerInstantiation(Inst, InstScope);

   Instantiator.ActOnDecl(Inst, Template->getDeclContext());

   LOG(Instantiations, "instantiated function '", Inst->getFullName(),
       "', requested from ",
       QC.CI.getFileMgr().getSourceLocationAsString(PointOfInstantiation));

   return finish(Inst);
}

QueryResult InstantiateMethodQuery::run()
{
   auto& SP = sema();
   SemaPass::DeclScopeRAII DSR(SP, Template->getDeclContext());

   // Only instantiate the complete initializer, the base initializer will be
   // synthesized afterwards.
   if (Template->isBaseInitializer()) {
      Template = cast<InitDecl>(Template)->getCompleteInit();
   }

   std::string MangledName;
   {
      llvm::raw_string_ostream OS(MangledName);
      SP.getMangler().manglePrefix(Template, *TemplateArgs, OS);
   }

   if (auto Inst = lookupExternalInstantiation<MethodDecl>(MangledName, SP)) {
      Inst->setImportedInstantiation(true);
      return finish(Inst);
   }

   unsigned Depth = getInstantiationDepth(Template, TemplateArgs);
   if (checkDepth(SP, PointOfInstantiation, Depth)) {
      return fail();
   }

   if (auto BT = Template->getBodyTemplate()) {
      if (auto Info = BT->getLazyFnInfo()) {
         // if the function is a template, we also need it's body
         Info->loadBody(BT);
      }

      if (QC.PrepareDeclForInstantiation(BT)) {
         return fail();
      }
   }
   else {
      if (QC.PrepareDeclForInstantiation(Template)) {
         return fail();
      }
   }

   auto* InstScope = SP.getCurrentDecl();

   MultiLevelFinalTemplateArgList MultiLevelList(*TemplateArgs);
   if (Template->getRecord()->isInstantiation()) {
      MultiLevelList.addOuterList(Template->getRecord()->getTemplateArgs());
   }

   InstantiatorImpl Instantiator(SP, move(MultiLevelList), Template, QualType(),
                                 PointOfInstantiation);

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

   if (!Inst) {
      return fail();
   }

   auto instInfo = new (SP.getContext()) InstantiationInfo<CallableDecl>(
       PointOfInstantiation, TemplateArgs, Template, Depth);

   Inst->setInstantiationInfo(instInfo);
   Inst->setIsInstantiation(true);
   SP.registerInstantiation(Inst, InstScope);

   Instantiator.ActOnDecl(Inst, Template->getDeclContext());

   LOG(Instantiations, "instantiated method '", Inst->getFullName(),
       "', requested from ",
       QC.CI.getFileMgr().getSourceLocationAsString(PointOfInstantiation));

   return finish(Inst);
}

StmtResult TemplateInstantiator::InstantiateStatement(
    SourceLocation, Statement* stmt, const sema::TemplateArgList& templateArgs)
{
   auto FinalList = MakeList(SP, templateArgs);

   InstantiatorImpl Instantiator(SP, *FinalList);
   return Instantiator.instantiateStatement(stmt);
}

StmtResult TemplateInstantiator::InstantiateStatement(
    SourceLocation instantiatedFrom, Statement* stmt,
    MultiLevelFinalTemplateArgList&& templateArgs)
{
   InstantiatorImpl Instantiator(SP, move(templateArgs));
   auto* Inst = Instantiator.instantiateStatement(stmt);
   assert(Instantiator.DelayedTypeDecls.empty());

   return Inst;
}

ExprResult TemplateInstantiator::InstantiateTypeExpr(RecordDecl* Rec,
                                                     Expression* E)
{
   llvm_unreachable("needed?");
   //   InstantiatorImpl Instantiator(SP, Rec);
   //   return cast<Expression>(Instantiator.instantiateStatement(E));
}

QueryResult InstantiateDeclQuery::run()
{
   // Make a copy of the template argument list.
   auto TemplateArgs = this->TemplateArgs;

   InstantiatorImpl Instantiator(*QC.Sema, move(TemplateArgs));
   auto* Inst = Instantiator.instantiateDecl(Template);
   if (!Inst) {
      return fail();
   }

   return finish(Inst);
}

DeclResult TemplateInstantiator::InstantiateDecl(
    SourceLocation instantiatedFrom, Decl* D,
    MultiLevelFinalTemplateArgList&& templateArgs)
{
   InstantiatorImpl Instantiator(SP, move(templateArgs), D);
   auto* Inst = Instantiator.instantiateDecl(D);

   return Inst;
}

StmtResult TemplateInstantiator::InstantiateStatement(StmtOrDecl,
                                                      Statement* stmt,
                                                      IdentifierInfo* SubstName,
                                                      il::Constant* SubstVal)
{
   InstantiatorImpl Instantiator(SP, SubstName, SubstVal);
   return Instantiator.instantiateStatement(stmt);
}

QueryResult InstantiateMethodBodyQuery::run()
{
   assert(Inst->getBodyTemplate() && "function does not need instantiation");

   if (auto Err = QC.TypecheckDecl(Inst->getBodyTemplate())) {
      return Query::finish(Err);
   }
   if (auto Err = QC.TypecheckDecl(Inst)) {
      return Query::finish(Err);
   }

   if (Inst->getBodyTemplate()->isInvalid()) {
      Inst->setIsInvalid(true);
      return Query::finish(DoneWithError);
   }

   if (!Inst->getBodyTemplate()->getBody()) {
      assert(Inst->getBodyTemplate()->hasAttribute<_BuiltinAttr>());
      return finish();
   }

   MultiLevelFinalTemplateArgList ArgList;
   if (Inst->isInstantiation()) {
      ArgList.addOuterList(Inst->getTemplateArgs());
   }
   if (isa<MethodDecl>(Inst) && Inst->getRecord()->isInstantiation()) {
      ArgList.addOuterList(Inst->getRecord()->getTemplateArgs());
   }

   bool IsExternallyLoaded = false;
   if (auto LazyInfo = Inst->getBodyTemplate()->getLazyFnInfo()) {
      LazyInfo->loadBody(Inst->getBodyTemplate());
      IsExternallyLoaded = true;
   }

   auto* InstScope = QC.Sema->getCurrentDecl();

   InstantiatorImpl Instantiator(*QC.Sema, move(ArgList), StmtOrDecl(), Inst);
   auto BodyInst
       = Instantiator.instantiateStatement(Inst->getBodyTemplate()->getBody());

   Inst->setBody(BodyInst);

   Status S = Done;
   {
      SemaPass::DeclScopeRAII raii(*QC.Sema, Inst);
      SemaPass::ScopeGuard scope(*QC.Sema, Inst);

      auto res = QC.Sema->visitStmt(Inst, BodyInst);
      if (res) {
         Inst->setBody(res.get());
      }

      if (!res || BodyInst->isInvalid()) {
         S = DoneWithError;
      }
   }

   if (auto I = dyn_cast<InitDecl>(Inst)) {
      if (I->isCompleteInitializer()) {
         QC.Sema->registerInstantiation(I->getBaseInit(), InstScope);
      }
      else {
         QC.Sema->registerInstantiation(I->getCompleteInit(), InstScope);
      }
   }

   // ILGen would skip this decl since it's external, but we still need to
   // define the body
   if (IsExternallyLoaded && isa<MethodDecl>(Inst)) {
      QC.Sema->getILGen().registerInstantiatedImportedMethod(
          cast<MethodDecl>(Inst));
   }

   if (auto Init = dyn_cast<InitDecl>(Inst)) {
      if (Init->isCompleteInitializer()) {
         Init->getBaseInit()->setBody(Init->getBody());
      }
   }

   LOG(Instantiations, "instantiated body of '", Inst->getFullName(), "'");
   return finish(S);
}

ExprResult TemplateInstantiator::InstantiateStaticExpr(
    SourceLocation instantiatedFrom, Expression* expr,
    const sema::TemplateArgList& templateArgs)
{
   auto Stmt = InstantiateStatement(instantiatedFrom, expr, templateArgs);
   if (!Stmt)
      return ExprError();

   return cast<Expression>(Stmt.getValue());
}

QueryResult InstantiateAliasQuery::run()
{
   if (auto Err = QC.PrepareDeclForInstantiation(Template)) {
      return Query::finish(Err);
   }

   auto& SP = sema();
   SemaPass::DeclScopeRAII DSR(SP, Template->getDeclContext());

   std::string MangledName;
   {
      llvm::raw_string_ostream OS(MangledName);
      SP.getMangler().manglePrefix(Template, *TemplateArgs, OS);
   }

   if (auto Inst = lookupExternalInstantiation<AliasDecl>(MangledName, SP)) {
      Inst->setImportedInstantiation(true);
      return finish(Inst);
   }

   unsigned Depth = getInstantiationDepth(Template, TemplateArgs);
   if (checkDepth(SP, PointOfInstantiation, Depth)) {
      return fail();
   }

   auto* InstScope = SP.getCurrentDecl();
   auto* InstInfo = new (SP.getContext()) InstantiationInfo<AliasDecl>(
       PointOfInstantiation, TemplateArgs, Template, Depth);

   InstantiatorImpl Instantiator(SP, *InstInfo->templateArgs, Template,
                                 QualType(), PointOfInstantiation);

   auto Inst = Instantiator.instantiateAliasDecl(Template);
   if (!Inst) {
      return fail();
   }

   Inst->setInstantiationInfo(InstInfo);
   Inst->setIsInstantiation(true);
   SP.registerInstantiation(Inst, InstScope);

   Instantiator.ActOnDecl(Inst, Template->getDeclContext());

   LOG(Instantiations, "instantiated alias '", Inst->getFullName(),
       "', requested from ",
       QC.CI.getFileMgr().getSourceLocationAsString(PointOfInstantiation));

   return finish(Inst);
}