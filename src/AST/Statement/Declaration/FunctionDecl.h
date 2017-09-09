//
// Created by Jonas Zell on 20.06.17.
//

#ifndef CDOT_FUNCTIONDECL_H
#define CDOT_FUNCTIONDECL_H


#include "../Statement.h"

class FuncArgDecl;
class TypeRef;
class CompoundStmt;

class FunctionDecl : public Statement {
public:
   FunctionDecl(string, std::shared_ptr<TypeRef> = nullptr);
   ~FunctionDecl() override;

   void setReturnType(std::shared_ptr<TypeRef> type) {
      returnType = type;
   }

   void isOperatorDecl(bool op) {
      isOperatorDeclaration = op;
   }

   string getName() {
      return funcName;
   }

   std::vector<std::shared_ptr<FuncArgDecl>> getArgs() {
      return args;
   }

   std::shared_ptr<TypeRef> getReturnType() {
      return returnType;
   }

   void setBody(std::shared_ptr<CompoundStmt> _body) {
      body = _body;
   }

   void setGenerics(std::vector<ObjectType *> &&gen) {
      generics = gen;
   }

   std::vector<ObjectType*> getGenerics() {
      return generics;
   }

   void addArgument(std::shared_ptr<FuncArgDecl> arg) {
      args.push_back(arg);
   }

   typedef std::shared_ptr<FunctionDecl> SharedPtr;
   std::vector<AstNode::SharedPtr> get_children() override;
   void __dump(int depth) override;

   NodeType get_type() override {
      return NodeType::FUNCTION_DECL;
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

protected:
   string funcName;
   std::shared_ptr<TypeRef> returnType;
   std::vector<std::shared_ptr<FuncArgDecl>> args;
   std::shared_ptr<CompoundStmt> body;

   std::vector<ObjectType*> generics;

   Function* declaredFunction;
   bool hasHiddenParam = false;
   bool isOperatorDeclaration = false;
};


#endif //CDOT_FUNCTIONDECL_H
