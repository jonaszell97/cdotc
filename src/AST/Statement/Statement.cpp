
#include "Statement.h"

namespace cdot {
namespace ast {

Statement::Statement(NodeType typeID) : AstNode(typeID) {}

Statement::~Statement() = default;

} // namespace ast
} // namespace cdot
