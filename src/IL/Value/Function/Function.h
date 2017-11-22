//
// Created by Jonas Zell on 16.11.17.
//

#ifndef CDOT_ILFUNCTION_H
#define CDOT_ILFUNCTION_H

#include "../Constant/Constant.h"
#include "../../../Variant/Type/QualType.h"
#include "../MetaData/MetaData.h"

#include <llvm/ADT/SmallVector.h>
#include <llvm/ADT/ArrayRef.h>

namespace cdot {

struct Argument;
class Callable;

namespace cl {

struct Method;

} // namespace cl

namespace ast {

class Function;

} // namespace cl

namespace il {

class BasicBlock;
class Module;
class Argument;

class MDFunction: public MetaData {
public:
   explicit MDFunction(Callable *c);

   Callable *getCallable() const;
   cl::Method *getMethod() const;
   ast::Function *getFunction() const;

protected:
   union {
      Callable *callable;
      cl::Method *method;
      ast::Function *function;
   };

public:
   static bool classof(MetaData const* T)
   {
      return T->getKind() == MDFunctionID;
   }
};

class Function: public Constant {
public:
   typedef llvm::SmallVector<BasicBlock*, 8> BBList;
   typedef BBList::iterator                  iterator;
   typedef BBList::const_iterator            const_iterator;

   typedef llvm::SmallVector<Argument*, 4>   ArgList;

   Function(Callable *c, Module *parent);

   Function(const std::string &name,
            QualType returnType,
            llvm::ArrayRef<cdot::Argument> args,
            Module *parent,
            SourceLocation loc,
            bool mightThrow);

   Module *getParent() const;
   const BBList &getBasicBlocks() const;
   bool isDeclared() const;
   QualType getReturnType() const;
   const ArgList &getArgs() const;

   bool mightThrow() const;

   bool isExternC() const;
   void setIsExternC(bool ext);

   bool isLambda() const;

   void addArgument(const std::string &name, ILType ty);
   void addArgumentAtBegin(const std::string &name, ILType ty);

   void addArgument(Argument *arg);
   void addArgumentAtBegin(Argument *arg);

   Argument *getArgument(unsigned idx) const;

   bool hasCallable() const;
   const std::string &getMangledName() const;
   std::string getLinkageName() const;
   Callable *getCallable() const;

   void addDefinition();
   Function *getDeclarationIn(Module *M) const;

   BBList::reference getEntryBlock();

   iterator getIteratorForBB(BasicBlock *bb);
   const_iterator getIteratorForBB(const BasicBlock *bb) const;

   iterator removeBasicBlock(const BasicBlock *inst);

   iterator insertBasicBlockAfter(BasicBlock *bb, iterator after);
   iterator insertBasicBlockBefore(BasicBlock *bb, iterator before);
   iterator insertBasicBlockAtEnd(BasicBlock *bb);
   iterator insertBasicBlockAtBegin(BasicBlock *bb);

protected:
   Module *parent;
   ArgList args;
   QualType returnType;
   BBList BasicBlocks;

   enum Flag : unsigned short {
      Throws = 0x1,
      Declared = 0x2,
      ExternC = 0x4,
      Lambda = 0x8,
      Property = 0x10,
      Static = 0x20,
   };

   Function(const std::string &name,
            FunctionType *ty,
            QualType returnType,
            llvm::ArrayRef<Argument*> args,
            Module *parent,
            SourceLocation loc,
            unsigned SubClassData);

public:
   static bool classof(Function const* T) { return true; }
   static inline bool classof(Value const* T) {
      switch(T->getTypeID()) {
         case FunctionID:
         case MethodID:
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
