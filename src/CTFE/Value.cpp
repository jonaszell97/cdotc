//
// Created by Jonas Zell on 02.01.18.
//

#include <sstream>
#include <iomanip>

#include "Value.h"

#include "../IL/Value/Function/Function.h"

#include "../Variant/Type/Type.h"

#include "../AST/Statement/Declaration/Class/RecordDecl.h"
#include "../AST/Statement/Declaration/Class/FieldDecl.h"
#include "../AST/Statement/Declaration/Class/EnumCaseDecl.h"
#include "../AST/Statement/Declaration/LocalVarDecl.h"
#include "../AST/Expression/TypeRef.h"

using std::string;
using namespace cdot::ast;

namespace cdot {
namespace ctfe {

void Value::destroyValue(Type *type)
{
   llvm::outs() << "freeing " << (void*)buffer << "\n";
   if (isa<FunctionType>(type)) {
      assert(!type->isRawFunctionTy());

      free(*reinterpret_cast<void**>(buffer));
      free(buffer);
   }
   else if (type->isPointerType()) {
//      free(buffer);
   }
   else if (type->isArrayType()) {
      free(buffer);
   }
   else if (type->isTupleType()) {
      free(buffer);
   }
   else if (type->isObjectType()) {
      if (!type->isStruct() && !type->isEnum())
         free(buffer);
   }
   else {
      llvm_unreachable("bad owning value kind");
   }
}

string Value::toString(Type *type)
{
   if (type->isIntegerType()) {
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
   else if (type->isPointerType()) {
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
   else if (type->isFunctionType()) {
      std::ostringstream s;
      s << "Lambda(" << std::setbase(16) << buffer << std::setbase(10)
        << ", "
        << reinterpret_cast<il::Function*>(
           reinterpret_cast<uintptr_t *>(buffer) + 1)->getName().str()
        << ")";

      return s.str();
   }
   else if (auto ArrTy = type->asArrayType()) {
      string s = "[";
      auto elementTy = ArrTy->getElementType();
      bool sret = elementTy->needsStructReturn();

      for (size_t i = 0; i < ArrTy->getNumElements(); ++i) {
         if (i != 0) { s += ", "; }
         auto V = getArrayElement(type, i);
         if (sret) {
            s += V.toString(*elementTy);
         }
         else {
            s += V.getValuePtr()->toString(*elementTy);
         }
      }

      s += "]";
      return s;
   }
   else if (auto TupleTy = type->asTupleType()) {
      string s = "(";
      char *ptr = buffer;
      auto elements = TupleTy->getContainedTypes();

      for (size_t i = 0; i < elements.size(); ++i) {
         if (i != 0) { s += ", "; }

         auto V = getTupleElement(type, i);
         auto ty = *elements[i];

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
   else if (auto Obj = type->asObjectType()) {
      auto R = Obj->getRecord();
      if (auto S = dyn_cast<StructDecl>(R)) {
         string s = "{ ";
         char *ptr = buffer;
         size_t i = 0;

         for (auto &F : S->getFields()) {
            if (i != 0) { s += ", "; }

            s += F->getName();
            s += ": ";

            auto V = getStructElement(type, i);
            if (F->getType()->getType()->needsStructReturn()) {
               s += V.toString(*F->getType()->getType());
            }
            else {
               s += V.getValuePtr()->toString(*F->getType()->getType());
            }

            ptr += F->getType()->getType()->getMemberSize();
            ++i;
         }

         s += " }";
         return s;
      }
      else if (auto U = dyn_cast<UnionDecl>(R)) {
         string s = "{ ";
         size_t i = 0;

         for (auto &decl : U->getDecls()) {
            if (auto F = dyn_cast<FieldDecl>(decl)) {
               if (i != 0) { s += ", "; }

               s += F->getName();
               s += ": ";
               s += getPreallocated(buffer).toString(*F->getType()->getType());

               ++i;
            }
         }

         s += " }";
         return s;
      }
      else if (auto E = dyn_cast<EnumDecl>(R)) {
         uint64_t caseVal = getEnumRawValue(type).getU64();

         string s;
         for (auto &decl : E->getDecls()) {
            if (auto C = dyn_cast<EnumCaseDecl>(decl)) {
               if (C->getRawValue() != caseVal)
                  continue;

               if (!C->getArgs().empty()) {
                  size_t i = 0;
                  s += "(";

                  for (auto &V : C->getArgs()) {
                     auto val = getEnumCaseValue(type, C->getName(), i);
                     if (V->getArgType()->getType()->needsStructReturn()) {
                        s += val.toString(*V->getArgType()->getType());
                     }
                     else {
                        s += val.getValuePtr()
                                ->toString(*V->getArgType()->getType());
                     }

                     i++;
                  }

                  s += ")";
               }

               break;
            }
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
   if (type->isVoidType())
      return {};

   if (type->isIntegerType()) {
      llvm::APInt Int(type->getBitwidth(), u64, type->isUnsigned());
      return Variant(std::move(Int));
   }
   else if (type->isFloatTy()) {
      return Variant(f);
   }
   else if (type->isDoubleTy()) {
      return Variant(d);
   }
   else if (type->isPointerType()) {
      if (type->getPointeeType()->isInt8Ty()) {
         return Variant(string(buffer));
      }

      llvm::APInt Int(sizeof(void*) * 8,
                      reinterpret_cast<unsigned long long>(buffer));

      return Variant(std::move(Int));
   }
   else if (type->isRawFunctionTy()) {
      return Variant(F->getName().str());
   }
   else if (type->isFunctionType()) {
      llvm_unreachable("todo");
   }
   else if (auto ArrTy = type->asArrayType()) {
      std::vector<Variant> vec;
      auto elementTy = ArrTy->getElementType();
      bool sret = elementTy->needsStructReturn();

      for (size_t i = 0; i < ArrTy->getNumElements(); ++i) {
         auto V = getArrayElement(type, i);
         if (sret) {
            vec.push_back(V.toVariant(*elementTy));
         }
         else {
            vec.push_back(V.getValuePtr()->toVariant(*elementTy));
         }
      }

      return Variant(VariantType::Array, std::move(vec));
   }
   else if (auto TupleTy = type->asTupleType()) {
      std::vector<Variant> vec;
      auto elements = TupleTy->getContainedTypes();

      for (size_t i = 0; i < elements.size(); ++i) {
         auto V = getTupleElement(type, i);
         auto ty = *elements[i];

         if (ty->needsStructReturn()) {
            vec.push_back(V.toVariant(ty));
         }
         else {
            vec.push_back(V.getValuePtr()->toVariant(ty));
         }
      }

      return Variant(VariantType::Struct, std::move(vec));
   }
   else if (auto Obj = type->asObjectType()) {
      auto R = Obj->getRecord();
      if (auto S = dyn_cast<StructDecl>(R)) {
         std::vector<Variant> vec;
         size_t i = 0;

         for (auto &F : S->getFields()) {
            auto V = getStructElement(type, i);
            if (F->getType()->getType()->needsStructReturn()) {
               vec.emplace_back(V.toVariant(*F->getType()->getType()));
            }
            else {
               vec.emplace_back(V.getValuePtr()
                                 ->toVariant(*F->getType()->getType()));
            }

            ++i;
         }

         return Variant(VariantType::Struct, std::move(vec));
      }
      else if (auto U = dyn_cast<UnionDecl>(R)) {
         std::vector<Variant> vec;
         size_t i = 0;

         U->forEach<FieldDecl>([this, &vec, &i](FieldDecl *F) {
            if (F->isStatic())
               return;

            vec.emplace_back(toVariant(*F->getType()->getType()));
            ++i;
         });

         return Variant(VariantType::Struct, std::move(vec));
      }
      else if (auto E = dyn_cast<EnumDecl>(R)) {
         uint64_t caseVal = getEnumRawValue(type).getU64();

         std::vector<Variant> vec;
         E->forEach<EnumCaseDecl>([this, &vec, &caseVal,&type](EnumCaseDecl *C){
            if (C->getRawValue() != caseVal)
               return;

            if (!C->getArgs().empty()) {
               size_t i = 0;
               for (auto &V : C->getArgs()) {
                  auto val = getEnumCaseValue(type, C->getName(), i);
                  if (V->getArgType()->getType()->needsStructReturn()) {
                     vec.push_back(val.toVariant(*V->getArgType()->getType()));
                  }
                  else {
                     vec.push_back(val.getValuePtr()
                                      ->toVariant(*V->getArgType()->getType()));
                  }

                  i++;
               }
            }
         });

         return Variant(VariantType::Struct, std::move(vec));
      }
   }

   llvm_unreachable("bad value type");
}

Value Value::getNullValue(Type *ty, Allocator &Alloc)
{
   if (ty->isIntegerType()) {
      return getInt(0);
   }
   else if (ty->isFloatTy()) {
      return getFloat(0.0f);
   }
   else if (ty->isDoubleTy()) {
      return getDouble(0.0);
   }
   else if (ty->isPointerType() || ty->isRawFunctionTy()) {
      return getConstPtr(nullptr);
   }
   else if (ty->isFunctionType()) {
      auto buffer = Alloc.Allocate(sizeof(void*) * 2, alignof(void*));

      auto env = Alloc.Allocate(sizeof(void*), alignof(void*));
      *reinterpret_cast<uintptr_t*>(env) = 0;

      *reinterpret_cast<void**>(buffer) = env;
      *(reinterpret_cast<uintptr_t*>(buffer) + 1) = 0;

      return Value(buffer);
   }
   else if (auto ArrTy = ty->asArrayType()) {
      auto buffer = Alloc.Allocate(
         ArrTy->getElementType()->getMemberSize() * ArrTy->getNumElements(),
         ArrTy->getElementType()->getAlignment());

      return Value(buffer);
   }
   else if (auto TupleTy = ty->asTupleType()) {
      size_t size = 0;
      for (auto &cont : TupleTy->getContainedTypes())
         size += cont->getMemberSize();

      return Value(Alloc.Allocate(size, 1));
   }
   else if (auto Obj = ty->asObjectType()) {
      auto R = Obj->getRecord();

      if (auto S = dyn_cast<StructDecl>(R)) {
         size_t size = 0;
         for (auto &F : S->getFields()) {
            size += F->getType()->getType()->getMemberSize();
         }

         return Value(Alloc.Allocate(size, 1));
      }
      else if (auto U = dyn_cast<UnionDecl>(R)) {
         return Value(Alloc.Allocate(U->getSize(), 1));
      }
      else if (auto E = dyn_cast<EnumDecl>(R)) {
         size_t size = E->getRawType()->getType()->getMemberSize();
         size += E->getMaxAssociatedTypes() * sizeof(void*);

         return Value(Alloc.Allocate(size, 1));
      }
   }

   llvm_unreachable("bad value type");
}

void Value::doStore(Type *ty, Value dst) const
{
   if (ty->isIntegerType()) {
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
   else if (ty->isPointerType() || ty->isRawFunctionTy() || ty->isClass()) {
      *reinterpret_cast<char**>(dst.getBuffer()) = buffer;
   }
   else {
      memcpy(dst.getBuffer(), buffer, ty->getSize());
   }
}

Value Value::getFunc(il::Function const *F)
{
   return Value(F);
}

Value Value::getStruct(Type *ty, llvm::ArrayRef<Value> fieldValues,
                       Allocator &Alloc) {
   auto S = cast<StructDecl>(ty->getRecord());
   auto V = getNullValue(ty, Alloc);
   auto ptr = V.getBuffer();

   size_t i = 0;
   for (auto &F : S->getFields()) {
      fieldValues[i].doStore(*F->getType()->getType(), getPreallocated(ptr));
      ptr += F->getType()->getType()->getMemberSize();
      i++;
   }

   return V;
}

Value Value::getArray(Type *ty, llvm::ArrayRef<Value> fieldValues,
                      Allocator &Alloc) {
   auto A = ty->asArrayType();
   auto V = getNullValue(ty, Alloc);
   auto ptr = V.getBuffer();
   auto memberSize = A->getElementType()->getMemberSize();

   for (size_t i = 0; i < A->getNumElements(); ++i) {
      fieldValues[i].doStore(*A->getElementType(), getPreallocated(ptr));
      ptr += memberSize;
   }

   return V;
}

Value Value::getUnion(Type *unionTy, Type *initTy, Value Initializer,
                      Allocator &Alloc) {
   auto V = getNullValue(unionTy, Alloc);
   Initializer.doStore(initTy, V);

   return V;
}

Value Value::getEnum(Type *ty, llvm::StringRef caseName,
                     llvm::ArrayRef<Value> fieldValues,
                     Allocator &Alloc) {
   auto E = cast<EnumDecl>(ty->getRecord());
   auto C = E->hasCase(caseName);

   auto V = getNullValue(ty, Alloc);
   getInt(C->getRawValue()).doStore(*E->getRawType()->getType(), V);

   auto buf = V.getBuffer() + E->getRawType()->getType()->getMemberSize();
   size_t i = 0;

   void **ptr = (void**)buf;
   for (auto &V : C->getArgs()) {
      auto argTy = *V->getArgType()->getType();

      auto alloc = (char*)Alloc.Allocate(argTy->getMemberSize(), alignof(char));

      fieldValues[i].doStore(argTy, getPreallocated(alloc));
      *ptr = alloc;

      ptr += 1;
      i += 1;
   }

   return V;
}

Value Value::getTuple(Type *ty, llvm::ArrayRef<Value> fieldValues,
                      Allocator &Alloc) {
   auto Tup = ty->asTupleType();
   auto V = getNullValue(Tup, Alloc);

   size_t i = 0;
   auto ptr = V.getBuffer();

   for (auto &cont : Tup->getContainedTypes()) {
      fieldValues[i].doStore(*cont, getPreallocated(ptr));
      ptr += cont->getMemberSize();
      i += 1;
   }

   return V;
}

Value Value::getLambda(il::Function const *F,
                       llvm::ArrayRef<std::pair<Type*, Value>> captures,
                       Allocator &Alloc) {
   auto buffer = Alloc.Allocate(2 * sizeof(void*), alignof(void*));
   auto env = (char*)Alloc.Allocate(captures.size() * sizeof(void*), 1);

   *reinterpret_cast<il::Function const**>(buffer) = F;

   auto ptr = env;
   for (auto &capt : captures) {
      capt.second.doStore(capt.first, getPreallocated(ptr));
      ptr += sizeof(void*);
   }

   auto envPtr = (char**)buffer + 1;
   *envPtr = env;

   return Value(buffer);
}

Value Value::getStructElement(Type *type, llvm::StringRef fieldName)
{
   auto S = dyn_cast<StructDecl>(type->getRecord());
   auto ptr = buffer;
   Type *fieldType = nullptr;

   for (auto F : S->getFields()) {
      if (F->getName() == fieldName) {
         fieldType = *F->getType()->getType();
         break;
      }

      ptr += F->getType()->getType()->getMemberSize();
   }

   assert(fieldType && "field does not exist on type");

   return getPreallocated(ptr);
}

Value Value::getStructElement(Type *type, size_t idx)
{
   auto S = dyn_cast<StructDecl>(type->getRecord());
   auto ptr = buffer;
   Type *fieldType = nullptr;
   size_t i = 0;

   for (auto &F : S->getFields()) {
      if (F->isStatic()) continue;
      if (i == idx) {
         fieldType = *F->getType()->getType();
         break;
      }

      ptr += F->getType()->getType()->getMemberSize();
      ++i;
   }

   assert(fieldType && "field does not exist on type");

   return getPreallocated(ptr);
}

Value Value::getElementPtr(Type *type, size_t idx)
{
   auto size = type->getPointeeType()->getMemberSize();
   auto ptr = buffer + size * idx;

   return getPreallocated(ptr);
}

Value Value::getArrayElement(Type *type, size_t idx)
{
   auto A = type->asArrayType();
   auto memberSize = A->getElementType()->getMemberSize();
   auto ty = A->getElementType();
   auto ptr = reinterpret_cast<Value*>(buffer + memberSize * idx);

   if (ty->needsStructReturn())
      return getPreallocated(ptr);

   return getPreallocated(ptr);
}

Value Value::getTupleElement(Type *type, size_t idx)
{
   auto T = type->asTupleType();
   auto Tys = T->getContainedTypes();

   size_t offset = 0;
   for (size_t i = 0; i < idx; ++i)
      offset += Tys[i]->getMemberSize();

   auto ptr = buffer + offset;
   return getPreallocated(ptr);
}

il::Function* Value::getLambdaFuncPtr()
{
   return *reinterpret_cast<il::Function**>(buffer);
}

Value Value::getLambdaEnvironment()
{
   auto env = reinterpret_cast<char**>(buffer) + 1;
   return getPreallocated(env);
}

Value Value::getEnumRawValue(Type *type)
{
   auto rawType = *cast<EnumDecl>(type->getRecord())->getRawType()->getType();
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

   return getInt(caseVal);
}

Value Value::getEnumCaseValue(Type *type, llvm::StringRef caseName, size_t idx)
{
   auto E = cast<EnumDecl>(type->getRecord());
   auto C = E->hasCase(caseName);

   assert(getEnumRawValue(type).getU64() == C->getRawValue());
   assert(idx < C->getArgs().size());

   void **ptr = (void**)buffer;
   ptr += idx + 1;

   return getPreallocated(*reinterpret_cast<Value**>(ptr));
}

std::string Value::getString() const
{
   return string(buffer);
}

} // namespace ctfe
} // namespace cdot