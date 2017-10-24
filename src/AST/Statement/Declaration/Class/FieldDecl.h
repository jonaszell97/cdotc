//
// Created by Jonas Zell on 27.06.17.
//

#ifndef CDOT_CLASSMEMBERDECL_H
#define CDOT_CLASSMEMBERDECL_H


#include "../../Statement.h"

class TypeRef;
class Expression;

namespace cdot {
namespace cl {
   struct Method;
   struct Field;
}
}

class FieldDecl : public Statement {
public:
   FieldDecl(
      std::string,
      std::shared_ptr<TypeRef>,
      AccessModifier = AccessModifier::PUBLIC,
      bool = false,
      bool = false,
      std::shared_ptr<Expression> = {}
   );

   void isProperty(bool prop)
   {
      is_property = prop;
   }

   inline void setDefault(std::shared_ptr<Expression> expr) {
      defaultVal = expr;
   }

   typedef std::shared_ptr<FieldDecl> SharedPtr;
   typedef std::unique_ptr<FieldDecl> UniquePtr;

   std::vector<std::shared_ptr<AstNode>> get_children() override;
   void __dump(int depth) override;

   NodeType get_type() override {
      return NodeType::FIELD_DECL;
   }

   llvm::Value* accept(CodeGen& v) override {
      return v.visit(this);
   }

   Type accept(SemaPass& v) override {
      return v.visit(this);
   }

   void accept(AbstractPass* v) override {
      v->visit(this);
   }

   Variant accept(ConstExprPass& v) override {
      return v.visit(this);
   }

   virtual inline void addGetter(std::shared_ptr<CompoundStmt> body = nullptr) {
      has_getter = true;
      getterBody = body;
   }

   virtual inline void addSetter(std::shared_ptr<CompoundStmt> body = nullptr) {
      has_setter = true;
      setterBody = body;
   }

   ADD_FRIEND_PASSES

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

   bool is_static;
   bool is_const;
   AccessModifier am;
   std::shared_ptr<TypeRef> type;
   string fieldName;
   std::shared_ptr<Expression> defaultVal;

   bool protocol_field = false;

   bool is_property = false;

   // codegen
   string className;
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

   Method *getGetterMethod() const
   {
      return getterMethod;
   }

   void setGetterMethod(Method *getterMethod)
   {
      FieldDecl::getterMethod = getterMethod;
   }

   Method *getSetterMethod() const
   {
      return setterMethod;
   }

   void setSetterMethod(Method *setterMethod)
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

   bool isStatic() const
   {
      return is_static;
   }

   void isStatic(bool is_static)
   {
      FieldDecl::is_static = is_static;
   }

   bool isConst() const
   {
      return is_const;
   }

   void isConst(bool is_const)
   {
      FieldDecl::is_const = is_const;
   }

   AccessModifier getAccess() const
   {
      return am;
   }

   void setAccess(AccessModifier am)
   {
      FieldDecl::am = am;
   }

   const std::shared_ptr<TypeRef> &getType() const
   {
      return type;
   }

   void setType(const std::shared_ptr<TypeRef> &type)
   {
      FieldDecl::type = type;
   }

   const string &getFieldName() const
   {
      return fieldName;
   }

   void setFieldName(const string &fieldName)
   {
      FieldDecl::fieldName = fieldName;
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

   bool isProperty() const
   {
      return is_property;
   }

   const string &getClassName() const
   {
      return className;
   }

   void setClassName(const string &className)
   {
      FieldDecl::className = className;
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


#endif //CDOT_CLASSMEMBERDECL_H
