//
// Created by Jonas Zell on 08.11.17.
//

#ifndef CDOT_OVERLOADRESOLVER_H
#define CDOT_OVERLOADRESOLVER_H

#include <vector>

#include "Util.h"
#include "Message/Diagnostics.h"

#include "AST/Passes/SemanticAnalysis/Template.h"

namespace cdot {

class QualType;
class FunctionType;

namespace ast {

class Expression;
class TypeRef;
class SemaPass;
class StaticExpr;
class CallableDecl;
class EnumCaseDecl;
class FuncArgDecl;
class Statement;

enum class FailureReason : unsigned char {
   None,
   IncompatibleArgCount,
   IncompatibleArgument,
   IncompatibleTemplateArg,
   CouldNotInferTemplateArg,
   FailedConstraint
};

struct CallCompatability {
   bool isCompatible() const
   {
      return failureReason == FailureReason::None;
   }

   bool isPerfectMatch()   const { return perfectMatch; }
   size_t getCastPenalty() const { return castPenalty; }

   FailureReason getFailureReason() const
   {
      return failureReason;
   }

   FailureReason failureReason = FailureReason::None;

   bool perfectMatch = false;

   union {
      size_t castPenalty = 0;
      StaticExpr *failedConstraint;
   };

   FunctionType* FuncTy;
   sema::TemplateArgList templateArgList;
   sema::TemplateArgList initializerTemplateArgList;

   size_t incompatibleArg;
   llvm::SmallVector<diag::DiagnosticBuilder, 4> diagnostics;
};

class OverloadResolver {
public:
   OverloadResolver(SemaPass &SP,
                    llvm::ArrayRef<Expression*> givenArgs,
                    const std::vector<TemplateArgExpr*> &givenTemplateArgs,
                    Statement *Caller = nullptr);

   CallCompatability checkIfViable(CallableDecl *callable);

   CallCompatability checkIfViable(FunctionType *funcTy);
   CallCompatability checkIfViable(EnumCaseDecl *Case);

   void isCallCompatible(CallCompatability &comp,
                         llvm::ArrayRef<QualType> givenArgs,
                         FunctionType *FuncTy,
                         size_t firstDefaultArg = size_t(-1));

   llvm::ArrayRef<Expression *> getGivenArgs()
   {
      return givenArgs;
   }

protected:
   SemaPass &SP;
   llvm::ArrayRef<Expression*> givenArgs;
   const std::vector<TemplateArgExpr*> &givenTemplateArgs;
   Statement *Caller;

   std::vector<QualType> resolveTemplateArgs(
                                    llvm::ArrayRef<FuncArgDecl*> neededArgs,
                                    sema::TemplateArgList const& templateArgs);

   void isVarargCallCompatible(CallCompatability &comp,
                               llvm::ArrayRef<QualType> givenArgs,
                               FunctionType *FuncTy);
};

} // namespace ast
} // namespace cdot

#endif //CDOT_OVERLOADRESOLVER_H
