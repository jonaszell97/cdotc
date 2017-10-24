//
// Created by Jonas Zell on 27.06.17.
//

#ifndef CDOT_MEMBERDECL_H
#define CDOT_MEMBERDECL_H

#include "../CallableDecl.h"

class TypeRef;
class CompoundStmt;
class FuncArgDecl;

namespace cdot {
   namespace cl {
      struct Method;
   }
}

class MethodDecl : public CallableDecl {
public:
   MethodDecl(std::string, std::shared_ptr<TypeRef>, std::vector<std::shared_ptr<FuncArgDecl>>, std::shared_ptr<CompoundStmt>,
         AccessModifier = AccessModifier::PUBLIC, bool = false);
   MethodDecl(std::string, std::shared_ptr<TypeRef>, std::vector<std::shared_ptr<FuncArgDecl>>, AccessModifier =
      AccessModifier::PUBLIC, bool = false);
   MethodDecl(string, string, std::vector<std::shared_ptr<FuncArgDecl>>);

   void isMutating(bool mut) {
      isMutating_ = mut;
   }

   void isCastOp(bool castop) {
      isCastOp_ = castop;
   }

   bool hasDefinition() {
      return hasDefinition_;
   }

   void setGenerics(std::vector<GenericConstraint> gen) {
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
   bool isStatic;
   bool isAbstract;
   bool isAlias = false;
   AccessModifier am;
   string methodName;
   std::shared_ptr<TypeRef> returnType;
   std::vector<std::shared_ptr<FuncArgDecl>> args;
   std::shared_ptr<CompoundStmt> body;

   std::vector<GenericConstraint> generics;

   string alias;
   bool isMutating_ = false;
   bool isCastOp_ = false;

   bool hasDefinition_ = false;

   // codegen
   std::string class_name;
   std::string selfBinding;
   cdot::cl::Method* method;

   bool isUsed = false;

public:
   bool isIsStatic() const {
      return isStatic;
   }

   void setIsStatic(bool isStatic) {
      MethodDecl::isStatic = isStatic;
   }

   bool isIsAbstract() const {
      return isAbstract;
   }

   void setIsAbstract(bool isAbstract) {
      MethodDecl::isAbstract = isAbstract;
   }

   bool isIsAlias() const {
      return isAlias;
   }

   void setIsAlias(bool isAlias) {
      MethodDecl::isAlias = isAlias;
   }

   AccessModifier getAm() const {
      return am;
   }

   void setAm(AccessModifier am) {
      MethodDecl::am = am;
   }

   const string &getMethodName() const {
      return methodName;
   }

   void setMethodName(const string &methodName) {
      MethodDecl::methodName = methodName;
   }

   const std::shared_ptr<TypeRef> &getReturnType() const {
      return returnType;
   }

   void setReturnType(const std::shared_ptr<TypeRef> &returnType) {
      MethodDecl::returnType = returnType;
   }

   const std::vector<std::shared_ptr<FuncArgDecl>> &getArgs() const {
      return args;
   }

   void setArgs(const vector<std::shared_ptr<FuncArgDecl>> &args) {
      MethodDecl::args = args;
   }

   const std::shared_ptr<CompoundStmt> &getBody() const {
      return body;
   }

   void setBody(const std::shared_ptr<CompoundStmt> &body) {
      MethodDecl::body = body;
   }

   const std::vector<GenericConstraint> &getGenerics() const {
      return generics;
   }

   const string &getAlias() const {
      return alias;
   }

   void setAlias(const string &alias) {
      MethodDecl::alias = alias;
   }

   bool isIsMutating_() const {
      return isMutating_;
   }

   void setIsMutating_(bool isMutating_) {
      MethodDecl::isMutating_ = isMutating_;
   }

   bool isIsCastOp_() const {
      return isCastOp_;
   }

   void setIsCastOp_(bool isCastOp_) {
      MethodDecl::isCastOp_ = isCastOp_;
   }

   bool isHasDefinition_() const {
      return hasDefinition_;
   }

   void setHasDefinition_(bool hasDefinition_) {
      MethodDecl::hasDefinition_ = hasDefinition_;
   }

   const string &getClass_name() const {
      return class_name;
   }

   void setClass_name(const string &class_name) {
      MethodDecl::class_name = class_name;
   }

   const string &getSelfBinding() const {
      return selfBinding;
   }

   void setSelfBinding(const string &selfBinding) {
      MethodDecl::selfBinding = selfBinding;
   }

   Method *getMethod() const {
      return method;
   }

   void setMethod(Method *method) {
      MethodDecl::method = method;
   }

   bool isIsUsed() const {
      return isUsed;
   }

   void setIsUsed(bool isUsed) {
      MethodDecl::isUsed = isUsed;
   }
};


#endif //CDOT_MEMBERDECL_H
