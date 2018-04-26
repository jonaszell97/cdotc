//
// Created by Jonas Zell on 19.01.18.
//

#include "Traverse.h"

#include "ASTVisitor.h"
#include "AST/Decl.h"
#include "AST/Expression.h"
#include "AST/Statement.h"

using namespace cdot::support;

namespace cdot {
namespace ast {

namespace {
//
//class ASTChildVisitor: ASTVisitor<> {
//public:
//   explicit ASTChildVisitor(llvm::function_ref<void(Statement*)> Callback)
//      : Callback(Callback)
//   {}
//
//   void visitAllChildren(Statement *Stmt)
//   {
//      auto CB = [&](Statement *Child) {
//         Callback(Child);
//         visitAllChildren(Child);
//      };
//
//      ASTChildVisitor Visitor(CB);
//      Visitor.visitDirectChildren(Stmt);
//   }
//
//   void visitDirectChildren(Statement *Stmt)
//   {
//      switch (Stmt->getTypeID()) {
//#     define CDOT_ASTNODE(Name)                        \
//      case AstNode::Name##ID:                          \
//         visit##Name(cast<Name>(Stmt));
//         break;
//#     include "AstNode.def"
//
//         default:
//            llvm_unreachable("bad node type");
//      }
//   }
//
//#  define CDOT_ASTNODE(Name)                        \
//      void visit##Name(Name *Stmt);
//#  include "AstNode.def"
//
//private:
//   llvm::function_ref<void(Statement*)> Callback;
//
//   template <class T>
//   void insertStmts(T const &vec)
//   {
//      for (auto &stmt : vec)
//         Callback(stmt);
//   }
//
//   template<class T>
//   void insertIfNotNull(T *t)
//   {
//      if (t)
//         Callback(t);
//   }
//};

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
}

void collectTranslationUnit(llvm::SmallVectorImpl<Statement*> &children,
                            TranslationUnit* stmt) {

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
}

void collectInitDecl(llvm::SmallVectorImpl<Statement*> &children,
                     InitDecl* stmt) {
   collectNamedDecl(children, stmt);
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
   insertIfNotNull(children, stmt->getBody());
}

void collectAssociatedTypeDecl(llvm::SmallVectorImpl<Statement*> &children,
                               AssociatedTypeDecl* stmt) {
   collectNamedDecl(children, stmt);
   insertStmts(children, stmt->getConstraints());
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
   insertIfNotNull(children, stmt->getGetterBody());
   insertIfNotNull(children, stmt->getSetterBody());
}

void collectTemplateParamDecl(llvm::SmallVectorImpl<Statement*> &children,
                              TemplateParamDecl* stmt) {
   collectNamedDecl(children, stmt);
   insertIfNotNull(children, stmt->getDefaultValue());
}

void collectRecordDecl(llvm::SmallVectorImpl<Statement*> &children,
                       RecordDecl* stmt) {
   collectNamedDecl(children, stmt);
}

void collectExtensionDecl(llvm::SmallVectorImpl<Statement*> &children,
                          ExtensionDecl* stmt) {

}

void collectCallableDecl(llvm::SmallVectorImpl<Statement*> &children,
                         CallableDecl* stmt) {
   collectNamedDecl(children, stmt);
   insertIfNotNull(children, stmt->getBody());
}

void collectFunctionDecl(llvm::SmallVectorImpl<Statement*> &children,
                         FunctionDecl* stmt) {
   collectCallableDecl(children, stmt);
}

void collectNamespaceDecl(llvm::SmallVectorImpl<Statement*> &children,
                          NamespaceDecl* stmt) {

}

void collectTypedefDecl(llvm::SmallVectorImpl<Statement*> &children,
                        TypedefDecl* stmt) {
   collectNamedDecl(children, stmt);
}

void collectTryStmt(llvm::SmallVectorImpl<Statement*> &children,
                    TryStmt* stmt) {
   insertIfNotNull(children, stmt->getBody());

   for (auto &c : stmt->getCatchBlocks()) {
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

void collectUsingDecl(llvm::SmallVectorImpl<Statement*> &children,
                      UsingDecl* stmt) {}

void collectModuleDecl(llvm::SmallVectorImpl<Statement*> &children,
                       ModuleDecl* stmt) {}

void collectImportDecl(llvm::SmallVectorImpl<Statement*> &children,
                       ImportDecl* stmt) {}

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

void collectParenExpr(llvm::SmallVectorImpl<Statement*> &children,
                      ParenExpr* stmt) {
   children.push_back(stmt->getParenthesizedExpr());
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
   for (auto &Seg : stmt->getSegments())
      children.push_back(Seg);
}

void collectLambdaExpr(llvm::SmallVectorImpl<Statement*> &children,
                       LambdaExpr* stmt) {
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
   insertStmts(children, stmt->getElements());
}

void collectIdentifierRefExpr(llvm::SmallVectorImpl<Statement*> &children,
                              IdentifierRefExpr* stmt) {
   insertIfNotNull(children, stmt->getParentExpr());
   insertStmts(children, stmt->getTemplateArgs());
}

void collectBuiltinIdentExpr(llvm::SmallVectorImpl<Statement*> &children,
                              BuiltinIdentExpr* stmt) {

}

void collectSelfExpr(llvm::SmallVectorImpl<Statement*> &children,
                     SelfExpr* stmt) {

}

void collectSuperExpr(llvm::SmallVectorImpl<Statement*> &children,
                      SuperExpr* stmt) {

}

void collectMemberRefExpr(llvm::SmallVectorImpl<Statement*> &children,
                          MemberRefExpr* stmt) {
   insertIfNotNull(children, stmt->getParentExpr());
   insertStmts(children, stmt->getTemplateArgs());
}

void collectTupleMemberExpr(llvm::SmallVectorImpl<Statement*> &children,
                            TupleMemberExpr* stmt) {
   insertIfNotNull(children, stmt->getParentExpr());
}

void collectCallExpr(llvm::SmallVectorImpl<Statement*> &children,
                     CallExpr* stmt) {
   insertIfNotNull(children, stmt->getParentExpr());
   insertStmts(children, stmt->getTemplateArgs());
   insertStmts(children, stmt->getArgs());
}

void collectAnonymousCallExpr(llvm::SmallVectorImpl<Statement*> &children,
                              AnonymousCallExpr* stmt) {
   insertIfNotNull(children, stmt->getParentExpr());
   insertStmts(children, stmt->getArgs());
}

void collectEnumCaseExpr(llvm::SmallVectorImpl<Statement*> &children,
                         EnumCaseExpr* stmt) {
   insertStmts(children, stmt->getArgs());
}

void collectSubscriptExpr(llvm::SmallVectorImpl<Statement*> &children,
                          SubscriptExpr* stmt) {
   insertIfNotNull(children, stmt->getParentExpr());
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

}

void collectExprSequence(llvm::SmallVectorImpl<Statement*> &children,
                         ExprSequence* stmt) {
   for (auto &el : stmt->getFragments())
      if (el.getKind() == SequenceElement::EF_Expression)
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

void collectAssignExpr(llvm::SmallVectorImpl<Statement*> &children,
                       AssignExpr* stmt) {
   children.push_back(stmt->getLhs());
   children.push_back(stmt->getRhs());
}

void collectTypePredicateExpr(llvm::SmallVectorImpl<Statement*> &children,
                              TypePredicateExpr* stmt) {
   children.push_back(stmt->getLHS());
   children.push_back(stmt->getRHS());
}

void collectCastExpr(llvm::SmallVectorImpl<Statement*> &children,
                     CastExpr* stmt) {
   children.push_back(stmt->getTarget());
}

void collectImplicitCastExpr(llvm::SmallVectorImpl<Statement*> &children,
                             ImplicitCastExpr* stmt) {
   children.push_back(stmt->getTarget());
}

void collectIfExpr(llvm::SmallVectorImpl<Statement*> &children,
                   IfExpr* stmt) {
   children.push_back(stmt->getCond());
   children.push_back(stmt->getTrueVal());
   children.push_back(stmt->getFalseVal());
}

void collectDeclStmt(llvm::SmallVectorImpl<Statement*> &children,
                     DeclStmt* stmt) {

}

void collectStaticExpr(llvm::SmallVectorImpl<Statement*> &children,
                       StaticExpr* stmt) {
   children.push_back(stmt->getExpr());
}

void collectConstraintExpr(llvm::SmallVectorImpl<Statement*> &children,
                           ConstraintExpr* stmt) {
}

void collectTraitsExpr(llvm::SmallVectorImpl<Statement*> &children,
                       TraitsExpr* stmt) {
   for (auto &arg : stmt->getArgs()) {
      switch (arg.getKind()) {
         case TraitsArgument::Type:
            break;
         case TraitsArgument::Stmt:
            children.push_back(arg.getStmt());
            break;
         case TraitsArgument::Expr:
            children.push_back(arg.getExpr());
            break;
         default:
            break;
      }
   }
}

void collectMixinExpr(llvm::SmallVectorImpl<Statement*> &children,
                       MixinExpr* stmt) {
   insertIfNotNull(children, stmt->getMixinExpr());
}

void collectMixinDecl(llvm::SmallVectorImpl<Statement*> &children,
                      MixinDecl* stmt) {
   insertIfNotNull(children, stmt->getMixinExpr());
}

void collectAttributedStmt(llvm::SmallVectorImpl<Statement*> &children,
                      AttributedStmt* stmt) {

}

void collectAttributedExpr(llvm::SmallVectorImpl<Statement*> &children,
                           AttributedExpr* expr) {

}

void collectFunctionTypeExpr(llvm::SmallVectorImpl<Statement*> &children,
                             FunctionTypeExpr* expr) {
   for (auto &Ty : expr->getArgTypes())
      children.push_back(Ty.getTypeExpr());

   children.push_back(expr->getReturnType().getTypeExpr());
}

void collectTupleTypeExpr(llvm::SmallVectorImpl<Statement*> &children,
                             TupleTypeExpr* expr) {
   for (auto &Ty : expr->getContainedTypes())
      children.push_back(Ty.getTypeExpr());
}

void collectArrayTypeExpr(llvm::SmallVectorImpl<Statement*> &children,
                             ArrayTypeExpr* expr) {

}

void collectDeclTypeExpr(llvm::SmallVectorImpl<Statement*> &children,
                             DeclTypeExpr* expr) {

}

void collectReferenceTypeExpr(llvm::SmallVectorImpl<Statement*> &children,
                             ReferenceTypeExpr* expr) {

}

void collectPointerTypeExpr(llvm::SmallVectorImpl<Statement*> &children,
                             PointerTypeExpr* expr) {

}

void collectOptionTypeExpr(llvm::SmallVectorImpl<Statement*> &children,
                             OptionTypeExpr* expr) {

}

void collectMixinStmt(llvm::SmallVectorImpl<Statement*> &children,
                           MixinStmt* expr) {

}

void collectMacroExpansionExpr(llvm::SmallVectorImpl<Statement*> &children,
                      MacroExpansionExpr* expr) {

}

void collectMacroVariableExpr(llvm::SmallVectorImpl<Statement*> &children,
                               MacroVariableExpr* expr) {

}

void collectMacroExpansionStmt(llvm::SmallVectorImpl<Statement*> &children,
                               MacroExpansionStmt* expr) {

}

} // anonymous namespace

void collectDirectChildren(llvm::SmallVectorImpl<Statement *> &children,
                           Statement const* constStmt) {
   auto stmt = const_cast<Statement*>(constStmt);
   switch (stmt->getTypeID()) {
#  define CDOT_ASTNODE(Name)                                   \
      case AstNode::Name##ID:                                  \
         collect##Name(children, support::cast<Name>(stmt));   \
         break;
#  include "AstNode.def"

      default:
         llvm_unreachable("bad node type");
   }
}

} // namespace ast
} // namespace cdot