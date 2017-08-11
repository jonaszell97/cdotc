//
// Created by Jonas Zell on 16.07.17.
//

#ifndef CDOT_CGCLASS_H
#define CDOT_CGCLASS_H


#include <string>
#include <llvm/IR/Type.h>
#include <llvm/IR/IRBuilder.h>

class CGClass {
public:
    static void CreateDefConstr(std::string, llvm::IRBuilder<>&, std::vector<llvm::Type*>);
};


#endif //CDOT_CGCLASS_H
