//
// Created by Jonas Zell on 27.06.17.
//

#ifndef CDOT_CLASSDECLEXPR_H
#define CDOT_CLASSDECLEXPR_H

#include "RecordDecl.h"

class FieldDecl;
class MethodDecl;
class ConstrDecl;
class TypedefDecl;
class DestrDecl;
class TypeRef;

namespace cdot {

struct FunctionTemplateInstantiation;

namespace cl {
struct Method;
class Class;

}
}

using cdot::cl::ExtensionConstraint;

class ClassDecl : public RecordDecl {
public:
   // class or struct
   ClassDecl(
      string &&class_name,
      std::vector<std::shared_ptr<FieldDecl>> &&fields,
      std::vector<std::shared_ptr<Statement>> &&methods,
      std::vector<std::shared_ptr<ConstrDecl>> &&constr,
      std::vector<std::shared_ptr<TypedefDecl>> &&typedefs,
      std::vector<std::shared_ptr<PropDecl>> &&properties,
      AccessModifier am,
      bool is_abstract,
      std::shared_ptr<TypeRef> &&extends,
      std::vector<std::shared_ptr<TypeRef>> &&conformsTo,
      std::shared_ptr<DestrDecl> &&destr,
      std::vector<Statement::SharedPtr> &&innerDeclarations,
      bool isStruct
   );

   // protocol
   ClassDecl(
      string &&className,
      std::vector<std::shared_ptr<FieldDecl>> &&fields,
      std::vector<std::shared_ptr<Statement>> &&methods,
      std::vector<std::shared_ptr<ConstrDecl>> &&constructors,
      std::vector<std::shared_ptr<TypedefDecl>>&&typedefs,
      std::vector<std::shared_ptr<PropDecl>> &&properties,
      AccessModifier am,
      std::vector<std::shared_ptr<TypeRef>> &&conformsTo,
      std::shared_ptr<DestrDecl> &&destr,
      std::vector<Statement::SharedPtr> &&innerDeclarations
   );

   virtual inline std::shared_ptr<TypeRef> getParentClass() {
      return parentClass;
   }

   virtual std::shared_ptr<DestrDecl>& getDestructor() {
      return destructor;
   }

   typedef std::shared_ptr<ClassDecl> SharedPtr;
   typedef std::unique_ptr<ClassDecl> UniquePtr;

   std::vector<std::shared_ptr<AstNode>> get_children() override;

   NodeType get_type() override {
      return NodeType::CLASS_DECL;
   }

   ASTNODE_ACCEPT_PASSES
   ADD_FRIEND_PASSES

protected:
   std::shared_ptr<TypeRef> parentClass = nullptr;

   bool is_abstract = false;
   bool is_protocol = false;
   bool is_struct = false;

   std::vector<std::shared_ptr<ConstrDecl>> constructors;
   std::shared_ptr<DestrDecl> destructor;
   std::vector<std::shared_ptr<FieldDecl>> fields;

   // codegen
   bool explicitMemberwiseInitializer = false;
   string selfBinding;
   cdot::cl::Method* defaultConstr = nullptr;

public:
   void setParentClass(const std::shared_ptr<TypeRef> &parentClass);

   bool isAbstract() const;
   void setIsAbstract(bool is_abstract);

   bool isProtocol() const;
   void setIsProtocol(bool is_protocol);

   bool isStruct() const;
   void setIsStruct(bool is_struct);

   const std::vector<std::shared_ptr<ConstrDecl>> &getConstructors() const;
   void setConstructors(
      const std::vector<std::shared_ptr<ConstrDecl>> &constructors);

   void setDestructor(const std::shared_ptr<DestrDecl> &destructor);

   const std::vector<std::shared_ptr<FieldDecl>> &getFields() const;
   void setFields(const std::vector<std::shared_ptr<FieldDecl>> &fields);

   bool isExplicitMemberwiseInitializer() const;
   void setExplicitMemberwiseInitializer(bool explicitMemberwiseInitializer);

   const string &getSelfBinding() const;
   void setSelfBinding(const string &selfBinding);

   cl::Class *getDeclaredClass() const;

   cl::Method *getDefaultConstr() const;
   void setDefaultConstr(cl::Method *defaultConstr);
};


#endif //CDOT_CLASSDECLEXPR_H
