//
// Created by Jonas Zell on 19.06.17.
//

#ifndef STATEMENT_H
#define STATEMENT_H

#include "../AstNode.h"

enum class ExternKind : unsigned char {
   NONE,
   C,
   CPP
};

class Statement : public AstNode {
public:
   void isDeclaration(bool decl) {
      is_declaration = decl;
   }

   bool isDeclaration() {
      return is_declaration;
   }

   typedef std::shared_ptr<Statement> SharedPtr;

   void setExternKind(ExternKind kind) {
      externKind = kind;
   }

   ExternKind getExternKind() const {
      return externKind;
   }

   ASTNODE_ACCEPT_PASSES
   ADD_FRIEND_PASSES

protected:
   ExternKind externKind = ExternKind::NONE;
   bool is_declaration = false;
};


#endif //STATEMENT_H
