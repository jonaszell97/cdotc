//
// Created by Jonas Zell on 16.11.17.
//

#ifndef CDOT_GLOBALVARIABLE_H
#define CDOT_GLOBALVARIABLE_H

#include <string>
#include "Constant/Constant.h"

namespace cdot {
namespace il {

class Module;

class GlobalVariable: public Constant {
public:
   GlobalVariable(Type *ty, bool isConstant,
                  Module *module,
                  Constant *initializer = nullptr,
                  const std::string &name = "",
                  const SourceLocation &loc = {});

   Module *getModule() const;
   Constant *getInitializer() const;
   bool hasInitializer() const;

   GlobalVariable *getDeclarationIn(Module *M);

   void setInitializer(Constant *initializer);

protected:
   Module *module;
   Constant *initializer;
   bool isConstant;

public:
   static bool classof(GlobalVariable const* T) { return true; }
   static inline bool classof(Value const* T) {
     switch(T->getTypeID()) {
        case GlobalVariableID:
        case FunctionID:
        case MethodID:
           return true;
        default:
           return false;
     }
   }
};

} // namespace il
} // namespace cdot


#endif //CDOT_GLOBALVARIABLE_H
