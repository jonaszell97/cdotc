//
// Created by Jonas Zell on 05.12.17.
//

#include "TemplateInstantiator.h"

#include "Template.h"
#include "SemaPass.h"
#include "AST/AbstractPass.h"
#include "AST/ASTContext.h"
#include "Message/Diagnostics.h"

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

class InstantiatorImpl {
public:
   InstantiatorImpl(SemaPass &SP, MultiLevelTemplateArgList &&templateArgs)
      : SP(SP), Context(SP.getContext()), InstScope(SP, &SP.getDeclContext()),
        templateArgs(move(templateArgs))
   {

   }

   InstantiatorImpl(SemaPass &SP,
                    MultiLevelTemplateArgList &&templateArgs,
                    NamedDecl* Template)
      : SP(SP), Context(SP.getContext()),
        InstScope(SP, Template->getDeclContext()),
        templateArgs(move(templateArgs)), Template(Template)
   {

   }

   InstantiatorImpl(SemaPass &SP,
                    IdentifierInfo *SubstName,
                    QualType SubstTy,
                    const Variant &SubstVal)
      : SP(SP), Context(SP.getContext()), InstScope(SP, &SP.getDeclContext()),
        ValueSubst{ SubstName, SubstTy, &SubstVal }
   {

   }

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
      return cast_or_null<Statement>(visit(Stmt));
   }

   NamedDecl *getTemplate() const { return Template; }
   const MultiLevelTemplateArgList &getTemplateArgs() const
   {
      return templateArgs;
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

      Expression *Inst;
      switch (expr->getTypeID()) {
#        define CDOT_EXPR(Name)                                      \
            case AstNode::Name##ID:                                  \
               Inst = visit##Name(static_cast<Name*>(expr)); break;
#        include "AST/AstNode.def"

         default:
            llvm_unreachable("not an expression!");
      }

      Inst->setEllipsisLoc(expr->getEllipsisLoc());
      return Inst;
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
      if (auto ND = dyn_cast<NamedDecl>(D))
         return visit(ND);

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

   SourceType visit(const SourceType &Ty)
   {
      if (Ty.isResolved()) {
         auto ResolvedTy = Ty.getResolvedType();
         if (!ResolvedTy->isDependentType())
            return SourceType(ResolvedTy);

         if (!ResolvedTy->isUnknownAnyType()) {
            auto Inst = SP.resolveDependencies(Ty, templateArgs,
                                               Ty.getTypeExpr());

            if (Inst && !Inst->isDependentType())
               return SourceType(Inst);
         }
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

   UsingDecl* visitUsingDecl(UsingDecl *node);
   ModuleDecl* visitModuleDecl(ModuleDecl *node);
   ImportDecl* visitImportDecl(ImportDecl *node);

   LocalVarDecl* visitLocalVarDecl(LocalVarDecl *node);
   GlobalVarDecl* visitGlobalVarDecl(GlobalVarDecl *node);

   LocalDestructuringDecl*
   visitLocalDestructuringDecl(LocalDestructuringDecl *node);

   GlobalDestructuringDecl*
   visitGlobalDestructuringDecl(GlobalDestructuringDecl *node);

   FunctionDecl* visitFunctionDecl(FunctionDecl *F);

   RecordDecl *visitRecordCommon(RecordDecl *Template);

   TemplateParamDecl *visitTemplateParamDecl(TemplateParamDecl *decl);

   ClassDecl* visitClassDecl(ClassDecl *node);
   StructDecl* visitStructDecl(StructDecl *node);
   ProtocolDecl* visitProtocolDecl(ProtocolDecl *node);
   ExtensionDecl* visitExtensionDecl(ExtensionDecl *Ext);
   EnumDecl* visitEnumDecl(EnumDecl *node);
   UnionDecl* visitUnionDecl(UnionDecl *node);

   EnumCaseDecl* visitEnumCaseDecl(EnumCaseDecl *node);

   FieldDecl* visitFieldDecl(FieldDecl *Decl);
   PropDecl* visitPropDecl(PropDecl *Decl);
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
   Expression* visitMemberRefExpr(MemberRefExpr *node);
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

   SemaPass &SP;
   ASTContext &Context;
   SemaPass::DeclScopeRAII InstScope;

   MultiLevelTemplateArgList templateArgs;
   NamedDecl* Template = nullptr;

   llvm::StringMap<ResolvedTemplateArg*> VariadicTemplateArgs;

   bool InUnevaluatedScope = false;

   struct {
      IdentifierInfo *Name;
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
                   const IdentifierInfo *FuncArg,
                   DeclarationName IdentSubst,
                   ResolvedTemplateArg *ArgSubst)
         : Param(Param), FuncArg(FuncArg), IdentSubst(IdentSubst),
           ArgSubst(ArgSubst)
      { }

      TemplateParamDecl *Param;

      const IdentifierInfo *FuncArg;
      DeclarationName IdentSubst;

      ResolvedTemplateArg *ArgSubst;
   };

   struct SubstContextRAII {
      SubstContextRAII(InstantiatorImpl &Inst,
                       TemplateParamDecl *Param,
                       const IdentifierInfo *FuncArg,
                       DeclarationName IdentSubst,
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
      for (auto &list : templateArgs) {
         NamedDecl *Curr = list->getTemplate();
         if (!Curr)
            continue;

         while (true) {
            for (TemplateParamDecl *Param : Curr->getTemplateParams())
               if (Param->getName() == Name)
                  return Param;

            auto Parent = Curr->getDeclContext();
            if (auto ND = dyn_cast<NamedDecl>(Parent)) {
               if (ND->isInstantiation()) {
                  Curr = ND->getSpecializedTemplate();
               }
               else {
                  Curr = ND;
               }
            }
            else {
               break;
            }
         }
      }

      return nullptr;
   }

   ResolvedTemplateArg* hasTemplateArg(llvm::StringRef Name)
   {
      if (auto P = getParameterSubst(Name))
         return P;

      return templateArgs.getNamedArg(Name);
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

   template<class T>
   std::vector<TemplateParamDecl*> copyTemplateParameters(T *from)
   {
      std::vector<TemplateParamDecl*> Params;
      auto OriginalParams = from->getTemplateParams();

      for (auto &P : OriginalParams) {
         if (!hasTemplateArg(P->getName()))
            Params.push_back(clone(P));
      }

      return Params;
   }

   void copyArgListAndFindVariadic(
                              CallableDecl *C,
                              llvm::SmallVectorImpl<FuncArgDecl*> &Variadics);

   template<class Container,
            class Expr = typename
               std::remove_reference<decltype(*Container().front())>::type>
   std::vector<Expr*> copyExprList(const Container &exprList);

   std::vector<SourceType> copyTypeList(llvm::ArrayRef<SourceType> Tys);

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

         return visitChildren(Expr);
      }

   private:
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

      auto GivenArg = hasTemplateArg(ParameterPack->getName());
      if (!GivenArg) {
         // current context is still templated
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

      auto GivenArg = hasTemplateArg(ParameterPack->getName());
      if (!GivenArg) {
         // current context is still templated
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

      auto GivenArg = hasTemplateArg(ParameterPack->getName());
      if (!GivenArg) {
         // current context is still templated
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
   if (P->isTypeName()) {
      return TemplateParamDecl::Create(Context, P->getDeclName(),
                                       visitOrAuto(P->getCovariance()),
                                       visitOrAuto(P->getContravariance()),
                                       copyOrNull(P->getDefaultValue()),
                                       P->getIndex(),
                                       P->getTypeNameOrValueLoc(),
                                       P->getNameLoc(), P->getEllipsisLoc());
   }

   return TemplateParamDecl::Create(Context, P->getDeclName(),
                                    visitOrAuto(P->getCovariance()),
                                    copyOrNull(P->getDefaultValue()),
                                    P->getIndex(),
                                    P->getTypeNameOrValueLoc(),
                                    P->getNameLoc(), P->getEllipsisLoc());
}

RecordDecl *InstantiatorImpl::instantiateRecordDecl(RecordDecl *Decl)
{
   AccessSpecifier access = Decl->getAccess();
   DeclarationName Name = Decl->getDeclName();

   if (Decl == Template)
      Name = Context.getDeclNameTable()
                    .getInstantiationName(Decl->getDeclName(),
                                          templateArgs.innermost());

   auto conformances = copyTypeList(Decl->getConformanceTypes());
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

   Inst->setLastMethodID(Decl->getLastMethodID());

   Context.setConstraints(Inst, constraints);
   return Inst;
}

RecordDecl *InstantiatorImpl::instantiateRecordDefinition(RecordDecl *Template,
                                                          RecordDecl *Inst) {
   {
      SemaPass::DeclContextRAII declContext(SP, Inst,
                                            SemaPass::SetParentContext);

      for (const auto &namedDecl : Template->getDecls()) {
         if (isa<TemplateParamDecl>(namedDecl))
            continue;

         if (namedDecl->isSynthesized() || namedDecl->isInExtension())
            continue;

         (void) visit(namedDecl);
      }
   }

   for (auto &E : Template->getExtensions()) {
      auto ExtInst = visitExtensionDecl(E);
      if (!ExtInst)
         continue;

      ExtInst->setExtendedRecord(Inst);
      Inst->addExtension(ExtInst);

      SP.declareExtensionDecl(ExtInst);
   }

   return Inst;
}

RecordDecl *InstantiatorImpl::visitRecordCommon(RecordDecl *Template)
{
   auto Inst = instantiateRecordDecl(Template);

   auto R = instantiateRecordDefinition(Template, Inst);
   if (R) {
      SP.ActOnRecordDecl(R);
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

   SP.addDeclToContext(SP.getDeclContext(), EC);
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

ExtensionDecl* InstantiatorImpl::visitExtensionDecl(ExtensionDecl *Ext)
{
   AccessSpecifier access = Ext->getAccess();
   auto conformances = copyTypeList(Ext->getConformanceTypes());
   auto constraints = cloneVector(Ext->getConstraints());

   auto Inst = ExtensionDecl::Create(Context, access, Ext->getExtLoc(),
                                     Ext->getExtendedRecord(),
                                     move(conformances));

   {
      SemaPass::DeclContextRAII declContext(SP, Inst,
                                            SemaPass::SetParentContext);

      for (const auto &namedDecl : Ext->getDecls()) {
         if (isa<TemplateParamDecl>(namedDecl))
            continue;

         (void) visit(namedDecl);
      }
   }

   Context.setConstraints(Inst, constraints);
   return Inst;
}

PropDecl* InstantiatorImpl::visitPropDecl(PropDecl *Decl)
{
   auto Prop =
      PropDecl::Create(Context, Decl->getAccess(), Decl->getSourceRange(),
                       Decl->getDeclName(), visit(Decl->getType()),
                       Decl->isStatic(), Decl->hasDefinition(),
                       Decl->hasGetter(), Decl->hasSetter(),
                       nullptr, nullptr,
                       Decl->getNewValNameInfo());

   Prop->setPropTemplate(Decl);
   SP.addDeclToContext(SP.getDeclContext(), Prop);

   return Prop;
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

   llvm::SmallVector<FuncArgDecl*, 4> Args;
   copyArgListAndFindVariadic(M, Args);

   auto templateParams = copyTemplateParameters(M);

   MethodDecl *Inst;
   if (M->isConversionOp()) {
      Inst = MethodDecl::CreateConversionOp(Context, M->getAccess(),
                                            M->getDefLoc(), M->getReturnType(),
                                            Args, move(templateParams),
                                            nullptr);
   }
   else if (M->isOperator()) {
      Inst = MethodDecl::CreateOperator(Context, M->getAccess(), M->getDefLoc(),
                                        Name, visit(M->getReturnType()),
                                        Args, move(templateParams), nullptr,
                                        M->getOperator(), M->isStatic());
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

   if (!IsMainTemplate)
      SP.ActOnMethodDecl(Inst);

   return Inst;
}

AssociatedTypeDecl*
InstantiatorImpl::visitAssociatedTypeDecl(AssociatedTypeDecl *node)
{
   auto Inst = AssociatedTypeDecl::Create(Context, node->getSourceLoc(),
                                          node->getProtoSpecInfo(),
                                          node->getDeclName(),
                                          visit(node->getActualType()));

   Context.setConstraints(Inst, cloneVector(node->getConstraints()));
   SP.addDeclToContext(SP.getDeclContext(), Inst);

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
      SP.addDeclToContext(SP.getDeclContext(), Inst);

   return Inst;
}

AliasDecl* InstantiatorImpl::visitAliasDecl(AliasDecl *Alias)
{
   bool IsMainTemplate = Alias == Template;
   DeclarationName Name = Alias->getDeclName();
   if (IsMainTemplate)
      Name = Context.getDeclNameTable()
                    .getInstantiationName(Alias->getDeclName(),
                                          templateArgs.innermost());

   auto Inst = AliasDecl::Create(Context, Alias->getSourceLoc(),
                                 Alias->getAccess(), Name,
                                 visit(Alias->getType()),
                                 copyOrNull(Alias->getAliasExpr()),
                                 copyTemplateParameters(Alias));

   Context.setConstraints(Inst, cloneVector(Alias->getConstraints()));

   if (!IsMainTemplate)
      SP.ActOnAliasDecl(Inst);

   return Inst;
}

FieldDecl* InstantiatorImpl::visitFieldDecl(FieldDecl *Decl)
{
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

   SP.addDeclToContext(SP.getDeclContext(), Inst);
   return Inst;
}

InitDecl* InstantiatorImpl::visitInitDecl(InitDecl *Decl)
{
   bool IsMainTemplate = Decl == Template;
   bool SkipBody = !IsMainTemplate;

   DeclarationName Name =
      Context.getDeclNameTable()
             .getConstructorName(
                Context.getRecordType(cast<RecordDecl>(&SP.getDeclContext())));

   if (IsMainTemplate)
      Name = Context.getDeclNameTable()
                    .getInstantiationName(Name, templateArgs.innermost());

   llvm::SmallVector<FuncArgDecl*, 4> Args;
   copyArgListAndFindVariadic(Decl, Args);

   auto templateParams = copyTemplateParameters(Decl);
   auto Inst = InitDecl::Create(Context, Decl->getAccess(),
                                Decl->getSourceLoc(), Args,
                                move(templateParams), nullptr,
                                Name);

   if (SkipBody) {
      Inst->setBodyTemplate(Decl);
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

   if (!IsMainTemplate)
      SP.ActOnInitDecl(Inst);

   return Inst;
}

DeinitDecl* InstantiatorImpl::visitDeinitDecl(DeinitDecl *Decl)
{
   DeclarationName Name =
      Context.getDeclNameTable()
             .getDestructorName(
                Context.getRecordType(cast<RecordDecl>(&SP.getDeclContext())));

   auto Inst = DeinitDecl::Create(Context, Decl->getSourceLoc(), nullptr, Name);

   Inst->setMethodID(Decl->getMethodID());
   Inst->setBodyTemplate(Decl);
   Inst->setFunctionFlags(Decl->getFunctionFlags());

   SP.ActOnDeinitDecl(Inst);
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
                                    nullptr, F->getOperator(),
                                    move(templateParams));

   {
      SemaPass::DeclContextRAII declContext(SP, Inst,
                                            SemaPass::SetParentContext);

      Inst->setBody(copyOrNull(F->getBody()));
   }

   Inst->setFunctionFlags(F->getFunctionFlags());
   VariadicTemplateArgs.clear();

   if (!IsMainTemplate)
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
                                copyTypeList(Expr->getContainedTypes()),
                                Expr->isMeta());
}

FunctionTypeExpr*
InstantiatorImpl::visitFunctionTypeExpr(FunctionTypeExpr *Expr)
{
   return FunctionTypeExpr::Create(Context, Expr->getSourceRange(),
                                   visit(Expr->getReturnType()),
                                   copyTypeList(Expr->getArgTypes()),
                                   Expr->isMeta());
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
   llvm::SmallVector<Statement*, 8> Stmts;
   for (auto &Stmt : node->getStatements())
      Stmts.push_back(visit(Stmt));

   return CompoundStmt::Create(Context, Stmts, node->preservesScope(),
                               node->getLBraceLoc(), node->getRBraceLoc());
}

void InstantiatorImpl::copyArgListAndFindVariadic(
                                 CallableDecl *C,
                                 llvm::SmallVectorImpl<FuncArgDecl*>&Variadics){
   auto argList = C->getArgs();
   for (auto arg : argList) {
      if (!arg->isVariadicArgPackExpansion()) {
         Variadics.push_back(clone(arg));
      }
      else {
         expandVariadicDecl(arg, Variadics);
      }
   }

//   std::vector<FuncArgDecl*> args;
//   for (const auto &arg : argList) {
//      if (!arg->isVariadicArgPackExpansion()) {
//         args.push_back(clone(arg));
//         continue;
//      }
//
//      auto GenTy = dyn_cast<GenericType>(arg->getType());
//      if (!GenTy) {
//         args.push_back(clone(arg));
//         continue;
//      }
//
//      auto name = GenTy->getGenericTypeName();
//      auto TA = hasTemplateArg(name);
//
//      if (TA && TA->isVariadic()) {
//         VariadicTemplateArgs.try_emplace(arg->getName(), TA);
//
//         string newName("__");
//         newName += arg->getName();
//
//         auto initialSize = newName.size();
//
//         size_t i = 0;
//         for (const auto &VA : TA->getVariadicArgs()) {
//            newName.resize(initialSize);
//            newName += std::to_string(i++);
//
//            auto Name = &Context.getIdentifiers().get(newName);
//            args.push_back(FuncArgDecl::Create(Context, arg->getVarOrLetLoc(),
//                                               arg->getColonLoc(), Name,
//                                               SourceType(VA.getType()),
//                                               copyOrNull(arg->getDefaultVal()),
//                                               false, arg->isConst()));
//         }
//
//         continue;
//      }
//
//      if (hasTemplateParam(name)) {
//         VariadicTemplateArgs.try_emplace(arg->getName(), nullptr);
//      }
//
//      args.push_back(clone(arg));
//   }
//
//   return args;
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

   return FuncArgDecl::Create(Context, Decl->getVarOrLetLoc(),
                              Decl->getColonLoc(), DeclName,
                              visit(Decl->getType()),
                              copyOrNull(Decl->getDefaultVal()),
                              Decl->isVariadicArgPackExpansion()
                                  && !IsSubstitution,
                              Decl->isConst(), Decl->isCstyleVararg());
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
   return IntegerLiteral::Create(Context, node->getSourceRange(),
                                 node->getType(),
                                 llvm::APSInt(node->getValue()),
                                 node->getSuffix());
}

FPLiteral *InstantiatorImpl::visitFPLiteral(FPLiteral *node)
{
   return FPLiteral::Create(Context, node->getSourceRange(),
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
   return SelfExpr::Create(Context, node->getSourceLoc());
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

NoneLiteral* InstantiatorImpl::visitNoneLiteral(NoneLiteral *node)
{
   return NoneLiteral::Create(Context, node->getSourceLoc());
}

StringInterpolation*
InstantiatorImpl::visitStringInterpolation(StringInterpolation *node)
{
   return StringInterpolation::Create(Context, node->getSourceRange(),
                                      cloneVector(node->getStrings()));
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

   SP.addDeclToContext(SP.getDeclContext(), Decl);
   return Decl;
}

ast::LocalDestructuringDecl*
InstantiatorImpl::visitLocalDestructuringDecl(LocalDestructuringDecl *node)
{
   return LocalDestructuringDecl::Create(Context, node->getSourceRange(),
                                         node->getAccess(), node->isConst(),
                                         cloneVector(node->getDecls()),
                                         visitOrAuto(node->getType()),
                                         copyOrNull(node->getValue()));
}

ast::GlobalDestructuringDecl*
InstantiatorImpl::visitGlobalDestructuringDecl(GlobalDestructuringDecl *node)
{
   return GlobalDestructuringDecl::Create(Context, node->getSourceRange(),
                                          node->getAccess(), node->isConst(),
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
         literal = CharLiteral::Create(Context, Expr->getSourceRange(),
                                       getContext().getCharTy(), Val.getChar());

         break;
      default: {
         literal = IntegerLiteral::Create(Context, Expr->getSourceRange(),
                                          valTy, llvm::APSInt(Val.getAPSInt()));
         break;
      }
      }
   }
   else if (valTy->isFPType()) {
      literal = FPLiteral::Create(Context, Expr->getSourceRange(), valTy,
                                  llvm::APFloat(Val.getAPFloat()));
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

Expression* InstantiatorImpl::visitIdentifierRefExpr(IdentifierRefExpr *Ident)
{
   if (!Ident->getParentExpr()) {
      if (auto Arg = hasTemplateArg(Ident->getIdent())) {
         if (Arg->isVariadic()) {
            auto Subst = getParameterSubst(Ident->getIdentInfo()
                                                ->getIdentifier());

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

         return makeLiteralExpr(Ident, hasTemplateParam(Ident->getIdent())
                                   ->getValueType(),
                                Arg->getValue());
      }

      if (ValueSubst.Name == Ident->getIdentInfo()) {
         return makeLiteralExpr(Ident, ValueSubst.Ty, *ValueSubst.Val);
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

   // only copy the context if it is not (in) a template, if it is it has to be
   // re-evaluated
   DeclContext *Ctx = Ident->getDeclCtx();
   if (auto ND = dyn_cast_or_null<NamedDecl>(Ctx))
      if (ND->inDependentContext())
         Ctx = nullptr;

   auto Inst = new(Context)
      IdentifierRefExpr(Ident->getSourceLoc(), DeclName,
                        copyExprList(Ident->getTemplateArgs()),
                        Ctx);

   Inst->setParentExpr(copyOrNull(Ident->getParentExpr()));
   Inst->setPointerAccess(Ident->isPointerAccess());

   return Inst;
}

BuiltinIdentExpr*InstantiatorImpl::visitBuiltinIdentExpr(BuiltinIdentExpr *node)
{
   return BuiltinIdentExpr::Create(Context, node->getSourceLoc(),
                                   node->getIdentifierKind());
}

Expression* InstantiatorImpl::visitMemberRefExpr(MemberRefExpr *node)
{
   DeclarationName DeclName = node->getDeclName();
   if (DeclName.isSimpleIdentifier()
         && ValueSubst.Name == DeclName.getIdentifierInfo()) {
      if (ValueSubst.Val->isStr()) {
         DeclName = &Context.getIdentifiers().get(ValueSubst.Val->getString());
      }
      else if (ValueSubst.Val->isInt()) {
         return new(Context)
            TupleMemberExpr(node->getSourceLoc(),
                            copyOrNull(node->getParentExpr()),
                            (unsigned)ValueSubst.Val->getZExtValue(), // FIXME
                            node->isPointerAccess());
      }
      else {
         llvm_unreachable("produce error here!");
      }
   }

   auto expr = new(Context) MemberRefExpr(node->getSourceLoc(),
                                          copyOrNull(node->getParentExpr()),
                                          DeclName, node->isPointerAccess());

   copyTemplateArgs(node, expr);
   return expr;
}

EnumCaseExpr* InstantiatorImpl::visitEnumCaseExpr(EnumCaseExpr *node)
{
   return new(Context) EnumCaseExpr(node->getSourceLoc(),
                                    node->getIdentInfo(),
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

            return IntegerLiteral::Create(Context, node->getSourceRange(),
                                          getContext().getUIntTy(),
                                          std::move(Int));
         }
      }
   }

   DeclarationName ident;
   if (auto DeclName = node->getDeclName()) {
      if (DeclName.isSimpleIdentifier())
         if (auto Param = hasTemplateArg(DeclName.getIdentifierInfo()
                                                 ->getIdentifier())) {
            auto ty = Param->getType();
            ident = &Context.getIdentifiers().get(ty->toString());
         }
   }

   if (!ident)
      ident = node->getDeclName();

   DeclContext *Ctx = node->getContext();
   if (auto ND = dyn_cast_or_null<NamedDecl>(Ctx))
      if (ND->inDependentContext())
         Ctx = nullptr;

   auto call = new(Context) CallExpr(node->getIdentLoc(), node->getParenRange(),
                                     copyOrNull(node->getParentExpr()),
                                     copyExprList(node->getArgs()), ident);

   copyTemplateArgs(node, call);

   call->setContext(Ctx);
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

   return ReturnStmt::Create(Context, node->getSourceLoc());
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
   return CastExpr::Create(Context, Expr->getAsLoc(),
                           Expr->getStrength(), visit(Expr->getTarget()),
                           visit(Expr->getTargetType()));
}

TypePredicateExpr*
InstantiatorImpl::visitTypePredicateExpr(TypePredicateExpr *Expr)
{
   return TypePredicateExpr::Create(Context,
                                    Expr->getIsLoc(),
                                    Expr->getSourceRange(),
                                    visit(Expr->getLHS()),
                                    clone(Expr->getRHS()));
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
         args.emplace_back(arg.getIdentifier(), arg.isConst(),
                           arg.getSourceLoc());
      }
   }

   return CasePattern::Create(Context, node->getSourceRange(),
                              node->getCaseNameIdent(), args);
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

UsingDecl* InstantiatorImpl::visitUsingDecl(UsingDecl *node)
{
   return UsingDecl::Create(Context, node->getSourceRange(),
                            node->getAccess(), node->getDeclName(),
                            node->getNestedImportName(),
                            node->isWildCardImport());
}

ModuleDecl* InstantiatorImpl::visitModuleDecl(ModuleDecl *node)
{
   return ModuleDecl::Create(Context, node->getSourceRange(),
                             node->getAccess(),
                             node->getQualifiedModuleName());
}

ImportDecl* InstantiatorImpl::visitImportDecl(ImportDecl *node)
{
   return ImportDecl::Create(Context, node->getSourceRange(),
                             node->getAccess(),
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
   auto Inst = MixinDecl::Create(Context, Decl->getMixinLoc(),
                                 Decl->getParenRange(),
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

   auto Decl = StaticIfDecl::Create(Context, node->getStaticLoc(),
                                    node->getRBRaceLoc(),
                                    clone(node->getCondition()),
                                    copyOrNull(node->getIfDecl()),
                                    copyOrNull(node->getElseDecl()));

   SP.addDeclToContext(SP.getDeclContext(), Decl);
   return Decl;
}

StaticForDecl* InstantiatorImpl::visitStaticForDecl(StaticForDecl *node)
{
   auto Decl = StaticForDecl::Create(Context, node->getStaticLoc(),
                                     node->getRBRaceLoc(),
                                     node->getElementName(),
                                     clone(node->getRange()),
                                     copyOrNull(node->getBodyDecl()));

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
                                     node->getElementName(),
                                     visitStaticExpr(node->getRange()),
                                     visit(node->getBody()));
}

StaticAssertStmt*InstantiatorImpl::visitStaticAssertStmt(StaticAssertStmt *node)
{
   auto Decl = StaticAssertStmt::Create(Context, node->getStaticAssertLoc(),
                                        node->getParenRange(),
                                        visitStaticExpr(node->getExpr()),
                                        string(node->getMessage()));

   SP.addDeclToContext(SP.getDeclContext(), Decl);
   return Decl;
}

StaticPrintStmt* InstantiatorImpl::visitStaticPrintStmt(StaticPrintStmt *node)
{
   auto Decl = StaticPrintStmt::Create(Context, node->getStaticPrintLoc(),
                                       node->getParenRange(),
                                       visitStaticExpr(node->getExpr()));

   SP.addDeclToContext(SP.getDeclContext(), Decl);
   return Decl;
}

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

RecordInstResult
TemplateInstantiator::InstantiateRecord(StmtOrDecl POI,
                                        RecordDecl *Template,
                                        TemplateArgList&& templateArgs,
                                        bool *isNew) {
   SP.ensureDeclared(Template);

   if (templateArgs.isStillDependent() || Template->isInvalid())
      return RecordInstResult();

   assert(!Template->isInstantiation() && "only instantiate base template!");

   void *insertPos;
   if (auto R = SP.getContext().getRecordTemplateInstantiation(Template,
                                                               templateArgs,
                                                               insertPos)) {
      if (isNew) *isNew = false;
      return R;
   }

   // create this beforehand so we don't accidentally use the template arg
   // list after moving it
   auto instInfo = new (SP.getContext()) InstantiationInfo<RecordDecl> {
      POI.getSourceRange().getStart(), move(templateArgs), Template,
      SP.getCurrentDecl()
   };

   InstantiatorImpl Instantiator(SP, instInfo->templateArgs, Template);
   InstPrettyStackTraceEntry STE(Instantiator);

   RecordDecl *Inst = Instantiator.instantiateRecordDecl(Template);
   Inst->setInstantiationInfo(instInfo);
   Inst->setIsInstantiation(true);

   bool DoingDeclarations = SP.getStage() == SemaPass::Stage::Declaration;

   bool AlreadyInstantiating = InstantiatingRecord;
   InstantiatingRecord = true;

   // avoid infinite recursion when this template is instantiated within its
   // own definition
   SP.getContext().insertRecordTemplateInstantiation(Inst, insertPos);
   Instantiator.instantiateRecordDefinition(Template, Inst);

   SP.ActOnRecordDecl(Inst);
   SP.declareRecordInstantiation(POI, Inst);

   // there might be dependencies between nested instantiations, so we can't
   // calculate record sizes until we are not nested anymore
   if (!AlreadyInstantiating && !DoingDeclarations) {
      SP.finalizeRecordDecls();
   }

   InstantiatingRecord = AlreadyInstantiating;

   if (isNew)
      *isNew = true;

   return Inst;
}

FunctionInstResult
TemplateInstantiator::InstantiateFunction(StmtOrDecl POI,
                                          FunctionDecl *Template,
                                          TemplateArgList&& templateArgs,
                                          bool *isNew) {
   SP.ensureVisited(Template);

   if (Template->isInvalid()) {
      POI.setIsInvalid(true);
      return FunctionInstResult();
   }

   if (templateArgs.isStillDependent())
      return FunctionInstResult();

   void *insertPos;
   if (auto F = SP.getContext().getFunctionTemplateInstantiation(Template,
                                                                 templateArgs,
                                                                 insertPos)) {
      if (isNew) *isNew = false;
      return cast<FunctionDecl>(F);
   }

   InstantiatorImpl Instantiator(SP, templateArgs, Template);
   InstPrettyStackTraceEntry STE(Instantiator);

   auto Inst = Instantiator.instantiateFunctionDecl(Template);

   SemaPass::DeclScopeRAII raii(SP, Inst->getDeclContext());
   SP.getContext().insertFunctionTemplateInstantiation(Inst, insertPos);

   auto instInfo = new (SP.getContext()) InstantiationInfo<CallableDecl>{
      POI.getSourceRange().getStart(), move(templateArgs), Template,
      SP.getCurrentDecl()
   };

   Inst->setInstantiationInfo(instInfo);
   Inst->setIsInstantiation(true);

   SP.ActOnFunctionDecl(Inst);
   SP.declareScoped(Inst);
   SP.registerDelayedInstantiation(Inst, POI);

   if (isNew)
      *isNew = true;

   return Inst;
}

MethodInstResult
TemplateInstantiator::InstantiateMethod(StmtOrDecl POI,
                                        MethodDecl *Template,
                                        TemplateArgList&& templateArgs,
                                        bool *isNew) {
   if (Template->isInvalid()) {
      POI.setIsInvalid(true);
      return MethodInstResult();
   }

   if (templateArgs.isStillDependent())
      return MethodInstResult();

   void *insertPos;
   if (auto M = SP.getContext().getFunctionTemplateInstantiation(Template,
                                                                 templateArgs,
                                                                 insertPos)) {
      if (isNew) *isNew = false;
      return cast<MethodDecl>(M);
   }

   MultiLevelTemplateArgList MultiLevelList(templateArgs);
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

   auto instInfo = new (SP.getContext()) InstantiationInfo<CallableDecl>{
      POI.getSourceRange().getStart(), move(templateArgs), Template,
      SP.getCurrentDecl()
   };

   Inst->setInstantiationInfo(instInfo);
   Inst->setIsInstantiation(true);

   SP.addDeclToContext(SP.getDeclContext(), Inst);
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
   SemaPass::DeclScopeRAII declScopeRAII(SP, Rec);
   InstantiatorImpl Instantiator(SP, {});

   auto decl = Instantiator.instantiateMethodDecl(M);
   (void)SP.declareStmt(Rec, decl);

   return decl;
}

StmtResult
TemplateInstantiator::InstantiateStatement(SourceLocation,
                                           Statement* stmt,
                                           sema::MultiLevelTemplateArgList
                                                               &&templateArgs) {
   InstantiatorImpl Instantiator(SP, move(templateArgs));
   return Instantiator.instantiateStatement(stmt);
}

StmtResult
TemplateInstantiator::InstantiateStatement(StmtOrDecl,
                                           Statement *stmt,
                                           IdentifierInfo *SubstName,
                                           QualType SubstTy,
                                           const Variant &SubstVal) {
   InstantiatorImpl Instantiator(SP, SubstName, SubstTy, SubstVal);
   return Instantiator.instantiateStatement(stmt);
}

StmtResult
TemplateInstantiator::InstantiateMethodBody(StmtOrDecl POI,
                                            MethodDecl *Method) {
   SP.ensureVisited(Method->getBodyTemplate());

   MultiLevelTemplateArgList ArgList;
   if (Method->getRecord()->isInstantiation()) {
      ArgList.addOuterList(Method->getRecord()->getTemplateArgs());
   }

   InstantiatorImpl Instantiator(SP, move(ArgList), Method);
   auto Inst = Instantiator.instantiateStatement(Method->getBodyTemplate()
                                                       ->getBody());

   Method->setBody(Inst);
   Method->setBodyInstantiationLoc(POI.getSourceLoc());

   auto instInfo = new (SP.getContext()) InstantiationInfo<CallableDecl>{
      POI.getSourceLoc(), {}, Method, SP.getCurrentDecl()
   };

   Method->setInstantiationInfo(instInfo);

   SP.declareScoped(Method);
   SP.registerDelayedInstantiation(Method, POI);

   return Inst;
}

ExprResult
TemplateInstantiator::InstantiateStaticExpr(SourceLocation instantiatedFrom,
                                            Expression* expr,
                                            sema::MultiLevelTemplateArgList
                                                         &&templateArgs) {
   auto Stmt = InstantiateStatement(instantiatedFrom,
                                    expr, move(templateArgs));
   
   if (!Stmt)
      return ExprError();
   
   return cast<Expression>(Stmt.getValue());
}

AliasInstResult
TemplateInstantiator::InstantiateAlias(AliasDecl *Template,
                                       SourceLocation instantiatedFrom,
                                       TemplateArgList &&templateArgs) {
   void *insertPos;
   if (auto Inst = SP.getContext().getAliasTemplateInstantiation(Template,
                                                                 templateArgs,
                                                                 insertPos)) {
      return Inst;
   }

   auto *InstInfo = new(SP.getContext()) InstantiationInfo<AliasDecl>(
      instantiatedFrom, move(templateArgs), Template, SP.getCurrentDecl()
   );

   InstantiatorImpl Instantiator(SP, InstInfo->templateArgs, Template);
   InstPrettyStackTraceEntry STE(Instantiator);

   auto Inst = Instantiator.instantiateAliasDecl(Template);
   Inst->setInstantiationInfo(InstInfo);
   Inst->setIsInstantiation(true);

   SP.ActOnAliasDecl(Inst);
   SP.getContext().insertAliasTemplateInstantiation(Inst, insertPos);

   return Inst;
}

} // namespace cdot