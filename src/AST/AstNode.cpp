//
// Created by Jonas Zell on 19.06.17.
//

#include "AST/Attribute/Attribute.h"

#include "AST/Expression/StaticExpr.h"

#include "AST/Statement/Declaration/Class/RecordDecl.h"
#include "AST/Statement/Declaration/Class/MethodDecl.h"
#include "AST/Statement/Declaration/CallableDecl.h"
#include "AST/Statement/Declaration/TypedefDecl.h"
#include "AST/Statement/Declaration/NamespaceDecl.h"
#include "AST/Statement/Block/CompoundStmt.h"

using namespace cdot::support;

namespace cdot {
namespace ast {

AstNode::AstNode(NodeType typeID) : typeID(typeID), SubclassData(0) { }

AstNode::~AstNode()
{

}

DeclContext* AstNode::castToDeclContext(const AstNode *D)
{
   switch (D->getTypeID()) {
#  define CDOT_DECL_CONTEXT(Name)                               \
      case Name##ID:                                            \
         return static_cast<Name*>(const_cast<AstNode*>(D));
#  include "AstNode.def"

      default:
         llvm_unreachable("not a named decl");
   }
}

AstNode* AstNode::castFromDeclContext(const DeclContext *Ctx)
{
   switch (Ctx->getDeclKind()) {
#  define CDOT_DECL_CONTEXT(Name)                                   \
      case Name##ID:                                   \
         return static_cast<Name*>(const_cast<DeclContext*>(Ctx));
#  include "AstNode.def"

      default:
         llvm_unreachable("not a named decl");
   }
}

void AstNode::setContextualType(QualType ty)
{
   contextualType = ty;

   if (auto S = dyn_cast<StaticExpr>(this)) {
      S->getExpr()->setContextualType(ty);
   }
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

   llvm_unreachable("Call hasAttribute first");
}

} // namespace ast
} // namespace cdot
