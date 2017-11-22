//
// Created by Jonas Zell on 16.11.17.
//

#ifndef CDOT_MODULE_H
#define CDOT_MODULE_H

#include <llvm/ADT/StringMap.h>
#include <llvm/ADT/DenseSet.h>

namespace llvm {
class raw_ostream;
}

namespace cdot {

struct CompilationUnit;

namespace il {

class Value;
class Constant;
class Function;
class GlobalVariable;
class ConstantString;
class AggregateType;
class Context;

class Module {
public:
   typedef llvm::StringMap<Function *> FuncMap;
   typedef llvm::StringMap<GlobalVariable *> GlobalVarMap;
   typedef llvm::DenseSet<AggregateType *> TypeSet;

   explicit Module(Context &Ctx, CompilationUnit &CU);

   const llvm::StringMap<Function *> &getFunctions() const;
   const llvm::StringMap<GlobalVariable *> &getGlobalVariables() const;
   CompilationUnit &getCU() const;

   const llvm::DenseSet<AggregateType *> &getReferencedTypes() const;

   Function *insertFunction(Function *func);
   Function *getFunction(llvm::StringRef name);
   Function *getOwnFunction(llvm::StringRef name);

   GlobalVariable *insertGlobal(GlobalVariable *global);
   GlobalVariable *getGlobal(llvm::StringRef name);
   GlobalVariable *getOwnGlobal(llvm::StringRef name);

   ConstantString *getString(const std::string &str);

   AggregateType *getType(llvm::StringRef name, bool recursive = true);
   void addTypeReference(AggregateType *ty, bool recursive = true);

   Context &getContext() const;
   void AssignNames();

   void dump();
   void writeTo(llvm::raw_ostream &out);

protected:
   TypeSet ReferencedTypes;
   llvm::StringMap<ConstantString*> Strings;
   FuncMap Functions;
   GlobalVarMap GlobalVariables;

   Context &Ctx;
   CompilationUnit &CU;
};

} // namespace il
} // namespace cdot


#endif //CDOT_MODULE_H
