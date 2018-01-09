//
// Created by Jonas Zell on 16.11.17.
//

#include "ConstantVal.h"

#include "../../Module/Module.h"

#include "../../../Variant/Type/Type.h"
#include "../../../Variant/Type/FPType.h"
#include "../../../Variant/Type/IntegerType.h"
#include "../../../Variant/Type/PointerType.h"
#include "../../../Variant/Type/ObjectType.h"
#include "../../../Variant/Type/ArrayType.h"

using std::string;

namespace cdot {
namespace il {

ConstantInt::ConstantInt(Type *ty, uint64_t value)
   : Constant(ConstantIntID, ty), u64(value)
{
   assert(ty->isIntegerTy());
}

ConstantInt* ConstantInt::get(Type *ty, uint64_t value)
{
   return new ConstantInt(ty, value);
}

uint64_t ConstantInt::getU64() const
{
   return u64;
}

uint32_t ConstantInt::getU32() const
{
   return u32;
}

uint16_t ConstantInt::getU16() const
{
   return u16;
}

uint8_t ConstantInt::getU8() const
{
   return u8;
}

bool ConstantInt::getU1() const
{
   return u1;
}

bool ConstantInt::isUnsigned() const
{
   return type->isUnsigned();
}

ConstantInt *ConstantInt::getCTFE()
{
   auto I = get(IntegerType::getBoolTy(), 0);
   I->u64 = Flags::Ctfe;

   return I;
}

bool ConstantInt::isCTFE() const
{
   if (!type->isInt1Ty())
      return false;

   return (u64 & Flags::Ctfe) != 0;
}

ConstantFloat::ConstantFloat(double val)
   : Constant(ConstantFloatID, FPType::getDoubleTy()), doubleVal(val)
{

}

ConstantFloat::ConstantFloat(float val)
   : Constant(ConstantFloatID, FPType::getFloatTy()), floatVal(val)
{

}

ConstantFloat* ConstantFloat::get(float val)
{
   return new ConstantFloat(val);
}

ConstantFloat* ConstantFloat::get(double val)
{
   return new ConstantFloat(val);
}

double ConstantFloat::getDoubleVal() const
{
   return doubleVal;
}

float ConstantFloat::getFloatVal() const
{
   return floatVal;
}

ConstantString::ConstantString(const std::string &val)
   : Constant(ConstantStringID, PointerType::get(IntegerType::getCharTy())),
     value(val)
{

}

ConstantString* ConstantString::get(const std::string &val)
{
   return new ConstantString(val);
}

const string &ConstantString::getValue() const
{
   return value;
}

ConstantArray::ConstantArray(llvm::ArrayRef<Constant *> vec)
   : Constant(ConstantArrayID, nullptr),
     vec(vec.begin(), vec.end())
{
   if (vec.empty()) {
      elementType = IntegerType::get(8)->getPointerTo();
      *type = ArrayType::get(elementType, 0);
   }
   else {
      *type = ArrayType::get(*vec.front()->getType(), vec.size());
      elementType = *vec.front()->getType();
   }

   numElements = vec.size();
}

ConstantArray::ConstantArray(ArrayTy &&Arr, bool useInt8PtrTy)
   : Constant(ConstantArrayID, nullptr), vec(std::move(Arr))
{
   if (useInt8PtrTy) {
      elementType = IntegerType::get(8)->getPointerTo();
      *type = ArrayType::get(elementType, vec.size());
   }
   else if (vec.empty()) {
      elementType = IntegerType::get(8)->getPointerTo();
      *type = ArrayType::get(elementType, 0);
   }
   else {
      *type = ArrayType::get(*vec.front()->getType(), vec.size());
      elementType = *vec.front()->getType();
   }

   numElements = vec.size();
}

ConstantArray::ConstantArray(Type *ty, size_t numElements)
   : Constant(ConstantArrayID, ArrayType::get(ty, numElements)),
     elementType(ty), numElements(numElements)
{

}

ConstantArray* ConstantArray::get(ArrayTy &&Arr)
{
   return new ConstantArray(std::move(Arr));
}

ConstantArray *ConstantArray::get(llvm::ArrayRef<Constant *> vec)
{
   return new ConstantArray(vec);
}

ConstantArray *ConstantArray::get(Type *ty, size_t numElements)
{
   return new ConstantArray(ty, numElements);
}

const ConstantArray::ArrayTy &ConstantArray::getVec() const
{
   return vec;
}

size_t ConstantArray::getNumElements() const
{
   return numElements;
}

Type *ConstantArray::getElementType() const
{
   return elementType;
}

VTable::VTable(ArrayTy &&Arr, ClassType *Owner)
   : ConstantArray(std::move(Arr), true), Owner(Owner)
{
   id = VTableID;
}

VTable* VTable::get(ArrayTy &&Arr, ClassType *Owner)
{
   return new VTable(std::move(Arr), Owner);
}

PTable::PTable(ArrayTy &&Arr, PositionMap &&PosMap, AggregateType *Owner)
   : ConstantArray(std::move(Arr), true),
     Owner(Owner), ContainedProtocols(std::move(PosMap))
{
   id = PTableID;
}

PTable* PTable::get(ArrayTy &&Arr, PositionMap &&PosMap, AggregateType *Owner)
{
   return new PTable(std::move(Arr), std::move(PosMap), Owner);
}

ConstantStruct::ConstantStruct(AggregateType *structTy,
                               llvm::ArrayRef<Constant *> vec)
   : Constant(ConstantStructID, ObjectType::get(structTy->getName())),
     structTy(structTy), elements(vec.begin(), vec.end())
{

}

ConstantStruct::ConstantStruct(TypeID id, AggregateType *structTy,
                               llvm::ArrayRef<Constant *> vec)
   : Constant(id, ObjectType::get(structTy->getName())),
     structTy(structTy), elements(vec.begin(), vec.end())
{

}

ConstantStruct* ConstantStruct::get(AggregateType *structTy,
                                    llvm::ArrayRef<Constant *> vec) {
   return new ConstantStruct(structTy, vec);
}

const ConstantStruct::ElementTy &ConstantStruct::getElements() const
{
   return elements;
}

TypeInfo::TypeInfo(Module *M, Type* forType,
                   il::Constant *ParentClass, il::Constant *TypeID,
                   il::Constant *TypeName, il::Constant *Deinitializer,
                   il::Constant *NumConformances, il::Constant *Conformances)
   : ConstantStruct(TypeInfoID, M->getType("cdot.TypeInfo"),
                    { ParentClass, TypeID, TypeName, Deinitializer,
                       NumConformances, Conformances }),
     forType(forType)
{

}

TypeInfo* TypeInfo::get(Module *M, Type* forType,
                        il::Constant *ParentClass, il::Constant *TypeID,
                        il::Constant *TypeName, il::Constant *Deinitializer,
                        il::Constant *NumConformances,
                        il::Constant *Conformances) {
   return new TypeInfo(M, forType, ParentClass, TypeID, TypeName, Deinitializer,
                       NumConformances, Conformances);
}

ConstantPointer::ConstantPointer(Type *ty, uintptr_t value)
   : Constant(ConstantPointerID, ty), value(value)
{
   assert(ty->isRefcounted() || ty->isPointerTy() || ty->isRawFunctionTy());
}

ConstantPointer* ConstantPointer::get(Type *ty, uintptr_t value)
{
   return new ConstantPointer(ty, value);
}

} // namespace il
} // namespace cdot