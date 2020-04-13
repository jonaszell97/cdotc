#ifndef CDOT_OVERLOADRESOLVER_H
#define CDOT_OVERLOADRESOLVER_H

#include "cdotc/AST/SourceType.h"
#include "cdotc/AST/StmtOrDecl.h"
#include "cdotc/Sema/CandidateSet.h"

#include <vector>

namespace cdot {

class QualType;
class FunctionType;

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
   OverloadResolver(SemaPass& SP, DeclarationName FuncName, Expression* SelfArg,
                    ArrayRef<Expression*> givenArgs,
                    ArrayRef<Expression*> givenTemplateArgs,
                    ArrayRef<IdentifierInfo*> givenLabels,
                    Statement* Caller = nullptr);

   using ConvSeqVec = SmallVectorImpl<ConversionSequenceBuilder>;
   using ArgVec = SmallVectorImpl<StmtOrDecl>;

   void resolve(CandidateSet& CandSet);
   void resolve(CandidateSet& CandSet, CandidateSet::Candidate& Cand,
                llvm::ArrayRef<Expression*> givenArgs, ConvSeqVec& Conversions,
                ArgVec& ArgExprs);

protected:
   SemaPass& SP;
   DeclarationName FuncName;
   Expression* SelfArg;
   ArrayRef<Expression*> givenArgs;
   ArrayRef<Expression*> givenTemplateArgs;
   ArrayRef<IdentifierInfo*> givenLabels;
   Statement* Caller;
};

} // namespace ast

namespace sema {

CandidateSet::Candidate* resolveCandidateSet(
    ast::SemaPass& Sema, CandidateSet& CandSet, ast::Expression* SelfArg,
    ArrayRef<ast::Expression*> UnorderedArgs, ArrayRef<IdentifierInfo*> Labels,
    ArrayRef<ast::Expression*> TemplateArgExprs, ast::SourceType RequiredType,
    ast::Statement* Caller, bool DiagnoseFailure = true,
    bool isFunctionArgument = false, ConstraintBuilder* outerBuilder = nullptr);

} // namespace sema
} // namespace cdot

#endif // CDOT_OVERLOADRESOLVER_H
