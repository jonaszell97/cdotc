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

   llvm::Value* accept(CodeGenVisitor& v) override {
      return v.visit(this);
   }

   Type* accept(TypeCheckVisitor& v) override {
      return v.visit(this);
   }

   friend class ConstExprVisitor;
   friend class CodeGenVisitor;
   friend class TypeCheckVisitor;

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

   // codegen
   std::string class_name;
   std::string selfBinding;
   cdot::cl::Method* method;
};


#endif //CDOT_MEMBERDECL_H
