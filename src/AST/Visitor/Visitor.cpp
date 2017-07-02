//
// Created by Jonas Zell on 25.06.17.
//

#include <iostream>
#include "Visitor.h"
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
#include "../Expression/Literal/StringLiteral.h"
#include "../Expression/Literal/ObjectLiteral.h"
#include "../Expression/Literal/LambdaExpr.h"
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
#include "../Statement/ControlFlow/ReturnStmt.h"
#include "../Statement/IO/InputStmt.h"
#include "../Expression/Literal/ArrayLiteral.h"
#include "../Statement/Declaration/Class/ClassDecl.h"
#include "../Statement/Declaration/Class/ConstrDecl.h"
#include "../Statement/Declaration/Class/FieldDecl.h"
#include "../Statement/Declaration/Class/MethodDecl.h"
#include "../Statement/Declaration/Class/OperatorDecl.h"
#include "../Statement/ControlFlow/SwitchStmt.h"
#include "../Statement/ControlFlow/CaseStmt.h"


Visitor::Visitor() {

}

Visitor::Visitor(const Visitor &v) : Visitor() {

}

Variant Visitor::visit(CompoundStmt *node) {

}

Variant Visitor::visit(FunctionDecl *node) {

}

Variant Visitor::visit(IdentifierRefExpr *node) {

}

Variant Visitor::visit(DeclStmt *node) {

}

Variant Visitor::visit(ForStmt *node) {

}

Variant Visitor::visit(WhileStmt *node) {

}

Variant Visitor::visit(ArrayLiteral *node) {

}

Variant Visitor::visit(LiteralExpr *node) {

}

Variant Visitor::visit(StringLiteral *node) {

}

Variant Visitor::visit(ObjectLiteral *node) {

}

Variant Visitor::visit(ObjectPropExpr *node) {

}

Variant Visitor::visit(ArrayAccessExpr *node) {

}

Variant Visitor::visit(MethodCallExpr *node) {

}

Variant Visitor::visit(CallExpr *node) {

}

Variant Visitor::visit(MemberRefExpr *node) {

}

Variant Visitor::visit(BinaryOperator *node) {

}

Variant Visitor::visit(ExplicitCastExpr *node) {

}

Variant Visitor::visit(TertiaryOperator *node) {

}

Variant Visitor::visit(UnaryOperator *node) {

}

Variant Visitor::visit(BreakStmt *node) {

}

Variant Visitor::visit(ContinueStmt *node) {

}

Variant Visitor::visit(IfStmt *node) {

}

Variant Visitor::visit(SwitchStmt *node) {

}

Variant Visitor::visit(CaseStmt *node) {

}

Variant Visitor::visit(LabelStmt *node) {

}

Variant Visitor::visit(GotoStmt *node) {

}

Variant Visitor::visit(FuncArgDecl *node) {

}

Variant Visitor::visit(ReturnStmt *node) {

}

Variant Visitor::visit(InputStmt *node) {

}

Variant Visitor::visit(OutputStmt *node) {

}

Variant Visitor::visit(Expression *node) {

}

Variant Visitor::visit(InstantiationExpr *node) {

}

Variant Visitor::visit(ClassDecl *node) {

}

Variant Visitor::visit(ConstrDecl *node) {

}

Variant Visitor::visit(FieldDecl *node) {

}

Variant Visitor::visit(MethodDecl *node) {

}

Variant Visitor::visit(LambdaExpr *node) {

}

Variant Visitor::visit(OperatorDecl *node) {

}