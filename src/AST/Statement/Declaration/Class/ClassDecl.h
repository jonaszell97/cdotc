//
// Created by Jonas Zell on 27.06.17.
//

#ifndef CDOT_CLASSDECLEXPR_H
#define CDOT_CLASSDECLEXPR_H

#include "../../../Statement/Statement.h"

class FieldDecl;
class MethodDecl;
class ConstrDecl;
class TypedefDecl;
class DestrDecl;
class TypeRef;

namespace cdot {
   namespace cl {
      struct Method;
      class Class;

      struct ExtensionConstraint {
         enum ConstraintKind {
            CONFORMANCE,
            TYPE_EQUALITY,
            TYPE_INEQUALITY,
            DEFAULT_CONSTRUCTIBLE,
            IS_STRUCT,
            IS_CLASS,
            IS_PROTOCOL,
            IS_ENUM
         };

         string constrainedGenericTypeName;
         std::shared_ptr<TypeRef> typeConstraint = nullptr;
         ConstraintKind kind;

         string reportFailure() const;
      };
   }
}

using cdot::cl::ExtensionConstraint;

class ClassDecl : public Statement {
public:
   // class or struct
   ClassDecl(
      string class_name,
      std::vector<std::shared_ptr<FieldDecl>> &&fields,
      std::vector<std::shared_ptr<MethodDecl>> &&methods,
      std::vector<std::shared_ptr<ConstrDecl>> &&constr,
      std::vector<std::shared_ptr<TypedefDecl>> &&typedefs,
      std::vector<GenericConstraint> &&generics,
      AccessModifier am,
      bool is_abstract,
      std::shared_ptr<TypeRef> extends,
      std::vector<std::shared_ptr<TypeRef>> &&conformsTo,
      std::shared_ptr<DestrDecl> &&destr,
      std::vector<Statement::SharedPtr> &&innerDeclarations,
      std::vector<ExtensionConstraint> &&constraints
   );

   // protocol
   ClassDecl(
      string className,
      std::vector<std::shared_ptr<FieldDecl>> &&fields,
      std::vector<std::shared_ptr<MethodDecl>> &&methods,
      std::vector<std::shared_ptr<ConstrDecl>> &&constructors,
      std::vector<std::shared_ptr<TypedefDecl>>&&typedefs,
      std::vector<GenericConstraint> &&generics,
      AccessModifier am,
      std::vector<std::shared_ptr<TypeRef>> &&conformsTo,
      std::shared_ptr<DestrDecl> &&destr,
      std::vector<Statement::SharedPtr> &&innerDeclarations
   );

   virtual inline bool isStruct() {
      return is_struct;
   }

   virtual inline void isStruct(bool str) {
      is_struct = str;
   }

   virtual inline bool isExtension() {
      return is_extension;
   }

   string& getClassName() {
      return className;
   }

   void setClassName(string &&name) {
      className = name;
   }

   virtual inline void isExtension(bool ext) {
      is_extension = ext;
   }

   virtual inline std::shared_ptr<TypeRef> getParentClass() {
      return parentClass;
   }

   virtual std::shared_ptr<DestrDecl>& getDestructor() {
      return destructor;
   }

   typedef std::shared_ptr<ClassDecl> SharedPtr;
   typedef std::unique_ptr<ClassDecl> UniquePtr;

   std::vector<std::shared_ptr<AstNode>> get_children() override;
   void __dump(int depth) override;

   NodeType get_type() override {
      return NodeType::CLASS_DECL;
   }

   llvm::Value* accept(CodeGen& v) override {
      return v.visit(this);
   }

