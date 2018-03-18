//
// Created by Jonas Zell on 25.06.17.
//

#ifndef CDOT_ASTVISITOR_H
#define CDOT_ASTVISITOR_H

#include "AST/Passes/ASTIncludes.h"
#include "AST/Traverse.h"

namespace cdot {
namespace ast {

template<class SubClass, class RetTy = void, class ...ParamTys>
class AbstractPass {
public:
#  define CDOT_EXPR(Name)                                   \
   RetTy visit##Name(Name*, ParamTys...) { return RetTy(); }
#  include "../AstNode.def"

#  define CDOT_EXPR(Name)
#  define CDOT_STMT(Name)                                   \
   void visit##Name(Name*, ParamTys...) { }
#  include "AST/AstNode.def"


   RetTy visit(Expression *node, ParamTys... params)
   {
      switch (node->getTypeID()) {
#     define CDOT_EXPR(Name)                                            \
         case AstNode::Name##ID:                                        \
            return static_cast<SubClass*>(this)                         \
               ->visit##Name(static_cast<Name*>(node),                  \
                             std::forward<ParamTys>(params)...);
#     include "AST/AstNode.def"

         default:
            llvm_unreachable("not an expression!");
      }
   }

   void visit(Statement *node, ParamTys... params)
   {
      switch (node->getTypeID()) {
#     define CDOT_EXPR(Name)                                            \
         case AstNode::Name##ID:                                        \
            static_cast<SubClass*>(this)                                \
               ->visit##Name(static_cast<Name*>(node),                  \
                             std::forward<ParamTys>(params)...);        \
            return;
#     define CDOT_STMT(Name)                                            \
         case AstNode::Name##ID:                                        \
            return static_cast<SubClass*>(this)                         \
               ->visit##Name(static_cast<Name*>(node),                  \
                             std::forward<ParamTys>(params)...);
#     include "AST/AstNode.def"

      default:
         llvm_unreachable("bad node kind!");
      }
   }
};


template<class SubClass>
class RecursiveASTVisitor {
public:
   bool visitChildren(Statement *Stmt)
   {
      llvm::SmallVector<Statement*, 8> Children;
      collectDirectChildren(Children, Stmt);

      for (auto &Child : Children) {
         if (!static_cast<SubClass*>(this)->visit(Child))
            return false;
      }

      return true;
   }

#  define CDOT_STMT(Name)                                                 \
   bool visit##Name(Name *stmt)                                           \
   {                                                                      \
      return visitChildren(stmt);                                         \
   }

#  define CDOT_EXPR(Name) CDOT_STMT(Name)
#  include "AST/AstNode.def"

   bool visit(Statement *node)
   {
      switch (node->getTypeID()) {
#     define CDOT_EXPR(Name)                                            \
         case AstNode::Name##ID:                                        \
            return static_cast<SubClass*>(this)                         \
               ->visit##Name(static_cast<Name*>(node));
#     define CDOT_STMT(Name)                                            \
         case AstNode::Name##ID:                                        \
            return static_cast<SubClass*>(this)                         \
               ->visit##Name(static_cast<Name*>(node));
#     include "AST/AstNode.def"

      default:
         llvm_unreachable("bad node kind!");
      }
   }
};

} // namespace ast
} // namespace cdot


#endif //CDOT_ASTVISITOR_H
