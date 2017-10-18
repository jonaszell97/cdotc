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

   Type accept(TypeCheckPass& v) override {
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
   string nsName;
   std::shared_ptr<CompoundStmt> contents;

   bool isAnonymousNamespace_ = false;

public:
   const string &getNsName() const
   {
      return nsName;
   }

   void setNsName(const string &nsName)
   {
      NamespaceDecl::nsName = nsName;
   }

   const std::shared_ptr<CompoundStmt> &getContents() const
   {
      return contents;
   }

   void setContents(const std::shared_ptr<CompoundStmt> &contents)
   {
      NamespaceDecl::contents = contents;
   }

   bool isAnonymousNamespace() const
   {
      return isAnonymousNamespace_;
   }

   void isAnonymousNamespace(bool isAnonymousNamespace)
   {
      NamespaceDecl::isAnonymousNamespace_ = isAnonymousNamespace;
   }
};


#endif //CDOT_MODULEDECL_H
