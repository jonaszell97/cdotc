//
// Created by Jonas Zell on 26.06.17.
//

#include "ContextVisitor.h"
#include "../AstNode.h"
#include "../Statement/CompoundStmt.h"
#include "../Expression/RefExpr/IdentifierRefExpr.h"
#include "../Expression/RefExpr/MemberRefExpr.h"
#include "../Statement/Declaration/DeclStmt.h"
#include "../Statement/ControlFlow/ForStmt.h"
#include "../Statement/ControlFlow/WhileStmt.h"
#include "../Statement/Declaration/FunctionDecl.h"
#include "../Expression/Expression.h"
#include "../Expression/Literal/LiteralExpr.h"
#include "../Expression/Literal/ObjectLiteral.h"
#include "../Expression/RefExpr/ArrayAccessExpr.h"
#include "../Expression/RefExpr/MethodCallExpr.h"
#include "../Expression/Class/InstantiationExpr.h"
#include "../Expression/RefExpr/CallExpr.h"
#include "../Operator/BinaryOperator.h"
#include "../Operator/UnaryOperator.h"
#include "../Operator/ExplicitCastExpr.h"
#include "../Operator/TertiaryOperator.h"
#include "../Statement/ControlFlow/ContinueStmt.h"
#include "../Statement/ControlFlow/BreakStmt.h"
#include "../Statement/ControlFlow/IfStmt.h"
#include "../Statement/IO/OutputStmt.h"
#include "../Statement/ControlFlow/ReturnStmt.h"
#include "../Statement/IO/InputStmt.h"
#include "../Statement/Declaration/Class/ClassDecl.h"
#include "../Statement/Declaration/Class/ConstrDecl.h"
#include "../Statement/Declaration/Class/FieldDecl.h"
#include "../Statement/Declaration/Class/MethodDecl.h"
#include "../Expression/Literal/ArrayLiteral.h"

ContextVisitor::ContextVisitor(bool root) :
    declared_identifiers(std::vector<std::string>()),
    referenced_identifiers(std::vector<std::string>()),
    is_progr_root(root)
{

}

ContextVisitor::ContextVisitor(const ContextVisitor &v) : ContextVisitor() {

}

Variant ContextVisitor::visit(CompoundStmt *node) {
    // program root
    if (is_progr_root) {
        root = node;
        is_progr_root = false;
        for (auto child : node->get_children()) {
            child->set_parent(node);
            child->set_root(root);
            child->accept(*this);
        }
    }
    // every compound statement has its own context
    else {
        ContextVisitor cv;
        cv.root = node;
        node->context->set_parent_ctx(root->context);
        for (auto child : node->get_children()) {
            child->set_parent(node);
            child->set_root(root);
            child->accept(cv);
        }
    }

    return nullptr;
}

Variant ContextVisitor::visit(FunctionDecl *node) {
    // don't visit body, will have to revisit later anyway
    for (auto child : node->_args) {
        child->set_parent(node);
        child->set_root(root);
        child->accept(*this);
    }

    return nullptr;
}

Variant ContextVisitor::visit(IdentifierRefExpr *node) {
    for (auto child : node->get_children()) {
        child->set_root(root);
        child->accept(*this);
    }

    return nullptr;
}

Variant ContextVisitor::visit(DeclStmt *node) {
    for (auto child : node->get_children()) {
        child->set_root(root);
        child->accept(*this);
    }

    return nullptr;
}

Variant ContextVisitor::visit(ForStmt *node) {
    for (auto child : node->get_children()) {
        child->set_root(root);
        child->accept(*this);
    }

    return nullptr;
}

Variant ContextVisitor::visit(WhileStmt *node) {
    for (auto child : node->get_children()) {
        child->set_root(root);
        child->accept(*this);
    }

    return nullptr;
}

Variant ContextVisitor::visit(ArrayLiteral *node) {
    for (auto child : node->get_children()) {
        child->set_root(root);
        child->accept(*this);
    }

    return nullptr;
}

Variant ContextVisitor::visit(LiteralExpr *node) {
    for (auto child : node->get_children()) {
        child->set_root(root);
        child->accept(*this);
    }

    return nullptr;
}

