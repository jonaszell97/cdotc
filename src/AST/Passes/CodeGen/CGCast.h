//
// Created by Jonas Zell on 31.08.17.
//

#ifndef CDOT_CGCAST_H
#define CDOT_CGCAST_H

#include <llvm/IR/Value.h>
#include "../StaticAnalysis/Record/Class.h"

class CodeGen;

namespace cdot {
   class BuiltinType;

namespace codegen {

   class CGCast {
   public:
      CGCast(CodeGen &CGM);

      llvm::Value *applyCast(Type& from, Type& to, llvm::Value *val);

      llvm::Value *integralCast(Type& from, Type& to, llvm::Value *val);
      llvm::Value *floatingPointCast(Type& from, Type& to, llvm::Value *val);

      llvm::Value *pointerCast(Type& from, Type& to, llvm::Value *val);
      llvm::Value *tupleCast(Type& from, Type& to, llvm::Value *val);

      CallCompatability hasCastOperator(Type& from, Type& to);
      llvm::Value *castOperator(Type& from, Type& to, llvm::Value *val,
         CallCompatability&);

      llvm::Value *protoToProtoCast(Type& from, Type& to, llvm::Value *val);
      llvm::Value *castFromProtocol(Type& from, Type& to, llvm::Value *val);
      llvm::Value *castToProtocol(Type& from, Type& to, llvm::Value *val);

      llvm::Value *staticUpcast(Type& from, Type& to, llvm::Value *val);

      llvm::Value *dynamicDowncast(Type& from, Type& to, llvm::Value *val);

      llvm::Value *functionCast(
         Type& from,
         Type& to,
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
