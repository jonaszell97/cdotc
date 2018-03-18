//
// Created by Jonas Zell on 05.12.17.
//

#include "TemplateInstantiator.h"

#include "AST/Passes/SemanticAnalysis/Template.h"
#include "AST/Passes/SemanticAnalysis/SemaPass.h"
#include "AST/Passes/AbstractPass.h"
#include "AST/ASTContext.h"
#include "Message/Diagnostics.h"

#include <sstream>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/ErrorHandling.h>
#include <llvm/ADT/SmallString.h>

using namespace cdot::ast;
using namespace cdot::support;
using namespace cdot::diag;
using namespace cdot::sema;

namespace cdot {
namespace {

class InstantiatorImpl {
public:
   InstantiatorImpl(SemaPass &SP, MultiLevelTemplateArgList const& templateArgs)
      : SP(SP), Context(SP.getContext()), InstScope(SP, &SP.getDeclContext()),
        templateArgs(&templateArgs)
   {

   }

   InstantiatorImpl(SemaPass &SP,
                    MultiLevelTemplateArgList const& templateArgs,
                    NamedDecl* Template)
      : SP(SP), Context(SP.getContext()),
        InstScope(SP, Template->getDeclContext()),
        templateArgs(&templateArgs), Template(Template),
        templateArgString(templateArgs.innermost().toString())
   {

   }

   InstantiatorImpl(SemaPass &SP,
                    llvm::StringRef SubstName,
                    QualType SubstTy,
                    const Variant &SubstVal)
      : SP(SP), Context(SP.getContext()), InstScope(SP, &SP.getDeclContext()),
        ValueSubst{ SubstName, SubstTy, &SubstVal }
   {

   }

   static bool canElideInstantiation(Statement *Stmt)
   {
      if (auto Decl = dyn_cast<DeclStmt>(Stmt)) {
         return canElideInstantiation(Decl->getDecl());
      }

      return !Stmt->isDependent() && Stmt->isSemanticallyChecked();
   }

   static bool canElideInstantiation(Expression *E)
   {
      if (!E->getExprType() || E->getExprType()->isUnknownAnyType())
         return false;

      return !E->isDependent() && E->isSemanticallyChecked();
   }

   static bool canElideInstantiation(Decl *D)
   {
      if (D->isDependent() || !D->isSemanticallyChecked())
         return false;

      if (isa<NamedDecl>(D)) {
         switch (D->getKind()) {
         case Decl::LocalVarDeclID:
         case Decl::FuncArgDeclID:
            return true;
         default:
            return false;
         }
      }

      return true;
   }

   Expression* visit(Expression *expr)
   {
      if (canElideInstantiation(expr))
         return expr;

      switch (expr->getTypeID()) {
#        define CDOT_EXPR(Name)                                \
            case AstNode::Name##ID:                            \
               return visit##Name(static_cast<Name*>(expr));
#        include "AST/AstNode.def"

         default:
            llvm_unreachable("not an expression!");
      }
   }

   Statement* visit(Statement *stmt)
   {
      if (auto E = dyn_cast<Expression>(stmt))
         return visit(E);

      if (canElideInstantiation(stmt))
         return stmt;

      switch (stmt->getTypeID()) {
#        define CDOT_STMT(Name)                                \
            case AstNode::Name##ID:                            \
               return visit##Name(static_cast<Name*>(stmt));
#        include "AST/AstNode.def"

         default:
            llvm_unreachable("not a statement!");
      }
   }

   Decl *visit(Decl *D)
   {
      if (canElideInstantiation(D))
         return D;

      switch (D->getKind()) {
#        define CDOT_DECL(Name)                                \
            case Decl::Name##ID:                               \
               return visit##Name(static_cast<Name*>(D));
#        include "AST/Decl.def"

      default:
         llvm_unreachable("not a decl!");
      }
   }

   SourceType visit(const SourceType &Ty)
   {
      if (Ty.isResolved()) {
         auto ResolvedTy = Ty.getResolvedType();
         if (!ResolvedTy->isDependentType())
            return Ty;

         if (!ResolvedTy->isUnknownAnyType() && templateArgs)
            return SourceType(SP.resolveDependencies(Ty, *templateArgs,
                                                     Ty.getTypeExpr()));
      }

      if (auto E = Ty.getTypeExpr()) {
         return SourceType(visit(Ty.getTypeExpr()));
      }

      return SourceType();
   }

   SourceType visitOrAuto(const SourceType &Ty)
   {
      auto Result = visit(Ty);
      if (!Result)
         return SourceType(Context.getAutoType());

      return Result;
   }

   CompoundDecl* visitCompoundDecl(CompoundDecl *D);
   CompoundStmt* visitCompoundStmt(CompoundStmt *node);
   TranslationUnit* visitTranslationUnit(TranslationUnit *node)
   {
      llvm_unreachable("can't instantiate translation unit!");
   }

   NamespaceDecl* visitNamespaceDecl(NamespaceDecl *node)
   {
      llvm_unreachable("should not be in a template!");
   }

   UsingStmt* visitUsingStmt(UsingStmt *node);
   ModuleStmt* visitModuleStmt(ModuleStmt *node);
   ImportStmt* visitImportStmt(ImportStmt *node);

   LocalVarDecl* visitLocalVarDecl(LocalVarDecl *node);
   GlobalVarDecl* visitGlobalVarDecl(GlobalVarDecl *node);

   LocalDestructuringDecl*
   visitLocalDestructuringDecl(LocalDestructuringDecl *node);

   GlobalDestructuringDecl*
   visitGlobalDestructuringDecl(GlobalDestructuringDecl *node);

   FunctionDecl* visitFunctionDecl(FunctionDecl *F);

   CallableDecl* visitCallableDecl(CallableDecl *node)
   {
      llvm_unreachable("call visitFunctionDecl or visitMethodDecl");
   }

   RecordDecl *instantiateRecordDecl(RecordDecl *src);
   RecordDecl *instantiateRecordDefinition(RecordDecl *Template,
                                           RecordDecl *Inst);

   RecordDecl *visitRecordCommon(RecordDecl *Template);

   RecordDecl* visitRecordDecl(RecordDecl *node)
   {
      RecordDecl* res;
      switch (node->getKind()) {
         case Decl::ClassDeclID:
            res = visitClassDecl(cast<ClassDecl>(node));
            break;
         case Decl::StructDeclID:
            res = visitStructDecl(cast<StructDecl>(node));
            break;
         case Decl::EnumDeclID:
            res = visitEnumDecl(cast<EnumDecl>(node));
            break;
         case Decl::UnionDeclID:
            res = visitUnionDecl(cast<UnionDecl>(node));
            break;
         case Decl::ProtocolDeclID:
            res = visitProtocolDecl(cast<ProtocolDecl>(node));
            break;
         case Decl::ExtensionDeclID:
            res = visitExtensionDecl(cast<ExtensionDecl>(node));
            break;
         default:
            llvm_unreachable("bad record decl kind");
      }

      return res;
   }

   TemplateParamDecl *visitTemplateParamDecl(TemplateParamDecl *decl);

   ClassDecl* visitClassDecl(ClassDecl *node);
   StructDecl* visitStructDecl(StructDecl *node);
   ProtocolDecl* visitProtocolDecl(ProtocolDecl *node);
   ExtensionDecl* visitExtensionDecl(ExtensionDecl *node);
   EnumDecl* visitEnumDecl(EnumDecl *node);
   UnionDecl* visitUnionDecl(UnionDecl *node);

