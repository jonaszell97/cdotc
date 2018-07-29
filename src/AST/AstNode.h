//
// Created by Jonas Zell on 19.06.17.
//

#ifndef ASTNODE_H
#define ASTNODE_H

#include "Lex/SourceLocation.h"
#include "Support/Casting.h"
#include "Type.h"

#include <llvm/Support/TrailingObjects.h>

namespace cdot {

class Attr;
class Type;

enum class AccessSpecifier : unsigned char;

using string = std::string;

namespace diag {
   class DiagnosticBuilder;
} // namespace diag

namespace ast {

class DeclContext;
class Statement;

class AstNode {
public:
   enum NodeType : uint8_t {
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
   unsigned SubclassData : 8;
   QualType contextualType;

   void setFlag(unsigned flag, bool set)
   {
      if (set)
         SubclassData |= flag;
      else
         SubclassData &= ~flag;
   }

   bool flagIsSet(unsigned flag) const
   {
      return (SubclassData & flag) != 0;
   }

public:
   unsigned getSubclassData() const { return SubclassData; }
   void setSubclassData(unsigned D) { SubclassData = D; }

   QualType getContextualType() { return contextualType; }
};

} // namespace ast
} // namespace cdot


#endif //ASTNODE_H
