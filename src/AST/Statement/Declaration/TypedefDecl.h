//
// Created by Jonas Zell on 01.08.17.
//

#ifndef CDOT_TYPEDEFDECL_H
#define CDOT_TYPEDEFDECL_H

#include "../Statement.h"

class TypeRef;

class TypedefDecl : public Statement {
public:
   TypedefDecl(
      AccessModifier access,
      string&& alias,
      std::shared_ptr<TypeRef>&& origin,
      std::vector<GenericConstraint>&& generics
   );

   typedef std::shared_ptr<TypedefDecl> SharedPtr;
   typedef std::unique_ptr<TypedefDecl> UniquePtr;

   std::vector<AstNode::SharedPtr> get_children() override;
   void __dump(int depth) override;

   NodeType get_type() override {
      return NodeType::TYPEDEF_DECL;
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

   Variant accept(ConstExprPass &v) override {
      return v.visit(this);
   }

   ADD_FRIEND_PASSES

protected:
   AccessModifier access;
   string alias;
   std::shared_ptr<TypeRef> origin;

   std::vector<GenericConstraint> generics;
};


#endif //CDOT_TYPEDEFDECL_H
