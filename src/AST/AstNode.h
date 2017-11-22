//
// Created by Jonas Zell on 19.06.17.
//

#ifndef ASTNODE_H
#define ASTNODE_H

#include <iostream>

#include "Attribute/Attribute.h"
#include "../Variant/Type/QualType.h"
#include "../lex/SourceLocation.h"

namespace cdot {

class Type;
class GenericType;

class Callable;

class TemplateArgList;
struct TemplateConstraint;

enum class AccessModifier : unsigned int;

namespace cl {

struct Method;
class Class;
class Record;
class Enum;
class Union;

} // namespace cl

namespace ast {

using cdot::Attribute;
using namespace cdot;

class AstNode {
public:
   typedef std::shared_ptr<AstNode> SharedPtr;

   enum NodeType {
#  define CDOT_ASTNODE(Name) \
      Name##ID,
#  define CDOT_INCLUDE_ALL
#  include "AstNode.def"
   };

   void setSourceLoc(size_t start,
                     size_t end,
                     size_t line,
                     size_t source);

   void setAttributes(std::vector<Attribute> &&attr);
   std::vector<Attribute>& getAttributes();

   bool hasAttribute(Attr kind) const;
   Attribute& getAttribute(Attr kind);

   void setContextualType(const QualType& t)
   {
      contextualType = t;
   }

   bool needsContextualInformation() const
   {
      return false;
   }

   NodeType getTypeID() const
   {
      return typeID;
   }

   static bool classof(AstNode const* T)
   {
      return true;
   }

protected:
   explicit AstNode(NodeType typeID) : typeID(typeID)
   {

   }

   NodeType typeID;

   SourceLocation loc;
   AstNode* parent = nullptr;
   std::vector<Attribute> attributes;
   QualType contextualType;

   // codegen
   string binding;

public:
   const SourceLocation &getSourceLoc() const;
   void setSourceLoc(const SourceLocation &loc);

   AstNode *getParent() const;
   void setParent(AstNode *parent);

   const QualType &getContextualType() const;

   const string &getBinding() const;
   void setBinding(const string &binding);
};

} // namespace ast
} // namespace cdot


#endif //ASTNODE_H
