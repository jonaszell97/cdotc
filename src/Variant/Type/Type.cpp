//
// Created by Jonas Zell on 13.08.17.
//

#include "Type.h"

#include "AutoType.h"
#include "VoidType.h"
#include "MetaType.h"
#include "TypeGroup.h"
#include "ArrayType.h"
#include "IntegerType.h"
#include "PointerType.h"
#include "FunctionType.h"
#include "TupleType.h"
#include "GenericType.h"
#include "FPType.h"
#include "ObjectType.h"
#include "GenericType.h"

#include "../../AST/Passes/SemanticAnalysis/Record/Enum.h"
#include "../../AST/Passes/SemanticAnalysis/Record/Class.h"
#include "../../AST/Passes/SemanticAnalysis/Record/Union.h"

#include "../../Basic/CastKind.h"

using std::string;
using namespace cdot::support;

namespace cdot {

llvm::StringMap<Type*> Type::Instances;

Type::~Type()
{

}

Type* Type::get(BoxedPrimitive bp)
{
   switch (bp) {
      case Float:
         return FPType::getFloatTy();
      case Double:
         return FPType::getDoubleTy();
      case Bool:
         return IntegerType::getBoolTy();
      case Char:
         return IntegerType::getCharTy();
      default: {
         unsigned bw = bp;
         bool isUnsigned = false;
         if (bw > 100) {
            isUnsigned = true;
            bw -= 100;
         }

         return IntegerType::get(bw, isUnsigned);
      }
   }
}

void Type::TearDown()
{

}

const PointerType* Type::asPointerTy() const
{
   return cast<PointerType>(this);
}

const IntegerType* Type::asIntegerTy() const
{
   return cast<IntegerType>(this);
}

const FPType* Type::asFloatingTy() const
{
   return cast<FPType>(this);
}

const TupleType* Type::asTupleTy() const
{
   return cast<TupleType>(this);
}

const FunctionType* Type::asFunctionTy() const
{
   return cast<FunctionType>(this);
}

const ObjectType* Type::asObjTy() const
{
   return cast<ObjectType>(this);
}

const GenericType* Type::asGenericTy() const
{
   return cast<GenericType>(this);
}

PointerType* Type::getPointerTo()
{
   return PointerType::get(this);
}

PointerType* Type::asPointerTy()
{
   return cast<PointerType>(this);
}

IntegerType* Type::asIntegerTy()
{
   return cast<IntegerType>(this);
}

FPType* Type::asFloatingTy()
{
   return cast<FPType>(this);
}

TupleType* Type::asTupleTy()
{
   return cast<TupleType>(this);
}

FunctionType* Type::asFunctionTy()
{
   return cast<FunctionType>(this);
}

ObjectType* Type::asObjTy()
{
   return cast<ObjectType>(this);
}

GenericType* Type::asGenericTy()
{
   return cast<GenericType>(this);
}

bool Type::isAutoTy() const
{
   return isa<AutoType>(this);
}

bool Type::isVoidTy() const
{
   return isa<VoidType>(this);
}

bool Type::isTypeGroup() const
{
   return isa<TypeGroup>(this);
}

bool Type::isArrayTy() const
{
   return isa<ArrayType>(this);
}

bool Type::isMetaType() const
{
   return isa<MetaType>(this);
}

bool Type::isGenericTy() const
{
   return isa<GenericType>(this);
}

bool Type::isObjectTy() const
{
   return isa<ObjectType>(this);
}

bool Type::isTupleTy() const
{
   return isa<TupleType>(this);
}

bool Type::isFPType() const
{
   return isa<FPType>(this);
}

bool Type::isDummyObject() const
{
   return isa<InconcreteObjectType>(this);
}

bool Type::is(BoxedPrimitive primitive) const
{
   auto obj = dyn_cast<ObjectType>(this);
   return obj && obj->getPrimitiveKind() == primitive;
}

Type::BoxedPrimitive Type::getPrimitiveKind() const
{
   if (auto Int = dyn_cast<IntegerType>(this)) {
      if (Int->getBitwidth() == 1)
         return BoxedPrimitive::Bool;

      return (BoxedPrimitive)(Int->getBitwidth() + 100 * Int->isUnsigned());
   }
   else if (auto FPTy = dyn_cast<FPType>(this)) {
      return (BoxedPrimitive)(200 + FPTy->getPrecision());
   }
   else if (auto Obj = dyn_cast<ObjectType>(this)) {
      if (Obj->isRawEnum())
         return cast<Enum>(Obj->getRecord())->getRawType()->getPrimitiveKind();

      return Obj->getPrimitiveKind();
   }

   return BP_None;
}

bool Type::isFloatTy() const
{
   if (auto FP = dyn_cast<FPType>(this)) {
      return FP->getPrecision() == sizeof(float) * 8;
   }

   return false;
}

bool Type::isDoubleTy() const
{
   if (auto FP = dyn_cast<FPType>(this)) {
      return FP->getPrecision() == sizeof(double) * 8;
   }

   return false;
}

bool Type::isIntegerTy() const
{
   return (isa<IntegerType>(this));
}

bool Type::isPointerTy() const
{
   return isa<PointerType>(this);
}

bool Type::isFunctionTy() const
{
   return isa<FunctionType>(this);
}

bool Type::isRawFunctionTy() const
{
   if (auto fun = dyn_cast<FunctionType>(this)) {
      return fun->isRawFunctionTy();
   }

   return false;
}

bool Type::isStringRepresentable() const
{
   return true;
}

bool Type::isSelfComparable() const
{
   switch (id) {
      default:
         return true;
      case TypeID::ObjectTypeID:
         return getRecord()->getOperatorEquals() != nullptr;
   }
}

bool Type::isHashable() const
{
   return true;
}

bool Type::isDependantType() const
{
   switch (id) {
      case TypeID::GenericTypeID:
         return true;
      case TypeID::AutoTypeID:
      case TypeID::VoidTypeID:
      case TypeID::FPTypeGroupID:
      case TypeID::StringTypeGroupID:
      case TypeID::IntegerTypeGroupID:
      case TypeID::IntegerTypeID:
      case TypeID::FPTypeID:
      case TypeID::EnumTypeGroupID:
      case TypeID::LambdaTypeGroupID:
         return false;
      case TypeID::MetaTypeID:
         return cast<MetaType>(this)->getUnderlyingType()->isDependantType();
      case TypeID::PointerTypeID:
         return cast<PointerType>(this)->getPointeeType()->isDependantType();
      case TypeID::ArrayTypeID:
      case TypeID::InferredArrayTypeID:
         return cast<ArrayType>(this)->getElementType()->isDependantType();
      case TypeID::ObjectTypeID:
         return getTemplateArgs().isStillDependant();
      case TypeID::InconcreteObjectTypeID:
         return true;
      case TypeID::FunctionTypeID: {
         auto Func = cast<FunctionType>(this);
         if (Func->getReturnType()->isDependantType())
            return true;

         for (const auto &Arg : Func->getArgTypes())
            if (Arg.type->isDependantType()) return true;

         return false;
      }
      case TypeID::TupleTypeID:
         for (const auto &Ty : cast<TupleType>(this)->getContainedTypes())
            if (Ty.second->isDependantType()) return true;

         return false;
      default:
         llvm_unreachable("bad type kind!");
   }
}

QualType Type::getPointeeType() const
{
   assert(isa<PointerType>(this) && "not a pointer type");
   return cast<PointerType>(this)->getPointeeType();
}

Type::operator QualType()
{
   return QualType(this);
}

bool Type::needsCastTo(const Type *ty) const
{
   return this != ty;
}

bool Type::isBoxedPrimitive() const
{
   return isObjectTy() && !is(BP_None);
}

unsigned short Type::getAlignment() const
{
   switch (id) {
      case TypeID::IntegerTypeID:
         return cast<IntegerType>(this)->getAlignment();
      case TypeID::FPTypeID:
         return cast<FPType>(this)->getAlignment();
      case TypeID::ObjectTypeID:
      case TypeID::InconcreteObjectTypeID:
         return cast<ObjectType>(this)->getAlignment();
      case TypeID::FunctionTypeID:
         return cast<FunctionType>(this)->getAlignment();
      case TypeID::TupleTypeID:
         return cast<TupleType>(this)->getAlignment();
      case TypeID::ArrayTypeID:
         return cast<ArrayType>(this)->getAlignment();
      default:
         return sizeof(void*);
   }
}

bool Type::isRawEnum() const
{
   return isObjectTy() && cast<ObjectType>(this)->isRawEnum();
}

bool Type::isSelfTy() const
{
   if (auto Gen = dyn_cast<GenericType>(this))
      return Gen->getGenericTypeName() == "Self";

   return false;
}

Type *Type::getGroupDefault() const
{
   assert(isa<TypeGroup>(this) && "not a type group!");
   return cast<TypeGroup>(this)->getGroupDefault();
}

sema::TemplateArgList const& Type::getTemplateArgs() const
{
   assert(isObjectTy() && "not an object");
   if (auto Inconcrete = dyn_cast<InconcreteObjectType>(this))
      return Inconcrete->getTemplateArgs();

   return cast<ObjectType>(this)->getTemplateArgs();
}

bool Type::hasTemplateArgs() const
{
   if (!isObjectTy())
      return false;

   if (auto Inconcrete = dyn_cast<InconcreteObjectType>(this))
      return Inconcrete->hasTemplateArgs();

   return cast<ObjectType>(this)->hasTemplateArgs();
}

cl::Record* Type::getRecord() const
{
   if (auto Gen = dyn_cast<GenericType>(this)) {
      return Gen->getActualType()->getRecord();
   }

   assert(isObjectTy() && "not an object");
   return cast<ObjectType>(this)->getRecord();
}

llvm::StringRef Type::getClassName() const
{
   return getRecord()->getName();
}

bool Type::implicitlyCastableTo(Type const* to) const
{
   auto res = getCastKind(this, to);
   return res.getStrength() == CastResult::Implicit;
}

bool Type::explicitlyCastableTo(Type const* to) const
{
   auto res = getCastKind(this, to);
   return res.getStrength() != CastResult::Implicit
          && res.isValid();
}

Type* Type::box() const
{
   if (auto Int = dyn_cast<IntegerType>(this)) {
      return Int->box();
   }
   if (auto FP = dyn_cast<FPType>(this)) {
      return FP->box();
   }

   llvm_unreachable("not a primitive type");
}

Type* Type::unbox() const
{
   if (auto Obj = dyn_cast<ObjectType>(this)) {
      return Obj->unbox();
   }

   llvm_unreachable("not a boxed primitive type");
}

bool Type::hasDefaultValue() const
{
   switch (id) {
      case TypeID::IntegerTypeID:
      case TypeID::FPTypeID:
      case TypeID::IntegerTypeGroupID:
      case TypeID::FPTypeGroupID:
      case TypeID::PointerTypeID:
      case TypeID::ArrayTypeID:
         return true;
      case TypeID::ObjectTypeID:
      case TypeID::InconcreteObjectTypeID: {
         auto rec = getRecord();
         if (auto S = dyn_cast<Struct>(rec)) {
            return S->getParameterlessConstructor() != nullptr;
         }
         else if (auto U = dyn_cast<Union>(rec)) {
            for (const auto &F : U->getFields())
               if (F.second->hasDefaultValue())
                  return true;

            return false;
         }

         return false;
      }
      case TypeID::FunctionTypeID:
         return false;
      case TypeID::TupleTypeID: {
         for (const auto &ty : cast<TupleType>(this)->getContainedTypes()) {
            if (!ty.second->hasDefaultValue())
               return false;
         }

         return true;
      }
      default:
         return false;
   }
}

size_t Type::getSize() const
{
   switch (id) {
      case TypeID::IntegerTypeID:
         return cast<IntegerType>(this)->getSize();
      case TypeID::FPTypeID:
         return cast<FPType>(this)->getSize();
      case TypeID::ObjectTypeID:
      case TypeID::InconcreteObjectTypeID:
         return cast<ObjectType>(this)->getSize();
      case TypeID::FunctionTypeID:
         return cast<FunctionType>(this)->getSize();
      case TypeID::TupleTypeID:
         return cast<TupleType>(this)->getSize();
      case TypeID::ArrayTypeID:
         return cast<ArrayType>(this)->getSize();
      default:
         return size_t(getAlignment());
   }
}

size_t Type::getMemberSize() const
{
   if (auto Obj = dyn_cast<ObjectType>(this)) {
      if (getRecord()->isClass())
         return sizeof(void*);

      return Obj->getSize();
   }

   return getSize();
}

unsigned short Type::getMemberAlignment() const
{
   if (auto Obj = dyn_cast<ObjectType>(this)) {
      if (getRecord()->isClass())
         return sizeof(void*);

      return Obj->getAlignment();
   }

   return getAlignment();
}

bool Type::isBoxedEquivOf(Type const *ty) const
{
   if (auto Obj = dyn_cast<ObjectType>(this)) {
      return Obj->isBoxedEquivOf(ty);
   }

   return false;
}

bool Type::isUnsigned() const
{
   return isa<IntegerType>(this) && cast<IntegerType>(this)->isUnsigned();
}

unsigned int Type::getBitwidth() const
{
   if (auto Int = dyn_cast<IntegerType>(this)) {
      return Int->getBitwidth();
   }
   if (auto FP = dyn_cast<FPType>(this)) {
      return FP->getPrecision();
   }
   if (isRawEnum()) {
      return cast<Enum>(getRecord())->getRawType()->getBitwidth();
   }

   llvm_unreachable("not a primitive type");
}

bool Type::isEnum() const
{
   return isObjectTy() && getRecord()->isEnum();
}

bool Type::isUnion() const
{
   return isObjectTy() && getRecord()->isUnion();
}

bool Type::isProtocol() const
{
   return isObjectTy() && getRecord()->isProtocol();
}

bool Type::isClass() const
{
   return isObjectTy() && getRecord()->isClass();
}

bool Type::isStruct() const
{
   return isObjectTy() && getRecord()->isStruct();
}

bool Type::isOptionTy() const
{
   if (!isObjectTy())
      return false;

   auto Rec = getRecord();
   while (Rec->getSpecializedTemplate())
      Rec = Rec->getSpecializedTemplate();

   return Rec->getName() == "Option";
}

bool Type::isOptionOf(const string &) const
{
   return false;
}

bool Type::needsCleanup() const
{
   switch (id) {
      case TypeID::ObjectTypeID:
      case TypeID::TupleTypeID:
      case TypeID::ArrayTypeID:
         return true;
      case TypeID::FunctionTypeID:
         return !isRawFunctionTy();
      default:
         return false;
   }
}

bool Type::needsStructReturn() const
{
   switch (id) {
      case TypeID::ObjectTypeID: {
         auto rec = getRecord();
         switch (rec->getTypeID()) {
            case Record::EnumID:
               return !cast<Enum>(rec)->isRawEnum();
            case Record::StructID:
            case Record::UnionID:
            case Record::ProtocolID:
               return true;
            case Record::ClassID:
               return false;
         }
      }
      case TypeID::FunctionTypeID:
         return !isRawFunctionTy();
      case TypeID::TupleTypeID:
      case TypeID::ArrayTypeID:
         return true;
      default:
         return false;
   }
}

bool Type::needsMemCpy() const
{
   return needsStructReturn();
}

string Type::toString() const
{
   switch (id) {
      case TypeID::IntegerTypeID:
         return cast<IntegerType>(this)->toString();
      case TypeID::FPTypeID:
         return cast<FPType>(this)->toString();
      case TypeID::ObjectTypeID:
      case TypeID::InconcreteObjectTypeID:
         return cast<ObjectType>(this)->toString();
      case TypeID::FunctionTypeID:
         return cast<FunctionType>(this)->toString();
      case TypeID::TupleTypeID:
         return cast<TupleType>(this)->toString();
      case TypeID::ArrayTypeID:
      case TypeID::InferredArrayTypeID:
         return cast<ArrayType>(this)->toString();
      case TypeID::AutoTypeID:
         return "<inferred>";
      case TypeID::VoidTypeID:
         return "void";
      case TypeID::MetaTypeID:
         return cast<MetaType>(this)->toString();
      case TypeID::GenericTypeID:
         return cast<GenericType>(this)->toString();
      case TypeID::PointerTypeID:
         return cast<PointerType>(this)->toString();
#   define CDOT_TYPE_GROUP(Name) \
      case TypeID::Name##ID: return #Name;
#   include "Types.def"
      case TypeID::NamespaceTypeID:
         return "namespace";
      default:
         llvm_unreachable("bad type kind!");
   }
}

string Type::toUniqueString() const
{
   if (auto Gen = dyn_cast<GenericType>(this)) {
      return Gen->toUniqueString();
   }

   return toString();
}

} // namespace cdot