//
// Created by Jonas Zell on 19.06.17.
//

#include "Expression.h"
#include "Statement.h"

using namespace cdot::support;

namespace cdot {
namespace ast {

AstNode::AstNode(NodeType typeID) : typeID(typeID), SubclassData(0) { }

AstNode::~AstNode()
{

}

void AstNode::setContextualType(QualType ty)
{
   contextualType = ty;
}

llvm::ArrayRef<Attr*> AstNode::getAttributes() const
{
   if (auto AttrStmt = dyn_cast<AttributedStmt>(this))
      return AttrStmt->getAttributes();

   if (auto AttrExpr = dyn_cast<AttributedExpr>(this))
      return AttrExpr->getAttributes();

   return {};
}

} // namespace ast
} // namespace cdot
