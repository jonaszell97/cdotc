//
// Created by Jonas Zell on 18.02.18.
//

#ifndef CDOT_BUILTINCANDIDATEBUILDER_H
#define CDOT_BUILTINCANDIDATEBUILDER_H

#include "AST/Type.h"
#include "Basic/DeclarationName.h"
#include "Basic/Precedence.h"

#include <unordered_map>
#include <llvm/ADT/ArrayRef.h>

namespace cdot {
   struct CandidateSet;
   class FunctionType;
namespace ast {

class Expression;
class SemaPass;

class BuiltinCandidateBuilder {
public:
   explicit BuiltinCandidateBuilder(SemaPass &SP)
      : SP(SP)
   {

   }

   void addBuiltinCandidates(CandidateSet &CandSet,
                             DeclarationName opName,
                             llvm::ArrayRef<Expression*> args);

   void addBuiltinCandidates(CandidateSet &CandSet,
                             DeclarationName opName,
                             QualType lhsType);

   struct CachedOp {
      CachedOp(FunctionType *Ty, PrecedenceGroup PG = PrecedenceGroup())
         : Ty(Ty), PG(PG)
      { }

      FunctionType *Ty;
      PrecedenceGroup PG;
   };

private:
   SemaPass &SP;

   using CacheVec = std::vector<CachedOp>;
   using BuiltinKindMap = std::unordered_map<op::OperatorKind, CacheVec>;
   using CacheMap = std::unordered_map<QualType, BuiltinKindMap>;

   CacheMap Cache;

   BuiltinKindMap::iterator fillCache(BuiltinKindMap &Map,
                                      QualType ty,
                                      op::OperatorKind opKind);

   void getOpKindAndFix(DeclarationName opName,
                        op::OperatorKind &opKind,
                        FixKind &fix);
};

} // namespace ast
} // namespace cdot

#endif //CDOT_BUILTINCANDIDATEBUILDER_H
