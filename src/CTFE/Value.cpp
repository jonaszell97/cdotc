//
// Created by Jonas Zell on 02.01.18.
//

#include <sstream>
#include <iomanip>

#include "Value.h"

#include "../IL/Value/Function/Function.h"

#include "../Variant/Type/ArrayType.h"
#include "../Variant/Type/TupleType.h"
#include "../Variant/Type/ObjectType.h"
#include "../Variant/Type/FunctionType.h"

#include "../AST/Passes/SemanticAnalysis/Record/Record.h"
#include "../AST/Passes/SemanticAnalysis/Record/Class.h"
#include "../AST/Passes/SemanticAnalysis/Record/Union.h"
#include "../AST/Passes/SemanticAnalysis/Record/Enum.h"

using std::string;

namespace cdot {
namespace ctfe {

llvm::SmallVector<void*, 128> Value::allocations;

void Value::destroyValue(Type *type)
{
   llvm::outs() << "freeing " << (void*)buffer << "\n";
   if (isa<FunctionType>(type)) {
      assert(!type->isRawFunctionTy());

      free(*reinterpret_cast<void**>(buffer));
      free(buffer);
   }
   else if (isa<PointerType>(type)) {
//      free(buffer);
   }
   else if (isa<ArrayType>(type)) {
      free(buffer);
   }
   else if (isa<TupleType>(type)) {
      free(buffer);
   }
   else if (isa<ObjectType>(type)) {
      if (!type->isStruct() && !type->isEnum())
         free(buffer);
   }
   else {
      llvm_unreachable("bad owning value kind");
   }
}

string Value::toString(Type *type)
{
   if (type->isIntegerTy()) {
      if (type->isUnsigned()) {
         switch (type->getBitwidth()) {
            case 1:
               return std::to_string(*(reinterpret_cast<bool*>(buffer)));
            case 8:
               return std::to_string(*(reinterpret_cast<uint8_t*>(buffer)));
            case 16:
               return std::to_string(*(reinterpret_cast<uint16_t*>(buffer)));
            case 32:
               return std::to_string(*(reinterpret_cast<uint32_t*>(buffer)));
            case 64:
               return std::to_string(*(reinterpret_cast<uint64_t*>(buffer)));
            default:
               llvm_unreachable("bad bitwidth");
         }
      }
      else {
         switch (type->getBitwidth()) {
            case 1:
               return std::to_string(*(reinterpret_cast<bool*>(buffer)));
            case 8:
               return std::to_string(*(reinterpret_cast<int8_t*>(buffer)));
            case 16:
               return std::to_string(*(reinterpret_cast<int16_t*>(buffer)));
            case 32:
               return std::to_string(*(reinterpret_cast<int32_t*>(buffer)));
            case 64:
               return std::to_string(*(reinterpret_cast<int64_t*>(buffer)));
            default:
               llvm_unreachable("bad bitwidth");
         }
      }
   }
   else if (type->isFloatTy()) {
      return std::to_string(*reinterpret_cast<float*>(buffer));
   }
   else if (type->isDoubleTy()) {
      return std::to_string(*reinterpret_cast<double*>(buffer));
   }
   else if (type->isPointerTy()) {
      if (type->getPointeeType()->isInt8Ty()) {
         return getString();
      }

      std::ostringstream s;
      s << "0x" << std::setbase(16) << reinterpret_cast<void*>(buffer);

      return s.str();
   }
   else if (type->isRawFunctionTy()) {
      return (reinterpret_cast<il::Function*>(buffer))->getName();
   }
   else if (type->isFunctionTy()) {
      std::ostringstream s;
      s << "Lambda(" << std::setbase(16) << buffer << std::setbase(10)
        << ", "
        << reinterpret_cast<il::Function*>(
           reinterpret_cast<uintptr_t *>(buffer) + 1)->getName().str()
        << ")";

      return s.str();
   }
   else if (auto ArrTy = dyn_cast<ArrayType>(type)) {
      string s = "[";
      auto elementTy = ArrTy->getElementType();
      bool sret = elementTy->needsStructReturn();

      for (size_t i = 0; i < ArrTy->getNumElements(); ++i) {
         if (i != 0) { s += ", "; }
         auto V = getArrayElement(type, i);
         if (sret) {
            s += V.toString(elementTy);
         }
         else {
            s += V.getValuePtr()->toString(elementTy);
         }
      }

      s += "]";
      return s;
   }
   else if (auto TupleTy = dyn_cast<TupleType>(type)) {
      string s = "(";
      char *ptr = buffer;
      auto &elements = TupleTy->getContainedTypes();

      for (size_t i = 0; i < elements.size(); ++i) {
         if (i != 0) { s += ", "; }

         auto V = getTupleElement(type, i);
         auto ty = *elements[i].second;

         if (ty->needsStructReturn()) {
            s += V.toString(ty);
         }
         else {
            s += V.getValuePtr()->toString(ty);
         }

         ptr += ty->getMemberSize();
      }

      s += ")";
      return s;
   }
   else if (auto Obj = dyn_cast<ObjectType>(type)) {
      auto R = Obj->getRecord();
      if (auto S = dyn_cast<Struct>(R)) {
         string s = "{ ";
         char *ptr = buffer;
         size_t i = 0;
         auto &fields = S->getFields();

         for (auto &F : fields) {
            if (F.isIsStatic()) continue;
            if (i != 0) { s += ", "; }

            s += F.getFieldName();
            s += ": ";

            auto V = getStructElement(type, i);
            if (F.getFieldType()->needsStructReturn()) {
               s += V.toString(*F.getFieldType());
            }
            else {
               s += V.getValuePtr()->toString(*F.getFieldType());
            }

            ptr += F.getFieldType()->getMemberSize();
            ++i;
         }

         s += " }";
         return s;
      }
      else if (auto U = dyn_cast<Union>(R)) {
         string s = "{ ";
         size_t i = 0;
         auto &fields = U->getFields();

         for (auto &F : fields) {
            if (i != 0) { s += ", "; }

            s += F.first;
            s += ": ";
            s += getPreallocated(F.second, buffer).toString(F.second);

            ++i;
         }

         s += " }";
         return s;
      }
      else if (auto E = dyn_cast<Enum>(R)) {
         uint64_t caseVal = getEnumRawValue(type).getU64();

         string s;
         for (auto &C : E->getCases()) {
            if (C.second.rawValue != caseVal)
               continue;

            if (!C.second.associatedValues.empty()) {
               size_t i = 0;
               s += "(";

               for (auto &V : C.second.associatedValues) {
                  auto val = getEnumCaseValue(type, C.second.name, i);
                  if (V.getType()->needsStructReturn()) {
                     s += val.toString(*V.getType());
                  }
                  else {
                     s += val.getValuePtr()->toString(*V.getType());
                  }

                  i++;
               }

               s += ")";
            }

            break;
         }

         if (s.empty())
            return "<invalid enum case>";

         return s;
      }
   }

   llvm_unreachable("bad value type");
}

Variant Value::toVariant(Type *type)
{
   if (type->isVoidTy())
      return {};

   if (type->isIntegerTy()) {
      if (type->isUnsigned()) {
         switch (type->getBitwidth()) {
            case 1:
            case 8:
               return Variant(type, (size_t)u8);
            case 16:
               return Variant(type, (size_t)u16);
            case 32:
               return Variant(type, (size_t)u32);
            case 64:
               return Variant(type, (size_t)u64);
            default:
               llvm_unreachable("bad bitwidth");
         }
      }
      else {
         switch (type->getBitwidth()) {
            case 1:
            case 8:
               return Variant(type, (size_t)i8);
            case 16:
               return Variant(type, (size_t)i16);
            case 32:
               return Variant(type, (size_t)i32);
            case 64:
               return Variant(type, (size_t)i64);
            default:
               llvm_unreachable("bad bitwidth");
         }
      }
   }
   else if (type->isFloatTy()) {
      return Variant(f);
   }
   else if (type->isDoubleTy()) {
      return Variant(d);
   }
   else if (type->isPointerTy()) {
      if (type->getPointeeType()->isInt8Ty()) {
         return Variant(string(buffer));
      }

      return Variant(type, reinterpret_cast<unsigned long long>(buffer));
   }
   else if (type->isRawFunctionTy()) {
      return Variant(F->getName().str());
   }
   else if (type->isFunctionTy()) {
      llvm_unreachable("todo");
   }
   else if (auto ArrTy = dyn_cast<ArrayType>(type)) {
      std::vector<Variant> vec;
      auto elementTy = ArrTy->getElementType();
      bool sret = elementTy->needsStructReturn();

      for (size_t i = 0; i < ArrTy->getNumElements(); ++i) {
         auto V = getArrayElement(type, i);
         if (sret) {
            vec.push_back(V.toVariant(elementTy));
         }
         else {
            vec.push_back(V.getValuePtr()->toVariant(elementTy));
         }
      }

      return Variant(std::move(vec), type);
   }
   else if (auto TupleTy = dyn_cast<TupleType>(type)) {
      std::vector<Variant> vec;
      auto &elements = TupleTy->getContainedTypes();

      for (size_t i = 0; i < elements.size(); ++i) {
         auto V = getTupleElement(type, i);
         auto ty = *elements[i].second;

         if (ty->needsStructReturn()) {
            vec.push_back(V.toVariant(ty));
         }
         else {
            vec.push_back(V.getValuePtr()->toVariant(ty));
         }
      }

      return Variant(std::move(vec), type);
   }
   else if (auto Obj = dyn_cast<ObjectType>(type)) {
      auto R = Obj->getRecord();
      if (auto S = dyn_cast<Struct>(R)) {
         std::vector<Variant::Field> vec;
         size_t i = 0;
         auto &fields = S->getFields();

         for (auto &F : fields) {
            if (F.isIsStatic()) continue;

            auto V = getStructElement(type, i);
            if (F.getFieldType()->needsStructReturn()) {
               vec.emplace_back(F.getFieldName(),
                                V.toVariant(*F.getFieldType()));
            }
            else {
               vec.emplace_back(F.getFieldName(),
                                V.getValuePtr()->toVariant(*F.getFieldType()));
            }

            ++i;
         }

         return Variant(type, std::move(vec));
      }
      else if (auto U = dyn_cast<Union>(R)) {
         std::vector<Variant::Field> vec;
         size_t i = 0;
         auto &fields = U->getFields();

         for (auto &F : fields) {
            vec.emplace_back(F.first, toVariant(F.second));
            ++i;
         }

         return Variant(type, std::move(vec));
      }
      else if (auto E = dyn_cast<Enum>(R)) {
         uint64_t caseVal = getEnumRawValue(type).getU64();

         std::vector<Variant> vec;
         for (auto &C : E->getCases()) {
            if (C.second.rawValue != caseVal)
               continue;

            if (!C.second.associatedValues.empty()) {
               size_t i = 0;
               for (auto &V : C.second.associatedValues) {
                  auto val = getEnumCaseValue(type, C.second.name, i);
                  if (V.getType()->needsStructReturn()) {
                     vec.push_back(val.toVariant(*V.getType()));
                  }
                  else {
                     vec.push_back(val.getValuePtr()->toVariant(*V.getType()));
                  }

                  i++;
               }
            }

            break;
         }

         return Variant(std::move(vec), type);
      }
   }

   llvm_unreachable("bad value type");
}

Value Value::getNullValue(Type *ty)
{
   if (ty->isIntegerTy()) {
      return getInt(ty, 0);
   }
   else if (ty->isFloatTy()) {
      return getFloat(0.0f);
   }
   else if (ty->isDoubleTy()) {
      return getDouble(0.0);
   }
   else if (ty->isPointerTy() || ty->isRawFunctionTy()) {
      return getConstPtr(ty, nullptr);
   }
   else if (ty->isFunctionTy()) {
      auto buffer = __managed_malloc(sizeof(void*) * 2);

      auto env = __managed_malloc(sizeof(void*));
      *reinterpret_cast<uintptr_t*>(env) = 0;

      *reinterpret_cast<void**>(buffer) = env;
      *(reinterpret_cast<uintptr_t*>(buffer) + 1) = 0;

      return Value(ty, buffer);
   }
   else if (auto ArrTy = dyn_cast<ArrayType>(ty)) {
      auto buffer = __managed_malloc(ArrTy->getElementType()->getMemberSize()
                           * ArrTy->getNumElements());

      return Value(ty, buffer);
   }
   else if (auto TupleTy = dyn_cast<TupleType>(ty)) {
      size_t size = 0;
      for (auto &cont : TupleTy->getContainedTypes())
         size += cont.second->getMemberSize();

      return Value(ty, __managed_malloc(size));
   }
   else if (auto Obj = dyn_cast<ObjectType>(ty)) {
      auto R = Obj->getRecord();

      if (auto S = dyn_cast<Struct>(R)) {
         size_t size = 0;
         for (auto &F : S->getFields()) {
            if (F.isIsStatic()) continue;
            size += F.getFieldType()->getMemberSize();
         }

         return Value(ty, __managed_malloc(size));
      }
      else if (auto U = dyn_cast<Union>(R)) {
         return Value(ty, __managed_malloc(U->getSize()));
      }
      else if (auto E = dyn_cast<Enum>(R)) {
         size_t size = E->getRawType()->getMemberSize();
         size += E->getMaxAssociatedValues() * sizeof(void*);

         return Value(ty, __managed_malloc(size));
      }
   }

   llvm_unreachable("bad value type");
}

void Value::doStore(Type *ty, Value dst) const
{
   if (ty->isIntegerTy()) {
      switch (ty->getBitwidth()) {
         case 1:
         case 8:
            *dst.getBuffer() = getU8();
            break;
         case 16:
            *reinterpret_cast<uint16_t*>(dst.getBuffer()) = getU16();
            break;
         case 32:
            *reinterpret_cast<uint32_t*>(dst.getBuffer()) = getU32();
            break;
         case 64:
            *reinterpret_cast<uint64_t*>(dst.getBuffer()) = getU64();
            break;
         default:
            llvm_unreachable("bad bitwidth");
      }
   }
   else if (ty->isFloatTy()) {
      *reinterpret_cast<float*>(dst.getBuffer()) = getFloat();
   }
   else if (ty->isDoubleTy()) {
      *reinterpret_cast<double*>(dst.getBuffer()) = getDouble();
   }
   else if (ty->isPointerTy() || ty->isRawFunctionTy() || ty->isClass()) {
      *reinterpret_cast<char**>(dst.getBuffer()) = buffer;
   }
   else {
      memcpy(dst.getBuffer(), buffer, ty->getSize());
   }
}

Value Value::getFunc(il::Function const *F)
{
   return Value(*F->getType(), F);
}

Value Value::getStruct(Type *ty, llvm::ArrayRef<Value> fieldValues)
{
   auto S = cast<Struct>(ty->getRecord());
   auto V = getNullValue(ty);
   auto ptr = V.getBuffer();

   size_t i = 0;
   for (auto &F : S->getFields()) {
      if (F.isIsStatic()) continue;

      fieldValues[i].doStore(*F.getFieldType(),
                             getPreallocated(*F.getFieldType(), ptr));

      ptr += F.getFieldType()->getMemberSize();
      i += 1;
   }

   return V;
}

Value Value::getArray(Type *ty, llvm::ArrayRef<Value> fieldValues)
{
   auto A = cast<ArrayType>(ty);
   auto V = getNullValue(ty);
   auto ptr = V.getBuffer();
   auto memberSize = A->getElementType()->getMemberSize();

   for (size_t i = 0; i < A->getNumElements(); ++i) {
      fieldValues[i].doStore(A->getElementType(),
                             getPreallocated(A->getElementType(), ptr));
      ptr += memberSize;
   }

   return V;
}

Value Value::getUnion(Type *unionTy, Type *initTy, Value Initializer)
{
   auto V = getNullValue(unionTy);

   Initializer.doStore(initTy, V);

   return V;
}

Value Value::getEnum(Type *ty, llvm::StringRef caseName,
                     llvm::ArrayRef<Value> fieldValues) {
   auto E = cast<Enum>(ty->getRecord());
   auto &C = E->getCase(caseName);

   auto V = getNullValue(ty);
   getInt(E->getRawType(), C.rawValue).doStore(E->getRawType(), V);

   auto buf = V.getBuffer() + E->getRawType()->getMemberSize();
   size_t i = 0;

   void **ptr = (void**)buf;
   for (auto &V : C.associatedValues) {
      auto alloc = (char*)__managed_malloc(V.getType()->getMemberSize());

      fieldValues[i].doStore(*V.getType(), getPreallocated(*V.getType(),
                                                           alloc));
      *ptr = alloc;

      ptr += 1;
      i += 1;
   }

   return V;
}

Value Value::getTuple(Type *ty, llvm::ArrayRef<Value> fieldValues)
{
   auto Tup = cast<TupleType>(ty);
   auto V = getNullValue(Tup);

   size_t i = 0;
   auto ptr = V.getBuffer();

   for (auto &cont : Tup->getContainedTypes()) {
      fieldValues[i].doStore(*cont.second, getPreallocated(*cont.second, ptr));
      ptr += cont.second->getMemberSize();
      i += 1;
   }

   return V;
}

Value Value::getLambda(il::Function const *F,
                       llvm::ArrayRef<std::pair<Type*, Value>> captures) {
   auto buffer = __managed_malloc(2 * sizeof(void*));
   auto env = (char*)__managed_malloc(captures.size() * sizeof(void*));

   *reinterpret_cast<il::Function const**>(buffer) = F;

   auto ptr = env;
   for (auto &capt : captures) {
      capt.second.doStore(capt.first, getPreallocated(capt.first, ptr));
      ptr += sizeof(void*);
   }

   auto envPtr = (char**)buffer + 1;
   *envPtr = env;

   return Value(*F->getType(), buffer);
}

Value Value::getStructElement(Type *type, llvm::StringRef fieldName)
{
   auto S = dyn_cast<Struct>(type->getRecord());
   auto ptr = buffer;
   Type *fieldType = nullptr;

   for (auto &F : S->getFields()) {
      if (F.isIsStatic()) continue;
      if (F.getFieldName() == fieldName) {
         fieldType = *F.getFieldType();
         break;
      }

      ptr += F.getFieldType()->getMemberSize();
   }

   assert(fieldType && "field does not exist on type");

   if (fieldType->needsStructReturn())
      return getPreallocated(fieldType, ptr);

   return getPreallocated(fieldType->getPointerTo(), ptr);
}

Value Value::getStructElement(Type *type, size_t idx)
{
   auto S = dyn_cast<Struct>(type->getRecord());
   auto ptr = buffer;
   Type *fieldType = nullptr;
   size_t i = 0;

   for (auto &F : S->getFields()) {
      if (F.isIsStatic()) continue;
      if (i == idx) {
         fieldType = *F.getFieldType();
         break;
      }

      ptr += F.getFieldType()->getMemberSize();
      ++i;
   }

   assert(fieldType && "field does not exist on type");

   if (fieldType->needsStructReturn())
      return getPreallocated(fieldType, ptr);

   return getPreallocated(fieldType->getPointerTo(), ptr);
}

Value Value::getElementPtr(Type *type, size_t idx)
{
   auto size = type->getPointeeType()->getMemberSize();
   auto ptr = buffer + size * idx;

   return getPreallocated(type, ptr);
}

Value Value::getArrayElement(Type *type, size_t idx)
{
   auto A = cast<ArrayType>(type);
   auto memberSize = A->getElementType()->getMemberSize();
   auto ty = A->getElementType();
   auto ptr = reinterpret_cast<Value*>(buffer + memberSize * idx);

   if (ty->needsStructReturn())
      return getPreallocated(ty, ptr);

   return getPreallocated(ty->getPointerTo(), ptr);
}

Value Value::getTupleElement(Type *type, size_t idx)
{
   auto T = cast<TupleType>(type);
   auto &Tys = T->getContainedTypes();

   size_t offset = 0;
   for (size_t i = 0; i < idx; ++i)
      offset += Tys[i].second->getMemberSize();

   auto ptr = buffer + offset;
   auto ty = *Tys[idx].second;

   if (ty->needsStructReturn())
      return getPreallocated(ty, ptr);

   return getPreallocated(ty->getPointerTo(), ptr);
}

il::Function* Value::getLambdaFuncPtr()
{
   return *reinterpret_cast<il::Function**>(buffer);
}

Value Value::getLambdaEnvironment()
{
   auto env = reinterpret_cast<char**>(buffer) + 1;
   return getPreallocated(IntegerType::getCharTy()->getPointerTo()
                                                  ->getPointerTo(), env);
}

Value Value::getEnumRawValue(Type *type)
{
   auto rawType = cast<Enum>(type->getRecord())->getRawType();
   auto bw = rawType->getBitwidth();
   uint64_t caseVal;

   switch (bw) {
      case 1:
      case 8:
         caseVal = *reinterpret_cast<uint8_t*>(buffer);
         break;
      case 16:
         caseVal = *reinterpret_cast<uint16_t*>(buffer);
         break;
      case 32:
         caseVal = *reinterpret_cast<uint32_t*>(buffer);
         break;
      case 64:
         caseVal = *reinterpret_cast<uint64_t*>(buffer);
         break;
      default:
         llvm_unreachable("bad bitwidth");
   }

   return getInt(rawType, caseVal);
}

Value Value::getEnumCaseValue(Type *type, llvm::StringRef caseName, size_t idx)
{
   auto E = cast<Enum>(type->getRecord());
   auto &C = E->getCase(caseName);

   assert(getEnumRawValue(type).getU64() == C.rawValue);
   assert(idx < C.associatedValues.size());

   void **ptr = (void**)buffer;
   ptr += idx + 1;

   auto ty = *C.associatedValues[idx].getType();
   return getPreallocated(ty->getPointerTo(),
                          *reinterpret_cast<Value**>(ptr));
}

std::string Value::getString() const
{
   return string(buffer);
}

} // namespace ctfe
} // namespace cdot