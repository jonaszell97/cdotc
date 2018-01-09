//
// Created by Jonas Zell on 25.06.17.
//

#ifndef CDOT_VISITOR_H
#define CDOT_VISITOR_H

#include <vector>

#include "../Expression/Expression.h"
#include "../Statement/Statement.h"

#include "../AstDeclarations.h"
#include "../../Compiler.h"

namespace cdot {
namespace ast {

class ASTPass {
public:
   enum TypeID {
#  define CDOT_AST_PASS(Name) \
      Name##ID,
#  define CDOT_INCLUDE_ALL
#  include "Passes.def"
   };

   explicit ASTPass(TypeID id) : typeID(id)
   {}

   void RunOn(std::vector<CompilationUnit> &CUs);

   TypeID getTypeID() const
   {
      return typeID;
   }

protected:
   TypeID typeID;
};

template<class SubClass, class RetTy>
class AbstractPass: public ASTPass {
public:

   RetTy visit(Expression *node);
   void visit(Statement *node);

   template<class T>
   auto visit(const std::shared_ptr<T> &node) -> decltype(visit(node.get()))
   {
      return visit(node.get());
   }

#  define CDOT_EXPR(Name)                                   \
   RetTy visit##Name(Name*) { return {}; }
#  include "../AstNode.def"

#  define CDOT_EXPR(Name)
#  define CDOT_STMT(Name)                                   \
   void visit##Name(Name*) { }
#  include "../AstNode.def"

protected:
   explicit AbstractPass(TypeID typeID) : ASTPass(typeID)
   {}

   void deferVisit(const std::shared_ptr<AstNode> &node)
   {
      DeferredNodes.push_back(node);
   }

   void visitDeferred()
   {
      while (!DeferredNodes.empty()) {
         auto next = DeferredNodes.back();
         DeferredNodes.pop_back();
      }
   }

   std::vector<std::shared_ptr<AstNode>> DeferredNodes;
};

template<class SubClass>
class AbstractPass<SubClass, void>: public ASTPass {
public:
   void visit(AstNode *node);

   void visit(const std::shared_ptr<AstNode> &node)
   {
      visit(node.get());
   }

#  define CDOT_ASTNODE(Name) \
   void visit##Name(Name*) {}
#  include "../AstNode.def"

protected:
   explicit AbstractPass(TypeID typeID) : ASTPass(typeID)
   {}

   void deferVisit(const std::shared_ptr<AstNode> &node)
   {
      DeferredNodes.push_back(node);
   }

   void visitDeferred()
   {
      while (!DeferredNodes.empty()) {
         auto next = DeferredNodes.back();
         DeferredNodes.pop_back();
      }
   }

   std::vector<std::shared_ptr<AstNode>> DeferredNodes;
};

}
}


#endif //CDOT_VISITOR_H
