//
// Created by Jonas Zell on 16.11.17.
//

#include "GlobalVariable.h"

#include "AST/ASTContext.h"
#include "Context.h"
#include "Module.h"

namespace cdot {
namespace il {

const char *LinkageTypeNames[] = {
   "external",
   "available_externally",
   "linkonce",
   "linkonce_odr",
   "weak",
   "weak_odr",
   "appending",
   "internal",
   "private",
   "extern_weak",
   "common"
};

const char *VisibilityTypeNames[] = {
   "default",
   "hidden",
   "protected"
};

const char *UnnamedAddrNames[] = {
   "",
   "local_unnamed_addr",
   "unnamed_addr"
};

GlobalObject::GlobalObject(TypeID id,
                           QualType ty,
                           Module *module,
                           llvm::StringRef name)
   : Constant(id, ValueType(module->getContext(), ty)),
     parent(module)
{
   this->name = name;
   Linkage = 0;
   Visibility = 0;
   UnnamedAddrVal = 0;
}

GlobalVariable::GlobalVariable(QualType ty,
                               bool isConstant,
                               llvm::StringRef name,
                               Module *module,
                               Constant *initializer)
   : GlobalObject(GlobalVariableID, ty, module, name),
     initializer(initializer)
{
   GVBits.Const = isConstant;

   if (isConstant) {
      type = getASTCtx().getReferenceType(ty);
   }
   else {
      type = getASTCtx().getMutableReferenceType(ty);
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
   GVBits = var.GVBits;
}

void GlobalVariable::setInitializer(Constant *initializer)
{
   GlobalVariable::initializer = initializer;
}

void GlobalVariable::makeMutable()
{
   GVBits.Const = false;
   type = getASTCtx().getMutableReferenceType(type->getReferencedType());
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