//
// Created by Jonas Zell on 15.12.17.
//

#include "PatternExpr.h"

namespace cdot {
namespace ast {

CasePattern::Argument::Argument(Argument &&arg)
   : sourceLoc(arg.sourceLoc), is_expr(arg.is_expr), is_const(arg.is_const)
{
   if (is_expr)
      new(&expr) std::shared_ptr<Expression>(move(arg.expr));
   else
      new(&identifier) std::string(move(arg.identifier));
}

CasePattern::Argument& CasePattern::Argument::operator=(Argument &&arg)
{
   destroyValue();

   sourceLoc = arg.sourceLoc;
   is_expr = arg.is_expr;
   is_const = arg.is_const;

   if (is_expr)
      new(&expr) std::shared_ptr<Expression>(move(arg.expr));
   else
      new(&identifier) std::string(move(arg.identifier));

   return *this;
}

void CasePattern::Argument::destroyValue()
{
   if (is_expr)
      expr.~shared_ptr();
   else
      identifier.~string();
}

CasePattern::Argument::~Argument()
{
   destroyValue();
}

} // namespace ast
} // namespace cdot