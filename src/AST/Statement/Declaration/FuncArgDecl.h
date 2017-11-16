//
// Created by Jonas Zell on 20.06.17.
//

#ifndef CDOT_FUNCARGDECL_H
#define CDOT_FUNCARGDECL_H

#include "../../Expression/Expression.h"

class TypeRef;

class FuncArgDecl : public Expression {
public:
   explicit FuncArgDecl(bool = false);
   FuncArgDecl(std::string, std::shared_ptr<TypeRef>,
               Expression::SharedPtr = {});
   
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

   std::shared_ptr<FuncArgDecl> clone() const;

   typedef std::shared_ptr<FuncArgDecl> SharedPtr;
   std::vector<AstNode::SharedPtr> get_children() override;

   NodeType get_type() override {
      return NodeType::FUNC_ARG_DECL;
   }

   ASTNODE_ACCEPT_PASSES
   ADD_FRIEND_PASSES

protected:
   std::string argName;
   std::shared_ptr<TypeRef> argType = nullptr;
   std::shared_ptr<Expression> defaultVal;
   bool mut = false;
   bool isStruct = false;
   bool visitDefault = false;

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

   bool isMut() const {
      return mut;
   }

   void setMut(bool mut) {
      FuncArgDecl::mut = mut;
   }

   bool isIsStruct() const {
      return isStruct;
   }

   void setIsStruct(bool isStruct) {
      FuncArgDecl::isStruct = isStruct;
   }

   bool isVisitDefault() const {
      return visitDefault;
   }

   void setVisitDefault(bool visitDefault) {
      FuncArgDecl::visitDefault = visitDefault;
   }
};


#endif //CDOT_FUNCARGDECL_H
