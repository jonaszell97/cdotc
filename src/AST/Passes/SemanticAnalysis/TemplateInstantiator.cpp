//
// Created by Jonas Zell on 05.12.17.
//

#include "TemplateInstantiator.h"

#include "AST/Passes/SemanticAnalysis/Template.h"
#include "AST/Passes/SemanticAnalysis/SemaPass.h"

#include "AST/Passes/StaticExpr/StaticExprEvaluator.h"
#include "AST/Passes/Declaration/DeclPass.h"

#include "AST/Passes/AggregateVisitor.h"

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

class InstantiatorImpl: public DiagnosticIssuer {
public:
   enum InstantiationKind {
      IK_Record,
      IK_Method,
      IK_Function,
      IK_ProtocolDefaultImpl,
      IK_AnyStmt,
      IK_Alias,
   };

   InstantiatorImpl(SemaPass &SP, TemplateArgList const& templateArgs)
      : SP(SP), kind(IK_AnyStmt), templateArgs(templateArgs),
        templateArgString(templateArgs.toString())
   {

   }

   InstantiatorImpl(SemaPass &SP,
                    TemplateArgList const& templateArgs,
                    NamedDecl const* Template)
      : SP(SP), kind(IK_AnyStmt), templateArgs(templateArgs),
        Template(Template),
        templateArgString(templateArgs.toString())
   {
      if (isa<FunctionDecl>(Template))
         kind = IK_Function;
      else if (isa<MethodDecl>(Template))
         kind = IK_Method;
      else if (isa<AliasDecl>(Template))
         kind = IK_Alias;
      else if (isa<RecordDecl>(Template))
         kind = IK_Record;
   }

   Expression* visitExpr(Expression *expr)
   {
      switch (expr->getTypeID()) {
#        define CDOT_EXPR(Name)                                \
            case AstNode::Name##ID:                            \
               return visit##Name(static_cast<Name*>(expr));
#        include "../../AstNode.def"

         default:
            llvm_unreachable("not an expression!");
      }
   }

   Statement* visitStmt(Statement *stmt)
   {
      switch (stmt->getTypeID()) {
#        define CDOT_STMT(Name)                                \
            case AstNode::Name##ID:                            \
               return visit##Name(static_cast<Name*>(stmt));
#        include "../../AstNode.def"

         default:
            llvm_unreachable("not a statement!");
      }
   }

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

   RecordDecl *visitRecordCommon(RecordDecl *src,
                                 RecordDecl *outer = nullptr);

   RecordDecl* visitRecordDecl(RecordDecl *node, RecordDecl *outer = nullptr)
   {
      RecordDecl* res;
      switch (node->getTypeID()) {
         case AstNode::ClassDeclID:
            res = visitClassDecl(cast<ClassDecl>(node), outer);
            break;
         case AstNode::StructDeclID:
            res = visitStructDecl(cast<StructDecl>(node), outer);
            break;
         case AstNode::EnumDeclID:
            res = visitEnumDecl(cast<EnumDecl>(node), outer);
            break;
         case AstNode::UnionDeclID:
            res = visitUnionDecl(cast<UnionDecl>(node), outer);
            break;
         case AstNode::ProtocolDeclID:
            res = visitProtocolDecl(cast<ProtocolDecl>(node), outer);
            break;
         case AstNode::ExtensionDeclID:
            res = visitExtensionDecl(cast<ExtensionDecl>(node));
            break;
         default:
            llvm_unreachable("bad record decl kind");
      }

      return res;
   }

   TemplateArgExpr *visitTemplateArgExpr(TemplateArgExpr *expr);
   TemplateParamDecl *visitTemplateParamDecl(TemplateParamDecl *decl);

   ClassDecl* visitClassDecl(ClassDecl *node, RecordDecl *outer = nullptr);
   StructDecl* visitStructDecl(StructDecl *node, RecordDecl *outer = nullptr);
   ProtocolDecl* visitProtocolDecl(ProtocolDecl *node,
                                   RecordDecl *outer = nullptr);
   ExtensionDecl* visitExtensionDecl(ExtensionDecl *node);
   EnumDecl* visitEnumDecl(EnumDecl *node, RecordDecl *outer = nullptr);
   UnionDecl* visitUnionDecl(UnionDecl *node, RecordDecl *outer = nullptr);

   EnumCaseDecl* visitEnumCaseDecl(EnumCaseDecl *node);

   FieldDecl* visitFieldDecl(FieldDecl *node);
   PropDecl* visitPropDecl(PropDecl *node);
   AssociatedTypeDecl* visitAssociatedTypeDecl
      (AssociatedTypeDecl *node);

   MethodDecl* visitMethodDecl(MethodDecl *node,
                               bool skipBody = false);
   InitDecl* visitInitDecl(InitDecl *node,
                               bool skipBody = false);
   DeinitDecl* visitDeinitDecl(DeinitDecl *node,
                             bool skipBody = false);

   FuncArgDecl* visitFuncArgDecl(FuncArgDecl *node);
   TypedefDecl* visitTypedefDecl(TypedefDecl *node);
   AliasDecl* visitAliasDecl(AliasDecl *node);

   Expression*
   visitIdentifierRefExpr(IdentifierRefExpr *node);

   Expression*
   visitBuiltinExpr(BuiltinExpr *node);

   SubscriptExpr* visitSubscriptExpr(SubscriptExpr *node);
   Expression* visitCallExpr(CallExpr *node);
   MemberRefExpr* visitMemberRefExpr(MemberRefExpr *node);
   EnumCaseExpr* visitEnumCaseExpr(EnumCaseExpr *node);

   ForStmt* visitForStmt(ForStmt *node);
   ForInStmt* visitForInStmt(ForInStmt *node);
   WhileStmt* visitWhileStmt(WhileStmt *node);
   IfStmt* visitIfStmt(IfStmt *node);
   LabelStmt* visitLabelStmt(LabelStmt *node);
   GotoStmt* visitGotoStmt(GotoStmt *node);

   MatchStmt* visitMatchStmt(MatchStmt *node);
   CaseStmt* visitCaseStmt(CaseStmt *node);

   ExpressionPattern*
   visitExpressionPattern(ExpressionPattern *node);

   IsPattern* visitIsPattern(IsPattern *node);
   CasePattern* visitCasePattern(CasePattern *node);

   ReturnStmt* visitReturnStmt(ReturnStmt *node);
   BreakStmt* visitBreakStmt(BreakStmt *node);
   ContinueStmt* visitContinueStmt(ContinueStmt *node);

   IntegerLiteral* visitIntegerLiteral(IntegerLiteral *node);
   FPLiteral* visitFPLiteral(FPLiteral *node);
   BoolLiteral* visitBoolLiteral(BoolLiteral *node);
   CharLiteral* visitCharLiteral(CharLiteral *node);

   DictionaryLiteral*
   visitDictionaryLiteral(DictionaryLiteral *node);

   ArrayLiteral*
   visitArrayLiteral(ArrayLiteral *node);

   NoneLiteral* visitNoneLiteral(NoneLiteral *node);
   StringLiteral* visitStringLiteral(StringLiteral *node);

   StringInterpolation*
   visitStringInterpolation(StringInterpolation *node);

   TupleLiteral* visitTupleLiteral(TupleLiteral *node);

   Expression* visitExprSequence(ExprSequence *node);
   Expression* visitBinaryOperator(BinaryOperator *node);
   TertiaryOperator*
   visitTertiaryOperator(TertiaryOperator *node);
   UnaryOperator* visitUnaryOperator(UnaryOperator *node);

   StaticExpr* visitStaticExpr(StaticExpr *node);
   ConstraintExpr* visitConstraintExpr(ConstraintExpr *node);
   TraitsExpr* visitTraitsExpr(TraitsExpr *node);

   StaticStmt* visitStaticStmt(StaticStmt *node);
   StaticAssertStmt*
   visitStaticAssertStmt(StaticAssertStmt *node);

   StaticPrintStmt*
   visitStaticPrintStmt(StaticPrintStmt *node);

   StaticIfStmt* visitStaticIfStmt(StaticIfStmt *node);
   StaticForStmt* visitStaticForStmt(StaticForStmt *node);

   TryStmt* visitTryStmt(TryStmt *node);
   ThrowStmt* visitThrowStmt(ThrowStmt *node);

   LambdaExpr* visitLambdaExpr(LambdaExpr *node);

   Expression*
   visitImplicitCastExpr(ImplicitCastExpr *node);

