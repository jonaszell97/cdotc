//
// Created by Jonas Zell on 10.10.17.
//

#ifndef CDOT_UNIONDECL_H
#define CDOT_UNIONDECL_H

#include "../../Statement.h"

namespace cdot {
   namespace cl {
      class Union;
   }
}

using cdot::cl::Union;

class UnionDecl: public Statement {
public:
   typedef unordered_map<string, std::shared_ptr<TypeRef>> UnionTypes;
   typedef std::shared_ptr<UnionDecl> SharedPtr;
   typedef std::unique_ptr<UnionDecl> UniquePtr;

   UnionDecl(string&& name, UnionTypes&& types, bool isConst);

   std::vector<std::shared_ptr<AstNode>> get_children() override;

   NodeType get_type() override {
      return NodeType::UNION_DECL;
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

   const string &getName() const {
      return name;
   }

   void setName(const string &name) {
      UnionDecl::name = name;
   }

   const UnionTypes &getContainedTypes() const {
      return containedTypes;
   }

   void setContainedTypes(const UnionTypes &containedTypes) {
      UnionDecl::containedTypes = containedTypes;
   }

   bool isConst() const;
   void isConst(bool is_const);

   Union *getDeclaredUnion() const {
      return declaredUnion;
   }

   void setDeclaredUnion(Union *declaredUnion) {
      UnionDecl::declaredUnion = declaredUnion;
   }

protected:
   string name;
   UnionTypes containedTypes;
   bool is_const = false;

   Union *declaredUnion;
};


#endif //CDOT_UNIONDECL_H
