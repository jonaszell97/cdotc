//
// Created by Jonas Zell on 13.08.17.
//

#include "ObjectType.h"
#include "../../AST/SymbolTable.h"
#include "IntegerType.h"
#include "FPType.h"
#include "../../AST/Visitor/CodeGen/CGMemory.h"
#include "../../AST/Visitor/StaticAnalysis/Class.h"
#include "PointerType.h"

namespace cdot {

   using cl::Class;

   std::unordered_map<std::string, llvm::StructType*> ObjectType::StructureTypes = {};

   ObjectType::ObjectType(string &&className, unordered_map<string, Type*>& concreteGenerics) :
      concreteGenericTypes(concreteGenerics)
   {
      id = TypeID::ObjectTypeID;
      this->className = className;
   }

   ObjectType::ObjectType(string &&className) {
      id = TypeID::ObjectTypeID;
      this->className = className;
   }

   ObjectType::ObjectType(string &className) {
      id = TypeID::ObjectTypeID;
      this->className = className;
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

   namespace {

      llvm::Value* CastToProtocol(
         llvm::Value *&val,
         cl::Class *&self,
         cl::Class *&proto,
         llvm::IRBuilder<> &Builder)
      {
         auto protoTy = ObjectType::getStructureType(proto->getName());
         auto alloca = CGMemory::CreateAlloca(protoTy);
         auto& vtbl = self->getProtocolVtable(proto->getName());

         if (vtbl != nullptr) {
            auto vtblPtr = Builder.CreateStructGEP(protoTy, alloca, 0);
            Builder.CreateStore(vtbl, vtblPtr);
         }

         auto objPtr = Builder.CreateStructGEP(protoTy, alloca, 1);
         Builder.CreateStore(
            Builder.CreateBitCast(val, Builder.getInt8PtrTy()),
            objPtr
         );

         return alloca;
      }

      llvm::Value* CastFromProtocol(
         llvm::Value *&val,
         cl::Class *&proto,
         cl::Class *&destClass,
         llvm::IRBuilder<> &Builder
      ) {
         auto protoTy = ObjectType::getStructureType(proto->getName());
         auto selfGep = Builder.CreateStructGEP(protoTy, val, 1);

         return Builder.CreateBitCast(
            Builder.CreateLoad(selfGep),
            ObjectType::getStructureType(destClass->getName())->getPointerTo()
         );
      }

      llvm::Value* ApplyDynamicDownCast(
         llvm::Value*& val,
         Class *&other,
         llvm::IRBuilder<> &Builder)
      {
         const auto Int8PtrTy = Builder.getInt8PtrTy();
         const auto ZERO = Builder.getInt64(0);
         const auto& TypeInfoTy = Class::TypeInfoType;

         // the ID we're looking for while going up the tree
         auto baseID = Builder.getInt64(other->getTypeID());
         auto vtbl = CodeGenVisitor::CreateLoad(CodeGenVisitor::AccessField(0, val));
         llvm::Value* currentTypeInfo = CGMemory::CreateAlloca(TypeInfoTy->getPointerTo());

         auto firstTypeInfo = Builder.CreateBitCast(
            CodeGenVisitor::CreateLoad(Builder.CreateInBoundsGEP(vtbl, { ZERO, ZERO })),
            TypeInfoTy->getPointerTo()
         );

         CodeGenVisitor::CreateStore(
            firstTypeInfo,
            currentTypeInfo
         );

         auto mergeBB = CodeGenVisitor::CreateBasicBlock("dyncast.merge");
         auto successBB = CodeGenVisitor::CreateBasicBlock("dyncast.success");
         auto failBB = CodeGenVisitor::CreateBasicBlock("dyncast.fail");
         auto loadBB = CodeGenVisitor::CreateBasicBlock("dyncast.load");
         auto compBB = CodeGenVisitor::CreateBasicBlock("dyncast.comp");

         // inital comparison
         auto parentTypeID = CodeGenVisitor::CreateLoad(
            Builder.CreateStructGEP(
               TypeInfoTy,
               firstTypeInfo,
               1
            )
         );

         auto comp = Builder.CreateICmpEQ(parentTypeID, baseID);
         Builder.CreateCondBr(comp, successBB, loadBB);

         // check if the parent vtable is null
         Builder.SetInsertPoint(loadBB);
         auto parentTypeInfo = CodeGenVisitor::CreateLoad(Builder.CreateStructGEP(
            TypeInfoTy,
            CodeGenVisitor::CreateLoad(currentTypeInfo),
            0
         ));

         parentTypeInfo = Builder.CreateBitCast(parentTypeInfo, TypeInfoTy->getPointerTo());

         auto isNull = Builder.CreateIsNull(parentTypeInfo);
         Builder.CreateCondBr(isNull, failBB, compBB);

         // check if we have reached the ID we're looking for
         Builder.SetInsertPoint(compBB);
         parentTypeID = CodeGenVisitor::CreateLoad(
            Builder.CreateStructGEP(
               TypeInfoTy,
               parentTypeInfo,
               1
            )
         );

         // store the new type info
         CodeGenVisitor::CreateStore(
            parentTypeInfo,
            currentTypeInfo
         );

         comp = Builder.CreateICmpEQ(parentTypeID, baseID);
         Builder.CreateCondBr(comp, successBB, loadBB);

         Builder.SetInsertPoint(successBB);
         Builder.CreateBr(mergeBB);

         Builder.SetInsertPoint(failBB);
         Builder.CreateBr(mergeBB);

         Builder.SetInsertPoint(mergeBB);
         auto phi = Builder.CreatePHI(Builder.getInt64Ty(), 2);
         phi->addIncoming(Builder.getInt64(0), failBB);
         phi->addIncoming(Builder.getInt64(1), successBB);

         auto alloca = CGMemory::CreateAlloca(ObjectType::getStructureType("Option"));
         CodeGenVisitor::SetField(0, alloca, phi);

         auto anyProto = CGMemory::CreateAlloca(ObjectType::getStructureType("Any"));
         CodeGenVisitor::SetField(1, anyProto, Builder.CreateBitCast(val, Int8PtrTy));
         CodeGenVisitor::SetField(1, alloca, Builder.CreateBitCast(anyProto, Int8PtrTy));

         return alloca;
      }
   }

   llvm::Value* ObjectType::castTo(llvm::Value *val, Type *destTy) {

      auto cl = SymbolTable::getClass(className);
      auto op = "infix as " + destTy->toString();

      std::vector<Type*> argTypes;
      auto castOp = cl->hasMethod(op, argTypes);

      if (castOp.compatibility == CompatibilityType::COMPATIBLE) {
         return Builder->CreateCall(CodeGenVisitor::getFunction(castOp.method->mangledName), { val });
      }

      switch (destTy->getTypeID()) {
         case TypeID::IntegerTypeID:
            return Builder->CreatePtrToInt(val, destTy->getLlvmType());
         case TypeID::FPTypeID: {
            // this one is a bit more tricky as there is no "ptrtodouble" instruction
            auto alloca = CGMemory::CreateAlloca(Builder->getInt64Ty());
            auto ptrToInt = Builder->CreatePtrToInt(val, Builder->getInt64Ty());
            CodeGenVisitor::CreateStore(ptrToInt, alloca);

            auto asFloatPtr = Builder->CreateBitCast(alloca, destTy->getLlvmType()->getPointerTo());

            return CodeGenVisitor::CreateLoad(asFloatPtr);
         }
         case TypeID::PointerTypeID: {
            return Builder->CreateBitCast(val, destTy->getLlvmType());
         }
         case TypeID::ObjectTypeID: {
            auto asObj = cast<ObjectType>(destTy);
            auto other = SymbolTable::getClass(destTy->getClassName());

            if (className == asObj->className) {
               return val;
            }

            auto self = SymbolTable::getClass(className);

            if (self->isProtocol()) {
               return CastFromProtocol(val, self, other, *Builder);
            }

            if (other->isProtocol()) {
               return CastToProtocol(val, self, other, *Builder);
            }

            // apply down cast, returns optional
            if (self->isBaseClassOf(other->getName())) {
               return ApplyDynamicDownCast(val, other, *Builder);
            }
         }
         default: {
            if (destTy->isStruct()) {
               destTy = destTy->getPointerTo();
            }

            return Builder->CreateBitCast(val, destTy->getLlvmType());
         }
      }
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
            auto cl = SymbolTable::getClass(className);
            auto op = "infix as " + other->toString();

            std::vector<Type*> argTypes;
            auto castOp = cl->hasMethod(op, argTypes);

            return castOp.compatibility == CompatibilityType::COMPATIBLE;
         }
         case TypeID::FPTypeID:{
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
            string boxedCl = string(asInt->isUnsigned() ? "U" : "") + "Int" + std::to_string(asInt->getBitwidth());

            return className == boxedCl || (asInt->getBitwidth() == sizeof(int*) * 8 && className == "Int");
         }
         case TypeID::FPTypeID: {
            auto asFloat = cast<FPType>(other);
            return className == asFloat->toString();
         }
         default:
            return false;
      }
   }

   bool ObjectType::hasDefaultValue() {
      auto cl = SymbolTable::getClass(className);
      auto constr = cl->hasMethod("init", {}, concreteGenericTypes);

      return constr.compatibility == CompatibilityType::COMPATIBLE;
   }

   llvm::Value* ObjectType::getDefaultVal() {
      auto alloca = CGMemory::CreateAlloca(StructureTypes[className]);
      auto cl = SymbolTable::getClass(className);
      auto constr = cl->hasMethod("init", {});

      assert(constr.compatibility == CompatibilityType::COMPATIBLE && "Should have been detected before");

      Builder->CreateCall(constr.method->llvmFunc, { alloca });

      return alloca;
   }

   bool ObjectType::isProtocol() {
      return SymbolTable::getClass(className)->isProtocol();
   }

   short ObjectType::getAlignment() {
      return SymbolTable::getClass(className)->getAlignment();
   }

   size_t ObjectType::getSize() {
      if (is_struct) {
         return SymbolTable::getClass(className)->getOccupiedBytes();
      }

      return 8;
   }

   llvm::Type* ObjectType::_getLlvmType() {
      if (is_struct) {
         return StructureTypes[className];
      }
      else {
         return StructureTypes[className]->getPointerTo();
      }
   }

   string ObjectType::toString() {
      auto str = className;
      if (!concreteGenericTypes.empty()) {
         str += "<";
         size_t size = concreteGenericTypes.size();
         size_t i = 0;
         for (const auto& gen : concreteGenericTypes) {
            str += gen.second->toString();
            if (i < size - 1) {
               str += ", ";
            }
         }
         str += ">";
      }

      return str;
   }

} // namespace cdot