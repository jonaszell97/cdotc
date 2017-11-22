//
// Created by Jonas Zell on 20.06.17.
//

#ifndef CDOT_FUNCARGDECL_H
#define CDOT_FUNCARGDECL_H

#include "../../Expression/Expression.h"

namespace cdot {
namespace ast {

class TypeRef;

class FuncArgDecl : public Statement {
public:
   FuncArgDecl(std::string &&argName,
               std::shared_ptr<TypeRef> &&argType,
               Expression::SharedPtr &&defaultValue = {});
   
   void setName(string name_) {
      argName = name_;
   }

   void setType(std::shared_ptr<TypeRef> type) {
      argType = type;
   }

   std::shared_ptr<TypeRef> getArgType() {
      return argType;
   }

   void setDefault(Expression::SharedPtr def) {
      defaultVal = def;
   }

   typedef std::shared_ptr<FuncArgDecl> SharedPtr;

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == FuncArgDeclID;
   }

protected:
   std::string argName;
   std::shared_ptr<TypeRef> argType = nullptr;
   std::shared_ptr<Expression> defaultVal;

public:
   const string &getArgName() const {
      return argName;
   }

   void setArgName(const string &argName) {
      FuncArgDecl::argName = argName;
   }

   void setArgType(const std::shared_ptr<TypeRef> &argType) {
      FuncArgDecl::argType = argType;
   }

   const std::shared_ptr<Expression> &getDefaultVal() const {
      return defaultVal;
   }

   void setDefaultVal(const std::shared_ptr<Expression> &defaultVal) {
      FuncArgDecl::defaultVal = defaultVal;
   }
};

} // namespace ast
} // namespace cdot

#endif //CDOT_FUNCARGDECL_H
