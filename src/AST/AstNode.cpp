//
// Created by Jonas Zell on 19.06.17.
//

#include "Statement.h"

#include "AST/Attribute/Attr.h"
#include "AST/AstNode.h"
#include "NamedDecl.h"

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

   if (auto S = dyn_cast<StaticExpr>(this)) {
      S->getExpr()->setContextualType(ty);
   }
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
