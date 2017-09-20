//
// Created by Jonas Zell on 02.07.17.
//

#ifndef CDOT_MODULEDECL_H
#define CDOT_MODULEDECL_H

#include "../Statement.h"

class CompoundStmt;

class NamespaceDecl : public Statement {
public:
   NamespaceDecl(string, std::shared_ptr<CompoundStmt>, bool);

   typedef std::shared_ptr<NamespaceDecl> SharedPtr;
   std::vector<AstNode::SharedPtr> get_children() override;
   void __dump(int depth) override;

   NodeType get_type() override {
      return NodeType::NAMESPACE_DECL;
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
   friend class TypeCheckPass;
   friend class DeclPass;
   friend class CodeGen;


protected:
   string nsName;
   std::shared_ptr<CompoundStmt> contents;

   bool isAnonymousNamespace = false;
};


#endif //CDOT_MODULEDECL_H
