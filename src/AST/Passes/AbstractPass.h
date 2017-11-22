//
// Created by Jonas Zell on 25.06.17.
//

#ifndef CDOT_VISITOR_H
#define CDOT_VISITOR_H

#include <vector>

#include "../AstNode.h"
#include "../AstDeclarations.h"
#include "../../Compiler.h"

namespace cdot {
namespace ast {

class AbstractPass {
public:
   enum TypeID {
#  define CDOT_AST_PASS(Name) \
      Name##ID,
#  define CDOT_INCLUDE_ALL
#  include "Passes.def"
   };

//   void visit(AstNode *node)
//   {
//      switch (node->getTypeID()) {
//#     define CDOT_ASTNODE(Name)             \
//         case Name##ID:                      \
//            visit(static_cast<Name*>(node)); \
//            break;
//#     define CDOT_INCLUDE_ALL
//#     include "../AstNode.def"
//      }
//   }
//
//   void visit(Expression *node)
//   {
//      visit((AstNode*)node);
//   }
//
//   void visit(Statement *node)
//   {
//      visit((AstNode*)node);
//   }

//   void visit(NamespaceDecl *node) {}
//   void visit(UsingStmt *node) {}
//   void visit(CompoundStmt *node) {}
//   void visit(DeclStmt *node) {}
//   void visit(FunctionDecl *node) {}
//   void visit(CallableDecl *node) {}
//   void visit(DeclareStmt *node) {}
//   void visit(UnionDecl *node) {}
//   void visit(ClassDecl *node) {}
//   void visit(MethodDecl *node) {}
//   void visit(FieldDecl *node) {}
//   void visit(ConstrDecl *node) {}
//   void visit(DestrDecl *node) {}
//   void visit(ExtensionDecl *node) {}
//   void visit(PropDecl *node) {}
//   void visit(RecordTemplateDecl *node) {}
//   void visit(CallableTemplateDecl *node) {}
//   void visit(MethodTemplateDecl *node) {}
//   void visit(EnumDecl *node) {}
//   void visit(EnumCaseDecl *node) {}
//   void visit(IdentifierRefExpr *node) {}
//   void visit(SubscriptExpr *node) {}
//   void visit(CallExpr *node) {}
//   void visit(MemberRefExpr *node) {}
//   void visit(ForStmt *node) {}
//   void visit(ForInStmt *node) {}
//   void visit(WhileStmt *node) {}
//   void visit(IfStmt *node) {}
//   void visit(MatchStmt *node) {}
//   void visit(CaseStmt *node) {}
//   void visit(LabelStmt *node) {}
//   void visit(GotoStmt *node) {}
//   void visit(ReturnStmt *node) {}
//   void visit(BreakStmt *node) {}
//   void visit(ContinueStmt *node) {}
//   void visit(CollectionLiteral *node) {}
//   void visit(IntegerLiteral *node) {}
//   void visit(FPLiteral *node) {}
//   void visit(BoolLiteral *node) {}
//   void visit(CharLiteral *node) {}
//   void visit(NoneLiteral *node) {}
//   void visit(StringLiteral *node) {}
//   void visit(StringInterpolation *node) {}
//   void visit(TupleLiteral *node) {}
//   void visit(UnaryOperator *node) {}
//   void visit(BinaryOperator *node) {}
//   void visit(TertiaryOperator *node) {}
//   void visit(FuncArgDecl *node) {}
//   void visit(LambdaExpr *node) {}
//   void visit(ImplicitCastExpr *node) {}
//   void visit(TypedefDecl *node) {}
//   void visit(TypeRef *node) {}
//   void visit(LvalueToRvalue *node) {}
//   void visit(EndOfFileStmt *node) {}
//   void visit(DebugStmt *node) {}
//   void visit(TryStmt *node) {}
//   void visit(ThrowStmt *node) {}
//   void visit(InheritanceConstraint *constraint) {}
//   void visit(ConformanceConstraint *constraint) {}
//   void visit(ValueExprConstraint *constraint) {}
//   void visit(TokenEqualityConstraint *constraint) {}

   TypeID getTypeID() const
   {
      return typeID;
   }

protected:
   explicit AbstractPass(TypeID typeID) : typeID(typeID)
   {}

   void deferVisit(const std::shared_ptr<AstNode> &node)
   {
      DeferredNodes.push_back(node);
   }

   void visitDeferred()
   {
      while (!DeferredNodes.empty()) {
         auto next = DeferredNodes.back();
         DeferredNodes.pop_back();
      }
   }

   TypeID typeID;
   std::vector<std::shared_ptr<AstNode>> DeferredNodes;
};

}
}


#endif //CDOT_VISITOR_H
