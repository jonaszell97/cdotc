#include "cdotc/IL/Function.h"

#include "cdotc/AST/Decl.h"
#include "cdotc/IL/Argument.h"
#include "cdotc/IL/BasicBlock.h"
#include "cdotc/IL/Module.h"
#include "cdotc/IL/Writer/ModuleWriter.h"

#include <llvm/Support/ErrorHandling.h>
#include <llvm/Support/raw_ostream.h>

using namespace cdot::support;

namespace cdot {
namespace il {

Function::Function(llvm::StringRef name, FunctionType* funcTy, Module* parent)
    : GlobalObject(FunctionID, funcTy->getCanonicalType(), parent, name),
      BasicBlocks(this)
{
   auto Canon = cast<FunctionType>(funcTy->getCanonicalType());

   FnBits.Declared = true;
   FnBits.Throws = Canon->throws();
   FnBits.Vararg = Canon->isCStyleVararg();
}

Function::Function(TypeID id, FunctionType* Ty, llvm::StringRef name,
                   Module* parent)
    : GlobalObject(id, Ty, parent, name), BasicBlocks(this)
{
   auto Canon = cast<FunctionType>(Ty->getCanonicalType());

   FnBits.Declared = true;
   FnBits.Throws = Canon->throws();
   FnBits.Vararg = Canon->isCStyleVararg();
}

Function::Function(const Function& other, Module& M)
    : GlobalObject(FunctionID, other.getType(), &M, other.name),
      BasicBlocks(this)
{
   metaData = other.metaData;
   knownFnKind = other.knownFnKind;
   FnBits = other.FnBits;
   FnBits.Declared = true;
   loc = other.loc;
   SourceLoc = other.SourceLoc;
   LazyFnInfo = other.LazyFnInfo;

   setLinkage(ExternalLinkage);

   new BasicBlock(*other.getEntryBlock(), *this);
}

Function::~Function() {}

llvm::StringRef Function::getUnmangledName() const { return getName(); }

bool Function::isLambda() const { return isa<Lambda>(this); }

BasicBlock const* Function::getEntryBlock() const
{
   return &BasicBlocks.front();
}

BasicBlock* Function::getEntryBlock() { return &BasicBlocks.front(); }

bool Function::isGlobalInitFn() const
{
   return this == parent->getGlobalInitFn();
}

Function* Function::getDeclarationIn(Module* Mod)
{
   if (parent == Mod)
      return this;

   if (auto Fn = Mod->getOwnFunction(getName()))
      return Fn;

   Function* f;
   if (auto Init = dyn_cast<Initializer>(this)) {
      f = new Initializer(*Init, *Mod);
   }
   else if (auto M = dyn_cast<Method>(this)) {
      f = new Method(*M, *Mod);
   }
   else if (auto L = dyn_cast<Lambda>(this)) {
      f = new Lambda(*L, *Mod);
   }
   else {
      f = new Function(*this, *Mod);
   }

   Mod->insertFunction(f);
   return f;
}

void Function::dump() const { print(llvm::outs()); }

void Function::print(llvm::raw_ostream& OS) const
{
   ModuleWriter Writer(this);
   Writer.WriteTo(OS);
}

Lambda::Lambda(FunctionType* funcTy, Module* parent)
    : Function(LambdaID, funcTy, "__anonymous_lambda", parent)
{
   setLinkage(InternalLinkage);
}

Lambda::Lambda(const il::Lambda& other, il::Module& M) : Function(other, M) {}

Method::Method(llvm::StringRef name, FunctionType* FuncTy, bool isStatic,
               bool isVirtual, bool isDeinit, Module* parent)
    : Function(MethodID, FuncTy, name, parent), Self(nullptr)
{
   FnBits.Static = isStatic;
   FnBits.Virtual = isVirtual;
   FnBits.Deinit = isDeinit;
}

Method::Method(const Method& other, Module& M)
    : Function(other, M), Self(nullptr)
{
   id = MethodID;
   vtableOffset = other.vtableOffset;
   ptableOffset = other.ptableOffset;
}

ast::RecordDecl* Method::getRecordType() const
{
   return BasicBlocks.front()
       .getArgs()
       .front()
       .getType()
       ->removeReference()
       ->getRecord();
}

Initializer::Initializer(llvm::StringRef methodName, FunctionType* FuncTy,
                         ConstructorKind Kind, Module* parent)
    : Method(methodName, FuncTy, false, false, false, parent)
{
   id = InitializerID;
   FnBits.CtorKind = (unsigned)Kind;
}

Initializer::Initializer(const Initializer& other, Module& M) : Method(other, M)
{
   id = InitializerID;
}

} // namespace il
} // namespace cdot