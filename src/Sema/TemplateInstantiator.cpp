#include "cdotc/Sema/TemplateInstantiator.h"

#include "cdotc/AST/ASTContext.h"
#include "cdotc/AST/TypeBuilder.h"
#include "cdotc/IL/Constants.h"
#include "cdotc/ILGen/ILGenPass.h"
#include "cdotc/Diagnostics/Diagnostics.h"
#include "cdotc/Module/Module.h"
#include "cdotc/Module/ModuleManager.h"
#include "cdotc/Query/QueryContext.h"
#include "cdotc/Sema/SemaPass.h"
#include "cdotc/Sema/Template.h"
#include "cdotc/Serialization/ModuleFile.h"
#include "cdotc/Support/Log.h"

#include <llvm/ADT/SmallString.h>
#include <llvm/Support/raw_ostream.h>

using namespace cdot;
using namespace cdot::ast;
using namespace cdot::support;
using namespace cdot::diag;
using namespace cdot::sema;

using std::string;

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
                    DeclContext* InstCtx = nullptr, SourceLocation POI = {},
                    QualType Self = QualType())
       : ASTTypeVisitor(false), TypeBuilder(SP, SourceRange(POI)), SP(SP),
         Context(SP.Context),
         InstScope(SP, InstCtx ? InstCtx : &SP.getDeclContext()),
         templateArgs(move(templateArgs)), Self(Self), POI(POI)
   {
      SourceID = SP.getCompilerInstance().getFileMgr().getSourceId(
          SP.getCurrentDecl()->getSourceLoc());
   }

   InstantiatorImpl(SemaPass& SP, MultiLevelFinalTemplateArgList&& templateArgs,
                    NamedDecl* Template, DeclContext* InstCtx = nullptr,
                    QualType Self = QualType(), SourceLocation POI = {})
       : ASTTypeVisitor(false), TypeBuilder(SP, SourceRange(POI)), SP(SP),
         Context(SP.Context), InstScope(SP, InstCtx ? InstCtx : Template->getDeclContext()),
         templateArgs(move(templateArgs)), Template(Template), Self(Self),
         POI(POI)
   {
      SourceID = SP.getCompilerInstance().getFileMgr().getSourceId(
          SP.getCurrentDecl()->getSourceLoc());

      if (Self) {
         InstantiatingRecord = Self->getRecord();
      }
   }

   InstantiatorImpl(SemaPass& SP, IdentifierInfo* SubstName,
                    il::Constant* SubstVal)
       : ASTTypeVisitor(false), TypeBuilder(SP, SourceRange()), SP(SP),
         Context(SP.Context),
         InstScope(SP, &SP.getDeclContext()), ValueSubst{SubstName, SubstVal}
   {
      SourceID = SP.getCompilerInstance().getFileMgr().getSourceId(
          SP.getCurrentDecl()->getSourceLoc());
   }

   friend class cdot::ast::TemplateInstantiator;

   RecordDecl* instantiateRecordDecl(RecordDecl* Decl);

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

   Statement* instantiateFunctionBody(CallableDecl* inst)
   {
      auto* Template = inst->getBodyTemplate();
      auto numArgs = inst->getArgs().size();

      int i = 0;
      for (auto* paramDecl : Template->getArgs()) {
         if (i >= numArgs) {
            continue;
         }

         instantiationMap.try_emplace(paramDecl, inst->getArgAt(i++));
      }

      return instantiateStatement(Template->getBody());
   }

   Statement* instantiateStatement(Statement* Stmt) { return visit(Stmt); }

   Decl* instantiateDecl(Decl* D) { return visit(D); }

   NamedDecl* getTemplate() const { return Template; }
   const MultiLevelFinalTemplateArgList& getTemplateArgs() const
   {
      return templateArgs;
   }

   void setSelfType(QualType Self) { this->Self = Self; }

   Decl* ActOnDecl(Decl* D, DeclContext* Ctx = nullptr)
   {
      if (!Ctx) {
         Ctx = &SP.getDeclContext();
      }

      SP.ActOnDecl(Ctx, D);
      return D;
   }

private:
   void instantiateConstraints(NamedDecl* D, NamedDecl* Inst)
   {
      auto* constraints = Context.getExtConstraints(D);
      if (!constraints->containsTemplateParam()) {
         return Context.setConstraints(D, constraints);
      }

      SmallVector<DeclConstraint*, 2> instantiatedConstraints;
      for (auto* DC : *constraints) {
         QualType constrainedType = DC->getConstrainedType();
         QualType instantiatedType = visit(constrainedType);

         if (constrainedType == instantiatedType) {
            instantiatedConstraints.push_back(DC);
            continue;
         }

         DeclConstraint* constraintInst;
         if (DC->getKind() == DeclConstraint::Concept) {
            constraintInst = DeclConstraint::Create(Context, instantiatedType,
                                                    DC->getConcept());
         }
         else {
            constraintInst = DeclConstraint::Create(
                Context, DC->getKind(), instantiatedType, DC->getType());
         }

         instantiatedConstraints.push_back(constraintInst);
      }

      auto* CS = ConstraintSet::Create(Context, instantiatedConstraints);
      Context.setConstraints(D, CS);
   }

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

      return !Stmt->needsInstantiation() && Stmt->isSemanticallyChecked();
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
            return D->isSemanticallyChecked();
         case Decl::MacroDeclID:
         case Decl::MacroExpansionDeclID:
            return true;
         case Decl::FuncArgDeclID: {
//            auto *Arg = cast<FuncArgDecl>(D);
//            return !Arg->getType()->isDependentType()
//                   && !Arg->getType()->containsTemplateParamType()
//                   && !Arg->isSelf();
            return false;
         }
         case Decl::AssociatedTypeDeclID: {
            auto *AT = cast<AssociatedTypeDecl>(D);
            return !AT->getDefaultType()->isDependentType()
                   && !AT->getDefaultType()->containsTemplateParamType();
         }
         default:
            return false;
         }
      }

      return false;
   }

   Expression* visit(Expression* expr)
   {
      if (canElideInstantiation(expr))
         return expr;

      Expression* Inst;
      switch (expr->getTypeID()) {
#define CDOT_EXPR(Name)                                                        \
   case Statement::Name##ID:                                                     \
      Inst = visit##Name(static_cast<Name*>(expr));                            \
      break;
#include "cdotc/AST/AstNode.def"

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

//      assert(stmt->isSemanticallyChecked() && "instantiating unchecked expr");

      if (canElideInstantiation(stmt))
         return stmt;

      Statement* Inst;
      switch (stmt->getTypeID()) {
#define CDOT_STMT(Name)                                                        \
   case Statement::Name##ID:                                                     \
      Inst = visit##Name(static_cast<Name*>(stmt));                            \
      break;
#include "cdotc/AST/AstNode.def"

      default:
         llvm_unreachable("not a statement!");
      }

      if (Inst)
         Inst->setUnsafe(stmt->isUnsafe());

      return Inst;
   }

   Decl* visit(Decl* D)
   {
      auto* inst = visitImpl(D);
      instantiationMap.try_emplace(D, inst);

      return inst;
   }

   Decl* visitImpl(Decl* D)
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
#include "cdotc/AST/Decl.def"

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
#include "cdotc/AST/Decl.def"

      default:
         llvm_unreachable("not a named decl!");
      }

      if (Inst) {
         Inst->setAccessLoc(D->getAccessLoc());
         Context.setAttributes(Inst, D->getAttributes());

         if (auto *DC = dyn_cast<DeclContext>(D)) {
            if (SP.LookupContextMap.find(DC) != SP.LookupContextMap.end()) {
               SP.LookupContextMap[cast<DeclContext>(Inst)]
                   = SP.LookupContextMap[DC];
            }
         }
      }

      return Inst;
   }

