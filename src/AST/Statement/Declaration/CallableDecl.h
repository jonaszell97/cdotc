//
// Created by Jonas Zell on 09.10.17.
//

#ifndef CDOT_CALLABLEDECL_H
#define CDOT_CALLABLEDECL_H

#include "../Statement.h"
#include "../../../Variant/Type/Generic.h"

namespace cdot {
namespace ast {

class FuncArgDecl;

class CallableDecl: public Statement {
public:
   void setInnerDecls(std::vector<std::shared_ptr<Statement>>&& decls)
   {
      innerDeclarations = decls;
   }

   void addInnerDecl(Statement::SharedPtr&& decl)
   {
      innerDeclarations.push_back(decl);
   }

   std::vector<Statement::SharedPtr>& getInnerDecls()
   {
      return innerDeclarations;
   }

protected:
   CallableDecl(NodeType typeID,
                AccessModifier am,
                string &&name,
                std::shared_ptr<TypeRef> &&returnType,
                std::vector<std::shared_ptr<FuncArgDecl>> &&args)
      : Statement(typeID), am(am), name(move(name)),
        returnType(move(returnType)), args(move(args))
   {

   }

   AccessModifier am;

   string name;
   std::shared_ptr<TypeRef> returnType;
   std::vector<std::shared_ptr<FuncArgDecl>> args;
   std::shared_ptr<CompoundStmt> body;

   std::vector<Statement::SharedPtr> innerDeclarations;

   bool has_sret = false;

   Callable *callable = nullptr;

public:
   AccessModifier getAm() const
   {
      return am;
   }

   void setAm(AccessModifier am)
   {
      CallableDecl::am = am;
   }

   const string &getName() const
   {
      return name;
   }

   void setName(const string &funcName)
   {
      CallableDecl::name = funcName;
   }

   std::vector<std::shared_ptr<FuncArgDecl>> &getArgs()
   {
      return args;
   }

   void setArgs(const std::vector<std::shared_ptr<FuncArgDecl>> &args)
   {
      CallableDecl::args = args;
   }

   void setBody(std::shared_ptr<CompoundStmt> &body)
   {
      this->body = body;
   }

   const std::shared_ptr<TypeRef> &getReturnType() const
   {
      return returnType;
   }

   void setReturnType(const std::shared_ptr<TypeRef> &returnType)
   {
      CallableDecl::returnType = returnType;
   }

   const std::vector<Statement::SharedPtr> &getInnerDeclarations() const
   {
      return innerDeclarations;
   }

   void setInnerDeclarations(
      const std::vector<Statement::SharedPtr> &innerDeclarations)
   {
      CallableDecl::innerDeclarations = innerDeclarations;
   }

   bool hasStructRet() const
   {
      return has_sret;
   }

   void hasStructRet(bool has_sret)
   {
      CallableDecl::has_sret = has_sret;
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

   Callable *getCallable()
   {
      return callable;
   }

   void setCallable(Callable *callable)
   {
      CallableDecl::callable = callable;
   }
};

} // namespace ast
} // namespace cdot

#endif //CDOT_CALLABLEDECL_H
