//
// Created by Jonas Zell on 02.01.18.
//

#ifndef CDOT_CTFEENGINE_H
#define CDOT_CTFEENGINE_H

#include <llvm/ADT/ArrayRef.h>
#include <llvm/ADT/SmallPtrSet.h>
#include "../Variant/Variant.h"
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
      : val(std::move(Val)), dependentDecl(nullptr)
   {}

   explicit CTFEResult(ast::CallableDecl *C) : dependentDecl(C)
   {}

   explicit CTFEResult(llvm::MutableArrayRef<diag::DiagnosticBuilder> diags)
      : dependentDecl(nullptr),
        diagnostics(std::make_move_iterator(diags.begin()),
                    std::make_move_iterator(diags.end()))
   {}

   bool hadError() const
   {
      return dependentDecl || !diagnostics.empty();
   }

   operator bool() const
   {
      return !hadError();
   }

   Variant &getVal()
   {
      return val;
   }

   ast::CallableDecl *getDependentDecl() const
   {
      return dependentDecl;
   }

   std::vector<diag::DiagnosticBuilder> &getDiagnostics()
   {
      return diagnostics;
   }

private:
   cdot::Variant val;
   ast::CallableDecl *dependentDecl;

   std::vector<diag::DiagnosticBuilder> diagnostics;
};

class CTFEEngine {
public:
   explicit CTFEEngine(ast::SemaPass &SP);
   ~CTFEEngine();

   CTFEResult evaluateFunction(il::Function *F,
                               llvm::ArrayRef<Value> args,
                               SourceLocation loc = {});

   Value CTFEValueFromVariant(Variant const &V, Type *Ty);

private:
   EngineImpl *pImpl;
};

} // namespace ctfe
} // namespace cdot


#endif //CDOT_CTFEENGINE_H