   TypeRef* visitTypeRef(TypeRef *node);

   Expression* visitLvalueToRvalue(LvalueToRvalue *node);

   DebugStmt* visitDebugStmt(DebugStmt *node);
   NullStmt* visitNullStmt(NullStmt *node);

   const string &getTemplateArgString() const
   {
      return templateArgString;
   }

   const SourceLocation &getFailedConstraintLoc() const
   {
      return failedConstraintLoc;
   }

   using ArgListTy = std::vector<FuncArgDecl*>;

   void findVariadicArgs(const ArgListTy &argList,
                         llvm::ArrayRef<TemplateParamDecl*> pars);

private:
   SemaPass &SP;
   InstantiationKind kind;
   TemplateArgList const& templateArgs;

   union {
      NamedDecl const* Template = nullptr;
   };

   llvm::StringMap<ResolvedTemplateArg const*> VariadicTemplateArgs;

   string templateArgString;
   size_t InnerDecls = 0;

   std::pair<llvm::StringRef, llvm::StringRef> CurrentVariadicArgSubstitution;
   std::pair<llvm::StringRef, ResolvedTemplateArg const*>
      CurrentVariadicTypeSubstitution;

   SourceLocation failedConstraintLoc;

   struct InnerDeclRAII {
      InnerDeclRAII(InstantiatorImpl &I) : I(I)
      {
         ++I.InnerDecls;
      }

      ~InnerDeclRAII()
      {
         --I.InnerDecls;
      }

   private:
      InstantiatorImpl &I;
   };

   static TemplateParamDecl const* hasTemplateParam(
                              const std::string &Name,
                              llvm::ArrayRef<TemplateParamDecl*> params) {
      for (const auto &TP : params) {
         if (TP->getName() == Name)
            return TP;
      }

      return nullptr;
   }

   TemplateParamDecl const* hasTemplateParam(const std::string &Name) const
   {
      if (!Template)
         return nullptr;

      for (auto Param : Template->getTemplateParams())
         if (Param->getName() == Name)
            return Param;

      return nullptr;
   }

   ResolvedTemplateArg const* hasTemplateArg(const std::string &Name) const
   {
      if (CurrentVariadicTypeSubstitution.first.equals(Name))
         return CurrentVariadicTypeSubstitution.second;

      auto TA = templateArgs.getNamedArg(Name);
      if (!TA || TA->isStillDependant())
         return nullptr;

      return TA;
   }

   ASTContext const& getContext() const
   {
      return SP.getCompilationUnit().getContext();
   }

   template<class T, class ...Args>
   T* makeStmt(Statement const* node, Args&& ...args)
   {
      auto ret = SP.makeStmt<T>(std::forward<Args&&>(args)...);

      auto loc = node->getSourceLoc();
      ret->setSourceLoc({ loc.getOffset(), loc.getSourceId() });

      return ret;
   }

   template<class T, class ...Args>
   T* makeStmt(NamedDecl const* node, Args&& ...args)
   {
      auto ret = SP.makeStmt<T>(std::forward<Args&&>(args)...);

      auto loc = node->getSourceLoc();
      ret->setSourceLoc({ loc.getOffset(), loc.getSourceId() });
      ret->setExternKind(node->getExternKind());

      return ret;
   }

   template<class T, class ...Args>
   T* makeExpr(Expression const* node, Args&& ...args)
   {
      auto ret = SP.makeStmt<T>(std::forward<Args&&>(args)...);

      ret->setSourceLoc(node->getSourceLoc());
      ret->setIsVariadicArgPackExpansion(node->isVariadicArgPackExpansion());

      if (auto M = node->getSubExpr())
         ret->setSubExpr(visitExpr(M));

      return ret;
   }

   template<class T>
   std::vector<T*>
   cloneVector(const std::vector<T*> &vec)
   {
      std::vector<T*> newVec;
      for (const auto &v : vec) {
         auto newStmt = visitStmt(v);
         newVec.push_back(cast<T>(newStmt));
      }

      return newVec;
   }

   template<class T>
   T* copyOrNull(T* sp)
   {
      if (!sp)
         return nullptr;

      return cast<T>(visitStmt(sp));
   }

   template<class T, class U>
   void copyTemplateArgs(const T& from, const U& to)
   {
      to->setTemplateArgs(copyTAList(from->getTemplateArgs()));
   }

   ArgListTy copyArgListAndFindVariadic(const ArgListTy &argList,
                                        llvm::ArrayRef<TemplateParamDecl*> pars);

   using ExprList = std::vector<Expression*>;
   ExprList copyExprList(const ExprList &exprList);

   using TypeList = std::vector<TypeRef*>;
   TypeList copyTypeList(const TypeList &typeList);

   template<class T>
   void copyVariadicExpr(std::vector<T*> &exprList,
                         T* const& expr) {
      llvm::SmallVector<Expression* , 4> exprs;
      bool hasContainedPacks = expandVariadicArgument(exprs, expr);

      if (hasContainedPacks) {
         for (auto &E : exprs)
            exprList.emplace_back(move(E));

         return;
      }

      std::vector<T*> copy;
      llvm::SmallVector<llvm::StringRef, 4> ContainedVariadics;
      getContainedVariadicTypes(ContainedVariadics, expr);

      if (ContainedVariadics.empty()) {
         if (expr->isVariadicArgPackExpansion())
            err(err_generic_error)
               << "expression does not contain expandable parameter packs"
               << expr;

         exprList.emplace_back(visitExpr(expr));
         return;
      }

      bool madeCopy = false;
      for (const auto &TypeName : ContainedVariadics) {
         auto Arg = hasTemplateArg(TypeName);
         if (Arg && Arg->isVariadic()) {
            auto Param = hasTemplateParam(TypeName);
            assert(Param && "no matching template parameter");

            for (const auto &VA : Arg->getVariadicArgs()) {
               CurrentVariadicTypeSubstitution = { TypeName, &VA };

               madeCopy = true;

               auto newTy = visitExpr(expr);
               newTy->setIsVariadicArgPackExpansion(false);

               exprList.emplace_back(std::move(newTy));
            }

            CurrentVariadicTypeSubstitution = { "", nullptr };
         }
      }

      if (!madeCopy)
         exprList.emplace_back(visitExpr(expr));
   }

   bool expandVariadicArgument(
                       llvm::SmallVector<Expression* , 4> &exprs,
                       Expression* const& variadicExpr) {
      llvm::SmallVector<llvm::StringRef, 4> ContainedPacks;
      getContainedParameterPacks(ContainedPacks, variadicExpr);

      for (const auto &P : ContainedPacks) {
         auto it = VariadicTemplateArgs.find(P);
         if (it != VariadicTemplateArgs.end()) {
            auto TA = it->second;

            // current context is still templated
            if (!TA)
               return false;

            assert(TA->isVariadic() && "trying to expand non-variadic arg");

            auto numVariadics = TA->getVariadicArgs().size();
            llvm::SmallString<128> ArgName;

            for (size_t i = 0; i < numVariadics; ++i) {
               ArgName += "__";
               ArgName += P;
               ArgName += std::to_string(i);

               CurrentVariadicArgSubstitution = { P, ArgName.str() };

               auto newExpr = visitExpr(variadicExpr);
               newExpr->setIsVariadicArgPackExpansion(false);

               exprs.emplace_back(move(newExpr));
               ArgName.clear();
            }

            CurrentVariadicArgSubstitution = { "", "" };
         }
      }

      return !ContainedPacks.empty();
   }

