//
// Created by Jonas Zell on 05.12.17.
//

#include "TemplateInstantiator.h"
#include "Template.h"

#include <sstream>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/ErrorHandling.h>

#include "../Declaration/DeclPass.h"

#include "../AggregateVisitor.h"

#include "Record/Class.h"
#include "Record/Union.h"

#include "../../SymbolTable.h"
#include "../../../Message/Diagnostics.h"
#include "../../../Variant/Type/PointerType.h"

#include "../StaticExpr/StaticExprEvaluator.h"

#include "../../../Variant/Type/PointerType.h"
#include "../../../Variant/Type/GenericType.h"
#include "../../../Variant/Type/IntegerType.h"

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

   InstantiatorImpl(InstantiationKind kind,
                    TemplateArgList const& templateArgs,
                    Callable const* C)
      : kind(kind), templateArgs(templateArgs), C(C),
        templateArgString(templateArgs.toString())
   {

   }

   InstantiatorImpl(TemplateArgList const& templateArgs)
      : kind(IK_AnyStmt), templateArgs(templateArgs), C(nullptr),
        templateArgString(templateArgs.toString())
   {

   }

   InstantiatorImpl(TemplateArgList const& templateArgs, Record const* R)
      : kind(IK_Record), templateArgs(templateArgs), R(R),
        templateArgString(templateArgs.toString())
   {

   }

   InstantiatorImpl(TemplateArgList const& templateArgs, Alias const* alias)
      : kind(IK_Alias), templateArgs(templateArgs), alias(alias),
        templateArgString(templateArgs.toString())
   {

   }

   std::shared_ptr<Expression> visitExpr(Expression *expr)
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

   std::shared_ptr<Expression>
   visitExpr(const std::shared_ptr<Expression> &expr)
   {
      return visitExpr(expr.get());
   }

   std::shared_ptr<Statement> visitStmt(Statement *stmt)
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

   std::shared_ptr<Statement>
   visitStmt(const std::shared_ptr<Statement> &stmt)
   {
      return visitStmt(stmt.get());
   }

   struct CommonRecordContents {
      AccessModifier am;
      string recordName;
      size_t namespaceLength;

      std::vector<std::shared_ptr<TypeRef>> conformsTo;

      std::vector<std::shared_ptr<FieldDecl>> fields;
      std::vector<std::shared_ptr<MethodDecl>> methods;
      std::vector<std::shared_ptr<TypedefDecl>> typedefs;
      std::vector<std::shared_ptr<PropDecl>> properties;
      std::vector<std::shared_ptr<AssociatedTypeDecl>> associatedTypes;

      std::vector<TemplateParameter> templateParams;
      std::vector<std::shared_ptr<StaticExpr>> constraints;

      std::vector<std::shared_ptr<RecordDecl>> innerDeclarations;
      std::vector<std::shared_ptr<StaticStmt>> staticStatements;
   };

   std::shared_ptr<CompoundStmt> visitCompoundStmt(CompoundStmt *node);

   std::shared_ptr<NamespaceDecl> visitNamespaceDecl(NamespaceDecl *node)
   {
      llvm_unreachable("should not be in a template!");
   }

   std::shared_ptr<UsingStmt> visitUsingStmt(UsingStmt *node);
   std::shared_ptr<ModuleStmt> visitModuleStmt(ModuleStmt *node);
   std::shared_ptr<ImportStmt> visitImportStmt(ImportStmt *node);

   std::shared_ptr<LocalVarDecl> visitLocalVarDecl(LocalVarDecl *node);
   std::shared_ptr<GlobalVarDecl> visitGlobalVarDecl(GlobalVarDecl *node);
   std::shared_ptr<FunctionDecl> visitFunctionDecl(FunctionDecl *node);

   std::shared_ptr<CallableDecl> visitCallableDecl(CallableDecl *node)
   {
      llvm_unreachable("call visitFunctionDecl or visitMethodDecl");
   }

   std::shared_ptr<DeclareStmt> visitDeclareStmt(DeclareStmt *node)
   {
      llvm_unreachable("should not be in a template!");
   }

   bool visitRecordDecl(CommonRecordContents &contents, RecordDecl *node);

   std::shared_ptr<RecordDecl> visitRecordDecl(RecordDecl *node)
   {
      std::shared_ptr<RecordDecl> res;
      switch (node->getTypeID()) {
         case AstNode::ClassDeclID:
            res = visitClassDecl(cast<ClassDecl>(node));
            break;
         case AstNode::EnumDeclID:
            res = visitEnumDecl(cast<EnumDecl>(node));
            break;
         case AstNode::UnionDeclID:
            res = visitUnionDecl(cast<UnionDecl>(node));
            break;
         case AstNode::ProtocolDeclID:
            res = visitProtocolDecl(cast<ProtocolDecl>(node));
            break;
         case AstNode::ExtensionDeclID:
            res = visitExtensionDecl(cast<ExtensionDecl>(node));
            break;
         default:
            llvm_unreachable("bad record decl kind");
      }

      return res;
   }

   std::shared_ptr<ClassDecl> visitClassDecl(ClassDecl *node);
   std::shared_ptr<ProtocolDecl> visitProtocolDecl(ProtocolDecl *node);
   std::shared_ptr<ExtensionDecl> visitExtensionDecl(ExtensionDecl *node);
   std::shared_ptr<EnumDecl> visitEnumDecl(EnumDecl *node);
   std::shared_ptr<UnionDecl> visitUnionDecl(UnionDecl *node);

   std::shared_ptr<EnumCaseDecl> visitEnumCaseDecl(EnumCaseDecl *node);

   std::shared_ptr<FieldDecl> visitFieldDecl(FieldDecl *node);
   std::shared_ptr<PropDecl> visitPropDecl(PropDecl *node);
   std::shared_ptr<AssociatedTypeDecl> visitAssociatedTypeDecl
      (AssociatedTypeDecl *node);

   std::shared_ptr<MethodDecl> visitMethodDecl(MethodDecl *node,
                                               bool skipBody = false);
   std::shared_ptr<ConstrDecl> visitConstrDecl(ConstrDecl *node,
                                               bool skipBody = false);
   std::shared_ptr<DestrDecl> visitDestrDecl(DestrDecl *node,
                                             bool skipBody = false);

   std::shared_ptr<FuncArgDecl> visitFuncArgDecl(FuncArgDecl *node);
   std::shared_ptr<TypedefDecl> visitTypedefDecl(TypedefDecl *node);
   std::shared_ptr<AliasDecl> visitAliasDecl(AliasDecl *node);

   std::shared_ptr<Expression>
   visitIdentifierRefExpr(IdentifierRefExpr *node);

   std::shared_ptr<Expression>
   visitNonTypeTemplateArgExpr(NonTypeTemplateArgExpr *node);

   std::shared_ptr<SubscriptExpr> visitSubscriptExpr(SubscriptExpr *node);
   std::shared_ptr<Expression> visitCallExpr(CallExpr *node);
   std::shared_ptr<MemberRefExpr> visitMemberRefExpr(MemberRefExpr *node);
   std::shared_ptr<EnumCaseExpr> visitEnumCaseExpr(EnumCaseExpr *node);

   std::shared_ptr<ForStmt> visitForStmt(ForStmt *node);
   std::shared_ptr<ForInStmt> visitForInStmt(ForInStmt *node);
   std::shared_ptr<WhileStmt> visitWhileStmt(WhileStmt *node);
   std::shared_ptr<IfStmt> visitIfStmt(IfStmt *node);
   std::shared_ptr<LabelStmt> visitLabelStmt(LabelStmt *node);
   std::shared_ptr<GotoStmt> visitGotoStmt(GotoStmt *node);

   std::shared_ptr<MatchStmt> visitMatchStmt(MatchStmt *node);
   std::shared_ptr<CaseStmt> visitCaseStmt(CaseStmt *node);

   std::shared_ptr<ExpressionPattern>
   visitExpressionPattern(ExpressionPattern *node);

   std::shared_ptr<IsPattern> visitIsPattern(IsPattern *node);
   std::shared_ptr<CasePattern> visitCasePattern(CasePattern *node);

   std::shared_ptr<ReturnStmt> visitReturnStmt(ReturnStmt *node);
   std::shared_ptr<BreakStmt> visitBreakStmt(BreakStmt *node);
   std::shared_ptr<ContinueStmt> visitContinueStmt(ContinueStmt *node);

   std::shared_ptr<IntegerLiteral> visitIntegerLiteral(IntegerLiteral *node);
   std::shared_ptr<FPLiteral> visitFPLiteral(FPLiteral *node);
   std::shared_ptr<BoolLiteral> visitBoolLiteral(BoolLiteral *node);
   std::shared_ptr<CharLiteral> visitCharLiteral(CharLiteral *node);

   std::shared_ptr<DictionaryLiteral>
   visitDictionaryLiteral(DictionaryLiteral *node);

   std::shared_ptr<ArrayLiteral>
   visitArrayLiteral(ArrayLiteral *node);

   std::shared_ptr<NoneLiteral> visitNoneLiteral(NoneLiteral *node);
   std::shared_ptr<StringLiteral> visitStringLiteral(StringLiteral *node);

   std::shared_ptr<StringInterpolation>
   visitStringInterpolation(StringInterpolation *node);

   std::shared_ptr<TupleLiteral> visitTupleLiteral(TupleLiteral *node);

   std::shared_ptr<Expression> visitExprSequence(ExprSequence *node);
   std::shared_ptr<Expression> visitBinaryOperator(BinaryOperator *node);
   std::shared_ptr<TertiaryOperator>
   visitTertiaryOperator(TertiaryOperator *node);
   std::shared_ptr<UnaryOperator> visitUnaryOperator(UnaryOperator *node);

   std::shared_ptr<StaticExpr> visitStaticExpr(StaticExpr *node);
   std::shared_ptr<ConstraintExpr> visitConstraintExpr(ConstraintExpr *node);
   std::shared_ptr<TraitsExpr> visitTraitsExpr(TraitsExpr *node);

   std::shared_ptr<StaticStmt> visitStaticStmt(StaticStmt *node);
   std::shared_ptr<StaticAssertStmt>
   visitStaticAssertStmt(StaticAssertStmt *node);

   std::shared_ptr<StaticPrintStmt>
   visitStaticPrintStmt(StaticPrintStmt *node);

   std::shared_ptr<StaticIfStmt> visitStaticIfStmt(StaticIfStmt *node);
   std::shared_ptr<StaticForStmt> visitStaticForStmt(StaticForStmt *node);

   std::shared_ptr<TryStmt> visitTryStmt(TryStmt *node);
   std::shared_ptr<ThrowStmt> visitThrowStmt(ThrowStmt *node);

   std::shared_ptr<LambdaExpr> visitLambdaExpr(LambdaExpr *node);

   std::shared_ptr<Expression>
   visitImplicitCastExpr(ImplicitCastExpr *node);

   std::shared_ptr<TypeRef> visitTypeRef(TypeRef *node);

   std::shared_ptr<Expression> visitLvalueToRvalue(LvalueToRvalue *node);

   std::shared_ptr<DebugStmt> visitDebugStmt(DebugStmt *node);
   std::shared_ptr<NullStmt> visitNullStmt(NullStmt *node);

   const string &getTemplateArgString() const
   {
      return templateArgString;
   }

   const SourceLocation &getFailedConstraintLoc() const
   {
      return failedConstraintLoc;
   }

   using ArgListTy = std::vector<std::shared_ptr<FuncArgDecl>>;

   void findVariadicArgs(const ArgListTy &argList,
                         llvm::ArrayRef<TemplateParameter> pars);