   Type accept(TypeCheckPass& v) override {
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
   std::shared_ptr<TypeRef> parentClass = nullptr;
   std::vector<std::shared_ptr<TypeRef>> conformsTo;

   bool is_abstract = false;
   bool is_protocol = false;
   bool is_struct = false;
   bool is_extension = false;

   AccessModifier am;
   string className;
   string qualifiedName;

   std::vector<std::shared_ptr<ConstrDecl>> constructors;
   std::shared_ptr<DestrDecl> destructor;
   std::vector<std::shared_ptr<FieldDecl>> fields;
   std::vector<std::shared_ptr<MethodDecl>> methods;
   std::vector<std::shared_ptr<TypedefDecl>> typedefs;

   std::vector<GenericConstraint> generics;
   std::vector<Statement::SharedPtr> innerDeclarations;

   std::vector<ExtensionConstraint> constraints;

   // codegen
   bool explicitMemberwiseInitializer = false;
   string selfBinding;
   cdot::cl::Class* declaredClass;
   cdot::cl::Method* defaultConstr = nullptr;

public:
   void setParentClass(const std::shared_ptr<TypeRef> &parentClass) {
      ClassDecl::parentClass = parentClass;
   }

   const std::vector<std::shared_ptr<TypeRef>> &getConformsTo() const {
      return conformsTo;
   }

   void setConformsTo(const std::vector<std::shared_ptr<TypeRef>> &conformsTo) {
      ClassDecl::conformsTo = conformsTo;
   }

   bool isIs_abstract() const {
      return is_abstract;
   }

   void setIs_abstract(bool is_abstract) {
      ClassDecl::is_abstract = is_abstract;
   }

   bool isIs_protocol() const {
      return is_protocol;
   }

   void setIs_protocol(bool is_protocol) {
      ClassDecl::is_protocol = is_protocol;
   }

   bool isIs_struct() const {
      return is_struct;
   }

   void setIs_struct(bool is_struct) {
      ClassDecl::is_struct = is_struct;
   }

   bool isIs_extension() const {
      return is_extension;
   }

   void setIs_extension(bool is_extension) {
      ClassDecl::is_extension = is_extension;
   }

   AccessModifier getAm() const {
      return am;
   }

   void setAm(AccessModifier am) {
      ClassDecl::am = am;
   }

   const string &getQualifiedName() const {
      return qualifiedName;
   }

   void setQualifiedName(const string &qualifiedName) {
      ClassDecl::qualifiedName = qualifiedName;
   }

   const std::vector<std::shared_ptr<ConstrDecl>> &getConstructors() const {
      return constructors;
   }

   void setConstructors(const std::vector<std::shared_ptr<ConstrDecl>> &constructors) {
      ClassDecl::constructors = constructors;
   }

   void setDestructor(const std::shared_ptr<DestrDecl> &destructor) {
      ClassDecl::destructor = destructor;
   }

   const vector<std::shared_ptr<FieldDecl>> &getFields() const {
      return fields;
   }

   void setFields(const std::vector<std::shared_ptr<FieldDecl>> &fields) {
      ClassDecl::fields = fields;
   }

   const std::vector<std::shared_ptr<MethodDecl>> &getMethods() const {
      return methods;
   }

   void setMethods(const std::vector<std::shared_ptr<MethodDecl>> &methods) {
      ClassDecl::methods = methods;
   }

   const std::vector<std::shared_ptr<TypedefDecl>> &getTypedefs() const {
      return typedefs;
   }

   void setTypedefs(const std::vector<std::shared_ptr<TypedefDecl>> &typedefs) {
      ClassDecl::typedefs = typedefs;
   }

   const std::vector<GenericConstraint> &getGenerics() const {
      return generics;
   }

   void setGenerics(const std::vector<GenericConstraint> &generics) {
      ClassDecl::generics = generics;
   }

   const std::vector<Statement::SharedPtr> &getInnerDeclarations() const {
      return innerDeclarations;
   }

   void setInnerDeclarations(const std::vector<Statement::SharedPtr> &innerDeclarations) {
      ClassDecl::innerDeclarations = innerDeclarations;
   }

   const std::vector<ExtensionConstraint> &getConstraints() const {
      return constraints;
   }

   void setConstraints(const std::vector<ExtensionConstraint> &constraints) {
      ClassDecl::constraints = constraints;
   }

   bool isExplicitMemberwiseInitializer() const {
      return explicitMemberwiseInitializer;
   }

   void setExplicitMemberwiseInitializer(bool explicitMemberwiseInitializer) {
      ClassDecl::explicitMemberwiseInitializer = explicitMemberwiseInitializer;
   }

   const string &getSelfBinding() const {
      return selfBinding;
   }

   void setSelfBinding(const string &selfBinding) {
      ClassDecl::selfBinding = selfBinding;
   }

   Class *getDeclaredClass() const {
      return declaredClass;
   }

   void setDeclaredClass(Class *declaredClass) {
      ClassDecl::declaredClass = declaredClass;
   }

   Method *getDefaultConstr() const {
      return defaultConstr;
   }

   void setDefaultConstr(Method *defaultConstr) {
      ClassDecl::defaultConstr = defaultConstr;
   }
};


#endif //CDOT_CLASSDECLEXPR_H
