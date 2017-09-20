//
// Created by Jonas Zell on 08.09.17.
//

#ifndef CDOT_DECLPASS_H
#define CDOT_DECLPASS_H

#include <string>
#include <stack>
#include "../Visitor.h"
#include "../StaticAnalysis/Function.h"

enum class AccessModifier : unsigned int;
using std::string;

class DeclPass {
public:
   DeclPass();

   void doInitialPass(std::vector<std::shared_ptr<Statement>>& statements);

   virtual void visit(CompoundStmt *node);

   virtual void visit(NamespaceDecl *node);
   virtual void visit(UsingStmt *node);
   virtual void visit(EndOfFileStmt *node);

   virtual void visit(FunctionDecl *node);
   virtual void visit(FuncArgDecl *node);

   virtual void visit(DeclStmt *node);
   virtual void visit(ClassDecl *node);
   virtual void visit(MethodDecl *node);
   virtual void visit(FieldDecl *node);
   virtual void visit(ConstrDecl *node);
   virtual void visit(DestrDecl *node);

   virtual void visit(TypedefDecl *node);
   virtual void visit(TypeRef *node);
   virtual void visit(DeclareStmt *node);
   virtual void visit(EnumDecl *node);

   virtual void visit(DebugStmt *node);
   virtual void visit(Statement *node);
   virtual void visit(Expression *node);

   string ns_prefix()
   {
      return currentNamespace.back().empty() ? "" : currentNamespace.back() + ".";
   }

   string declareVariable(string &name, Type *type, AccessModifier access, AstNode *cause);
   Type*& declareFunction(Function::UniquePtr &&func, std::vector<ObjectType *> &generics, AstNode *cause);

protected:
   std::vector<string> currentNamespace = {""};
   std::vector<string> importedNamespaces = {""};

   std::vector<string> UserTypes;
   std::vector<string> Typedefs;

   std::stack<std::vector<ObjectType*>*> GenericsStack = {};

   void pushNamespace(string &ns, bool declare = true);
   void popNamespace();
};


#endif //CDOT_DECLPASS_H
