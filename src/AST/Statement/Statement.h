//
// Created by Jonas Zell on 19.06.17.
//

#ifndef STATEMENT_H
#define STATEMENT_H

#include "../AstNode.h"

namespace cdot {
namespace ast {

enum class ExternKind : unsigned char {
   NONE,
   C,
   CPP
};

class Statement : public AstNode {
public:
   void isDeclaration(bool decl)
   {
      is_declaration = decl;
   }

   bool isDeclaration()
   {
      return is_declaration;
   }

   typedef std::shared_ptr<Statement> SharedPtr;

   void setExternKind(ExternKind kind)
   {
      externKind = kind;
   }

   ExternKind getExternKind() const
   {
      return externKind;
   }

   static bool classof(AstNode const* T)
   {
       switch (T->getTypeID()) {
#      define CDOT_ASTNODE(Name) \
          case Name##ID:
#      define CDOT_INCLUDE_STMT
#      include "../AstNode.def"
             return true;
          default:
             return false;
       }
   }

protected:
   explicit Statement(NodeType typeID) : AstNode(typeID) {}
   ExternKind externKind = ExternKind::NONE;
   bool is_declaration = false;
};

} // namespace ast
} // namespace cdot


#endif //STATEMENT_H
