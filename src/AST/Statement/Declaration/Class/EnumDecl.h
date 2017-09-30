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
      std::vector<ObjectType*>&& conformsTo,
      std::vector<ObjectType*>&& generics,
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
   std::vector<std::shared_ptr<EnumCaseDecl>> cases;
   std::vector<ObjectType*> conformsTo;

   std::shared_ptr<TypeRef> rawType;

   AccessModifier am;
   string className;
   string qualifiedName;

   std::vector<std::shared_ptr<MethodDecl>> methods;
   std::vector<ObjectType*> generics;

   std::vector<Statement::SharedPtr> innerDeclarations;

   // codegen
   string selfBinding;
   cdot::cl::Enum* declaredEnum;
};


#endif //CDOT_ENUMDECL_H
