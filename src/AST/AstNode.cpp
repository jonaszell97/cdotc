//
// Created by Jonas Zell on 19.06.17.
//

#include "AstNode.h"
#include "Attribute/Attribute.h"

namespace cdot {
namespace ast {

void AstNode::setAttributes(std::vector<Attribute> &&attr)
{
   attributes = move(attr);
}

std::vector<Attribute>& AstNode::getAttributes()
{
   return attributes;
}

void AstNode::setSourceLoc(size_t start, size_t end,
                           size_t line, size_t source) {
   loc = SourceLocation(start, line, end - start, source);
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
