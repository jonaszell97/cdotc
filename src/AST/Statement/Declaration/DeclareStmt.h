//
// Created by Jonas Zell on 13.08.17.
//

#ifndef CDOT_DECLARESTMT_H
#define CDOT_DECLARESTMT_H


#include "../Statement.h"

class FuncArgDecl;
class TypeRef;

class DeclareStmt : public Statement {
public:
   explicit DeclareStmt(ExternKind kind);

   void addDeclaration(Statement::SharedPtr&& stmt);

   std::vector<AstNode::SharedPtr> get_children() override;

   typedef std::shared_ptr<DeclareStmt> SharedPtr;

   NodeType get_type() override {
      return NodeType::DECLARE_STMT;
   }

   ASTNODE_ACCEPT_PASSES
   ADD_FRIEND_PASSES

protected:
   std::vector<Statement::SharedPtr> declarations;

public:
   const std::vector<Statement::SharedPtr> &getDeclarations() const
   {
      return declarations;
   }

   void setDeclarations(const std::vector<Statement::SharedPtr> &declarations)
   {
      DeclareStmt::declarations = declarations;
   }
};


#endif //CDOT_DECLARESTMT_H