public:
   SourceType visit(const SourceType& Ty)
   {
      if (!Ty.isResolved()) {
         if (auto *E = Ty.getTypeExpr()) {
            return SourceType(visit(E));
         }

         return SourceType();
      }

      QualType ResolvedTy = Ty.getResolvedType();
      uint16_t Props = ResolvedTy->properties().getRawProperties();

      bool containsDependentType = (Props & DependentMask) != 0;
      bool containsTemplate = false;
      SP.QC.ContainsTemplate(containsTemplate, ResolvedTy);

      while (containsDependentType || containsTemplate) {
         QualType Inst = ResolvedTy;

         // Substitute associateed types.
         if (Inst->containsAssociatedType() && Self) {
            if (SP.QC.SubstAssociatedTypes(Inst, Inst, Self, POI)) {
               return Ty;
            }
         }

         // Substitute nested records of instantiations.
         if (containsTemplate) {
            Inst = TypeBuilder::visit(Inst);
         }

         // Substitute template parameters.
         if ((Props & GenericMask) != 0) {
            if (!SubstContexts.empty()) {
               Inst = VariadicParamSubstVisitor(*this).visit(Inst);
            }

            if (SP.QC.SubstTemplateParamTypes(Inst, Inst, templateArgs, POI)) {
               return Ty;
            }
         }

         // No more changes.
         if (Inst == ResolvedTy) {
            break;
         }

         ResolvedTy = Inst;

         // Update type properties.
         Props = ResolvedTy->properties().getRawProperties();
         containsDependentType = (Props & DependentMask) != 0;

         // Check if the type still contains a template.
         // FIXME can this happen more than once?
         containsTemplate = false;
         SP.QC.ContainsTemplate(containsTemplate, ResolvedTy);
      }

      return ResolvedTy;
   }

   QualType visitRecordType(RecordType* T)
   {
      if (!Self || !T->getRecord()->isTemplateOrInTemplate()) {
         return T;
      }

      bool replace = false;
      NamedDecl *Cur = T->getRecord();
      while (Cur && Cur->isTemplateOrInTemplate()) {
         if (Cur == Self->getRecord()->getSpecializedTemplate()) {
            replace = true;
            break;
         }

         Cur = dyn_cast<NamedDecl>(Cur->getDeclContext()->lookThroughExtension());
      }

      if (replace) {
         auto *Inst = SP.getInstantiator().getInstantiation(
             T->getRecord(), &Self->getTemplateArgs());

         if (!Inst) {
            return T;
         }

         return Context.getRecordType(Inst);
      }

      return T;
   }

   const TemplateArgument* hasTemplateArg(TemplateParamDecl* P)
   {
      return templateArgs.getArgForParam(P);
   }

   struct SubstContextRAII {
      SubstContextRAII(InstantiatorImpl& Inst, TemplateParamDecl* param,
                       DeclarationName declName, unsigned index,
                       std::function<Expression*(SourceRange)>&& makeSubstExpr)
          : Inst(Inst)
      {
         Inst.SubstContexts.emplace_back(param, declName, index,
                                         move(makeSubstExpr));
      }

      ~SubstContextRAII() { Inst.SubstContexts.pop_back(); }

   private:
      InstantiatorImpl& Inst;
   };

private:
   Decl* getInstantiation(Decl* D)
   {
      auto it = instantiationMap.find(D);
      if (it == instantiationMap.end()) {
         if (canElideInstantiation(D)) {
            return D;
         }

         assert(Self && isa<NamedDecl>(D)
                && "no type to lookup instantiations!");

         NamedDecl* result;
         if (SP.QC.FindEquivalentDecl(result, cast<NamedDecl>(D),
                                      Self->getRecord(), Self)) {
            return D;
         }

         assert(result && "no instantiated decl!");
         instantiationMap[D] = result;

         return result;
      }

      return it->getSecond();
   }

   template<class T> T* getInstantiation(Decl* D)
   {
      return cast<T>(getInstantiation(D));
   }

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

   Expression* visitDeclRefExpr(DeclRefExpr* Expr);
   Expression* visitMemberRefExpr(MemberRefExpr* Expr);

   OverloadedDeclRefExpr* visitOverloadedDeclRefExpr(OverloadedDeclRefExpr*)
   {
      llvm_unreachable("should never be dependent!");
   }

   MacroExpansionExpr* visitMacroExpansionExpr(MacroExpansionExpr* Expr);
   MacroExpansionStmt* visitMacroExpansionStmt(MacroExpansionStmt* Stmt);
   MacroExpansionDecl* visitMacroExpansionDecl(MacroExpansionDecl* Decl);

   ImportDecl* visitImportDecl(ImportDecl* node);

   UnittestDecl* visitUnittestDecl(UnittestDecl* D);

   LocalVarDecl* visitLocalVarDecl(LocalVarDecl* node);
   GlobalVarDecl* visitGlobalVarDecl(GlobalVarDecl* node);
   DestructuringDecl* visitDestructuringDecl(DestructuringDecl* Decl);

   FunctionDecl* visitFunctionDecl(FunctionDecl* F);

   TemplateParamDecl* visitTemplateParamDecl(TemplateParamDecl* decl);

   ClassDecl* visitClassDecl(ClassDecl* node) { llvm_unreachable(""); }
   StructDecl* visitStructDecl(StructDecl* node) { llvm_unreachable(""); }
   ProtocolDecl* visitProtocolDecl(ProtocolDecl* node) { llvm_unreachable(""); }
   ExtensionDecl* visitExtensionDecl(ExtensionDecl* Ext,
                                     RecordDecl* RecInst = nullptr);

   EnumDecl* visitEnumDecl(EnumDecl* node) { llvm_unreachable(""); }
   UnionDecl* visitUnionDecl(UnionDecl* node) { llvm_unreachable(""); }

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

   BreakStmt* visitBreakStmt(BreakStmt* node) { return node; }
   ContinueStmt* visitContinueStmt(ContinueStmt* node) { return node; }

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
   Decl* visitStaticIfDecl(StaticIfDecl* D);
   Decl* visitStaticForDecl(StaticForDecl* node);

   Expression* visitVariadicExpansionExpr(VariadicExpansionExpr* Expr);
   void visitVariadicExpansionExpr(VariadicExpansionExpr* Expr,
                                   SmallVectorImpl<Expression*>& Exprs);

   Statement* visitStaticIfStmt(StaticIfStmt* Stmt);
   Statement* visitVariadicForStmt(StaticForStmt* node);
   Statement* visitStaticForStmt(StaticForStmt* node);

   Decl* visitVariadicForDecl(StaticForDecl* Decl);

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

   NullStmt* visitNullStmt(NullStmt* node) { return node; }

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

   /// Map from templates to instantiated declarations.
   llvm::DenseMap<const Decl*, Decl*> instantiationMap;

   /// True iff we're currently in an unevaluated scope.
   bool InUnevaluatedScope = false;

   /// Source ID we're instantiating from.
   unsigned SourceID = fs::InvalidID;

   /// Value substitution in a 'static for'.
   struct {
      IdentifierInfo* Name;
      il::Constant* SubstVal = nullptr;
   } ValueSubst;

   struct SubstContext {
   public:
      SubstContext(TemplateParamDecl* param, DeclarationName declName,
                   unsigned index,
                   std::function<Expression*(SourceRange)>&& makeSubstExpr)
          : param(param), declName(declName), index(index),
            makeSubstExpr(move(makeSubstExpr))
      {
      }

      TemplateParamDecl* param;
      DeclarationName declName;
      unsigned index;
      std::function<Expression*(SourceRange)> makeSubstExpr;
   };

   std::vector<SubstContext> SubstContexts;

   Expression* getVariadicSubstitution(DeclarationName declName, SourceRange SR)
   {
      for (auto it = SubstContexts.rbegin(); it != SubstContexts.rend(); ++it) {
         if (it->declName == declName) {
            return it->makeSubstExpr(SR);
         }
      }

      return nullptr;
   }

   sema::FinalTemplateArgumentList*
   instantiateTemplateArgs(sema::FinalTemplateArgumentList* templateArgs,
                           NamedDecl* Template);

   void instantiateTemplateArgument(const TemplateArgument& TA,
                                    TemplateArgList& list, bool variadic);

   QualType instantiateConversionStep(ConversionSequenceBuilder& builder,
                                      const ConversionStep& step,
                                      QualType currentType);

   IfCondition visitIfCondition(const IfCondition& C);

   Expression* makeLiteralExpr(Expression* Expr, QualType Ty,
                               const Variant& Val);

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
            instantiationMap[P] = P;
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
            if (Arg->isVariadic()) {
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

   class VariadicParamSubstVisitor
       : public TypeBuilder<VariadicParamSubstVisitor> {
      InstantiatorImpl& instantiator;

   public:
      explicit VariadicParamSubstVisitor(InstantiatorImpl& instantiator)
          : TypeBuilder(instantiator.SP, {}), instantiator(instantiator)
      {
      }

      void visitTemplateParamType(TemplateParamType* T,
                                  SmallVectorImpl<QualType>& Types)
      {
         Types.push_back(visitTemplateParamType(T));
      }

      QualType visitTemplateParamType(TemplateParamType* T)
      {
         const TemplateArgument* Arg
             = instantiator.templateArgs.getArgForParam(T->getParam());
         if (!Arg || Arg->isNull()) {
            return T;
         }

         assert(!Arg->isValue() && "should not appear in type position!");
         if (Arg->isVariadic()) {
            for (auto it = instantiator.SubstContexts.rbegin();
                 it != instantiator.SubstContexts.rend(); ++it) {
               if (it->param == T->getParam()) {
                  return Arg->getVariadicArgs()[it->index].getType();
               }
            }
         }

         return Arg->getType();
      }

      QualType visitTypedefType(TypedefType* T)
      {
         if (T->getTypedef()->isInstantiation()) {
            return TypeBuilder::visitTypedefType(T);
         }

         return visit(T->getDesugaredType());
      }
   };

   template<class Container>
   using element_type =
       typename std::remove_pointer<typename std::remove_reference<decltype(
           ((Container*)0)->front())>::type>::type;

   template<class Container, class Element = element_type<Container>>
   bool expandVariadicArgument(Container& exprs, Expression* variadicExpr)
   {
      if (true || !variadicExpr->isVariadicArgPackExpansion()) {
         exprs.emplace_back(cast<Element>(visit(variadicExpr)));
         return false;
      }

      //      SmallPtrSet<TemplateParamDecl*, 4> VariadicParams;
      //      SmallPtrSet<FuncArgDecl*, 4> PackArguments;
      //
      //      VariadicParamFinder Visitor(VariadicParams, PackArguments);
      //      Visitor.visit(variadicExpr);
      //
      //      assert(!VariadicParams.empty() && "undiagnosed invalid pack
      //      expansion");
      //
      //      if (VariadicParams.size() > 1) {
      //         llvm_unreachable("FIXME error message");
      //      }
      //
      //      TemplateParamDecl *ParameterPack = *VariadicParams.begin();
      //      FuncArgDecl *PackArgument = nullptr;
      //
      //      if (!PackArguments.empty())
      //         PackArgument = *PackArguments.begin();
      //
      //      auto GivenArg = hasTemplateArg(ParameterPack);
      //      if (!GivenArg) {
      //         // current context is still templated
      //         exprs.emplace_back(cast<Element>(visit(variadicExpr)));
      //         return false;
      //      }
      //
      //      assert(GivenArg->isVariadic() && "invalid passed template
      //      argument");
      //
      //      unsigned numVariadics =
      //      (unsigned)GivenArg->getVariadicArgs().size(); for (unsigned i = 0;
      //      i < numVariadics; ++i) {
      //         DeclarationName IdentSubst;
      //         const IdentifierInfo *FuncArg = nullptr;
      //
      //         if (PackArgument) {
      //            FuncArg = PackArgument->getIdentifierInfo();
      //            IdentSubst = Context.getDeclNameTable()
      //                                .getPackExpansionName(FuncArg, i);
      //         }
      //
      //         SubstContextRAII raii(*this, ParameterPack, FuncArg,
      //         IdentSubst,
      //                               &GivenArg->getVariadicArgs()[i]);
      //
      //         Expression *newExpr = visit(variadicExpr);
      //         newExpr->setEllipsisLoc(SourceLocation());
      //
      //         exprs.emplace_back(newExpr);
      //      }
      //
      //      return true;
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
                                {}, move(templateParmams));
      break;
   case Decl::ClassDeclID:
      Inst = ClassDecl::Create(Context, access, Decl->getKeywordLoc(), Name,
                               {}, move(templateParmams),
                               enumRawTypeOrExtends, isAbstract);
      break;
   case Decl::EnumDeclID:
      Inst = EnumDecl::Create(Context, access, Decl->getKeywordLoc(), Name,
                              {}, move(templateParmams),
                              enumRawTypeOrExtends);
      break;
   default:
      llvm_unreachable("not a record decl!");
   }

   // make sure to set the parent context before calling this
   Inst->setParentCtx(&SP.getCompilerInstance().getGlobalDeclCtx());
   if (auto MF = Decl->getModFile())
      Inst->setModFile(MF->copy());

   Inst->setLastMethodID(Decl->getLastMethodID());
   Context.setAttributes(Inst, Decl->getAttributes());

   if (auto* E = dyn_cast<EnumDecl>(Decl)) {
      cast<EnumDecl>(Inst)->setMaxAssociatedValues(E->getMaxAssociatedValues());
      cast<EnumDecl>(Inst)->setUnpopulated(E->isUnpopulated());
   }

   return Inst;
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

