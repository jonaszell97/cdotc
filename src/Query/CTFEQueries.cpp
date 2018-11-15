//
// Created by Jonas Zell on 25.08.18.
//
#include "Query.h"

#include "IL/Constants.h"
#include "QueryContext.h"
#include "Sema/SemaPass.h"

using namespace cdot;
using namespace cdot::ast;
using namespace cdot::diag;
using namespace cdot::support;

QueryResult ResolveStaticExprQuery::run()
{
   llvm_unreachable("unimplemented");
}

QueryResult ResolveStaticExprToBoolQuery::run()
{
   il::Constant *Result;
   if (QC.ResolveStaticExpr(Result, Expr)) {
      return fail();
   }

   if (!Result->getType()->isInt1Ty()) {
      sema().diagnose(Expr, err_generic_error, "value must be boolean",
                      Expr->getSourceRange());

      return finish(false, DoneWithError);
   }

   return finish(cast<il::ConstantInt>(Result)->getBoolValue());
}