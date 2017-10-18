//
// Created by Jonas Zell on 06.10.17.
//

#ifndef CDOT_CGEXCEPTION_H
#define CDOT_CGEXCEPTION_H

#include <llvm/IR/Value.h>
#include <llvm/IR/IRBuilder.h>

class CodeGen;

namespace cdot {
namespace eh {

   class CGException {
   public:
      CGException(CodeGen &CGM);

      llvm::Constant *getBeginCatchFn();
      llvm::Constant *getEndCatchFn();
      llvm::Constant *getThrowFn();
      llvm::Constant *getRethrowFn();
      llvm::Constant *getAllocExcFn();

      llvm::Constant *getPersonalityFn();

   protected:
      CodeGen &CGM;

      llvm::Constant *BeginCatchFn = nullptr;
      llvm::Constant *EndCatchFn = nullptr;
      llvm::Constant *ThrowFn = nullptr;
      llvm::Constant *RethrowFn = nullptr;
      llvm::Constant *AllocExcFn = nullptr;
      llvm::Constant *PersonalityFn = nullptr;
   };

}
}

#endif //CDOT_CGEXCEPTION_H
