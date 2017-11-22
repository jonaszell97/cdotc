//
// Created by Jonas Zell on 31.08.17.
//

#ifndef CDOT_CGCAST_H
#define CDOT_CGCAST_H

#include <llvm/IR/Value.h>
#include "../SemanticAnalysis/Record/Class.h"



namespace cdot {
class Type;

namespace ast {
class CodeGen;
}

namespace codegen {

class CGCast {
public:
   CGCast(ast::CodeGen &CGM);

   llvm::Value *applyCast(const QualType& from, const QualType& to,
                          llvm::Value *val);

   llvm::Value *integralCast(const QualType& from, const QualType& to, llvm::Value *val);
   llvm::Value *floatingPointCast(const QualType& from, const QualType& to, llvm::Value *val);

   llvm::Value *pointerCast(const QualType& from, const QualType& to, llvm::Value *val);
   llvm::Value *tupleCast(const QualType& from, const QualType& to, llvm::Value *val);

   cl::Method *hasCastOperator(const QualType& from, const QualType& to);
   llvm::Value *castOperator(const QualType& from, const QualType& to, llvm::Value *val,
      cl::Method*);

   llvm::Value *protoToProtoCast(const QualType& from, const QualType& to, llvm::Value *val);
   llvm::Value *castFromProtocol(const QualType& from, const QualType& to, llvm::Value *val);
   llvm::Value *castToProtocol(const QualType& from, const QualType& to, llvm::Value *val);

   llvm::Value *staticUpcast(const QualType& from, const QualType& to, llvm::Value *val);

   llvm::Value *dynamicDowncast(const QualType& from, const QualType& to, llvm::Value *val);

   llvm::Value *functionCast(
      const QualType& from,
      const QualType& to,
      llvm::Value *val,
      llvm::Function* func,
      bool isLambda,
      bool hasSelfParam,
      bool hasStructRet
   );

private:
   ast::CodeGen &CGM;
};

}
}

#endif //CDOT_CGCAST_H
