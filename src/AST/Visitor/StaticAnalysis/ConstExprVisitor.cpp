////
//// Created by Jonas Zell on 07.07.17.
////
//
//#include "ConstExprVisitor.h"
//#include "../Visitor.cpp"
//
//
//Variant ConstExprVisitor::visit(NamespaceDecl *node) {
//    for (auto child : node->get_children()) {
//        child->accept(*this);
//    }
//
//    return {};
//}
//
//Variant ConstExprVisitor::visit(CompoundStmt *node) {
//    for (auto child : node->get_children()) {
//        child->accept(*this);
//    }
//
//    return {};
//}
//
//Variant ConstExprVisitor::visit(FunctionDecl *node) {
//    for (auto child : node->_args) {
//        child->accept(*this);
//    }
//
//    return {};
//}
//
//Variant ConstExprVisitor::visit(IdentifierRefExpr *node) {
//    for (auto child : node->get_children()) {
//        child->accept(*this);
//    }
//
//    return {};
//}
//
//Variant ConstExprVisitor::visit(DeclStmt *node) {
//    for (auto child : node->get_children()) {
//        child->accept(*this);
//    }
//
//    return {};
//}
//
//
//Variant ConstExprVisitor::visit(ForStmt *node) {
//    for (auto child : node->get_children()) {
//        child->accept(*this);
//    }
//
//    return {};
//}
//
//Variant ConstExprVisitor::visit(WhileStmt *node) {
//    for (auto child : node->get_children()) {
//        child->accept(*this);
//    }
//
//    return {};
//}
//
//Variant ConstExprVisitor::visit(CollectionLiteral *node) {
//    for (auto child : node->get_children()) {
//        child->accept(*this);
//    }
//
//    return {};
//}
//
//Variant ConstExprVisitor::visit(NumericLiteral *node) {
//    for (auto child : node->get_children()) {
//        child->accept(*this);
//    }
//
//    return {};
//}
//
//Variant ConstExprVisitor::visit(StringLiteral *node) {
//    return { node->value };
//}
//
//Variant ConstExprVisitor::visit(SubscriptExpr *node) {
//    for (auto child : node->get_children()) {
//        child->accept(*this);
//    }
//
//    return {};
//}
//
//Variant ConstExprVisitor::visit(CallExpr *node) {
//    for (auto child : node->get_children()) {
//        child->accept(*this);
//    }
//
//    return {};
//}
//
//Variant ConstExprVisitor::visit(MemberRefExpr *node) {
//    for (auto child : node->get_children()) {
//        child->accept(*this);
//    }
//
//    return {};
//}
//
//Variant ConstExprVisitor::visit(BinaryOperator *node) {
//
//}
//
//Variant ConstExprVisitor::visit(ExplicitCastExpr *node) {
//    for (auto child : node->get_children()) {
//        child->accept(*this);
//    }
//
//    return {};
//}
//
//Variant ConstExprVisitor::visit(TertiaryOperator *node) {
//    for (auto child : node->get_children()) {
//        child->accept(*this);
//    }
//
//    return {};
//}
//
//Variant ConstExprVisitor::visit(UnaryOperator *node) {
//    for (auto child : node->get_children()) {
//        child->accept(*this);
//    }
//
//    return {};
//}
//
//Variant ConstExprVisitor::visit(BreakStmt *node) {
//    for (auto child : node->get_children()) {
//        child->accept(*this);
//    }
//
//    return {};
//}
//
//Variant ConstExprVisitor::visit(ContinueStmt *node) {
//    for (auto child : node->get_children()) {
//        child->accept(*this);
//    }
//
//    return {};
//}
//
//Variant ConstExprVisitor::visit(IfStmt *node) {
//    for (auto child : node->get_children()) {
//        child->accept(*this);
//    }
//
//    return {};
//}
//
//Variant ConstExprVisitor::visit(MatchStmt *node) {
//    for (auto child : node->get_children()) {
//        child->accept(*this);
//    }
//
//    return {};
//}
//
//Variant ConstExprVisitor::visit(CaseStmt *node) {
//    for (auto child : node->get_children()) {
//        child->accept(*this);
//    }
//
//    return {};
//}
//
//Variant ConstExprVisitor::visit(LabelStmt *node) {
//    return {};
//}
//
//Variant ConstExprVisitor::visit(GotoStmt *node) {
//    return {};
//}
//
//Variant ConstExprVisitor::visit(FuncArgDecl *node) {
//    for (auto child : node->get_children()) {
//        child->accept(*this);
//    }
//
//    return {};
//}
//
//Variant ConstExprVisitor::visit(ReturnStmt *node) {
//    for (auto child : node->get_children()) {
//        child->accept(*this);
//    }
//
//    return {};
//}
//
//Variant ConstExprVisitor::visit(Expression *node) {
//    for (auto child : node->get_children()) {
//        child->accept(*this);
//    }
//
//    return {};
//}
//
//Variant ConstExprVisitor::visit(ClassDecl *node) {
//    for (auto child : node->get_children()) {
//        child->accept(*this);
//    }
//
//    return {};
//}
//
//Variant ConstExprVisitor::visit(ConstrDecl *node) {
//    for (auto child : node->get_children()) {
//        child->accept(*this);
//    }
//
//    return {};
//}
//
//Variant ConstExprVisitor::visit(FieldDecl *node) {
//    for (auto child : node->get_children()) {
//        child->accept(*this);
//    }
//
//    return {};
//}
//
//Variant ConstExprVisitor::visit(MethodDecl *node) {
//    for (auto child : node->get_children()) {
//        child->accept(*this);
//    }
//
//    return {};
//}
//
//Variant ConstExprVisitor::visit(LambdaExpr *node) {
//    for (auto child : node->get_children()) {
//        child->accept(*this);
//    }
//
//    return {};
//}
//
//Variant ConstExprVisitor::visit(OperatorDecl *node) {
//    for (auto child : node->get_children()) {
//        child->accept(*this);
//    }
//
//    return {};
//}
//
//Variant ConstExprVisitor::visit(UsingStmt *node) {
//    for (auto child : node->get_children()) {
//        child->accept(*this);
//    }
//
//    return {};
//}
//
//Variant ConstExprVisitor::visit(EndOfFileStmt *node) {
//    for (auto child : node->get_children()) {
//        child->accept(*this);
//    }
//
//    return {};
//}
//
//Variant ConstExprVisitor::visit(InterfaceDecl *node) {
//
//}
//
//Variant ConstExprVisitor::visit(ImplicitCastExpr *) {
//
//}
//
//Variant ConstExprVisitor::visit(ExtendStmt *) {
//
//}
//
//Variant ConstExprVisitor::visit(TypedefDecl *) {
//
//}
//
//Variant ConstExprVisitor::visit(TypeRef *) {
//
//}
//
//Variant ConstExprVisitor::visit(DeclareStmt *) {
//
//}