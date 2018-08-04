//
// Created by Jonas Zell on 17.11.17.
//

#ifndef CDOT_CONTEXT_H
#define CDOT_CONTEXT_H

#include "IL/Constants.h"

#include <llvm/ADT/APFloat.h>
#include <llvm/ADT/APSInt.h>
#include <llvm/ADT/DenseMap.h>
#include <llvm/ADT/FoldingSet.h>
#include <llvm/ADT/SmallPtrSet.h>
#include <llvm/ADT/StringMap.h>

namespace llvm {

struct DenseMapAPIntKeyInfo {
   static inline APSInt getEmptyKey() {
      APInt V(nullptr, 0);
      V.U.VAL = 0;
      return APSInt(V);
   }

   static inline APSInt getTombstoneKey() {
      APInt V(nullptr, 0);
      V.U.VAL = 1;
      return APSInt(V);
   }

   static unsigned getHashValue(const APSInt &Key)
   {
      return static_cast<unsigned>(hash_value(Key)) ^ Key.isUnsigned();
   }

   static bool isEqual(const APSInt &LHS, const APSInt &RHS)
   {
      return LHS.getBitWidth() == RHS.getBitWidth()
             && LHS.isUnsigned() == RHS.isUnsigned()
             && LHS == RHS;
   }
};

struct DenseMapAPFloatKeyInfo {
   static inline APFloat getEmptyKey()
   { return APFloat(APFloat::Bogus(), 1); }

   static inline APFloat getTombstoneKey()
   { return APFloat(APFloat::Bogus(), 2); }

   static unsigned getHashValue(const APFloat &Key) {
      return static_cast<unsigned>(hash_value(Key));
   }

   static bool isEqual(const APFloat &LHS, const APFloat &RHS) {
      return LHS.bitwiseIsEqual(RHS);
   }
};

} // namespace llvm

namespace cdot {
namespace ast {
   class ASTContext;
   class Decl;
} // namespace ast

class CompilerInstance;
class Type;

namespace il {

class Module;
class Function;
class GlobalVariable;

class Context final {
public:
   using ModuleList         = llvm::SmallPtrSet<Module*, 4>;

   explicit Context(CompilerInstance &CU);
   ~Context();

   void registerModule(Module *M);
   void removeModule(Module *M);
   const llvm::SmallPtrSetImpl<Module*> &getModules() const { return Modules; }

   Function *getFunction(llvm::StringRef name);
   Function *getFunctionDefinition(llvm::StringRef name);

   GlobalVariable *getGlobal(llvm::StringRef name);
   GlobalVariable *getGlobalDefinition(llvm::StringRef name);

   ast::ASTContext &getASTCtx() const;
   CompilerInstance &getCompilation() const { return CI; }

   friend class ConstantInt;     // for IntConstants
   friend class ConstantFloat;   // for FPConstants
   friend class ConstantString;  // for StringConstants
   friend class ConstantPointer; // for NullConstants
   friend class ConstantArray;
   friend class ConstantTuple;
   friend class ConstantStruct;
   friend class ConstantClass;
   friend class ConstantUnion;
   friend class ConstantEnum;
   friend class UndefValue;
   friend class ConstantExpr;
   friend class MagicConstant;
   friend class ConstantTokenNone;
   friend class TypeInfo;
   friend class VTable;

private:
   CompilerInstance &CI;
   ModuleList Modules;

   using IntMapTy = llvm::DenseMap<llvm::APSInt, std::unique_ptr<ConstantInt>,
                                   llvm::DenseMapAPIntKeyInfo>;

   using FPMapTy = llvm::DenseMap<llvm::APFloat, std::unique_ptr<ConstantFloat>,
                                  llvm::DenseMapAPFloatKeyInfo>;

   using StringMapTy = llvm::StringMap<std::unique_ptr<ConstantString>>;
   using NullPtrMapTy = llvm::DenseMap<QualType,
                                       std::unique_ptr<ConstantPointer>>;
   using UndefMapTy = llvm::DenseMap<QualType, std::unique_ptr<UndefValue>>;

   IntMapTy IntConstants;
   FPMapTy FP32Constants;
   FPMapTy FP64Constants;
   StringMapTy StringConstants;
   NullPtrMapTy NullConstants;
   UndefMapTy UndefConstants;
   il::MagicConstant *MagicConstants[4] = { nullptr };

   llvm::FoldingSet<ConstantArray> ArrayConstants;
   llvm::FoldingSet<ConstantTuple> TupleConstants;
   llvm::FoldingSet<ConstantStruct> StructConstants;
   llvm::FoldingSet<ConstantClass> ClassConstants;
   llvm::FoldingSet<ConstantEnum> EnumConstants;
   llvm::FoldingSet<ConstantUnion> UnionConstants;

   llvm::DenseMap<QualType, ConstantArray*> AllZeroArrayConstants;
   llvm::DenseMap<QualType, ConstantTuple*> AllZeroTupleConstants;
   llvm::DenseMap<QualType, ConstantStruct*> AllZeroStructConstants;

   llvm::FoldingSet<ConstantBitCastInst> BitCastConstants;
   llvm::FoldingSet<ConstantAddrOfInst> AddrOfConstants;
   llvm::FoldingSet<ConstantIntCastInst> IntCastConstants;
   llvm::FoldingSet<ConstantOperatorInst> OperatorConstants;
   llvm::FoldingSet<ConstantGEPInst> GEPConstants;
   llvm::FoldingSet<ConstantLoadInst> LoadConstants;

   llvm::DenseMap<QualType, TypeInfo*> TypeInfoMap;
   llvm::DenseMap<void*, VTable*> VTableMap;

   ConstantTokenNone *TokNone = nullptr;
   ConstantInt *TrueVal  = nullptr;
   ConstantInt *FalseVal = nullptr;
   ConstantTuple *EmptyTuple = nullptr;
};

} // namespace il
} // namespace cdot


#endif //CDOT_CONTEXT_H
