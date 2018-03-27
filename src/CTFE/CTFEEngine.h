//
// Created by Jonas Zell on 02.01.18.
//

#ifndef CDOT_CTFEENGINE_H
#define CDOT_CTFEENGINE_H

#include <llvm/ADT/ArrayRef.h>
#include <llvm/ADT/SmallPtrSet.h>
#include "Basic/Variant.h"
#include "../Message/Diagnostics.h"

namespace cdot {

namespace ast {
   class SemaPass;
   class CallableDecl;
} // namespace ast

namespace il {
   class Function;
} // namespace il;

namespace ctfe {

class EngineImpl;
class Value;

struct CTFEResult {
   explicit CTFEResult(Variant &&Val)
      : val(std::move(Val)), HadError(false)
   {}

   CTFEResult() : HadError(true)
   {}

   bool hadError() const
   {
      return HadError;
   }

   operator bool() const
   {
      return !hadError();
   }

   Variant &getVal()
   {
      return val;
   }

private:
   cdot::Variant val;
   bool HadError;
};

inline CTFEResult CTFEError() { return CTFEResult(); }

class CTFEEngine {
public:
   explicit CTFEEngine(ast::SemaPass &SP);
   ~CTFEEngine();

   CTFEResult evaluateFunction(il::Function *F,
                               llvm::ArrayRef<Value> args,
                               SourceLocation loc = {});

   Value CTFEValueFromVariant(Variant const &V, const QualType &Ty);

private:
   EngineImpl *pImpl;
};

} // namespace ctfe
} // namespace cdot


#endif //CDOT_CTFEENGINE_H
