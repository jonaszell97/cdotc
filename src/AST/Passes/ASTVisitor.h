//
// Created by Jonas Zell on 07.02.18.
//

#ifndef CDOT_NULLASTVISITOR_H
#define CDOT_NULLASTVISITOR_H

#include "AST/AstDeclarations.h"

namespace cdot {
namespace ast {

template<class ExprRetTy = void,
         class StmtRetTy = ExprRetTy,
         class DeclRetTy = StmtRetTy,
         class ...ParamTys>
class ASTVisitor {
public:
#  define CDOT_STMT(Name)                                               \
   StmtRetTy visit##Name(Name*, ParamTys...) { return StmtRetTy(); }
#  define CDOT_EXPR(Name)                                               \
   ExprRetTy visit##Name(Name*, ParamTys...) { return ExprRetTy(); }
#  include "AST/AstNode.def"

#  define CDOT_DECL(Name)                                               \
   DeclRetTy visit##Name(Name*, ParamTys...) { return DeclRetTy(); }
#  include "AST/Decl.def"
};


} // namespace ast
} // namespace cdot

#endif //CDOT_NULLASTVISITOR_H
