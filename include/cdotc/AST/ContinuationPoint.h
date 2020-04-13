#ifndef CDOT_CONTINUATIONPOINT_H
#define CDOT_CONTINUATIONPOINT_H

namespace cdot {

class Scope;

namespace ast {
class DeclContext;
} // namespace ast

struct ContinuationPoint {
   ContinuationPoint(Scope* S = nullptr, ast::DeclContext* Ctx = nullptr)
       : S(S), Ctx(Ctx)
   {
   }

   Scope* S;
   ast::DeclContext* Ctx;
};

} // namespace cdot

#endif // CDOT_CONTINUATIONPOINT_H
