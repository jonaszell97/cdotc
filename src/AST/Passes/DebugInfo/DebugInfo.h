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

using std::unordered_map;
using std::string;

namespace cdot {

struct SourceLocation;
class Type;
struct Argument;
struct CompilationUnit;

namespace ast {

class CodeGen;
class BinaryOperator;

} // namespace ast

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

      llvm::DIType *getTypeDI(Type *ty);
      llvm::DIType *getRecordDI(Type *ty);

      llvm::dwarf::Tag getTagForRecord(Record *rec);

      llvm::Instruction *emitLocalVarDI(ast::LocalVarDecl *node,
                                        llvm::Value *alloca,
                                        llvm::Instruction *inst);
      void emitAssignmentDI(ast::BinaryOperator *node, llvm::Value *addr);
      llvm::MDNode *emitGlobalVarDI(ast::LocalVarDecl *node,
                                    llvm::GlobalVariable *var);

      llvm::MDNode *emitMethodDI(Method *method, llvm::Function *func);
      llvm::MDNode *emitFunctionDI(ast::FunctionDecl *node,
                                   llvm::Function *func);
      llvm::MDNode *emitLambdaDI(ast::LambdaExpr *node, llvm::Function *func);

      void emitParameterDI(
         const std::vector<ast::FuncArgDecl* > &args,
         llvm::Function *func,
         unsigned int argBegin = 0,
         bool emitSelf = false
      );

      llvm::DIScope *getFunctionScope(llvm::StringRef funcName);

      void beginGeneratedFunctionScope(
         llvm::StringRef funcName,
         const std::vector<Type*> &args
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
