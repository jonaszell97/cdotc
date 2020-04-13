#ifndef CDOT_EMPTYASTVISITOR_H
#define CDOT_EMPTYASTVISITOR_H

namespace cdot {
namespace ast {

#define CDOT_STMT(NAME) class NAME;
#define CDOT_EXPR(NAME) class NAME;
#define CDOT_DECL(NAME) class NAME;

#include "AST/AstNode.def"
#include "AST/Decl.def"

template<class ExprRetTy = void,
         class StmtRetTy = ExprRetTy,
         class DeclRetTy = void,
         class ...ParamTys>
class EmptyASTVisitor {
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

#endif //CDOT_EMPTYASTVISITOR_H
