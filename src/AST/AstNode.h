//
// Created by Jonas Zell on 19.06.17.
//

#ifndef ASTNODE_H
#define ASTNODE_H

#include <iostream>

#include "Attribute/Attribute.h"
#include "../Variant/Type/Type.h"
#include "../lex/SourceLocation.h"
#include "../Support/Casting.h"

namespace cdot {

class Type;
class GenericType;
enum class AccessModifier : unsigned char;

using string = std::string;

namespace diag {
   class DiagnosticBuilder;
} // namespace diag

namespace ast {

class DeclContext;
class TransformImpl;
class Statement;

class AstNode {
public:
   friend class TransformImpl;

   enum NodeType {
#  define CDOT_ASTNODE(Name) \
      Name##ID,
#  define CDOT_INCLUDE_ALL
#  include "AstNode.def"
   };

   std::string getNodeTypeAsString() const
   {
      switch (typeID) {
#        define CDOT_ASTNODE(Name)  \
         case Name##ID:             \
            return #Name;
#        include "AstNode.def"
      }
   }

   void setAttributes(std::vector<Attribute> &&attr);
   std::vector<Attribute>& getAttributes();

   std::vector<Attribute> const& getAttributes() const
   { return attributes; }

   bool hasAttribute(Attr kind) const;
   Attribute& getAttribute(Attr kind);

   void setContextualType(QualType ty);

   bool needsContextualInformation() const
   {
      return false;
   }

   NodeType getTypeID() const
   {
      return typeID;
   }

   static bool classof(AstNode const* T)     { return true; }
   static bool classof(DeclContext const* T) { return false; }

   static DeclContext *castToDeclContext(AstNode const *D);
   static AstNode *castFromDeclContext(DeclContext const *Ctx);

protected:
   explicit AstNode(NodeType typeID);

#  ifndef NDEBUG
   virtual
#  endif
   ~AstNode();

   NodeType typeID;
   uint32_t SubclassData;

   SourceLocation loc;
   std::vector<Attribute> attributes;
   QualType contextualType;

   void toggleFlag(uint32_t flag)
   {
      SubclassData ^= flag;
   }

   void setFlag(uint32_t flag, bool set)
   {
      if (set)
         SubclassData |= flag;
      else
         SubclassData &= ~flag;
   }

   bool flagIsSet(uint32_t flag) const
   {
      return (SubclassData & flag) != 0;
   }

public:
   uint32_t getSubclassData() const
   {
      return SubclassData;
   }

public:
   const SourceLocation &getSourceLoc() const { return loc; }
   void setSourceLoc(const SourceLocation &loc) { this->loc = loc; }

   QualType getContextualType()
   {
      return contextualType;
   }
};

} // namespace ast
} // namespace cdot


#endif //ASTNODE_H
