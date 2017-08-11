////
//// Created by Jonas Zell on 26.06.17.
////
//
//#include "CaptureVisitor.h"
//#include "Visitor.cpp"
//
//CaptureVisitor::CaptureVisitor(Context* ctx) :
//    declared_identifiers(std::set<std::string>()),
//    referenced_identifiers(std::set<std::string>()),
//    ctx(ctx)
//{
//
//}
//
//void CaptureVisitor::capture() {
//    for (auto ident : referenced_identifiers) {
//        if (auto parent = ctx->get_parent_ctx().lock()) {
//            if (!parent->declare_variable(ident)) {
//                ctx->set_variable(ident, parent->get_variable(ident));
//            }
//        }
//    }
//}
//
//Variant CaptureVisitor::visit(ModuleDecl *node) {
//    for (auto child : node->get_children()) {
//        child->accept(*this);
//    }
//
//    return {};
//}
//
//Variant CaptureVisitor::visit(CompoundStmt *node) {
//    for (auto child : node->get_children()) {
//        child->accept(*this);
//    }
//
//    return {};
//}
//
//Variant CaptureVisitor::visit(FunctionDecl *node) {
//    for (auto child : node->_args) {
//        child->accept(*this);
//    }
//
//    return {};
//}
//
//Variant CaptureVisitor::visit(IdentifierRefExpr *node) {
//    for (auto child : node->get_children()) {
//        if (std::find(declared_identifiers.begin(), declared_identifiers.end(), node->_ident) == declared_identifiers.end())
//        {
//            referenced_identifiers.emplace(node->_ident);
//        }
//
//        child->accept(*this);
//    }
//
//    return {};
//}
//
//Variant CaptureVisitor::visit(DeclStmt *node) {
//    for (auto child : node->get_children()) {
//        for (int i = 0; i < node->decl_identifiers.size(); ++i) {
//            declared_identifiers.emplace(node->decl_identifiers[i]);
//        }
//        child->accept(*this);
//    }
//
//    return {};
//}
//
//
//Variant CaptureVisitor::visit(ForStmt *node) {
//    for (auto child : node->get_children()) {
//        child->accept(*this);
//    }
//
//    return {};
//}
//
//Variant CaptureVisitor::visit(WhileStmt *node) {
//    for (auto child : node->get_children()) {
//        child->accept(*this);
//    }
//
//    return {};
//}
//
//Variant CaptureVisitor::visit(ArrayLiteral *node) {
//    for (auto child : node->get_children()) {
//        child->accept(*this);
//    }
//
//    return {};
//}
//
//Variant CaptureVisitor::visit(LiteralExpr *node) {
//    for (auto child : node->get_children()) {
//        child->accept(*this);
//    }
//
//    return {};
//}
//
//Variant CaptureVisitor::visit(StringLiteral *node) {
//    for (auto child : node->get_children()) {
//        child->accept(*this);
//    }
//
//    return {};
//}
//
//Variant CaptureVisitor::visit(ArrayAccessExpr *node) {
//    for (auto child : node->get_children()) {
//        child->accept(*this);
//    }
//
//    return {};
//}
//
//Variant CaptureVisitor::visit(CallExpr *node) {
//    for (auto child : node->get_children()) {
//        child->accept(*this);
//    }
//
//    return {};
//}
//
//Variant CaptureVisitor::visit(MemberRefExpr *node) {
//    for (auto child : node->get_children()) {
//        child->accept(*this);
//    }
//
//    return {};
//}
//
//Variant CaptureVisitor::visit(BinaryOperator *node) {
//    for (auto child : node->get_children()) {
//        child->accept(*this);
//    }
//
//    return {};
//}
//
//Variant CaptureVisitor::visit(ExplicitCastExpr *node) {
//    for (auto child : node->get_children()) {
//        child->accept(*this);
//    }
//
//    return {};
//}
//
//Variant CaptureVisitor::visit(TertiaryOperator *node) {
//    for (auto child : node->get_children()) {
//        child->accept(*this);
//    }
//
//    return {};
//}
//
//Variant CaptureVisitor::visit(UnaryOperator *node) {
//    for (auto child : node->get_children()) {
//        child->accept(*this);
//    }
//
//    return {};
//}
//
//Variant CaptureVisitor::visit(BreakStmt *node) {
//    for (auto child : node->get_children()) {
//        child->accept(*this);
//    }
//
//    return {};
//}
//
//Variant CaptureVisitor::visit(ContinueStmt *node) {
//    for (auto child : node->get_children()) {
//        child->accept(*this);
//    }
//
//    return {};
//}
//
//Variant CaptureVisitor::visit(IfStmt *node) {
//    for (auto child : node->get_children()) {
//        child->accept(*this);
//    }
//
//    return {};
//}
//
//Variant CaptureVisitor::visit(SwitchStmt *node) {
//    for (auto child : node->get_children()) {
//        child->accept(*this);
//    }
//
//    return {};
//}
//
//Variant CaptureVisitor::visit(CaseStmt *node) {
//    for (auto child : node->get_children()) {
//        child->accept(*this);
//    }
//
//    return {};
//}
//
//Variant CaptureVisitor::visit(LabelStmt *node) {
//    return {};
//}
//
//Variant CaptureVisitor::visit(GotoStmt *node) {
//    return {};
//}
//
//Variant CaptureVisitor::visit(FuncArgDecl *node) {
//    for (auto child : node->get_children()) {
//        child->accept(*this);
//    }
//
//    return {};
//}
//
//Variant CaptureVisitor::visit(ReturnStmt *node) {
//    for (auto child : node->get_children()) {
//        child->accept(*this);
//    }
//
//    return {};
//}
//
//Variant CaptureVisitor::visit(InputStmt *node) {
//    for (auto child : node->get_children()) {
//        child->accept(*this);
//    }
//
//    return {};
//}
//
//Variant CaptureVisitor::visit(OutputStmt *node) {
//    for (auto child : node->get_children()) {
//        child->accept(*this);
//    }
//
//    return {};
//}
//
//Variant CaptureVisitor::visit(Expression *node) {
//    for (auto child : node->get_children()) {
//        child->accept(*this);
//    }
//
//    return {};
//}
//
//Variant CaptureVisitor::visit(MethodCallExpr *node) {
//    for (auto child : node->get_children()) {
//        child->accept(*this);
//    }
//
//    return {};
//}
//
//Variant CaptureVisitor::visit(FunctionCallExpr *node) {
//    for (auto child : node->get_children()) {
//        child->accept(*this);
//    }
//
//    return {};
//}
//
//Variant CaptureVisitor::visit(ClassDecl *node) {
//    for (auto child : node->get_children()) {
//        child->accept(*this);
//    }
//
//    return {};
//}
//
//Variant CaptureVisitor::visit(ConstrDecl *node) {
//    for (auto child : node->get_children()) {
//        child->accept(*this);
//    }
//
//    return {};
//}
//
//Variant CaptureVisitor::visit(FieldDecl *node) {
//    for (auto child : node->get_children()) {
//        child->accept(*this);
//    }
//
//    return {};
//}
//
//Variant CaptureVisitor::visit(MethodDecl *node) {
//    for (auto child : node->get_children()) {
//        child->accept(*this);
//    }
//
//    return {};
//}
//
//Variant CaptureVisitor::visit(LambdaExpr *node) {
//    for (auto child : node->get_children()) {
//        child->accept(*this);
//    }
//
//    return {};
//}
//
//Variant CaptureVisitor::visit(OperatorDecl *node) {
//    for (auto child : node->get_children()) {
//        child->accept(*this);
//    }
//
//    return {};
//}
//
//Variant CaptureVisitor::visit(ImportStmt *node) {
//    for (auto child : node->get_children()) {
//        child->accept(*this);
//    }
//
//    return {};
//}
//
//Variant CaptureVisitor::visit(ExportStmt *node) {
//    for (auto child : node->get_children()) {
//        child->accept(*this);
//    }
//
//    return {};
//}
//
//Variant CaptureVisitor::visit(StructDecl *node) {
//    for (auto child : node->get_children()) {
//        child->accept(*this);
//    }
//
//    return {};
//}
//
//Variant CaptureVisitor::visit(InterfaceDecl *node) {
//
//}
//
//Variant CaptureVisitor::visit(ImplicitCastExpr *) {
//
//}