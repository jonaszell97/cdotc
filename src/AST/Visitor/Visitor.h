//
// Created by Jonas Zell on 25.06.17.
//

#ifndef CDOT_VISITOR_H
#define CDOT_VISITOR_H

class AstNode;
class FunctionDecl;
class CompoundStmt;
class IdentifierRefExpr;
class DeclStmt;
class ForStmt;
class ForInStmt;
class WhileStmt;
class CollectionLiteral;
class NumericLiteral;
class SubscriptExpr;
class CallExpr;
class MemberRefExpr;
class TertiaryOperator;
class UnaryOperator;
class BreakStmt;
class ContinueStmt;
class IfStmt;
class FuncArgDecl;
class ReturnStmt;
class Expression;
class Statement;
class ClassDecl;
class FieldDecl;
class MethodDecl;
class ConstrDecl;
class DestrDecl;
class MatchStmt;
class CaseStmt;
class LabelStmt;
class GotoStmt;
class StringLiteral;
class LambdaExpr;
class NamespaceDecl;
class UsingStmt;
class EndOfFileStmt;
class ImplicitCastExpr;
class TypedefDecl;
class TypeRef;
class DeclareStmt;
class LvalueToRvalue;
class DebugStmt;
class TupleLiteral;
class EnumDecl;
class Statement;
class NoneLiteral;
class StringInterpolation;

namespace cdot {
   class BinaryOperator;
}

#endif //CDOT_VISITOR_H
