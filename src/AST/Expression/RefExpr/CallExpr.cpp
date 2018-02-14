//
// Created by Jonas Zell on 21.06.17.
//

#include "CallExpr.h"
#include "IdentifierRefExpr.h"
#include "../TypeRef.h"

#include "../../SymbolTable.h"

#include "../../Passes/SemanticAnalysis/Builtin.h"
#include "../../Statement/Declaration/CallableDecl.h"
#include "../../Statement/Declaration/Class/MethodDecl.h"

namespace cdot {
namespace ast {


CallExpr::CallExpr(std::vector<Expression* > &&args,
                   string &&ident)
   : IdentifiedExpr(CallExprID, move(ident)), args(move(args)),
     is_pointer_access(false),
     implicitSelf(false),
     builtinFnKind(BuiltinFn::None)
{

}

CallExpr::CallExpr(std::vector<Expression *> &&args, CallableDecl *C)
   : IdentifiedExpr(CallExprID, ""), args(move(args)),
     is_pointer_access(false),
     implicitSelf(false),
     builtinFnKind(BuiltinFn::None)
{
   if (auto F = support::dyn_cast<FunctionDecl>(C)) {
      func = F;
      returnType = func->getReturnType()->getType();

      kind = CallKind::NamedFunctionCall;
   }
   else {
      method = support::cast<MethodDecl>(C);
      returnType = method->getReturnType()->getType();

      if (method->isStatic())
         kind = CallKind::StaticMethodCall;
      else
         kind = CallKind::MethodCall;
   }
}

} // namespace ast
} // namespace cdot