   void getContainedVariadicTypes(llvm::SmallVector<llvm::StringRef, 4> &Tys,
                                  Expression *expr) {
      if (auto Ty = dyn_cast<TypeRef>(expr)) {
         switch (Ty->getKind()) {
            case TypeRef::ObjectType: {
               if (Ty->isSingularType()) {
                  auto Arg = hasTemplateParam(Ty->getSingularTypeName());
                  if (Arg && Arg->isVariadic())
                     Tys.push_back(Ty->getSingularTypeName());
               }

               for (auto &NS : Ty->getNamespaceQual())
                  for (auto &TA : NS.second)
                     if (TA->isTypeName())
                        getContainedVariadicTypes(Tys, TA->getType());

               break;
            }
            case TypeRef::FunctionType:
            case TypeRef::TupleType:
               for (const auto &Cont : Ty->getContainedTypes())
                  getContainedVariadicTypes(Tys, Cont.second);

               if (Ty->getKind() == TypeRef::FunctionType)
                  getContainedVariadicTypes(Tys, Ty->getReturnType());

               break;
            default:
               break;
         }

         return;
      }
      else if (auto Ident = dyn_cast<IdentifierRefExpr>(expr)) {
         for (const auto &TA : Ident->getTemplateArgs())
            if (TA->isTypeName())
               getContainedVariadicTypes(Tys, TA->getType());

         if (auto P = hasTemplateParam(Ident->getIdent()))
            if (P->isVariadic())
               Tys.push_back(Ident->getIdent());
      }
      else if (auto RefExpr = dyn_cast<MemberRefExpr>(expr)) {
         for (const auto &TA : RefExpr->getTemplateArgs())
            if (TA->isTypeName())
               getContainedVariadicTypes(Tys, TA->getType());

         if (auto P = hasTemplateParam(RefExpr->getIdent()))
            if (P->isVariadic())
               Tys.push_back(RefExpr->getIdent());
      }
      else if (auto Call = dyn_cast<CallExpr>(expr)) {
         for (const auto &TA : Call->getTemplateArgs())
            if (TA->isTypeName())
               getContainedVariadicTypes(Tys, TA->getType());

         if (auto P = hasTemplateParam(Call->getIdent()))
            if (P->isVariadic())
               Tys.push_back(Call->getIdent());
      }
      else if (auto Seq = dyn_cast<ExprSequence>(expr)) {
         for (const auto &F : Seq->getFragments()) {
            switch (F.getKind()) {
               case ExprSequence::SequenceElement::EF_Expression:
                  getContainedVariadicTypes(Tys, F.getExpr());
                  break;
               case ExprSequence::SequenceElement::EF_PossibleOperator:
                  if (auto P = hasTemplateParam(F.getOp()))
                     if (P->isVariadic())
                        Tys.push_back(F.getOp());

                  break;
               default:
                  break;
            }
         }
      }

      if (auto MemExpr = expr->getSubExpr())
         getContainedVariadicTypes(Tys, MemExpr);
   }

   void getContainedParameterPacks(llvm::SmallVector<llvm::StringRef, 4> &Packs,
                                   Expression* const& expr) {
      auto Predicate = [this](IdentifierRefExpr *node) {
         auto it = VariadicTemplateArgs.find(node->getIdent());
         return it != VariadicTemplateArgs.end();
      };

      auto Visitor = makeAggrVisitor<IdentifierRefExpr>(expr, Predicate);
      for (const auto &node : Visitor.getFoundNodes())
         Packs.push_back(node->getIdent());
   }

   std::vector<TemplateArgExpr*> copyTAList(const std::vector<TemplateArgExpr*>& list);
   TypeRef::NamespaceVec copyTAList(const TypeRef::NamespaceVec& list);
};

TemplateArgExpr* InstantiatorImpl::visitTemplateArgExpr(TemplateArgExpr *expr)
{
   if (expr->isTypeName()) {
      return makeExpr<TemplateArgExpr>(expr, visitTypeRef(expr->getType()));
   }
   else {
      return makeExpr<TemplateArgExpr>(expr, visitStaticExpr(expr->getExpr()));
   }
}

TemplateParamDecl*
InstantiatorImpl::visitTemplateParamDecl(TemplateParamDecl *P)
{
   if (P->isTypeName()) {
      return makeStmt<TemplateParamDecl>(
         P, string(P->getName()),
         visitTypeRef(P->getCovariance()),
         visitTypeRef(P->getContravariance()), P->isVariadic(),
         copyOrNull(P->getDefaultValue()));
   }
   else {
      return makeStmt<TemplateParamDecl>(
         P, string(P->getName()),
         visitTypeRef(P->getCovariance()),
         P->isVariadic(),
         copyOrNull(P->getDefaultValue()));
   }
}

RecordDecl *InstantiatorImpl::visitRecordCommon(RecordDecl *src,
                                                RecordDecl *outer) {
   auto access = src->getAccess();
   string recordName;

   if (outer) {
      recordName += outer->getName();

      auto skip = outer->getName().size() - templateArgString.size();
      recordName += src->getName().substr(skip);
   }
   else {
      recordName += src->getName();
      recordName += templateArgString;
   }

   auto conformances = copyTypeList(src->getConformanceTypes());
   auto constraints = cloneVector(src->getConstraints());

   TypeRef *enumRawTypeOrExtends = nullptr;
   bool isAbstract = false;

   if (auto E = dyn_cast<EnumDecl>(src)) {
      enumRawTypeOrExtends = copyOrNull(E->getRawType());
   }
   else if (auto C = dyn_cast<ClassDecl>(src)) {
      isAbstract = C->isAbstract();
      enumRawTypeOrExtends = copyOrNull(C->getParentType());
   }

   RecordDecl *decl;
   switch (src->getTypeID()) {
      case AstNode::StructDeclID:
         decl = makeStmt<StructDecl>(src, access,
                                     move(recordName),
                                     move(conformances),
                                     move(constraints));
         break;
      case AstNode::ClassDeclID:
         decl = makeStmt<ClassDecl>(src, access,
                                    move(recordName),
                                    move(conformances),
                                    move(constraints),
                                    enumRawTypeOrExtends, isAbstract);
         break;
      case AstNode::EnumDeclID:
         decl = makeStmt<EnumDecl>(src, access,
                                   move(recordName),
                                   move(conformances),
                                   move(constraints),
                                   enumRawTypeOrExtends);
         break;
      case AstNode::UnionDeclID:
         decl = makeStmt<UnionDecl>(src, access,
                                    move(recordName),
                                    move(conformances),
                                    move(constraints));
         break;
      case AstNode::ProtocolDeclID:
         decl = makeStmt<ProtocolDecl>(src, access,
                                       move(recordName),
                                       move(conformances),
                                       move(constraints));
         break;
      case AstNode::ExtensionDeclID:
         decl = makeStmt<ExtensionDecl>(src, access,
                                        move(recordName),
                                        move(conformances),
                                        move(constraints));
         break;
      default:
         llvm_unreachable("not a record decl!");
   }

   for (const auto &namedDecl : src->getDecls()) {
      if (auto R = dyn_cast<RecordDecl>(namedDecl)) {
         decl->addDecl(visitRecordDecl(R, decl));
      }
      else if (isa<TemplateParamDecl>(namedDecl)) {
         continue;
      }
      else {
         decl->addDecl(cast<NamedDecl>(visitStmt(namedDecl)));
      }
   }

   for (const auto &Static : src->getStaticStatements()) {
      decl->addStaticStatement(visitStaticStmt(Static));
   }

   return decl;
}

ClassDecl* InstantiatorImpl::visitClassDecl(ClassDecl *node,
                                            RecordDecl *outer) {
   return cast<ClassDecl>(visitRecordCommon(node, outer));
}

StructDecl* InstantiatorImpl::visitStructDecl(StructDecl *node,
                                              RecordDecl *outer) {
   return cast<StructDecl>(visitRecordCommon(node, outer));
}

EnumDecl* InstantiatorImpl::visitEnumDecl(EnumDecl *node, RecordDecl *outer)
{
   return cast<EnumDecl>(visitRecordCommon(node, outer));
}

EnumCaseDecl* InstantiatorImpl::visitEnumCaseDecl(EnumCaseDecl *node)
{
   return makeStmt<EnumCaseDecl>(node,
                                 string(node->getName()),
                                 copyOrNull(node->getRawValExpr()),
                                 cloneVector(node->getArgs()));
}

UnionDecl* InstantiatorImpl::visitUnionDecl(UnionDecl *node, RecordDecl *outer)
{
   return cast<UnionDecl>(visitRecordCommon(node, outer));
}

ProtocolDecl* InstantiatorImpl::visitProtocolDecl(ProtocolDecl *node,
                                                  RecordDecl *outer) {
   return cast<ProtocolDecl>(visitRecordCommon(node, outer));
}

ExtensionDecl* InstantiatorImpl::visitExtensionDecl(ExtensionDecl *node)
{
   return cast<ExtensionDecl>(visitRecordCommon(node));
}

PropDecl* InstantiatorImpl::visitPropDecl(PropDecl *node)
{
   return makeStmt<PropDecl>(node,
                             string(node->getName()),
                             visitTypeRef(node->getType()),
                             node->getAccess(), node->isStatic(),
                             node->hasDefinition(),
                             node->hasGetter(), node->hasSetter(),
                             copyOrNull(node->getGetterBody()),
                             copyOrNull(node->getSetterBody()),
                             string(node->getNewValName()));
}

