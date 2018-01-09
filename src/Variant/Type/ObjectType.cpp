//
// Created by Jonas Zell on 13.08.17.
//

#include "ObjectType.h"

#include <llvm/ADT/StringSwitch.h>

#include "../../AST/SymbolTable.h"

#include "../../AST/Passes/CodeGen/CGMemory.h"
#include "../../AST/Passes/SemanticAnalysis/Record/Class.h"
#include "../../AST/Passes/SemanticAnalysis/Record/Enum.h"

#include "Generic.h"
#include "IntegerType.h"
#include "FPType.h"
#include "GenericType.h"
#include "PointerType.h"
#include "../../AST/Passes/CodeGen/CodeGen.h"
#include "../../AST/Passes/Declaration/DeclPass.h"
#include "../../AST/Passes/SemanticAnalysis/TemplateInstantiator.h"

using namespace cdot::support;
using cl::Class;

namespace cdot {

namespace {

Type::BoxedPrimitive getBoxedPrimitiveType(llvm::StringRef name)
{
   using BP = Type::BoxedPrimitive ;
   return llvm::StringSwitch<Type::BoxedPrimitive>(name)
#     define CDOT_PRIMITIVE(Name, BW, Unsigned) \
      .Case(#Name, BP::Name)
#     include "Primitive.def"
      .Default(BP::BP_None);
}

} // anonymous namespace

ObjectType* ObjectType::get(cl::Record *record)
{
   auto key = "__obj." + record->getName().str();
   if (Instances.find(key) == Instances.end()) {
      Instances.try_emplace(key, new ObjectType(record,
                                     getBoxedPrimitiveType(record->getName())));
   }

   return cast<ObjectType>(Instances[key]);
}

ObjectType* ObjectType::get(llvm::StringRef className)
{
   return get(SymbolTable::getRecord(className));
}

ObjectType* ObjectType::get(BoxedPrimitive primitive)
{
   switch (primitive) {
#  define CDOT_PRIMITIVE(Name, BW, Unsigned) \
      case Name: return get(#Name);
#  include "Primitive.def"
      default:
         llvm_unreachable("not a primitive!");
   }
}

ObjectType::ObjectType(cl::Record *record, BoxedPrimitive primitive)
   : Rec(record), primitiveType(primitive)
{
   id = TypeID::ObjectTypeID;
}

ObjectType* ObjectType::getAnyTy()
{
   return get("Any");
}

const sema::TemplateArgList& ObjectType::getTemplateArgs() const
{
   return Rec->getTemplateArgs();
}

bool ObjectType::hasTemplateArgs() const
{
   return !getTemplateArgs().empty();
}

bool ObjectType::isRawEnum() const
{
   auto rec = getRecord();
   return rec && rec->isRawEnum();
}

Type* ObjectType::unbox() const
{
   assert(isBoxedPrimitive());
   switch (primitiveType) {
      case Float:
         return FPType::getFloatTy();
      case Double:
         return FPType::getDoubleTy();
      case Bool:
         return IntegerType::getBoolTy();
      case Char:
         return IntegerType::getCharTy();
      default: {
         unsigned bw = primitiveType;
         bool isUnsigned = false;
         if (bw > 100) {
            isUnsigned = true;
            bw -= 100;
         }

         return IntegerType::get(bw, isUnsigned);
      }
   }
}

bool ObjectType::isBoxedEquivOf(Type const* other) const
{
   if (is(BP_None))
      return false;

   switch (other->getTypeID()) {
      case TypeID::FPTypeID:
         return primitiveType == (other->isFloatTy() ? Float : Double);
      case TypeID::IntegerTypeID: {
         if (other->getBitwidth() == 8 && primitiveType == Char)
            return true;

         if (other->getBitwidth() == 1 && primitiveType == Bool)
            return true;

         unsigned bw = primitiveType;
         if (bw > 200)
            return false;

         bool isUnsigned = false;
         if (bw > 100) {
            isUnsigned = true;
            bw -= 100;
         }

         return other->isIntNTy(bw, isUnsigned);
      }
      default:
         return false;
   }
}

unsigned short ObjectType::getAlignment() const
{
   return Rec->getAlignment();
}

size_t ObjectType::getSize() const
{
   return Rec->getSize();
}

string ObjectType::toString() const
{
   return Rec->getName();
}

InconcreteObjectType*
InconcreteObjectType::get(llvm::StringRef className,
                          sema::TemplateArgList &&templateArgs) {
   return get(SymbolTable::getRecord(className), std::move(templateArgs));
}

InconcreteObjectType*
InconcreteObjectType::get(cl::Record *record,
                          sema::TemplateArgList &&templateArgs) {
   llvm::SmallString<128> fullName("__iobj");
   fullName += record->getName();
   fullName += templateArgs.toString();

   if (Instances.find(fullName.str()) == Instances.end()) {
      Instances.try_emplace(fullName.str(),
                            new InconcreteObjectType(record,
                                                     std::move(templateArgs)));
   }

   return cast<InconcreteObjectType>(Instances[fullName.str()]);
}

InconcreteObjectType::InconcreteObjectType(llvm::StringRef className,
                                           sema::TemplateArgList &&templateArgs)
   : InconcreteObjectType(SymbolTable::getRecord(className),
                          std::move(templateArgs))
{

}
InconcreteObjectType::InconcreteObjectType(cl::Record *record,
                                           sema::TemplateArgList &&templateArgs)
   : ObjectType(record), templateArgs(std::move(templateArgs))
{
   assert(!record->getSpecializedTemplate());
   id = TypeID::InconcreteObjectTypeID;
}

} // namespace cdot