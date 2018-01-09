//
// Created by Jonas Zell on 19.06.17.
//

#include "AstNode.h"
#include "Attribute/Attribute.h"
#include "../Variant/Type/Generic.h"

#include "Passes/ASTIncludes.h"

namespace cdot {
namespace ast {

AstNode::AstNode(NodeType typeID) : typeID(typeID), SubclassData(0) { }

AstNode::~AstNode()
{
   destroyValue();
}

void AstNode::destroyValue()
{
   switch (typeID) {
#  define CDOT_ASTNODE(Name)                                   \
      case Name##ID:                                           \
         static_cast<Name*>(this)->destroyValueImpl(); return;
#  include "AstNode.def"

      default:
         llvm_unreachable("bad ASTNode kind");
   }
}

void AstNode::destroyValueImpl()
{

}

void AstNode::setAttributes(std::vector<Attribute> &&attr)
{
   attributes = move(attr);
}

std::vector<Attribute>& AstNode::getAttributes()
{
   return attributes;
}

bool AstNode::hasAttribute(Attr kind) const
{
   for (const auto& attr : attributes) {
      if (attr.kind == kind) {
         return true;
      }
   }

   return false;
}

Attribute& AstNode::getAttribute(Attr kind)
{
   for (auto& attr : attributes) {
      if (attr.kind == kind) {
         return attr;
      }
   }

   assert(false && "Call hasAttribute first");
   llvm_unreachable("see above");
}

const SourceLocation &AstNode::getSourceLoc() const
{
   return loc;
}

void AstNode::setSourceLoc(const SourceLocation &loc)
{
   AstNode::loc = loc;
}

AstNode *AstNode::getParent() const
{
   return parent;
}

void AstNode::setParent(AstNode *parent)
{
   AstNode::parent = parent;
}

const QualType &AstNode::getContextualType() const
{
   return contextualType;
}
const string &AstNode::getBinding() const
{
   return binding;
}

void AstNode::setBinding(const string &binding)
{
   AstNode::binding = binding;
}

} // namespace ast
} // namespace cdot
