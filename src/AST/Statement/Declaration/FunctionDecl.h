//
// Created by Jonas Zell on 20.06.17.
//

#ifndef CDOT_FUNCTIONDECL_H
#define CDOT_FUNCTIONDECL_H

#include "CallableDecl.h"

class FuncArgDecl;
class TypeRef;
class CompoundStmt;

class FunctionDecl : public CallableDecl {
public:
   explicit FunctionDecl(
      AccessModifier am,
      string &&funcName,
      std::shared_ptr<TypeRef> returnType = nullptr
   );

   FunctionDecl(
      AccessModifier am,
      string &&funcName,
      std::shared_ptr<TypeRef> &&returnType,
      std::vector<std::shared_ptr<FuncArgDecl>> &&args,
      std::shared_ptr<CompoundStmt> &&body,
      bool has_sret
   );

   typedef std::shared_ptr<FunctionDecl> SharedPtr;
   std::vector<AstNode::SharedPtr> get_children() override;

   NodeType get_type() override {
      return NodeType::FUNCTION_DECL;
   }

   ASTNODE_ACCEPT_PASSES
   ADD_FRIEND_PASSES
};


#endif //CDOT_FUNCTIONDECL_H
