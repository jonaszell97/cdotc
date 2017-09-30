//
// Created by Jonas Zell on 31.08.17.
//

#ifndef CDOT_CGCAST_H
#define CDOT_CGCAST_H

#include <llvm/IR/Value.h>
#include <llvm/IR/IRBuilder.h>
#include "../StaticAnalysis/Class.h"

namespace cdot {
   class Type;

namespace codegen {

   class CGCast {
   public:
      static llvm::Value *applyCast(Type *from, Type *to, llvm::Value *val, llvm::IRBuilder<> &Builder);

      static llvm::Value *integralCast(Type *from, Type *to, llvm::Value *val, llvm::IRBuilder<> &Builder);
      static llvm::Value *floatingPointCast(Type *from, Type *to, llvm::Value *val, llvm::IRBuilder<> &Builder);

      static llvm::Value *pointerCast(Type *from, Type *to, llvm::Value *val, llvm::IRBuilder<> &Builder);
      static llvm::Value *tupleCast(Type *from, Type *to, llvm::Value *val, llvm::IRBuilder<> &Builder);

      static CallCompatability hasCastOperator(Type* from, Type* to, llvm::IRBuilder<> &Builder);
      static llvm::Value *castOperator(Type *from, Type *to, llvm::Value *val,
         CallCompatability&, llvm::IRBuilder<> &Builder);

      static llvm::Value *protoToProtoCast(Type *from, Type *to, llvm::Value *val, llvm::IRBuilder<> &Builder);
      static llvm::Value *castFromProtocol(Type *from, Type *to, llvm::Value *val, llvm::IRBuilder<> &Builder);
      static llvm::Value *castToProtocol(Type *from, Type *to, llvm::Value *val, llvm::IRBuilder<> &Builder);

      static llvm::Value *staticUpcast(Type *from, Type *to, llvm::Value *val, llvm::IRBuilder<> &Builder);

      static llvm::Value *dynamicDowncast(Type *from, Type *to, llvm::Value *val, llvm::IRBuilder<> &Builder);

   private:
      CGCast() = default;
   };

}
}

#endif //CDOT_CGCAST_H
