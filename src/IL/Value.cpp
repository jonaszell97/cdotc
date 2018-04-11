//
// Created by Jonas Zell on 16.11.17.
//

#include "Value.h"

#include "AST/ASTContext.h"
#include "Constants.h"
#include "Context.h"
#include "Instructions.h"
#include "MetaData.h"
#include "Module.h"
#include "Use.h"
#include "ValueSymbolTable.h"

using namespace cdot::support;

namespace cdot {
namespace il {

ValueType::ValueType(il::Context &Ctx, Type *ty)
   : Ty(ty ? ty->getCanonicalType() : nullptr),
     Ctx(&Ctx)
{

}

ValueType::ValueType(il::Context &Ctx, QualType ty)
   : Ty(ty ? ty->getCanonicalType() : nullptr),
     Ctx(&Ctx)
{

}

ValueType ValueType::getPointerTo() const
{
   return ValueType(*Ctx, Ctx->getASTCtx().getPointerType(Ty));
}

ValueType ValueType::getReferenceTo() const
{
   return ValueType(*Ctx, Ctx->getASTCtx().getReferenceType(Ty));
}

void ValueType::makeReference()
{
   Ty = Ctx->getASTCtx().getReferenceType(Ty);
}

Value::Value(TypeID id, ValueType ty)
   : id(id), SubclassData(0), type(ty), uses(0), metaData(nullptr)
{
}

Value::~Value()
{

}

void Value::deleteValue()
{
   switch (id) {
#define CDOT_ALL(name) \
      case name##ID:           \
         delete static_cast<name*>(this); break;
#include "Instructions.def"
      default:
         llvm_unreachable("unknown value kind");
   }
}

bool Value::isSelf() const
{
   if (auto Arg = dyn_cast<Argument>(this)) {
      return Arg->isSelf();
   }

   return false;
}

ast::ASTContext& Value::getASTCtx() const
{
   return getCtx().getASTCtx();
}

bool Value::isLvalue() const
{
   return type->isReferenceType();
}

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

void Value::detachAndErase()
{
   detachFromParent();
   assert(!getNumUses() && "can't erase value with multiple users");

   deleteValue();
}

void Value::addUse(Value *User)
{
   if (!uses) {
      uses = new Use(User); return;
   }

   uses->addUseAtEnd(new Use(User));
}

void Value::removeUser(Value *User)
{
   for (auto *use : *uses) {
      if (use->getUser() == User) {
         if (!use->hasNext() && !use->hasPrev()) {
            // this was the last use
            delete use;
            uses = nullptr;
         }
         else {
            use->remove();
         }

         checkIfStillInUse();

         return;
      }
   }

   llvm_unreachable("user not found");
}

void Value::checkIfStillInUse()
{
   bool useFound = false;
   if (uses) {
      for (auto *use : *uses) {
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
   if (!uses) return 0;
   return uses->count();
}

bool Value::isUnused() const
{
   return !getNumUses();
}

void Value::replaceAllUsesWith(Value *V)
{
   assert(type == V->getType() && "replacement value must be of same type");
   if (uses) {
      for (auto *use : *uses) {
         cast<Instruction>(use->getUser())->replaceOperand(this, V);
      }
   }

   V->uses = uses;
   uses = nullptr;

   if (auto Inst = dyn_cast<Instruction>(this)) {
      Inst->handleReplacement(V);
   }
   if (auto Const = dyn_cast<Constant>(this)) {
      Const->handleReplacement(V);
   }

   if (hasName() && !isa<Constant>(V)) {
      V->setName(name);
   }

   checkIfStillInUse();
}

llvm::StringRef Value::getName() const
{
   return name;
}

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

bool Value::hasName() const
{
   return !name.empty();
}

void Value::setLocation(SourceLocation location)
{
   loc = location;
}

SourceLocation Value::getSourceLoc() const
{
   return loc;
}

MDSet *Value::getMetaData() const
{
   return metaData;
}

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

void Value::addMetaData(MetaData *MD)
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

} // namespace il
} // namespace cdot
