//
// Created by Jonas Zell on 19.06.17.
//

#ifndef STATEMENT_H
#define STATEMENT_H

#include "../AstNode.h"

namespace cdot {

namespace module {
   class Module;
} // namespace module

namespace ast {

class Expression;

class Statement: public AstNode {
public:
   enum Flags: uint32_t {
      TypeDependant     = 1,
      ValueDependant    = TypeDependant     << 1,
      GlobalInitializer = ValueDependant    << 1,
      HadError          = GlobalInitializer << 1,
      CTFEDependant     = HadError          << 1,
   };

   friend class TransformImpl;

   bool isDependent() const
   {
      return isTypeDependent() || isValueDependent();
   }

   bool isTypeDependent() const
   {
      return flagIsSet(TypeDependant);
   }

   bool isValueDependent() const
   {
      return flagIsSet(ValueDependant);
   }

   void setIsTypeDependent(bool typeDependant)
   {
      setFlag(TypeDependant, typeDependant);
   }

   void setIsValueDependent(bool valueDependant)
   {
      setFlag(ValueDependant, valueDependant);
   }

   bool isCTFEDependant() const
   {
      return flagIsSet(CTFEDependant);
   }

   void setIsCTFEDependant(bool dependant)
   {
      setFlag(CTFEDependant, dependant);
   }

   bool isGlobalInitializer() const
   {
      return flagIsSet(GlobalInitializer);
   }

   void setIsGlobalInitializer(bool globalInit)
   {
      setFlag(GlobalInitializer, globalInit);
   }

   bool hadError() const
   {
      return flagIsSet(HadError);
   }

   void setHadError(bool error)
   {
      setFlag(HadError, error);
   }

   static bool classof(AstNode const* T)
   {
      return classofKind(T->getTypeID());
   }

   static bool classofKind(AstNode::NodeType kind)
   {
       switch (kind) {
#      define CDOT_STMT(Name) \
          case Name##ID:
#      include "../AstNode.def"
             return true;
          default:
             return false;
       }
   }

protected:
   explicit Statement(NodeType typeID)
      : AstNode(typeID)
   {}
};

} // namespace ast
} // namespace cdot


#endif //STATEMENT_H
