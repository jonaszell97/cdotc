#include "cdotc/IL/Value.h"

#include "cdotc/AST/ASTContext.h"
#include "cdotc/AST/Decl.h"
#include "cdotc/IL/Constants.h"
#include "cdotc/IL/Context.h"
#include "cdotc/IL/Instructions.h"
#include "cdotc/IL/MetaData.h"
#include "cdotc/IL/Module.h"
#include "cdotc/IL/Use.h"
#include "cdotc/IL/ValueSymbolTable.h"

#include <llvm/Support/raw_ostream.h>

using namespace cdot::support;

namespace cdot {
namespace il {

llvm::raw_ostream& operator<<(llvm::raw_ostream& OS, MemoryOrder MO)
{
   switch (MO) {
   case MemoryOrder::NotAtomic:
      return OS << "<not atomic>";
   case MemoryOrder::Relaxed:
      return OS << "relaxed";
   case MemoryOrder::Consume:
      return OS << "consume";
   case MemoryOrder::Acquire:
      return OS << "acquire";
   case MemoryOrder::Release:
      return OS << "release";
   case MemoryOrder::AcquireRelease:
      return OS << "acq_rel";
   case MemoryOrder::SequentiallyConsistent:
      return OS << "seq_cst";
   }
}

ValueType::ValueType(il::Context& Ctx, QualType ty)
    : Ty(ty ? ty.getCanonicalType()->getDesugaredType() : QualType()), Ctx(&Ctx)
{
}

ValueType& ValueType::operator=(QualType ty)
{
   Ty = ty ? ty.getCanonicalType()->getDesugaredType() : QualType();
   return *this;
}

ValueType ValueType::getPointerTo() const
{
   return ValueType(*Ctx, Ctx->getASTCtx().getPointerType(Ty));
}

ValueType ValueType::getReferenceTo() const
{
   return ValueType(*Ctx, Ctx->getASTCtx().getReferenceType(Ty));
}

void ValueType::makeReference() { Ty = Ctx->getASTCtx().getReferenceType(Ty); }

Value::Value(TypeID id, ValueType ty)
    : id(id), SubclassData(0), type(ty), uses(0), metaData(nullptr)
{
}

Value::~Value()
{
   if (uses) {
      uses->Delete();
   }
}

StringRef Value::getValueKindDescription(TypeID ID)
{
   switch (ID) {
#define CDOT_ALL(NAME)                                                         \
   case NAME##ID:                                                              \
      return #NAME;
#define CDOT_ABSTRACT(NAME)                                                    \
   case NAME:                                                                  \
      return #NAME;
#include "cdotc/IL/Instructions.def"
   }
}

void Value::deleteValue()
{
   switch (id) {
#define CDOT_ALL(name)                                                         \
   case name##ID:                                                              \
      delete static_cast<name*>(this);                                         \
      break;
#include "cdotc/IL/Instructions.def"
   default:
      llvm_unreachable("unknown value kind");
   }
}

bool Value::isSelf() const
{
   Function* Fn = nullptr;
   if (auto Arg = dyn_cast<Argument>(this)) {
      Fn = Arg->getParent()->getParent();
   }
   else if (auto Inst = dyn_cast<Instruction>(this)) {
      Fn = Inst->getParent()->getParent();
   }

   if (Fn) {
      auto M = dyn_cast<Method>(Fn);
      if (!M)
         return false;

      return this == M->getSelf();
   }

   return false;
}

bool Value::isIndexingInstruction() const
{
   switch (getTypeID()) {
   case GEPInstID:
   case FieldRefInstID:
   case TupleExtractInstID:
      return true;
   default:
      return false;
   }
}

ast::ASTContext& Value::getASTCtx() const { return getCtx().getASTCtx(); }

bool Value::isLvalue() const { return type->isReferenceType(); }

void Value::setIsLvalue(bool ref)
{
   if (isLvalue() == ref)
      return;

   if (ref) {
      type = getASTCtx().getReferenceType((QualType)type);
   }
   else {
      type = type->asReferenceType()->getReferencedType();
   }
}

void Value::detachFromParent()
{
   if (auto Inst = dyn_cast<Instruction>(this)) {
      auto func = Inst->getParent();
      if (!func)
         return;

      func->getInstructions().remove(Inst->getIterator());
   }
   else if (auto Arg = dyn_cast<Argument>(this)) {
      auto func = Arg->getParent();
      if (!func)
         return;

      func->getArgs().remove(Arg->getIterator());
   }
   else if (auto Func = dyn_cast<Function>(this)) {
      auto Module = Func->getParent();
      if (!Module)
         return;

      Module->getFuncList().remove(Func->getIterator());
   }
   else if (auto Global = dyn_cast<GlobalVariable>(this)) {
      auto Module = Global->getParent();
      if (!Module)
         return;

      Module->getGlobalList().remove(Global->getIterator());
   }
   else if (auto BB = dyn_cast<BasicBlock>(this)) {
      auto Fun = BB->getParent();
      if (!Fun)
         return;

      Fun->getBasicBlocks().remove(BB->getIterator());
   }
   else {
      llvm_unreachable("cannot remove value!");
   }
}

void Value::eraseValue()
{
   assert(!getNumUses() && "can't erase value with multiple users");

   if (auto I = dyn_cast<Instruction>(this)) {
      for (auto op : I->getOperands()) {
         op->removeUser(I);
      }
   }

   deleteValue();
}

void Value::detachAndErase()
{
   detachFromParent();
   eraseValue();
}

void Value::addUse(Value* User)
{
   if (!uses) {
      uses = new Use(User);
      return;
   }

#ifndef NDEBUG
   uses->verify();
#endif

   uses->addUseAtEnd(new Use(User));
}

void Value::removeUser(Value* User)
{
   for (auto* use : *uses) {
      if (use->getUser() == User) {
         if (!use->hasNext() && !use->hasPrev()) {
            // this was the last use
            delete use;
            uses = nullptr;
         }
         else {
            if (use == uses)
               uses = use->getNext();

            use->remove();

#ifndef NDEBUG
            uses->verify();
#endif
         }

         return;
      }
   }
}

void Value::replaceUser(cdot::il::Value* User, il::Value* ReplaceWith)
{
   for (auto* use : *uses) {
      if (use->getUser() == User) {
         use->setUser(ReplaceWith);
         return;
      }
   }
}

void Value::checkIfStillInUse()
{
   bool useFound = false;
   if (uses) {
      for (auto* use : *uses) {
         if (isa<BasicBlock>(use->getUser())) {
            continue;
         }

         useFound = true;
         break;
      }
   }

   if (!useFound) {
      deleteValue();
   }
}

size_t Value::getNumUses() const
{
   if (!uses)
      return 0;
   return uses->count();
}

bool Value::isUnused() const { return !getNumUses(); }

void Value::replaceAllUsesWith(Value* V)
{
   assert(type == V->getType() && "replacement value must be of same type");
   if (uses) {
      for (auto* use : *uses) {
         if (auto I = dyn_cast<Instruction>(use->getUser()))
            I->replaceOperand(this, V);
         else if (auto C = dyn_cast<Constant>(use->getUser()))
            C->replaceOperand(cast<Constant>(this), cast<Constant>(V));
         else
            llvm_unreachable("bad value kind");

         V->addUse(use->getUser());
      }

      uses->Delete();
      uses = nullptr;
   }

   if (auto Inst = dyn_cast<Instruction>(this)) {
      Inst->handleReplacement(V);
   }
   else if (auto Const = dyn_cast<Constant>(this)) {
      Const->handleReplacement(V);
   }

   if (hasName() && !isa<Constant>(V)) {
      V->setName(name);
   }
}

il::Value* Value::ignoreBitCast()
{
   if (auto BC = dyn_cast<BitCastInst>(this))
      return BC->getOperand(0);

   return this;
}

llvm::StringRef Value::getName() const { return name; }

void Value::setName(llvm::StringRef name)
{
   if (auto Inst = dyn_cast<Instruction>(this)) {
      auto BB = Inst->getParent();
      BB->getSymTab()->reinsertWithName(name, this);
   }
   else if (auto BB = dyn_cast<BasicBlock>(this)) {
      auto func = BB->getParent();
      func->getSymTab()->reinsertWithName(name, this);
   }
   else if (auto Func = dyn_cast<Function>(this)) {
      auto Module = Func->getParent();
      Module->getFunSymTab()->reinsertWithName(name, this);
   }
   else if (auto Global = dyn_cast<GlobalVariable>(this)) {
      auto Module = Global->getParent();
      Module->getGlobSymTab()->reinsertWithName(name, this);
   }
   else if (auto Arg = dyn_cast<Argument>(this)) {
      auto BB = Arg->getParent();
      BB->getArgSymTab()->reinsertWithName(name, this);
   }
   else {
      llvm_unreachable("cannot rename value!");
   }
}

bool Value::hasName() const { return !name.empty(); }

MDSet* Value::getMetaData() const { return metaData; }

bool Value::hasMetaData(MDKind kind) const
{
   if (!metaData)
      return false;

   return metaData->hasNode(kind);
}

MetaData* Value::getMetaData(MDKind kind) const
{
   if (!metaData)
      return nullptr;

   return metaData->getNode(kind);
}

void Value::addMetaData(MetaData* MD)
{
   if (!metaData)
      metaData = new MDSet;

   metaData->setNode(MD);
}

CallSite Value::getAsCallSite()
{
   if (auto C = dyn_cast<CallInst>(this))
      return CallSite(C);

   if (auto C = dyn_cast<InvokeInst>(this))
      return CallSite(C);

   return CallSite();
}

ImmutableCallSite Value::getAsImmutableCallSite() const
{
   if (auto C = dyn_cast<CallInst>(this))
      return ImmutableCallSite(C);

   if (auto C = dyn_cast<InvokeInst>(this))
      return ImmutableCallSite(C);

   return ImmutableCallSite();
}

bool Value::isAllZerosValue() const
{
   if (auto* C = dyn_cast<Constant>(this))
      return C->isAllZerosValue();

   return false;
}

const llvm::APSInt* Value::getConstantIntegerValue() const
{
   if (auto *CI = dyn_cast<ConstantInt>(this)) {
      return &CI->getValue();
   }

   if (auto *Ld = dyn_cast<LoadInst>(this)) {
      return Ld->getOperand(0)->getConstantIntegerValue();
   }

   if (auto *GEP = dyn_cast<GEPInst>(this)) {
      return GEP->getVal()->getConstantIntegerValue();
   }

   if (!isa<ConstantStruct>(this)) {
      return nullptr;
   }

   auto *RT = getType()->asRecordType();
   if (!RT || !RT->isStruct()) {
      return nullptr;
   }

   auto *S = cast<ast::StructDecl>(RT->getRecord());
   if (!S->hasAttribute<_BuiltinAttr>()
   || !S->getDeclName().getIdentifierInfo()->getIdentifier().contains("Int")
   || !S->getStoredFields().front()->getType()->isIntegerType()) {
      return nullptr;
   }

   return cast<ConstantStruct>(this)->getElements().front()->getConstantIntegerValue();
}

const llvm::APFloat* Value::getConstantFPValue() const
{
   if (auto *CI = dyn_cast<ConstantFloat>(this)) {
      return &CI->getValue();
   }

   if (auto *Ld = dyn_cast<LoadInst>(this)) {
      return Ld->getOperand(0)->getConstantFPValue();
   }

   if (auto *GEP = dyn_cast<GEPInst>(this)) {
      return GEP->getVal()->getConstantFPValue();
   }

   if (!isa<ConstantStruct>(this)) {
      return nullptr;
   }

   auto *RT = getType()->asRecordType();
   if (!RT || !RT->isStruct()) {
      return nullptr;
   }

   auto *S = cast<ast::StructDecl>(RT->getRecord());
   if (!S->hasAttribute<_BuiltinAttr>()
       || !S->getStoredFields().front()->getType()->isFPType()) {
      return nullptr;
   }

   return cast<ConstantStruct>(this)->getElements().front()->getConstantFPValue();
}

} // namespace il
} // namespace cdot
