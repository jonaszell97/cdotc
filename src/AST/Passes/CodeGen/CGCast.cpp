//
// Created by Jonas Zell on 31.08.17.
//

#include "CGCast.h"
#include "CodeGen.h"
#include "CGMemory.h"

#include "../../SymbolTable.h"
#include "../SemanticAnalysis/Record/Enum.h"

#include "../../../Variant/Type/IntegerType.h"
#include "../../../Variant/Type/FunctionType.h"
#include "../../../Variant/Type/FPType.h"
#include "../../../Variant/Type/TupleType.h"

namespace cdot {
namespace codegen {
   
   CGCast::CGCast(CodeGen &CGM) : CGM(CGM)
   {
      
   }

   llvm::Value* CGCast::applyCast(
      Type& from,
      Type& to,
      llvm::Value *val)
   {
      auto &Builder = CGM.Builder;
      
      if (from.needsLvalueToRvalueConv() && !to.isLvalue()) {
         val = CGM.CreateLoad(val);
         from.isLvalue(false);
      }

      if (to->isBoxedEquivOf(*from)
         || (to->isObject() && to->getClassName() == "Char" && from->isIntegerTy())
         || (to->isObject() && to->getClassName() == "Bool" && from->isIntegerTy()))
      {
         auto structTy = CodeGen::getStructTy(to->getClassName());
         auto alloca = CGM.Mem->CreateAlloca(structTy);
         Builder.CreateCall(
            CGM.getOwnDecl(SymbolTable::getClass(to->getClassName())->getMemberwiseInitializer()),
            { alloca, val }
         );

         return alloca;
      }

      if (from->isBoxedEquivOf(*to)
         || (from->isObject() && from->getClassName() == "Char" && to->isIntegerTy())
         || (from->isObject() && from->getClassName() == "Bool" && to->isIntegerTy()))
      {
         return CGM.CreateLoad(
            CGM.AccessField(0, val)
         );
      }

      if (from->isBoxedPrimitive() && to->isNumeric()) {
         val = CGM.CreateLoad(CGM.AccessField(0, val));

         Type unboxed(from->unbox());
         if (to->isIntegerTy()) {
            return integralCast(unboxed, to, val);
         }
         else {
            return floatingPointCast(unboxed, to, val);
         }
      }

      if (to->isBoxedPrimitive() && from->isNumeric()) {
         llvm::Value* cast;
         Type unboxed(to->unbox());
         if (from->isIntegerTy()) {
            cast = integralCast(from, unboxed, val);
         }
         else {
            cast = floatingPointCast(from, unboxed, val);
         }

         //FIXME remove once integer types implemented
         llvm::StructType* structTy;
         if (!SymbolTable::hasClass(to->getClassName())) {
            structTy = CodeGen::getStructTy("Int");
         }
         else {
            structTy = CodeGen::getStructTy(to->getClassName());
         }

         auto alloca = CGM.Mem->CreateAlloca(structTy);
         Builder.CreateCall(
            CGM.getOwnDecl(SymbolTable::getClass(to->getClassName())->getMemberwiseInitializer()),
            { alloca, cast }
         );

         return alloca;
      }

      auto res = hasCastOperator(from, to);
      if (res.compatibility == CompatibilityType::COMPATIBLE) {
         return castOperator(from, to, val, res);
      }

      if (from->isIntegerTy()) {
         return integralCast(from, to, val);
      }

      if (from->isFPType()) {
         return floatingPointCast(from, to, val);
      }

      if (from->isPointerTy()) {
         return pointerCast(from, to, val);
      }

      if (from->isTupleTy()) {
         return tupleCast(from, to ,val);
      }

      if (from->isProtocol()) {
         if (to->isProtocol()) {
            return protoToProtoCast(from, to, val);
         }

         return castFromProtocol(from, to, val);
      }

      if (from->isObject()) {
         if (!to->isObject()) {
            return pointerCast(from, to, val);
         }

         if (from->getClassName() == to->getClassName()) {
            return val;
         }

         if (to->isProtocol()) {
            return castToProtocol(from, to, val);
         }

         auto self = SymbolTable::getClass(from->getClassName());
         if (self->isBaseClassOf(to->getClassName())) {
            return dynamicDowncast(from, to, val);
         }

         auto other = SymbolTable::getClass(to->getClassName());
         if (other->isBaseClassOf(from->getClassName())) {
            return staticUpcast(from, to, val);
         }

         // TODO unsafe cast
         auto destTy = to->getLlvmType();
         if (!destTy->isPointerTy()) {
            destTy = destTy->getPointerTo();
         }

         return Builder.CreateBitCast(val, destTy);
      }

      if (from->isFunctionTy() && to->isFunctionTy()) {
         llvm_unreachable("Should be treated specially");
      }

      if (from->isRawFunctionTy()) {
         if (to->isPointerTy()) {
            return Builder.CreateBitCast(val, to->getLlvmType());
         }
         if (to->isIntegerTy()) {
            return Builder.CreatePtrToInt(val, to->getLlvmType());
         }
      }

      if (to->isRawFunctionTy()) {
         if (from->isPointerTy()) {
            return Builder.CreateBitCast(val, to->getLlvmType());
         }
         if (from->isIntegerTy()) {
            return Builder.CreateIntToPtr(val, to->getLlvmType());
         }
      }

      llvm_unreachable("Unsupported cast!");
   }