   EnumCaseDecl* visitEnumCaseDecl(EnumCaseDecl *node);

   FieldDecl* visitFieldDecl(FieldDecl *node);
   PropDecl* visitPropDecl(PropDecl *node, bool skipBody = true);
   AssociatedTypeDecl* visitAssociatedTypeDecl(AssociatedTypeDecl *node);

   MethodDecl* visitMethodDecl(MethodDecl *M, bool skipBody = true);
   InitDecl* visitInitDecl(InitDecl *node, bool skipBody = true);
   DeinitDecl* visitDeinitDecl(DeinitDecl *node, bool skipBody = true);

   FuncArgDecl* visitFuncArgDecl(FuncArgDecl *node);
   TypedefDecl* visitTypedefDecl(TypedefDecl *node);
   AliasDecl* visitAliasDecl(AliasDecl *Alias);

   Expression* visitParenExpr(ParenExpr *Expr);
   Expression* visitIdentifierRefExpr(IdentifierRefExpr *Ident);
   BuiltinIdentExpr* visitBuiltinIdentExpr(BuiltinIdentExpr *node);
   SelfExpr* visitSelfExpr(SelfExpr *node);
   SuperExpr* visitSuperExpr(SuperExpr *node);

   Expression* visitBuiltinExpr(BuiltinExpr *node);

   SubscriptExpr* visitSubscriptExpr(SubscriptExpr *node);
   Expression* visitCallExpr(CallExpr *node);
   MemberRefExpr* visitMemberRefExpr(MemberRefExpr *node);
   EnumCaseExpr* visitEnumCaseExpr(EnumCaseExpr *node);
   TupleMemberExpr* visitTupleMemberExpr(TupleMemberExpr *node);

   ForStmt* visitForStmt(ForStmt *node);
   ForInStmt* visitForInStmt(ForInStmt *node);
   WhileStmt* visitWhileStmt(WhileStmt *node);
   IfStmt* visitIfStmt(IfStmt *node);
   LabelStmt* visitLabelStmt(LabelStmt *node);
   GotoStmt* visitGotoStmt(GotoStmt *node);

   MatchStmt* visitMatchStmt(MatchStmt *node);
   CaseStmt* visitCaseStmt(CaseStmt *node);
   ExpressionPattern* visitExpressionPattern(ExpressionPattern *node);

   IsPattern* visitIsPattern(IsPattern *node);
   CasePattern* visitCasePattern(CasePattern *node);

   ReturnStmt* visitReturnStmt(ReturnStmt *node);
   BreakStmt* visitBreakStmt(BreakStmt *node);
   ContinueStmt* visitContinueStmt(ContinueStmt *node);

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
   StaticIfDecl* visitStaticIfDecl(StaticIfDecl *node);
   StaticForDecl* visitStaticForDecl(StaticForDecl *node);

   StaticIfStmt* visitStaticIfStmt(StaticIfStmt *node);
   StaticForStmt* visitStaticForStmt(StaticForStmt *node);

   AttributedStmt *visitAttributedStmt(AttributedStmt *S);
   AttributedExpr *visitAttributedExpr(AttributedExpr *E);

   TryStmt* visitTryStmt(TryStmt *node);
   ThrowStmt* visitThrowStmt(ThrowStmt *node);

   LambdaExpr* visitLambdaExpr(LambdaExpr *node);

   Expression* visitImplicitCastExpr(ImplicitCastExpr *node);

   DebugStmt* visitDebugStmt(DebugStmt *node);
   NullStmt* visitNullStmt(NullStmt *node);

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

private:
   SemaPass &SP;
   ASTContext &Context;
   SemaPass::DeclScopeRAII InstScope;

   const MultiLevelTemplateArgList *templateArgs = nullptr;
   NamedDecl* Template = nullptr;

   llvm::StringMap<ResolvedTemplateArg*> VariadicTemplateArgs;

   bool InUnevaluatedScope = false;
   string templateArgString;

   struct {
      llvm::StringRef Name;
      QualType Ty;
      const Variant *Val = nullptr;
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
                   llvm::StringRef FuncArg,
                   llvm::StringRef IdentSubst,
                   ResolvedTemplateArg *ArgSubst)
         : Param(Param), FuncArg(FuncArg), IdentSubst(IdentSubst),
           ArgSubst(ArgSubst)
      { }

      TemplateParamDecl *Param;
      llvm::StringRef FuncArg;

      llvm::StringRef IdentSubst;
      ResolvedTemplateArg *ArgSubst;
   };

   struct SubstContextRAII {
      SubstContextRAII(InstantiatorImpl &Inst,
                       TemplateParamDecl *Param,
                       llvm::StringRef FuncArg,
                       llvm::StringRef IdentSubst,
                       ResolvedTemplateArg *ArgSubst)
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

   llvm::StringRef getVariadicArgumentSubst(llvm::StringRef name)
   {
      auto end_it = SubstContexts.rend();
      for (auto it = SubstContexts.rbegin(); it != end_it; ++it) {
         auto &Subst = *it;
         if (Subst.FuncArg.empty())
            continue;

         if (Subst.FuncArg == name)
            return Subst.IdentSubst;
      }

      return "";
   }

   ResolvedTemplateArg *getParameterSubst(llvm::StringRef name)
   {
      auto end_it = SubstContexts.rend();
      for (auto it = SubstContexts.rbegin(); it != end_it; ++it) {
         auto &Subst = *it;
         if (!Subst.ArgSubst)
            continue;

         if (Subst.Param->getName() == name)
            return Subst.ArgSubst;
      }

      return nullptr;
   }

   TemplateParamDecl* hasTemplateParam(llvm::StringRef Name) const
   {
      if (!Template)
         return nullptr;

      for (TemplateParamDecl *Param : Template->getTemplateParams())
         if (Param->getName() == Name)
            return Param;

      return nullptr;
   }

   ResolvedTemplateArg* hasTemplateArg(llvm::StringRef Name)
   {
      if (auto P = getParameterSubst(Name))
         return P;

      if (!templateArgs)
         return nullptr;

      return templateArgs->getNamedArg(Name);
   }

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
      ret->setExternKind(node->getExternKind());

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

   template<class T, class U>
   void copyTemplateArgs(const T& from, const U& to)
   {
      to->setTemplateArgs(copyExprList(from->getTemplateArgs()));
   }

   template<class T, class U>
   void copyTemplateParameters(T *from, U *to, size_t beginIdx = 0)
   {
      std::vector<TemplateParamDecl*> Params;
      auto OriginalParams = from->getTemplateParams();
      auto NumParams = OriginalParams.size();

      for (; beginIdx < NumParams; ++beginIdx) {
         auto p = OriginalParams[beginIdx];
         if (!hasTemplateArg(p->getName()))
            Params.push_back(clone(p));
      }

      to->setTemplateParams(move(Params));
   }

   std::vector<FuncArgDecl*> copyArgListAndFindVariadic(CallableDecl *C);

   template<class Container,
            class Expr = typename
               std::remove_reference<decltype(*Container().front())>::type>
   std::vector<Expr*> copyExprList(const Container &exprList);

   std::vector<SourceType> copyTypeList(llvm::ArrayRef<SourceType> Tys);