ExtensionDecl* InstantiatorImpl::visitExtensionDecl(ExtensionDecl* Ext,
                                                    RecordDecl* RecInst)
{
   assert(RecInst && "instantiating extension without instantiated record!");

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
       visit(Decl->getType()), Decl->isStatic(), Decl->isReadWrite(),
       nullptr, nullptr);

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

   auto templateParams = copyTemplateParameters(M);

   SmallVector<FuncArgDecl*, 4> Args;
   copyArgListAndFindVariadic(M, Args);

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
   SourceType DefaultType = visit(Decl->getDefaultType());
   auto Inst = AssociatedTypeDecl::Create(Context, Decl->getSourceLoc(),
                                          Decl->getDeclName(), DefaultType,
                                          visit(Decl->getCovariance()));

   instantiateConstraints(Decl, Inst);
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
   if (Alias->isTemplate() && IsMainTemplate) {
      Name = Context.getDeclNameTable().getInstantiationName(
          Alias->getDeclName(), templateArgs.innermost());
   }

   StaticExpr *Expr = nullptr;
   SourceType Ty = Alias->getType();

   // Associated type instantiations are handled below by TemplateInstantiator.
   if (!Alias->isImplOfProtocolRequirement()) {
      Expr = copyOrNull(Alias->getAliasExpr());
      Ty = visitOrAuto(Ty);
   }
   else {
      Ty = visitOrAuto(Ty);
   }

   auto templateParams = copyTemplateParameters(Alias);
   auto Inst
       = AliasDecl::Create(Context, Alias->getSourceLoc(), Alias->getAccess(),
                           Name, Ty, Expr, templateParams);

   instantiateConstraints(Alias, Inst);
   return Inst;
}

