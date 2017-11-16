//
// Created by Jonas Zell on 13.08.17.
//

#include "ObjectType.h"

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

namespace cdot {

using cl::Class;

unordered_map<string, ObjectType*> ObjectType::Instances;
unordered_map<string, DummyObjectType*> DummyObjectType::Instances;

ObjectType* ObjectType::get(const string &className)
{
   if (Instances.find(className) == Instances.end()) {
      Instances.emplace(className, new ObjectType(className));
   }

   return Instances[className];
}

ObjectType::ObjectType(const string &className)
{
   this->className = className;
   id = TypeID::ObjectTypeID;
   if (SymbolTable::hasRecord(this->className)) {
      auto cl = SymbolTable::getRecord(className);
      is_struct = cl->isStruct();
      is_protocol = cl->isProtocol();
      is_enum = cl->isEnum();
      is_union = cl->isUnion();

      if (is_enum) {
         auto en = cl->getAs<Enum>();
         is_raw_enum = !en->hasAssociatedValues();
      }
   }
}

ObjectType* ObjectType::getOptionOf(BuiltinType *T)
{
   std::vector<TemplateArg> templateArgs{TemplateArg(GenericType::get("T", T))};

   // instantiate option type
   auto rec = SymbolTable::getRecord("Option",
      new ResolvedTemplateArgList(move(templateArgs)));

   return get(rec->getName());
}

ObjectType* ObjectType::getAnyTy()
{
   return get("Any");
}

Record* ObjectType::getRecord() const
{
   return SymbolTable::getRecord(className);
}

const std::vector<TemplateArg>& ObjectType::getTemplateArgs() const
{
   auto rec = getRecord();
   assert(rec->isTemplated());

   return rec->getTemplateArgs();
}

bool ObjectType::hasTemplateArgs() const
{
   return getRecord()->isTemplated();
}

GenericType *const
ObjectType::getNamedTemplateArg(const string &genericName) const
{
   auto &templateArgs = getTemplateArgs();
   for (const auto& gen : templateArgs) {
      if (!gen.isTypeName()) {
         continue;
      }
      if (gen.getGenericTy()->getClassName() == genericName) {
         return gen.getGenericTy();
      }
   }

   return nullptr;
}

bool ObjectType::isOptionTy() const
{
   auto rec = getRecord();
   if (!rec->isTemplated()) {
      return false;
   }

   return rec->getTemplate()->recordName == "Option";
}

bool ObjectType::isOptionOf(const string &className) const
{
   if (!isOptionTy()) {
      return false;
   }

   auto TemplateArg = getRecord()->getTemplateArg("T");
   return TemplateArg.isTypeName() && TemplateArg.getGenericTy()
                                                 ->getActualType()
                                                 ->getClassName() == className;
}

bool ObjectType::needsCleanup() const
{
   return isRefcounted() || (is_struct && getRecord()
      ->getAs<Class>()->hasNonEmptyDeinitializer());
}

BuiltinType* ObjectType::unbox() const
{
   assert(util::matches(
      "(Float|Double|U?Int(1|8|16|32|64)?)",
      className
   ) && "Not a primitive!");

   if (className == "Float") {
      return FPType::getFloatTy();
   }
   if (className == "Double") {
      return FPType::getDoubleTy();
   }

   bool isUnsigned = false;
   string className = this->className;

   if (className.front() == 'U') {
      isUnsigned = true;
      className = className.substr(1);
   }

   if (className.length() <= 3) {
      return IntegerType::get();
   }

   return IntegerType::get(std::stoi(className.substr(3)), isUnsigned);
}

bool ObjectType::implicitlyCastableTo(BuiltinType *other) const
{
   switch (other->getTypeID()) {
      case TypeID::GenericTypeID:
         return implicitlyCastableTo(other->asGenericTy()->getActualType());
      case TypeID::AutoTypeID:
         return true;
      case TypeID::FunctionTypeID:
         return false;
      case TypeID::PointerTypeID: {
         auto asPtr = other->asPointerTy();
         auto pointee = asPtr->getPointeeType();

         // string to char array
         if (pointee->isIntegerTy()) {
            return pointee->getBitwidth() == 8 && !pointee->isUnsigned();
         }

         // special handling for structs
         if (is_struct && implicitlyCastableTo(*pointee)) {
            return true;
         }

         return false;
      }
      case TypeID::ObjectTypeID: {
         auto asObj = other->asObjTy();
         auto& otherClassName = other->getClassName();

         if (!SymbolTable::hasClass(otherClassName)) {
            return false;
         }

         if (SymbolTable::getClass(otherClassName)->isProtocol()) {
            if (className == otherClassName) {
               return true;
            }

            return SymbolTable::getClass(className)
               ->conformsTo(otherClassName);
         }

         if (otherClassName == "String") {
            return SymbolTable::getClass(className)
               ->conformsTo("StringRepresentable");
         }

         if (className != asObj->className) {
            cdot::cl::Class* cl = SymbolTable::getClass(asObj->className);

            return cl->isBaseClassOf(className) ||
               SymbolTable::getClass(className)->conformsTo(asObj->className);
         }

         return true;
      }
      case TypeID::IntegerTypeID: {
         if (is_enum) {
            auto en = SymbolTable::getClass(className);
            return !en->getAs<Enum>()->hasAssociatedValues();
         }

         if (other->isIntNTy(8) && className == "Char") {
            return true;
         }

         if (other->isIntNTy(1) && className == "Bool") {
            return true;
         }

         if (isBoxedEquivOf(other)) {
            return true;
         }

         if (isBoxedPrimitive()) {
            auto unboxed = unbox();
            if (unboxed->implicitlyCastableTo(other)) {
               return true;
            }
         }

         auto cl = SymbolTable::getClass(className);
         auto op = "infix as " + other->toString();

         return cl->hasMethodWithName(op);
      }
      case TypeID::FPTypeID: {
         if (isBoxedEquivOf(other)) {
            return true;
         }

         auto asFloat = cast<FPType>(other);
         string boxedCl = asFloat->getPrecision() == 64 ? "Double" : "Float";

         return className == boxedCl;
      }
      default:
         return false;
   }
}

bool ObjectType::explicitlyCastableTo(BuiltinType *other) const
{
   if (implicitlyCastableTo(other)) {
      return true;
   }

   switch (other->getTypeID()) {
      case TypeID::IntegerTypeID:
         return true;
      default:
         return false;
   }
}

bool ObjectType::isBoxedEquivOf(BuiltinType *other) const
{
   switch (other->getTypeID()) {
      case TypeID::IntegerTypeID: {
         auto asInt = cast<IntegerType>(other);
         if (className == "Bool") {
            return asInt->getBitwidth() == 1;
         }
         if (className == "Char") {
            return asInt->getBitwidth() == 8;
         }

         string boxedCl = string(asInt->isUnsigned() ? "U" : "")
                          + "Int" + std::to_string(asInt->getBitwidth());
         return className == boxedCl
                || (asInt->getBitwidth() == sizeof(int*) * 8
                    && className == "Int");
      }
      case TypeID::FPTypeID: {
         auto asFloat = cast<FPType>(other);
         return (className == "Double" && asFloat->getPrecision() == 64) ||
            (className == "Float" && asFloat->getPrecision() == 32);
      }
      default:
         return false;
   }
}

bool ObjectType::isRefcounted() const
{
   return !is_protocol && !is_struct && !is_enum;
}

bool ObjectType::isValueType() const
{
   return is_protocol || is_struct || is_enum;
}

bool ObjectType::hasDefaultValue() const
{
   return false;
}

llvm::Value* ObjectType::getDefaultVal(CodeGen &CGM) const
{
   if (is_struct) {
      auto& fields = SymbolTable::getClass(className)->getFields();
      std::vector<llvm::Constant*> vals;
      for (const auto& field : fields) {
         if (field.second->isStatic) {
            continue;
         }

         vals.push_back(llvm::cast<llvm::Constant>(
            field.second->fieldType->getDefaultVal(CGM)));
      }

      return llvm::ConstantStruct::get(CodeGen::getStructTy(className),
                                       vals);
   }

   return llvm::ConstantPointerNull::get(CodeGen::getStructTy(className)
                                            ->getPointerTo());
}

llvm::Constant* ObjectType::getConstantVal(Variant &val) const
{
   assert(isBoxedPrimitive() && "Can't emit constant val otherwise");

   auto unboxed = unbox();
   auto intVal = unboxed->getConstantVal(val);

   return llvm::ConstantStruct::get(
      CodeGen::getStructTy(className),
      { intVal }
   );
}

short ObjectType::getAlignment() const
{
   if (is_raw_enum) {
      return static_cast<cl::Enum*>(getRecord())->getRawType()->getAlignment();
   }

   return SymbolTable::getClass(className)->getAlignment();
}

size_t ObjectType::getSize() const
{
   return SymbolTable::getRecord(className)->getSize();
}

llvm::Type* ObjectType::getLlvmType() const
{
   if (is_enum) {
      auto en = static_cast<cl::Enum*>(getRecord());
      if (!is_raw_enum) {
         return CodeGen::getStructTy(className);
      }

      return en->getRawType()->getLlvmType();
   }
   else if (is_struct || is_protocol) {
      return CodeGen::getStructTy(className);
   }
   else if (is_union) {
      return llvm::IntegerType::get(CodeGen::Context, 8)->getPointerTo();
   }
   else {
      return CodeGen::getStructTy(className)->getPointerTo();
   }
}

string ObjectType::toString() const
{
   return className;
}

DummyObjectType::DummyObjectType(
   const string &className,
   std::vector<TemplateArg> &templateArgs)
   : ObjectType(className), templateArgs(templateArgs)
{

}

DummyObjectType* DummyObjectType::get(const string &className,
                                      std::vector<TemplateArg> &templateArgs) {
   auto name = className + util::TemplateArgsToString(templateArgs);
   if (Instances.find(name) == Instances.end()) {
      Instances.emplace(name, new DummyObjectType(className,
                                                  templateArgs));
   }

   return Instances[name];
}

} // namespace cdot