   class VariadicParamFinder: public RecursiveASTVisitor<VariadicParamFinder> {
   public:
      VariadicParamFinder(
         InstantiatorImpl &Inst,
         llvm::SmallPtrSetImpl<TemplateParamDecl*> &Params,
         llvm::SmallPtrSetImpl<IdentifierRefExpr*> &PackArguments)
         : Inst(Inst), VariadicParams(Params), PackArguments(PackArguments)
      { }

      bool visitIdentifierRefExpr(IdentifierRefExpr *Expr)
      {
         if (Expr->getParentExpr())
            return false;

         auto Param = Inst.hasTemplateParam(Expr->getIdent());
         if (Param && Param->isVariadic()) {
            VariadicParams.insert(Param);
            return false;
         }

         auto it = Inst.VariadicTemplateArgs.find(Expr->getIdent());
         if (it != Inst.VariadicTemplateArgs.end()) {
            PackArguments.insert(Expr);

            if (Inst.templateArgs)
               VariadicParams.insert(
                  Inst.templateArgs->getParameter(it->getValue()));
         }

         return false;
      }

   private:
      InstantiatorImpl &Inst;
      llvm::SmallPtrSetImpl<TemplateParamDecl*> &VariadicParams;
      llvm::SmallPtrSetImpl<IdentifierRefExpr*> &PackArguments;
   };

