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
   ClassDecl(string, std::vector<std::shared_ptr<FieldDecl>>&&, std::vector<std::shared_ptr<MethodDecl>>&&,
      std::vector<std::shared_ptr<ConstrDecl>>&&, std::vector<std::shared_ptr<TypedefDecl>>&&,
      std::vector<ObjectType*>&&, AccessModifier, bool, ObjectType*, std::vector<ObjectType*>&&,
      std::shared_ptr<DestrDecl>&&);

   ClassDecl(string, std::vector<std::shared_ptr<FieldDecl>>&&, std::vector<std::shared_ptr<MethodDecl>>&&,
      std::vector<std::shared_ptr<ConstrDecl>>&&, std::vector<std::shared_ptr<TypedefDecl>>&&,
      std::vector<ObjectType*>&&, AccessModifier, std::vector<ObjectType*>&&, std::shared_ptr<DestrDecl>&&);

   virtual inline bool isStruct() {
      return is_struct;
   }

   virtual inline void isStruct(bool str) {
      is_struct = str;
   }

   virtual inline bool isExtension() {
      return is_extension;
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

   void accept(DeclPass& v) override {
      v.visit(this);
   }

   Variant accept(ConstExprPass& v) override {
      return v.visit(this);
   }

   friend class ConstExprPass;
   friend class CodeGen;
   friend class TypeCheckPass;
   friend class DeclPass;
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
   
   bool explicitMemberwiseInitializer = false;

   std::vector<ObjectType*> generics;

   // codegen
   string selfBinding;
   cdot::cl::Class* declaredClass;
   cdot::cl::Method* defaultConstr = nullptr;
};


#endif //CDOT_CLASSDECLEXPR_H
