//
// Created by Jonas Zell on 19.06.17.
//

#ifndef CDOT_DECLSTATEMENT_H
#define CDOT_DECLSTATEMENT_H


#include "../Statement.h"

namespace cdot {
namespace ast {

class TypeRef;
class Expression;

class DeclStmt : public Statement {
public:
   DeclStmt(string &&ident,
            std::shared_ptr<TypeRef> &&type,
            bool isConst,
            std::shared_ptr<Expression> &&val = nullptr);

   typedef std::shared_ptr<DeclStmt> SharedPtr;

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

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == DeclStmtID;
   }

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
   bool captured = false;
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

   std::shared_ptr<Expression> &getValue()
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

   bool isCaptured() const
   {
      return captured;
   }

   void setCaptured(bool captured)
   {
      DeclStmt::captured = captured;
   }
};

} // namespace ast
} // namespace cdot

#endif //CDOT_DECLSTATEMENT_H