   template<class Container,
            class Element = typename
               std::remove_pointer<typename
                  std::remove_reference<decltype(Container().front())>
                     ::type>::type>
   bool expandVariadicArgument(Container &exprs,
                               Expression* variadicExpr) {
      if (!variadicExpr->isVariadicArgPackExpansion()) {
         exprs.emplace_back(cast<Element>(visit(variadicExpr)));
         return false;
      }

      llvm::SmallPtrSet<TemplateParamDecl*, 4> VariadicParams;
      llvm::SmallPtrSet<IdentifierRefExpr*, 4> PackArguments;

      VariadicParamFinder Visitor(*this, VariadicParams, PackArguments);
      Visitor.visit(variadicExpr);

      assert(!VariadicParams.empty() && "undiagnosed invalid pack expansion");

      if (VariadicParams.size() > 1) {
         llvm_unreachable("FIXME error message");
      }

      TemplateParamDecl *ParameterPack = *VariadicParams.begin();
      IdentifierRefExpr *PackArgument = nullptr;

      if (!PackArguments.empty())
         PackArgument = *PackArguments.begin();

      auto GivenArg = hasTemplateArg(ParameterPack->getName());
      if (!GivenArg) {
         // current context is still templated
         return false;
      }

      assert(GivenArg->isVariadic() && "invalid passed template argument");

      auto numVariadics = GivenArg->getVariadicArgs().size();

      llvm::SmallString<128> ArgName;
      if (PackArgument) {
         ArgName += "__";
         ArgName += PackArgument->getIdent();
      }

      size_t originalSize = ArgName.size();
      for (size_t i = 0; i < numVariadics; ++i) {
         llvm::StringRef IdentSubst;
         llvm::StringRef FuncArg;

         if (PackArgument) {
            ArgName += std::to_string(i);

            IdentSubst = ArgName.str();
            FuncArg = PackArgument->getIdent();
         }

         SubstContextRAII raii(*this, ParameterPack, FuncArg, IdentSubst,
                               &GivenArg->getVariadicArgs()[i]);

         auto newExpr = cast<Element>(visit(variadicExpr));
         newExpr->setIsVariadicArgPackExpansion(false);

         exprs.emplace_back(newExpr);
         ArgName.resize(originalSize);
      }

      return true;
   }
};

TemplateParamDecl*
InstantiatorImpl::visitTemplateParamDecl(TemplateParamDecl *P)
{
   if (P->isTypeName()) {
      return makeDecl<TemplateParamDecl>(P, string(P->getName()),
                                         visitOrAuto(P->getCovariance()),
                                         visitOrAuto(P->getContravariance()),
                                         P->isVariadic(),
                                         copyOrNull(P->getDefaultValue()));
   }
   else {
      return makeDecl<TemplateParamDecl>(P, string(P->getName()),
                                         visitOrAuto(P->getCovariance()),
                                         P->isVariadic(),
                                         copyOrNull(P->getDefaultValue()));
   }
}

RecordDecl *InstantiatorImpl::instantiateRecordDecl(RecordDecl *src)
{
   auto access = src->getAccess();

   string recordName;
   recordName += src->getName();
   recordName += templateArgString;

   auto conformances = copyTypeList(src->getConformanceTypes());
   auto constraints = cloneVector(src->getConstraints());

   SourceType enumRawTypeOrExtends;
   bool isAbstract = false;

   if (auto E = dyn_cast<EnumDecl>(src)) {
      enumRawTypeOrExtends = visit(E->getRawType());
   }
   else if (auto C = dyn_cast<ClassDecl>(src)) {
      isAbstract = C->isAbstract();
      enumRawTypeOrExtends = visit(C->getParentType());
   }

   RecordDecl *decl;
   switch (src->getKind()) {
   case Decl::StructDeclID:
      decl = makeDecl<StructDecl>(src, access,
                                  move(recordName),
                                  move(conformances),
                                  move(constraints));
      break;
   case Decl::ClassDeclID:
      decl = makeDecl<ClassDecl>(src, access,
                                 move(recordName),
                                 move(conformances),
                                 move(constraints),
                                 enumRawTypeOrExtends, isAbstract);
      break;
   case Decl::EnumDeclID:
      decl = makeDecl<EnumDecl>(src, access,
                                move(recordName),
                                move(conformances),
                                move(constraints),
                                enumRawTypeOrExtends);
      break;
   case Decl::UnionDeclID:
      decl = makeDecl<UnionDecl>(src, access,
                                 move(recordName),
                                 move(conformances),
                                 move(constraints));
      break;
   case Decl::ProtocolDeclID:
      decl = makeDecl<ProtocolDecl>(src, access,
                                    move(recordName),
                                    move(conformances),
                                    move(constraints));
      break;
   case Decl::ExtensionDeclID:
      decl = makeDecl<ExtensionDecl>(src, access,
                                     move(recordName),
                                     move(conformances),
                                     move(constraints));
      break;
   default:
      llvm_unreachable("not a record decl!");
   }

   return decl;
}

RecordDecl *InstantiatorImpl::instantiateRecordDefinition(RecordDecl *Template,
                                                          RecordDecl *Inst) {
   {
      SemaPass::DeclContextRAII declContext(SP, Inst);

      for (const auto &namedDecl : Template->getDecls()) {
         if (isa<TemplateParamDecl>(namedDecl)) {
            continue;
         }
         else {
            (void) visit(namedDecl);
         }
      }
   }

   SP.ActOnRecordDecl(Inst);

   return Inst;
}

RecordDecl *InstantiatorImpl::visitRecordCommon(RecordDecl *Template)
{
   auto Inst = instantiateRecordDecl(Template);
   return instantiateRecordDefinition(Template, Inst);
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
   return makeDecl<EnumCaseDecl>(node,
                                 string(node->getName()),
                                 copyOrNull(node->getRawValExpr()),
                                 cloneVector(node->getArgs()));
}

UnionDecl* InstantiatorImpl::visitUnionDecl(UnionDecl *node)
{
   return cast<UnionDecl>(visitRecordCommon(node));
}

ProtocolDecl* InstantiatorImpl::visitProtocolDecl(ProtocolDecl *node)
{
   return cast<ProtocolDecl>(visitRecordCommon(node));
}

ExtensionDecl* InstantiatorImpl::visitExtensionDecl(ExtensionDecl *node)
{
   return cast<ExtensionDecl>(visitRecordCommon(node));
}

PropDecl* InstantiatorImpl::visitPropDecl(PropDecl *node, bool skipBody)
{
   auto Prop =
      makeDecl<PropDecl>(node,
                         string(node->getName()),
                         visit(node->getType()),
                         node->getAccess(), node->isStatic(),
                         node->hasDefinition(),
                         node->hasGetter(), node->hasSetter(),
                         skipBody ? nullptr
                                  : copyOrNull(node->getGetterBody()),
                         skipBody ? nullptr
                                  : copyOrNull(node->getSetterBody()),
                         string(node->getNewValName()));

   if (skipBody)
      Prop->setPropTemplate(node);

   SP.addDeclToContext(SP.getDeclContext(), Prop);
   return Prop;
}

MethodDecl* InstantiatorImpl::visitMethodDecl(MethodDecl *M, bool skipBody)
{
   if (M->isSynthesized())
      return nullptr;

   std::vector<FuncArgDecl*> args = copyArgListAndFindVariadic(M);

   string methodName = M->getName();
   if (M == Template)
      methodName += templateArgString;

   MethodDecl* decl = makeDecl<MethodDecl>(M, move(methodName),
                                           visitOrAuto(M->getReturnType()),
                                           move(args),
                                           cloneVector(M->getConstraints()),
                                           nullptr, M->getOperator(),
                                           M->isCastOp(), M->getAccess(),
                                           M->isStatic());

   if (!skipBody) {
      Statement *Body = M->getBody() ? M->getBody()
                                     : M->getBodyTemplate();

      SemaPass::DeclContextRAII declContext(SP, decl);
      decl->setBody(copyOrNull(Body));
   }

   decl->setFunctionFlags(M->getFunctionFlags());
   copyTemplateParameters(M, decl);

   if (skipBody)
      decl->setBodyTemplate(M->getBody());;

   VariadicTemplateArgs.clear();

   SP.addDeclToContext(SP.getDeclContext(), decl);
   return decl;
}

AssociatedTypeDecl*
InstantiatorImpl::visitAssociatedTypeDecl(AssociatedTypeDecl *node)
{
   auto Inst = makeDecl<AssociatedTypeDecl>(node,
                                            string(
                                               node->getProtocolSpecifier()),
                                            string(node->getName()),
                                            cloneVector(node->getConstraints()),
                                            visit(node->getActualType()));

   SP.addDeclToContext(SP.getDeclContext(), Inst);
   return Inst;
}

TypedefDecl*
InstantiatorImpl::visitTypedefDecl(TypedefDecl *node)
{
   auto Inst = makeDecl<TypedefDecl>(node, node->getAccess(),
                                     string(node->getName()),
                                     visit(node->getOriginTy()));

   SP.addDeclToContext(SP.getDeclContext(), Inst);
   return Inst;
}

AliasDecl*
InstantiatorImpl::visitAliasDecl(AliasDecl *Alias)
{
   auto Inst = makeDecl<AliasDecl>(Alias, string(Alias->getName()),
                                   cloneVector(Alias->getConstraints()),
                                   visitStaticExpr(Alias->getAliasExpr()));

   copyTemplateParameters(Alias, Inst);

   SP.ActOnAliasDecl(Inst);
   return Inst;
}

FieldDecl* InstantiatorImpl::visitFieldDecl(FieldDecl *node)
{
   auto Inst = makeDecl<FieldDecl>(node, string(node->getName()),
                                   visit(node->getType()),
                                   node->getAccess(), node->isStatic(),
                                   node->isConst(),
                                   copyOrNull(node->getDefaultVal()));

   SP.addDeclToContext(SP.getDeclContext(), Inst);
   return Inst;
}

InitDecl*
InstantiatorImpl::visitInitDecl(InitDecl *node, bool skipBody)
{
   if (node->isSynthesized())
      return nullptr;

   std::vector<FuncArgDecl*> args;
   if (!skipBody) {
      args = cloneVector(node->getArgs());
   }
   else {
      args = copyArgListAndFindVariadic(node);
   }

   string methodName = node->getName();
   if (node == Template)
      methodName += templateArgString;

   auto decl = makeDecl<InitDecl>(node, move(args), node->getAccess(),
                                  nullptr, move(methodName));

   if (!skipBody) {
      Statement *Body = decl->getBody() ? decl->getBody()
                                        : decl->getBodyTemplate();

      SemaPass::DeclContextRAII declContext(SP, decl);
      decl->setBody(copyOrNull(Body));
   }

   copyTemplateParameters(node, decl,
                          node->getRecord()->getTemplateParams().size());

   decl->setFunctionFlags(node->getFunctionFlags());

   if (skipBody)
      decl->setBodyTemplate(node->getBody());

   SP.addDeclToContext(SP.getDeclContext(), decl);
   return decl;
}

DeinitDecl* InstantiatorImpl::visitDeinitDecl(DeinitDecl *node, bool skipBody)
{
   if (node->isSynthesized())
      return nullptr;

   auto decl = makeDecl<DeinitDecl>(node, nullptr);

   if (!skipBody) {
      Statement *Body = decl->getBody() ? decl->getBody()
                                        : decl->getBodyTemplate();

      SemaPass::DeclContextRAII declContext(SP, decl);
      decl->setBody(copyOrNull(Body));
   }

   decl->setTemplateParams(cloneVector(node->getTemplateParams()));
   decl->setFunctionFlags(node->getFunctionFlags());

   if (skipBody)
      decl->setBodyTemplate(node->getBody());

   SP.addDeclToContext(SP.getDeclContext(), decl);
   return decl;
}

FunctionDecl* InstantiatorImpl::visitFunctionDecl(FunctionDecl *F)
{
   auto args = copyArgListAndFindVariadic(F);

   string funcName = F->getName();
   if (F == Template)
      funcName += templateArgString;

   auto Inst = makeDecl<FunctionDecl>(F, F->getAccess(), move(funcName),
                                      move(args),
                                      visitOrAuto(F->getReturnType()),
                                      cloneVector(F->getConstraints()),
                                      nullptr, F->getOperator());

   {
      SemaPass::DeclContextRAII declContext(SP, Inst);
      Inst->setBody(copyOrNull(F->getBody()));
   }

   copyTemplateParameters(F, Inst);
   Inst->setFunctionFlags(F->getFunctionFlags());
   Inst->getCalledFunctions().insert(F->getCalledFunctions().begin(),
                                     F->getCalledFunctions().end());

   VariadicTemplateArgs.clear();

   SP.ActOnFunctionDecl(Inst);
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
                                copyTypeList(Expr->getContainedTypes()));
}

FunctionTypeExpr*
InstantiatorImpl::visitFunctionTypeExpr(FunctionTypeExpr *Expr)
{
   return FunctionTypeExpr::Create(Context, Expr->getSourceRange(),
                                   visit(Expr->getReturnType()),
                                   copyTypeList(Expr->getArgTypes()));
}

ArrayTypeExpr *InstantiatorImpl::visitArrayTypeExpr(ArrayTypeExpr *Expr)
{
   return ArrayTypeExpr::Create(Context, Expr->getSourceRange(),
                                visit(Expr->getElementTy()),
                                clone<StaticExpr>(Expr->getSizeExpr()));
}

DeclTypeExpr *InstantiatorImpl::visitDeclTypeExpr(DeclTypeExpr *Expr)
{
   return DeclTypeExpr::Create(Context, Expr->getSourceRange(),
                               visit(Expr->getTyExpr()));
}

PointerTypeExpr *InstantiatorImpl::visitPointerTypeExpr(PointerTypeExpr *Expr)
{
   return PointerTypeExpr::Create(Context, Expr->getSourceRange(),
                                  visit(Expr->getSubType()));
}

ReferenceTypeExpr*
InstantiatorImpl::visitReferenceTypeExpr(ReferenceTypeExpr *Expr)
{
   return ReferenceTypeExpr::Create(Context, Expr->getSourceRange(),
                                    visit(Expr->getSubType()));
}

OptionTypeExpr *InstantiatorImpl::visitOptionTypeExpr(OptionTypeExpr *Expr)
{
   return OptionTypeExpr::Create(Context, Expr->getSourceRange(),
                                 visit(Expr->getSubType()));
}

Expression* InstantiatorImpl::visitImplicitCastExpr(ImplicitCastExpr *node)
{
   return visit(node->getTarget());
}

CompoundDecl* InstantiatorImpl::visitCompoundDecl(CompoundDecl *D)
{
   auto Inst = makeDecl<CompoundDecl>(D, D->getLBraceLoc(), D->isTransparent());

   SemaPass::DeclContextRAII declContextRAII(SP, Inst);
   for (auto &decl : D->getDecls())
      visit(decl);

   Inst->setRBraceLoc(D->getRBraceLoc());
   return Inst;
}

CompoundStmt* InstantiatorImpl::visitCompoundStmt(CompoundStmt *node)
{
   llvm::SmallVector<Statement*, 8> Stmts;
   for (auto &Stmt : node->getStatements())
      Stmts.push_back(visit(Stmt));

   return CompoundStmt::Create(Context, Stmts, node->preservesScope(),
                               node->getLBraceLoc(), node->getRBraceLoc());
}

std::vector<FuncArgDecl*>
InstantiatorImpl::copyArgListAndFindVariadic(CallableDecl *C)
{
   auto &argList = C->getArgs();

   std::vector<FuncArgDecl*> args;
   for (const auto &arg : argList) {
      if (!arg->isVariadicArgPackExpansion()) {
         args.push_back(clone<FuncArgDecl>(arg));
         continue;
      }

      auto GenTy = arg->getArgType()->asGenericType();
      if (!GenTy) {
         args.push_back(clone<FuncArgDecl>(arg));
         continue;
      }

      auto name = GenTy->getGenericTypeName();
      auto TA = hasTemplateArg(name);

      if (TA && TA->isVariadic()) {
         VariadicTemplateArgs.try_emplace(arg->getArgName(), TA);

         string newName("__");
         newName += arg->getArgName();

         auto initialSize = newName.size();

         size_t i = 0;
         for (const auto &VA : TA->getVariadicArgs()) {
            newName.resize(initialSize);
            newName += std::to_string(i++);

            args.push_back(
               makeDecl<FuncArgDecl>(arg, string(newName),
                                     SourceType(VA.getType()),
                                     copyOrNull(arg->getDefaultVal()),
                                     false, arg->isConst()));
         }

         continue;
      }
      else if (hasTemplateParam(name)) {
         VariadicTemplateArgs.try_emplace(arg->getArgName(), nullptr);
      }

      args.push_back(clone<FuncArgDecl>(arg));
   }

   return args;
}

template<class Container, class Expr> std::vector<Expr*>
InstantiatorImpl::copyExprList(const Container &exprList)
{
   std::vector<Expr*> list;
   for (const auto &expr : exprList)
      expandVariadicArgument(list, expr);

   return list;
}

std::vector<SourceType>
InstantiatorImpl::copyTypeList(llvm::ArrayRef<SourceType> Tys)
{
   llvm::SmallVector<Expression*, 4> Exprs;
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

FuncArgDecl*
InstantiatorImpl::visitFuncArgDecl(FuncArgDecl *node)
{
   return makeDecl<FuncArgDecl>(node,
                                string(node->getArgName()),
                                visitOrAuto(node->getArgType()),
                                copyOrNull(node->getDefaultVal()),
                                node->isVariadicArgPackExpansion(),
                                node->isConst(), node->isCstyleVararg());
}

NullStmt *InstantiatorImpl::visitNullStmt(NullStmt *node)
{
   return NullStmt::Create(Context, node->getSourceLoc());
}

BreakStmt *InstantiatorImpl::visitBreakStmt(BreakStmt *node)
{
   return BreakStmt::Create(Context, node->getSourceLoc());
}

ContinueStmt *InstantiatorImpl::visitContinueStmt(ContinueStmt *node)
{
   return ContinueStmt::Create(Context, node->getSourceLoc());
}

GotoStmt *InstantiatorImpl::visitGotoStmt(GotoStmt *node)
{
   return GotoStmt::Create(Context, node->getSourceLoc(), node->getLabel());
}

LabelStmt *InstantiatorImpl::visitLabelStmt(LabelStmt *node)
{
   return LabelStmt::Create(Context, node->getSourceLoc(), node->getLabel());
}

IntegerLiteral *InstantiatorImpl::visitIntegerLiteral(IntegerLiteral *node)
{
   return IntegerLiteral::Create(Context, node->getSourceLoc(),
                                 node->getType(),
                                 llvm::APSInt(node->getValue()),
                                 node->getSuffix());
}

FPLiteral *InstantiatorImpl::visitFPLiteral(FPLiteral *node)
{
   return FPLiteral::Create(Context, node->getSourceLoc(),
                            node->getType(), llvm::APFloat(node->getValue()),
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
      return CharLiteral::Create(Context, node->getSourceLoc(), node->getType(),
                                 node->getWide());
   }
   else {
      return CharLiteral::Create(Context, node->getSourceLoc(), node->getType(),
                                 node->getNarrow());
   }
}

SelfExpr* InstantiatorImpl::visitSelfExpr(SelfExpr *node)
{
   return SelfExpr::Create(Context, node->getSourceLoc());
}

SuperExpr* InstantiatorImpl::visitSuperExpr(SuperExpr *node)
{
   return SuperExpr::Create(Context, node->getSourceLoc());
}

DebugStmt* InstantiatorImpl::visitDebugStmt(DebugStmt *node)
{
   return new (Context) DebugStmt(node->getSourceLoc());
}

NoneLiteral* InstantiatorImpl::visitNoneLiteral(NoneLiteral *node)
{
   return NoneLiteral::Create(Context, node->getSourceLoc());
}

StringInterpolation*
InstantiatorImpl::visitStringInterpolation(StringInterpolation *node)
{
   return StringInterpolation::Create(Context, node->getSourceLoc(),
                                      cloneVector(node->getStrings()));
}

StringLiteral *InstantiatorImpl::visitStringLiteral(StringLiteral *node)
{
   return StringLiteral::Create(Context, node->getSourceLoc(),
                                node->getValue().str());
}

TupleLiteral* InstantiatorImpl::visitTupleLiteral(TupleLiteral *node)
{
   return TupleLiteral::Create(Context, node->getSourceRange(),
                               copyExprList(node->getElements()));
}

LocalVarDecl* InstantiatorImpl::visitLocalVarDecl(LocalVarDecl *node)
{
   auto Decl = makeDecl<LocalVarDecl>(node, node->getAccess(), node->isConst(),
                                      string(node->getName()),
                                      visitOrAuto(node->getTypeRef()),
                                      copyOrNull(node->getValue()));

   Decl->setDeclContext(&SP.getDeclContext());
   return Decl;
}

GlobalVarDecl* InstantiatorImpl::visitGlobalVarDecl(GlobalVarDecl *node)
{
   auto Decl = makeDecl<GlobalVarDecl>(node, node->getAccess(), node->isConst(),
                                       string(node->getName()),
                                       visitOrAuto(node->getTypeRef()),
                                       copyOrNull(node->getValue()));

   SP.addDeclToContext(SP.getDeclContext(), Decl);
   return Decl;
}

ast::LocalDestructuringDecl*
InstantiatorImpl::visitLocalDestructuringDecl(LocalDestructuringDecl *node)
{
   return LocalDestructuringDecl::Create(Context, node->getAccess(),
                                         node->isConst(),
                                         cloneVector(node->getDecls()),
                                         visitOrAuto(node->getType()),
                                         copyOrNull(node->getValue()));
}

ast::GlobalDestructuringDecl*
InstantiatorImpl::visitGlobalDestructuringDecl(GlobalDestructuringDecl *node)
{
   return GlobalDestructuringDecl::Create(Context, node->getAccess(),
                                          node->isConst(),
                                          cloneVector(node->getDecls()),
                                          visitOrAuto(node->getType()),
                                          copyOrNull(node->getValue()));
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
         literal =
            CharLiteral::Create(Context, Expr->getSourceLoc(),
                                getContext().getCharTy(), Val.getChar());

         break;
      default: {
         literal = IntegerLiteral::Create(Context, Expr->getSourceLoc(),
                                          valTy, llvm::APSInt(Val.getAPSInt()));
         break;
      }
      }
   }
   else if (valTy->isFPType()) {
      literal = FPLiteral::Create(Context, Expr->getSourceLoc(), valTy,
                                  llvm::APFloat(Val.getAPFloat()));
   }
   else if (valTy->isObjectType() && valTy->getClassName() == "String") {
      literal = StringLiteral::Create(Context, Expr->getSourceLoc(),
                                      string(Val.getString()));
   }
   else {
      llvm_unreachable("bad variant kind!");
   }

