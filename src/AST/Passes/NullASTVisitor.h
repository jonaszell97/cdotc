//
// Created by Jonas Zell on 07.02.18.
//

#ifndef CDOT_NULLASTVISITOR_H
#define CDOT_NULLASTVISITOR_H

#include "AST/AstDeclarations.h"

namespace cdot {
namespace ast {

template<class RetTy = void, class ...ParamTys>
class NullASTVisitor {
public:
#  define CDOT_EXPR(Name)                                   \
   RetTy visit##Name(Name*, ParamTys...) { return RetTy(); }
#  include "../AstNode.def"

#  define CDOT_EXPR(Name)
#  define CDOT_STMT(Name)                                   \
   void visit##Name(Name*, ParamTys...) { }
#  include "AST/AstNode.def"
};


} // namespace ast
} // namespace cdot

#endif //CDOT_NULLASTVISITOR_H
