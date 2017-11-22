//
// Created by Jonas Zell on 27.06.17.
//

#ifndef CDOT_CLASSMEMBERDECL_H
#define CDOT_CLASSMEMBERDECL_H


#include "../../Statement.h"
#include "RecordSubDecl.h"

namespace cdot {

namespace cl {
struct Method;
struct Field;
}

namespace ast {

class TypeRef;
class FuncArgDecl;
class Expression;
class CompoundStmt;

class FieldDecl : public RecordSubDecl {
public:
   FieldDecl(
      std::string &&name,
      std::shared_ptr<TypeRef> &&type,
      AccessModifier am,
      bool isStatic = false,
      bool isConst = false,
      std::shared_ptr<Expression> &&defaultVal = {}
   );

   inline void setDefault(std::shared_ptr<Expression> &&expr)
   {
      defaultVal = move(expr);
   }

   typedef std::shared_ptr<FieldDecl> SharedPtr;

   void addGetter(std::shared_ptr<CompoundStmt> &&body = nullptr)
   {
      has_getter = true;
      getterBody = move(body);
   }

   void addSetter(std::shared_ptr<CompoundStmt> &&body = nullptr)
   {
      has_setter = true;
      setterBody = move(body);
   }

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == FieldDeclID;
   }

protected:
   bool has_getter = false;
   bool has_setter = false;

   std::shared_ptr<CompoundStmt> getterBody = nullptr;
   std::shared_ptr<CompoundStmt> setterBody = nullptr;

   string getterSelfBinding;
   string setterSelfBinding;

   cdot::cl::Method *getterMethod;
   cdot::cl::Method *setterMethod;

   std::shared_ptr<FuncArgDecl> newVal = nullptr;

   bool is_const;
   std::shared_ptr<TypeRef> type;
   std::shared_ptr<Expression> defaultVal;

   bool protocol_field = false;

   // codegen
   string getterBinding;
   string setterBinding;

public:
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

   const std::shared_ptr<CompoundStmt> &getGetterBody() const
   {
      return getterBody;
   }

   void setGetterBody(const std::shared_ptr<CompoundStmt> &getterBody)
   {
      FieldDecl::getterBody = getterBody;
   }

   const std::shared_ptr<CompoundStmt> &getSetterBody() const
   {
      return setterBody;
   }

   void setSetterBody(const std::shared_ptr<CompoundStmt> &setterBody)
   {
      FieldDecl::setterBody = setterBody;
   }

   const string &getGetterSelfBinding() const
   {
      return getterSelfBinding;
   }

   void setGetterSelfBinding(const string &getterSelfBinding)
   {
      FieldDecl::getterSelfBinding = getterSelfBinding;
   }

   const string &getSetterSelfBinding() const
   {
      return setterSelfBinding;
   }

   void setSetterSelfBinding(const string &setterSelfBinding)
   {
      FieldDecl::setterSelfBinding = setterSelfBinding;
   }

   cdot::cl::Method *getGetterMethod() const
   {
      return getterMethod;
   }

   void setGetterMethod(cdot::cl::Method *getterMethod)
   {
      FieldDecl::getterMethod = getterMethod;
   }

   cdot::cl::Method *getSetterMethod() const
   {
      return setterMethod;
   }

   void setSetterMethod(cdot::cl::Method *setterMethod)
   {
      FieldDecl::setterMethod = setterMethod;
   }

   const std::shared_ptr<FuncArgDecl> &getNewVal() const
   {
      return newVal;
   }

   void setNewVal(const std::shared_ptr<FuncArgDecl> &newVal)
   {
      FieldDecl::newVal = newVal;
   }

   bool isConst() const
   {
      return is_const;
   }

   void isConst(bool is_const)
   {
      FieldDecl::is_const = is_const;
   }

   const std::shared_ptr<TypeRef> &getType() const
   {
      return type;
   }

   void setType(const std::shared_ptr<TypeRef> &type)
   {
      FieldDecl::type = type;
   }

   const std::shared_ptr<Expression> &getDefaultVal() const
   {
      return defaultVal;
   }

   void setDefaultVal(const std::shared_ptr<Expression> &defaultVal)
   {
      FieldDecl::defaultVal = defaultVal;
   }

   bool isProtocolFfield() const
   {
      return protocol_field;
   }

   void isProtocolField(bool protocol_field)
   {
      FieldDecl::protocol_field = protocol_field;
   }

   const string &getGetterBinding() const
   {
      return getterBinding;
   }

   void setGetterBinding(const string &getterBinding)
   {
      FieldDecl::getterBinding = getterBinding;
   }

   const string &getSetterBinding() const
   {
      return setterBinding;
   }

   void setSetterBinding(const string &setterBinding)
   {
      FieldDecl::setterBinding = setterBinding;
   }
};

} // namespace ast
} // namespace cdot

#endif //CDOT_CLASSMEMBERDECL_H
