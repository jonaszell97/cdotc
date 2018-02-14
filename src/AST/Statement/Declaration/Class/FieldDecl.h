//
// Created by Jonas Zell on 27.06.17.
//

#ifndef CDOT_CLASSMEMBERDECL_H
#define CDOT_CLASSMEMBERDECL_H

#include "../NamedDecl.h"

namespace cdot {
namespace ast {

class TypeRef;
class FuncArgDecl;
class Expression;
class CompoundStmt;

class FieldDecl: public NamedDecl {
public:
   FieldDecl(std::string &&name,
             TypeRef* type,
             AccessModifier am,
             bool isStatic = false,
             bool isConst = false,
             Expression* defaultVal = {})
      : NamedDecl(FieldDeclID, am, move(name)),
        type(type), defaultVal(defaultVal)
   {
      setDeclFlag(DF_Static, isStatic);
      setDeclFlag(DF_Const, isConst);
   }

   inline void setDefault(Expression* expr)
   {
      defaultVal = expr;
   }

   void addGetter(CompoundStmt* body = nullptr)
   {
      has_getter = true;
      getterBody = body;
   }

   void addSetter(CompoundStmt* body = nullptr)
   {
      has_setter = true;
      setterBody = body;
   }

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == FieldDeclID;
   }

   friend class TransformImpl;

protected:
   std::string linkageName;

   bool has_getter = false;
   bool has_setter = false;

   CompoundStmt* getterBody = nullptr;
   CompoundStmt* setterBody = nullptr;

   MethodDecl *getterMethod;
   MethodDecl *setterMethod;

   FuncArgDecl* newVal = nullptr;

   TypeRef* type;
   Expression* defaultVal;

   size_t globalOrdering = 0;

public:
   const string &getLinkageName() const
   {
      return linkageName;
   }

   void setLinkageName(string &&linkageName)
   {
      FieldDecl::linkageName = move(linkageName);
   }

   bool hasGetter() const
   {
      return has_getter;
   }

   void hasGetter(bool hasGetter)
   {
      FieldDecl::has_getter = hasGetter;
   }

   bool hasSetter() const
   {
      return has_setter;
   }

   void hasSetter(bool hasSetter)
   {
      FieldDecl::has_setter = hasSetter;
   }

   CompoundStmt* getGetterBody() const
   {
      return getterBody;
   }

   void setGetterBody(CompoundStmt* getterBody)
   {
      FieldDecl::getterBody = getterBody;
   }

   CompoundStmt* getSetterBody() const
   {
      return setterBody;
   }

   void setSetterBody(CompoundStmt* setterBody)
   {
      FieldDecl::setterBody = setterBody;
   }

   MethodDecl *getGetterMethod() const
   {
      return getterMethod;
   }

   void setGetterMethod(MethodDecl *getterMethod)
   {
      FieldDecl::getterMethod = getterMethod;
   }

   MethodDecl *getSetterMethod() const
   {
      return setterMethod;
   }

   void setSetterMethod(MethodDecl *setterMethod)
   {
      FieldDecl::setterMethod = setterMethod;
   }

   FuncArgDecl* getNewVal() const
   {
      return newVal;
   }

   void setNewVal(FuncArgDecl* newVal)
   {
      FieldDecl::newVal = newVal;
   }

   TypeRef* getType() const
   {
      return type;
   }

   void setType(TypeRef* type)
   {
      FieldDecl::type = type;
   }

   Expression* getDefaultVal() const
   {
      return defaultVal;
   }

   size_t getGlobalOrdering() const
   {
      return globalOrdering;
   }

   void setGlobalOrdering(size_t globalOrder)
   {
      FieldDecl::globalOrdering = globalOrder;
   }
};

class AssociatedTypeDecl: public NamedDecl {
public:
   AssociatedTypeDecl(std::string &&protoSpec,
                      string &&name,
                      std::vector<StaticExpr* > &&constraints,
                      TypeRef* actualType)
      : NamedDecl(AssociatedTypeDeclID, (AccessModifier)0, move(name)),
        protocolSpecifier(move(protoSpec)),
        constraints(move(constraints)), actualType(actualType)
   {}

   const std::vector<StaticExpr* > &getConstraints() const
   {
      return constraints;
   }

   TypeRef* getActualType() const
   {
      return actualType;
   }

   void setActualType(TypeRef* actualType)
   {
      AssociatedTypeDecl::actualType = actualType;
   }

   llvm::StringRef getProtocolSpecifier() const
   {
      return protocolSpecifier;
   }

   ProtocolDecl *getProto() const { return Proto; }

   void setProto(ProtocolDecl *Proto)
   {
      AssociatedTypeDecl::Proto = Proto;
   }

   static bool classof(AstNode const *T)
   {
      return T->getTypeID() == AssociatedTypeDeclID;
   }

   friend class TransformImpl;

private:
   std::string protocolSpecifier;
   std::vector<StaticExpr* > constraints;
   TypeRef* actualType;

   ProtocolDecl *Proto = nullptr;
};

} // namespace ast
} // namespace cdot

#endif //CDOT_CLASSMEMBERDECL_H
