//
// Created by Jonas Zell on 08.11.17.
//

#ifndef CDOT_OVERLOADRESOLVER_H
#define CDOT_OVERLOADRESOLVER_H

#include "AST/StmtOrDecl.h"
#include "Sema/CandidateSet.h"

#include <vector>

namespace cdot {

class QualType;
class FunctionType;

namespace sema {
   class MultiLevelTemplateArgList;
} // namespace sema

namespace ast {

class Expression;
class SemaPass;
class StaticExpr;
class CallableDecl;
class EnumCaseDecl;
class FuncArgDecl;
class Statement;

class OverloadResolver {
public:
   OverloadResolver(SemaPass &SP,
                    DeclarationName FuncName,
                    Expression *SelfArg,
                    ArrayRef<Expression*> givenArgs,
                    ArrayRef<Expression*> givenTemplateArgs,
                    ArrayRef<IdentifierInfo*> givenLabels,
                    Statement *Caller = nullptr);

   using ConvSeqVec = SmallVectorImpl<ConversionSequenceBuilder>;
   using ArgVec     = SmallVectorImpl<StmtOrDecl>;

   void resolve(CandidateSet &CandSet);
   void resolve(CandidateSet &CandSet,
                CandidateSet::Candidate &Cand,
                llvm::ArrayRef<Expression*> givenArgs,
                ConvSeqVec &Conversions,
                ArgVec &ArgExprs);

protected:
   SemaPass &SP;
   DeclarationName FuncName;
   Expression *SelfArg;
   ArrayRef<Expression*> givenArgs;
   ArrayRef<Expression*> givenTemplateArgs;
   ArrayRef<IdentifierInfo*> givenLabels;
   Statement *Caller;
};

} // namespace ast
} // namespace cdot

#endif //CDOT_OVERLOADRESOLVER_H
