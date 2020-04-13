namespace cdot {
namespace ast {

#define CDOT_ASTNODE(Name) class Name;
#include "cdotc/AST/AstNode.def"

#define CDOT_DECL(Name) class Name;
#include "cdotc/AST/Decl.def"

class Decl;
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

} // namespace ast
} // namespace cdot