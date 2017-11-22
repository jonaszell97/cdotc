//
// Created by Jonas Zell on 16.11.17.
//

#include "GlobalVariable.h"
#include "../Module/Module.h"

namespace cdot {
namespace il {

GlobalVariable::GlobalVariable(Type *ty, bool isConstant,
                               Module *module,
                               Constant *initializer,
                               const std::string &name,
                               const SourceLocation &loc)
   : Constant(GlobalVariableID, ty, name, loc),
     initializer(initializer), module(module)
{
   module->insertGlobal(this);
}

Module *GlobalVariable::getModule() const
{
   return module;
}

Constant *GlobalVariable::getInitializer() const
{
   return initializer;
}

bool GlobalVariable::hasInitializer() const
{
   return initializer != nullptr;
}

void GlobalVariable::setInitializer(Constant *initializer)
{
   GlobalVariable::initializer = initializer;
}

GlobalVariable* GlobalVariable::getDeclarationIn(Module *M)
{
   return new GlobalVariable(*type, isConstant, M, initializer,
                             name, getLocation());
}

} // namespace il
} // namespace cdot