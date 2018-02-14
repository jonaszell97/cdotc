//
// Created by Jonas Zell on 06.02.18.
//

#include "ParentMap.h"

#include "AST/Traverse.h"
#include "AST/Statement/Statement.h"

#include <llvm/ADT/DenseMap.h>

namespace cdot {
namespace ast {
namespace {

class ParentMapImpl {
public:
   void updateParentMap(Statement *Stmt)
   {
      visitDirectChildren(Stmt, [Stmt, this](Statement *Child) {
         if (Child) {
            Map[Child] = Stmt;
            return updateParentMap(Child);
         }
      });
   }

   Statement *getParent(Statement *Child)
   {
      auto it = Map.find(Child);
      assert (it != Map.end() && "didn't update parent map for child");

      return it->getSecond();
   }

   void updateParent(Statement *Child, Statement *Parent)
   {
      Map[Child] = Parent;
   }

private:
   llvm::DenseMap<Statement*, Statement*> Map;
};

} // anonymous namespace

ParentMap::ParentMap()
   : pImpl(new ParentMapImpl)
{

}

ParentMap::~ParentMap()
{
   delete pImpl;
}

void ParentMap::updateParentMap(Statement *Stmt)
{
   reinterpret_cast<ParentMapImpl*>(pImpl)->updateParentMap(Stmt);
}

Statement* ParentMap::getParent(Statement *Child)
{
   return reinterpret_cast<ParentMapImpl*>(pImpl)->getParent(Child);
}

void ParentMap::updateParent(Statement *Child, Statement *Parent)
{
   reinterpret_cast<ParentMapImpl*>(pImpl)->updateParent(Child, Parent);
}

} // namespace ast
} // namespace cdot