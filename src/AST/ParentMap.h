//
// Created by Jonas Zell on 06.02.18.
//

#ifndef CDOT_PARENTMAP_H
#define CDOT_PARENTMAP_H

namespace cdot {
namespace ast {

class Statement;

class ParentMap {
public:
   ParentMap();
   ~ParentMap();

   void updateParentMap(Statement *Stmt);
   Statement *getParent(Statement *Child);
   void updateParent(Statement *Child, Statement *Parent);

private:
   void *pImpl;
};

} // namespace ast
} // namespace cdot

#endif //CDOT_PARENTMAP_H
