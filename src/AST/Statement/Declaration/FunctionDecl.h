//
// Created by Jonas Zell on 20.06.17.
//

#ifndef CDOT_FUNCTIONDECL_H
#define CDOT_FUNCTIONDECL_H

#include "CallableDecl.h"

namespace cdot {
namespace ast {

class FuncArgDecl;
class TypeRef;
class CompoundStmt;

class FunctionDecl : public CallableDecl {
public:
   FunctionDecl(AccessModifier am,
                string &&funcName,
                std::shared_ptr<TypeRef> &&returnType = nullptr)
      : CallableDecl(FunctionDeclID, am, move(funcName), move(returnType), {})
   {

   }

   typedef std::shared_ptr<FunctionDecl> SharedPtr;

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == FunctionDeclID;
   }
};

} // namespace ast
} // namespace cdot

#endif //CDOT_FUNCTIONDECL_H