   return literal;
}

Expression*
InstantiatorImpl::visitIdentifierRefExpr(IdentifierRefExpr *Ident)
{
   if (!Ident->getParentExpr()) {
      if (auto Arg = hasTemplateArg(Ident->getIdent())) {
         if (Arg->isType()) {
            return new(Context) IdentifierRefExpr(
               Ident->getSourceLoc(), IdentifierKind::MetaType,
               getContext().getMetaType(Arg->getType()));
         }
         else {
            return makeLiteralExpr(Ident, hasTemplateParam(Ident->getIdent())
                                      ->getValueType(),
                                   Arg->getValue());
         }
      }

      if (ValueSubst.Name == Ident->getIdent()) {
         return makeLiteralExpr(Ident, ValueSubst.Ty, *ValueSubst.Val);
      }
   }

   string ident;

   if (Ident->getKind() == IdentifierKind::FunctionArg) {
      if (Ident->getFuncArg()->isVariadicArgPackExpansion()) {
         auto Subst = getVariadicArgumentSubst(Ident->getIdent());
         if (!Subst.empty())
            ident = Subst.str();
      }
   }

   if (ident.empty())
      ident = Ident->getIdent();

   auto expr = new(Context) IdentifierRefExpr(Ident->getSourceLoc(),
                                              move(ident));

   copyTemplateArgs(Ident, expr);
   return expr;
}

