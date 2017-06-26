//
// Created by Jonas Zell on 25.06.17.
//

#ifndef CDOT_VISITOR_H
#define CDOT_VISITOR_H

#include <vector>
#include "../Context.h"

enum class VisitorFlag {
    NONE,
    LINK_TREE
};

class AstNode;
class InputStmt;
class FunctionDecl;
class CompoundStmt;
class IdentifierRefExpr;
class DeclStmt;
class ForStmt;
class WhileStmt;
class ArrayLiteral;
class LiteralExpr;
class ObjectLiteral;
class ObjectPropExpr;
class ArrayAccessExpr;
class CallExpr;
class MemberRefExpr;
class BinaryOperator;
class ExplicitCastExpr;
class TertiaryOperator;
class UnaryOperator;
class BreakStmt;
class ContinueStmt;
class IfStmt;
class FuncArgDecl;
class ReturnStmt;
class InputStmt;
class OutputStmt;
class Expression;
class Statement;

class Visitor {
public:
    Visitor();
    Visitor(Context::SharedPtr);
    Visitor(const Visitor&);

    inline Context::SharedPtr get_context() {
        return context;
    }

    void accept(FunctionDecl*, VisitorFlag);
    void accept(CompoundStmt*, VisitorFlag);
    void accept(IdentifierRefExpr*, VisitorFlag);
    void accept(DeclStmt*, VisitorFlag);
    void accept(ForStmt*, VisitorFlag);
    void accept(WhileStmt*, VisitorFlag);

    void accept(ArrayLiteral*, VisitorFlag);
    void accept(LiteralExpr*, VisitorFlag);
    void accept(ObjectLiteral*, VisitorFlag);
    void accept(ObjectPropExpr*, VisitorFlag);
    void accept(ArrayAccessExpr*, VisitorFlag);
    void accept(CallExpr*, VisitorFlag);
    void accept(MemberRefExpr*, VisitorFlag);
    void accept(BinaryOperator*, VisitorFlag);
    void accept(ExplicitCastExpr*, VisitorFlag);
    void accept(TertiaryOperator*, VisitorFlag);
    void accept(UnaryOperator*, VisitorFlag);
    void accept(BreakStmt*, VisitorFlag);
    void accept(ContinueStmt*, VisitorFlag);
    void accept(IfStmt*, VisitorFlag);
    void accept(FuncArgDecl*, VisitorFlag);
    void accept(ReturnStmt*, VisitorFlag);
    void accept(InputStmt*, VisitorFlag);
    void accept(OutputStmt*, VisitorFlag);
    void accept(Expression*, VisitorFlag);


protected:
    Context::SharedPtr context;
    Statement *root_stmt;
    std::string root_type;
    CompoundStmt* func_context;
    std::vector<std::string> declared_identifiers;
    std::vector<std::string> referenced_identifiers;
};


#endif //CDOT_VISITOR_H
