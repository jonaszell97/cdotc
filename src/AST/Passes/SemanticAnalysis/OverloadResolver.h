//
// Created by Jonas Zell on 08.11.17.
//

#ifndef CDOT_OVERLOADRESOLVER_H
#define CDOT_OVERLOADRESOLVER_H

#include <vector>

#include "AST/Passes/SemanticAnalysis/CandidateSet.h"
#include "AST/Passes/SemanticAnalysis/Template.h"

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
   OverloadResolver(SemaPass &SP,
                    llvm::ArrayRef<Expression*> givenArgs,
                    llvm::ArrayRef<Expression*> givenTemplateArgs,
                    Statement *Caller = nullptr);

   void resolve(CandidateSet &CandSet);
   void resolve(CandidateSet::Candidate &Cand,
                llvm::ArrayRef<Expression*> givenArgs,
                llvm::SmallVectorImpl<ConversionSequence> &Conversions);

   void isCallCompatible(CandidateSet::Candidate &comp,
                         llvm::ArrayRef<QualType> givenArgs,
                         FunctionType *FuncTy,
                         llvm::SmallVectorImpl<ConversionSequence> &Conversions,
                         size_t firstDefaultArg = size_t(-1));

   llvm::ArrayRef<Expression *> getGivenArgs()
   {
      return givenArgs;
   }

protected:
   SemaPass &SP;
   llvm::ArrayRef<Expression*> givenArgs;
   llvm::ArrayRef<Expression*> givenTemplateArgs;
   Statement *Caller;

   void resolveTemplateArgs(std::vector<QualType> &resolvedArgs,
                            llvm::ArrayRef<FuncArgDecl*> neededArgs,
                            sema::TemplateArgList const& templateArgs);

   void isVarargCallCompatible(CandidateSet::Candidate &comp,
                               llvm::ArrayRef<QualType> givenArgs,
                               FunctionType *FuncTy);
};

} // namespace ast
} // namespace cdot

#endif //CDOT_OVERLOADRESOLVER_H