private:
   InstantiationKind kind;
   TemplateArgList const& templateArgs;

   union {
      Callable const* C;
      Record const* R;
      Alias const* alias;
   };

   llvm::StringMap<ResolvedTemplateArg const*> VariadicTemplateArgs;

   string templateArgString;

   llvm::SmallVector<TemplateParameter const*, 4> InnerParams;
   size_t InnerDecls = 0;

   std::pair<llvm::StringRef, llvm::StringRef> CurrentVariadicArgSubstitution;
   std::pair<llvm::StringRef, ResolvedTemplateArg const*>
      CurrentVariadicTypeSubstitution;

   SourceLocation failedConstraintLoc;

   static TemplateParameter const* hasTemplateParam(
                              const std::string &Name,
                              llvm::ArrayRef<TemplateParameter> params) {
      for (const auto &TP : params) {
         if (TP.genericTypeName == Name)
            return &TP;
      }

      return nullptr;
   }

   TemplateParameter const* hasTemplateParam(const std::string &Name) const
   {
      for (const auto &Inner : InnerParams)
         if (Name == Inner->genericTypeName)
            return nullptr;

      if (kind == IK_AnyStmt)
         return nullptr;

      if (kind == IK_Record) {
         return hasTemplateParam(Name, R->getTemplateParams());
      }
      else if (kind == IK_Alias) {
         return hasTemplateParam(Name, alias->getTemplateParams());
      }
      else {
         if (auto TP = hasTemplateParam(Name, C->getTemplateParams()))
            return TP;

         if (auto M = dyn_cast<Method>(C))
            return hasTemplateParam(Name, M->getOwningRecord()
                                           ->getTemplateParams());
      }

      return nullptr;
   }

   ResolvedTemplateArg const* hasTemplateArg(const std::string &Name) const
   {
      for (const auto &Inner : InnerParams)
         if (Name == Inner->genericTypeName)
            return nullptr;

      if (CurrentVariadicTypeSubstitution.first.equals(Name))
         return CurrentVariadicTypeSubstitution.second;

      auto TA = templateArgs.getNamedArg(Name);
      if (!TA || TA->isStillDependant())
         return nullptr;

      return TA;
   }

   template<class T, class ...Args>
   std::shared_ptr<T> makeStmt(Statement *node, Args&& ...args)
   {
      auto ret = std::shared_ptr<T>::make_shared(std::forward<Args>(args)...);

      auto loc = node->getSourceLoc();
      ret->setSourceLoc({ loc.getOffset(), loc.getSourceId() });
      ret->setExternKind(node->getExternKind());

      return ret;
   }

   template<class T, class ...Args>
   std::shared_ptr<T> makeExpr(Expression *node, Args&& ...args)
   {
      auto ret = std::shared_ptr<T>::make_shared(std::forward<Args>(args)...);
      ret->setSourceLoc(node->getSourceLoc());
      ret->setIsVariadicArgPackExpansion(node->isVariadicArgPackExpansion());

      if (auto M = node->getMemberExpr())
         ret->setMemberExpr(visitExpr(M.get()));

      return ret;
   }

   template<class T>
   std::vector<std::shared_ptr<T>>
   cloneVector(const std::vector<std::shared_ptr<T>> &vec)
   {
      std::vector<std::shared_ptr<T>> newVec;
      for (const auto &v : vec) {
         auto newStmt = visitStmt(v.get());
         assert(isa<T>(newStmt.get()));

         newVec.push_back(std::static_pointer_cast<T>(newStmt));
      }

      return newVec;
   }

   template<class T>
   std::shared_ptr<T> copyOrNull(const std::shared_ptr<T> &sp)
   {
      if (!sp)
         return nullptr;

      auto copy = visitStmt(sp.get());
      assert(isa<T>(copy.get()));

      return std::static_pointer_cast<T>(copy);
   }

   template<class T, class U>
   void copyTemplateArgs(const T& from, const U& to)
   {
      to->setTemplateArgs(copyTAList(from->getTemplateArgs()));
   }

   ArgListTy copyArgListAndFindVariadic(const ArgListTy &argList,
                                        llvm::ArrayRef<TemplateParameter> pars);

   using ExprList = std::vector<std::shared_ptr<Expression>>;
   ExprList copyExprList(const ExprList &exprList);

   using TypeList = std::vector<std::shared_ptr<TypeRef>>;
   TypeList copyTypeList(const TypeList &typeList);

   template<class T>
   void copyVariadicExpr(std::vector<std::shared_ptr<T>> &exprList,
                         std::shared_ptr<T> const& expr) {
      llvm::SmallVector<std::shared_ptr<Expression>, 4> exprs;
      bool hasContainedPacks = expandVariadicArgument(exprs, expr);

      if (hasContainedPacks) {
         for (auto &E : exprs)
            exprList.emplace_back(move(E));

         return;
      }

      std::vector<std::shared_ptr<T>> copy;
      llvm::SmallVector<llvm::StringRef, 4> ContainedVariadics;
      getContainedVariadicTypes(ContainedVariadics, expr.get());

      if (ContainedVariadics.empty()) {
         if (expr->isVariadicArgPackExpansion())
            err(err_generic_error)
               << "expression does not contain expandable parameter packs"
               << expr;

         return exprList.emplace_back(visitExpr(expr));
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
                       llvm::SmallVector<std::shared_ptr<Expression>, 4> &exprs,
                       std::shared_ptr<Expression> const& variadicExpr) {
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
                  if (Arg && Arg->isVariadic)
                     Tys.push_back(Ty->getSingularTypeName());
               }

               for (auto &NS : Ty->getNamespaceQual())
                  for (auto &TA : NS.second)
                     if (TA.isTypeName())
                        getContainedVariadicTypes(Tys, TA.getType().get());

               break;
            }
            case TypeRef::FunctionType:
            case TypeRef::TupleType:
               for (const auto &Cont : Ty->getContainedTypes())
                  getContainedVariadicTypes(Tys, Cont.second.get());

               if (Ty->getKind() == TypeRef::FunctionType)
                  getContainedVariadicTypes(Tys, Ty->getReturnType().get());

               break;
            default:
               break;
         }

         return;
      }
      else if (auto Ident = dyn_cast<IdentifierRefExpr>(expr)) {
         for (const auto &TA : Ident->getTemplateArgs())
            if (TA.isTypeName())
               getContainedVariadicTypes(Tys, TA.getType().get());

         if (auto P = hasTemplateParam(Ident->getIdent()))
            if (P->isVariadic)
               Tys.push_back(Ident->getIdent());
      }
      else if (auto RefExpr = dyn_cast<MemberRefExpr>(expr)) {
         for (const auto &TA : RefExpr->getTemplateArgs())
            if (TA.isTypeName())
               getContainedVariadicTypes(Tys, TA.getType().get());

         if (auto P = hasTemplateParam(RefExpr->getIdent()))
            if (P->isVariadic)
               Tys.push_back(RefExpr->getIdent());
      }
      else if (auto Call = dyn_cast<CallExpr>(expr)) {
         for (const auto &TA : Call->getTemplateArgs())
            if (TA.isTypeName())
               getContainedVariadicTypes(Tys, TA.getType().get());

         if (auto P = hasTemplateParam(Call->getIdent()))
            if (P->isVariadic)
               Tys.push_back(Call->getIdent());
      }
      else if (auto Seq = dyn_cast<ExprSequence>(expr)) {
         for (const auto &F : Seq->getFragments()) {
            switch (F.getKind()) {
               case ExprSequence::SequenceElement::EF_Expression:
                  getContainedVariadicTypes(Tys, F.getExpr().get());
                  break;
               case ExprSequence::SequenceElement::EF_PossibleOperator:
                  if (auto P = hasTemplateParam(F.getOp()))
                     if (P->isVariadic)
                        Tys.push_back(F.getOp());

                  break;
               default:
                  break;
            }
         }
      }

      if (auto MemExpr = expr->getMemberExpr())
         getContainedVariadicTypes(Tys, MemExpr.get());
   }

   void getContainedParameterPacks(llvm::SmallVector<llvm::StringRef, 4> &Packs,
                                   std::shared_ptr<Expression> const& expr) {
      auto Predicate = [this](IdentifierRefExpr *node) {
         auto it = VariadicTemplateArgs.find(node->getIdent());
         return it != VariadicTemplateArgs.end();
      };

      auto Visitor = makeAggrVisitor<IdentifierRefExpr>(expr, Predicate);
      for (const auto &node : Visitor.getFoundNodes())
         Packs.push_back(node->getIdent());
   }

   std::vector<TemplateArg> copyTAList(const std::vector<TemplateArg>& list);
   TypeRef::NamespaceVec copyTAList(const TypeRef::NamespaceVec& list);
};

