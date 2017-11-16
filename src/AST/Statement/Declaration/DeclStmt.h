//
// Created by Jonas Zell on 19.06.17.
//

#ifndef CDOT_DECLSTATEMENT_H
#define CDOT_DECLSTATEMENT_H


#include "../Statement.h"

class TypeRef;
class Expression;
enum class AccessModifier : unsigned int;

class DeclStmt : public Statement {
public:
   DeclStmt(
      string, std::shared_ptr<TypeRef>, bool, std::shared_ptr<Expression> = nullptr
   );

   std::vector<AstNode::SharedPtr> get_children() override;

   typedef std::shared_ptr<DeclStmt> SharedPtr;

   NodeType get_type() override {
      return NodeType::DECLARATION;
   }

   const string &getIdentifier()
   {
      return identifier;
   }

   std::shared_ptr<TypeRef> getType() {
      return type;
   }

   void setAccess(AccessModifier access) {
      this->access = access;
   }

   AccessModifier getAccess()
   {
      return access;
   }

   ASTNODE_ACCEPT_PASSES
   ADD_FRIEND_PASSES

protected:
   AccessModifier access;

   string identifier;
   std::shared_ptr<TypeRef> type;
   std::shared_ptr<Expression> value = nullptr;
   bool is_const;
   bool is_global;
   bool struct_alloca = false;
   bool protocol_decl = false;
   bool declared = false;

   bool inc_refcount = false;

public:
   void setIdentifier(const string &identifier)
   {
      DeclStmt::identifier = identifier;
   }

   void setType(const std::shared_ptr<TypeRef> &type)
   {
      DeclStmt::type = type;
   }

   const std::shared_ptr<Expression> &getValue() const
   {
      return value;
   }

   void setValue(const std::shared_ptr<Expression> &value)
   {
      DeclStmt::value = value;
   }

   bool isConst() const
   {
      return is_const;
   }

   void isConst(bool is_const)
   {
      DeclStmt::is_const = is_const;
   }

   bool isGlobal() const
   {
      return is_global;
   }

   void isGlobal(bool is_global)
   {
      DeclStmt::is_global = is_global;
   }

   bool isStructAlloca() const
   {
      return struct_alloca;
   }

   void isStructAlloca(bool isStructAlloca)
   {
      DeclStmt::struct_alloca = isStructAlloca;
   }

   bool isProtocolDecl() const
   {
      return protocol_decl;
   }

   void isProtocolDecl(bool isProtocolDecl)
   {
      DeclStmt::protocol_decl = isProtocolDecl;
   }

   bool isDeclared() const
   {
      return declared;
   }

   void setDeclared(bool declared)
   {
      DeclStmt::declared = declared;
   }

   bool incRefCount() const
   {
      return inc_refcount;
   }

   void incRefCount(bool incRefCount)
   {
      DeclStmt::inc_refcount = incRefCount;
   }
};


#endif //CDOT_DECLSTATEMENT_H
