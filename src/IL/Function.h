#ifndef CDOT_ILFUNCTION_H
#define CDOT_ILFUNCTION_H

#include "Argument.h"
#include "BasicBlock.h"

#include "MetaData.h"
#include "SymbolTableList.h"
#include "GlobalVariable.h"

#include <llvm/ADT/SmallVector.h>
#include <llvm/ADT/ArrayRef.h>

namespace cdot {

enum class KnownFunction   : unsigned char;
enum class ConstructorKind : unsigned char;

namespace serial {
   class LazyILFunctionInfo;
} // namespace serial

namespace il {

class BasicBlock;
class Module;
class Argument;

class Function: public GlobalObject,
                public llvm::ilist_node_with_parent<Function, Module> {
public:
   using BasicBlockList = SymbolTableList<BasicBlock>;
   using iterator       = BasicBlockList::iterator;
   using const_iterator = BasicBlockList::const_iterator;

   Function(llvm::StringRef name,
            FunctionType *funcTy,
            Module *parent);

   ~Function();

   llvm::StringRef getUnmangledName() const;

   Module *getParent() const { return parent; }
   void setParent(Module *p) { parent = p; }

   BasicBlockList const& getBasicBlocks() const { return BasicBlocks; };
   BasicBlockList& getBasicBlocks() { return BasicBlocks; };

   QualType getReturnType() const
   {
      return type->asFunctionType()->getReturnType();
   }

   bool isDeclared() const { return FnBits.Declared; }
   bool mightThrow() const { return FnBits.Throws; }
   bool isCStyleVararg() const { return FnBits.Vararg; }
   bool isLambda() const;

   bool hasStructReturn() const { return FnBits.SRet; }
   void setStructReturn(bool V) { FnBits.SRet = V; }

   bool isAsync() const { return FnBits.Async; }
   void setAsync(bool V) { FnBits.Async = V; }

   bool isGlobalCtor() const { return FnBits.GlobalCtor; }
   bool isGlobalDtor() const { return FnBits.GlobalDtor; }

   void setGlobalCtor(bool ctor) { FnBits.GlobalCtor = ctor; }
   void setGlobalDtor(bool dtor) { FnBits.GlobalDtor = dtor; }

   void addDefinition() { FnBits.Declared = false; }
   void setDeclared(bool b) { FnBits.Declared = b; }

   bool isInvalid() const { return FnBits.Invalid; }
   void setInvalid(bool V) { FnBits.Invalid = V; }

   bool isReadyForCtfe() const { return FnBits.ReadyForCtfe; }
   void setReadyForCtfe(bool V) { FnBits.ReadyForCtfe = V; }

   Function *getDeclarationIn(Module *M);

   ValueSymbolTable* getSymTab() const
   { return BasicBlocks.getSymTab().get(); }

   BasicBlock const* getEntryBlock() const;
   BasicBlock* getEntryBlock();

   iterator begin() { return BasicBlocks.begin(); }
   iterator end() { return BasicBlocks.end(); }
   const_iterator begin() const { return BasicBlocks.begin(); }
   const_iterator end() const { return BasicBlocks.end(); }

   const il::BasicBlock& front() const { return BasicBlocks.front(); }
   il::BasicBlock& front() { return BasicBlocks.front(); }

   const il::BasicBlock& back() const { return BasicBlocks.back(); }
   il::BasicBlock& back() { return BasicBlocks.back(); }

   unsigned size() const { return (unsigned)BasicBlocks.size(); }

   bool isGlobalInitFn() const;
   uint16_t getPriority() const { return Priority; }
   void setPriority(uint16_t Priority) { Function::Priority = Priority; }

   KnownFunction getKnownFnKind() const { return knownFnKind; }
   void setKnownFnKind(KnownFunction kind) { knownFnKind = kind; }

   serial::LazyILFunctionInfo* getLazyFnInfo() const { return LazyFnInfo; }
   void setLazyFnInfo(serial::LazyILFunctionInfo* V) { LazyFnInfo = V; }

   bool overridePreviousDefinition() const { return FnBits.OverridePrevious; }
   void setOverridePreviousDefinition(bool b) { FnBits.OverridePrevious = b; }

   static BasicBlockList Function::*getSublistAccess(BasicBlock*)
   {
      return &Function::BasicBlocks;
   }

   void dump() const;
   void print(llvm::raw_ostream &OS) const;

protected:
   Module *parent;
   BasicBlockList BasicBlocks;
   KnownFunction knownFnKind = KnownFunction(0);
   uint16_t Priority = 1;

   serial::LazyILFunctionInfo *LazyFnInfo = nullptr;

   Function(const Function &other, Module &M);

   Function(TypeID id,
            FunctionType *Ty,
            llvm::StringRef name,
            Module *parent);

public:
   static inline bool classof(Value const* T) {
      switch(T->getTypeID()) {
         case FunctionID:
         case LambdaID:
         case MethodID:
         case InitializerID:
            return true;
         default:
            return false;
      }
   }
};

class Lambda: public Function {
public:
   Lambda(FunctionType *funcTy,
          Module *parent);

   llvm::ArrayRef<QualType> getCaptures() const
   {
      return captures;
   }

   void addCapture(QualType type)
   {
      captures.push_back(type);
   }

   static inline bool classof(Value const* T)
   {
      return T->getTypeID() == LambdaID;
   }

   friend class Function; // for copy init

private:
   Lambda(const Lambda &other, Module &M);

   std::vector<QualType> captures;
};

class Method: public Function {
public:
   friend class Function; // for copy init

   Method(llvm::StringRef name,
          FunctionType *FuncTy,
          bool isStatic,
          bool isVirtual,
          bool isDeinit,
          Module *parent);

   ast::RecordDecl *getRecordType() const;

   bool isStatic() const { return FnBits.Static; }
   bool isVirtual() const { return FnBits.Virtual; }
   bool isDeinit() const { return FnBits.Deinit; }

   il::Value *getSelf() const { return Self; }
   void setSelf(il::Value *S) { Self = S; }

   unsigned getVtableOffset() const { return vtableOffset; }
   void setVtableOffset(unsigned offset) { vtableOffset = offset; }

   unsigned getPtableOffset() const { return ptableOffset; }
   void setPtableOffset(unsigned offset) { ptableOffset = offset; }

protected:
   il::Value *Self;

   unsigned vtableOffset = -1;
   unsigned ptableOffset = -1;

   Method(const Method &other, Module &M);

public:
   static inline bool classof(Value const* T) {
      switch(T->getTypeID()) {
      case MethodID:
      case InitializerID:
         return true;
      default:
         return false;
      }
   }
};

class Initializer: public Method {
public:
   friend class Function; // for copy init

   Initializer(llvm::StringRef methodName,
               FunctionType *FuncTy,
               ConstructorKind Kind,
               Module *parent);

   ConstructorKind getCtorKind() const
   {
      return (ConstructorKind)FnBits.CtorKind;
   }

protected:
   Initializer(const Initializer &other, Module &M);

public:
   static inline bool classof(Value const* T) {
      switch(T->getTypeID()) {
      case InitializerID:
         return true;
      default:
         return false;
      }
   }
};

} // namespace il
} // namespace cdot


#endif //CDOT_ILFUNCTION_H
