//
// Created by Jonas Zell on 20.06.17.
//

#ifndef CDOT_FUNCTIONDECL_H
#define CDOT_FUNCTIONDECL_H

#include "CallableDecl.h"

class FuncArgDecl;
class TypeRef;
class CompoundStmt;

class FunctionDecl : public CallableDecl {
public:
   FunctionDecl(
      string funcName,
      std::shared_ptr<TypeRef> returnType = nullptr
   );

   FunctionDecl(
      string &&funcName,
      std::shared_ptr<TypeRef> &&returnType,
      std::vector<std::shared_ptr<FuncArgDecl>> &&args,
      std::shared_ptr<CompoundStmt> &&body,
      std::vector<GenericConstraint> &&generics,
      bool has_sret
   );

   void setReturnType(std::shared_ptr<TypeRef> type)
   {
      returnType = type;
   }

   string getName()
   {
      return funcName;
   }

   std::vector<std::shared_ptr<FuncArgDecl>> &getArgs()
   {
      return args;
   }

   std::shared_ptr<TypeRef> getReturnType()
   {
      return returnType;
   }

   void setBody(std::shared_ptr<CompoundStmt> _body)
   {
      body = _body;
   }

   void setGenerics(std::vector<GenericConstraint> &&gen)
   {
      generics = gen;
   }

   std::vector<GenericConstraint>& getGenerics()
   {
      return generics;
   }

   void addArgument(std::shared_ptr<FuncArgDecl> arg)
   {
      args.push_back(arg);
   }

   bool hasDefinition()
   {
      return !is_declaration;
   }

   std::shared_ptr<CompoundStmt>& getBody()
   {
      return body;
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

   Type accept(SemaPass& v) override {
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
   string funcName;
   std::shared_ptr<TypeRef> returnType;
   std::vector<std::shared_ptr<FuncArgDecl>> args;
   std::shared_ptr<CompoundStmt> body;

   std::vector<GenericConstraint> generics;

   Function* declaredFunction;
   bool has_sret = false;

public:
   const string &getFuncName() const
   {
      return funcName;
   }

   void setFuncName(const string &funcName)
   {
      FunctionDecl::funcName = funcName;
   }

   void setArgs(const std::vector<std::shared_ptr<FuncArgDecl>> &args)
   {
      FunctionDecl::args = args;
   }

   Function *getDeclaredFunction() const
   {
      return declaredFunction;
   }

   void setDeclaredFunction(Function *declaredFunction)
   {
      FunctionDecl::declaredFunction = declaredFunction;
   }

   bool hasStructRet() const
   {
      return has_sret;
   }

   void hasStructRet(bool has_sret)
   {
      FunctionDecl::has_sret = has_sret;
   }
};


#endif //CDOT_FUNCTIONDECL_H
