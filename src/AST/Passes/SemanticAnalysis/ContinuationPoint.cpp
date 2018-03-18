//
// Created by Jonas Zell on 13.03.18.
//

#include "ContinuationPoint.h"
#include "SemaPass.h"

namespace cdot {
namespace ast {

ContinuationPoint::ContinuationPoint(SemaPass &SP, Statement *Stmt)
   : StmtOrDecl(Stmt), scope(SP.getScope()),
     declContext(&SP.getDeclContext())
{}

ContinuationPoint::ContinuationPoint(SemaPass &SP, Decl *D)
   : StmtOrDecl(D), scope(SP.getScope()),
     declContext(&SP.getDeclContext())
{}

} // namespace ast
} // namespace cdot
