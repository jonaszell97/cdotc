//
// Created by Jonas Zell on 16.11.17.
//

#ifndef CDOT_ILFUNCTION_H
#define CDOT_ILFUNCTION_H

#include "../MetaData/MetaData.h"

#include "../SymbolTableList.h"
#include "../GlobalVariable.h"
#include "BasicBlock.h"
#include "Argument.h"

#include <llvm/ADT/SmallVector.h>
#include <llvm/ADT/ArrayRef.h>

namespace cdot {
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

   Function(const std::string &name,
            FunctionType *funcTy,
            Module *parent,
            bool isExternC);

   Module *getParent() const { return parent; }
   void setParent(Module *p) { parent = p; }

   BasicBlockList const& getBasicBlocks() const { return BasicBlocks; };
   BasicBlockList& getBasicBlocks() { return BasicBlocks; };

   bool isDeclared() const;
   void setIsDeclared(bool decl)
   {
      if (decl)
         SubclassData |= Flag::Declared;
      else
         SubclassData &= ~Flag::Declared;
   }

   QualType getReturnType() const
   {
      return support::cast<FunctionType>(type)->getReturnType();
   }

   bool mightThrow() const;

   bool isExternC() const;
   void setIsExternC(bool ext);

   bool isCStyleVararg() const { return (SubclassData & Flag::Vararg) != 0; }

   bool isLambda() const;

   bool hasStructReturn() const;

   void addDefinition();
   Function *getDeclarationIn(Module *M);

   const llvm::StringRef &getUnmangledName() const
   {
      return unmangledName;
   }

   void setUnmangledName(const llvm::StringRef &unmangledName)
   {
      Function::unmangledName = unmangledName;
   }

   ValueSymbolTable* getSymTab() const
   { return BasicBlocks.getSymTab().get(); }

   BasicBlock const* getEntryBlock() const;
   BasicBlock* getEntryBlock();

   iterator begin() { return BasicBlocks.begin(); }
   iterator end() { return BasicBlocks.end(); }
   const_iterator begin() const { return BasicBlocks.begin(); }
   const_iterator end() const { return BasicBlocks.end(); }

   bool isGlobalInitFn() const;

   static BasicBlockList Function::*getSublistAccess(BasicBlock*)
   {
      return &Function::BasicBlocks;
   }

protected:
   Module *parent;
   BasicBlockList BasicBlocks;

   llvm::StringRef unmangledName;

   enum Flag : unsigned short {
      Throws = 0x1,
      Declared = 1 << 1,
      ExternC = 1 << 2,
      Property = 1 << 4,
      Static = 1 << 5,
      Operator = 1 << 6,
      BoxedOperator = 1 << 7,
      ConversionOp = 1 << 8,
      SRet = 1 << 9,
      Virtual = SRet << 1,
      Vararg = Virtual << 1,
   };

   Function(const Function &other);

   Function(TypeID id,
            FunctionType *Ty,
            const std::string &name,
            Module *parent,
            bool isExternC);

public:
   static bool classof(Function const* T) { return true; }
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

   struct Capture {
      Capture(uintptr_t id, QualType type)
         : id(id), type(type)
      { }

      uintptr_t id;
      QualType type;
   };

   llvm::ArrayRef<Capture> getCaptures() const
   {
      return captures;
   }

   void addCapture(uintptr_t id, QualType type)
   {
      captures.emplace_back(id, type);
   }

   static inline bool classof(Value const* T)
   {
      return T->getTypeID() == LambdaID;
   }

private:
   std::vector<Capture> captures;
};

} // namespace il
} // namespace cdot


#endif //CDOT_ILFUNCTION_H
