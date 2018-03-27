//
// Created by Jonas Zell on 07.02.18.
//

#ifndef CDOT_STATICEVALUATOR_H
#define CDOT_STATICEVALUATOR_H

#include "Basic/Variant.h"
#include "Message/Diagnostics.h"

#include <llvm/ADT/SmallVector.h>
#include <llvm/ADT/ArrayRef.h>

namespace cdot {

namespace ast {
   class Expression;
   class SemaPass;
} // namespace ast

namespace diag {
   class DiagnosticBuilder;
} // namespace diag

class StaticEvaluator {
public:
   explicit StaticEvaluator(ast::SemaPass &SP);
   ~StaticEvaluator();

   struct StaticExprResult {
      StaticExprResult(Variant &&V)
         : HadError(false), TypeDependent(false), ValueDependent(false),
           Result(std::move(V))
      {}

      StaticExprResult(bool TypeDependent, bool ValueDependent)
         : TypeDependent(TypeDependent),
           ValueDependent(ValueDependent)
      { }

      StaticExprResult()
         : HadError(true)
      { }

      bool hadError() const
      {
         return HadError;
      }

      bool isTypeDependent() const
      {
         return TypeDependent;
      }

      bool isValueDependent() const
      {
         return ValueDependent;
      }

      Variant &getResult()
      {
         return Result;
      }

      operator bool() const
      {
         return !HadError && !TypeDependent && !ValueDependent;
      }

   private:
      bool HadError       : 1;
      bool TypeDependent  : 1;
      bool ValueDependent : 1;

      Variant Result;
   };

   StaticExprResult evaluate(ast::Expression *Expr);

private:
   void *pImpl;
};

} // namespace cdot

#endif //CDOT_STATICEVALUATOR_H
