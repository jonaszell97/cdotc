//
// Created by Jonas Zell on 19.01.18.
//

#ifndef CDOT_TRAVERSE_H
#define CDOT_TRAVERSE_H

#include <llvm/ADT/SmallVector.h>

namespace cdot {
namespace ast {

class Statement;

void collectDirectChildren(llvm::SmallVectorImpl<Statement*> &children,
                           Statement const* stmt);

inline void collectChildren(llvm::SmallVectorImpl<Statement*> &children,
                            Statement const* stmt) {
   auto size = children.size();
   collectDirectChildren(children, stmt);

   for (auto i = size; i < children.size(); ++i)
      collectChildren(children, children[i]);
}

template<class Fn>
inline void visitAllChildren(Statement const* stmt, Fn const& fn)
{
   llvm::SmallVector<Statement*, 8> children;
   collectChildren(children, stmt);

   for (auto &c : children) fn(c);
}

template<class Fn>
inline void visitDirectChildren(Statement const* stmt, Fn const& fn)
{
   llvm::SmallVector<Statement*, 8> children;
   collectDirectChildren(children, stmt);

   for (auto &c : children) fn(c);
}

} // namespace ast
} // namespace cdot

#endif //CDOT_TRAVERSE_H
