//
// Created by Jonas Zell on 08.09.17.
//

#ifndef CDOT_DECLPASS_H
#define CDOT_DECLPASS_H

#include <string>
#include "../Visitor.h"
#include "../StaticAnalysis/Function.h"

using std::string;

class DeclPass {
public:
   DeclPass();

   void doInitialPass(std::shared_ptr<CompoundStmt>& root);

   virtual void visit(CompoundStmt *node);

   virtual void visit(NamespaceDecl *node);
   virtual void visit(UsingStmt *node);

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

   virtual void visit(Statement *node);
   virtual void visit(Expression *node);

   string ns_prefix()
   {
      return currentNamespace.back().empty() ? "" : currentNamespace.back() + ".";
   }

   string declareVariable(string &name, Type *type, AstNode *cause);
   Type*& declareFunction(Function::UniquePtr &&func, std::vector<ObjectType *> &generics, AstNode *cause);

protected:
   std::vector<string> currentNamespace = {""};
   std::vector<string> importedNamespaces = {""};

   std::vector<string> UserTypes;
   std::vector<string> Typedefs;

   std::vector<ObjectType*>* currentClassGenerics = nullptr;
   string currentClass;

   void pushNamespace(string &ns);
   void popNamespace();
};


#endif //CDOT_DECLPASS_H
