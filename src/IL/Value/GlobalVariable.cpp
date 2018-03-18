//
// Created by Jonas Zell on 16.11.17.
//

#include "GlobalVariable.h"
#include "../Module/Module.h"

namespace cdot {
namespace il {

GlobalObject::GlobalObject(TypeID id,
                           QualType ty,
                           Module *module,
                           llvm::StringRef name)
   : Constant(id, ValueType(module->getContext(), ty)),
     parent(module)
{
   this->name = name;
}

GlobalVariable::GlobalVariable(QualType ty,
                               bool isConstant,
                               llvm::StringRef name,
                               Module *module,
                               Constant *initializer)
   : GlobalObject(GlobalVariableID, ty, module, name),
     initializer(initializer)
{
   setIsLvalue(true);

   if (isConstant) {
      SubclassData |= Flags::Const;
   }

   if (module)
      module->insertGlobal(this);
}

GlobalVariable::GlobalVariable(const GlobalVariable &var)
   : GlobalObject(GlobalVariableID, var.getType(), nullptr, var.name),
     initializer(nullptr)
{
   setIsLvalue(true);
   loc = var.loc;
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