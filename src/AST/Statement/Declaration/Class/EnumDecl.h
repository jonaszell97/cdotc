//
// Created by Jonas Zell on 29.08.17.
//

#ifndef CDOT_ENUMDECL_H
#define CDOT_ENUMDECL_H


#include "../../Statement.h"

class MethodDecl;
class EnumCaseDecl;

namespace cdot {
   namespace cl {
      struct Method;
      class Enum;
   }
}

class EnumDecl: public Statement {
public:
   EnumDecl(
      AccessModifier am,
      string&& enumName,
      std::shared_ptr<TypeRef>& rawType,
      std::vector<std::shared_ptr<MethodDecl>>&& methods,
      std::vector<std::shared_ptr<TypeRef>>&& conformsTo,
      std::vector<GenericConstraint>&& generics,
      std::vector<std::shared_ptr<EnumCaseDecl>>&& cases,
      std::vector<Statement::SharedPtr>&& innerDeclarations
   );

   string& getClassName() {
      return className;
   }

   void setClassName(string&& name) {
      className = name;
   }

   typedef std::shared_ptr<EnumDecl> SharedPtr;
   typedef std::unique_ptr<EnumDecl> UniquePtr;

   std::vector<std::shared_ptr<AstNode>> get_children() override;
   void __dump(int depth) override;

   NodeType get_type() override {
      return NodeType::ENUM_DECL;
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

   ADD_FRIEND_PASSES
   friend class cdot::cl::Class;

protected:
   std::vector<std::shared_ptr<EnumCaseDecl>> cases;
   std::vector<std::shared_ptr<TypeRef>> conformsTo;

   std::shared_ptr<TypeRef> rawType;

   AccessModifier am;
   string className;
   string qualifiedName;

   std::vector<std::shared_ptr<MethodDecl>> methods;
   std::vector<GenericConstraint> generics;

   std::vector<Statement::SharedPtr> innerDeclarations;

   // codegen
   string selfBinding;
   cdot::cl::Enum* declaredEnum;

public:
   const std::vector<std::shared_ptr<EnumCaseDecl>> &getCases() const
   {
      return cases;
   }

   void setCases(const std::vector<std::shared_ptr<EnumCaseDecl>> &cases)
   {
      EnumDecl::cases = cases;
   }

   const std::vector<std::shared_ptr<TypeRef>> &getConformsTo() const
   {
      return conformsTo;
   }

   void setConformsTo(const std::vector<std::shared_ptr<TypeRef>> &conformsTo)
   {
      EnumDecl::conformsTo = conformsTo;
   }

   const std::shared_ptr<TypeRef> &getRawType() const
   {
      return rawType;
   }

   void setRawType(const std::shared_ptr<TypeRef> &rawType)
   {
      EnumDecl::rawType = rawType;
   }

   AccessModifier getAm() const
   {
      return am;
   }

   void setAm(AccessModifier am)
   {
      EnumDecl::am = am;
   }

   const string &getQualifiedName() const
   {
      return qualifiedName;
   }

   void setQualifiedName(const string &qualifiedName)
   {
      EnumDecl::qualifiedName = qualifiedName;
   }

   const std::vector<std::shared_ptr<MethodDecl>> &getMethods() const
   {
      return methods;
   }

   void setMethods(const std::vector<std::shared_ptr<MethodDecl>> &methods)
   {
      EnumDecl::methods = methods;
   }

   const std::vector<GenericConstraint> &getGenerics() const
   {
      return generics;
   }

   void setGenerics(const std::vector<GenericConstraint> &generics)
   {
      EnumDecl::generics = generics;
   }

   const std::vector<Statement::SharedPtr> &getInnerDeclarations() const
   {
      return innerDeclarations;
   }

   void setInnerDeclarations(const std::vector<Statement::SharedPtr> &innerDeclarations)
   {
      EnumDecl::innerDeclarations = innerDeclarations;
   }

   const string &getSelfBinding() const
   {
      return selfBinding;
   }

   void setSelfBinding(const string &selfBinding)
   {
      EnumDecl::selfBinding = selfBinding;
   }

   Enum *getDeclaredEnum() const
   {
      return declaredEnum;
   }

   void setDeclaredEnum(Enum *declaredEnum)
   {
      EnumDecl::declaredEnum = declaredEnum;
   }
};


#endif //CDOT_ENUMDECL_H
