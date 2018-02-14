//
// Created by Jonas Zell on 10.10.17.
//

namespace cdot {
namespace ast {

#define CDOT_INCLUDE_ALL
#define CDOT_ASTNODE(Name) class Name;

#include "AstNode.def"

class RecordDecl;
class NamedDecl;
class CallableDecl;

class DeclContext;

class Statement;
class Expression;
class AstNode;

class StaticStmt;
class VarDecl;
class DestructuringDecl;
class PatternExpr;

class InheritanceConstraint;
class ConformanceConstraint;
class ValueExprConstraint;
class TokenEqualityConstraint;

} // namespace ast
} // namespace cdot