//
// Created by Jonas Zell on 13.08.17.
//

#include "Type.h"

#include "AST/ASTContext.h"
#include "AST/Statement/Declaration/Class/RecordDecl.h"
#include "AST/Statement/Declaration/Class/FieldDecl.h"
#include "AST/Statement/Declaration/TypedefDecl.h"
#include "AST/Statement/Declaration/NamespaceDecl.h"
#include "AST/Expression/TypeRef.h"

#include "Basic/CastKind.h"
#include "Support/Casting.h"
#include "Message/Diagnostics.h"

#include <llvm/ADT/StringSwitch.h>
#include <llvm/ADT/SmallString.h>
#include <sstream>

using std::string;
using namespace cdot::support;
using namespace cdot::ast;

namespace cdot {

Type::~Type()
{

}

/*
   bool Type::isAutoType() const
   {
      if (id == TypeID::TypedefTypeID)
         return asRealTypedefType()->getAliasedType()
                                                     .isNull();

      return AutoType::classof(this);
   }

   const AutoType* Type::asAutoType() const
   {
      if (id == TypeID::TypedefTypeID)
         return asRealTypedefType()->getAliasedType()
                                                     ->asAutoType();

      if (AutoType::classof(this))
         return static_cast<AutoType const*>(this);

      return nullptr;
   }

   AutoType* Type::asAutoType()
   {
      if (id == TypeID::TypedefTypeID)
         return static_cast<TypedefType*>(this)->getAliasedType()
                                               ->asAutoType();

      if (AutoType::classof(this))
         return static_cast<AutoType*>(this);

      return nullptr;
   }
*/

#define CDOT_TYPE(Name)                                              \
bool Type::is##Name() const                                          \
{                                                                    \
   if (getTypeID() == TypeID::TypedefTypeID)                         \
      return asRealTypedefType()->getAliasedType()->is##Name();      \
                                                                     \
   return Name::classof(this);                                       \
}                                                                    \
                                                                     \
const Name* Type::as##Name() const                                   \
{                                                                    \
   if (getTypeID() == TypeID::TypedefTypeID)                         \
      return asRealTypedefType()->getAliasedType()->as##Name();      \
                                                                     \
   if (Name::classof(this))                                          \
      return static_cast<Name const*>(this);                         \
                                                                     \
   return nullptr;                                                   \
}                                                                    \
                                                                     \
Name* Type::as##Name()                                               \
{                                                                    \
   if (getTypeID() == TypeID::TypedefTypeID)                         \
      return static_cast<TypedefType*>(this)->getAliasedType()       \
                                            ->as##Name();            \
                                                                     \
   if (Name::classof(this))                                          \
      return static_cast<Name*>(this);                               \
                                                                     \
   return nullptr;                                                   \
}

#define CDOT_BASE_TYPE(Name) CDOT_TYPE(Name)
#include "Types.def"

bool Type::isBoxedPrimitive(BoxedPrimitive primitive) const
{
   auto obj = this->asObjectType();
   return obj && obj->getPrimitiveKind() == primitive;
}

PointerType* Type::getPointerTo(cdot::ast::ASTContext &Ctx) const
{
   return Ctx.getPointerType(QualType(const_cast<Type*>(this)));
}

QualType QualType::getPointerTo(cdot::ast::ASTContext &Ctx) const
{
   return Ctx.getPointerType(*this);
}

Type::BoxedPrimitive Type::getPrimitiveKind() const
{
   if (auto Int = this->asIntegerType()) {
      if (Int->getBitwidth() == 1)
         return BoxedPrimitive::Bool;

      return (BoxedPrimitive)(Int->getBitwidth() + 100 * Int->isUnsigned());
   }
   else if (auto FPTy = this->asFPType()) {
      return (BoxedPrimitive)(200 + FPTy->getPrecision());
   }
   else if (auto Obj = this->asObjectType()) {
      if (Obj->isRawEnum())
         return cast<EnumDecl>(Obj->getRecord())->getRawType()->getType()
                                                ->getPrimitiveKind();

      return Obj->getPrimitiveKind();
   }

   return BP_None;
}

bool Type::isFloatTy() const
{
   if (auto FP = this->asFPType()) {
      return FP->getPrecision() == sizeof(float) * 8;
   }

   return false;
}

bool Type::isDoubleTy() const
{
   if (auto FP = this->asFPType()) {
      return FP->getPrecision() == sizeof(double) * 8;
   }

   return false;
}

bool Type::isRawFunctionTy() const
{
   if (auto fun = this->asFunctionType()) {
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
   switch (getTypeID()) {
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

bool Type::isDependentType() const
{
   switch (getTypeID()) {
      case TypeID::TypedefTypeID:
         return asRealTypedefType()->getAliasedType()->isDependentType();
      case TypeID::GenericTypeID:
         return true;
      case TypeID::AutoTypeID:
      case TypeID::VoidTypeID:
      case TypeID::IntegerTypeID:
      case TypeID::FPTypeID:
         return false;
      case TypeID::MetaTypeID:
         return this->asMetaType()->getUnderlyingType()->isDependentType();
      case TypeID::PointerTypeID:
         return this->asPointerType()->getPointeeType()->isDependentType();
      case TypeID::ArrayTypeID:
      case TypeID::InferredArrayTypeID:
         return this->asArrayType()->getElementType()->isDependentType();
      case TypeID::ObjectTypeID: {
         auto R = getRecord();
         if (R->isTemplate())
            return true;

         if (!R->isInstantiation())
            return false;

         return R->getTemplateArgs().isStillDependent();
      }
      case TypeID::InconcreteObjectTypeID:
         return true;
      case TypeID::FunctionTypeID: {
         auto Func = this->asFunctionType();
         if (Func->getReturnType()->isDependentType())
            return true;

         for (const auto &Arg : Func->getArgTypes())
            if (Arg->isDependentType()) return true;

         return false;
      }
      case TypeID::TupleTypeID:
         for (const auto &Ty : this->asTupleType()->getContainedTypes())
            if (Ty->isDependentType()) return true;

         return false;
      default:
         llvm_unreachable("bad type kind!");
   }
}

bool Type::isBoxedEquivOf(const Type *ty) const
{
   if (auto I = ty->asIntegerType()) {
      unsigned val = I->getBitwidth();
      auto PK = getPrimitiveKind();

      switch (val) {
         case 1:
            if (PK == Bool)
               return true;
            goto _default;
         case 8:
            if (PK == Char)
               return true;
            LLVM_FALLTHROUGH;
         default:
         _default:
            val += 100 * I->isUnsigned();
            return getPrimitiveKind() == val;
      }
   }

   if (auto FP = ty->asFPType()) {
      unsigned val = 200 + FP->getPrecision();
      return getPrimitiveKind() == val;
   }

   return false;
}

bool Type::isUnboxedEquivOf(const Type *ty) const
{
   return ty->isBoxedEquivOf(this);
}

QualType Type::getPointeeType() const
{
   assert(this->isPointerType() && "not a pointer type");
   return this->asPointerType()->getPointeeType();
}

Type::operator QualType()
{
   return QualType(this);
}

bool Type::isBoxedPrimitive() const
{
   return isObjectType() && !isBoxedPrimitive(BP_None);
}

unsigned short Type::getAlignment() const
{
   switch (getTypeID()) {
      case TypeID::TypedefTypeID:
         return asRealTypedefType()->getAliasedType()
                                                     ->getAlignment();
      case TypeID::IntegerTypeID:
         return this->asIntegerType()->getAlignment();
      case TypeID::FPTypeID:
         return this->asFPType()->getAlignment();
      case TypeID::ObjectTypeID:
      case TypeID::InconcreteObjectTypeID:
         return this->asObjectType()->getAlignment();
      case TypeID::FunctionTypeID:
         return this->asFunctionType()->getAlignment();
      case TypeID::TupleTypeID:
         return this->asTupleType()->getAlignment();
      case TypeID::ArrayTypeID:
         return this->asArrayType()->getAlignment();
      default:
         return sizeof(void*);
   }
}

bool Type::isRawEnum() const
{
   return isObjectType() && this->asObjectType()->isRawEnum();
}

bool Type::isSelfTy() const
{
   if (auto Gen = this->asGenericType())
      return Gen->getGenericTypeName() == "Self";

   return false;
}

bool Type::isRealTypedefType() const
{
   return getTypeID() == TypeID::TypedefTypeID;
}

TypedefType* Type::asRealTypedefType()
{
   if (!isRealTypedefType())
      return nullptr;

   return static_cast<TypedefType*>(this);
}

TypedefType const* Type::asRealTypedefType() const
{
   if (!isRealTypedefType())
      return nullptr;

   return static_cast<TypedefType const*>(this);
}

Type* Type::getCanonicalType(ast::ASTContext &Ctx)
{
   switch (getTypeID()) {
      case TypeID::TypedefTypeID:
         return asRealTypedefType()->getAliasedType()->getCanonicalType(Ctx);
      case TypeID::GenericTypeID: {
         auto G = asGenericType();
         return Ctx.getTemplateArgType(G->getActualType()->getCanonicalType(Ctx),
                                       G->getGenericTypeName());

      }
      case TypeID::PointerTypeID:
         return getPointeeType()->getCanonicalType(Ctx)->getPointerTo(Ctx);
      case TypeID::TupleTypeID: {
         std::vector<QualType> vec;
         for (auto &el : asTupleType()->getContainedTypes())
            vec.push_back(el.getCanonicalType(Ctx));

         return Ctx.getTupleType(vec);
      }
      case TypeID::FunctionTypeID: {
         auto fn = asFunctionType();
         auto ret = fn->getReturnType().getCanonicalType(Ctx);

         std::vector<QualType> args;
         for (auto &arg : fn->getArgTypes())
            args.push_back(arg.getCanonicalType(Ctx));

         return Ctx.getFunctionType(ret, move(args), fn->getRawFlags());
      }
      case TypeID::ArrayTypeID: {
         auto arr = asArrayType();
         return Ctx.getArrayType(arr->getElementType()->getCanonicalType(Ctx),
                                 arr->getNumElements());
      }
      default:
         return this;
   }
}

Type* Type::getCanonicalType(ast::ASTContext &Ctx) const
{
   return const_cast<Type*>(this)->getCanonicalType(Ctx);
}

sema::TemplateArgList const& Type::getTemplateArgs() const
{
   assert(isObjectType() && "not an object");
   if (auto Inconcrete = this->asInconcreteObjectType())
      return Inconcrete->getTemplateArgs();

   return this->asObjectType()->getTemplateArgs();
}

bool Type::hasTemplateArgs() const
{
   if (!isObjectType())
      return false;

   if (auto Inconcrete = this->asInconcreteObjectType())
      return Inconcrete->hasTemplateArgs();

   return this->asObjectType()->hasTemplateArgs();
}

ast::RecordDecl *Type::getRecord() const
{
   if (auto Gen = this->asGenericType()) {
      return Gen->getActualType()->getRecord();
   }

   assert(isObjectType() && "not an object");
   return this->asObjectType()->getRecord();
}

llvm::StringRef Type::getClassName() const
{
   return getRecord()->getName();
}

size_t Type::getSize() const
{
   switch (getTypeID()) {
      case TypeID::TypedefTypeID:
         return asRealTypedefType()->getAliasedType()->getSize();
      case TypeID::IntegerTypeID:
         return this->asIntegerType()->getSize();
      case TypeID::FPTypeID:
         return this->asFPType()->getSize();
      case TypeID::ObjectTypeID:
      case TypeID::InconcreteObjectTypeID:
         return this->asObjectType()->getSize();
      case TypeID::FunctionTypeID:
         return this->asFunctionType()->getSize();
      case TypeID::TupleTypeID:
         return this->asTupleType()->getSize();
      case TypeID::ArrayTypeID:
         return this->asArrayType()->getSize();
      default:
         return size_t(getAlignment());
   }
}

size_t Type::getMemberSize() const
{
   if (auto Obj = this->asObjectType()) {
      if (getRecord()->isClass())
         return sizeof(void*);

      return Obj->getSize();
   }

   return getSize();
}

unsigned short Type::getMemberAlignment() const
{
   if (auto Obj = this->asObjectType()) {
      if (getRecord()->isClass())
         return sizeof(void*);

      return Obj->getAlignment();
   }

   return getAlignment();
}

bool Type::isUnsigned() const
{
   return this->isIntegerType() && this->asIntegerType()->isUnsigned();
}

unsigned int Type::getBitwidth() const
{
   if (auto Int = this->asIntegerType()) {
      return Int->getBitwidth();
   }
   if (auto FP = this->asFPType()) {
      return FP->getPrecision();
   }
   if (isRawEnum()) {
      return cast<EnumDecl>(getRecord())->getRawType()->getType()
                                        ->getBitwidth();
   }

   llvm_unreachable("not a primitive type");
}

bool Type::isEnum() const
{
   return isObjectType() && getRecord()->isEnum();
}

bool Type::isUnion() const
{
   return isObjectType() && getRecord()->isUnion();
}

bool Type::isProtocol() const
{
   return isObjectType() && getRecord()->isProtocol();
}

bool Type::isClass() const
{
   return isObjectType() && getRecord()->isClass();
}

bool Type::isStruct() const
{
   return isObjectType() && getRecord()->isStruct();
}

bool Type::isOptionTy() const
{
   if (!isObjectType())
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
   switch (getTypeID()) {
      case TypeID::TypedefTypeID:
         return asRealTypedefType()->getAliasedType()->needsCleanup();
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
   switch (getTypeID()) {
      case TypeID::TypedefTypeID:
         return asRealTypedefType()->getAliasedType()->needsStructReturn();
      case TypeID::ObjectTypeID: {
         auto rec = getRecord();
         switch (rec->getTypeID()) {
            case AstNode::EnumDeclID:
               return !cast<EnumDecl>(rec)->isRawEnum();
            case AstNode::StructDeclID:
            case AstNode::UnionDeclID:
            case AstNode::ProtocolDeclID:
               return true;
            case AstNode::ClassDeclID:
               return false;
            default:
               llvm_unreachable("bad record kind!");
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
   switch (getTypeID()) {
      case TypeID::TypedefTypeID:
         return asRealTypedefType()->getAliasName();
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
      case TypeID::UnknownAnyTypeID:
         return "<unknown_any>";
      case TypeID::VoidTypeID:
         return "void";
      case TypeID::MetaTypeID: {
         llvm::SmallString<64> str;
         str += "MetaType[";
         str += cast<MetaType>(this)->getUnderlyingType()->toString();
         str += "]";

         return str.str();
      }
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
   if (isRealTypedefType()) {
      auto td = asRealTypedefType();
      auto aliasedTy = td->getAliasedType()->toString();

      llvm::SmallString<64> str;
      str += td->getAliasName();
      str += " (aka '";
      str += aliasedTy;
      str += "')";

      return str.str();
   }

   if (auto Gen = this->asGenericType()) {
      return Gen->toUniqueString();
   }

   if (auto Obj = asObjectType())
      return Obj->getRecord()->getFullName();

//   if (isRawEnum()) {
//      auto E = cast<EnumDecl>(getRecord());
//
//      string str;
//
//      str += toString();
//
//      if (auto ty = E->getRawType()) {
//         str += '(';
//         str += ty->toString();
//         str += ')';
//      }
//
//      return str;
//   }

   return toString();
}

string ArrayType::toString() const
{
   return "<dependent sized array>";
}

std::string FunctionType::toString() const
{
   std::ostringstream res;
   res << "(";

   size_t i = 0;
   for (const auto& arg : argTypes) {
      if (arg.isLvalue())
         res << "ref ";

      res << arg.toString();

      if (i < argTypes.size() - 1) {
         res << ", ";
      }

      ++i;
   }

   res << ") -> " << returnType->toString();
   return res.str();
}

std::string TupleType::toString() const
{
   string str = "(";
   size_t i = 0;

   for (const auto& ty : containedTypes) {
      str += ty.toString();
      if (i < containedTypes.size() - 1) {
         str += ", ";
      }

      ++i;
   }

   return str + ")";
}

const sema::TemplateArgList& ObjectType::getTemplateArgs() const
{
   return Rec->getTemplateArgs();
}

bool ObjectType::hasTemplateArgs() const
{
   return !getTemplateArgs().empty();
}

bool ObjectType::isRawEnum() const
{
   auto rec = getRecord();
   return rec && rec->isRawEnum();
}

bool ObjectType::isBoxedEquivOf(Type const* other) const
{
   if (isBoxedPrimitive(BP_None))
      return false;

   auto primitiveType = RecordBits.primitiveType;

   switch (other->getTypeID()) {
      case TypeID::FPTypeID:
         return primitiveType == (other->isFloatTy() ? Float : Double);
      case TypeID::IntegerTypeID: {
         if (other->getBitwidth() == 8 && primitiveType == Char)
            return true;

         if (other->getBitwidth() == 1 && primitiveType == Bool)
            return true;

         unsigned bw = primitiveType;
         if (bw > 200)
            return false;

         bool isUnsigned = false;
         if (bw > 100) {
            isUnsigned = true;
            bw -= 100;
         }

         return other->isIntNTy(bw, isUnsigned);
      }
      default:
         return false;
   }
}

unsigned short ObjectType::getAlignment() const
{
   return Rec->getAlignment();
}

size_t ObjectType::getSize() const
{
   return Rec->getSize();
}

string ObjectType::toString() const
{
   return Rec->getFullName();
}

void InconcreteObjectType::Profile(llvm::FoldingSetNodeID &ID,
                                   cdot::ast::RecordDecl *R,
                                   cdot::sema::TemplateArgList *templateArgs) {
   ID.AddPointer(R);
   ID.AddInteger(templateArgs->size());

   for (auto &arg : *templateArgs)
      arg.Profile(ID);
}

llvm::StringRef NamespaceType::getNamespaceName() const
{
   return NS->getName();
}

QualType TypedefType::getAliasedType() const
{
   return td->getOriginTy()->getType();
}

llvm::StringRef TypedefType::getAliasName() const
{
   return td->getName();
}

} // namespace cdot