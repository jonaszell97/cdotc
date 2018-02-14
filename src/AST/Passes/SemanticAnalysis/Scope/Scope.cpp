//
// Created by Jonas Zell on 29.01.18.
//

#include "Scope.h"

#include "AST/Statement/Declaration/Class/MethodDecl.h"
#include "AST/Expression/Literal/LambdaExpr.h"

using namespace cdot::support;

namespace cdot {

unsigned long long BlockScope::lastID = 0;

MethodScope::MethodScope(ast::MethodDecl *M, Scope *enclosingScope)
   : FunctionScope(MethodScopeID, M, enclosingScope)
{

}

ast::MethodDecl* MethodScope::getMethodDecl() const
{
   return cast<ast::MethodDecl>(CD);
}

LambdaScope::LambdaScope(ast::LambdaExpr *L, Scope *enclosingScope)
   : FunctionScope(LambdaScopeID, L->getFunc(), enclosingScope),
     L(L)
{

}

} // namespace cdot