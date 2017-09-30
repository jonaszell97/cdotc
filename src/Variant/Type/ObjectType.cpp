//
// Created by Jonas Zell on 13.08.17.
//

#include "ObjectType.h"
#include "../../AST/SymbolTable.h"
#include "IntegerType.h"
#include "FPType.h"
#include "../../AST/Passes/CodeGen/CGMemory.h"
#include "../../AST/Passes/StaticAnalysis/Class.h"
#include "../../AST/Passes/StaticAnalysis/Enum.h"
#include "PointerType.h"

namespace cdot {

   using cl::Class;

   std::unordered_map<std::string, llvm::StructType*> ObjectType::StructureTypes = {};

   ObjectType* AnyTy = nullptr;

   ObjectType::ObjectType(string &&className, unordered_map<string, Type*>& concreteGenerics) :
      concreteGenericTypes(concreteGenerics)
   {
      id = TypeID::ObjectTypeID;
      this->className = className;
      for (const auto& gen : concreteGenerics) {
         gen.second->isGeneric(true);
      }
   }

   ObjectType::ObjectType(string &&className) {
      id = TypeID::ObjectTypeID;
      this->className = className;
   }

   ObjectType::ObjectType(string &className) {
      id = TypeID::ObjectTypeID;
      this->className = className;
   }

   ObjectType::~ObjectType() {
      for (const auto& gen : concreteGenericTypes) {
         delete gen.second;
      }

      for (const auto& gen : unqualifiedGenerics) {
         delete gen;
      }
   }

   ObjectType* ObjectType::get(string className) {
      return new ObjectType(className);
   }

   ObjectType* ObjectType::getOptionOf(Type *T) {
      auto opt = new ObjectType("Option");
      opt->isEnum(true);
      opt->getConcreteGenericTypes().emplace("T", T);

      return opt;
   }

   ObjectType* ObjectType::getAnyTy() {
      if (AnyTy == nullptr) {
         AnyTy = get("Any");
         AnyTy->isProtocol(true);
      }

      return AnyTy;
   }

   cl::Class* ObjectType::getClass()
   {
      return SymbolTable::getClass(className);
   }

   bool ObjectType::operator==(Type *&other) {
      switch (other->getTypeID()) {
         case TypeID::ObjectTypeID:
         case TypeID::CollectionTypeID: {
            auto asObj = cast<ObjectType>(other);
            if (className != asObj->className || !Type::operator==(other)) {
               return false;
            }

            if (concreteGenericTypes.size() != asObj->concreteGenericTypes.size()) {
               return false;
            }

//            for (const auto &gen : concreteGenericTypes) {
//               if (*gen.second != asObj->concreteGenericTypes[gen.first]) {
//                  return false;
//               }
//            }

            return true;
         }
         default:
            return false;
      }
   }

   std::vector<Type*> ObjectType::getContainedTypes(bool includeSelf) {
      std::vector<Type*> cont;

      if (includeSelf) {
         cont.push_back(this);
      }

      for (const auto& gen : concreteGenericTypes) {
         cont.push_back(gen.second);
      }

      return cont;
   }

   std::vector<Type**> ObjectType::getTypeReferences() {
      std::vector<Type**> cont;

      for (auto& gen : concreteGenericTypes) {
         cont.push_back(&gen.second);
      }

      return cont;
   }

   Type* ObjectType::deepCopy() {
      auto newTy = new ObjectType(*this);
      for (const auto& cont : newTy->concreteGenericTypes) {
         newTy->concreteGenericTypes[cont.first] = cont.second->deepCopy();
      }
      if (contravariance != nullptr) {
         newTy->contravariance = contravariance->deepCopy();
      }

      return newTy;
   }

   Type* ObjectType::unbox() {
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

      auto bitwidth = std::stoi(className.substr(3));
      return IntegerType::get(bitwidth, isUnsigned);
   }

