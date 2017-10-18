//
// Created by Jonas Zell on 09.10.17.
//

#ifndef CDOT_CALLABLEDECL_H
#define CDOT_CALLABLEDECL_H


#include "../Statement.h"

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
   CallableDecl() = default;

   std::vector<Statement::SharedPtr> innerDeclarations;
};


#endif //CDOT_CALLABLEDECL_H
