//
// Created by Jonas Zell on 02.01.18.
//

#ifndef CDOT_CTFEENGINE_H
#define CDOT_CTFEENGINE_H

#include <llvm/ADT/ArrayRef.h>
#include "../Variant/Variant.h"
#include "../Message/Diagnostics.h"

namespace cdot {

class Callable;

namespace il {
   class Function;
} // namespace il;

namespace ctfe {

class EngineImpl;
class Value;

struct CTFEResult {
   explicit CTFEResult(Variant &&Val)
      : val(std::move(Val)), hadError(false)
   {}

   explicit
   CTFEResult(llvm::SmallVector<diag::DiagnosticBuilder, 4> &&diagnostics)
      : hadError(true), diagnostics(std::move(diagnostics))
   {}

   explicit
   CTFEResult(llvm::SmallPtrSet<Callable*, 8> &&Dependencies)
      : hadError(false), Dependencies(std::move(Dependencies))
   {}

   cdot::Variant val;
   bool hadError;
   llvm::SmallVector<diag::DiagnosticBuilder, 4> diagnostics;
   llvm::SmallPtrSet<Callable*, 8> Dependencies;
};

class CTFEEngine {
public:
   CTFEEngine();
   ~CTFEEngine();

   CTFEResult evaluateFunction(il::Function *F,
                               llvm::ArrayRef<Value> args,
                               SourceLocation loc = {});

private:
   EngineImpl *pImpl;
};

} // namespace ctfe
} // namespace cdot


#endif //CDOT_CTFEENGINE_H
