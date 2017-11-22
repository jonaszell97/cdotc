//
// Created by Jonas Zell on 16.11.17.
//

#include "ConstantVal.h"

#include "../../../Variant/Type/Type.h"
#include "../../../Variant/Type/FPType.h"
#include "../../../Variant/Type/IntegerType.h"
#include "../../../Variant/Type/PointerType.h"
#include "../../../Variant/Type/ObjectType.h"
#include "../../../Variant/Type/ArrayType.h"

namespace cdot {
namespace il {

ConstantInt::ConstantInt(Type *ty, uint64_t value)
   : Constant(ConstantIntID, ty), u64(value)
{
   assert(ty->isIntegerTy());
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

ConstantFloat::ConstantFloat(double val)
   : Constant(ConstantFloatID, FPType::getDoubleTy()), doubleVal(val)
{

}

ConstantFloat::ConstantFloat(float val)
   : Constant(ConstantFloatID, FPType::getFloatTy()), floatVal(val)
{

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

ConstantArray::ConstantArray(ArrayTy &&Arr)
   : Constant(ConstantArrayID, nullptr), vec(std::move(Arr))
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

ConstantArray::ConstantArray(Type *ty, size_t numElements)
   : Constant(ConstantArrayID, ArrayType::get(ty, numElements)),
     numElements(numElements), elementType(ty)
{

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
   : ConstantArray(std::move(Arr)), Owner(Owner)
{
   id = VTableID;
}

PTable::PTable(ArrayTy &&Arr, PositionMap &&PosMap, ClassType *Owner)
   : ConstantArray(std::move(Arr)), ContainedProtocols(std::move(PosMap)),
     Owner(Owner)
{
   id = PTableID;
}

ConstantStruct::ConstantStruct(AggregateType *structTy,
                               llvm::ArrayRef<Constant *> vec)
   : Constant(ConstantStructID, ObjectType::get(structTy->getName())),
     elements(vec.begin(), vec.end())
{

}

const ConstantStruct::ElementTy &ConstantStruct::getElements() const
{
   return elements;
}

} // namespace il
} // namespace cdot