   bool ObjectType::implicitlyCastableTo(Type *other) {
      switch (other->getTypeID()) {
         case TypeID::AutoTypeID:
            return true;
         case TypeID::FunctionTypeID:
            return false;
         case TypeID::PointerTypeID: {
            auto asPtr = cast<PointerType>(other);

            // string to char array
            if (isa<IntegerType>(asPtr->getPointeeType())) {
               auto asInt = cast<IntegerType>(asPtr->getPointeeType());
               return asInt->getBitwidth() == 8 && !asInt->isUnsigned();
            }

            // special handling for structs
            if (is_struct && lvalue && implicitlyCastableTo(asPtr->getPointeeType())) {
               return true;
            }

            return false;
         }
         case TypeID::ObjectTypeID:
         case TypeID::CollectionTypeID: {
            auto asObj = cast<ObjectType>(other);
            auto& otherClassName = asObj->getClassName();

            if (!SymbolTable::hasClass(otherClassName)) {
               return false;
            }

            if (SymbolTable::getClass(otherClassName)->isProtocol()) {
               if (className == otherClassName) {
                  return true;
               }

               return SymbolTable::getClass(className)->conformsTo(otherClassName);
            }

            if (SymbolTable::getClass(className)->isProtocol()) {
               if (className == otherClassName) {
                  return true;
               }

               return SymbolTable::getClass(otherClassName)->conformsTo(className);
            }

            if (!Type::operator==(other)) {
               return false;
            }

            if (className != asObj->className) {
               cdot::cl::Class* cl = SymbolTable::getClass(asObj->className);

               if (SymbolTable::getClass(className)->isProtocol()) {
                  return cl->conformsTo(className);
               }
               else {
                  return cl->isBaseClassOf(className) ||
                     SymbolTable::getClass(className)->conformsTo(asObj->className);
               }
            }

            if (concreteGenericTypes.size() != asObj->getConcreteGenericTypes().size()) {
               return false;
            }

            for (const auto& gen : concreteGenericTypes) {
               if (!asObj->concreteGenericTypes[gen.first] ||
                  !gen.second->implicitlyCastableTo(asObj->concreteGenericTypes[gen.first]))
               {
                  return false;
               }
            }

            return true;
         }
         case TypeID::IntegerTypeID: {
            if (is_enum) {
               auto en = SymbolTable::getClass(className);
               return !static_cast<cdot::cl::Enum*>(en)->hasAssociatedValues();
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
                  delete unboxed;
                  return true;
               }

               delete unboxed;
            }

            auto cl = SymbolTable::getClass(className);
            auto op = "infix as " + other->toString();

            auto castOp = cl->hasMethod(op);
            return castOp.compatibility == CompatibilityType::COMPATIBLE;
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

   bool ObjectType::explicitlyCastableTo(Type *other) {
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

   bool ObjectType::isBoxedEquivOf(Type *&other) {
      switch (other->getTypeID()) {
         case TypeID::IntegerTypeID: {
            auto asInt = cast<IntegerType>(other);
            if (className == "Bool") {
               return asInt->getBitwidth() == 1;
            }
            if (className == "Char") {
               return asInt->getBitwidth() == 8;
            }

            string boxedCl = string(asInt->isUnsigned() ? "U" : "") + "Int" + std::to_string(asInt->getBitwidth());
            return className == boxedCl || (asInt->getBitwidth() == sizeof(int*) * 8 && className == "Int");
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

   bool ObjectType::isRefcounted() {
      return !is_protocol && !is_struct && !is_enum;
   }

   bool ObjectType::isValueType() {
      return is_protocol || is_struct || is_enum;
   }

   bool ObjectType::hasDefaultValue() {
      auto cl = SymbolTable::getClass(className);

      auto constr = cl->hasMethod("init", {}, {}, concreteGenericTypes);

      return constr.compatibility == CompatibilityType::COMPATIBLE;
   }

   llvm::Value* ObjectType::getDefaultVal() {
      if (is_struct) {
         auto& fields = SymbolTable::getClass(className)->getFields();
         std::vector<llvm::Constant*> vals;
         for (const auto& field : fields) {
            vals.push_back(llvm::cast<llvm::Constant>(field.second->fieldType->getDefaultVal()));
         }

         return llvm::ConstantStruct::get(ObjectType::getStructureType(className), vals);
      }

      return llvm::ConstantPointerNull::get(ObjectType::getStructureType(className)->getPointerTo());
   }

   llvm::Constant* ObjectType::getConstantVal(Variant &val)
   {
      assert(isBoxedPrimitive() && "Can't emit constant val otherwise");

      auto unboxed = unbox();
      auto intVal = unboxed->getConstantVal(val);

      return llvm::ConstantStruct::get(
         ObjectType::getStructureType(className),
         { intVal }
      );
   }

   short ObjectType::getAlignment() {
      return SymbolTable::getClass(className)->getAlignment();
   }

   size_t ObjectType::getSize() {
      if (is_struct) {
         return SymbolTable::getClass(className)->getOccupiedBytes();
      }

      return sizeof(int*);
   }

   llvm::Type* ObjectType::_getLlvmType() {
      if (is_struct || is_protocol) {
         return StructureTypes[className];
      }
      else {
         return StructureTypes[className]->getPointerTo();
      }
   }

   string ObjectType::_toString() {
      string str;
      if (isGeneric()) {
         str = genericClassName;
      }
      else {
         str = className;
      }

      if (!concreteGenericTypes.empty()) {
         str += "<";
         size_t size = concreteGenericTypes.size();
         size_t i = 0;
         for (const auto& gen : concreteGenericTypes) {
            str += gen.second->toString();
            if (i < size - 1) {
               str += ", ";
            }

            ++i;
         }
         str += ">";
      }

      return str;
   }

} // namespace cdot