bool InstantiatorImpl::visitRecordDecl(CommonRecordContents &contents,
                                       RecordDecl *node) {
//   StaticExprEvaluator Eval(SP, nullptr, nullptr, {}, templateArgs);
//   for (const auto &Const : node->getConstraints()) {
//      auto copy = visitStaticExpr(Const.get());
//      auto res = Eval.evaluate(copy.get());
//      assert(res.val.isInt());
//
//      if (!res.val.intVal) {
//         failedConstraintLoc = copy->getSourceLoc();
//         return false;
//      }
//   }

   contents.am = node->getAm();
   contents.namespaceLength = node->getNamespaceLength();

   contents.recordName = node->getRecordName();
   if (!InnerDecls)
      contents.recordName += templateArgString;

   contents.conformsTo = copyTypeList(node->getConformsTo());

   for (const auto &Prop : node->getProperties()) {
      contents.properties.push_back(visitPropDecl(Prop.get()));
   }

   for (const auto &F : node->getFields()) {
      contents.fields.push_back(visitFieldDecl(F.get()));
   }

   for (const auto &AssTy : node->getAssociatedTypes()) {
      contents.associatedTypes.push_back(visitAssociatedTypeDecl(AssTy.get()));
   }

   for (const auto &M : node->getMethods()) {
      contents.methods.push_back(
         visitMethodDecl(M.get(), !M->getMethod()->isProtocolMethod()));
   }

   for (const auto &TD : node->getTypedefs()) {
      contents.typedefs.push_back(visitTypedefDecl(TD.get()));
   }

   for (const auto &Inner : node->getInnerDeclarations()) {
      auto prevSize = InnerParams.size();
      for (const auto &TP : Inner->getTemplateParams())
         InnerParams.push_back(&TP);

      ++InnerDecls;

      contents.innerDeclarations.push_back(visitRecordDecl(Inner.get()));

      InnerParams.resize(prevSize);
      --InnerDecls;
   }

   for (const auto &Static : node->getStaticStatements()) {
      contents.staticStatements.push_back(visitStaticStmt(Static.get()));
   }

   auto const& params = node->getRecord()
                        ? node->getRecord()->getTemplateParams()
                        : node->getTemplateParams();

   for (const auto &TP : params) {
      if (!hasTemplateArg(TP.genericTypeName))
         contents.templateParams.push_back(TP);
   }

   return true;
}

std::shared_ptr<ClassDecl> InstantiatorImpl::visitClassDecl(ClassDecl *node)
{
   CommonRecordContents contents;
   auto isValid = visitRecordDecl(contents, node);

   if (!isValid)
      return nullptr;

   return makeStmt<ClassDecl>(node,
                              move(contents.recordName),
                              move(contents.fields),
                              move(contents.methods),
                              cloneVector(node->getConstructors()),
                              move(contents.typedefs),
                              move(contents.properties),
                              move(contents.associatedTypes),
                              move(contents.templateParams),
                              move(contents.constraints),
                              contents.am,
                              node->isAbstract(),
                              node->isStruct(),
                              copyOrNull(node->getParentClass()),
                              move(contents.conformsTo),
                              copyOrNull(node->getDestructor()),
                              move(contents.innerDeclarations),
                              move(contents.staticStatements));
}

std::shared_ptr<EnumDecl> InstantiatorImpl::visitEnumDecl(EnumDecl *node)
{
   CommonRecordContents contents;
   auto isValid = visitRecordDecl(contents, node);

   if (!isValid)
      return nullptr;

   return makeStmt<EnumDecl>(node, contents.am,
                             move(contents.recordName),
                             copyOrNull(node->getRawType()),
                             move(contents.fields),
                             move(contents.methods), move(contents.properties),
                             move(contents.associatedTypes),
                             move(contents.templateParams),
                             move(contents.constraints),
                             move(contents.conformsTo),
                             cloneVector(node->getCases()),
                             move(contents.innerDeclarations),
                             move(contents.staticStatements));
}

std::shared_ptr<ast::EnumCaseDecl>
InstantiatorImpl::visitEnumCaseDecl(EnumCaseDecl *node)
{
   EnumCaseDecl::AssociatedTypeVec associatedTypes;
   for (const auto &Ty : node->getAssociatedTypes()) {
      associatedTypes.emplace_back(Ty.first, visitTypeRef(Ty.second.get()));
   }

   return makeStmt<EnumCaseDecl>(node,
                                 string(node->getCaseName()),
                                 copyOrNull(node->getRawVal()),
                                 move(associatedTypes));
}

