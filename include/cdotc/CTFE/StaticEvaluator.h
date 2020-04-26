#ifndef CDOT_STATICEVALUATOR_H
#define CDOT_STATICEVALUATOR_H

#include "cdotc/Diagnostics/Diagnostics.h"

#include <llvm/ADT/ArrayRef.h>
#include <llvm/ADT/SmallVector.h>

namespace cdot {

namespace ast {
class Expression;
class SemaPass;
} // namespace ast

namespace diag {
class DiagnosticBuilder;
} // namespace diag

namespace il {
class Constant;
} // namespace il

class StaticEvaluator {
public:
   explicit StaticEvaluator(ast::SemaPass& SP);
   ~StaticEvaluator();

   struct StaticExprResult {
      StaticExprResult(il::Constant* V)
          : HadError(false), TypeDependent(false), ValueDependent(false),
            Result(V)
      {
      }

      StaticExprResult(bool TypeDependent, bool ValueDependent)
          : HadError(false), TypeDependent(TypeDependent),
            ValueDependent(ValueDependent), Result(nullptr)
      {
      }

      StaticExprResult()
          : HadError(true), TypeDependent(false), ValueDependent(false),
            Result(nullptr)
      {
      }

      bool hadError() const { return HadError; }
      bool isTypeDependent() const { return TypeDependent; }
      bool isValueDependent() const { return ValueDependent; }

      il::Constant* getResult() { return Result; }

      operator bool() const
      {
         return !HadError && !TypeDependent && !ValueDependent;
      }

   private:
      bool HadError : 1;
      bool TypeDependent : 1;
      bool ValueDependent : 1;

      il::Constant* Result;
   };

   StaticExprResult evaluate(ast::Expression* Expr);

private:
   void* pImpl;
};

} // namespace cdot

#endif // CDOT_STATICEVALUATOR_H