MethodDecl*
InstantiatorImpl::visitMethodDecl(MethodDecl *node, bool skipBody)
{
   skipBody &= InnerDecls == 0;

   std::vector<FuncArgDecl*> args =
      copyArgListAndFindVariadic(node->getArgs(),
                                 node->getTemplateParams());

   string methodName = node->getName();
   if (kind == IK_Method)
      methodName += templateArgString;

   MethodDecl* decl;
   if (node->isAlias()) {
      decl = makeStmt<MethodDecl>(node,
                                  string(node->getAlias()),
                                  move(methodName), move(args));
   }
   else {
      decl = makeStmt<MethodDecl>(node,
                                  move(methodName),
                                  visitTypeRef(node->getReturnType()),
                                  move(args),
                                  cloneVector(node->getConstraints()),
                                  skipBody ? nullptr
                                           : copyOrNull(node->getBody()),
                                  node->getOperator(),
                                  node->isCastOp(),
                                  node->getAccess(), node->isStatic());
   }

   decl->setFunctionFlags(node->getFunctionFlags());

   auto instantiationInfo = new (getContext()) InstantiationInfo<CallableDecl>;
   instantiationInfo->specializedTemplate = node;

   decl->setInstantiationInfo(instantiationInfo);

   if (kind == IK_Record)
      decl->setMethodID(node->getMethodID());

   VariadicTemplateArgs.clear();

   return decl;
}

AssociatedTypeDecl*
InstantiatorImpl::visitAssociatedTypeDecl(AssociatedTypeDecl *node)
{
   return makeStmt<AssociatedTypeDecl>(node,
                                       string(node->getProtocolSpecifier()),
                                       string(node->getName()),
                                       cloneVector(node->getConstraints()),
                                       copyOrNull(node->getActualType()));
}

TypedefDecl*
InstantiatorImpl::visitTypedefDecl(TypedefDecl *node)
{
   return makeStmt<TypedefDecl>(node,
                                node->getAccess(),
                                string(node->getName()),
                                visitTypeRef(node->getOriginTy()));
}

AliasDecl*
InstantiatorImpl::visitAliasDecl(AliasDecl *node)
{
   return makeStmt<AliasDecl>(node, string(node->getName()),
                              cloneVector(node->getConstraints()),
                              visitStaticExpr(node->getAliasExpr()));
}

FieldDecl* InstantiatorImpl::visitFieldDecl(FieldDecl *node)
{
   return makeStmt<FieldDecl>(node, string(node->getName()),
                              visitTypeRef(node->getType()),
                              node->getAccess(), node->isStatic(),
                              node->isConst(),
                              copyOrNull(node->getDefaultVal()));
}

InitDecl*
InstantiatorImpl::visitInitDecl(InitDecl *node, bool skipBody)
{
   std::vector<FuncArgDecl*> args;
   if (!skipBody) {
      args = cloneVector(node->getArgs());
   }
   else {
      args = copyArgListAndFindVariadic(node->getArgs(),
                                        node->getTemplateParams());
   }

   string methodName = node->getName();
   if (kind == IK_Method)
      methodName += templateArgString;

   auto decl = makeStmt<InitDecl>(node, move(args),
                                  node->getAccess(),
                                  copyOrNull(node->getBody()),
                                  move(methodName));

   if (kind == IK_Record)
      decl->setMethodID(node->getMethodID());

   decl->setFunctionFlags(node->getFunctionFlags());

   auto info = new (SP.getContext()) InstantiationInfo<CallableDecl>;
   info->specializedTemplate = node;

   decl->setInstantiationInfo(info);

   return decl;
}

DeinitDecl* InstantiatorImpl::visitDeinitDecl(DeinitDecl *node,
                                            bool skipBody) {
   auto decl = makeStmt<DeinitDecl>(node,
                                   copyOrNull(node->getBody()));

   if (kind == IK_Record)
      decl->setMethodID(node->getMethodID());

   decl->setFunctionFlags(node->getFunctionFlags());

   auto info = new (SP.getContext()) InstantiationInfo<CallableDecl>;
   info->specializedTemplate = node;

   decl->setInstantiationInfo(info);

   return decl;
}

FunctionDecl*
InstantiatorImpl::visitFunctionDecl(FunctionDecl *F)
{
   auto args = copyArgListAndFindVariadic(F->getArgs(),
                                          F->getTemplateParams());

   string funcName = F->getNameWithoutFix();
   if (kind == IK_Function)
      funcName += templateArgString;

   auto func = makeStmt<FunctionDecl>(F, F->getAccess(), move(funcName),
                                      move(args),
                                      visitTypeRef(F->getReturnType()),
                                      cloneVector(F->getConstraints()),
                                      copyOrNull(F->getBody()),
                                      F->getOperator());

   func->setFunctionFlags(F->getFunctionFlags());

   VariadicTemplateArgs.clear();
   return func;
}

TypeRef* InstantiatorImpl::visitTypeRef(TypeRef *node)
{
   TypeRef* TR = nullptr;
   switch (node->getKind()) {
      case TypeRef::ObjectType: {
         auto &SubTypes = node->getNamespaceQual();
         Type *substitution = nullptr;

         TypeRef::NamespaceVec NSCopy;

         for (auto const& Ty : SubTypes) {
            llvm::StringRef name;
            if (auto Arg = hasTemplateArg(Ty.first)) {
               if (Arg->isType() && !Arg->isVariadic()) {
                  if (!Arg->getType()->isObjectType())
                     substitution = Arg->getType();
                  else
                     NSCopy.emplace_back(Arg->getType()->getClassName(),
                                         std::vector<TemplateArgExpr*>());

                  break;
               }
            }

            NSCopy.emplace_back(string(Ty.first), copyTAList(Ty.second));
         }

         if (substitution) {
            if (SubTypes.size() != 1)
               diag::err(err_generic_error)
                  << "could not substitute nested type"
                  << node << diag::term;

            TR = makeExpr<TypeRef>(node, QualType(substitution));
         }
         else {
            TR = makeExpr<TypeRef>(node, move(NSCopy));
         }

         break;
      }
      case TypeRef::FunctionType: {
         std::vector<pair<string, TypeRef*>> argTypes;
         for (const auto &arg : node->getContainedTypes()) {
            std::vector<Expression*> variadicTypes;
            copyVariadicExpr<Expression>(variadicTypes, arg.second);

            if (variadicTypes.empty())
               argTypes.emplace_back("", visitTypeRef(arg.second));
            else for (auto &var : variadicTypes)
               argTypes.emplace_back("", cast<TypeRef>(var));
         }

         TR = makeExpr<TypeRef>(node,
                                visitTypeRef(node->getReturnType()),
                                move(argTypes));
         break;
      }
      case TypeRef::TupleType: {
         std::vector<pair<string, TypeRef*>> cont;
         for (const auto &arg : node->getContainedTypes()) {
            std::vector<Expression*> variadicTypes;
            copyVariadicExpr<Expression>(variadicTypes, arg.second);

            if (variadicTypes.empty())
               cont.emplace_back("", visitTypeRef(arg.second));
            else for (auto &var : variadicTypes)
               cont.emplace_back("", cast<TypeRef>(var));
         }

         TR = makeExpr<TypeRef>(node, move(cont));
         break;
      }
      case TypeRef::ArrayType:
         TR = makeExpr<TypeRef>(node,
                                visitTypeRef(node->getElementType()),
                                visitStaticExpr(node->getArraySize()));
         break;
      case TypeRef::DeclTypeExpr:
         TR = makeExpr<TypeRef>(node, visitExpr(node->getDeclTypeExpr()));
         break;
      case TypeRef::Pointer:
         TR = makeExpr<TypeRef>(node, visitTypeRef(node->getSubject()),
                                TypeRef::Pointer);
         break;
      case TypeRef::Option:
         TR = makeExpr<TypeRef>(node, visitTypeRef(node->getSubject()),
                                TypeRef::Option);
         break;
      default:
         if (node->isResolved())
            TR = makeExpr<TypeRef>(node,
                                   SP.resolveDependencies(*node->getType(),
                                                          templateArgs));
         else
            TR = makeExpr<TypeRef>(node);

         break;
   }

   TR->isReference(node->isReference());
   TR->setGlobalLookup(node->isGlobalLookup());

   return TR;
}

