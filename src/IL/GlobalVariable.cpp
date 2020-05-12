#include "cdotc/IL/GlobalVariable.h"

#include "cdotc/AST/ASTContext.h"
#include "cdotc/IL/Context.h"
#include "cdotc/IL/Module.h"
#include "cdotc/IL/Writer/ModuleWriter.h"

namespace cdot {
namespace il {

const char* LinkageTypeNames[] = {"external",  "available_externally",
                                  "linkonce",  "linkonce_odr",
                                  "weak",      "weak_odr",
                                  "appending", "internal",
                                  "private",   "extern_weak",
                                  "common"};

const char* VisibilityTypeNames[] = {"default", "hidden", "protected"};

const char* UnnamedAddrNames[] = {"", "local_unnamed_addr", "unnamed_addr"};

GlobalObject::GlobalObject(TypeID id, QualType ty, Module* module,
                           llvm::StringRef name)
    : Constant(id, ValueType(module->getContext(), ty)), parent(module)
{
   this->name = name;
   Linkage = 0;
   Visibility = 0;
   UnnamedAddrVal = 0;
}

bool GlobalObject::isExternallyVisible() const
{
   if (Visibility != DefaultVisibility)
      return false;

   switch (Linkage) {
   default:
      return true;
   case InternalLinkage:
   case PrivateLinkage:
      return false;
   }
}

GlobalVariable::GlobalVariable(QualType ty, bool isConstant,
                               llvm::StringRef name, Module* module,
                               Constant* initializer)
    : GlobalObject(GlobalVariableID, ty, module, name), initializer(initializer)
{
   if (initializer) {
      initializer->addUse(this);
   }
   else {
      GVBits.Declared = true;
   }

   GVBits.Const = isConstant;

   if (isConstant) {
      type = getASTCtx().getReferenceType(ty);
   }
   else {
      type = getASTCtx().getMutableReferenceType(ty);
   }
}

GlobalVariable::GlobalVariable(const GlobalVariable& var, Module& M)
    : GlobalObject(GlobalVariableID, var.getType(), &M, var.name),
      initializer(nullptr)
{
   loc = var.loc;
   SourceLoc = var.SourceLoc;
   GVBits = var.GVBits;
   LazyGlobalInfo = var.LazyGlobalInfo;

   if (var.InitializedFlag)
      InitializedFlag = var.InitializedFlag->getDeclarationIn(&M);

   if (var.InitFn)
      InitFn = var.InitFn->getDeclarationIn(&M);

   setDeclared(true);
   setLinkage(ExternalLinkage);
}

void GlobalVariable::setInitializer(Constant* initializer)
{
   GlobalVariable::initializer = initializer;
   if (initializer) {
      initializer->addUse(this);
      setDeclared(false);
   }
}

void GlobalVariable::makeMutable()
{
   GVBits.Const = false;
   type = getASTCtx().getMutableReferenceType(type->getReferencedType());
}

void GlobalVariable::dump() const { print(llvm::outs()); }

void GlobalVariable::print(llvm::raw_ostream& OS) const
{
   ModuleWriter Writer(this);
   Writer.WriteTo(OS);
}

GlobalVariable* GlobalVariable::getDeclarationIn(Module* M)
{
   if (M == parent)
      return this;

   if (auto G = M->getOwnGlobal(getName()))
      return G;

   auto global = new GlobalVariable(*this, *M);
   M->insertGlobal(global);

   return global;
}

} // namespace il
} // namespace cdot