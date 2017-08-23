//
// Created by Jonas Zell on 20.08.17.
//

#include "Builtin.h"
#include "../../../Variant/Type/Type.h"
#include "../../../Variant/Type/IntegerType.h"
#include "../../../Variant/Type/GenericType.h"
#include "../../SymbolTable.h"
#include "Class.h"

namespace cdot {

    std::vector<string> BuiltinNamespaces = {
        "Builtin",
        "Builtin.Primitive",
        "Builtin.RawPointer"
    };

    void ImportPrimitive() {
        SymbolTable::declareTypedef("Builtin.Primitive.int64", IntegerType::get(64));
        SymbolTable::declareTypedef("Builtin.Primitive.int32", IntegerType::get(32));
        SymbolTable::declareTypedef("Builtin.Primitive.int16", IntegerType::get(16));
        SymbolTable::declareTypedef("Builtin.Primitive.int8", IntegerType::get(8));
        SymbolTable::declareTypedef("Builtin.Primitive.int1", IntegerType::get(1));

        SymbolTable::declareTypedef("Builtin.Primitive.uint64", IntegerType::get(64, true));
        SymbolTable::declareTypedef("Builtin.Primitive.uint32", IntegerType::get(32, true));
        SymbolTable::declareTypedef("Builtin.Primitive.uint16", IntegerType::get(16, true));
        SymbolTable::declareTypedef("Builtin.Primitive.uint8", IntegerType::get(8, true));
        SymbolTable::declareTypedef("Builtin.Primitive.uint1", IntegerType::get(1, true));
    }

    void ImportRawPointer(llvm::IRBuilder<> Builder, llvm::LLVMContext& Context) {
        string className = "Builtin.RawPointer";
        auto RawPointer = SymbolTable::declareClass(className, ObjectType::get("Any"), {}, { new GenericType
            ("T") }, nullptr, false);

        ObjectType::declareStructureType(className, llvm::StructType::get(Context));

        RawPointer->finalize();
        RawPointer->generateMemoryLayout(Builder);
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