//
// Created by Jonas Zell on 17.11.17.
//

#ifndef CDOT_CONTEXT_H
#define CDOT_CONTEXT_H

#include "AST/Type.h"

#include <llvm/ADT/SmallVector.h>
#include <llvm/ADT/StringMap.h>
#include <llvm/ADT/APSInt.h>
#include <llvm/ADT/APFloat.h>
#include <llvm/ADT/DenseMap.h>

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

class Type;

namespace il {

class Module;
class Function;
class GlobalVariable;
class Value;
class ConstantInt;
class ConstantFloat;
class ConstantString;
class ConstantPointer;

class Context final {
public:
   using ModuleList         = llvm::SmallVector<Module*, 8>;
   using mod_iterator       = ModuleList::iterator;
   using mod_const_iterator = ModuleList::const_iterator;

   explicit Context(ast::ASTContext &ASTCtx);
   ~Context();

   void registerModule(Module *M) { Modules.push_back(M); }

   Function *getFunction(llvm::StringRef name);
   Function *getFunctionDefinition(llvm::StringRef name);

   GlobalVariable *getGlobal(llvm::StringRef name);
   GlobalVariable *getGlobalDefinition(llvm::StringRef name);

   ast::ASTContext &getASTCtx() const { return ASTCtx; }

   friend class ConstantInt;     // for IntConstants
   friend class ConstantFloat;   // for FPConstants
   friend class ConstantString;  // for StringConstants
   friend class ConstantPointer; // for NullConstants

private:
   llvm::SmallVector<Module*, 8> Modules;
   ast::ASTContext &ASTCtx;

   using IntMapTy = llvm::DenseMap<llvm::APSInt, std::unique_ptr<ConstantInt>,
                                   llvm::DenseMapAPIntKeyInfo>;

   using FPMapTy = llvm::DenseMap<llvm::APFloat, std::unique_ptr<ConstantFloat>,
                                  llvm::DenseMapAPFloatKeyInfo>;

   using StringMapTy = llvm::StringMap<std::unique_ptr<ConstantString>>;
   using NullPtrMapTy = llvm::DenseMap<QualType,
                                       std::unique_ptr<ConstantPointer>>;

   IntMapTy IntConstants;
   FPMapTy FPConstants;
   StringMapTy StringConstants;
   NullPtrMapTy NullConstants;

   ConstantInt *TrueVal  = nullptr;
   ConstantInt *FalseVal = nullptr;
};

} // namespace il
} // namespace cdot


#endif //CDOT_CONTEXT_H