BuiltinIdentExpr*InstantiatorImpl::visitBuiltinIdentExpr(BuiltinIdentExpr *node)
{
   return BuiltinIdentExpr::Create(Context, node->getSourceLoc(),
                                   node->getIdentifierKind());
}

MemberRefExpr* InstantiatorImpl::visitMemberRefExpr(MemberRefExpr *node)
{
   auto expr = new(Context) MemberRefExpr(node->getSourceLoc(),
                                          copyOrNull(node->getParentExpr()),
                                          string(node->getIdent()),
                                          node->isPointerAccess());

   copyTemplateArgs(node, expr);
   return expr;
}

EnumCaseExpr* InstantiatorImpl::visitEnumCaseExpr(EnumCaseExpr *node)
{
   return new(Context) EnumCaseExpr(node->getSourceLoc(),
                                    string(node->getIdent()),
                                    copyExprList(node->getArgs()));
}

Expression* InstantiatorImpl::visitCallExpr(CallExpr *node)
{
   if (node->getKind() == CallKind::VariadicSizeof) {
      auto TA = hasTemplateArg(
         cast<IdentifierRefExpr>(node->getTemplateArgs().front())->getIdent());

      if (TA) {
         if (!TA->isVariadic())
            SP.diagnose(node, err_generic_error,
                        "sizeof... requires variadic template argument, "
                        + TA->toString() + " given");
         else {
            llvm::APSInt Int(
               llvm::APInt(sizeof(size_t) * 8,
                           uint64_t(TA->getVariadicArgs().size())), true);

            return IntegerLiteral::Create(Context, node->getSourceLoc(),
                                          getContext().getUIntTy(),
                                          std::move(Int));
         }
      }
   }

   string ident;
   if (auto Param = hasTemplateArg(node->getIdent())) {
      auto ty = Param->getType();
      ident = ty->toString();
   }

   if (ident.empty())
      ident = node->getIdent();

   auto call = new(Context) CallExpr(node->getIdentLoc(), node->getParenRange(),
                                     copyOrNull(node->getParentExpr()),
                                     copyExprList(node->getArgs()),
                                     move(ident));

   copyTemplateArgs(node, call);
   call->setIsPointerAccess(node->isPointerAccess());

   return call;
}

SubscriptExpr *InstantiatorImpl::visitSubscriptExpr(SubscriptExpr *node)
{
   return SubscriptExpr::Create(Context, node->getSourceRange(),
                                copyOrNull(node->getParentExpr()),
                                cloneVector(node->getIndices()));
}

TupleMemberExpr* InstantiatorImpl::visitTupleMemberExpr(TupleMemberExpr *node)
{
   return new (Context) TupleMemberExpr(node->getSourceLoc(),
                                        copyOrNull(node->getParentExpr()),
                                        node->getIndex(),
                                        node->isPointerAccess());
}

