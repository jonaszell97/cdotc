//
// Created by Jonas Zell on 31.08.17.
//

#include "CGCast.h"
#include "CodeGenVisitor.h"
#include "../../SymbolTable.h"
#include "../../../Variant/Type/IntegerType.h"
#include "../../../Variant/Type/FPType.h"
#include "../../../Variant/Type/TupleType.h"
#include "CGMemory.h"

namespace cdot {
namespace codegen {

   llvm::Value* CGCast::applyCast(
      Type *from,
      Type *to,
      llvm::Value *val,
      llvm::IRBuilder<> &Builder)
   {
      if (to->needsLvalueToRvalueConv()) {
         to->isLvalue(false);
         to = to->getPointerTo();
      }

      auto res = hasCastOperator(from, to, Builder);
      if (res.compatibility == CompatibilityType::COMPATIBLE) {
         return castOperator(from, to, val, res, Builder);
      }

      if (from->isIntegerTy()) {
         return integralCast(from, to, val, Builder);
      }

      if (from->isFPType()) {
         return floatingPointCast(from, to, val, Builder);
      }

      if (from->isPointerTy()) {
         return pointerCast(from, to, val, Builder);
      }

      if (from->isTupleTy()) {
         return tupleCast(from, to ,val, Builder);
      }

      if (from->isProtocol()) {
         return castFromProtocol(from, to, val, Builder);
      }

      if (from->isObject()) {
         if (!to->isObject()) {
            return pointerCast(from, to, val, Builder);
         }

         if (from->getClassName() == to->getClassName()) {
            return val;
         }

         if (to->isProtocol()) {
            return castToProtocol(from, to, val, Builder);
         }

         auto self = SymbolTable::getClass(from->getClassName());
         if (self->isBaseClassOf(to->getClassName())) {
            return dynamicDowncast(from, to, val, Builder);
         }

         auto other = SymbolTable::getClass(to->getClassName());
         if (other->isBaseClassOf(from->getClassName())) {
            return staticUpcast(from, to, val, Builder);
         }

         llvm_unreachable("Unsupported object cast!");
      }

      llvm_unreachable("Unsupported cast!");
   }

   llvm::Value* CGCast::integralCast(
      Type *from,
      Type *to,
      llvm::Value *val,
      llvm::IRBuilder<> &Builder)
   {
      auto asInt = cast<IntegerType>(from);
      auto toType = to->getLlvmType();

      if (to->isLvalue()) {
         toType = toType->getPointerTo();
      }

      switch (to->getTypeID()) {
         case TypeID::IntegerTypeID:
            return Builder.CreateSExtOrTrunc(val, toType);
         case TypeID::FPTypeID:
            if (asInt->isUnsigned()) {
               return Builder.CreateUIToFP(val, toType);
            }

            return Builder.CreateSIToFP(val, toType);
         case TypeID::ObjectTypeID: {
            auto other = SymbolTable::getClass(to->getClassName());

            if (other->isProtocol()) {
               assert(other->getName() == "Any" && "Integers only conform to any!");
               auto AnyTy = ObjectType::getStructureType("Any");
               auto alloca = CGMemory::CreateAlloca(AnyTy);

               auto objPtr = Builder.CreateStructGEP(AnyTy, alloca, 1);
               Builder.CreateStore(
                  Builder.CreateIntToPtr(val, Builder.getInt8PtrTy()),
                  objPtr
               );

               return alloca;
            }
         }
         default:
            assert(toType->isPointerTy() && "Can't inttoptr to non-pointer type!");
            return Builder.CreateIntToPtr(val, toType);
      }
   }

   llvm::Value* CGCast::floatingPointCast(
      Type *from,
      Type *to,
      llvm::Value *val,
      llvm::IRBuilder<> &Builder)
   {
      auto asFP = cast<FPType>(from);
      auto toType = to->getLlvmType();

      if (to->isLvalue()) {
         toType = toType->getPointerTo();
      }

      switch (to->getTypeID()) {
         case TypeID::IntegerTypeID: {
            auto asInt = cast<IntegerType>(to);
            if (asInt->isUnsigned()) {
               return Builder.CreateUIToFP(val, toType);
            }

            return Builder.CreateSIToFP(val, toType);
         }
         case TypeID::FPTypeID:
            return Builder.CreateFPCast(val, toType);
         case TypeID::ObjectTypeID: {
            auto other = SymbolTable::getClass(to->getClassName());

            if (other->isProtocol()) {
               assert(other->getName() == "Any" && "Floats only conform to any!");
               auto AnyTy = ObjectType::getStructureType("Any");
               auto alloca = CGMemory::CreateAlloca(AnyTy);
               auto floatAlloca = CGMemory::CreateAlloca(from->getLlvmType());
               CodeGenVisitor::CreateStore(val, floatAlloca);

               floatAlloca = Builder.CreateBitCast(floatAlloca, Builder.getIntNTy(sizeof(int*))->getPointerTo());

               auto objPtr = Builder.CreateStructGEP(AnyTy, alloca, 1);
               Builder.CreateStore(
                  Builder.CreateBitCast(floatAlloca, Builder.getInt8PtrTy()),
                  objPtr
               );

               return alloca;
            }
         }
         default: {
            assert(val->getType()->isPointerTy() && "Can't inttoptr to non-pointer type!");
            auto asIntPtr = Builder.CreateBitCast(val, Builder.getIntNTy(sizeof(int*))->getPointerTo());
            return Builder.CreateBitCast(asIntPtr, toType);
         }
      }
   }