Expression*
InstantiatorImpl::visitImplicitCastExpr(ImplicitCastExpr *node)
{
   return visitExpr(node->getTarget());
}

Expression*
InstantiatorImpl::visitLvalueToRvalue(LvalueToRvalue *node)
{
   return visitExpr(node->getTarget());
}

CompoundStmt*
InstantiatorImpl::visitCompoundStmt(CompoundStmt *node)
{
   auto Compound = makeStmt<CompoundStmt>(node,
                                          node->preservesScope());

   for (const auto &stmt : node->getStatements()) {
      Compound->addStatement(visitStmt(stmt));
   }

   return Compound;
}

std::vector<FuncArgDecl*>
InstantiatorImpl::copyArgListAndFindVariadic(
                     const std::vector<FuncArgDecl*> &argList,
                     llvm::ArrayRef<TemplateParamDecl*> params) {
   std::vector<FuncArgDecl*> args;
   for (const auto &arg : argList) {
      bool isVariadic = false;
      if (arg->isVariadicArgPackExpansion()) {
         auto &name = arg->getArgType()->getNamespaceQual().front().first;
         auto TA = hasTemplateArg(name);

         if (TA && TA->isVariadic()) {
            VariadicTemplateArgs.try_emplace(arg->getArgName(), TA);

            size_t i = 0;
            for (const auto &VA : TA->getVariadicArgs()) {
               string newName("__");
               newName += arg->getArgName();
               newName += std::to_string(i++);

               QualType argTy(VA.getType(),
                              arg->getArgType()->isReference(),
                              arg->isConst());

               auto typeref = makeExpr<TypeRef>(arg->getArgType(),
                                                argTy);

               args.push_back(
                  makeStmt<FuncArgDecl>(arg,
                                        move(newName),
                                        move(typeref),
                                        copyOrNull(arg->getDefaultVal()),
                                        false, arg->isConst()));
            }

            isVariadic = true;
         }
         else if (hasTemplateParam(name, params)) {
            VariadicTemplateArgs.try_emplace(arg->getArgName(), nullptr);
         }
      }

      if (!isVariadic)
         args.push_back(visitFuncArgDecl(arg));
   }

   return args;
}

void InstantiatorImpl::findVariadicArgs(const ArgListTy &argList,
                                        llvm::ArrayRef<TemplateParamDecl*> pars){
   for (const auto &arg : argList) {
      if (arg->isVariadicArgPackExpansion()) {
         auto &name = arg->getArgType()->getNamespaceQual().front().first;
         auto TA = hasTemplateArg(name);

         if (TA && TA->isVariadic()) {
            VariadicTemplateArgs.try_emplace(arg->getArgName(), TA);
         }
         else if (auto TP = hasTemplateParam(name, pars)) {
            VariadicTemplateArgs.try_emplace(arg->getArgName(), nullptr);
         }
      }
   }
}

InstantiatorImpl::ExprList
InstantiatorImpl::copyExprList(const ExprList &exprList)
{
   ExprList list;
   for (const auto &expr : exprList) {
      copyVariadicExpr(list, expr);
   }

   return list;
}

InstantiatorImpl::TypeList
InstantiatorImpl::copyTypeList(const TypeList &typeList)
{
   TypeList copy;
   for (const auto &Ty : typeList) {
      bool madeCopy = false;
      if (Ty->isVariadicArgPackExpansion()) {
         llvm::SmallVector<llvm::StringRef, 4> ContainedVariadics;
         getContainedVariadicTypes(ContainedVariadics, Ty);

         for (const auto &TypeName : ContainedVariadics) {
            auto Arg = hasTemplateArg(TypeName);
            if (Arg && Arg->isVariadic()) {
               auto Param = hasTemplateParam(TypeName);
               assert(Param && "no matching template parameter");

               madeCopy = true;

               CurrentVariadicTypeSubstitution.first =
                  Param->getName();

               for (const auto &VA : Arg->getVariadicArgs()) {
                  CurrentVariadicTypeSubstitution.second = &VA;

                  auto newTy = visitTypeRef(Ty);
                  newTy->setIsVariadicArgPackExpansion(false);

                  copy.emplace_back(std::move(newTy));
               }

               CurrentVariadicTypeSubstitution.first = "";
            }
         }
      }

      if (!madeCopy)
         copy.push_back(visitTypeRef(Ty));
   }

   return copy;
}

std::vector<TemplateArgExpr*>
InstantiatorImpl::copyTAList(const std::vector<TemplateArgExpr*> &list)
{
   std::vector<TemplateArgExpr*> copy;
   for (auto &TA : list) {
      if (!TA->isTypeName()) {
         copy.push_back(makeExpr<TemplateArgExpr>(
            TA, visitStaticExpr(TA->getExpr())));
         continue;
      }

      auto type = TA->getType();
      if (!type)
         continue;

      if (type->isSingularType()) {
         auto Name = type->getSingularTypeName();
         if (auto Arg = hasTemplateArg(Name)) {
            if (Arg->isValue()) {
               if (Arg->isVariadic() && type->isVariadicArgPackExpansion()) {
                  for (const auto &VA : Arg->getVariadicArgs())
                     copy.push_back(new (getContext()) TemplateArgExpr(
                        SP.makeStmt<StaticExpr>(Variant(VA.getValue()))));
               }
               else {
                  copy.push_back(new (getContext()) TemplateArgExpr(
                     SP.makeStmt<StaticExpr>(Variant(Arg->getValue()))));
               }

               continue;
            }
         }
      }

      if (!type->isVariadicArgPackExpansion()) {
         copy.emplace_back(new (getContext()) TemplateArgExpr(
            visitTypeRef(type)));
         continue;
      }

      auto newList = copyTypeList({ type });
      for (auto &el : newList)
         copy.push_back(new (getContext()) TemplateArgExpr(el));
   }

   return copy;
}

TypeRef::NamespaceVec
InstantiatorImpl::copyTAList(const TypeRef::NamespaceVec &list)
{
   TypeRef::NamespaceVec copy;
   for (auto &NS : list) {
      copy.emplace_back(NS.first, copyTAList(NS.second));
   }

   return copy;
}

FuncArgDecl*
InstantiatorImpl::visitFuncArgDecl(FuncArgDecl *node)
{
   return makeStmt<FuncArgDecl>(node,
                                string(node->getArgName()),
                                visitTypeRef(node->getArgType()),
                                copyOrNull(node->getDefaultVal()),
                                node->isVariadicArgPackExpansion(),
                                node->isConst(), node->isCstyleVararg());
}

#define CDOT_INSTANTIATE_SIMPLE(Name)                               \
Name* InstantiatorImpl::visit##Name(Name *node)                     \
{                                                                   \
   return makeStmt<Name>(node, *node);                              \
}

#define CDOT_INSTANTIATE_EXPR(Name)                                 \
Name* InstantiatorImpl::visit##Name(Name *node)                     \
{                                                                   \
   return makeExpr<Name>(node, *node);                              \
}

CDOT_INSTANTIATE_EXPR(IntegerLiteral)
CDOT_INSTANTIATE_EXPR(FPLiteral)
CDOT_INSTANTIATE_EXPR(BoolLiteral)
CDOT_INSTANTIATE_EXPR(CharLiteral)
CDOT_INSTANTIATE_EXPR(StringLiteral)

CDOT_INSTANTIATE_SIMPLE(NullStmt)
CDOT_INSTANTIATE_SIMPLE(LabelStmt)
CDOT_INSTANTIATE_SIMPLE(GotoStmt)
CDOT_INSTANTIATE_SIMPLE(BreakStmt)
CDOT_INSTANTIATE_SIMPLE(ContinueStmt)

#undef CDOT_INSTANTIATE_SIMPLE
#undef CDOT_INSTANTIATE_EXPR

DebugStmt*
InstantiatorImpl::visitDebugStmt(DebugStmt *node)
{
   return makeStmt<DebugStmt>(node, *node);
}

NoneLiteral*
InstantiatorImpl::visitNoneLiteral(NoneLiteral *node)
{
   return makeExpr<NoneLiteral>(node);
}

StringInterpolation*
InstantiatorImpl::visitStringInterpolation(StringInterpolation *node)
{
   return makeExpr<StringInterpolation>(node, cloneVector(node->getStrings()));
}

