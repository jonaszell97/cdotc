//
// Created by Jonas Zell on 25.06.17.
//

#include <iostream>
#include "Visitor.h"
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

Visitor::Visitor() :
    context(std::make_shared<Context>()),
    declared_identifiers(std::vector<std::string>()),
    referenced_identifiers(std::vector<std::string>())
{

}

Visitor::Visitor(const Visitor &v) : Visitor() {
    func_context = v.func_context;
    context->set_parent_ctx(v.context);
}

Visitor::Visitor(Context::SharedPtr ctx) : Visitor() {
    context = ctx;
}

void Visitor::accept(CompoundStmt *node, VisitorFlag f) {
    if (f == VisitorFlag::LINK_TREE) {
        if (node->_terminable) {
            func_context = node;
        }

        for (auto child : node->get_children()) {
            child->set_parent(node);
            child->visit(*this, f);
        }

        context->capture();
    }
}

void Visitor::accept(FunctionDecl *node, VisitorFlag f) {
    if (f == VisitorFlag::LINK_TREE) {
        node->set_context(context);
        declared_identifiers.push_back(node->_func_name);
        context->declare_variable(node->_func_name);

        Visitor v(*this);
        node->_body->visit(v, f);

        for (auto arg : node->_args) {
            arg->visit(*this, f);
        }
    }
}

void Visitor::accept(IdentifierRefExpr *node, VisitorFlag f) {
    if (f == VisitorFlag::LINK_TREE) {
        node->set_context(context);
        if (std::find(declared_identifiers.begin(), declared_identifiers.end(), node->_ident)
            == declared_identifiers.end())
        {
            context->add_capture(node->_ident);
        }

        for (auto child : node->get_children()) {
            child->visit(*this, f);
        }
    }
}

void Visitor::accept(DeclStmt *node, VisitorFlag f) {
    if (f == VisitorFlag::LINK_TREE) {
        node->set_context(context);
        declared_identifiers.push_back(node->_ident);
        context->declare_variable(node->_ident);

        for (auto child : node->get_children()) {
            child->visit(*this, f);
        }
    }
}

void Visitor::accept(ForStmt *node, VisitorFlag f) {
    if (f == VisitorFlag::LINK_TREE) {
        Visitor v(*this);
        v.root_stmt = node;
        v.root_type = "for";
        for (auto child : node->get_children()) {
            child->visit(v, f);
        }
    }
}

void Visitor::accept(WhileStmt *node, VisitorFlag f) {
    if (f == VisitorFlag::LINK_TREE) {
        Visitor v(*this);
        v.root_stmt = node;
        v.root_type = "while";
        node->_condition->visit(*this, f);
        node->_while_block->visit(v, f);
    }
}

void Visitor::accept(ArrayLiteral *node, VisitorFlag f) {
    if (f == VisitorFlag::LINK_TREE) {
        for (auto child : node->get_children()) {
            child->visit(*this, f);
        }
    }
}

void Visitor::accept(LiteralExpr *node, VisitorFlag f) {
    if (f == VisitorFlag::LINK_TREE) {

    }
}

void Visitor::accept(ObjectLiteral *node, VisitorFlag f) {
    if (f == VisitorFlag::LINK_TREE) {
        for (auto prop : node->get_children()) {
            prop->visit(*this, f);
        }
    }
}

void Visitor::accept(ObjectPropExpr *node, VisitorFlag f) {
    if (f == VisitorFlag::LINK_TREE) {
        for (auto child : node->get_children()) {
            child->visit(*this, f);
        }
    }
}

void Visitor::accept(ArrayAccessExpr *node, VisitorFlag f) {
    if (f == VisitorFlag::LINK_TREE) {
        for (auto child : node->get_children()) {
            child->visit(*this, f);
        }
    }
}

void Visitor::accept(CallExpr *node, VisitorFlag f) {
    if (f == VisitorFlag::LINK_TREE) {
        for (auto child : node->get_children()) {
            child->visit(*this, f);
        }
    }
}

void Visitor::accept(MemberRefExpr *node, VisitorFlag f) {
    if (f == VisitorFlag::LINK_TREE) {
        for (auto child : node->get_children()) {
            child->visit(*this, f);
        }
    }
}

void Visitor::accept(BinaryOperator *node, VisitorFlag f) {
    if (f == VisitorFlag::LINK_TREE) {
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
            child->visit(*this, f);
        }
    }
}

void Visitor::accept(ExplicitCastExpr *node, VisitorFlag f) {
    if (f == VisitorFlag::LINK_TREE) {
        for (auto child : node->get_children()) {
            child->visit(*this, f);
        }
    }
}

void Visitor::accept(TertiaryOperator *node, VisitorFlag f) {
    if (f == VisitorFlag::LINK_TREE) {
        for (auto child : node->get_children()) {
            child->visit(*this, f);
        }
    }
}

void Visitor::accept(UnaryOperator *node, VisitorFlag f) {
    if (f == VisitorFlag::LINK_TREE) {
        for (auto child : node->get_children()) {
            child->visit(*this, f);
        }
    }
}

void Visitor::accept(BreakStmt *node, VisitorFlag f) {
    if (f == VisitorFlag::LINK_TREE) {
        node->set_stmt(root_stmt, root_type);
    }
}

void Visitor::accept(ContinueStmt *node, VisitorFlag f) {
    if (f == VisitorFlag::LINK_TREE) {
        node->set_stmt(root_stmt, root_type);
    }
}

void Visitor::accept(IfStmt *node, VisitorFlag f) {
    if (f == VisitorFlag::LINK_TREE) {
        node->_condition->visit(*this, f);

        if (node->_if_branch != nullptr) {
            Visitor v(*this);
            node->_if_branch->visit(v, f);
        }

        if (node->_else_branch != nullptr) {
            Visitor v(*this);
            node->_else_branch->visit(v, f);
        }
    }
}

void Visitor::accept(FuncArgDecl *node, VisitorFlag f) {
    if (f == VisitorFlag::LINK_TREE) {
        for (auto child : node->get_children()) {
            child->visit(*this, f);
        }
    }
}

void Visitor::accept(ReturnStmt *node, VisitorFlag f) {
    if (f == VisitorFlag::LINK_TREE) {
        node->set_function(func_context);
        node->_return_val->visit(*this, f);
    }
}

void Visitor::accept(InputStmt *node, VisitorFlag f) {
    if (f == VisitorFlag::LINK_TREE) {
        for (auto child : node->get_children()) {
            child->visit(*this, f);
        }
    }
}

void Visitor::accept(OutputStmt *node, VisitorFlag f) {
    if (f == VisitorFlag::LINK_TREE) {
        for (auto child : node->get_children()) {
            child->visit(*this, f);
        }
    }
}

void Visitor::accept(Expression *node, VisitorFlag f) {
    if (f == VisitorFlag::LINK_TREE) {
        for (auto child : node->get_children()) {
            child->visit(*this, f);
        }
    }
}