//
// Created by Jonas Zell on 16.11.17.
//

#include "ConstantVal.h"

#include "../../Module/Module.h"

#include "../../../Variant/Type/Type.h"

using std::string;

namespace cdot {
namespace il {

ConstantString* ConstantString::get(PointerType *Int8PtrTy,
                                    const std::string &val) {
   return new ConstantString(Int8PtrTy, val);
}

ConstantArray* ConstantArray::get(ArrayType *ty, ArrayTy &&Arr)
{
   return new ConstantArray(ty, std::move(Arr));
}

ConstantArray *ConstantArray::get(ArrayType *ty,
                                  llvm::ArrayRef<Constant*> vec) {
   return new ConstantArray(ty, vec);
}

ConstantArray *ConstantArray::get(ArrayType *ty)
{
   return new ConstantArray(ty);
}

ConstantStruct::ConstantStruct(AggregateType *structTy,
                               llvm::ArrayRef<Constant *> vec)
   : Constant(ConstantStructID, structTy->getType()),
     structTy(structTy), elements(vec.begin(), vec.end())
{

}

ConstantStruct::ConstantStruct(TypeID id, AggregateType *structTy,
                               llvm::ArrayRef<Constant *> vec)
   : Constant(id, structTy->getType()),
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
   assert(ty->isRefcounted() || ty->isPointerType() || ty->isRawFunctionTy());
}

ConstantPointer* ConstantPointer::get(Type *ty, uintptr_t value)
{
   return new ConstantPointer(ty, value);
}

} // namespace il
} // namespace cdot