std::shared_ptr<UnionDecl> InstantiatorImpl::visitUnionDecl(UnionDecl *node)
{
   CommonRecordContents contents;
   auto isValid = visitRecordDecl(contents, node);

   if (!isValid)
      return nullptr;

   UnionDecl::UnionTypes typeMap;
   for (const auto &Ty : node->getContainedTypes()) {
      typeMap.emplace(Ty.first, visitTypeRef(Ty.second.get()));
   }

   return makeStmt<UnionDecl>(node,
                              move(contents.recordName), move(typeMap),
                              node->isConst(), move(contents.fields),
                              move(contents.methods),
                              move(contents.typedefs),
                              move(contents.properties),
                              move(contents.associatedTypes),
                              move(contents.templateParams),
                              move(contents.constraints),
                              move(contents.innerDeclarations),
                              move(contents.staticStatements));
}

std::shared_ptr<ProtocolDecl>
InstantiatorImpl::visitProtocolDecl(ProtocolDecl *node)
{
   CommonRecordContents c;
   auto isValid = visitRecordDecl(c, node);

   if (!isValid)
      return nullptr;

   return makeStmt<ProtocolDecl>(node, c.am, move(c.recordName),
                                 move(c.conformsTo), move(c.methods),
                                 move(c.typedefs), move(c.properties),
                                 move(c.associatedTypes),
                                 cloneVector(node->getConstructors()),
                                 move(c.templateParams),
                                 move(c.constraints),
                                 move(c.innerDeclarations),
                                 move(c.staticStatements));
}

std::shared_ptr<ast::ExtensionDecl>
InstantiatorImpl::visitExtensionDecl(ExtensionDecl *node)
{
   CommonRecordContents c;
   auto isValid = visitRecordDecl(c, node);

   if (!isValid)
      return nullptr;

   return makeStmt<ExtensionDecl>(node, c.am,
                                  move(c.recordName),
                                  move(c.conformsTo),
                                  move(c.fields),
                                  move(c.methods),
                                  move(c.typedefs), move(c.properties),
                                  move(c.associatedTypes),
                                  move(c.templateParams),
                                  move(c.constraints),
                                  cloneVector(node->getInitializers()),
                                  move(c.innerDeclarations),
                                  move(c.staticStatements));
}

std::shared_ptr<PropDecl> InstantiatorImpl::visitPropDecl(PropDecl *node)
{
   auto prop = node->getProp();
   return makeStmt<PropDecl>(node,
                             string(prop->getName()),
                             visitTypeRef(node->getType().get()),
                             node->getAccess(), node->isStatic(),
                             node->hasDefinition(),
                             node->hasGetter(), node->hasSetter(),
                             copyOrNull(node->getGetterBody()),
                             copyOrNull(node->getSetterBody()),
                             string(prop->getNewValName()));
}

std::shared_ptr<MethodDecl>
InstantiatorImpl::visitMethodDecl(MethodDecl *node, bool skipBody)
{
   skipBody &= InnerDecls == 0;

   std::vector<std::shared_ptr<FuncArgDecl>> args =
      copyArgListAndFindVariadic(node->getArgs(),
                                 node->getMethod()
                                     ->getTemplateParams());

   std::vector<TemplateParameter> templateParams;

   // node might never have been declared if it's in a static_if
   if (node->getCallable()) {
      for (const auto &TP : node->getMethod()->getTemplateParams()) {
         if (!hasTemplateArg(TP.genericTypeName))
            templateParams.push_back(TP);
      }
   }
   else {
      for (const auto &TP : node->getTemplateParams()) {
         if (!hasTemplateArg(TP.genericTypeName))
            templateParams.push_back(TP);
      }
   }

   string methodName = node->getName();
   if (kind == IK_Method)
      methodName += templateArgString;

   MethodDecl::SharedPtr decl;
   if (node->isIsAlias()) {
      decl = makeStmt<MethodDecl>(node,
                                  string(node->getAlias()),
                                  move(methodName), move(args));
   }
   else if (!node->getBody()) {
      decl = makeStmt<MethodDecl>(node,
                                  move(methodName),
                                  visitTypeRef(node->getReturnType().get()),
                                  move(args),
                                  move(templateParams),
                                  cloneVector(node->getConstraints()),
                                  node->getAm(), node->isIsStatic());
   }
   else {
      std::shared_ptr<CompoundStmt> body = nullptr;
      if (!skipBody)
         body = visitCompoundStmt(node->getBody().get());

      decl = makeStmt<MethodDecl>(node,
                                  move(methodName),
                                  visitTypeRef(node->getReturnType().get()),
                                  move(args), move(templateParams),
                                  cloneVector(node->getConstraints()),
                                  move(body),
                                  node->getOperator(),
                                  node->isIsCastOp_(),
                                  node->getAm(), node->isIsStatic());
   }

   decl->setIsStatic(node->isIsStatic());
   decl->setIsMutating_(node->isIsMutating_());
   decl->setInnerDecls(cloneVector(node->getInnerDecls()));
   decl->setSpecializedTemplate(node->getMethod());

   if (auto M = node->getMethod())
      if (kind == IK_Record)
         decl->setMethodID(M->getMethodID());

   VariadicTemplateArgs.clear();

   return decl;
}

std::shared_ptr<ast::AssociatedTypeDecl>
InstantiatorImpl::visitAssociatedTypeDecl(AssociatedTypeDecl *node)
{
   return makeStmt<AssociatedTypeDecl>(node,
                                       string(node->getProtocolSpecifier()),
                                       string(node->getName()),
                                       cloneVector(node->getConstraints()),
                                       copyOrNull(node->getActualType()));
}

std::shared_ptr<TypedefDecl>
InstantiatorImpl::visitTypedefDecl(TypedefDecl *node)
{
   auto td = node->getTypedef();

   std::vector<TemplateParameter> templateParams;

   if (td) {
      for (const auto &TP : td->templateParams) {
         if (!hasTemplateArg(TP.genericTypeName))
            templateParams.push_back(TP);
      }
   }
   else {
      for (const auto &TP : node->getTemplateParams()) {
         if (!hasTemplateArg(TP.genericTypeName))
            templateParams.push_back(TP);
      }
   }

   return makeStmt<TypedefDecl>(node,
                                node->getAccess(),
                                string(node->getAlias()),
                                visitTypeRef(node->getOrigin().get()),
                                move(templateParams));
}

std::shared_ptr<ast::AliasDecl>
InstantiatorImpl::visitAliasDecl(AliasDecl *node)
{
   auto alias = node->getAlias();
   std::vector<TemplateParameter> templateParams;

   for (const auto &TP : alias->getTemplateParams()) {
      if (!hasTemplateArg(TP.genericTypeName))
         templateParams.push_back(TP);
   }

   return makeStmt<AliasDecl>(node, string(node->getName()),
                              move(templateParams),
                              cloneVector(alias->getConstraints()),
                              visitStaticExpr(alias->getAliasExpr().get()));
}

std::shared_ptr<FieldDecl> InstantiatorImpl::visitFieldDecl(FieldDecl *node)
{
   return makeStmt<FieldDecl>(node, string(node->getName()),
                              visitTypeRef(node->getType().get()),
                              node->getAccess(), node->isStatic(),
                              node->isConst(),
                              copyOrNull(node->getDefaultVal()));
}

std::shared_ptr<ConstrDecl>
InstantiatorImpl::visitConstrDecl(ConstrDecl *node, bool skipBody)
{
   std::vector<std::shared_ptr<FuncArgDecl>> args;
   if (!skipBody) {
      args = cloneVector(node->getArgs());
   }
   else {
      args = copyArgListAndFindVariadic(node->getArgs(),
                                        node->getMethod()
                                            ->getTemplateParams());
   }

   std::vector<TemplateParameter> templateParams;
   for (size_t i = node->getRecord()->getTemplateParams().size();
        i < node->getMethod()->getTemplateParams().size(); ++i) {
      auto &TP = node->getMethod()->getTemplateParams()[i];
      if (!hasTemplateArg(TP.genericTypeName))
         templateParams.push_back(TP);
   }

   string methodName = "init";
   if (kind == IK_Method)
      methodName += templateArgString;

   auto decl = makeStmt<ConstrDecl>(node, move(args),
                                    node->getAm(),
                                    move(templateParams),
                                    copyOrNull(node->getBody()),
                                    move(methodName));

   if (auto M = node->getMethod())
      if (kind == IK_Record)
         decl->setMethodID(M->getMethodID());

   decl->setSpecializedTemplate(node->getMethod());

   return decl;
}

