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
      std::vector<std::shared_ptr<MethodDecl>>&& methods,
      std::vector<ObjectType*>&& conformsTo,
      std::vector<ObjectType*>&& generics,
      std::vector<std::shared_ptr<EnumCaseDecl>>&& cases
   );

   typedef std::shared_ptr<EnumDecl> SharedPtr;
   typedef std::unique_ptr<EnumDecl> UniquePtr;

   std::vector<std::shared_ptr<AstNode>> get_children() override;
   void __dump(int depth) override;

   NodeType get_type() override {
      return NodeType::ENUM_DECL;
   }

   llvm::Value* accept(CodeGenVisitor& v) override {
      return v.visit(this);
   }

   Type* accept(TypeCheckVisitor& v) override {
      return v.visit(this);
   }

   friend class ConstExprVisitor;
   friend class CodeGenVisitor;
   friend class TypeCheckVisitor;
   friend class cdot::cl::Class;

protected:
   std::vector<std::shared_ptr<EnumCaseDecl>> cases;
   std::vector<ObjectType*> conformsTo;

   AccessModifier am;
   string className;
   string qualifiedName;

   std::vector<std::shared_ptr<MethodDecl>> methods;
   std::vector<ObjectType*> generics;

   // codegen
   string selfBinding;
   cdot::cl::Enum* declaredEnum;
};


#endif //CDOT_ENUMDECL_H
