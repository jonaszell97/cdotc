#include "cdotc/Sema/Scope/Scope.h"

#include "cdotc/AST/Decl.h"
#include "cdotc/AST/Expression.h"

using namespace cdot::support;

namespace cdot {

ast::CallableDecl* FunctionScope::getCallableDecl()
{
   if (auto L = dyn_cast<LambdaScope>(this))
      return L->getLambdaExpr()->getFunc();

   return CD;
}

MethodScope::MethodScope(ast::MethodDecl* M, bool InferrableReturnType,
                         Scope* enclosingScope)
    : FunctionScope(MethodScopeID, M, InferrableReturnType, enclosingScope)
{
}

ast::MethodDecl* MethodScope::getMethodDecl() const
{
   return cast<ast::MethodDecl>(CD);
}

LambdaScope::LambdaScope(ast::LambdaExpr* L, Scope* enclosingScope)
    : FunctionScope(LambdaScopeID, L->getFunc(), true, enclosingScope), L(L)
{
}

StaticForScope::StaticForScope(IdentifierInfo* elementName, QualType elementTy,
                               Scope* enclosingScope)
    : Scope(StaticForScopeID, enclosingScope), elementName(elementName),
      elementTy(elementTy)
{
}

} // namespace cdot