   llvm::Value* CGCast::pointerCast(
      Type *from,
      Type *to,
      llvm::Value *val,
      llvm::IRBuilder<> &Builder)
   {
      auto toType = to->getLlvmType();

      if (to->isLvalue()) {
         toType = toType->getPointerTo();
      }

      switch (to->getTypeID()) {
         case TypeID::IntegerTypeID:
            return Builder.CreatePtrToInt(val, toType);
         case TypeID::FPTypeID: {
            val = Builder.CreateBitCast(val, toType->getPointerTo());
            return CodeGenVisitor::CreateLoad(val);
         }
         default:
            return Builder.CreateBitCast(val, toType);
      }
   }

   llvm::Value* CGCast::tupleCast(
      Type *from,
      Type *to,
      llvm::Value *val,
      llvm::IRBuilder<> &Builder)
   {
      assert(to->isTupleTy() && "cast shouldn't be allowed otherwise");

      auto fromTuple = cast<TupleType>(from);
      auto toTuple = cast<TupleType>(to);

      assert(fromTuple->getArity() == toTuple->getArity() && "cast shouldn't be allowed otherwise");
      auto srcTy = fromTuple->getLlvmType();
      auto llvmTy = toTuple->getLlvmType();
      auto alloca = CGMemory::CreateAlloca(llvmTy);

      for (size_t i = 0; i < fromTuple->getArity(); ++i) {
         Type* from = fromTuple->getContainedType(i);
         Type* to = toTuple->getContainedType(i);

         llvm::Value* srcGep = Builder.CreateStructGEP(srcTy, val, i);
         llvm::Value* dstGep = Builder.CreateStructGEP(llvmTy, alloca, i);

         if (from->isStruct()) {
            Builder.CreateMemCpy(dstGep, srcGep, from->getSize(), from->getAlignment());
         }
         else {
            llvm::Value* castVal = CodeGenVisitor::CreateLoad(srcGep);
            if (*from != to) {
               castVal = from->castTo(castVal, to);
            }

            Builder.CreateStore(castVal, dstGep);
         }
      }

      return alloca;
   }

   MethodResult CGCast::hasCastOperator(
      Type *from,
      Type *to,
      llvm::IRBuilder<> &Builder)
   {
      if (!from->isObject()) {
         return MethodResult();
      }

      auto cl = SymbolTable::getClass(from->getClassName());
      auto op = "infix as " + to->toString();

      std::vector<Type*> argTypes;
      auto castOp = cl->hasMethod(op, argTypes);

      return castOp;
   }

   llvm::Value* CGCast::castOperator(
      Type *from,
      Type *to,
      llvm::Value *val,
      MethodResult &Result,
      llvm::IRBuilder<> &Builder)
   {
      return Builder.CreateCall(Result.method->llvmFunc, { val });
   }

   llvm::Value* CGCast::staticUpcast(
      Type *from,
      Type *to,
      llvm::Value *val,
      llvm::IRBuilder<> &Builder)
   {
      assert(from->isObject() && to->isObject() && "Incompatible types for cast");
      auto toClass = SymbolTable::getClass(to->getClassName());

      assert(toClass->isBaseClassOf(from->getClassName()) && "Class is not a base class of target!");

      return Builder.CreateBitCast(val, to->getLlvmType());
   }

   llvm::Value* CGCast::castToProtocol(
      Type *from,
      Type *to,
      llvm::Value *val,
      llvm::IRBuilder<> &Builder)
   {
      auto proto = SymbolTable::getClass(to->getClassName());
      auto self = SymbolTable::getClass(from->getClassName());

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

   llvm::Value* CGCast::castFromProtocol(
      Type *from,
      Type *to,
      llvm::Value *val,
      llvm::IRBuilder<> &Builder)
   {
      auto protoTy = ObjectType::getStructureType(from->getClassName());
      auto selfGep = Builder.CreateStructGEP(protoTy, val, 1);

      if (to->isLvalue()) {
         return Builder.CreateBitCast(
            selfGep,
            to->getLlvmType()
         );
      }

      auto ty = to->getLlvmType();
      if (ty->isFloatingPointTy()) {
         auto intPtr = Builder.CreateBitCast(Builder.CreateLoad(selfGep), ty->getPointerTo());
         return CodeGenVisitor::CreateLoad(intPtr);
      }

      if (ty->isIntegerTy()) {
         return Builder.CreatePtrToInt(Builder.CreateLoad(selfGep), ty);
      }

      // in case of a struct
      if (!ty->isPointerTy()) {
         ty = ty->getPointerTo();
      }

      return Builder.CreateBitCast(
         Builder.CreateLoad(selfGep),
         ty
      );
   }

   llvm::Value* CGCast::dynamicDowncast(
      Type *from,
      Type *to,
      llvm::Value *val,
      llvm::IRBuilder<> &Builder)
   {
      const auto Int8PtrTy = Builder.getInt8PtrTy();
      const auto ZERO = Builder.getInt64(0);
      const auto& TypeInfoTy = cdot::cl::Class::TypeInfoType;

      // the ID we're looking for while going up the tree
      auto baseID = Builder.getInt64(SymbolTable::getClass(to->getClassName())->getTypeID());
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
}