//
// Created by Jonas Zell on 12.07.17.
//

#ifndef CDOT_CGFUNCTIONTYPE_H
#define CDOT_CGFUNCTIONTYPE_H


#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/IRBuilder.h>
#include <unordered_map>
#include "../../../Variant/Variant.h"

class CodeGenVisitor;

class CGType {
public:
    static llvm::FunctionType* getFuncType(TypeSpecifier&, std::vector<llvm::Type*>);
    static llvm::Type* getType(TypeSpecifier&, bool = true);
    static llvm::PointerType* getPtrType(TypeSpecifier&);
    static unsigned short getAlignment(TypeSpecifier&);

    static llvm::Constant* getConstantVal(Variant&);
    static llvm::Constant* getConstantVal(TypeSpecifier &);
    static llvm::Value* getDefaultVal(TypeSpecifier &);

    static inline void declareStructureType(std::string name, llvm::StructType *type) {
        StructureTypes.emplace(name, type);
    }

    static inline llvm::StructType* getStructureType(std::string name) {
        return StructureTypes[name];
    }

    static inline void declareEnvType(unsigned int id, llvm::StructType *type) {
        EnvTypes.emplace(id, type);
    }

protected:
    static llvm::Type* _getType(TypeSpecifier&, bool);
    static std::unordered_map<std::string, llvm::StructType*> StructureTypes;
    static std::unordered_map<unsigned int, llvm::StructType*> EnvTypes;
};


#endif //CDOT_CGFUNCTIONTYPE_H