std::shared_ptr<DestrDecl> InstantiatorImpl::visitDestrDecl(DestrDecl *node,
                                                            bool skipBody) {
   auto decl = makeStmt<DestrDecl>(node,
                                   copyOrNull(node->getBody()));

   if (auto M = node->getMethod())
      decl->setMethodID(M->getMethodID());

   decl->setSpecializedTemplate(node->getMethod());

   return decl;
}

std::shared_ptr<ast::FunctionDecl>
InstantiatorImpl::visitFunctionDecl(FunctionDecl *node)
{
   auto args = copyArgListAndFindVariadic(node->getArgs(),
                                          node->getCallable()
                                              ->getTemplateParams());
   auto F = node->getCallable();

   std::vector<TemplateParameter> templateParams;
   for (const auto &TP : F->getTemplateParams()) {
      if (!hasTemplateArg(TP.genericTypeName))
         templateParams.push_back(TP);
   }

   string funcName = F->getNameWithoutFix();
   if (kind == IK_Function)
      funcName += templateArgString;

   auto func = makeStmt<FunctionDecl>(node,
                                      node->getAm(), move(funcName),
                                      move(args),
                                      visitTypeRef(node->getReturnType().get()),
                                      move(templateParams),
                                      cloneVector(node->getConstraints()),
                                      copyOrNull(node->getBody()),
                                      node->getOperator());

   VariadicTemplateArgs.clear();
   return func;
}

