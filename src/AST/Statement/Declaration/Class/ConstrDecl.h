//
// Created by Jonas Zell on 27.06.17.
//

#ifndef CDOT_CONSTRDECL_H
#define CDOT_CONSTRDECL_H

#include "../CallableDecl.h"

class FuncArgDecl;
class CompoundStmt;

namespace cdot {
   namespace cl {
      struct Method;
   }
}

class ConstrDecl : public CallableDecl {
public:
   ConstrDecl();
   ConstrDecl(std::vector<std::shared_ptr<FuncArgDecl>>, std::shared_ptr<CompoundStmt>, AccessModifier);

   typedef std::shared_ptr<ConstrDecl> SharedPtr;
   typedef std::unique_ptr<ConstrDecl> UniquePtr;

   std::vector<std::shared_ptr<AstNode>> get_children() override;
   void __dump(int depth) override;

   NodeType get_type() override {
      return NodeType::CONSTR_DECL;
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
   bool memberwise = false;
   AccessModifier am;
   std::vector<std::shared_ptr<FuncArgDecl>> args;
   std::shared_ptr<CompoundStmt> body;

   // codegen
   std::string selfBinding;
   string className;
   cdot::cl::Method* method;

public:
   bool isMemberwise() const
   {
      return memberwise;
   }

   void setMemberwise(bool memberwise)
   {
      ConstrDecl::memberwise = memberwise;
   }

   AccessModifier getAm() const
   {
      return am;
   }

   void setAm(AccessModifier am)
   {
      ConstrDecl::am = am;
   }

   const std::vector<std::shared_ptr<FuncArgDecl>> &getArgs() const
   {
      return args;
   }

   void setArgs(const std::vector<std::shared_ptr<FuncArgDecl>> &args)
   {
      ConstrDecl::args = args;
   }

   const std::shared_ptr<CompoundStmt> &getBody() const
   {
      return body;
   }

   void setBody(const std::shared_ptr<CompoundStmt> &body)
   {
      ConstrDecl::body = body;
   }

   const string &getSelfBinding() const
   {
      return selfBinding;
   }

   void setSelfBinding(const string &selfBinding)
   {
      ConstrDecl::selfBinding = selfBinding;
   }

   const string &getClassName() const
   {
      return className;
   }

   void setClassName(const string &className)
   {
      ConstrDecl::className = className;
   }

   Method *getMethod() const
   {
      return method;
   }

   void setMethod(Method *method)
   {
      ConstrDecl::method = method;
   }
};


#endif //CDOT_CONSTRDECL_H
