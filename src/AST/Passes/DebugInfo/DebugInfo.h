//
// Created by Jonas Zell on 12.10.17.
//

#ifndef CDOT_DEBUGINFO_H
#define CDOT_DEBUGINFO_H

#include <stack>
#include <unordered_map>
#include <llvm/IR/IRBuilder.h>
#include "llvm/IR/DIBuilder.h"
#include "../../AstDeclarations.h"

class CodeGen;

using std::unordered_map;
using std::string;

namespace cdot {
   struct SourceLocation;
   class BuiltinType;
   struct Argument;
   struct CompilationUnit;
   class BinaryOperator;

namespace cl {
   class Record;
   struct Method;
}

using cl::Record;
using cl::Method;

namespace codegen {

   class DebugInfo {
   public:
      explicit DebugInfo(const CompilationUnit &CU, llvm::IRBuilder<> &Builder);

      llvm::DIScope *getCurrentScope()
      {
         return ScopeStack.top();
      }

      void setDebugLoc(const SourceLocation &loc);

      llvm::MDNode *emitModuleDI();

      llvm::DIFile *getFileDI(const SourceLocation &loc);

      llvm::DIType *getTypeDI(BuiltinType *ty);
      llvm::DIType *getRecordDI(BuiltinType *ty);

      llvm::dwarf::Tag getTagForRecord(Record *rec);

      llvm::Instruction *emitLocalVarDI(DeclStmt *node, llvm::Value *alloca, llvm::Instruction *inst);
      void emitAssignmentDI(BinaryOperator *node, llvm::Value *addr);
      llvm::MDNode *emitGlobalVarDI(DeclStmt *node, llvm::GlobalVariable *var);

      llvm::MDNode *emitMethodDI(Method *method, llvm::Function *func);
      llvm::MDNode *emitFunctionDI(FunctionDecl *node, llvm::Function *func);
      llvm::MDNode *emitLambdaDI(LambdaExpr *node, llvm::Function *func);

      void emitParameterDI(
         const std::vector<std::shared_ptr<FuncArgDecl>> &args,
         llvm::Function *func,
         unsigned int argBegin = 0,
         bool emitSelf = false
      );

      llvm::DIScope *getFunctionScope(llvm::StringRef funcName);

      void beginGeneratedFunctionScope(
         llvm::StringRef funcName,
         const std::vector<BuiltinType*> &args
      );

      void beginLexicalScope(const SourceLocation &loc);

      void beginScope(llvm::DIScope *scope);
      void endScope();

   protected:
      unordered_map<uintptr_t, llvm::DIType*> TypeNodes;
      unordered_map<string, llvm::DIType*> StructTypes;
      unordered_map<size_t, llvm::DIFile*> FileNodes;
      unordered_map<string, llvm::DISubprogram*> Subprograms;

      llvm::DIBuilder DI;
      llvm::IRBuilder<> &Builder;
      llvm::Module *Module;

      std::stack<llvm::DIScope*> ScopeStack;
      llvm::DIFile *File;
   };

}
}

#endif //CDOT_DEBUGINFO_H