ReturnStmt* InstantiatorImpl::visitReturnStmt(ReturnStmt *node)
{
   if (auto Val = node->getReturnValue()) {
      return ReturnStmt::Create(Context, node->getSourceLoc(), visit(Val));
   }
   else {
      return ReturnStmt::Create(Context, node->getSourceLoc());
   }
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

CastExpr* InstantiatorImpl::visitCastExpr(CastExpr *Expr)
{
   return CastExpr::Create(Context, Expr->getSourceRange().getStart(),
                           Expr->getStrength(), visit(Expr->getTarget()),
                           visit(Expr->getTargetType()));
}

TypePredicateExpr*
InstantiatorImpl::visitTypePredicateExpr(TypePredicateExpr *Expr)
{
   return TypePredicateExpr::Create(Context,
                                    Expr->getSourceRange().getStart(),
                                    visit(Expr->getLHS()),
                                    visit(Expr->getRHS()));
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
            auto maybeIdent =
               SP.makeStmt<IdentifierRefExpr>(string(El.getOp()));

            maybeIdent->setSourceLoc(El.getLoc());
            auto expr = visitIdentifierRefExpr(maybeIdent);

            auto ident = dyn_cast<IdentifierRefExpr>(expr);
            if (!ident || ident->getIdent() != El.getOp())
               fragments.emplace_back(expr);
            else
               fragments.emplace_back(string(El.getOp()), El.getLoc());

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
   return LambdaExpr::Create(Context, node->getParenRange(),
                             node->getArrowLoc(),
                             visitOrAuto(node->getReturnType()),
                             cloneVector(node->getArgs()),
                             visit(node->getBody()));
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

IfStmt* InstantiatorImpl::visitIfStmt(IfStmt *node)
{
   return IfStmt::Create(Context, node->getSourceLoc(),
                         visit(node->getCondition()),
                         visit(node->getIfBranch()),
                         copyOrNull(node->getElseBranch()));
}

WhileStmt* InstantiatorImpl::visitWhileStmt(WhileStmt *node)
{
   return WhileStmt::Create(Context, node->getSourceLoc(),
                            visit(node->getCondition()),
                            visit(node->getBody()),
                            node->isAtLeastOnce());
}

ForStmt* InstantiatorImpl::visitForStmt(ForStmt *node)
{
   return ForStmt::Create(Context, node->getSourceLoc(),
                          copyOrNull(node->getInitialization()),
                          copyOrNull(node->getTermination()),
                          copyOrNull(node->getIncrement()),
                          copyOrNull(node->getBody()));
}

ForInStmt* InstantiatorImpl::visitForInStmt(ForInStmt *node)
{
   return ForInStmt::Create(Context, node->getSourceLoc(),
                            clone<LocalVarDecl>(node->getDecl()),
                            visit(node->getRangeExpr()),
                            visit(node->getBody()));
}

MatchStmt* InstantiatorImpl::visitMatchStmt(MatchStmt *node)
{
   return MatchStmt::Create(Context, node->getMatchLoc(),
                            node->getBraceRange(),
                            visit(node->getSwitchValue()),
                            cloneVector(node->getCases()));
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
   std::vector<CasePatternArgument> args;
   for (const auto &arg : node->getArgs()) {
      if (arg.isExpr()) {
         args.emplace_back(visit(arg.getExpr()), arg.getSourceLoc());
      }
      else {
         args.emplace_back(string(arg.getIdentifier()), arg.isConst(),
                           arg.getSourceLoc());
      }
   }

   return CasePattern::Create(Context, node->getSourceRange(),
                              string(node->getCaseName()), args);
}

TryStmt* InstantiatorImpl::visitTryStmt(TryStmt *node)
{
   std::vector<CatchBlock> catchBlocks;
   for (const auto &CB : node->getCatchBlocks()) {
      catchBlocks.emplace_back(visitLocalVarDecl(CB.varDecl),
                               visit(CB.body));
   }

   return new(Context) TryStmt(node->getSourceRange(), visit(node->getBody()),
                               move(catchBlocks),
                               copyOrNull(node->getFinallyBlock()));
}

ThrowStmt* InstantiatorImpl::visitThrowStmt(ThrowStmt *node)
{
   return new(Context) ThrowStmt(node->getSourceLoc(),
                                 visit(node->getThrownVal()));
}

UsingStmt* InstantiatorImpl::visitUsingStmt(UsingStmt *node)
{
   return UsingStmt::Create(Context, node->getSourceRange(),
                            node->getDeclContextSpecifier(),
                            node->getImportedItems(),
                            node->isWildCardImport());
}

ModuleStmt* InstantiatorImpl::visitModuleStmt(ModuleStmt *node)
{
   return ModuleStmt::Create(Context, node->getSourceRange(),
                             node->getQualifiedModuleName());
}

ImportStmt* InstantiatorImpl::visitImportStmt(ImportStmt *node)
{
   return ImportStmt::Create(Context, node->getSourceRange(),
                             node->getQualifiedImportName());
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
   auto Inst = MixinDecl::Create(Context, Decl->getSourceRange(),
                                 visit(Decl->getMixinExpr()));

   SP.addDeclToContext(SP.getDeclContext(), Inst);
   return Inst;
}

StaticIfDecl* InstantiatorImpl::visitStaticIfDecl(StaticIfDecl *node)
{
   // if the condition is dependent, neither the true branch nor the false
   // branch will have been checked for dependence, so we need to
   // conservatively instantiate everything in them
   UnevalutedScopeRAII unevalutedScopeRAII(*this, node->isDependent());

   auto Decl = makeDecl<StaticIfDecl>(node, node->getStaticLoc(),
                                 node->getRBRaceLoc(),
                                 clone<StaticExpr>(node->getCondition()),
                                 copyOrNull<CompoundDecl>(node->getIfDecl()),
                                 copyOrNull<CompoundDecl>(node->getElseDecl()));

   SP.addDeclToContext(SP.getDeclContext(), Decl);
   return Decl;
}

StaticForDecl* InstantiatorImpl::visitStaticForDecl(StaticForDecl *node)
{
   auto Decl = makeDecl<StaticForDecl>(node, node->getStaticLoc(),
                                  node->getRBRaceLoc(),
                                  node->getElementName(),
                                  clone<StaticExpr>(node->getRange()),
                                  copyOrNull<CompoundDecl>(node->getBodyDecl()));

   SP.addDeclToContext(SP.getDeclContext(), Decl);
   return Decl;
}

StaticIfStmt* InstantiatorImpl::visitStaticIfStmt(StaticIfStmt *node)
{
   // if the condition is dependent, neither the true branch nor the false
   // branch will have been checked for dependence, so we need to
   // conservatively instantiate everything in them
   UnevalutedScopeRAII unevalutedScopeRAII(*this, node->isDependent());

   return new(Context) StaticIfStmt(node->getStaticLoc(), node->getIfLoc(),
                                    visitStaticExpr(node->getCondition()),
                                    visit(node->getIfBranch()),
                                    copyOrNull(node->getElseBranch()));
}

StaticForStmt* InstantiatorImpl::visitStaticForStmt(StaticForStmt *node)
{
   // see above
   UnevalutedScopeRAII unevalutedScopeRAII(*this, node->isDependent());

   return new(Context) StaticForStmt(node->getStaticLoc(), node->getForLoc(),
                                     string(node->getElementName()),
                                     visitStaticExpr(node->getRange()),
                                     visit(node->getBody()));
}

StaticAssertStmt*InstantiatorImpl::visitStaticAssertStmt(StaticAssertStmt *node)
{
   auto Decl = makeDecl<StaticAssertStmt>(node,
                                          visitStaticExpr(node->getExpr()),
                                          string(node->getMessage()));

   SP.addDeclToContext(SP.getDeclContext(), Decl);
   return Decl;
}

StaticPrintStmt* InstantiatorImpl::visitStaticPrintStmt(StaticPrintStmt *node)
{
   auto Decl = makeDecl<StaticPrintStmt>(node,visitStaticExpr(node->getExpr()));
   SP.addDeclToContext(SP.getDeclContext(), Decl);

   return Decl;
}

} // anonymous namespace

RecordDecl*
TemplateInstantiator::InstantiateRecord(Statement *POI,
                                        RecordDecl *Template,
                                        TemplateArgList&& templateArgs,
                                        bool *isNew) {
   if (templateArgs.isStillDependent())
      return Template;

   assert(!Template->isInstantiation() && "only instantiate base template!");

   void *insertPos;
   if (auto R = Context.getRecordTemplateInstantiation(Template,
                                                               templateArgs,
                                                               insertPos)) {
      if (isNew) *isNew = false;
      return R;
   }

   // create this beforehand so we don't accidentally use the template arg
   // list after moving it
   auto instInfo = new (Context) InstantiationInfo<RecordDecl> {
      POI->getSourceRange().getStart(), move(templateArgs), Template,
      SP.getCurrentDecl()
   };

   InstantiatorImpl Instantiator(SP, instInfo->templateArgs, Template);

   RecordDecl *Inst = Instantiator.instantiateRecordDecl(Template);
   Inst->setInstantiationInfo(instInfo);

   // avoid infinite recursion when this template is instantiated within its
   // own definition
   Context.insertRecordTemplateInstantiation(Inst, insertPos);
   Instantiator.instantiateRecordDefinition(Template, Inst);

   SP.declareRecordInstantiation(POI, Inst);

   if (isNew)
      *isNew = true;

   return Inst;
}

FunctionDecl*
TemplateInstantiator::InstantiateFunction(Statement *POI,
                                          FunctionDecl *Template,
                                          TemplateArgList&& templateArgs,
                                          bool *isNew) {
   if (Template->isInvalid()) {
      POI->setHadError(true);
      return Template;
   }

   if (templateArgs.isStillDependent())
      return Template;

   void *insertPos;
   if (auto F = Context.getFunctionTemplateInstantiation(Template,
                                                                 templateArgs,
                                                                 insertPos)) {
      if (isNew) *isNew = false;
      return cast<FunctionDecl>(F);
   }

   InstantiatorImpl Instantiator(SP, templateArgs, Template);
   auto decl = Instantiator.visitFunctionDecl(Template);

   SemaPass::DeclScopeRAII raii(SP, decl->getDeclContext());
   Context.insertFunctionTemplateInstantiation(decl, insertPos);

   auto instInfo = new (Context) InstantiationInfo<CallableDecl>{
      POI->getSourceRange().getStart(), move(templateArgs), Template,
      SP.getCurrentDecl()
   };

   decl->setInstantiationInfo(instInfo);

   SP.declareScoped(decl);
   SP.registerDelayedInstantiation(decl, POI);

   if (isNew)
      *isNew = true;

   return decl;
}

MethodDecl*
TemplateInstantiator::InstantiateMethod(Statement *POI,
                                        MethodDecl *Template,
                                        TemplateArgList&& templateArgs,
                                        bool *isNew) {
   if (Template->isInvalid()) {
      POI->setHadError(true);
      return Template;
   }

   if (templateArgs.isStillDependent())
      return Template;

   void *insertPos;
   if (auto M = Context.getFunctionTemplateInstantiation(Template,
                                                                 templateArgs,
                                                                 insertPos)) {
      if (isNew) *isNew = false;
      return cast<MethodDecl>(M);
   }

   MultiLevelTemplateArgList MultiLevelList(templateArgs);
   if (Template->getRecord()->isInstantiation()) {
      MultiLevelList.addOuterList(Template->getRecord()->getTemplateArgs());
   }

   InstantiatorImpl Instantiator(SP, MultiLevelList, Template);

   MethodDecl* decl;
   if (auto C = dyn_cast<InitDecl>(Template)) {
      decl = Instantiator.visitInitDecl(C, false);
   }
   else if (auto D = dyn_cast<DeinitDecl>(Template)) {
      decl = Instantiator.visitDeinitDecl(D, false);
   }
   else {
      decl = Instantiator.visitMethodDecl(Template, false);
   }

   Context.insertFunctionTemplateInstantiation(decl, insertPos);

   auto instInfo = new (Context) InstantiationInfo<CallableDecl>{
      POI->getSourceRange().getStart(), move(templateArgs), Template,
      SP.getCurrentDecl()
   };

   decl->setInstantiationInfo(instInfo);

   SP.declareScoped(decl);
   SP.registerDelayedInstantiation(decl, POI);

   if (isNew)
      *isNew = true;

   return decl;
}

MethodDecl*
TemplateInstantiator::InstantiateProtocolDefaultImpl(SourceLocation POI,
                                                     RecordDecl *Rec,
                                                     MethodDecl *M) {
   InstantiatorImpl Instantiator(SP, {});

   auto decl = Instantiator.visitMethodDecl(M);
   SP.declareScoped(decl);

   auto instInfo = new (Context) InstantiationInfo<CallableDecl>{
      POI, {}, M, SP.getCurrentDecl()
   };

   decl->setInstantiationInfo(instInfo);
   SP.addDeclToContext(*Rec, decl);

   return decl;
}

Statement*
TemplateInstantiator::InstantiateStatement(SourceLocation,
                                           Statement* stmt,
                                           TemplateArgList const& templateArgs){
   InstantiatorImpl Instantiator(SP, templateArgs);
   return Instantiator.visit(stmt);
}

Statement* TemplateInstantiator::InstantiateStatement(Statement *,
                                                      Statement *stmt,
                                                      llvm::StringRef SubstName,
                                                      QualType SubstTy,
                                                      const Variant &SubstVal) {
   InstantiatorImpl Instantiator(SP, SubstName, SubstTy, SubstVal);
   return Instantiator.visit(stmt);
}

Statement*
TemplateInstantiator::InstantiateMethodBody(ast::Statement *POI,
                                            MethodDecl *Method) {
   InstantiatorImpl Instantiator(SP, Method->getRecord()
                                           ->getTemplateArgs());

   auto Inst = Instantiator.visit(Method->getBodyTemplate());

   Method->setBody(Inst);
   Method->setBodyInstantiationLoc(POI->getSourceLoc());

   auto instInfo = new (Context) InstantiationInfo<CallableDecl>{
      POI->getSourceLoc(), {}, Method, SP.getCurrentDecl()
   };

   Method->setInstantiationInfo(instInfo);

   SP.declareScoped(Method);
   SP.registerDelayedInstantiation(Method, POI);

   return Inst;
}

Expression*
TemplateInstantiator::InstantiateStaticExpr(SourceLocation instantiatedFrom,
                                            Expression* expr,
                                            TemplateArgList const& templateArgs) {
   return cast<Expression>(InstantiateStatement(instantiatedFrom,
                                                expr, templateArgs));
}

AliasDecl*
TemplateInstantiator::InstantiateAlias(AliasDecl *Template,
                                       SourceLocation instantiatedFrom,
                                       TemplateArgList &&templateArgs) {
   void *insertPos;
   if (auto Inst = Context.getAliasTemplateInstantiation(Template,
                                                                 templateArgs,
                                                                 insertPos)) {
      return Inst;
   }

   InstantiatorImpl Instantiator(SP, templateArgs, Template);

   auto Inst = Instantiator.visitAliasDecl(Template);
   Inst->setInstantiationInfo(new(Context) InstantiationInfo<AliasDecl>(
      instantiatedFrom, move(templateArgs), Template, SP.getCurrentDecl()
   ));

   Context.insertAliasTemplateInstantiation(Inst, insertPos);

   return Inst;
}

} // namespace cdot