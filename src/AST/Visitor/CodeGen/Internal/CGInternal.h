//
// Created by Jonas Zell on 30.07.17.
//

#ifndef CDOT_CGINTERNAL_H
#define CDOT_CGINTERNAL_H


#include <string>
#include "../../../../Variant/Variant.h"
#include "../CodeGenVisitor.h"
#include <vector>
#include <unordered_map>
#include <map>

using std::string;
using std::unordered_map;
using std::map;

class InternalClass {
public:
    InternalClass(CodeGenVisitor&, string);
    InternalClass& DeclareProperty(string, llvm::Type*, bool);
    InternalClass& Get();

    llvm::Function* DeclareMethod(string, llvm::Type*, std::vector<llvm::Type*>, bool, bool = false);

    llvm::Value* CallMethod(string, std::vector<llvm::Value*>, llvm::Value*, bool);
    llvm::Value* AccessField(string, llvm::Value*, bool);

    friend class CGInternal;

protected:
    CodeGenVisitor& Visitor;
    bool finalized = false;
    llvm::StructType* type;
    string name;

    unordered_map<string, llvm::Function*> methods = {};
    map<string, llvm::Type*> fields = {};
};


class CGInternal {
public:
    static void CreateString(CodeGenVisitor&);
    static llvm::GlobalVariable* CreateInt(CodeGenVisitor&);
};

#endif //CDOT_CGINTERNAL_H
