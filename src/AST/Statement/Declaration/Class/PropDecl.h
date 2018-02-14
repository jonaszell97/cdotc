//
// Created by Jonas Zell on 14.11.17.
//

#ifndef CDOT_PROPDECL_H
#define CDOT_PROPDECL_H

#include "../NamedDecl.h"

namespace cdot {
namespace ast {

class TypeRef;
class CompoundStmt;

class PropDecl: public NamedDecl {
public:
   PropDecl(string &&propName,
            TypeRef* type,
            AccessModifier access,
            bool isStatic,
            bool hasDefinition,
            bool hasGetter,
            bool hasSetter,
            CompoundStmt* getter,
            CompoundStmt* setter,
            string &&newValName)
      : NamedDecl(PropDeclID, access, std::move(propName)),
        type(type),
        getterBody(getter), setterBody(setter),
        getter(hasGetter), setter(hasSetter), newValName(move(newValName))
   {
      setDeclFlag(DF_Static, isStatic);
      setDeclFlag(DF_HasDefinition, hasDefinition);
   }

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == PropDeclID;
   }

   friend class TransformImpl;

protected:
   TypeRef* type;

   CompoundStmt* getterBody = nullptr;
   CompoundStmt* setterBody = nullptr;

   bool getter = false;
   bool setter = false;

   string newValName;

   MethodDecl *getterMethod;
   MethodDecl *setterMethod;

public:
   TypeRef* getType() const { return type; }

   bool hasGetter() const { return getter; }
   bool hasSetter() const { return setter; }

   CompoundStmt* getGetterBody() const { return getterBody; }
   CompoundStmt* getSetterBody() const { return setterBody; }

   string &getNewValName() { return newValName; }
   string const& getNewValName() const { return newValName; }

   MethodDecl *getGetterMethod() const
   {
      return getterMethod;
   }

   void setGetterMethod(MethodDecl *getterMethod)
   {
      PropDecl::getterMethod = getterMethod;
   }

   MethodDecl *getSetterMethod() const
   {
      return setterMethod;
   }

   void setSetterMethod(MethodDecl *setterMethod)
   {
      PropDecl::setterMethod = setterMethod;
   }
};

} // namespace ast
} // namespace cdot

#endif //CDOT_PROPDECL_H