FieldDecl* InstantiatorImpl::visitFieldDecl(FieldDecl* Decl)
{
   // Make sure the default value was visited.
   if (SP.QC.TypecheckDecl(Decl))
      return nullptr;

   DeclarationName DeclName = Decl->getDeclName();

   auto Inst = FieldDecl::Create(Context, Decl->getAccess(),
                                 Decl->getVarOrLetLoc(), Decl->getColonLoc(),
                                 DeclName, visit(Decl->getType()),
                                 Decl->isStatic(), Decl->isConst(), nullptr);

   Inst->setValue(copyOrNull(Decl->getDefaultVal()));

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

   auto templateParams = copyTemplateParameters(Decl);

   SmallVector<FuncArgDecl*, 4> Args;
   copyArgListAndFindVariadic(Decl, Args);

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

   auto templateParams = copyTemplateParameters(F);

   SmallVector<FuncArgDecl*, 4> Args;
   copyArgListAndFindVariadic(F, Args);

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

QualType
InstantiatorImpl::instantiateConversionStep(ConversionSequenceBuilder& builder,
                                            const ConversionStep& step,
                                            QualType currentType)
{
   switch (step.getKind()) {
   case CastKind::LValueToRValue:
      builder.addStep(CastKind::LValueToRValue, currentType->removeReference(),
                      CastStrength::Implicit);

      break;
   case CastKind::NoOp:
      break;
   default:
      builder.addStep(step.getKind(), visit(step.getResultType()),
                      CastStrength::Implicit);

      break;
   }

   return builder.getSteps().back().getResultType();
}

Expression* InstantiatorImpl::visitImplicitCastExpr(ImplicitCastExpr* node)
{
   auto* expr = visit(node->getTarget());
   QualType currentType = visit(node->getTarget()->getExprType());

   ConversionSequenceBuilder builder;
   for (auto& step : node->getConvSeq().getSteps()) {
      currentType = instantiateConversionStep(builder, step, currentType);
   }

   return ImplicitCastExpr::Create(
       Context, expr, ConversionSequence::Create(Context, builder));
}

CompoundDecl* InstantiatorImpl::visitCompoundDecl(CompoundDecl* D)
{
   auto Inst
       = CompoundDecl::Create(Context, D->getLBraceLoc(), D->isTransparent());

   SP.ActOnDecl(&SP.getDeclContext(), Inst);

   for (auto& decl : D->getDecls()) {
      auto* declInst = visit(decl);
      if (declInst) {
         SP.ActOnDecl(Inst, declInst);
      }
   }

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
      if (!arg->isVariadic()) {
         auto* argInst = clone(arg);
         Variadics.push_back(argInst);
         instantiationMap.try_emplace(arg, argInst);
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

   auto* Inst = FuncArgDecl::Create(
       Context, Decl->getOwnershipLoc(), Decl->getColonLoc(), DeclName,
       Decl->getLabel(), Decl->getConvention(), visit(Decl->getType()),
       copyOrNull(Decl->getDefaultVal()), Decl->isVariadic() && !IsSubstitution,
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
                                       Context.getBoolTy(),
                                       Val.getZExtValue() != 0);

         break;
      case 8:
         literal = CharLiteral::Create(Context, Expr->getSourceRange(),
                                       Context.getCharTy(), Val.getChar());

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
   auto *Copy = new(Context) IdentifierRefExpr(
       Ident->getSourceRange(),
       copyOrNull(Ident->getParentExpr()), Ident->getDeclName());

   Copy->setIsPointerAccess(Ident->isPointerAccess());
   Copy->setIsInTypePosition(Ident->isInTypePosition());
   Copy->setIsSynthesized(Ident->isSynthesized());
   Copy->setLeadingDot(Ident->hasLeadingDot());

   return Copy;
}

Expression* InstantiatorImpl::visitDeclRefExpr(DeclRefExpr* Expr)
{
   auto* decl = Expr->getDecl();
   switch (decl->getKind()) {
   case Decl::LocalVarDeclID: {
      auto* localVar = cast<LocalVarDecl>(decl);
      if (localVar->isVariadicForDecl()) {
         auto* subst = getVariadicSubstitution(
             localVar->getDeclName().getLocalVarName(), Expr->getSourceRange());

         assert(subst && "no substitution for variadic parameter!");
         return subst;
      }

      goto case_default;
   }
   case Decl::AliasDeclID: {
      auto* alias = cast<AliasDecl>(decl);
      if (alias->isVariadicForDecl()) {
         auto* subst = getVariadicSubstitution(
             alias->getDeclName().getLocalVarName(), Expr->getSourceRange());

         assert(subst && "no substitution for variadic parameter!");
         return subst;
      }

      goto case_default;
   }
   case Decl::AssociatedTypeDeclID: {
      assert(Self && "no Self type to find associated type implementation!");

      NamedDecl* inst;
      if (SP.QC.FindEquivalentDecl(
              inst, decl, cast<DeclContext>(SP.getTypeDecl(Self)), Self)) {
         return nullptr;
      }

      assert(inst && "associated type not implemented");
      return DeclRefExpr::Create(Context, inst, Expr->getSourceRange());
   }
   case Decl::TemplateParamDeclID: {
      auto* param = cast<TemplateParamDecl>(decl);
      if (const TemplateArgument* arg = hasTemplateArg(param)) {
         assert(!arg->isVariadic() && "should have been substituted");

         if (arg->isValue()) {
            return StaticExpr::Create(Context, arg->getValueType(),
                                      Expr->getSourceRange(), arg->getValue());
         }

         return new (Context)
             IdentifierRefExpr(Expr->getSourceRange(), IdentifierKind::MetaType,
                               Context.getMetaType(arg->getType()));
      }

      goto case_default;
   }
   case_default:
   default:
      return DeclRefExpr::Create(Context, getInstantiation<NamedDecl>(decl),
                                 Expr->getSourceRange());
   }
}

Expression* InstantiatorImpl::visitMemberRefExpr(MemberRefExpr* Expr)
{
   CanType parentType
       = Expr->getParentExpr()->ignoreParensAndImplicitCasts()->getExprType();

   if (!parentType->containsTemplateParamType()
       && !parentType->containsAssociatedType()) {
      return MemberRefExpr::Create(Context, visit(Expr->getParentExpr()),
                                   Expr->getMemberDecl(),
                                   Expr->getSourceRange());
   }

   SourceType instantiatedType(parentType);
   instantiatedType = visit(instantiatedType);

   if (!instantiatedType) {
      return nullptr;
   }

   NamedDecl *MemberDecl = Expr->getMemberDecl();
   QualType lookupType = instantiatedType->removeReference()->removeMetaType();

   NamedDecl *decl;
   if (MemberDecl->isImplOfProtocolRequirement() || MemberDecl->isProtocolRequirement()) {
      decl = SP.Context.getProtocolImpl(lookupType->getRecord(), Expr->getMemberDecl());
   }
   else {
      decl = SP.getInstantiator().getMemberInstantiation(
         lookupType->getRecord(), Expr->getMemberDecl(),
         &lookupType->getRecord()->getTemplateArgs());
   }

   assert(decl && "no instantiated decl!");
   decl = SP.maybeInstantiateTemplateMember(lookupType->getRecord(), decl);

   if (MemberRefExpr::needsMemberRefExpr(decl)) {
      return MemberRefExpr::Create(Context, visit(Expr->getParentExpr()), decl,
                                   Expr->getSourceRange());
   }
   else {
      return DeclRefExpr::Create(Context, decl, Expr->getSourceRange());
   }
}

BuiltinIdentExpr*
InstantiatorImpl::visitBuiltinIdentExpr(BuiltinIdentExpr* node)
{
   return BuiltinIdentExpr::Create(Context, node->getSourceLoc(),
                                   node->getIdentifierKind());
}

FinalTemplateArgumentList* InstantiatorImpl::instantiateTemplateArgs(
    FinalTemplateArgumentList* templateArgs, NamedDecl* Template)
{
   TemplateArgList realTemplateArgs(SP, Template);
   for (const auto& TA : *templateArgs) {
      instantiateTemplateArgument(TA, realTemplateArgs, false);
   }

   return FinalTemplateArgumentList::Create(SP.Context, realTemplateArgs);
}

void InstantiatorImpl::instantiateTemplateArgument(const TemplateArgument& TA,
                                                   TemplateArgList& list,
                                                   bool variadic)
{
   if (TA.isVariadic()) {
      for (const auto& VA : TA.getVariadicArgs()) {
         instantiateTemplateArgument(VA, list, true);
      }

      return;
   }

   TemplateArgument newArg;
   if (TA.isValue()) {
      auto* valueExpr = cast<StaticExpr>(visit(TA.getValueExpr()));
      newArg = TemplateArgument(TA.getParam(), valueExpr, TA.getLoc());
   }
   else {
      auto type = visit(TA.getType());
      newArg = TemplateArgument(TA.getParam(), type, TA.getLoc());
   }

   if (variadic) {
      list.getArgForParam(TA.getParam())
          ->getVariadicArgs()
          .push_back(move(newArg));
   }
   else {
      list.setParamValue(TA.getParam(), move(newArg));
   }
}

Expression* InstantiatorImpl::visitCallExpr(CallExpr* node)
{
   SmallVector<Expression*, 4> ArgVec;
   ArgVec.reserve(node->getArgs().size());

   SmallVector<IdentifierInfo*, 4> Labels;
   Labels.reserve(node->getLabels().size());

   copyArgumentList(node->getArgs(), node->getLabels(), ArgVec, Labels);

   auto* func = node->getFunc();
   if (Expression* PE = dyn_cast_or_null<MemberRefExpr>(node->getParentExpr())) {
      PE = PE->getParentExpr();

      QualType lookupType
          = visit(SourceType(PE->getExprType()->removeReference()))
                ->removeMetaType();

      NamedDecl *decl;
      if (func->isImplOfProtocolRequirement() || func->isProtocolRequirement()) {
         decl = SP.Context.getProtocolImpl(lookupType->getRecord(), func);
      }
      else if (isa<MethodDecl>(func) && func->getRecord()->isTemplate()) {
         decl = SP.getInstantiator().getMemberInstantiation(
             lookupType->getRecord(), func,
             &lookupType->getRecord()->getTemplateArgs());
      }
      else {
         decl = func;
      }

      assert(decl && "no instantiated decl!");
      if (lookupType->isRecordType()) {
         decl
             = SP.maybeInstantiateTemplateMember(lookupType->getRecord(), decl);
      }

      func = cast<CallableDecl>(decl);
      func = SP.maybeInstantiateMemberFunction(func, node);
   }

   if (auto* templateArgs = node->getTemplateArgs()) {
      auto* instTemplateArgs = instantiateTemplateArgs(templateArgs, func);
      if (instTemplateArgs->isStillDependent()) {
         auto* callInst = SP.CreateCall(func, ArgVec, node->getSourceLoc());
         callInst->setTemplateArgs(instTemplateArgs);

         return callInst;
      }

      func = SP.getInstantiator().InstantiateCallable(
         func, instTemplateArgs, POI.getStart());

      if (!func) {
         return node;
      }
   }

   return SP.CreateCall(func, ArgVec, node->getSourceLoc());
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
      if (!C.BindingData.Decl) {
         return IfCondition((LocalVarDecl*)nullptr);
      }

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
      catchBlocks.emplace_back(copyOrNull(CB.varDecl), visit(CB.Body));
   }

   return DoStmt::Create(Context, node->getSourceRange(), visit(node->getBody()),
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

Decl* InstantiatorImpl::visitStaticIfDecl(StaticIfDecl* D)
{
   // instantiating a static if or for requires some more care since none of
   // the branches will have been Sema-checked if the condition is dependent,
   // however instantiation relies on everything being checked. If the
   // condition is dependent, mark the instantiation as dependent for now and
   // let Sema handle the instantiation once it determine which branch to take.
   if (D->isSemanticallyChecked()) {
      if (cast<il::ConstantInt>(D->getCondition()->getEvaluatedExpr())
              ->getBoolValue()) {
         return visit(D->getIfDecl());
      }

      return visit(D->getElseDecl());
   }

   StaticIfDecl* Template = D;
   if (auto PrevTemplate = D->getTemplate())
      Template = PrevTemplate;

   auto Decl = StaticIfDecl::Create(Context, D->getStaticLoc(), D->getRBRaceLoc(),
                                    clone(D->getCondition()), Template);

   return Decl;
}

Decl* InstantiatorImpl::visitStaticForDecl(StaticForDecl* node)
{
   if (node->isVariadic()) {
      return visitVariadicForDecl(node);
   }

   auto Decl = StaticForDecl::Create(
       Context, node->getStaticLoc(), node->getRBRaceLoc(),
       node->getElementName(), clone(node->getRange()),
       copyOrNull(node->getBodyDecl()));

   return Decl;
}

Statement* InstantiatorImpl::visitStaticIfStmt(StaticIfStmt* Stmt)
{
   if (Stmt->isSemanticallyChecked()) {
      if (cast<il::ConstantInt>(Stmt->getCondition()->getEvaluatedExpr())
          ->getBoolValue()) {
         return visit(Stmt->getIfBranch());
      }

      return visit(Stmt->getElseBranch());
   }

   StaticIfStmt* Template = Stmt;
   if (auto PrevTemplate = Stmt->getTemplate())
      Template = PrevTemplate;

   return StaticIfStmt::Create(Context, Stmt->getStaticLoc(), Stmt->getIfLoc(),
                               visitStaticExpr(Stmt->getCondition()), Template);
}

template<class CallbackFn>
void expandVariadicStmt(InstantiatorImpl& instantiator, ASTContext& Context,
                        NamedDecl* decl, DeclarationName elementName,
                        DeclarationName declName, CallbackFn&& callbackFn)
{
   TemplateParamDecl* param;
   bool isMeta;

   if (auto* localVar = dyn_cast<LocalVarDecl>(decl)) {
      isMeta = false;
      param = localVar->getType()->asTemplateParamType()->getParam();
   }
   else {
      isMeta = true;
      param = cast<AliasDecl>(decl)
                  ->getType()
                  ->removeMetaType()
                  ->asTemplateParamType()
                  ->getParam();
   }

   auto* subst = instantiator.hasTemplateArg(param);

   SmallVector<Statement*, 4> stmts;
   stmts.reserve(subst->getVariadicArgs().size());

   int i = 0;
   for (auto& VA : subst->getVariadicArgs()) {
      std::function<Expression*(SourceRange)> makeSubstExpr;
      if (isMeta) {
         makeSubstExpr = [&](SourceRange SR) {
            return new (Context)
                IdentifierRefExpr(SR, IdentifierKind::MetaType,
                                  Context.getMetaType(VA.getType()));
         };
      }
      else {
         DeclarationName instName
             = Context.getDeclNameTable().getPackExpansionName(declName, i);

         makeSubstExpr = [&](SourceRange SR) {
            return new (Context) IdentifierRefExpr(SR, instName);
         };
      }

      InstantiatorImpl::SubstContextRAII substContext(
          instantiator, param, elementName, i, move(makeSubstExpr));

      callbackFn();
      ++i;
   }
}

Expression*
InstantiatorImpl::visitVariadicExpansionExpr(VariadicExpansionExpr* Expr)
{
   SmallVector<Expression*, 2> Exprs;
   visitVariadicExpansionExpr(Expr, Exprs);

   auto* tup = TupleLiteral::Create(Context, Expr->getSourceRange(), Exprs);

   ConversionSequenceBuilder convSeq;
   convSeq.addStep(CastKind::ToEmptyTuple, Context.getEmptyTupleType());

   return ImplicitCastExpr::Create(
       Context, tup, ConversionSequence::Create(Context, convSeq));
}

void InstantiatorImpl::visitVariadicExpansionExpr(
    VariadicExpansionExpr* Expr, SmallVectorImpl<Expression*>& Exprs)
{
   auto* expandedExpr = Expr->getExpr();
   auto* decl = Expr->getParameterPack();
   DeclarationName declName = decl->getDeclName();

   expandVariadicStmt(*this, Context, Expr->getElementDecl(), declName,
                      declName, [&]() {
                         auto result = visit(expandedExpr);
                         if (result) {
                            Exprs.push_back(result);
                         }
                      });
}

Statement* InstantiatorImpl::visitVariadicForStmt(StaticForStmt* node)
{
   DeclarationName declName;
   if (auto* declRef = dyn_cast<DeclRefExpr>(node->getRange()->getExpr())) {
      declName = declRef->getDecl()->getDeclName();
   }
   else {
      declName = cast<MemberRefExpr>(node->getRange()->getExpr())
                     ->getMemberDecl()
                     ->getDeclName();
   }

   auto* decl = node->getVariadicDecl();

   DeclarationName elementName = node->getElementName();
   SmallVector<Statement*, 4> stmts;

   expandVariadicStmt(*this, Context, decl, elementName, declName, [&]() {
      auto bodyInst = visit(node->getBody());
      if (bodyInst) {
         stmts.push_back(bodyInst);
      }
   });

   return CompoundStmt::Create(Context, stmts, true,
                               node->getSourceRange().getStart(),
                               node->getSourceRange().getEnd());
}

Decl* InstantiatorImpl::visitVariadicForDecl(StaticForDecl* Decl)
{
   DeclarationName declName;
   if (auto* declRef = dyn_cast<DeclRefExpr>(Decl->getRange()->getExpr())) {
      declName = declRef->getDecl()->getDeclName();
   }
   else {
      llvm_unreachable("should not be possible");
   }

   auto* decl = Decl->getVariadicDecl();
   DeclarationName elementName = Decl->getElementName();

   auto* compoundDecl
       = CompoundDecl::Create(Context, Decl->getSourceLoc(), true);
   SP.ActOnDecl(&SP.getDeclContext(), compoundDecl);

   SemaPass::DeclContextRAII DCR(SP, compoundDecl);
   expandVariadicStmt(*this, Context, decl, elementName, declName,
                      [&]() { (void)visit(Decl->getBodyDecl()); });

   return compoundDecl;
}

Statement* InstantiatorImpl::visitStaticForStmt(StaticForStmt* node)
{
   if (node->isVariadic()) {
      return visitVariadicForStmt(node);
   }

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
   return FinalTemplateArgumentList::Create(SP.Context, list);
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
   return lookupExternalInstantiation<T>(
       MangledName,
       SP.getCompilerInstance().getModuleMgr().getMainModule()->getDecl());
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
   unsigned MaxDepth
       = SP.getCompilerInstance().getOptions().MaxInstantiationDepth;
   if (Depth > MaxDepth) {
      SP.diagnose(err_generic_error,
                  "maximum template instantiation depth ("
                      + std::to_string(MaxDepth) + ") exceeded",
                  POI);

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

TemplateInstantiator::TemplateInstantiator(SemaPass &SP)
   : SP(SP), QC(SP.QC)
{

}

NamedDecl*
TemplateInstantiator::getInstantiationImpl(NamedDecl *Template,
                                           FinalTemplateArgumentList *TemplateArgs)
{
   auto key = std::make_pair(Template, (uintptr_t)TemplateArgs);

   auto it = InstMap.find(key);
   if (it == InstMap.end()) {
      return nullptr;
   }

   return InstMap[key];
}

NamedDecl*
TemplateInstantiator::getMemberInstantiationImpl(RecordDecl *Inst,
                                                 NamedDecl *TemplateMember,
                                                 FinalTemplateArgumentList *TemplateArgs)
{
   auto key = std::make_pair(Inst, TemplateMember);

   auto it = MemberInstMap.find(key);
   if (it != MemberInstMap.end()) {
      auto it2 = it->getSecond().find(TemplateArgs);
      if (it2 != it->getSecond().end()) {
         return it2->getSecond();
      }
   }

   return nullptr;
}

void TemplateInstantiator::registerInstantiation(NamedDecl *Template,
                                                 FinalTemplateArgumentList *TemplateArgs,
                                                 NamedDecl *Inst)
{
   // FIXME
   if (isa<InitDecl>(Inst) && cast<InitDecl>(Inst)->isCompleteInitializer()) {
      if (!cast<InitDecl>(Template)->isCompleteInitializer()) {
         Template = cast<InitDecl>(Template)->getCompleteInit();
      }
   }
   if (isa<InitDecl>(Inst) && cast<InitDecl>(Inst)->isBaseInitializer()) {
      if (!cast<InitDecl>(Template)->isBaseInitializer()) {
         Template = cast<InitDecl>(Template)->getBaseInit();
      }
   }

   auto key = std::make_pair(Template, (uintptr_t)TemplateArgs);
   auto result = InstMap.try_emplace(key, Inst);

   (void)result;
   assert(result.second && "duplicate instantiation!");
}

bool TemplateInstantiator::PrepareForInstantiation(NamedDecl *ND)
{
   switch (ND->getKind()) {
   case Decl::StructDeclID:
   case Decl::ClassDeclID:
   case Decl::EnumDeclID: {
      auto *R = cast<RecordDecl>(ND);
      if (QC.DeclareImplicitInitializers(R)) {
         return true;
      }

      break;
   }
   case Decl::FunctionDeclID:
   case Decl::MethodDeclID:
   case Decl::InitDeclID:
   case Decl::DeinitDeclID:
      if (QC.PrepareDeclInterface(ND)) {
         return true;
      }

      // Make sure default values are instantiated.
      for (auto* Arg : cast<CallableDecl>(ND)->getArgs()) {
         if (auto Err = QC.TypecheckDecl(Arg)) {
            return true;
         }
      }

      break;
   case Decl::PropDeclID:
      if (auto Err = QC.PrepareDeclInterface(ND)) {
         return true;
      }

      if (auto* G = cast<PropDecl>(ND)->getGetterMethod()) {
         if (auto Err = PrepareForInstantiation(G)) {
            return true;
         }
      }

      if (auto* S = cast<PropDecl>(ND)->getSetterMethod()) {
         if (auto Err = PrepareForInstantiation(S)) {
            return true;
         }
      }

      break;
   case Decl::SubscriptDeclID:
      if (auto Err = QC.PrepareDeclInterface(ND)) {
         return true;
      }

      if (auto* G = cast<SubscriptDecl>(ND)->getGetterMethod()) {
         if (auto Err = PrepareForInstantiation(G)) {
            return true;
         }
      }

      if (auto* S = cast<SubscriptDecl>(ND)->getSetterMethod()) {
         if (auto Err = PrepareForInstantiation(S)) {
            return true;
         }
      }

      break;
   default:
      if (auto Err = QC.TypecheckDecl(ND)) {
         return true;
      }

      break;
   }

   if (ND->isInvalid()) {
      return true;
   }

   return false;
}

static bool CreateTemplateParamAliases(QueryContext &QC,
                                       NamedDecl *Template, DeclContext *Inst,
                                       FinalTemplateArgumentList *TemplateArgs)
{
   for (auto *Param : Template->getTemplateParams()) {
      auto *Arg = TemplateArgs->getArgForParam(Param);
      assert(Arg && "invalid template argument");

      if (Arg->isVariadic()) {
         continue;
      }

      SourceType T;
      StaticExpr *Expr;

      if (Arg->isType()) {
         T = QC.Context.getMetaType(Arg->getType());
         Expr = StaticExpr::Create(QC.Context,
             new(QC.Context) IdentifierRefExpr(Param->getSourceRange(),
                                               IdentifierKind::MetaType, T));

         Expr->setExprType(T);
      }
      else {
         T = Param->getCovariance();
         Expr = StaticExpr::Create(QC.Context, Arg->getValueType(),
                                   Param->getSourceRange(), Arg->getValue());
      }

      Expr->setSemanticallyChecked(true);

      auto *Alias = AliasDecl::Create(QC.Context, Param->getSourceLoc(),
          AccessSpecifier::Public,  Param->getDeclName(), T, Expr, {});

      QC.Sema->ActOnDecl(Inst, Alias);
   }

   return false;
}

RecordDecl*
TemplateInstantiator::InstantiateRecord(RecordDecl *Template,
                                        FinalTemplateArgumentList *TemplateArgs,
                                        SourceLocation PointOfInstantiation,
                                        RecordDecl *OuterInst)
{
   if (Template->isNestedTemplate()) {
      auto *RealTemplate = cast<RecordDecl>(Template->getOuterTemplate());
      auto *RealInst = InstantiateRecord(RealTemplate, TemplateArgs,
                                         PointOfInstantiation);

      if (!RealInst) {
         return nullptr;
      }

      return QC.LookupSingleAs<RecordDecl>(RealInst, Template->getDeclName());
   }

   if (auto *Inst = getInstantiation(Template, TemplateArgs)) {
      if (!Inst->getInstantiatedFrom()) {
         Inst->getInstantiationInfo()->instantiatedFrom = PointOfInstantiation;
      }

      return Inst;
   }

   bool shouldInstantiateShallowly = this->InstantiateShallowly;
   if (PrepareForInstantiation(Template)) {
      return nullptr;
   }

   // Check if this instantiation exists in an external module.
   if (auto* MF = Template->getModFile()) {
      MF->LoadAllDecls(*Template);
   }

   if (Template->isExternal()) {
      std::string MangledName;
      {
         // Mangling needs full function types to be resolved.
         if (QC.PrepareDeclInterface(cast<NamedDecl>(Template->getDeclContext()))) {
            return nullptr;
         }

         llvm::raw_string_ostream OS(MangledName);
         SP.getMangler().manglePrefix(Template, *TemplateArgs, OS);
      }

      if (auto Inst = lookupExternalInstantiation<RecordDecl>(MangledName, SP)) {
         Inst->setImportedInstantiation(true);
         return Inst;
      }

      if (auto *MF = Template->getModFile()) {
         MF->LoadAllDecls(*Template);
      }
   }

   unsigned Depth = ::getInstantiationDepth(Template, TemplateArgs);
   if (checkDepth(SP, PointOfInstantiation, Depth)) {
      return nullptr;
   }

   // Instantiate the interface only for now.
   InstantiatorImpl Instantiator(SP, *TemplateArgs, Template,
       nullptr, QualType(), PointOfInstantiation);

   auto* Inst = Instantiator.instantiateRecordDecl(Template);

   // Remember the scope in which this instantiation was requested.
   auto* InstScope = SP.getCurrentDecl();
   auto InstInfo = new (QC.Context) InstantiationInfo<RecordDecl>(
      PointOfInstantiation, TemplateArgs, Template, Depth,
      OuterInst);

   Inst->setInstantiationInfo(InstInfo);
   Inst->setIsInstantiation(true);
   SP.registerInstantiation(Inst, InstScope);

   if (OuterInst) {
      // Declaration was visible under the new name.
      OuterInst->removeVisibleDecl(Template, Inst->getDeclName());
      OuterInst->removeVisibleDecl(Template, Template->getDeclName());
      QC.Sema->ActOnDecl(OuterInst, Inst);

      (void) OuterInst->makeDeclAvailable(Template->getDeclName(), Inst);
   }
   else {
      Instantiator.ActOnDecl(Inst, Template->getDeclContext());
   }

   if (CreateTemplateParamAliases(QC, Template, Inst, TemplateArgs)) {
      return nullptr;
   }

   registerInstantiation(Template, TemplateArgs, Inst);

   LOG(Instantiations, "instantiated record '", Inst->getFullName(),
       "', requested from ",
       QC.CI.getFileMgr().getSourceLocationAsString(PointOfInstantiation),
       shouldInstantiateShallowly ? " (shallow)" : "");

   // Instantiate inner records.
   SmallVector<Decl*, 2> decls;
   for (auto *Inner : Template->getDecls<RecordDecl>()) {
      decls.push_back(Inner);
   }

   for (auto *Ext : Template->getExtensions()) {
      for (auto *Inner : Ext->getDecls<RecordDecl>()) {
         decls.push_back(Inner);
      }
   }

   for (auto *Inner : decls) {
      auto *R = cast<RecordDecl>(Inner);
      if (R->isTemplate()) {
         continue;
      }

      InstantiateRecord(R, TemplateArgs, PointOfInstantiation, Inst);
   }

   // Now instantiate meta declarations.
   decls.clear();
   for (auto* D : Template->getDecls<Decl>()) {
      switch (D->getKind()) {
      case Decl::StaticIfDeclID:
      case Decl::StaticForDeclID:
      case Decl::StaticAssertDeclID:
      case Decl::StaticPrintDeclID: {
         decls.push_back(D);
         break;
      }
      default:
         break;
      }
   }

   if (!decls.empty()) {
      Instantiator.setSelfType(QC.Context.getRecordType(Inst));

      SemaPass::DeclContextRAII DCR(*QC.Sema, Inst);
      for (auto* metaDecl : decls) {
         auto* inst = Instantiator.instantiateDecl(metaDecl);
         if (!inst) {
            continue;
         }

         if (auto* compoundDecl = dyn_cast<CompoundDecl>(inst)) {
            for (auto* innerCompoundDecl :
                compoundDecl->getDecls<CompoundDecl>()) {
               for (auto* innerDecl :
                   innerCompoundDecl->getDecls<NamedDecl>()) {
                  QC.Sema->makeDeclAvailable(*Inst, innerDecl);
               }
            }
         }
      }
   }

   if (shouldInstantiateShallowly) {
      ShallowInstantiations.insert(Inst);
      QC.Sema->FindDependencies(Inst);
      return Inst;
   }

   completeShallowInstantiation(Inst);
   return Inst;
}

bool TemplateInstantiator::completeShallowInstantiations()
{
   bool error = false;
   while (!ShallowInstantiations.empty()) {
      auto *Next = *ShallowInstantiations.begin();
      error |= completeShallowInstantiation(Next);
   }

   return error;
}

bool TemplateInstantiator::completeShallowInstantiation(RecordDecl *Inst)
{
   bool wasShallow = ShallowInstantiations.remove(Inst);

   auto *Info = Inst->getInstantiationInfo();
   auto *Template = Info->specializedTemplate;

   // Make all protocol requirements visible.
   auto *ProtocolImpls = QC.Context.getProtocolImpls(Template);
   if (ProtocolImpls != nullptr) {
      for (auto [req, impl] : *ProtocolImpls) {
         if (isa<AssociatedTypeDecl>(req)) {
            continue;
         }

         QC.Context.addProtocolImpl(Inst, req, impl);
      }
   }

   if (auto *MF = Template->getModFile()) {
      MF->LoadAllDecls(*Template);
   }

   // Make all declarations visible without instantiating them.
   SmallVector<NamedDecl*, 2> VisibleDecls;
   for (auto* ND : Template->getDecls<NamedDecl>()) {
      VisibleDecls.push_back(ND);
   }

   // Check which extensions apply, and make their declarations available too.
   for (auto* Ext : Template->getExtensions()) {
      bool Applies;
      if (QC.CheckTemplateExtensionApplicability(Applies, Inst, Ext)) {
         continue;
      }

      if (!Applies)
         continue;

      if (auto *MF = Ext->getModFile()) {
         MF->LoadAllDecls(*Ext);
      }

      for (auto* ND : Ext->getDecls<NamedDecl>()) {
         VisibleDecls.push_back(ND);
      }
   }

   for (auto *ND : VisibleDecls) {
      if (ND->isSynthesized() || ND->instantiatedFromProtocolDefaultImpl()) {
         continue;
      }

      // Don't instantiate template parameters and associated type impls.
      if (isa<TemplateParamDecl>(ND)
          || (isa<AliasDecl>(ND) && ND->isImplOfProtocolRequirement())
          || (isa<RecordDecl>(ND) && !ND->isTemplate())) {
         continue;
      }

      SP.makeDeclAvailable(*Inst, getNameFor(QC.Context, ND, Inst), ND);

      if (auto *Sub = dyn_cast<SubscriptDecl>(ND)) {
         if (auto *Get = Sub->getGetterMethod()) {
            SP.makeDeclAvailable(*Inst, Get->getDeclName(), Get);
         }
         if (auto *Set = Sub->getSetterMethod()) {
            SP.makeDeclAvailable(*Inst, Set->getDeclName(), Set);
         }
      }
      else if (auto *Prop = dyn_cast<PropDecl>(ND)) {
         if (auto *Get = Prop->getGetterMethod()) {
            SP.makeDeclAvailable(*Inst, Get->getDeclName(), Get);
         }
         if (auto *Set = Prop->getSetterMethod()) {
            SP.makeDeclAvailable(*Inst, Set->getDeclName(), Set);
         }
      }
   }

   // Instantiate fields and cases.
   if (auto* S = dyn_cast<StructDecl>(Inst)) {
      if (InstantiateFields(S)) {
         return true;
      }
   }
   else if (auto* E = dyn_cast<EnumDecl>(Inst)) {
      if (InstantiateCases(E)) {
         return true;
      }
   }

   if (!wasShallow) {
      if (QC.Sema->PrepareNameLookup(Inst)) {
         return true;
      }
   }

   // Instantiate the deinitializer.
   if (auto *Deinit = Template->getDeinitializer()) {
      if (auto *LazyFnInfo = Deinit->getLazyFnInfo()) {
         LazyFnInfo->loadBody(Deinit);
      }

      if (Deinit->getBody() != nullptr) {
         auto *DeinitInst = QC.Sema->maybeInstantiateTemplateMember(
             Inst, Deinit);

         Inst->setDeinitializer(DeinitInst);
         if (DeinitInst) {
            QC.Sema->maybeInstantiateMemberFunction(DeinitInst, Inst);
         }
      }
   }

   if (QC.TypecheckDecl(Inst)) {
      return true;
   }

   QC.Sema->updateLookupLevel(Inst, LookupLevel::Complete);
   return false;
}

NamedDecl*
TemplateInstantiator::InstantiateTemplateMember(NamedDecl *TemplateMember,
                                                RecordDecl *Inst,
                                                FinalTemplateArgumentList *TemplateArgs,
                                                SourceLocation PointOfInstantiation,
                                                bool instantiateAccessorMethods)
{
   if (isa<TemplateParamDecl>(TemplateMember)) {
      return TemplateMember;
   }

   auto key = std::make_pair(Inst, TemplateMember);

   auto it = MemberInstMap.find(key);
   if (it != MemberInstMap.end()) {
      auto it2 = it->getSecond().find(TemplateArgs);
      if (it2 != it->getSecond().end()) {
         return it2->getSecond();
      }
   }

   if (PrepareForInstantiation(TemplateMember)) {
      return nullptr;
   }

   if (auto* C = dyn_cast<CallableDecl>(TemplateMember)) {
      if (C->getBodyTemplate() && PrepareForInstantiation(C->getBodyTemplate())) {
         return nullptr;
      }
   }

   // Instantiate declarations provided by protocol extensions.
   if (isa<ProtocolDecl>(TemplateMember->getRecord())) {
      if (Inst->isTemplate()) {
         return TemplateMember;
      }

      NamedDecl* Result = InstantiateProtocolDefaultImpl(
         TemplateMember, QC.Context.getRecordType(Inst), false);

      if (!Result) {
         return nullptr;
      }

      Inst->removeVisibleDecl(TemplateMember, Result->getDeclName());
      Inst->removeVisibleDecl(TemplateMember, TemplateMember->getDeclName());
      QC.Sema->ActOnDecl(Inst, Result);

      return Result;
   }

   sema::FinalTemplateArgumentList *OrigTemplateArgs = TemplateArgs;
   if (!TemplateArgs) {
      TemplateArgs = &Inst->getTemplateArgs();
   }
   if (!PointOfInstantiation) {
      PointOfInstantiation = Inst->getInstantiatedFrom();
   }

   NamedDecl* MemberInst;
   if (auto* R = dyn_cast<RecordDecl>(TemplateMember)) {
      if (InstantiateRecord(R, TemplateArgs, PointOfInstantiation)) {
         return nullptr;
      }

      MemberInst = R;

      if (MemberInst == TemplateMember) {
         return MemberInst;
      }
      if (!MemberInst) {
         return nullptr;
      }
   }
   else {
      InstantiatorImpl Instantiator(
         *QC.Sema, *TemplateArgs, TemplateMember->getRecord(),
         TemplateMember->getRecord(),
         QC.Context.getRecordType(Inst), PointOfInstantiation);

      MemberInst = cast_or_null<NamedDecl>(
         Instantiator.instantiateDecl(TemplateMember));

      if (MemberInst == TemplateMember) {
         return MemberInst;
      }
      if (!MemberInst) {
         return nullptr;
      }

      // Declaration was visible under the new name.
      Inst->removeVisibleDecl(TemplateMember, MemberInst->getDeclName());
      Inst->removeVisibleDecl(TemplateMember, TemplateMember->getDeclName());

      QC.Sema->ActOnDecl(Inst, MemberInst);
   }

   MemberInstMap[key][OrigTemplateArgs] = MemberInst;
   QC.Context.setConstraints(MemberInst, QC.Context.getExtConstraints(TemplateMember));

   if (TemplateMember->isImplOfProtocolRequirement()) {
      MemberInst->setImplOfProtocolRequirement(true);
      QC.Context.updateProtocolImpl(Inst, TemplateMember, MemberInst);
   }

   if (TemplateMember->instantiatedFromProtocolDefaultImpl()) {
      MemberInst->setInstantiatedFromProtocolDefaultImpl(true);
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

   if (isa<FieldDecl>(MemberInst)) {
      if (QC.TypecheckDecl(MemberInst)) {
         return nullptr;
      }
   }

   // Instantiate the full subscript if a single accessor is used.
   if (auto *M = dyn_cast<MethodDecl>(MemberInst)) {
      if (M->isSubscript()) {
         auto *Sub = QC.Sema->getSubscriptDecl(cast<MethodDecl>(TemplateMember));
         auto *SubInst = cast_or_null<SubscriptDecl>(
             InstantiateTemplateMember(Sub, Inst, TemplateArgs,
                                       PointOfInstantiation, false));

         if (!SubInst) {
            return nullptr;
         }

         if (M->getDeclName().getSubscriptKind() == DeclarationName::SubscriptKind::Getter) {
            assert(!SubInst->getGetterMethod());
            SubInst->setGetterMethod(M);
         }
         else {
            assert(!SubInst->getSetterMethod());
            SubInst->setSetterMethod(M);
         }
      }
   }
   else if (auto *SubInst = dyn_cast<SubscriptDecl>(MemberInst)) {
      if (!instantiateAccessorMethods) {
         return MemberInst;
      }

      auto *Sub = cast<SubscriptDecl>(TemplateMember);
      if (Sub->hasGetter() && !SubInst->hasGetter()) {
         InstantiateTemplateMember(Sub->getGetterMethod(), Inst,
                                   TemplateArgs, PointOfInstantiation);
      }
      if (Sub->hasSetter() && !SubInst->hasSetter()) {
         InstantiateTemplateMember(Sub->getSetterMethod(), Inst,
                                   TemplateArgs, PointOfInstantiation);
      }
   }

   return MemberInst;
}

CallableDecl*
TemplateInstantiator::InstantiateCallable(CallableDecl *Template,
                                          FinalTemplateArgumentList *TemplateArgs,
                                          SourceLocation PointOfInstantiation)
{
   if (auto* F = dyn_cast<FunctionDecl>(Template)) {
      return InstantiateFunction(F, TemplateArgs, PointOfInstantiation);
   }

   return InstantiateMethod(cast<MethodDecl>(Template), TemplateArgs, PointOfInstantiation);
}

FunctionDecl*
TemplateInstantiator::InstantiateFunction(FunctionDecl *Template,
                                          FinalTemplateArgumentList *TemplateArgs,
                                          SourceLocation PointOfInstantiation)
{
   if (auto *Inst = getInstantiation(Template, TemplateArgs)) {
      return Inst;
   }

   if (PrepareForInstantiation(Template)) {
      return nullptr;
   }

   std::string MangledName;
   {
      llvm::raw_string_ostream OS(MangledName);
      SP.getMangler().manglePrefix(Template, *TemplateArgs, OS);
   }

   if (auto Inst = lookupExternalInstantiation<FunctionDecl>(MangledName, SP)) {
      Inst->setImportedInstantiation(true);
      return Inst;
   }

   unsigned Depth = ::getInstantiationDepth(Template, TemplateArgs);
   if (checkDepth(SP, PointOfInstantiation, Depth)) {
      return nullptr;
   }

   auto* InstScope = SP.getCurrentDecl();

   InstantiatorImpl Instantiator(SP, *TemplateArgs, Template, nullptr,
                                 QualType(), PointOfInstantiation);

   auto Inst = Instantiator.instantiateFunctionDecl(Template);
   if (!Inst) {
      return nullptr;
   }

   // Remember where this template was instantiated.
   auto instInfo = new (SP.Context) InstantiationInfo<CallableDecl>(
      PointOfInstantiation, TemplateArgs, Template, Depth);

   Inst->setInstantiationInfo(instInfo);
   Inst->setIsInstantiation(true);

   SP.registerInstantiation(Inst, InstScope);
   registerInstantiation(Template, TemplateArgs, Inst);

   Instantiator.ActOnDecl(Inst, Template->getDeclContext());

   if (CreateTemplateParamAliases(QC, Template, Inst, TemplateArgs)) {
      return nullptr;
   }

   LOG(Instantiations, "instantiated function '", Inst->getFullName(),
       "', requested from ",
       QC.CI.getFileMgr().getSourceLocationAsString(PointOfInstantiation));

   return Inst;
}

MethodDecl*
TemplateInstantiator::InstantiateMethod(MethodDecl *Template,
                                        FinalTemplateArgumentList *TemplateArgs,
                                        SourceLocation PointOfInstantiation)
{
   // Only instantiate the complete initializer, the base initializer will be
   // synthesized afterwards.
   bool wasBaseInit = Template->isBaseInitializer();
   if (wasBaseInit) {
      Template = cast<InitDecl>(Template)->getCompleteInit();
   }

   if (auto *Inst = getInstantiation(Template, TemplateArgs)) {
      return wasBaseInit ? cast<InitDecl>(Inst)->getBaseInit() : Inst;
   }

   if (PrepareForInstantiation(Template)) {
      return nullptr;
   }

   SemaPass::DeclScopeRAII DSR(SP, Template->getDeclContext());

   std::string MangledName;
   {
      llvm::raw_string_ostream OS(MangledName);
      SP.getMangler().manglePrefix(Template, *TemplateArgs, OS);
   }

   if (auto Inst = lookupExternalInstantiation<MethodDecl>(MangledName, SP)) {
      Inst->setImportedInstantiation(true);
      return wasBaseInit ? cast<InitDecl>(Inst)->getBaseInit() : Inst;
   }

   unsigned Depth = ::getInstantiationDepth(Template, TemplateArgs);
   if (checkDepth(SP, PointOfInstantiation, Depth)) {
      return nullptr;
   }

   if (auto BT = Template->getBodyTemplate()) {
      if (auto Info = BT->getLazyFnInfo()) {
         // if the function is a template, we also need it's body
         Info->loadBody(BT);
      }

      if (PrepareForInstantiation(BT)) {
         return nullptr;
      }
   }
   else {
      if (PrepareForInstantiation(Template)) {
         return nullptr;
      }
   }

   auto* InstScope = SP.getCurrentDecl();

   MultiLevelFinalTemplateArgList MultiLevelList(*TemplateArgs);
   if (Template->getRecord()->isInstantiation()) {
      MultiLevelList.addOuterList(Template->getRecord()->getTemplateArgs());
   }

   InstantiatorImpl Instantiator(SP, move(MultiLevelList), Template, nullptr,
                                 QualType(), PointOfInstantiation);

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
      return nullptr;
   }

   auto instInfo = new (SP.Context) InstantiationInfo<CallableDecl>(
      PointOfInstantiation, TemplateArgs, Template, Depth);

   Inst->setInstantiationInfo(instInfo);
   Inst->setIsInstantiation(true);

   SP.registerInstantiation(Inst, InstScope);
   registerInstantiation(Template, TemplateArgs, Inst);

   Instantiator.ActOnDecl(Inst, Template->getDeclContext());

   if (CreateTemplateParamAliases(QC, Template, Inst, TemplateArgs)) {
      return nullptr;
   }

   LOG(Instantiations, "instantiated method '", Inst->getFullName(),
       "', requested from ",
       QC.CI.getFileMgr().getSourceLocationAsString(PointOfInstantiation));

   if (auto *Init = dyn_cast<InitDecl>(Inst)) {
      assert(Init->isCompleteInitializer());
      QC.CreateBaseInit(Init);

      if (CreateTemplateParamAliases(QC, cast<InitDecl>(Template)->getBaseInit(),
                                     Init->getBaseInit(), TemplateArgs)) {
         return nullptr;
      }
   }

   return wasBaseInit ? cast<InitDecl>(Inst)->getBaseInit() : Inst;
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
   return Instantiator.instantiateStatement(stmt);
}

Decl*
TemplateInstantiator::InstantiateDecl(Decl *Template,
                                      MultiLevelFinalTemplateArgList *TemplateArgs,
                                      SourceLocation POI)
{
   // Make a copy of the template argument list.
   auto Args = *TemplateArgs;

   InstantiatorImpl Instantiator(*QC.Sema, move(Args));
   return Instantiator.instantiateDecl(Template);
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

bool TemplateInstantiator::InstantiateFunctionBody(CallableDecl *Inst)
{
   if (Inst->getBody() != nullptr) {
      return false;
   }

   assert(Inst->getBodyTemplate() && "function does not need instantiation");

   if (auto Err = QC.TypecheckDecl(Inst->getBodyTemplate())) {
      return true;
   }
   if (auto Err = QC.TypecheckDecl(Inst)) {
      return true;
   }

   if (Inst->getBodyTemplate()->isInvalid()) {
      Inst->setIsInvalid(true);
      return true;
   }

   if (!Inst->getBodyTemplate()->willHaveDefinition()) {
      assert(Inst->getBodyTemplate()->hasAttribute<_BuiltinAttr>());
      return false;
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

   NamedDecl *InstScope = QC.Sema->getInstantiationScope(Inst);
   if (!InstScope) {
      InstScope = QC.Sema->getCurrentDecl();
   }

   QualType Self;
   if (auto* M = dyn_cast<MethodDecl>(Inst)) {
      Self = QC.Context.getRecordType(M->getRecord());
   }

   InstantiatorImpl Instantiator(*QC.Sema, move(ArgList), StmtOrDecl(), Inst,
                                 {}, Self);

   auto BodyInst = Instantiator.instantiateFunctionBody(Inst);
   Inst->setBody(BodyInst);

   bool error = false;
   {
      SemaPass::DeclScopeRAII raii(*QC.Sema, Inst);
      SemaPass::ScopeGuard scope(*QC.Sema, Inst);
      SemaPass::InStaticContextRAII StaticCtx(
          *QC.Sema, Inst->isStatic() && !isa<InitDecl>(Inst));

      auto res = QC.Sema->visitStmt(Inst, BodyInst);
      if (res) {
         Inst->setBody(res.get());
      }

      if (!res || BodyInst->isInvalid()) {
         error = true;
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
   return error;
}

AliasDecl*
TemplateInstantiator::InstantiateAlias(AliasDecl *Template,
                                       FinalTemplateArgumentList *TemplateArgs,
                                       SourceLocation PointOfInstantiation)
{
   if (auto *Inst = getInstantiation(Template, TemplateArgs)) {
      return Inst;
   }

   if (PrepareForInstantiation(Template)) {
      return nullptr;
   }

   SemaPass::DeclScopeRAII DSR(SP, Template->getDeclContext());

   if (Template->isExternal()) {
      std::string MangledName;
      {
         llvm::raw_string_ostream OS(MangledName);
         SP.getMangler().manglePrefix(Template, *TemplateArgs, OS);
      }

      if (auto Inst = lookupExternalInstantiation<AliasDecl>(MangledName, SP)) {
         Inst->setImportedInstantiation(true);
         return Inst;
      }
   }

   unsigned Depth = ::getInstantiationDepth(Template, TemplateArgs);
   if (checkDepth(SP, PointOfInstantiation, Depth)) {
      return nullptr;
   }

   auto* InstScope = SP.getCurrentDecl();
   auto* InstInfo = new (SP.Context) InstantiationInfo<AliasDecl>(
      PointOfInstantiation, TemplateArgs, Template, Depth);

   InstantiatorImpl Instantiator(SP, *InstInfo->templateArgs, Template,
                                 nullptr, QualType(), PointOfInstantiation);

   auto Inst = Instantiator.instantiateAliasDecl(Template);
   if (!Inst) {
      return nullptr;
   }

   Inst->setInstantiationInfo(InstInfo);
   Inst->setIsInstantiation(true);

   SP.registerInstantiation(Inst, InstScope);
   registerInstantiation(Template, TemplateArgs, Inst);

   Instantiator.ActOnDecl(Inst, Template->getDeclContext());

   if (CreateTemplateParamAliases(QC, Template, Inst, TemplateArgs)) {
      return nullptr;
   }

   LOG(Instantiations, "instantiated alias '", Inst->getFullName(),
       "', requested from ",
       QC.CI.getFileMgr().getSourceLocationAsString(PointOfInstantiation));

   return Inst;
}
