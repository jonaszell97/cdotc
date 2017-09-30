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

namespace cdot {
   namespace cl {
      struct Method;
      class Class;
   }
}

class ClassDecl : public Statement {
public:
   // class or struct
   ClassDecl(
      string class_name,
      std::vector<std::shared_ptr<FieldDecl>> &&fields,
      std::vector<std::shared_ptr<MethodDecl>> &&methods,
      std::vector<std::shared_ptr<ConstrDecl>> &&constr,
      std::vector<std::shared_ptr<TypedefDecl>> &&typedefs,
      std::vector<ObjectType*> &&generics,
      AccessModifier am,
      bool is_abstract,
      ObjectType* extends,
      std::vector<ObjectType*> &&conformsTo,
      std::shared_ptr<DestrDecl> &&destr,
      std::vector<Statement::SharedPtr> &&innerDeclarations
   );

   // protocol
   ClassDecl(
      string className,
      std::vector<std::shared_ptr<FieldDecl>> &&fields,
      std::vector<std::shared_ptr<MethodDecl>> &&methods,
      std::vector<std::shared_ptr<ConstrDecl>> &&constructors,
      std::vector<std::shared_ptr<TypedefDecl>>&&typedefs,
      std::vector<ObjectType *> &&generics,
      AccessModifier am,
      std::vector<ObjectType*> &&conformsTo,
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

   virtual inline ObjectType* getParentClass() {
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

   Type* accept(TypeCheckPass& v) override {
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
   ObjectType* parentClass = nullptr;
   std::vector<ObjectType*> conformsTo;

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

   std::vector<ObjectType*> generics;
   std::vector<Statement::SharedPtr> innerDeclarations;

   // codegen
   bool explicitMemberwiseInitializer = false;
   string selfBinding;
   cdot::cl::Class* declaredClass;
   cdot::cl::Method* defaultConstr = nullptr;
};


#endif //CDOT_CLASSDECLEXPR_H
