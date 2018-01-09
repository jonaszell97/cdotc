//
// Created by Jonas Zell on 16.11.17.
//

#include "GlobalVariable.h"
#include "../Module/Module.h"

namespace cdot {
namespace il {

GlobalVariable::GlobalVariable(Type *ty,
                               bool isConstant,
                               llvm::StringRef name,
                               Module *module,
                               Constant *initializer)
   : GlobalObject(GlobalVariableID, ty, module, name),
     initializer(initializer)
{
   setIsLvalue(true);

   if (isConstant) {
      type.isConst(true);
      SubclassData |= Flags::Const;
   }

   if (module)
      module->insertGlobal(this);
}

GlobalVariable::GlobalVariable(const GlobalVariable &var)
   : GlobalObject(GlobalVariableID, *var.getType(), nullptr, var.name),
     initializer(nullptr)
{
   setIsLvalue(true);
   if (auto L = var.getLocation())
      addMetaData(L);

   SubclassData |= Flags::Declared;
}

void GlobalVariable::setInitializer(Constant *initializer)
{
   GlobalVariable::initializer = initializer;
}

GlobalVariable* GlobalVariable::getDeclarationIn(Module *M)
{
   auto global = new GlobalVariable(*this);

   global->parent = M;
   M->insertGlobal(global);

   return global;
}

} // namespace il
} // namespace cdot