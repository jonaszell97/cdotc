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
class LambdaExpr;
class Statement;

} // namespace ast

namespace sema {

CandidateSet::Candidate* resolveCandidateSet(
    ast::SemaPass& Sema, CandidateSet& CandSet, ast::Expression* SelfArg,
    ArrayRef<ast::Expression*> UnorderedArgs, ArrayRef<IdentifierInfo*> Labels,
    ArrayRef<ast::Expression*> TemplateArgExprs, ast::SourceType RequiredType,
    ast::Statement* Caller, bool DiagnoseFailure = true,
    bool isFunctionArgument = false, ConstraintBuilder* outerBuilder = nullptr);

void markCaptures(ast::SemaPass& Sema, ast::LambdaExpr *Expr);

} // namespace sema
} // namespace cdot

#endif // CDOT_OVERLOADRESOLVER_H
