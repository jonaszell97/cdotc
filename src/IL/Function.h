//
// Created by Jonas Zell on 16.11.17.
//

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

enum class KnownFunction: unsigned char;

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

   bool isGlobalCtor() const { return FnBits.GlobalCtor; }
   bool isGlobalDtor() const { return FnBits.GlobalDtor; }

   void setGlobalCtor(bool ctor) { FnBits.GlobalCtor = ctor; }
   void setGlobalDtor(bool dtor) { FnBits.GlobalDtor = dtor; }

   void addDefinition() { FnBits.Declared = false; }
   Function *getDeclarationIn(Module *M);

   ValueSymbolTable* getSymTab() const
   { return BasicBlocks.getSymTab().get(); }

   BasicBlock const* getEntryBlock() const;
   BasicBlock* getEntryBlock();

   iterator begin() { return BasicBlocks.begin(); }
   iterator end() { return BasicBlocks.end(); }
   const_iterator begin() const { return BasicBlocks.begin(); }
   const_iterator end() const { return BasicBlocks.end(); }

   bool isGlobalInitFn() const;

   KnownFunction getKnownFnKind() const { return knownFnKind; }
   void setKnownFnKind(KnownFunction kind) { knownFnKind = kind; }

   static BasicBlockList Function::*getSublistAccess(BasicBlock*)
   {
      return &Function::BasicBlocks;
   }

protected:
   Module *parent;
   BasicBlockList BasicBlocks;
   KnownFunction knownFnKind = KnownFunction(0);

   Function(const Function &other);

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

private:
   std::vector<QualType> captures;
};

class Method: public Function {
public:
   friend class Function; // for copy init

   Method(llvm::StringRef name,
          FunctionType *FuncTy,
          bool isStatic,
          bool isVirtual,
          Module *parent);

   ast::RecordDecl *getRecordType() const;

   bool isStatic() const { return FnBits.Static; }
   bool isVirtual() const { return FnBits.Virtual; }

   Argument *getSelf();

   unsigned getVtableOffset() const { return vtableOffset; }
   void setVtableOffset(unsigned offset) { vtableOffset = offset; }

   unsigned getPtableOffset() const { return ptableOffset; }
   void setPtableOffset(unsigned offset) { ptableOffset = offset; }

protected:
   Argument *Self;

   unsigned vtableOffset = 0;
   unsigned ptableOffset = 0;

   Method(const Method &other);

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
               Module *parent);

protected:
   Initializer(const Initializer &other);

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
