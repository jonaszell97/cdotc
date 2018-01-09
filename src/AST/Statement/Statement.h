//
// Created by Jonas Zell on 19.06.17.
//

#ifndef STATEMENT_H
#define STATEMENT_H

#include "../AstNode.h"

namespace cdot {
namespace ast {

enum class ExternKind : unsigned char {
   NONE,
   C,
   CPP,
   Native
};

class Expression;

class Statement : public AstNode {
public:
   enum Flags: uint32_t {
      TypeDependant     = 1,
      ValueDependant    = TypeDependant     << 1,
      GlobalInitializer = ValueDependant    << 1,
      HadError          = GlobalInitializer << 1,
      CTFEDependant     = HadError          << 1,
   };

   void isDeclaration(bool decl)
   {
      is_declaration = decl;
   }

   bool isDeclaration()
   {
      return is_declaration;
   }

   typedef std::shared_ptr<Statement> SharedPtr;

   void setExternKind(ExternKind kind)
   {
      externKind = kind;
   }

   ExternKind getExternKind() const
   {
      return externKind;
   }

   bool isTypeDependant() const
   {
      return flagIsSet(TypeDependant);
   }

   bool isValueDependant() const
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
       switch (T->getTypeID()) {
#      define CDOT_STMT(Name) \
          case Name##ID:
#      include "../AstNode.def"
             return true;
          default:
             return false;
       }
   }

protected:
   explicit Statement(NodeType typeID);
   ~Statement();

   ExternKind externKind = ExternKind::NONE;
   bool is_declaration = false;
};

} // namespace ast
} // namespace cdot


#endif //STATEMENT_H
