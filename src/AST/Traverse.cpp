//
// Created by Jonas Zell on 19.01.18.
//

#include "Traverse.h"
#include "Passes/ASTIncludes.h"

namespace cdot {
namespace ast {

namespace {

template <class T>
void insertStmts(llvm::SmallVectorImpl<Statement *> &children, T const &t)
{
   children.insert(children.end(), t.begin(), t.end());
}

template<class T>
void insertIfNotNull(llvm::SmallVectorImpl<Statement*> &children, T *t)
{
   if (t)
      children.push_back(t);
}

void collectNamedDecl(llvm::SmallVectorImpl<Statement*> &children,
                      NamedDecl *ND) {
   insertStmts(children, ND->getConstraints());
   insertStmts(children, ND->getTemplateParams());
}

void collectTranslationUnit(llvm::SmallVectorImpl<Statement*> &children,
                            TranslationUnit* stmt) {
   insertStmts(children, stmt->getImports());
   insertStmts(children, stmt->getStatements());
}

void collectCompoundStmt(llvm::SmallVectorImpl<Statement*> &children,
                         CompoundStmt* stmt) {
   insertStmts(children, stmt->getStatements());
}

void collectBreakStmt(llvm::SmallVectorImpl<Statement*> &children,
                      BreakStmt* stmt) {

}

void collectContinueStmt(llvm::SmallVectorImpl<Statement*> &children,
                         ContinueStmt* stmt) {

}

void collectReturnStmt(llvm::SmallVectorImpl<Statement*> &children,
                       ReturnStmt* stmt) {
   if (auto ret = stmt->getReturnValue())
      children.push_back(ret);
}

void collectCaseStmt(llvm::SmallVectorImpl<Statement*> &children,
                     CaseStmt* stmt) {
   if (auto pat = stmt->getPattern())
      children.push_back(pat);

   if (auto body = stmt->getBody())
      children.push_back(body);
}

void collectLabelStmt(llvm::SmallVectorImpl<Statement*> &children,
                      LabelStmt* stmt) {

}

void collectGotoStmt(llvm::SmallVectorImpl<Statement*> &children,
                     GotoStmt* stmt) {}

void collectForStmt(llvm::SmallVectorImpl<Statement*> &children,
                    ForStmt* stmt) {
   insertIfNotNull(children, stmt->getInitialization());
   insertIfNotNull(children, stmt->getTermination());
   insertIfNotNull(children, stmt->getIncrement());
   insertIfNotNull(children, stmt->getBody());
}

void collectIfStmt(llvm::SmallVectorImpl<Statement*> &children,
                   IfStmt* stmt) {
   insertIfNotNull(children, stmt->getCondition());
   insertIfNotNull(children, stmt->getIfBranch());
   insertIfNotNull(children, stmt->getElseBranch());
}

void collectWhileStmt(llvm::SmallVectorImpl<Statement*> &children,
                      WhileStmt* stmt) {
   insertIfNotNull(children, stmt->getCondition());
   insertIfNotNull(children, stmt->getBody());
}

void collectForInStmt(llvm::SmallVectorImpl<Statement*> &children,
                      ForInStmt* stmt) {
   insertIfNotNull(children, stmt->getRangeExpr());
   insertIfNotNull(children, stmt->getBody());
}

void collectMatchStmt(llvm::SmallVectorImpl<Statement*> &children,
                      MatchStmt* stmt) {
   insertIfNotNull(children, stmt->getSwitchValue());
   insertStmts(children, stmt->getCases());
}

void collectLocalVarDecl(llvm::SmallVectorImpl<Statement*> &children,
                         LocalVarDecl* stmt) {
   insertIfNotNull(children, stmt->getValue());
}

void collectGlobalVarDecl(llvm::SmallVectorImpl<Statement*> &children,
                          GlobalVarDecl* stmt) {
   insertIfNotNull(children, stmt->getValue());
}

void collectLocalDestructuringDecl(llvm::SmallVectorImpl<Statement*> &children,
                                   LocalDestructuringDecl* stmt) {
   insertIfNotNull(children, stmt->getValue());
}

void collectGlobalDestructuringDecl(llvm::SmallVectorImpl<Statement*> &children,
                                    GlobalDestructuringDecl* stmt) {
   insertIfNotNull(children, stmt->getValue());
}

void collectAliasDecl(llvm::SmallVectorImpl<Statement*> &children,
                      AliasDecl* stmt) {
   collectNamedDecl(children, stmt);
   insertIfNotNull(children, stmt->getAliasExpr());
}

void collectEnumCaseDecl(llvm::SmallVectorImpl<Statement*> &children,
                         EnumCaseDecl* stmt) {
   collectNamedDecl(children, stmt);
   insertStmts(children, stmt->getArgs());
}

void collectInitDecl(llvm::SmallVectorImpl<Statement*> &children,
                     InitDecl* stmt) {
   collectNamedDecl(children, stmt);
   insertStmts(children, stmt->getArgs());
   insertIfNotNull(children, stmt->getBody());
}

void collectFieldDecl(llvm::SmallVectorImpl<Statement*> &children,
                      FieldDecl* stmt) {
   collectNamedDecl(children, stmt);
   insertIfNotNull(children, stmt->getDefaultVal());
}

void collectMethodDecl(llvm::SmallVectorImpl<Statement*> &children,
                       MethodDecl* stmt) {
   collectNamedDecl(children, stmt);
   insertStmts(children, stmt->getArgs());
   children.push_back(stmt->getReturnType());
   insertIfNotNull(children, stmt->getBody());
}

void collectAssociatedTypeDecl(llvm::SmallVectorImpl<Statement*> &children,
                               AssociatedTypeDecl* stmt) {
   collectNamedDecl(children, stmt);
   insertStmts(children, stmt->getConstraints());
   insertIfNotNull(children, stmt->getActualType());
}

void collectFuncArgDecl(llvm::SmallVectorImpl<Statement*> &children,
                        FuncArgDecl* stmt) {
   collectNamedDecl(children, stmt);
   insertIfNotNull(children, stmt->getDefaultVal());
}

void collectDeinitDecl(llvm::SmallVectorImpl<Statement*> &children,
                      DeinitDecl* stmt) {
   collectNamedDecl(children, stmt);
   insertIfNotNull(children, stmt->getBody());
}

void collectPropDecl(llvm::SmallVectorImpl<Statement*> &children,
                     PropDecl* stmt) {
   collectNamedDecl(children, stmt);
   children.push_back(stmt->getType());
   insertIfNotNull(children, stmt->getGetterBody());
   insertIfNotNull(children, stmt->getSetterBody());
}

void collectTemplateParamDecl(llvm::SmallVectorImpl<Statement*> &children,
                              TemplateParamDecl* stmt) {
   collectNamedDecl(children, stmt);
   insertIfNotNull(children, stmt->getCovariance());
   insertIfNotNull(children, stmt->getContravariance());
   insertIfNotNull(children, stmt->getDefaultValue());
}

void collectTemplateArgExpr(llvm::SmallVectorImpl<Statement*> &children,
                            TemplateArgExpr* stmt) {
   if (stmt->isTypeName())
      children.push_back(stmt->getType());
   else
      children.push_back(stmt->getExpr());
}

void collectRecordDecl(llvm::SmallVectorImpl<Statement*> &children,
                       RecordDecl* stmt) {
   collectNamedDecl(children, stmt);
   insertStmts(children, stmt->getConformanceTypes());
   insertStmts(children, stmt->getDecls());
   insertStmts(children, stmt->getStaticStatements());
}

void collectExtensionDecl(llvm::SmallVectorImpl<Statement*> &children,
                          ExtensionDecl* stmt) {
   collectRecordDecl(children, stmt);
}

void collectCallableDecl(llvm::SmallVectorImpl<Statement*> &children,
                         CallableDecl* stmt) {
   collectNamedDecl(children, stmt);
   insertStmts(children, stmt->getArgs());
   children.push_back(stmt->getReturnType());
   insertIfNotNull(children, stmt->getBody());
}

void collectFunctionDecl(llvm::SmallVectorImpl<Statement*> &children,
                         FunctionDecl* stmt) {
   collectCallableDecl(children, stmt);
}

void collectNamespaceDecl(llvm::SmallVectorImpl<Statement*> &children,
                          NamespaceDecl* stmt) {
   children.push_back(stmt->getBody());
}

void collectTypedefDecl(llvm::SmallVectorImpl<Statement*> &children,
                        TypedefDecl* stmt) {
   collectNamedDecl(children, stmt);
   children.push_back(stmt->getOriginTy());
}

void collectTryStmt(llvm::SmallVectorImpl<Statement*> &children,
                    TryStmt* stmt) {
   insertIfNotNull(children, stmt->getBody());

   for (auto &c : stmt->getCatchBlocks()) {
      children.push_back(c.varDecl);
      children.push_back(c.body);
   }

   insertIfNotNull(children, stmt->getFinallyBlock());
}

void collectThrowStmt(llvm::SmallVectorImpl<Statement*> &children,
                      ThrowStmt* stmt) {
   insertIfNotNull(children, stmt->getThrownVal());
}

void collectDebugStmt(llvm::SmallVectorImpl<Statement*> &children,
                      DebugStmt* stmt) {}

void collectNullStmt(llvm::SmallVectorImpl<Statement*> &children,
                     NullStmt* stmt) {}

void collectUsingStmt(llvm::SmallVectorImpl<Statement*> &children,
                      UsingStmt* stmt) {}

void collectModuleStmt(llvm::SmallVectorImpl<Statement*> &children,
                       ModuleStmt* stmt) {}

void collectImportStmt(llvm::SmallVectorImpl<Statement*> &children,
                       ImportStmt* stmt) {}

void collectStaticIfStmt(llvm::SmallVectorImpl<Statement*> &children,
                         StaticIfStmt* stmt) {
   insertIfNotNull(children, stmt->getCondition());
   insertIfNotNull(children, stmt->getIfBranch());
   insertIfNotNull(children, stmt->getElseBranch());
}

void collectStaticForStmt(llvm::SmallVectorImpl<Statement*> &children,
                          StaticForStmt* stmt) {
   insertIfNotNull(children, stmt->getRange());
   insertIfNotNull(children, stmt->getBody());
}

void collectStaticAssertStmt(llvm::SmallVectorImpl<Statement*> &children,
                             StaticAssertStmt* stmt) {
   insertIfNotNull(children, stmt->getExpr());
}

void collectStaticPrintStmt(llvm::SmallVectorImpl<Statement*> &children,
                            StaticPrintStmt* stmt) {
   children.push_back(stmt->getExpr());
}

void collectClassDecl(llvm::SmallVectorImpl<Statement*> &children,
                      ClassDecl* stmt) {
   collectRecordDecl(children, stmt);
   insertIfNotNull(children, stmt->getParentType());
}

void collectStructDecl(llvm::SmallVectorImpl<Statement*> &children,
                       StructDecl* stmt) {
   collectRecordDecl(children, stmt);
}

void collectEnumDecl(llvm::SmallVectorImpl<Statement*> &children,
                     EnumDecl* stmt) {
   collectRecordDecl(children, stmt);
}

void collectUnionDecl(llvm::SmallVectorImpl<Statement*> &children,
                      UnionDecl* stmt) {
   collectRecordDecl(children, stmt);
}

void collectProtocolDecl(llvm::SmallVectorImpl<Statement*> &children,
                         ProtocolDecl* stmt) {
   collectRecordDecl(children, stmt);
}

void collectTypeRef(llvm::SmallVectorImpl<Statement*> &children,
                    TypeRef* stmt) {
   switch (stmt->getKind()) {
      case TypeRef::ObjectType:
         for (auto &ns : stmt->getNamespaceQual())
            insertStmts(children, ns.second);

         break;
      case TypeRef::FunctionType:
         children.push_back(stmt->getReturnType());
         LLVM_FALLTHROUGH;
      case TypeRef::TupleType:
         for (auto &ty : stmt->getContainedTypes())
            children.push_back(ty.second);

         break;
      case TypeRef::DeclTypeExpr:
         children.push_back(stmt->getDeclTypeExpr());
         break;
      case TypeRef::ArrayType:
         children.push_back(stmt->getArraySize());
         for (auto &ty : stmt->getContainedTypes())
            children.push_back(ty.second);

         break;
      case TypeRef::Option:
      case TypeRef::Pointer:
         children.push_back(stmt->getSubject());
         break;
      default:
         break;
   }
}

void collectIntegerLiteral(llvm::SmallVectorImpl<Statement*> &children,
                           IntegerLiteral* stmt) {}

void collectFPLiteral(llvm::SmallVectorImpl<Statement*> &children,
                      FPLiteral* stmt) {}

void collectBoolLiteral(llvm::SmallVectorImpl<Statement*> &children,
                        BoolLiteral* stmt) {}

void collectCharLiteral(llvm::SmallVectorImpl<Statement*> &children,
                        CharLiteral* stmt) {}

void collectNoneLiteral(llvm::SmallVectorImpl<Statement*> &children,
                        NoneLiteral* stmt) {}

void collectStringLiteral(llvm::SmallVectorImpl<Statement*> &children,
                          StringLiteral* stmt) {}

void collectStringInterpolation(llvm::SmallVectorImpl<Statement*> &children,
                                StringInterpolation* stmt) {
   insertStmts(children, stmt->getStrings());
}

void collectLambdaExpr(llvm::SmallVectorImpl<Statement*> &children,
                       LambdaExpr* stmt) {
   insertStmts(children, stmt->getArgs());
   insertIfNotNull(children, stmt->getReturnType());
   insertIfNotNull(children, stmt->getBody());
}

void collectDictionaryLiteral(llvm::SmallVectorImpl<Statement*> &children,
                              DictionaryLiteral* stmt) {
   insertStmts(children, stmt->getKeys());
   insertStmts(children, stmt->getValues());
}

void collectArrayLiteral(llvm::SmallVectorImpl<Statement*> &children,
                         ArrayLiteral* stmt) {
   insertStmts(children, stmt->getValues());
}

void collectTupleLiteral(llvm::SmallVectorImpl<Statement*> &children,
                         TupleLiteral* stmt) {
   for (auto &el : stmt->getElements())
      children.push_back(el.second);
}

void collectIdentifierRefExpr(llvm::SmallVectorImpl<Statement*> &children,
                              IdentifierRefExpr* stmt) {
   insertStmts(children, stmt->getTemplateArgs());
   insertIfNotNull(children, stmt->getSubExpr());
}

void collectMemberRefExpr(llvm::SmallVectorImpl<Statement*> &children,
                          MemberRefExpr* stmt) {
   insertStmts(children, stmt->getTemplateArgs());
   insertIfNotNull(children, stmt->getSubExpr());
}

void collectCallExpr(llvm::SmallVectorImpl<Statement*> &children,
                     CallExpr* stmt) {
   insertStmts(children, stmt->getTemplateArgs());
   insertStmts(children, stmt->getArgs());
}

void collectEnumCaseExpr(llvm::SmallVectorImpl<Statement*> &children,
                         EnumCaseExpr* stmt) {
   insertStmts(children, stmt->getArgs());
}

void collectSubscriptExpr(llvm::SmallVectorImpl<Statement*> &children,
                          SubscriptExpr* stmt) {
   insertStmts(children, stmt->getIndices());
}

void collectBuiltinExpr(llvm::SmallVectorImpl<Statement*> &children,
                                   BuiltinExpr* stmt) {}

void collectExpressionPattern(llvm::SmallVectorImpl<Statement*> &children,
                              ExpressionPattern* stmt) {
   insertIfNotNull(children, stmt->getExpr());
}

void collectCasePattern(llvm::SmallVectorImpl<Statement*> &children,
                        CasePattern* stmt) {
   for (auto &arg : stmt->getArgs())
      if (arg.isExpr())
         children.push_back(arg.getExpr());
}

void collectIsPattern(llvm::SmallVectorImpl<Statement*> &children,
                      IsPattern* stmt) {
   children.push_back(stmt->getIsType());
}

void collectExprSequence(llvm::SmallVectorImpl<Statement*> &children,
                         ExprSequence* stmt) {
   for (auto &el : stmt->getFragments())
      if (el.getKind() == ExprSequence::SequenceElement::EF_Expression)
         children.push_back(el.getExpr());
}

void collectUnaryOperator(llvm::SmallVectorImpl<Statement*> &children,
                          UnaryOperator* stmt) {
   children.push_back(stmt->getTarget());
}

void collectBinaryOperator(llvm::SmallVectorImpl<Statement*> &children,
                           BinaryOperator* stmt) {
   children.push_back(stmt->getLhs());
   children.push_back(stmt->getRhs());
}

void collectTertiaryOperator(llvm::SmallVectorImpl<Statement*> &children,
                             TertiaryOperator* stmt) {
   children.push_back(stmt->getCondition());
   children.push_back(stmt->getLhs());
   children.push_back(stmt->getRhs());
}

void collectImplicitCastExpr(llvm::SmallVectorImpl<Statement*> &children,
                             ImplicitCastExpr* stmt) {
   children.push_back(stmt->getTarget());
}

void collectLvalueToRvalue(llvm::SmallVectorImpl<Statement*> &children,
                           LvalueToRvalue* stmt) {
   children.push_back(stmt->getTarget());
}

void collectStaticExpr(llvm::SmallVectorImpl<Statement*> &children,
                       StaticExpr* stmt) {
   children.push_back(stmt->getExpr());
}

void collectConstraintExpr(llvm::SmallVectorImpl<Statement*> &children,
                           ConstraintExpr* stmt) {
   insertIfNotNull(children, stmt->getTypeConstraint());
}

void collectTraitsExpr(llvm::SmallVectorImpl<Statement*> &children,
                       TraitsExpr* stmt) {
   for (auto &arg : stmt->getArgs()) {
      switch (arg.getKind()) {
         case TraitsExpr::TraitsArgument::Type:
            children.push_back(arg.getType());
            break;
         case TraitsExpr::TraitsArgument::Stmt:
            children.push_back(arg.getStmt());
            break;
         case TraitsExpr::TraitsArgument::Expr:
            children.push_back(arg.getExpr());
            break;
         default:
            break;
      }
   }
}


} // anonymous namespace

void collectDirectChildren(llvm::SmallVectorImpl<Statement *> &children,
                           Statement const* constStmt) {
   auto stmt = const_cast<Statement*>(constStmt);
   switch (stmt->getTypeID()) {
#  define CDOT_ASTNODE(Name)                        \
      case AstNode::Name##ID:                       \
         collect##Name(children, support::cast<Name>(stmt)); \
         break;
#  include "AstNode.def"

      default:
         llvm_unreachable("bad node type");
   }

   if (auto E = support::dyn_cast<Expression>(stmt))
      insertIfNotNull(children, E->getSubExpr());
}

} // namespace ast
} // namespace cdot