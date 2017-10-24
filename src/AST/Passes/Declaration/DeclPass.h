//
// Created by Jonas Zell on 08.09.17.
//

#ifndef CDOT_DECLPASS_H
#define CDOT_DECLPASS_H

#include <string>
#include <stack>

#include "../AbstractPass.h"
#include "../SemanticAnalysis/Function.h"
#include "../../Attribute/Attribute.h"

enum class AccessModifier : unsigned int;

using std::string;
using namespace cdot;

class DeclPass : AbstractPass {
public:
   DeclPass();

   void doInitialPass(std::vector<std::shared_ptr<Statement>>& statements);
   void declareGlobalTypedefs(std::vector<std::shared_ptr<Statement>>& statements);

   void visit(CompoundStmt *node) override;

   void visit(NamespaceDecl *node) override;
   void visit(UsingStmt *node) override;
   void visit(EndOfFileStmt *node) override;

   void visit(FunctionDecl *node) override;
   void visit(FuncArgDecl *node) override;

   void visit(DeclStmt *node) override;
   void visit(ClassDecl *node) override;
   void visit(MethodDecl *node) override;
   void visit(FieldDecl *node) override;
   void visit(ConstrDecl *node) override;
   void visit(DestrDecl *node) override;
   void visit(UnionDecl *node) override;

   void visit(TypedefDecl *node) override;
   void visit(TypeRef *node) override;
   void visit(DeclareStmt *node) override;
   void visit(EnumDecl *node) override;

   void visit(DebugStmt *node) override;
   void visit(Statement *node) override;
   void visit(Expression *node) override;

   string ns_prefix()
   {
      return currentNamespace.back().empty() ? "" : currentNamespace.back() + ".";
   }

   string declareVariable(
      string &name,
      Type &type,
      AccessModifier access,
      AstNode *cause
   );

   Type declareFunction(
      Function::UniquePtr &&func,
      std::vector<GenericConstraint> &generics,
      AstNode *cause
   );

   static void resolveType(
      TypeRef *node,
      std::vector<std::vector<GenericConstraint>*>& GenericsStack,
      std::vector<string>& importedNamespaces,
      std::vector<string>& currentNamespace
   );

protected:
   static std::vector<string> UserTypes;

   std::vector<string> currentNamespace = {""};
   std::vector<string> importedNamespaces = {""};

   std::vector<std::vector<GenericConstraint>*> GenericsStack;

   void pushNamespace(string &ns, bool declare = true);
   void popNamespace();

   void CheckThrowsAttribute(Callable *callable, Attribute &attr);
};


#endif //CDOT_DECLPASS_H
