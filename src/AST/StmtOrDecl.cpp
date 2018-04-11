//
// Created by Jonas Zell on 09.04.18.
//

#include "StmtOrDecl.h"

#include "Decl.h"
#include "Statement.h"

using namespace cdot::ast;

namespace cdot {

#define DISPATCH(METHOD) Union.is<Statement*>()   \
   ? Union.get<Statement*>()->METHOD            \
   : Union.get<Decl*>()->METHOD

SourceLocation StmtOrDecl::getSourceLoc() const
{
   return DISPATCH(getSourceLoc());
}

SourceRange StmtOrDecl::getSourceRange() const
{
   return DISPATCH(getSourceRange());
}

bool StmtOrDecl::isInvalid() const
{
   return DISPATCH(isInvalid());
}

void StmtOrDecl::setIsInvalid(bool b) const
{
   return DISPATCH(setIsInvalid(b));
}

void StmtOrDecl::copyStatusFlags(StmtOrDecl Other) const
{
   if (Union.is<Statement*>()) {
      if (Other.Union.is<Statement*>()) {
         Union.get<Statement*>()
            ->copyStatusFlags(Other.Union.get<Statement*>());
      }
      else {
         Union.get<Statement*>()
              ->copyStatusFlags(Other.Union.get<Decl*>());
      }
   }
   else {
      if (Other.Union.is<Statement*>()) {
         Union.get<Decl*>()
              ->copyStatusFlags(Other.Union.get<Statement*>());
      }
      else {
         Union.get<Decl*>()
              ->copyStatusFlags(Other.Union.get<Decl*>());
      }
   }
}

} // namespace cdot