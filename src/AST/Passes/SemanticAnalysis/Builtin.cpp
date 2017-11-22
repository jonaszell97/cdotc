//
// Created by Jonas Zell on 20.08.17.
//

#include "Builtin.h"
#include "../../../Variant/Type/Type.h"
#include "../../../Variant/Type/IntegerType.h"
#include "../../SymbolTable.h"
#include "Record/Class.h"
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

      SymbolTable::declareTypedef("Builtin.Primitive.int64", IntegerType::get(64));
      SymbolTable::declareTypedef("Builtin.Primitive.int32", IntegerType::get(32));
      SymbolTable::declareTypedef("Builtin.Primitive.int16", IntegerType::get(16));
      SymbolTable::declareTypedef("Builtin.Primitive.int8", IntegerType::get(8));
      SymbolTable::declareTypedef("Builtin.Primitive.int1", IntegerType::get(1));

      SymbolTable::declareTypedef("Builtin.Primitive.word",
         IntegerType::get(sizeof(int*) * 8));
      SymbolTable::declareTypedef("Builtin.Primitive.uword",
         IntegerType::get(sizeof(int*) * 8, true));

      SymbolTable::declareTypedef("Builtin.Primitive.signed",
         IntegerType::get(sizeof(signed) * 8));

      SymbolTable::declareTypedef("Builtin.Primitive.unsigned",
         IntegerType::get(sizeof(unsigned) * 8, true));

      SymbolTable::declareTypedef("Builtin.Primitive.intptr",
         IntegerType::get(sizeof(intptr_t) * 8));
      SymbolTable::declareTypedef("Builtin.Primitive.uintptr",
         IntegerType::get(sizeof(uintptr_t) * 8, true));

      SymbolTable::declareTypedef("Builtin.Primitive.uint64", IntegerType::get(64, true));
      SymbolTable::declareTypedef("Builtin.Primitive.uint32", IntegerType::get(32, true));
      SymbolTable::declareTypedef("Builtin.Primitive.uint16", IntegerType::get(16, true));
      SymbolTable::declareTypedef("Builtin.Primitive.uint8", IntegerType::get(8, true));
      SymbolTable::declareTypedef("Builtin.Primitive.uint1", IntegerType::get(1, true));

      SymbolTable::declareTypedef("Builtin.Primitive.float", FPType::getFloatTy());
      SymbolTable::declareTypedef("Builtin.Primitive.double", FPType::getDoubleTy());

      SymbolTable::declareTypedef("Builtin.Primitive.void", VoidType::get());

      primitiveDeclared = true;
   }

   void Builtin::ImportBuiltin(const string &name) {
      bool fullImport = name == "Builtin";

      if (fullImport || name == "Builtin.Primitive") {
         ImportPrimitive();
      }
   }

}