//
// Created by Jonas Zell on 20.08.17.
//

#include "Builtin.h"
#include "../../../Variant/Type/Type.h"
#include "../../../Variant/Type/IntegerType.h"
#include "../../SymbolTable.h"
#include "Class.h"
#include "../../../Variant/Type/FPType.h"
#include "../../../Variant/Type/VoidType.h"

namespace cdot {

   std::vector<string> BuiltinNamespaces = {
      "Builtin",
      "Builtin.Primitive",
      "Builtin.RawPointer"
   };

   bool primitiveDeclared = false;

   void ImportPrimitive() {
      if (primitiveDeclared) {
         return;
      }

      SymbolTable::declareTypedef("Builtin.Primitive.int64", IntegerType::ConstInt64);
      SymbolTable::declareTypedef("Builtin.Primitive.int32", IntegerType::ConstInt32);
      SymbolTable::declareTypedef("Builtin.Primitive.int16", IntegerType::get(16));
      SymbolTable::declareTypedef("Builtin.Primitive.int8", IntegerType::ConstInt8);
      SymbolTable::declareTypedef("Builtin.Primitive.int1", IntegerType::ConstInt1);

      SymbolTable::declareTypedef("Builtin.Primitive.uint64", IntegerType::get(64, true));
      SymbolTable::declareTypedef("Builtin.Primitive.uint32", IntegerType::get(32, true));
      SymbolTable::declareTypedef("Builtin.Primitive.uint16", IntegerType::get(16, true));
      SymbolTable::declareTypedef("Builtin.Primitive.uint8", IntegerType::get(8, true));
      SymbolTable::declareTypedef("Builtin.Primitive.uint1", IntegerType::get(1, true));

      SymbolTable::declareTypedef("Builtin.Primitive.float", FPType::ConstFloatTy);
      SymbolTable::declareTypedef("Builtin.Primitive.double", FPType::ConstDoubleTy);

      SymbolTable::declareTypedef("Builtin.Primitive.void", new VoidType);

      primitiveDeclared = true;
   }

   void ImportRawPointer(llvm::IRBuilder<> Builder, llvm::LLVMContext& Context) {

   }

   void Builtin::ImportBuiltin(string &name) {
      bool fullImport = name == "Builtin";

      if (fullImport || name == "Builtin.Primitive") {
         ImportPrimitive();
      }
      if (fullImport || name == "Builtin.RawPointer") {
         ImportRawPointer(CodeGenVisitor::Builder, CodeGenVisitor::Context);
      }
   }

}