   llvm::Value* CGCast::integralCast(
      Type& from,
      Type& to,
      llvm::Value *val)
   {
      auto &Builder = CGM.Builder;
      auto toType = to->getLlvmType();
      if (to.isLvalue()) {
         toType = toType->getPointerTo();
      }

      if (to->isEnum() && to->getRecord()->isRawEnum()) {
         *to = static_cast<cl::Enum*>(to->getRecord())->getRawType();
      }

      if (from->isEnum() && from->getRecord()->isRawEnum()) {
         *from = static_cast<cl::Enum*>(from->getRecord())->getRawType();
      }

      switch (to->getTypeID()) {
         case TypeID::IntegerTypeID: {
            if (to->isUnsigned()) {
               return Builder.CreateZExtOrTrunc(val, toType);
            }

            return Builder.CreateSExtOrTrunc(val, toType);
         }
         case TypeID::FPTypeID:
            if (from->isUnsigned()) {
               return Builder.CreateUIToFP(val, toType);
            }

            return Builder.CreateSIToFP(val, toType);
         case TypeID::ObjectTypeID: {
            auto other = SymbolTable::getClass(to->getClassName());

            if (other->isProtocol()) {
               assert(other->getName() == "Any" && "Integers only conform to any!");
               auto AnyTy = CodeGen::getStructTy("Any");
               auto alloca = CGM.Mem->CreateAlloca(AnyTy);

               auto objPtr = Builder.CreateStructGEP(AnyTy, alloca, Class::ProtoObjPos);
               Builder.CreateStore(
                  val,
                  Builder.CreateBitCast(objPtr, val->getType()->getPointerTo())
               );

               auto sizePtr = Builder.CreateStructGEP(AnyTy, alloca, Class::ProtoSizePos);
               Builder.CreateStore(
                  CGM.wordSizedInt(CGM.getAlignment(val)),
                  sizePtr
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
      Type& from,
      Type& to,
      llvm::Value *val)
   {
      auto &Builder = CGM.Builder;
      auto asFP = cast<FPType>(*from);
      auto toType = to->getLlvmType();

      if (to.isLvalue()) {
         toType = toType->getPointerTo();
      }

      switch (to->getTypeID()) {
         case TypeID::IntegerTypeID: {
            if (to->isUnsigned()) {
               return Builder.CreateFPToUI(val, toType);
            }

            return Builder.CreateFPToSI(val, toType);
         }
         case TypeID::FPTypeID:
            return Builder.CreateFPCast(val, toType);
         case TypeID::ObjectTypeID: {
            auto other = SymbolTable::getClass(to->getClassName());

            if (other->isProtocol()) {
               assert(other->getName() == "Any" && "Floats only conform to any!");
               auto AnyTy = CodeGen::getStructTy("Any");
               auto alloca = CGM.Mem->CreateAlloca(AnyTy);

               auto objPtr = Builder.CreateStructGEP(AnyTy, alloca, 1);
               Builder.CreateStore(
                  val,
                  Builder.CreateBitCast(objPtr, val->getType()->getPointerTo())
               );

               auto sizePtr = Builder.CreateStructGEP(AnyTy, alloca, Class::ProtoSizePos);
               Builder.CreateStore(
                  CGM.wordSizedInt(CGM.getAlignment(val)),
                  sizePtr
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
      Type& from,
      Type& to,
      llvm::Value *val)
   {
      auto &Builder = CGM.Builder;
      auto toType = to->getLlvmType();
      if (to.isLvalue()) {
         toType = toType->getPointerTo();
      }

      switch (to->getTypeID()) {
         case TypeID::IntegerTypeID: {
            if (SymbolTable::hasClass(from->getClassName())) {
               auto cl = SymbolTable::getClass(from->getClassName());
               if (cl->isEnum() && !static_cast<cdot::cl::Enum*>(cl)->hasAssociatedValues()) {
                  return val;
               }
            }

            return Builder.CreatePtrToInt(val, toType);
         }
         case TypeID::FPTypeID: {
            val = Builder.CreateBitCast(val, toType->getPointerTo());
            return CGM.CreateLoad(val);
         }
         default:
            if (!toType->isPointerTy()) {
               toType = toType->getPointerTo();
            }

            return Builder.CreateBitCast(val, toType);
      }
   }

   llvm::Value* CGCast::tupleCast(
      Type& from,
      Type& to,
      llvm::Value *val)
   {
      assert(to->isTupleTy() && "cast shouldn't be allowed otherwise");

      auto &Builder = CGM.Builder;
      auto fromTuple = cast<TupleType>(*from);
      auto toTuple = cast<TupleType>(*to);

      assert(fromTuple->getArity() == toTuple->getArity() && "cast shouldn't be allowed otherwise");
      auto srcTy = fromTuple->getLlvmType();
      auto llvmTy = toTuple->getLlvmType();
      auto alloca = CGM.Mem->CreateAlloca(llvmTy);

      for (size_t i = 0; i < fromTuple->getArity(); ++i) {
         Type from(fromTuple->getContainedType(i));
         Type to(toTuple->getContainedType(i));

         llvm::Value* srcGep = Builder.CreateStructGEP(srcTy, val, i);
         llvm::Value* dstGep = Builder.CreateStructGEP(llvmTy, alloca, i);

         if (from->isStruct()) {
            Builder.CreateMemCpy(dstGep, srcGep, from->getSize(), from->getAlignment());
         }
         else {
            llvm::Value* castVal = CGM.CreateLoad(srcGep);
            if (from != to) {
               castVal = CGCast::applyCast(from, to, castVal);
            }

            Builder.CreateStore(castVal, dstGep);
         }
      }

      return alloca;
   }

   CallCompatability CGCast::hasCastOperator(
      Type& from,
      Type& to)
   {
      auto &Builder = CGM.Builder;
      if (!from->isObject()) {
         return CallCompatability();
      }

      auto cl = SymbolTable::getClass(from->getClassName());
      auto op = "infix as " + to->toString();

      auto castOp = cl->hasMethod(op);
      return castOp;
   }

   llvm::Value* CGCast::castOperator(
      Type& from,
      Type& to,
      llvm::Value *val,
      CallCompatability &Result)
   {
      return CGM.Builder.CreateCall(CGM.getOwnDecl(Result.method), { val });
   }

   llvm::Value* CGCast::staticUpcast(
      Type& from,
      Type& to,
      llvm::Value *val)
   {
      auto &Builder = CGM.Builder;

      assert(from->isObject() && to->isObject() && "Incompatible types for cast");
      auto toClass = SymbolTable::getClass(to->getClassName());

      assert(toClass->isBaseClassOf(from->getClassName()) && "Class is not a base class of target!");

      return Builder.CreateBitCast(val, to->getLlvmType());
   }

   llvm::Value* CGCast::castToProtocol(
      Type& from,
      Type& to,
      llvm::Value *val)
   {
      auto &Builder = CGM.Builder;
      auto proto = SymbolTable::getClass(to->getClassName());
      auto self = SymbolTable::getClass(from->getClassName());

      // we need to allocate the struct on the heap, since the value might escape
      // the current stack frame. FIXME handle deallocation
      if (self->isStruct()) {
         auto allocTy = from->getLlvmType();
         if (allocTy->isPointerTy()) {
            allocTy = allocTy->getPointerElementType();
         }

         auto alloca = CGM.Mem->CreateAlloca(allocTy, true);
         Builder.CreateMemCpy(alloca, val, from->getSize(), from->getAlignment());
         val = alloca;
      }

      auto protoTy = CodeGen::getStructTy(proto->getName());
      auto alloca = CGM.Mem->CreateAlloca(protoTy);
      auto vtbl = self->getProtocolVtable(proto->getName(), CGM);

      if (vtbl != nullptr) {
         auto vtblPtr = Builder.CreateStructGEP(protoTy, alloca, Class::ProtoVtblPos);
         Builder.CreateStore(vtbl, vtblPtr);
      }
      else {
         assert(proto->isEmptyProtocol() && "Didn't generate vtable for protocol!");
      }

      auto objPtr = Builder.CreateStructGEP(protoTy, alloca, Class::ProtoObjPos);
      if (from->isNumeric()) {
         Builder.CreateStore(
            val,
            Builder.CreateBitCast(objPtr, val->getType()->getPointerTo())
         );
      }
      else {
         Builder.CreateStore(
            Builder.CreateBitCast(val, Builder.getInt8PtrTy()),
            objPtr
         );
      }

      auto sizeGep = Builder.CreateStructGEP(protoTy, alloca, Class::ProtoSizePos);
      if (proto->isNonUnion()) {
         CGM.CreateStore(CGM.wordSizedInt(0), sizeGep);
      }
      else {
         CGM.CreateStore(CGM.wordSizedInt((int)self->getSize()), sizeGep);
      }

      return alloca;
   }

   llvm::Value* CGCast::castFromProtocol(
      Type& from,
      Type& to,
      llvm::Value *val)
   {
      auto &Builder = CGM.Builder;
      auto protoTy = CodeGen::getStructTy(from->getClassName());
      auto ty = to->getLlvmType();

      if (from.needsLvalueToRvalueConv()) {
         val = CGM.CreateLoad(val);
      }

      if (to->isPointerTy()) {
         return Builder.CreateBitCast(val, to->getLlvmType());
      }

      // in case of a struct or primitive type
      if (!ty->isPointerTy()) {
         ty = ty->getPointerTo();
      }

      auto selfGep = Builder.CreateStructGEP(protoTy, val, Class::ProtoObjPos);
      if (to->isIntegerTy() || to->isFPType()) {
         auto intPtr = Builder.CreateBitCast(selfGep, ty);
         if (to.isLvalue()) {
            return intPtr;
         }

         return CGM.CreateLoad(intPtr);
      }

      if (to.isLvalue()) {
         return Builder.CreateBitCast(
            selfGep,
            to->isRefcounted() ? ty : ty->getPointerTo()
         );
      }

      return Builder.CreateBitCast(
         Builder.CreateLoad(selfGep),
         ty
      );
   }

   llvm::Value* CGCast::protoToProtoCast(
      Type& from,
      Type& to,
      llvm::Value *val)
   {
      auto &Builder = CGM.Builder;
      auto fromProto = SymbolTable::getClass(from->getClassName());
      auto offset = fromProto->getVTableOffset(to->getClassName());
      auto toProtoTy = CodeGen::getStructTy(to->getClassName());
      auto alloca = CGM.Mem->CreateAlloca(toProtoTy);

      // shift the vtable pointer to the correct position
      llvm::Value* offsetPtr;
      if (SymbolTable::getClass(to->getClassName())->isEmptyProtocol()) {
         offsetPtr = llvm::ConstantPointerNull::get(
            llvm::cast<llvm::PointerType>(toProtoTy->getContainedType(Class::ProtoVtblPos)));
      }
      else {
         auto vtbl = CGM.AccessField(0, val);
         vtbl = CGM.CreateLoad(vtbl);
         offsetPtr = Builder.CreateInBoundsGEP(vtbl, { Builder.getInt64(0), Builder.getInt64(offset) });
      }

      CGM.SetField(
         Class::ProtoVtblPos,
         alloca,
         Builder.CreateBitCast(offsetPtr, toProtoTy->getContainedType(Class::ProtoVtblPos))
      );
      CGM.SetField(
         Class::ProtoObjPos,
         alloca,
         CGM.CreateLoad(
            CGM.AccessField(Class::ProtoObjPos, val)
         )
      );
      CGM.SetField(
         Class::ProtoSizePos,
         alloca,
         CGM.CreateLoad(
            CGM.AccessField(Class::ProtoSizePos, val)
         )
      );

      return alloca;
   }

   llvm::Value* CGCast::dynamicDowncast(
      Type& from,
      Type& to,
      llvm::Value *val)
   {
      auto &Builder = CGM.Builder;
      const auto Int8PtrTy = Builder.getInt8PtrTy();
      const auto ZERO = Builder.getInt64(0);
      const auto& TypeInfoTy = CGM.TypeInfoType;

      // the ID we're looking for while going up the tree
      auto baseID = Builder.getInt64(SymbolTable::getClass(to->getClassName())->getTypeID());
      auto vtbl = CGM.CreateLoad(CGM.AccessField(0, val));
      llvm::Value* currentTypeInfo = CGM.Mem->CreateAlloca(TypeInfoTy->getPointerTo());

      auto firstTypeInfo = Builder.CreateBitCast(
         CGM.CreateLoad(Builder.CreateInBoundsGEP(vtbl, { ZERO, ZERO })),
         TypeInfoTy->getPointerTo()
      );

      CGM.CreateStore(
         firstTypeInfo,
         currentTypeInfo
      );

      auto mergeBB = CGM.CreateBasicBlock("dyncast.merge");
      auto successBB = CGM.CreateBasicBlock("dyncast.success");
      auto failBB = CGM.CreateBasicBlock("dyncast.fail");
      auto loadBB = CGM.CreateBasicBlock("dyncast.load");
      auto compBB = CGM.CreateBasicBlock("dyncast.comp");

      // inital comparison
      auto parentTypeID = CGM.CreateLoad(
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
      auto parentTypeInfo = CGM.CreateLoad(Builder.CreateStructGEP(
         TypeInfoTy,
         CGM.CreateLoad(currentTypeInfo),
         0
      ));

      parentTypeInfo = Builder.CreateBitCast(parentTypeInfo, TypeInfoTy->getPointerTo());

      auto isNull = Builder.CreateIsNull(parentTypeInfo);
      Builder.CreateCondBr(isNull, failBB, compBB);

      // check if we have reached the ID we're looking for
      Builder.SetInsertPoint(compBB);
      parentTypeID = CGM.CreateLoad(
         Builder.CreateStructGEP(
            TypeInfoTy,
            parentTypeInfo,
            1
         )
      );

      // store the new type info
      CGM.CreateStore(
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

      auto alloca = CGM.Mem->CreateAlloca(CodeGen::getStructTy("Option"));
      CGM.SetField(0, alloca, phi);

      auto anyProto = CGM.Mem->CreateAlloca(CodeGen::getStructTy("Any"));
      CGM.SetField(1, anyProto, Builder.CreateBitCast(val, Int8PtrTy));
      CGM.SetField(1, alloca, Builder.CreateBitCast(anyProto, Int8PtrTy));

      return alloca;
   }

   // wrap a function that takes the needed parameters and casts them to the needed types,
   // then calls the original function
   llvm::Value* CGCast::functionCast(
      Type &from,
      Type &to,
      llvm::Value *val,
      llvm::Function* func,
      bool isLambda,
      bool hasSelfParam,
      bool hasStructRet)
   {
      auto &Builder = CGM.Builder;
      auto funcTy = to->asFunctionTy()->getLlvmFunctionType();
      assert(funcTy->isFunctionTy() && "getLlvmFunctionType didn't return a function type");

      auto prevBB = Builder.GetInsertBlock();
      auto wrapperFunc = llvm::Function::Create(
         llvm::cast<llvm::FunctionType>(funcTy),
         llvm::Function::PrivateLinkage, func->getName() + "__wrapped",
         CGM.Module.get()
      );

      auto entryBB = llvm::BasicBlock::Create(CGM.Context, "entry", wrapperFunc);
      Builder.SetInsertPoint(entryBB);
      CGM.Mem->StackAllocBlock.push(entryBB);

      size_t i = 0;
      size_t sRetPos = 1;
      auto argValIt = wrapperFunc->arg_begin();
      std::vector<llvm::Value*> castArgs;
      auto& toArgs = to->asFunctionTy()->getArgTypes();

      if (isLambda) {
         // env param
         castArgs.push_back(&*argValIt);
         ++argValIt;
         ++sRetPos;
      }
      else if (hasSelfParam) {
         castArgs.push_back(&*argValIt);
         ++argValIt;
         ++sRetPos;
      }
      if (hasStructRet) {
         castArgs.push_back(&*argValIt);
         ++argValIt;
      }

      for (auto& arg : from->asFunctionTy()->getArgTypes()) {
         if (i > toArgs.size() || argValIt == wrapperFunc->arg_end()) {
            break;
         }

         llvm::Value* argVal = &*argValIt;
         if (arg.type != toArgs[i].type) {
            // reverse cast
            argVal = applyCast(toArgs[i].type, arg.type, argVal);
         }

         ++i;
         ++argValIt;
         castArgs.push_back(argVal);
      }

      auto call = Builder.CreateCall(func, castArgs);
      if (hasStructRet) {
         call->addAttribute(sRetPos, llvm::Attribute::NoAlias);
         call->addAttribute(sRetPos, llvm::Attribute::StructRet);

         wrapperFunc->addAttribute(sRetPos, llvm::Attribute::NoAlias);
         wrapperFunc->addAttribute(sRetPos, llvm::Attribute::StructRet);
      }

      if (!funcTy->getContainedType(0)->isVoidTy()) {
         Builder.CreateRet(call);
      }
      else {
         Builder.CreateRetVoid();
      }

      Builder.SetInsertPoint(prevBB);
      CGM.Mem->StackAllocBlock.pop();

      auto alloca = CGM.Mem->CreateAlloca(CGM.LambdaTy);
      Builder.CreateStore(
         Builder.CreateBitCast(wrapperFunc, Builder.getInt8PtrTy()),
         CGM.AccessField(CGM.LambdaFuncPtrPos, alloca)
      );
      Builder.CreateStore(
         CGM.CreateLoad(CGM.AccessField(CGM.LambdaEnvPos, val)),
         CGM.AccessField(CGM.LambdaEnvPos, alloca)
      );

      return alloca;
   }
}
}