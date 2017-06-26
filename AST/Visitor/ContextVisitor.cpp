//
// Created by Jonas Zell on 26.06.17.
//

#include "ContextVisitor.h"
#include "../AstNode.h"
#include "../Statement/CompoundStmt.h"
#include "../Expression/RefExpr/IdentifierRefExpr.h"
#include "../Expression/RefExpr/MemberRefExpr.h"
#include "../Statement/DeclStmt.h"
#include "../Statement/ControlFlow/ForStmt.h"
#include "../Statement/ControlFlow/WhileStmt.h"
#include "../Statement/Function/FunctionDecl.h"
#include "../Expression/Expression.h"
#include "../Expression/Literal/LiteralExpr.h"
#include "../Expression/Literal/ObjectLiteral.h"
#include "../Expression/RefExpr/ArrayAccessExpr.h"
#include "../Expression/RefExpr/CallExpr.h"
#include "../Operator/BinaryOperator.h"
#include "../Operator/UnaryOperator.h"
#include "../Operator/ExplicitCastExpr.h"
#include "../Operator/TertiaryOperator.h"
#include "../Statement/ControlFlow/ContinueStmt.h"
#include "../Statement/ControlFlow/BreakStmt.h"
#include "../Statement/ControlFlow/IfStmt.h"
#include "../Statement/IO/OutputStmt.h"
#include "../Statement/Function/ReturnStmt.h"
#include "../Statement/IO/InputStmt.h"
#include "../Expression/Literal/ArrayLiteral.h"

ContextVisitor::ContextVisitor() :
    context(std::make_shared<Context>()),
    declared_identifiers(std::vector<std::string>()),
    referenced_identifiers(std::vector<std::string>())
{

}

ContextVisitor::ContextVisitor(const ContextVisitor &v) : ContextVisitor() {
    func_context = v.func_context;
    context->set_parent_ctx(v.context);
}

ContextVisitor::ContextVisitor(Context::SharedPtr ctx) : ContextVisitor() {
    context = ctx;
}

Variant ContextVisitor::visit(CompoundStmt *node) {
    if (node->_terminable) {
        func_context = node;
    }

    for (auto child : node->get_children()) {
        child->set_parent(node);
        child->accept(*this);
    }

    context->capture();

    return nullptr;
}

Variant ContextVisitor::visit(FunctionDecl *node) {
    node->set_context(context);
    declared_identifiers.push_back(node->_func_name);
    context->declare_variable(node->_func_name);

    ContextVisitor v(*this);
    node->_body->accept(v);

    for (auto arg : node->_args) {
        arg->accept(*this);
    }

    return nullptr;
}

Variant ContextVisitor::visit(IdentifierRefExpr *node) {
    node->set_context(context);
    if (std::find(declared_identifiers.begin(), declared_identifiers.end(), node->_ident)
        == declared_identifiers.end())
    {
        context->add_capture(node->_ident);
    }

    for (auto child : node->get_children()) {
        child->accept(*this);
    }

    return nullptr;
}

Variant ContextVisitor::visit(DeclStmt *node) {
    node->set_context(context);
    declared_identifiers.push_back(node->_ident);
    context->declare_variable(node->_ident);

    for (auto child : node->get_children()) {
        child->accept(*this);
    }

    return nullptr;
}

Variant ContextVisitor::visit(ForStmt *node) {
    ContextVisitor v(*this);
    v.root_stmt = node;
    v.root_type = "for";
    for (auto child : node->get_children()) {
        child->accept(v);
    }

    return nullptr;
}

Variant ContextVisitor::visit(WhileStmt *node) {
    ContextVisitor v(*this);
    v.root_stmt = node;
    v.root_type = "while";
    node->_condition->accept(*this);
    node->_while_block->accept(v);

    return nullptr;
}

Variant ContextVisitor::visit(ArrayLiteral *node) {
    for (auto child : node->get_children()) {
        child->accept(*this);
    }

    return nullptr;
}

Variant ContextVisitor::visit(LiteralExpr *node) {
    return nullptr;
}

Variant ContextVisitor::visit(ObjectLiteral *node) {
    for (auto prop : node->get_children()) {
        prop->accept(*this);
    }

    return nullptr;
}

Variant ContextVisitor::visit(ObjectPropExpr *node) {
    for (auto child : node->get_children()) {
        child->accept(*this);
    }

    return nullptr;
}

Variant ContextVisitor::visit(ArrayAccessExpr *node) {
    for (auto child : node->get_children()) {
        child->accept(*this);
    }

    return nullptr;
}

Variant ContextVisitor::visit(CallExpr *node) {
    for (auto child : node->get_children()) {
        child->accept(*this);
    }

    return nullptr;
}

Variant ContextVisitor::visit(MemberRefExpr *node) {
    for (auto child : node->get_children()) {
        child->accept(*this);
    }

    return nullptr;
}

Variant ContextVisitor::visit(BinaryOperator *node) {
    if (node->_operator == ":") {
        BinaryOperator::SharedPtr cond_op = std::dynamic_pointer_cast<BinaryOperator>(node->_first_child);
        
        if (cond_op == nullptr || cond_op->_operator != "?") {
            ParseError::raise(ERR_UNEXPECTED_CHARACTER, "Cannot use tertiary operators '?' and ':' alone");
        }

        Expression::SharedPtr cond = cond_op->_first_child;
        Expression::SharedPtr if_br = cond_op->_second_child;
        Expression::SharedPtr else_br = node->_second_child;
    }

    for (auto child : node->get_children()) {
        child->accept(*this);
    }

    return nullptr;
}

Variant ContextVisitor::visit(ExplicitCastExpr *node) {
    for (auto child : node->get_children()) {
        child->accept(*this);
    }

    return nullptr;
}

Variant ContextVisitor::visit(TertiaryOperator *node) {
    for (auto child : node->get_children()) {
        child->accept(*this);
    }

    return nullptr;
}

Variant ContextVisitor::visit(UnaryOperator *node) {
    for (auto child : node->get_children()) {
        child->accept(*this);
    }

    return nullptr;
}

Variant ContextVisitor::visit(BreakStmt *node) {
    node->set_stmt(root_stmt, root_type);

    return nullptr;
}

Variant ContextVisitor::visit(ContinueStmt *node) {
    node->set_stmt(root_stmt, root_type);

    return nullptr;
}

Variant ContextVisitor::visit(IfStmt *node) {
    node->_condition->accept(*this);

    if (node->_if_branch != nullptr) {
        ContextVisitor v(*this);
        node->_if_branch->accept(v);
    }

    if (node->_else_branch != nullptr) {
        ContextVisitor v(*this);
        node->_else_branch->accept(v);
    }

    return nullptr;
}

Variant ContextVisitor::visit(FuncArgDecl *node) {
    for (auto child : node->get_children()) {
        child->accept(*this);
    }

    return nullptr;
}

Variant ContextVisitor::visit(ReturnStmt *node) {
    node->set_function(func_context);
    node->_return_val->accept(*this);

    return nullptr;
}

Variant ContextVisitor::visit(InputStmt *node) {
    for (auto child : node->get_children()) {
        child->accept(*this);
    }

    return nullptr;
}

Variant ContextVisitor::visit(OutputStmt *node) {
    for (auto child : node->get_children()) {
        child->accept(*this);
    }

    return nullptr;
}

Variant ContextVisitor::visit(Expression *node) {
    for (auto child : node->get_children()) {
        child->accept(*this);
    }

    return nullptr;
}

Variant ContextVisitor::visit(MethodCallExpr *node) {

}