TupleLiteral*
InstantiatorImpl::visitTupleLiteral(TupleLiteral *node)
{
   std::vector<pair<string, Expression*>> elements;
   for (const auto &El : node->getElements()) {
      elements.emplace_back(El.first, visitExpr(El.second));
   }

   return makeExpr<TupleLiteral>(node, move(elements));
}

LocalVarDecl*
InstantiatorImpl::visitLocalVarDecl(LocalVarDecl *node)
{
   return makeStmt<LocalVarDecl>(node, node->getAccess(), node->isConst(),
                                 string(node->getName()),
                                 copyOrNull(node->getTypeRef()),
                                 copyOrNull(node->getValue()));
}

GlobalVarDecl*
InstantiatorImpl::visitGlobalVarDecl(GlobalVarDecl *node)
{
   return makeStmt<GlobalVarDecl>(node, node->getAccess(), node->isConst(),
                                  string(node->getName()),
                                  copyOrNull(node->getTypeRef()),
                                  copyOrNull(node->getValue()));
}

ast::LocalDestructuringDecl*
InstantiatorImpl::visitLocalDestructuringDecl(LocalDestructuringDecl *node)
{
   VarDecl **decls = new (SP.getContext()) VarDecl*[node->getNumDecls()];

   size_t i = 0;
   for (auto it = node->getDecls().begin(); it; ++it)
      decls[i] = cast<VarDecl>(visitStmt(*it));

   return makeStmt<LocalDestructuringDecl>(node, node->getAccess(),
                                           node->isConst(), decls,
                                           node->getNumDecls(),
                                           copyOrNull(node->getType()),
                                           copyOrNull(node->getValue()));
}

ast::GlobalDestructuringDecl*
InstantiatorImpl::visitGlobalDestructuringDecl(GlobalDestructuringDecl *node)
{
   VarDecl **decls = new (SP.getContext()) VarDecl*[node->getNumDecls()];

   size_t i = 0;
   for (auto it = node->getDecls().begin(); it; ++it)
      decls[i] = cast<VarDecl>(visitStmt(*it));

   return makeStmt<GlobalDestructuringDecl>(node, node->getAccess(),
                                            node->isConst(), decls,
                                            node->getNumDecls(),
                                            copyOrNull(node->getType()),
                                            copyOrNull(node->getValue()));
}

Expression*
InstantiatorImpl::visitBuiltinExpr(BuiltinExpr *node)
{
   llvm_unreachable("suck my peepee");
}

Expression*
InstantiatorImpl::visitIdentifierRefExpr(IdentifierRefExpr *node)
{
   string ident;
   if (auto Arg = hasTemplateArg(node->getIdent())) {
      if (Arg->isType()) {
         if (!node->getSubExpr()) {
            ident = Arg->getType()->toString(); //FIXME function types, array
            // types, etc
         }
         else if (Arg->getType()->isObjectType())
            ident = Arg->getType()->toString();
      }
      else {
         auto valTy = *hasTemplateParam(node->getIdent())->getValueType()
                                                         ->getType();

         Expression* literal;
         if (valTy->isIntegerType()) {
            switch (valTy->getBitwidth()) {
               case 1:
                  literal =
                     makeExpr<BoolLiteral>(node, getContext().getBoolTy(),
                                           Arg->getValue().getZExtValue() != 0);
               case 8:
                  literal =
                     makeExpr<CharLiteral>(node, getContext().getCharTy(),
                                           Arg->getValue().getChar());
               default: {
                  literal =
                     makeExpr<IntegerLiteral>(node, valTy,
                                              llvm::APSInt(Arg->getValue()
                                                              .getAPSInt()));
                  break;
               }
            }
         }
         else if (valTy->isFPType()) {
            literal = makeExpr<FPLiteral>(node, valTy,
                                          llvm::APFloat(Arg->getValue()
                                                           .getAPFloat()));
         }
         else if (valTy->isObjectType() && valTy->getClassName() == "String") {
            literal = makeExpr<StringLiteral>(
               node, string(Arg->getValue().getString()));
         }
         else {
            llvm_unreachable("bad variant kind!");
         }

         return literal;
      }
   }
   if (CurrentVariadicArgSubstitution.first.equals(node->getIdent()))
      ident = CurrentVariadicArgSubstitution.second;

   if (ident.empty())
      ident = node->getIdent();

   auto expr = makeExpr<IdentifierRefExpr>(node, move(ident));
   copyTemplateArgs(node, expr);

   return expr;
}

MemberRefExpr*
InstantiatorImpl::visitMemberRefExpr(MemberRefExpr *node)
{
   if (node->isTupleAccess()) {
      return makeExpr<MemberRefExpr>(node, node->getTupleIndex(),
                                     node->isPointerAccess());
   }

   auto expr = makeExpr<MemberRefExpr>(node, string(node->getIdent()),
                                       node->isPointerAccess());

   expr->setIsPointerAccess(node->isPointerAccess());
   expr->isTupleAccess(node->isTupleAccess());
   copyTemplateArgs(node, expr);

   return expr;
}

EnumCaseExpr*
InstantiatorImpl::visitEnumCaseExpr(EnumCaseExpr *node)
{
   return makeExpr<EnumCaseExpr>(node, string(node->getIdent()),
                                 copyExprList(node->getArgs()));
}

