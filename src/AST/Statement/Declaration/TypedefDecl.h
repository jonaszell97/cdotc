//
// Created by Jonas Zell on 01.08.17.
//

#ifndef CDOT_TYPEDEFDECL_H
#define CDOT_TYPEDEFDECL_H

#include "../Statement.h"

class TypeRef;

class TypedefDecl : public Statement {
public:
   TypedefDecl(string, std::shared_ptr<TypeRef>);

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

   Type* accept(TypeCheckPass& v) override {
      return v.visit(this);
   }

   void accept(DeclPass &v) override {
      v.visit(this);
   }

   Variant accept(ConstExprPass &v) override {
      return v.visit(this);
   }

   friend class ConstExprPass;
   friend class CodeGen;
   friend class TypeCheckPass;
   friend class DeclPass;

protected:
   string alias;
   std::shared_ptr<TypeRef> origin;
};


#endif //CDOT_TYPEDEFDECL_H
