//
// Created by Jonas Zell on 27.06.17.
//

#ifndef CDOT_MEMBERDECL_H
#define CDOT_MEMBERDECL_H


#include "../../Statement.h"

class TypeRef;
class CompoundStmt;
class FuncArgDecl;

namespace cdot {
   namespace cl {
      struct Method;
   }
}

class MethodDecl : public Statement {
public:
   MethodDecl(std::string, std::shared_ptr<TypeRef>, std::vector<std::shared_ptr<FuncArgDecl>>, std::shared_ptr<CompoundStmt>,
         AccessModifier = AccessModifier::PUBLIC, bool = false);
   MethodDecl(std::string, std::shared_ptr<TypeRef>, std::vector<std::shared_ptr<FuncArgDecl>>, AccessModifier =
      AccessModifier::PUBLIC, bool = false);
   MethodDecl(string, string, std::vector<std::shared_ptr<FuncArgDecl>>);

   ~MethodDecl() {

   }

   void isMutating(bool mut) {
      isMutating_ = mut;
   }

   bool hasDefinition() {
      return hasDefinition_;
   }

   void setGenerics(std::vector<ObjectType*> gen) {
      generics = gen;
   }

   typedef std::shared_ptr<MethodDecl> SharedPtr;
   typedef std::unique_ptr<MethodDecl> UniquePtr;

   std::vector<AstNode::SharedPtr> get_children() override;
   void __dump(int depth) override;

   NodeType get_type() override {
      return NodeType::METHOD_DECL;
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

protected:
   bool isStatic;
   bool isAbstract;
   bool isAlias = false;
   AccessModifier am;
   string methodName;
   std::shared_ptr<TypeRef> returnType;
   std::vector<std::shared_ptr<FuncArgDecl>> args;
   std::shared_ptr<CompoundStmt> body;

   std::vector<ObjectType*> generics;

   string alias;
   bool isMutating_ = false;

   bool hasDefinition_ = false;

   // codegen
   std::string class_name;
   std::string selfBinding;
   cdot::cl::Method* method;

   bool isUsed = false;
};


#endif //CDOT_MEMBERDECL_H