Expression* InstantiatorImpl::visitCallExpr(CallExpr *node)
{
   if (node->getKind() == CallKind::VariadicSizeof) {
      auto TA = hasTemplateArg(node->getTemplateArgs().front()->getType()
                               ->getSingularTypeName());


      if (TA) {
         if (!TA->isVariadic())
            err(err_generic_error)
               << "sizeof... requires variadic template argument, "
                  + TA->toString() + " given"
               << node;
         else {
            llvm::APInt Int(sizeof(size_t) * 8,
                            uint64_t(TA->getVariadicArgs().size()));

            return makeExpr<IntegerLiteral>(node, getContext().getUIntTy(),
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

   auto call = makeExpr<CallExpr>(node, copyExprList(node->getArgs()),
                                  move(ident));

   call->setBuiltinFnKind(node->getBuiltinFnKind());
   call->setIsPointerAccess(node->isPointerAccess());

   copyTemplateArgs(node, call);

   return call;
}

SubscriptExpr*
InstantiatorImpl::visitSubscriptExpr(SubscriptExpr *node)
{
   return makeExpr<SubscriptExpr>(node, cloneVector(node->getIndices()));
}

ReturnStmt*
InstantiatorImpl::visitReturnStmt(ReturnStmt *node)
{
   if (auto Val = node->getReturnValue()) {
      return makeStmt<ReturnStmt>(node, visitExpr(Val));
   }
   else {
      return makeStmt<ReturnStmt>(node);
   }
}

UnaryOperator*
InstantiatorImpl::visitUnaryOperator(UnaryOperator *node)
{
   return makeExpr<UnaryOperator>(node, string(node->getOp()),
                                  visitExpr(node->getTarget()),
                                  node->isPrefix() ? "prefix" : "postfix");
}

Expression*
InstantiatorImpl::visitBinaryOperator(BinaryOperator *node)
{
   if (auto Ident = dyn_cast<IdentifierRefExpr>(node->getRhs())) {
      if (Ident->getIdent() == "...") {
         llvm::SmallVector<Expression* , 4> exprs;
         expandVariadicArgument(exprs, node->getLhs());

         if (exprs.empty()) {
            err(err_generic_error)
               << "left hand side of fold expression must contain at least "
                  "one parameter pack"
               << node;

            goto end;
         }

         auto numExprs = exprs.size();
         if (numExprs == 1) {
            return exprs.front();
         }
         else {
            size_t i = 2;
            auto binOp = makeExpr<BinaryOperator>(node, string(node->getOp()),
                                                  exprs[0],
                                                  exprs[1]);

            while (i < numExprs) {
               binOp = makeExpr<BinaryOperator>(node, string(node->getOp()),
                                                binOp, exprs[i]);

               ++i;
            }

            return binOp;
         }
      }
   }

   end:
   return makeExpr<BinaryOperator>(node, string(node->getOp()),
                                   visitExpr(node->getLhs()),
                                   visitExpr(node->getRhs()));
}

TertiaryOperator*
InstantiatorImpl::visitTertiaryOperator(TertiaryOperator *node)
{
   return makeExpr<TertiaryOperator>(node, visitExpr(node->getCondition()),
                                     visitExpr(node->getLhs()),
                                     visitExpr(node->getRhs()));
}

Expression*
InstantiatorImpl::visitExprSequence(ExprSequence *node)
{
   if (node->getFragments().size() == 1
       && node->isParenthesized()
       && node->getFragments().front().getExpr()
              ->isVariadicArgPackExpansion()) {
      std::vector<Expression*> tupleElements;
      copyVariadicExpr(tupleElements,
                       node->getFragments().front().getExpr());

      std::vector<pair<string, Expression*>> namedEls;
      for (auto &el : tupleElements)
         namedEls.emplace_back("", move(el));

      return makeExpr<TupleLiteral>(node, move(namedEls));
   }

   std::vector<ExprSequence::SequenceElement> fragments;
   for (const auto &El : node->getFragments()) {
      switch (El.getKind()) {
         case ExprSequence::SequenceElement::EF_Operator:
            fragments.emplace_back(El.getOperatorKind(), El.getLoc());
            break;
         case ExprSequence::SequenceElement::EF_PossibleOperator: {
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
         case ExprSequence::SequenceElement::EF_Expression:
            fragments.emplace_back(visitExpr(El.getExpr()));
            break;
      }
   }

   return makeExpr<ExprSequence>(node, move(fragments),
                                 node->isParenthesized());
}

LambdaExpr*
InstantiatorImpl::visitLambdaExpr(LambdaExpr *node)
{
   return makeExpr<LambdaExpr>(node,
                               visitTypeRef(node->getReturnType()),
                               cloneVector(node->getArgs()),
                               visitStmt(node->getBody()));
}

DictionaryLiteral*
InstantiatorImpl::visitDictionaryLiteral(DictionaryLiteral *node)
{
   return makeExpr<DictionaryLiteral>(node, cloneVector(node->getKeys()),
                                      cloneVector(node->getValues()));
}

ArrayLiteral*
InstantiatorImpl::visitArrayLiteral(ArrayLiteral *node)
{
   return makeExpr<ArrayLiteral>(node, cloneVector(node->getValues()));
}

IfStmt* InstantiatorImpl::visitIfStmt(IfStmt *node)
{
   return makeStmt<IfStmt>(node,
                           visitExpr(node->getCondition()),
                           visitStmt(node->getIfBranch()),
                           copyOrNull(node->getElseBranch()));
}

WhileStmt* InstantiatorImpl::visitWhileStmt(WhileStmt *node)
{
   return makeStmt<WhileStmt>(node,
                              visitExpr(node->getCondition()),
                              visitStmt(node->getBody()),
                              node->isAtLeastOnce());
}

ForStmt* InstantiatorImpl::visitForStmt(ForStmt *node)
{
   return makeStmt<ForStmt>(node,
                            visitStmt(node->getInitialization()),
                            visitExpr(node->getTermination()),
                            visitStmt(node->getIncrement()),
                            visitStmt(node->getBody()));
}

ForInStmt* InstantiatorImpl::visitForInStmt(ForInStmt *node)
{
   return makeStmt<ForInStmt>(node,
                              visitLocalVarDecl(node->getDecl()),
                              visitExpr(node->getRangeExpr()),
                              visitStmt(node->getBody()));
}

MatchStmt*
InstantiatorImpl::visitMatchStmt(MatchStmt *node)
{
   return makeStmt<MatchStmt>(node,
                              visitExpr(node->getSwitchValue()),
                              cloneVector(node->getCases()));
}

CaseStmt* InstantiatorImpl::visitCaseStmt(CaseStmt *node)
{
   if (node->isDefault())
      return makeStmt<CaseStmt>(node,
                                copyOrNull(node->getBody()));

   return makeStmt<CaseStmt>(node,
                             copyOrNull(node->getPattern()),
                             copyOrNull(node->getBody()));
}

ExpressionPattern*
InstantiatorImpl::visitExpressionPattern(ExpressionPattern *node)
{
   return makeExpr<ExpressionPattern>(node, visitExpr(node->getExpr()));
}

IsPattern*
InstantiatorImpl::visitIsPattern(IsPattern *node)
{
   return makeExpr<IsPattern>(node, visitTypeRef(node->getIsType()));
}

CasePattern*
InstantiatorImpl::visitCasePattern(CasePattern *node)
{
   std::vector<CasePattern::Argument> args;
   for (const auto &arg : node->getArgs()) {
      if (arg.isExpr()) {
         args.emplace_back(visitExpr(arg.getExpr()), arg.getSourceLoc());
      }
      else {
         args.emplace_back(string(arg.getIdentifier()), arg.isConst(),
                           arg.getSourceLoc());
      }
   }

   return makeExpr<CasePattern>(node, string(node->getCaseName()), move(args));
}

TryStmt* InstantiatorImpl::visitTryStmt(TryStmt *node)
{
   std::vector<CatchBlock> catchBlocks;
   for (const auto &CB : node->getCatchBlocks()) {
      catchBlocks.emplace_back(visitLocalVarDecl(CB.varDecl),
                               visitStmt(CB.body));
   }

   return makeStmt<TryStmt>(node, visitStmt(node->getBody()),
                            move(catchBlocks),
                            copyOrNull(node->getFinallyBlock()));
}

ThrowStmt* InstantiatorImpl::visitThrowStmt(ThrowStmt *node)
{
   return makeStmt<ThrowStmt>(node,
                              visitExpr(node->getThrownVal()));
}

UsingStmt* InstantiatorImpl::visitUsingStmt(UsingStmt *node)
{
   return makeStmt<UsingStmt>(
      node,
      std::vector<string>(node->getDeclContextSpecifier()),
      std::vector<string>(node->getImportedItems()),
      node->isWildCardImport());
}

ModuleStmt* InstantiatorImpl::visitModuleStmt(ModuleStmt *node)
{
   return makeStmt<ModuleStmt>(
      node, std::vector<string>(node->getQualifiedModuleName()));
}

ImportStmt* InstantiatorImpl::visitImportStmt(ImportStmt *node)
{
   return makeStmt<ImportStmt>(
      node, std::vector<string>(node->getQualifiedModuleName()));
}

StaticExpr* InstantiatorImpl::visitStaticExpr(StaticExpr *node)
{
   return makeExpr<StaticExpr>(node, visitExpr(node->getExpr()));
}

ConstraintExpr* InstantiatorImpl::visitConstraintExpr(ConstraintExpr *node)
{
   if (node->getKind() == ConstraintExpr::Type)
      return makeExpr<ConstraintExpr>(node,
                                visitTypeRef(node->getTypeConstraint()));

   return makeExpr<ConstraintExpr>(node, node->getKind());
}

TraitsExpr*
InstantiatorImpl::visitTraitsExpr(TraitsExpr *node)
{
   std::vector<TraitsExpr::TraitsArgument> args;
   for (const auto &arg : node->getArgs()) {
      switch (arg.getKind()) {
         case TraitsExpr::TraitsArgument::Expr:
            args.emplace_back(visitExpr(arg.getExpr()));
            break;
         case TraitsExpr::TraitsArgument::Stmt:
            args.emplace_back(visitStmt(arg.getStmt()));
            break;
         case TraitsExpr::TraitsArgument::Type:
            args.emplace_back(visitTypeRef(arg.getType()));
            break;
         case TraitsExpr::TraitsArgument::String:
            args.emplace_back(string(arg.getStr()));
            break;
      }
   }

   return makeExpr<TraitsExpr>(node, node->getKind(), move(args));
}

StaticStmt*
InstantiatorImpl::visitStaticStmt(StaticStmt *node)
{
   switch (node->getTypeID()) {
      case AstNode::StaticForStmtID:
         return visitStaticForStmt(cast<StaticForStmt>(node));
      case AstNode::StaticIfStmtID:
         return visitStaticIfStmt(cast<StaticIfStmt>(node));
      case AstNode::StaticAssertStmtID:
         return visitStaticAssertStmt(cast<StaticAssertStmt>(node));
      default:
         llvm_unreachable("bad static stmt kind");
   }
}

StaticIfStmt*
InstantiatorImpl::visitStaticIfStmt(StaticIfStmt *node)
{
   return makeStmt<StaticIfStmt>(node,
                                 visitStaticExpr(node->getCondition()),
                                 visitStmt(node->getIfBranch()),
                                 copyOrNull(node->getElseBranch()));
}

StaticForStmt*
InstantiatorImpl::visitStaticForStmt(StaticForStmt *node)
{
   return makeStmt<StaticForStmt>(node,
                                  string(node->getElementName()),
                                  visitStaticExpr(node->getRange()),
                                  visitStmt(node->getBody()));
}

StaticAssertStmt*
InstantiatorImpl::visitStaticAssertStmt(StaticAssertStmt *node)
{
   return makeStmt<StaticAssertStmt>(node,
                                     visitStaticExpr(node->getExpr()),
                                     string(node->getMessage()));
}

StaticPrintStmt*
InstantiatorImpl::visitStaticPrintStmt(StaticPrintStmt *node)
{
   return makeStmt<StaticPrintStmt>(node,
                                    visitStaticExpr(node->getExpr()));
}

} // anonymous namespace

RecordDecl *
TemplateInstantiator::InstantiateRecord(SemaPass &SP,
                                        SourceLocation instantiatedFrom,
                                        RecordDecl *Template,
                                        TemplateArgList&& templateArgs,
                                        bool *isNew) {
   if (templateArgs.isStillDependent())
      return Template;

   assert(!Template->isInstantiation() && "only instantiate base template!");

   void *insertPos;
   if (auto R = SP.getContext().getRecordTemplateInstantiation(Template,
                                                               templateArgs,
                                                               insertPos)) {
      if (isNew) *isNew = false;
      return R;
   }

   InstantiatorImpl Instantiator(SP, templateArgs, Template);

   auto &pass = *SP.getDeclPass();
   DeclPass::DeclScopeRAII scopeRAII(pass, SP.getNearestDeclContext(Template));

   auto recordDecl = Instantiator.visitRecordDecl(Template);
   SP.updateParentMapForTemplateInstantiation(Template, recordDecl);

   pass.DeclareRecord(recordDecl);
   SP.getContext().insertRecordTemplateInstantiation(recordDecl, insertPos);

   if (auto Loc = Instantiator.getFailedConstraintLoc())
      diag::err(err_generic_error)
         << "constraint not satisfied"
         << Loc << diag::term;

   auto instInfo = new (SP.getContext()) InstantiationInfo<RecordDecl>;
   instInfo->specializedTemplate = Template;
   instInfo->templateArgs = move(templateArgs);
   instInfo->instantiatedFrom = instantiatedFrom;

   recordDecl->setInstantiationInfo(instInfo);

   if (SP.getStage() != SemaPass::Stage::Declaration) {
      SP.visitRecordInstantiation(recordDecl);
   }
   else {
      pass.visitScoped(recordDecl);
      SP.registerDelayedInstantiation(recordDecl);
   }

   if (isNew)
      *isNew = true;

   return recordDecl;
}

FunctionDecl*
TemplateInstantiator::InstantiateFunction(SemaPass &SP,
                                          SourceLocation instantiatedFrom,
                                          FunctionDecl *Template,
                                          TemplateArgList&& templateArgs,
                                          bool *isNew) {
   if (templateArgs.isStillDependent())
      return Template;

   void *insertPos;
   if (auto F = SP.getContext().getFunctionTemplateInstantiation(Template,
                                                                 templateArgs,
                                                                 insertPos)) {
      if (isNew) *isNew = false;
      return cast<FunctionDecl>(F);
   }

   InstantiatorImpl Instantiator(SP, templateArgs, Template);

   auto &pass = *SP.getDeclPass();
   DeclPass::DeclScopeRAII guard(pass, SP.getNearestDeclContext(Template));

   auto decl = Instantiator.visitFunctionDecl(Template);
   SP.updateParentMapForTemplateInstantiation(Template, decl);

   pass.DeclareFunction(decl);
   pass.visitScoped(decl);

   SP.getContext().insertFunctionTemplateInstantiation(decl, insertPos);

   auto instInfo = new (SP.getContext()) InstantiationInfo<CallableDecl>{
      instantiatedFrom, move(templateArgs), Template
   };

   decl->setInstantiationInfo(instInfo);

   SP.visitFunctionInstantiation(decl);

   if (isNew)
      *isNew = true;

   return decl;
}

MethodDecl*
TemplateInstantiator::InstantiateMethod(SemaPass &SP,
                                        SourceLocation instantiatedFrom,
                                        MethodDecl *Template,
                                        TemplateArgList&& templateArgs,
                                        bool *isNew,
                                        RecordDecl *R) {
   if (templateArgs.isStillDependent())
      return Template;

   void *insertPos;
   if (auto M = SP.getContext().getFunctionTemplateInstantiation(Template,
                                                                 templateArgs,
                                                                 insertPos)) {
      if (isNew) *isNew = false;
      return cast<MethodDecl>(M);
   }

   InstantiatorImpl Instantiator(SP, templateArgs, Template);

   if (!R)
      R = Template->getRecord();

   MethodDecl* decl;
   if (auto C = dyn_cast<InitDecl>(Template)) {
      decl = Instantiator.visitInitDecl(C);
   }
   else if (auto D = dyn_cast<DeinitDecl>(Template)) {
      decl = Instantiator.visitDeinitDecl(D);
   }
   else {
      decl = Instantiator.visitMethodDecl(Template);
   }

   SP.updateParentMapForTemplateInstantiation(Template, decl);

   auto &pass = *SP.getDeclPass();
   DeclPass::DeclScopeRAII scopeRAII(pass, SP.getNearestDeclContext(Template));

   pass.visit(decl);

   SP.getContext().insertFunctionTemplateInstantiation(decl, insertPos);

   auto instInfo = new (SP.getContext()) InstantiationInfo<CallableDecl>{
      instantiatedFrom, move(templateArgs), Template
   };

   decl->setInstantiationInfo(instInfo);
   SP.visitFunctionInstantiation(decl);

   if (isNew)
      *isNew = true;

   return decl;
}

MethodDecl*TemplateInstantiator::InstantiateProtocolDefaultImpl(
                                       SemaPass &SP,
                                       SourceLocation instantiatedFrom,
                                       RecordDecl *Rec,
                                       MethodDecl *M) {
   InstantiatorImpl Instantiator(SP, {});

   auto decl = Instantiator.visitMethodDecl(M);
   auto &pass = *SP.getDeclPass();

   DeclPass::DeclScopeRAII scopeRAII(pass, Rec);
   pass.visit(decl);

   auto instInfo = new (SP.getContext()) InstantiationInfo<CallableDecl>{
      instantiatedFrom, {}, M
   };

   decl->setInstantiationInfo(instInfo);
   Rec->addDecl(decl);

   return decl;
}

Statement*
TemplateInstantiator::InstantiateStatement(
                                    SemaPass &SP,
                                    SourceLocation instantiatedFrom,
                                    Statement* stmt,
                                    sema::TemplateArgList const& templateArgs) {
   InstantiatorImpl Instantiator(SP, templateArgs);
   return Instantiator.visitStmt(stmt);
}

Statement* TemplateInstantiator::InstantiateMethodBody(
                                    SemaPass &SP,
                                    SourceLocation instantiatedFrom,
                                    MethodDecl const* baseMethod,
                                    MethodDecl* newMethod) {
   InstantiatorImpl Instantiator(SP, newMethod->getRecord()
                                              ->getTemplateArgs());

//   Instantiator.findVariadicArgs(baseMethod->getArgs(),
//                                 baseMethod->getTemplateParams());

   return Instantiator.visitStmt(baseMethod->getBody());
}

StaticExpr*
TemplateInstantiator::InstantiateStaticExpr(SemaPass &SP,
                                            SourceLocation instantiatedFrom,
                                            StaticExpr* stmt,
                                            TemplateArgList const& templateArgs) {
   return cast<StaticExpr>(InstantiateStatement(SP, instantiatedFrom,
                                                stmt, templateArgs));
}

AliasDecl*
TemplateInstantiator::InstantiateAlias(SemaPass &SP,
                                       AliasDecl *Template,
                                       SourceLocation instantiatedFrom,
                                       TemplateArgList &&templateArgs) {
   void *insertPos;
   if (auto Inst = SP.getContext().getAliasTemplateInstantiation(Template,
                                                                 templateArgs,
                                                                 insertPos)) {
      return Inst;
   }

   InstantiatorImpl Instantiator(SP, templateArgs, Template);

   auto Inst = Instantiator.visitAliasDecl(Template);
   SP.updateParentMapForTemplateInstantiation(Template, Inst);

   Inst->setInstantiationInfo(new(SP.getContext()) InstantiationInfo<AliasDecl>(
      instantiatedFrom, move(templateArgs), Template
   ));

   SP.getContext().insertAliasTemplateInstantiation(Inst, insertPos);

   return Inst;
}

} // namespace cdot