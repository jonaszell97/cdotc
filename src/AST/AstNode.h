//
// Created by Jonas Zell on 19.06.17.
//

#ifndef ASTNODE_H
#define ASTNODE_H

#include "lex/SourceLocation.h"
#include "Support/Casting.h"
#include "Type.h"

#include <llvm/Support/TrailingObjects.h>

namespace cdot {

class Attr;
class Type;

enum class AccessModifier : unsigned char;

using string = std::string;

namespace diag {
   class DiagnosticBuilder;
} // namespace diag

namespace ast {

class DeclContext;
class Statement;

class AstNode {
public:

   enum NodeType {
#  define CDOT_ASTNODE(Name)              \
      Name##ID,
#  define CDOT_ABSTRACT(Name)             \
      Name##ID,
#  include "AstNode.def"
   };

   llvm::StringRef getNodeTypeAsString() const
   {
      switch (typeID) {
#        define CDOT_ASTNODE(Name)        \
         case Name##ID:                   \
            return #Name;
#        include "AstNode.def"

      default:
         llvm_unreachable("bad node kind");
      }
   }

   llvm::ArrayRef<Attr*> getAttributes() const;

   template<class T>
   bool hasAttribute() const
   {
      for (auto &Attr : getAttributes())
         if (support::isa<T>(Attr))
            return true;

      return false;
   }

   template<class T>
   T *getAttribute()
   {
      for (auto &Attr : getAttributes())
         if (auto A = support::dyn_cast<T>(Attr))
            return A;

      return nullptr;
   }

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

protected:
   explicit AstNode(NodeType typeID);

#  ifndef NDEBUG
   virtual
#  endif
   ~AstNode();

   NodeType typeID;
   uint32_t SubclassData;

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
   QualType getContextualType()
   {
      return contextualType;
   }
};

} // namespace ast
} // namespace cdot


#endif //ASTNODE_H
