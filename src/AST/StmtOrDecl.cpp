#include "cdotc/AST/StmtOrDecl.h"

#include "cdotc/AST/Decl.h"
#include "cdotc/AST/Statement.h"

using namespace cdot::ast;

namespace cdot {

#define DISPATCH(METHOD, TYPE)                                                 \
   Union.isNull() ? TYPE()                                                     \
                  : Union.is<Statement*>() ? Union.get<Statement*>()->METHOD   \
                                           : Union.get<Decl*>()->METHOD

SourceLocation StmtOrDecl::getSourceLoc() const
{
   return DISPATCH(getSourceLoc(), SourceLocation);
}

SourceRange StmtOrDecl::getSourceRange() const
{
   return DISPATCH(getSourceRange(), SourceRange);
}

bool StmtOrDecl::isInvalid() const { return DISPATCH(isInvalid(), bool); }

void StmtOrDecl::setIsInvalid(bool b) const
{
   return DISPATCH(setIsInvalid(b), void);
}

bool StmtOrDecl::isTypeDependent() const
{
   return DISPATCH(isTypeDependent(), bool);
}

void StmtOrDecl::setTypeDependent(bool b) const
{
   return DISPATCH(setIsTypeDependent(b), void);
}

bool StmtOrDecl::isValueDependent() const
{
   return DISPATCH(isValueDependent(), bool);
}

void StmtOrDecl::setValueDependent(bool b) const
{
   return DISPATCH(setIsValueDependent(b), void);
}

void StmtOrDecl::copyStatusFlags(StmtOrDecl Other) const
{
   if (Union.isNull())
      return;

   if (Union.is<Statement*>()) {
      if (Other.Union.is<Statement*>()) {
         Union.get<Statement*>()->copyStatusFlags(
             Other.Union.get<Statement*>());
      }
      else {
         Union.get<Statement*>()->copyStatusFlags(Other.Union.get<Decl*>());
      }
   }
   else {
      if (Other.Union.is<Statement*>()) {
         Union.get<Decl*>()->copyStatusFlags(Other.Union.get<Statement*>());
      }
      else {
         Union.get<Decl*>()->copyStatusFlags(Other.Union.get<Decl*>());
      }
   }
}

} // namespace cdot