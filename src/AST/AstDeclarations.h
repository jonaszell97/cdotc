//
// Created by Jonas Zell on 10.10.17.
//

namespace cdot {
namespace ast {

class AstNode;
class CompoundStmt;

class NullStmt;

class IdentifierRefExpr;
class DeclStmt;

class SubscriptExpr;
class CallExpr;
class MemberRefExpr;

class ExplicitCastExpr;
class ImplicitCastExpr;
class TertiaryOperator;
class UnaryOperator;

class Expression;
class Statement;

class NamespaceDecl;
class UsingStmt;
class EndOfFileStmt;
class LvalueToRvalue;
class DebugStmt;

class ForStmt;
class WhileStmt;
class BreakStmt;
class ContinueStmt;
class IfStmt;
class ForInStmt;
class ReturnStmt;
class MatchStmt;
class CaseStmt;
class LabelStmt;
class GotoStmt;

class NoneLiteral;
class StringInterpolation;
class CollectionLiteral;
class IntegerLiteral;
class FPLiteral;
class BoolLiteral;
class CharLiteral;
class StringLiteral;
class LambdaExpr;
class TupleLiteral;

class TypedefDecl;
class TypeRef;

class DeclareStmt;
class ExtendStmt;
class ClassDecl;
class ExtensionDecl;
class FieldDecl;
class MethodDecl;
class ConstrDecl;
class DestrDecl;
class PropDecl;

class EnumDecl;
class EnumCaseDecl;

class UnionDecl;

class CallableDecl;
class FunctionDecl;
class FuncArgDecl;

class TryStmt;
class ThrowStmt;

class RecordTemplateDecl;
class CallableTemplateDecl;
class MethodTemplateDecl;

class BinaryOperator;
class InheritanceConstraint;
class ConformanceConstraint;
class ValueExprConstraint;
class TokenEqualityConstraint;

} // namespace ast
} // namespace cdot