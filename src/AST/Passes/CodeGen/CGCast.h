//
// Created by Jonas Zell on 31.08.17.
//

#ifndef CDOT_CGCAST_H
#define CDOT_CGCAST_H

#include <llvm/IR/Value.h>
#include "../SemanticAnalysis/Record/Class.h"

class CodeGen;

namespace cdot {
   class BuiltinType;

namespace codegen {

   class CGCast {
   public:
      CGCast(CodeGen &CGM);

      llvm::Value *applyCast(const Type& from, const Type& to,
                             llvm::Value *val);

      llvm::Value *integralCast(const Type& from, const Type& to, llvm::Value *val);
      llvm::Value *floatingPointCast(const Type& from, const Type& to, llvm::Value *val);

      llvm::Value *pointerCast(const Type& from, const Type& to, llvm::Value *val);
      llvm::Value *tupleCast(const Type& from, const Type& to, llvm::Value *val);

      cl::Method *hasCastOperator(const Type& from, const Type& to);
      llvm::Value *castOperator(const Type& from, const Type& to, llvm::Value *val,
         cl::Method*);

      llvm::Value *protoToProtoCast(const Type& from, const Type& to, llvm::Value *val);
      llvm::Value *castFromProtocol(const Type& from, const Type& to, llvm::Value *val);
      llvm::Value *castToProtocol(const Type& from, const Type& to, llvm::Value *val);

      llvm::Value *staticUpcast(const Type& from, const Type& to, llvm::Value *val);

      llvm::Value *dynamicDowncast(const Type& from, const Type& to, llvm::Value *val);

      llvm::Value *functionCast(
         const Type& from,
         const Type& to,
         llvm::Value *val,
         llvm::Function* func,
         bool isLambda,
         bool hasSelfParam,
         bool hasStructRet
      );

   private:
      CodeGen &CGM;
   };

}
}

#endif //CDOT_CGCAST_H