Variant ContextVisitor::visit(ObjectLiteral *node) {
    for (auto child : node->get_children()) {
        child->set_root(root);
        child->accept(*this);
    }

    return nullptr;
}

Variant ContextVisitor::visit(ObjectPropExpr *node) {
    for (auto child : node->get_children()) {
        child->set_root(root);
        child->accept(*this);
    }

    return nullptr;
}

Variant ContextVisitor::visit(ArrayAccessExpr *node) {
    for (auto child : node->get_children()) {
        child->set_root(root);
        child->accept(*this);
    }

    return nullptr;
}

Variant ContextVisitor::visit(CallExpr *node) {
    for (auto child : node->get_children()) {
        child->set_root(root);
        child->accept(*this);
    }

    return nullptr;
}

Variant ContextVisitor::visit(MemberRefExpr *node) {
    for (auto child : node->get_children()) {
        child->set_root(root);
        child->accept(*this);
    }

    return nullptr;
}

Variant ContextVisitor::visit(BinaryOperator *node) {
    for (auto child : node->get_children()) {
        child->set_root(root);
        child->accept(*this);
    }

    return nullptr;
}

Variant ContextVisitor::visit(ExplicitCastExpr *node) {
    for (auto child : node->get_children()) {
        child->set_root(root);
        child->accept(*this);
    }

    return nullptr;
}

Variant ContextVisitor::visit(TertiaryOperator *node) {
    for (auto child : node->get_children()) {
        child->set_root(root);
        child->accept(*this);
    }

    return nullptr;
}

Variant ContextVisitor::visit(UnaryOperator *node) {
    for (auto child : node->get_children()) {
        child->set_root(root);
        child->accept(*this);
    }

    return nullptr;
}

Variant ContextVisitor::visit(BreakStmt *node) {
    for (auto child : node->get_children()) {
        child->set_root(root);
        child->accept(*this);
    }

    return nullptr;
}

Variant ContextVisitor::visit(ContinueStmt *node) {
    for (auto child : node->get_children()) {
        child->set_root(root);
        child->accept(*this);
    }

    return nullptr;
}

Variant ContextVisitor::visit(IfStmt *node) {
    for (auto child : node->get_children()) {
        child->set_root(root);
        child->accept(*this);
    }

    return nullptr;
}

Variant ContextVisitor::visit(FuncArgDecl *node) {
    for (auto child : node->get_children()) {
        child->set_root(root);
        child->accept(*this);
    }

    return nullptr;
}

Variant ContextVisitor::visit(ReturnStmt *node) {
    for (auto child : node->get_children()) {
        child->set_root(root);
        child->accept(*this);
    }

    return nullptr;
}

Variant ContextVisitor::visit(InputStmt *node) {
    for (auto child : node->get_children()) {
        child->set_root(root);
        child->accept(*this);
    }

    return nullptr;
}

Variant ContextVisitor::visit(OutputStmt *node) {
    for (auto child : node->get_children()) {
        child->set_root(root);
        child->accept(*this);
    }

    return nullptr;
}

Variant ContextVisitor::visit(Expression *node) {
    for (auto child : node->get_children()) {
        child->set_root(root);
        child->accept(*this);
    }

    return nullptr;
}

Variant ContextVisitor::visit(MethodCallExpr *node) {
    for (auto child : node->get_children()) {
        child->set_root(root);
        child->accept(*this);
    }

    return nullptr;
}

Variant ContextVisitor::visit(InstantiationExpr *node) {
    for (auto child : node->get_children()) {
        child->set_root(root);
        child->accept(*this);
    }

    return nullptr;
}

Variant ContextVisitor::visit(ClassDecl *node) {
    for (auto child : node->get_children()) {
        child->set_root(root);
        child->accept(*this);
    }

    return nullptr;
}

Variant ContextVisitor::visit(ConstrDecl *node) {
    for (auto child : node->get_children()) {
        child->set_root(root);
        child->accept(*this);
    }

    return nullptr;
}

Variant ContextVisitor::visit(FieldDecl *node) {
    for (auto child : node->get_children()) {
        child->set_root(root);
        child->accept(*this);
    }

    return nullptr;
}

Variant ContextVisitor::visit(MethodDecl *node) {
    for (auto child : node->get_children()) {
        child->set_root(root);
        child->accept(*this);
    }

    return nullptr;
}