std::shared_ptr<TypeRef> InstantiatorImpl::visitTypeRef(TypeRef *node)
{
   TypeRef::SharedPtr TR = nullptr;
   switch (node->getKind()) {
      case TypeRef::ObjectType: {
         auto &SubTypes = node->getNamespaceQual();
         Type *substitution = nullptr;

         TypeRef::NamespaceVec NSCopy;

         for (auto const& Ty : SubTypes) {
            llvm::StringRef name;
            if (auto Arg = hasTemplateArg(Ty.first)) {
               if (Arg->isType() && !Arg->isVariadic()) {
                  if (!Arg->getType()->isObjectTy())
                     substitution = Arg->getType();
                  else
                     NSCopy.emplace_back(Arg->getType()->getClassName(),
                                         std::vector<TemplateArg>());

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
         std::vector<pair<string, TypeRef::SharedPtr>> argTypes;
         for (const auto &arg : node->getContainedTypes()) {
            std::vector<std::shared_ptr<Expression>> variadicTypes;
            copyVariadicExpr<Expression>(variadicTypes, arg.second);

            if (variadicTypes.empty())
               argTypes.emplace_back("", visitTypeRef(arg.second.get()));
            else for (auto &var : variadicTypes)
               argTypes.emplace_back("",
                                     std::static_pointer_cast<TypeRef>(var));
         }

         TR = makeExpr<TypeRef>(node,
                                visitTypeRef(node->getReturnType().get()),
                                move(argTypes));
         break;
      }
      case TypeRef::TupleType: {
         std::vector<pair<string, TypeRef::SharedPtr>> cont;
         for (const auto &arg : node->getContainedTypes()) {
            std::vector<std::shared_ptr<Expression>> variadicTypes;
            copyVariadicExpr<Expression>(variadicTypes, arg.second);

            if (variadicTypes.empty())
               cont.emplace_back("", visitTypeRef(arg.second.get()));
            else for (auto &var : variadicTypes)
               cont.emplace_back("", std::static_pointer_cast<TypeRef>(var));
         }

         TR = makeExpr<TypeRef>(node, move(cont));
         break;
      }
      case TypeRef::ArrayType:
         TR = makeExpr<TypeRef>(node,
                                visitTypeRef(node->getElementType().get()),
                                visitStaticExpr(node->getArraySize().get()));
         break;
      case TypeRef::DeclTypeExpr:
         TR = makeExpr<TypeRef>(node, visitExpr(node->getDeclTypeExpr()));
         break;
      case TypeRef::Pointer:
         TR = makeExpr<TypeRef>(node, visitTypeRef(node->getSubject().get()),
                                TypeRef::Pointer);
         break;
      case TypeRef::Option:
         TR = makeExpr<TypeRef>(node, visitTypeRef(node->getSubject().get()),
                                TypeRef::Option);
         break;
      default:
         if (node->isResolved())
            TR = makeExpr<TypeRef>(node, node->getType());
         else
            TR = makeExpr<TypeRef>(node);

         break;
   }

   TR->isReference(node->isReference());

   return TR;
}

std::shared_ptr<ast::Expression>
InstantiatorImpl::visitImplicitCastExpr(ImplicitCastExpr *node)
{
   return visitExpr(node->getTarget());
}

std::shared_ptr<ast::Expression>
InstantiatorImpl::visitLvalueToRvalue(LvalueToRvalue *node)
{
   return visitExpr(node->getTarget());
}

std::shared_ptr<CompoundStmt>
InstantiatorImpl::visitCompoundStmt(CompoundStmt *node)
{
   auto Compound = makeStmt<CompoundStmt>(node,
                                          node->preservesScope());

   for (const auto &stmt : node->getStatements()) {
      Compound->addStatement(visitStmt(stmt.get()));
   }

   return Compound;
}

std::vector<std::shared_ptr<FuncArgDecl>>
InstantiatorImpl::copyArgListAndFindVariadic(
                     const std::vector<std::shared_ptr<FuncArgDecl>> &argList,
                     llvm::ArrayRef<TemplateParameter> params) {
   std::vector<std::shared_ptr<FuncArgDecl>> args;
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

               auto typeref = makeExpr<TypeRef>(arg->getArgType().get(),
                                                argTy);

               args.push_back(
                  makeStmt<FuncArgDecl>(arg.get(),
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
         args.push_back(visitFuncArgDecl(arg.get()));
   }

   return args;
}

void InstantiatorImpl::findVariadicArgs(const ArgListTy &argList,
                                        llvm::ArrayRef<TemplateParameter> pars){
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
         getContainedVariadicTypes(ContainedVariadics, Ty.get());

         for (const auto &TypeName : ContainedVariadics) {
            auto Arg = hasTemplateArg(TypeName);
            if (Arg && Arg->isVariadic()) {
               auto Param = hasTemplateParam(TypeName);
               assert(Param && "no matching template parameter");

               madeCopy = true;

               CurrentVariadicTypeSubstitution.first =
                  Param->getGenericTypeName();

               for (const auto &VA : Arg->getVariadicArgs()) {
                  CurrentVariadicTypeSubstitution.second = &VA;

                  auto newTy = visitTypeRef(Ty.get());
                  newTy->setIsVariadicArgPackExpansion(false);

                  copy.emplace_back(std::move(newTy));
               }

               CurrentVariadicTypeSubstitution.first = "";
            }
         }
      }

      if (!madeCopy)
         copy.push_back(visitTypeRef(Ty.get()));
   }

   return copy;
}

std::vector<TemplateArg>
InstantiatorImpl::copyTAList(const std::vector<TemplateArg> &list)
{
   std::vector<TemplateArg> copy;
   for (auto &TA : list) {
      if (!TA.isTypeName()) {
         copy.emplace_back(visitStaticExpr(TA.getStaticExpr().get()));
         continue;
      }

      auto &type = TA.getType();
      if (!type)
         continue;

      if (type->isSingularType()) {
         auto Name = type->getSingularTypeName();
         if (auto Arg = hasTemplateArg(Name)) {
            if (Arg->isValue()) {
               if (Arg->isVariadic() && type->isVariadicArgPackExpansion()) {
                  for (const auto &VA : Arg->getVariadicArgs())
                     copy.emplace_back(
                        std::make_shared<StaticExpr>(Variant(VA.getValue())));
               }
               else {
                  copy.emplace_back(
                     std::make_shared<StaticExpr>(Variant(Arg->getValue())));
               }

               continue;
            }
         }
      }

      if (!type->isVariadicArgPackExpansion()) {
         copy.emplace_back(visitTypeRef(type.get()));
         continue;
      }

      auto newList = copyTypeList({ type });
      for (auto &el : newList)
         copy.emplace_back(move(el));
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

std::shared_ptr<FuncArgDecl>
InstantiatorImpl::visitFuncArgDecl(FuncArgDecl *node)
{
   return makeStmt<FuncArgDecl>(node,
                                string(node->getArgName()),
                                visitTypeRef(node->getArgType().get()),
                                copyOrNull(node->getDefaultVal()),
                                node->isVariadicArgPackExpansion(),
                                node->isConst());
}

#define CDOT_INSTANTIATE_SIMPLE(Name)                               \
std::shared_ptr<Name> InstantiatorImpl::visit##Name(Name *node)     \
{                                                                   \
   return makeStmt<Name>(node, *node);                              \
}

#define CDOT_INSTANTIATE_EXPR(Name)                                 \
std::shared_ptr<Name> InstantiatorImpl::visit##Name(Name *node)     \
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

std::shared_ptr<ast::DebugStmt>
InstantiatorImpl::visitDebugStmt(DebugStmt *node)
{
   return makeStmt<DebugStmt>(node, *node);
}

std::shared_ptr<NoneLiteral>
InstantiatorImpl::visitNoneLiteral(NoneLiteral *node)
{
   return makeExpr<NoneLiteral>(node);
}

std::shared_ptr<StringInterpolation>
InstantiatorImpl::visitStringInterpolation(StringInterpolation *node)
{
   return makeExpr<StringInterpolation>(node, cloneVector(node->getStrings()));
}

std::shared_ptr<TupleLiteral>
InstantiatorImpl::visitTupleLiteral(TupleLiteral *node)
{
   std::vector<pair<string, Expression::SharedPtr>> elements;
   for (const auto &El : node->getElements()) {
      elements.emplace_back(El.first, visitExpr(El.second));
   }

   return makeExpr<TupleLiteral>(node, move(elements));
}

std::shared_ptr<LocalVarDecl>
InstantiatorImpl::visitLocalVarDecl(LocalVarDecl *node)
{
   return makeStmt<LocalVarDecl>(node,
                                 std::vector<string>(node->getIdentifiers()),
                                 visitTypeRef(node->getType().get()),
                                 node->isConst(), copyOrNull(node->getValue()));
}

std::shared_ptr<ast::GlobalVarDecl>
InstantiatorImpl::visitGlobalVarDecl(GlobalVarDecl *node)
{
   return makeStmt<GlobalVarDecl>(node, node->getAccess(),
                                  std::vector<string>(node->getIdentifiers()),
                                  visitTypeRef(node->getType().get()),
                                  node->isConst(),copyOrNull(node->getValue()));
}

std::shared_ptr<Expression>
InstantiatorImpl::visitNonTypeTemplateArgExpr(NonTypeTemplateArgExpr *node)
{
   llvm_unreachable("suck my peepee");
}

std::shared_ptr<Expression>
InstantiatorImpl::visitIdentifierRefExpr(IdentifierRefExpr *node)
{
   string ident;
   if (auto Arg = hasTemplateArg(node->getIdent())) {
      if (Arg->isType()) {
         if (!node->getMemberExpr()) {
            return makeExpr<TypeRef>(node, QualType(Arg->getType()));
         }

         if (Arg->getType()->isObjectTy())
            ident = Arg->getType()->toString();
      }
      else {
         auto valTy = hasTemplateParam(node->getIdent())->valueType;
         std::shared_ptr<Expression> literal;

         if (valTy->isIntegerTy()) {
            switch (valTy->getBitwidth()) {
               case 1:
                  literal = makeExpr<BoolLiteral>(node,
                                                  Arg->getValue().intVal != 0);
               case 8:
                  literal = makeExpr<CharLiteral>(node,
                                                  (char)Arg->getValue().intVal);
               default: {
                  auto Int = makeExpr<IntegerLiteral>(node,
                                                      Variant(Arg->getValue()));

                  Int->setType(valTy);
                  literal = Int;

                  break;
               }
            }
         }
         else if (valTy->isFPType()) {
            auto FP = makeExpr<FPLiteral>(node, Variant(Arg->getValue()));
            FP->setType(valTy);

            literal = FP;
         }
         else if (valTy->isObjectTy() && valTy->getClassName() == "String") {
            literal = makeExpr<StringLiteral>(
               node, string(Arg->getValue().getString()));
         }

         literal->setContextualType(QualType(valTy));
         return literal;
      }
   }
   if (CurrentVariadicArgSubstitution.first.equals(node->getIdent()))
      ident = CurrentVariadicArgSubstitution.second;

   if (ident.empty())
      ident = node->getIdent();

   auto expr = makeExpr<IdentifierRefExpr>(node, move(ident));
   expr->isLetExpr(node->isLetExpr());
   expr->isVarExpr(node->isVarExpr());
   copyTemplateArgs(node, expr);

   return expr;
}

std::shared_ptr<MemberRefExpr>
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

std::shared_ptr<ast::EnumCaseExpr>
InstantiatorImpl::visitEnumCaseExpr(EnumCaseExpr *node)
{
   return makeExpr<EnumCaseExpr>(node, string(node->getIdent()),
                                 copyExprList(node->getArgs()));
}

std::shared_ptr<Expression> InstantiatorImpl::visitCallExpr(CallExpr *node)
{
   if (node->getKind() == CallKind::VariadicSizeof) {
      auto TA = hasTemplateArg(node->getTemplateArgs().front().getType()
                               ->getSingularTypeName());


      if (TA) {
         if (!TA->isVariadic())
            err(err_generic_error)
               << "sizeof... requires variadic template argument, "
                  + TA->toString() + " given"
               << node;
         else {
            auto var = Variant(IntegerType::getUnsigned(),
                               TA->getVariadicArgs().size());

            return makeExpr<IntegerLiteral>(node, std::move(var));
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
   call->setIsPointerAccess_(node->isPointerAccess_());

   copyTemplateArgs(node, call);

   return call;
}

std::shared_ptr<SubscriptExpr>
InstantiatorImpl::visitSubscriptExpr(SubscriptExpr *node)
{
   return makeExpr<SubscriptExpr>(node, visitExpr(node->getIndex()));
}

std::shared_ptr<ReturnStmt>
InstantiatorImpl::visitReturnStmt(ReturnStmt *node)
{
   if (auto Val = node->getReturnValue()) {
      return makeStmt<ReturnStmt>(node, visitExpr(Val.get()));
   }
   else {
      return makeStmt<ReturnStmt>(node);
   }
}

std::shared_ptr<UnaryOperator>
InstantiatorImpl::visitUnaryOperator(UnaryOperator *node)
{
   return makeExpr<UnaryOperator>(node, string(node->getOp()),
                                  visitExpr(node->getTarget()),
                                  node->isPrefix() ? "prefix" : "postfix");
}

std::shared_ptr<Expression>
InstantiatorImpl::visitBinaryOperator(BinaryOperator *node)
{
   if (auto Ident = dyn_cast<IdentifierRefExpr>(node->getRhs().get())) {
      if (Ident->getIdent() == "...") {
         llvm::SmallVector<std::shared_ptr<Expression>, 4> exprs;
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
                                                  move(exprs[0]),
                                                  move(exprs[1]));

            while (i < numExprs) {
               binOp = makeExpr<BinaryOperator>(node, string(node->getOp()),
                                                move(binOp), move(exprs[i]));

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

std::shared_ptr<TertiaryOperator>
InstantiatorImpl::visitTertiaryOperator(TertiaryOperator *node)
{
   return makeExpr<TertiaryOperator>(node, visitExpr(node->getCondition()),
                                     visitExpr(node->getLhs()),
                                     visitExpr(node->getRhs()));
}

std::shared_ptr<Expression>
InstantiatorImpl::visitExprSequence(ExprSequence *node)
{
   if (node->getFragments().size() == 1
       && node->isParenthesized()
       && node->getFragments().front().getExpr()
              ->isVariadicArgPackExpansion()) {
      std::vector<std::shared_ptr<Expression>> tupleElements;
      copyVariadicExpr(tupleElements,
                       node->getFragments().front().getExpr());

      std::vector<pair<string, Expression::SharedPtr>> namedEls;
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
               std::make_shared<IdentifierRefExpr>(string(El.getOp()));

            maybeIdent->setSourceLoc(El.getLoc());
            auto expr = visitIdentifierRefExpr(maybeIdent.get());

            auto ident = dyn_cast<IdentifierRefExpr>(expr.get());
            if (!ident || ident->getIdent() != El.getOp())
               fragments.emplace_back(std::move(expr));
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

std::shared_ptr<ast::LambdaExpr>
InstantiatorImpl::visitLambdaExpr(LambdaExpr *node)
{
   return makeExpr<LambdaExpr>(node,
                               visitTypeRef(node->getReturnType().get()),
                               cloneVector(node->getArgs()),
                               visitStmt(node->getBody()));
}

std::shared_ptr<ast::DictionaryLiteral>
InstantiatorImpl::visitDictionaryLiteral(DictionaryLiteral *node)
{
   return makeExpr<DictionaryLiteral>(node, cloneVector(node->getKeys()),
                                      cloneVector(node->getValues()));
}

std::shared_ptr<ast::ArrayLiteral>
InstantiatorImpl::visitArrayLiteral(ArrayLiteral *node)
{
   return makeExpr<ArrayLiteral>(node, cloneVector(node->getValues()));
}

std::shared_ptr<IfStmt> InstantiatorImpl::visitIfStmt(IfStmt *node)
{
   return makeStmt<IfStmt>(node,
                           visitExpr(node->getCondition().get()),
                           visitStmt(node->getIfBranch().get()),
                           copyOrNull(node->getElseBranch()));
}

std::shared_ptr<WhileStmt> InstantiatorImpl::visitWhileStmt(WhileStmt *node)
{
   return makeStmt<WhileStmt>(node,
                              visitExpr(node->getCondition()),
                              visitStmt(node->getBody()),
                              node->isAtLeastOnce());
}

std::shared_ptr<ForStmt> InstantiatorImpl::visitForStmt(ForStmt *node)
{
   return makeStmt<ForStmt>(node,
                            visitStmt(node->getInitialization()),
                            visitExpr(node->getTermination()),
                            visitStmt(node->getIncrement()),
                            visitStmt(node->getBody()));
}

std::shared_ptr<ForInStmt> InstantiatorImpl::visitForInStmt(ForInStmt *node)
{
   return makeStmt<ForInStmt>(node,
                              visitLocalVarDecl(node->getDecl().get()),
                              visitExpr(node->getRangeExpr()),
                              visitStmt(node->getBody()));
}

std::shared_ptr<ast::MatchStmt>
InstantiatorImpl::visitMatchStmt(MatchStmt *node)
{
   return makeStmt<MatchStmt>(node,
                              visitExpr(node->getSwitchValue()),
                              cloneVector(node->getCases()));
}

std::shared_ptr<ast::CaseStmt> InstantiatorImpl::visitCaseStmt(CaseStmt *node)
{
   if (node->isDefault())
      return makeStmt<CaseStmt>(node,
                                copyOrNull(node->getBody()));

   return makeStmt<CaseStmt>(node,
                             copyOrNull(node->getPattern()),
                             copyOrNull(node->getBody()));
}

std::shared_ptr<ast::ExpressionPattern>
InstantiatorImpl::visitExpressionPattern(ExpressionPattern *node)
{
   return makeExpr<ExpressionPattern>(node, visitExpr(node->getExpr()));
}

std::shared_ptr<ast::IsPattern>
InstantiatorImpl::visitIsPattern(IsPattern *node)
{
   return makeExpr<IsPattern>(node, visitTypeRef(node->getIsType().get()));
}

std::shared_ptr<ast::CasePattern>
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

std::shared_ptr<ast::TryStmt> InstantiatorImpl::visitTryStmt(TryStmt *node)
{
   std::vector<CatchBlock> catchBlocks;
   for (const auto &CB : node->getCatchBlocks()) {
      catchBlocks.emplace_back(visitTypeRef(CB.caughtType.get()),
         string(CB.identifier), visitStmt(CB.body));
   }

   return makeStmt<TryStmt>(node, visitStmt(node->getBody()),
                            move(catchBlocks),
                            copyOrNull(node->getFinallyBlock()));
}

std::shared_ptr<ast::ThrowStmt>
InstantiatorImpl::visitThrowStmt(ThrowStmt *node)
{
   return makeStmt<ThrowStmt>(node,
                              visitExpr(node->getThrownVal()));
}

std::shared_ptr<UsingStmt> InstantiatorImpl::visitUsingStmt(UsingStmt *node)
{
   return makeStmt<UsingStmt>(node,
                              std::vector<string>(node->getFullNames()));
}

std::shared_ptr<ast::ModuleStmt>
InstantiatorImpl::visitModuleStmt(ModuleStmt *node)
{
   return makeStmt<ModuleStmt>(
      node, std::vector<string>(node->getQualifiedModuleName()));
}

std::shared_ptr<ast::ImportStmt>
InstantiatorImpl::visitImportStmt(ImportStmt *node)
{
   return makeStmt<ImportStmt>(
      node, std::vector<string>(node->getQualifiedModuleName()));
}

std::shared_ptr<ast::StaticExpr>
InstantiatorImpl::visitStaticExpr(StaticExpr *node)
{
   return makeExpr<StaticExpr>(node, visitExpr(node->getExpr()));
}

std::shared_ptr<ast::ConstraintExpr>
InstantiatorImpl::visitConstraintExpr(ConstraintExpr *node)
{
   if (node->getKind() == ConstraintExpr::Type)
      return makeExpr<ConstraintExpr>(node,
                                visitTypeRef(node->getTypeConstraint().get()));

   return makeExpr<ConstraintExpr>(node, node->getKind());
}

std::shared_ptr<ast::TraitsExpr>
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
            args.emplace_back(visitTypeRef(arg.getType().get()));
            break;
         case TraitsExpr::TraitsArgument::String:
            args.emplace_back(string(arg.getStr()));
            break;
      }
   }

   return makeExpr<TraitsExpr>(node, node->getKind(), move(args));
}

std::shared_ptr<ast::StaticStmt>
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

std::shared_ptr<ast::StaticIfStmt>
InstantiatorImpl::visitStaticIfStmt(StaticIfStmt *node)
{
   return makeStmt<StaticIfStmt>(node,
                                 visitStaticExpr(node->getCondition().get()),
                                 visitStmt(node->getIfBranch()),
                                 copyOrNull(node->getElseBranch()));
}

std::shared_ptr<ast::StaticForStmt>
InstantiatorImpl::visitStaticForStmt(StaticForStmt *node)
{
   return makeStmt<StaticForStmt>(node,
                                  string(node->getElementName()),
                                  visitStaticExpr(node->getRange().get()),
                                  visitStmt(node->getBody()));
}

std::shared_ptr<ast::StaticAssertStmt>
InstantiatorImpl::visitStaticAssertStmt(StaticAssertStmt *node)
{
   return makeStmt<StaticAssertStmt>(node,
                                     visitStaticExpr(node->getExpr().get()),
                                     string(node->getMessage()));
}

std::shared_ptr<ast::StaticPrintStmt>
InstantiatorImpl::visitStaticPrintStmt(StaticPrintStmt *node)
{
   return makeStmt<StaticPrintStmt>(node,
                                    visitStaticExpr(node->getExpr().get()));
}

void emitErrors(InstantiatorImpl &Inst)
{
   bool isFatal = false;
   for (auto &diag : Inst.getDiagnostics()) {
      isFatal |= diag.getDiagnosticKind() == DiagnosticKind::ERROR;
      diag << diag::cont;
   }

   if (isFatal)
      exit(EXIT_FAILURE);
}

} // anonymous namespace

cl::Record* TemplateInstantiator::InstantiateRecord(
                                 ast::SemaPass &SP,
                                 const SourceLocation &instantiatedFrom,
                                 cl::Record *rec,
                                 TemplateArgList&& templateArgs,
                                 bool *isNew) {
   if (templateArgs.isStillDependant())
      return rec;

   assert(!rec->getSpecializedTemplate() && "only instantiate base template!");

   InstantiatorImpl Instantiator(templateArgs, rec);

   if (auto Inst = rec->hasInstantiation(Instantiator.getTemplateArgString())) {
      rec = Inst;

      if (isNew)
         *isNew = false;
   }
   else {
      DeclPass pass(SP, rec, false);
      pass.visitRecordDeclIfNotAlreadyVisited(rec->getDecl());

      diag::DiagnosticBuilder::pushInstantiationCtx(
         InstantiationContext((InstantiationContext::Kind)rec->getTypeID(),
                              rec->getName(), instantiatedFrom, &templateArgs));

      auto recordDecl_ = Instantiator.visitRecordDecl(rec->getDecl());
      emitErrors(Instantiator);

      if (!recordDecl_) {
         diag::DiagnosticBuilder::popInstantiationCtx();
         return rec;
      }

      pass.DeclareRecord(recordDecl_);

      auto recordDecl = recordDecl_.get();
      rec->addInstantiation(move(recordDecl_));

      if (auto Loc = Instantiator.getFailedConstraintLoc())
         diag::err(err_generic_error)
            << "constraint not satisfied"
            << Loc << diag::term;


      pass.pushNamespace(rec->getNameWitoutNamespace().str()
                         + Instantiator.getTemplateArgString());

      pass.importFileImports(rec->getSourceLoc().getSourceId());
      pass.popNamespace();

      for (const auto &ext : rec->getExtensions()) {
         auto newExt = Instantiator.visitExtensionDecl(ext.get());
         if (newExt)
            pass.DeclareRecord(newExt);
      }

      auto newRec = recordDecl->getRecord();
      newRec->setTemplateArgs(std::move(templateArgs));
      newRec->setSpecializedTemplate(rec);
      newRec->setInstantiatedFrom(instantiatedFrom);

      diag::DiagnosticBuilder::popInstantiationCtx();
      diag::DiagnosticBuilder::pushInstantiationCtx(newRec);

      if (SP.getStage() != SemaPass::Stage::Declaration)
         SP.visitRecordInstantiation(newRec);
      else
         SP.addDelayedInstantiation(newRec);

      rec = newRec;

      diag::DiagnosticBuilder::popInstantiationCtx();

      if (isNew)
         *isNew = true;
   }

   return rec;
}

Function* TemplateInstantiator::InstantiateFunction(
                                 ast::SemaPass &SP,
                                 const SourceLocation &instantiatedFrom,
                                 Function *F,
                                 TemplateArgList&& templateArgs,
                                 bool *isNew) {
   if (templateArgs.isStillDependant())
      return F;

   InstantiatorImpl Instantiator(InstantiatorImpl::IK_Function,
                                 templateArgs, F);

   if (auto Inst = F->hasInstantiation(Instantiator.getTemplateArgString())) {
      if (isNew)
         *isNew = false;

      return Inst;
   }
   else {
      DeclPass pass(SP, F->getDeclarationNamespace());

      auto decl = Instantiator.visitFunctionDecl(
         cast<FunctionDecl>(F->getDeclaration()));

      emitErrors(Instantiator);

      diag::DiagnosticBuilder::pushInstantiationCtx(
         InstantiationContext(InstantiationContext::Kind::Function,
                              F->getName(),
                              instantiatedFrom,
                              &templateArgs));

      pass.DeclareFunction(decl.get());

      pass.importFileImports(F->getSourceLoc().getSourceId());
      pass.visitFunctionDecl(decl.get());

      auto newFun = cast<Function>(decl->getCallable());
      newFun->setInstantiatedFrom(instantiatedFrom);
      newFun->setTemplateArgs(std::move(templateArgs));
      newFun->setSpecializedTemplate(F);

      F->addInstantiation(move(decl));
      SP.visitFunctionInstantiation(newFun);

      diag::DiagnosticBuilder::popInstantiationCtx();

      if (isNew)
         *isNew = true;

      return newFun;
   }
}

cl::Method* TemplateInstantiator::InstantiateMethod(
                                 ast::SemaPass &SP,
                                 const SourceLocation &instantiatedFrom,
                                 cl::Method *M,
                                 TemplateArgList&& templateArgs,
                                 bool *isNew, cl::Record *R) {
   InstantiatorImpl Instantiator(InstantiatorImpl::IK_Method, templateArgs, M);

   if (auto Inst = M->hasInstantiation(Instantiator.getTemplateArgString())) {
      if (isNew)
         *isNew = false;

      return Inst;
   }
   else {
      if (!R)
         R = M->getOwningRecord();

      std::shared_ptr<CallableDecl> decl;
      if (M->isInitializer()) {
         auto initDecl = Instantiator.visitConstrDecl(
            cast<ConstrDecl>(M->getDeclaration()));

         initDecl->setRecord(R);
         decl = initDecl;
      }
      else {
         auto methodDecl = Instantiator.visitMethodDecl(
            cast<MethodDecl>(M->getDeclaration()));

         methodDecl->setRecord(R);
         decl = methodDecl;
      }

      emitErrors(Instantiator);

      diag::DiagnosticBuilder::pushInstantiationCtx(
         InstantiationContext(InstantiationContext::Kind::Method,
                              M->getName(),
                              instantiatedFrom,
                              &templateArgs));

      DeclPass pass(SP, R->getDeclarationNamespace());
      pass.importFileImports(M->getSourceLoc().getSourceId());
      pass.beginRecordScope(R);
      pass.visit(decl.get());

      auto newMethod = cast<Method>(decl->getCallable());
      newMethod->setInstantiatedFrom(instantiatedFrom);
      newMethod->setSpecializedTemplate(M);
      newMethod->setTemplateArgs(std::move(templateArgs));

      M->addInstantiation(move(decl));

      SP.visitFunctionInstantiation(newMethod);

      diag::DiagnosticBuilder::popInstantiationCtx();

      if (isNew)
         *isNew = true;

      return newMethod;
   }
}

cl::Method* TemplateInstantiator::InstantiateProtocolDefaultImpl(
                                       ast::SemaPass &SP,
                                       const SourceLocation &instantiatedFrom,
                                       cl::Record *Rec, cl::Method const* M) {
   InstantiatorImpl Instantiator(InstantiatorImpl::IK_ProtocolDefaultImpl,
                                 {}, M);

   auto decl = Instantiator.visitMethodDecl(
      cast<MethodDecl>(M->getDeclaration()));

   emitErrors(Instantiator);

   decl->setRecord(Rec);

   diag::DiagnosticBuilder::pushInstantiationCtx(
      InstantiationContext(InstantiationContext::Kind::Method,
                           M->getName(),
                           instantiatedFrom, {}));

   DeclPass pass(SP);
   pass.importFileImports(M->getSourceLoc().getSourceId());
   pass.beginRecordScope(Rec);
   pass.visitMethodDecl(decl.get());

   auto newMethod = cast<Method>(decl->getCallable());
   newMethod->setInstantiatedFrom(instantiatedFrom);
   newMethod->setSpecializedTemplate(M);

   Rec->addProtocolMethodInstantiation(decl);

   diag::DiagnosticBuilder::popInstantiationCtx();

   return newMethod;
}

std::shared_ptr<ast::Statement>
TemplateInstantiator::InstantiateStatement(
                                 ast::SemaPass &SP,
                                 const SourceLocation &instantiatedFrom,
                                 const std::shared_ptr<ast::Statement> &stmt,
                                 sema::TemplateArgList const& templateArgs) {
   InstantiatorImpl Instantiator(templateArgs);
   auto res = Instantiator.visitStmt(stmt.get());
   emitErrors(Instantiator);

   return res;
}

std::shared_ptr<ast::Statement> TemplateInstantiator::InstantiateMethodBody(
                                    ast::SemaPass &SP,
                                    const SourceLocation &instantiatedFrom,
                                    cl::Method const*baseMethod,
                                    cl::Method* newMethod) {
   InstantiatorImpl Instantiator(InstantiatorImpl::IK_Method,
                                 newMethod->getOwningRecord()
                                          ->getTemplateArgs(),
                                 baseMethod);

   Instantiator.findVariadicArgs(baseMethod->getDeclaration()->getArgs(),
                                 baseMethod->getTemplateParams());

   return Instantiator.visitStmt(baseMethod->getDeclaration()->getBody());
}

std::shared_ptr<ast::StaticExpr> TemplateInstantiator::InstantiateStaticExpr(
                                 ast::SemaPass &SP,
                                 const SourceLocation &instantiatedFrom,
                                 const std::shared_ptr<ast::StaticExpr> &stmt,
                                 sema::TemplateArgList const& templateArgs) {
   return std::static_pointer_cast<StaticExpr>(
      InstantiateStatement(SP, instantiatedFrom, stmt, templateArgs));
}

std::shared_ptr<ast::StaticExpr> TemplateInstantiator::InstantiateAlias(
                                 ast::SemaPass &SP,
                                 Alias *alias,
                                 const SourceLocation &instantiatedFrom,
                                 const std::shared_ptr<ast::StaticExpr> &stmt,
                                 sema::TemplateArgList const &templateArgs) {
   InstantiatorImpl Inst(templateArgs, alias);
   return Inst.visitStaticExpr(stmt.get());
}

} // namespace cdot