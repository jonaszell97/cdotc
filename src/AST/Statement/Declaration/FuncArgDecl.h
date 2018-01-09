//
// Created by Jonas Zell on 20.06.17.
//

#ifndef CDOT_FUNCARGDECL_H
#define CDOT_FUNCARGDECL_H

#include "../../Expression/Expression.h"

namespace cdot {
namespace ast {

class TypeRef;

class FuncArgDecl: public Statement {
public:
   FuncArgDecl(std::string &&argName,
               std::shared_ptr<TypeRef> &&argType,
               Expression::SharedPtr &&defaultValue,
               bool variadicArgPackExpansion,
               bool isConst);

   typedef std::shared_ptr<FuncArgDecl> SharedPtr;

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == FuncArgDeclID;
   }

protected:
   std::string argName;
   std::shared_ptr<TypeRef> argType;
   std::shared_ptr<Expression> defaultVal;

   bool variadicArgPackExpansion : 1;
   bool is_const : 1;

public:
   const string &getArgName() const
   {
      return argName;
   }

   std::shared_ptr<TypeRef> getArgType()
   {
      return argType;
   }

   const std::shared_ptr<Expression> &getDefaultVal() const
   {
      return defaultVal;
   }

   bool isVariadicArgPackExpansion() const
   {
      return variadicArgPackExpansion;
   }

   bool isConst() const
   {
      return is_const;
   }
};

} // namespace ast
} // namespace cdot

#endif //CDOT_FUNCARGDECL_H
