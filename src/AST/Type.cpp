#include "cdotc/AST/Type.h"

#include "cdotc/AST/ASTContext.h"
#include "cdotc/AST/Decl.h"
#include "cdotc/AST/TypeVisitor.h"
#include "cdotc/IL/Constant.h"
#include "cdotc/Diagnostics/Diagnostics.h"
#include "cdotc/Support/Casting.h"
#include "cdotc/Support/StringSwitch.h"

using std::string;

using namespace cdot;
using namespace cdot::ast;
using namespace cdot::support;

namespace cdot {

bool TypeProperties::isDependent() const
{
   static constexpr uint16_t DependentMask
       = ContainsDependentSizeArrayType | ContainsUnknownAny;

   return (Props & DependentMask) != 0;
}

bool TypeProperties::containsTemplateParamType() const
{
   return (Props & ContainsTemplateParamType) != 0;
}

bool TypeProperties::containsAssociatedType() const
{
   return (Props & ContainsAssociatedType) != 0;
}

bool TypeProperties::containsTemplate() const
{
   llvm_unreachable("remove this!");
   return (Props & ContainsTemplate) != 0;
}

bool TypeProperties::containsUnexpandedParameterPack() const
{
   return (Props & ContainsUnexpandedParameterPack) != 0;
}

bool TypeProperties::containsProtocolWithAssociatedTypes() const
{
   llvm_unreachable("remove this!");
   return (Props & ContainsProtocolWithAssociatedTypes) != 0;
}

bool TypeProperties::containsTypeVariable() const
{
   return (Props & ContainsTypeVariable) != 0;
}

CanType::CanType(Type* T) : QualType(T) { assert(!T || T->isCanonical()); }

CanType::CanType(QualType T) : QualType(T) { assert(!T || T->isCanonical()); }

CanType::CanType(void* Ptr) : QualType(Ptr, FromOpaque::Placeholder) {}

CanType CanType::getFromOpaquePtr(void* Ptr)
{
   return CanType(QualType::getFromOpaquePtr(Ptr));
}

CanType CanType::getFromOpaquePtrUnchecked(void* Ptr) { return CanType(Ptr); }

Type::~Type() {}

#ifndef NDEBUG

void Type::verifyID(TypeID ID)
{
   switch (ID) {
#define CDOT_TYPE(NAME, PARENT)                                                \
   case NAME##ID:                                                              \
      break;
#include "cdotc/AST/Types.def"

   default:
      llvm_unreachable("bad type kind");
   }
}

#endif

#define CDOT_TYPE(Name, Parent)                                                \
   bool Type::is##Name() const { return this->isa<Name>(); }                   \
                                                                               \
   Name* Type::as##Name() const { return const_cast<Name*>(getAs<Name>()); }   \
                                                                               \
   Name* Type::as##Name() { return const_cast<Name*>(getAs<Name>()); }         \
                                                                               \
   Name* Type::uncheckedAs##Name() const                                       \
   {                                                                           \
      assert(Name::classof(CanonicalType) && "incompatible type!");            \
      return static_cast<Name*>(CanonicalType);                                \
   }

#define CDOT_BASE_TYPE(Name) CDOT_TYPE(Name, "")
#include "cdotc/AST/Types.def"

Type::child_iterator Type::child_begin() const
{
   switch (getTypeID()) {
#define CDOT_TYPE(SubClass, Parent)                                            \
   case TypeID::SubClass##ID:                                                  \
      static_assert(&Type::child_begin != &SubClass::child_begin,              \
                    #SubClass " does not implement child_begin!");             \
      return static_cast<const SubClass*>(this)->child_begin();
#include "cdotc/AST/Types.def"
   }
}

Type::child_iterator Type::child_end() const
{
   switch (getTypeID()) {
#define CDOT_TYPE(SubClass, Parent)                                            \
   case TypeID::SubClass##ID:                                                  \
      static_assert(&Type::child_end != &SubClass::child_end,                  \
                    #SubClass " does not implement child_end!");               \
      return static_cast<const SubClass*>(this)->child_end();
#include "cdotc/AST/Types.def"
   }
}

Type::child_iterator_range Type::children() const
{
   return child_iterator_range(child_begin(), child_end());
}

bool Type::isIntegerType() const
{
   if (BuiltinType* BI = asBuiltinType())
      return BI->isAnyIntegerType();

   return false;
}

bool Type::isFPType() const
{
   if (BuiltinType* BI = asBuiltinType())
      return BI->isAnyFloatingPointType();

   return false;
}

bool Type::isUnknownAnyType() const
{
   if (BuiltinType* BI = asBuiltinType())
      return BI->isUnknownAnyTy();

   return false;
}

bool Type::isVoidType() const
{
   if (BuiltinType* BI = asBuiltinType())
      return BI->isVoidTy();

   return false;
}

bool Type::isAutoType() const
{
   if (BuiltinType* BI = asBuiltinType())
      return BI->isAutoTy();

   return false;
}

bool Type::isEmptyTupleType() const
{
   if (TupleType* TT = asTupleType())
      return TT->getArity() == 0;

   return false;
}

bool Type::isErrorType() const
{
   if (BuiltinType* BI = asBuiltinType())
      return BI->isErrorTy();

   return false;
}

PointerType* Type::getPointerTo(cdot::ast::ASTContext& Ctx) const
{
   return Ctx.getPointerType(QualType(const_cast<Type*>(this)));
}

QualType QualType::getPointerTo(cdot::ast::ASTContext& Ctx) const
{
   return Ctx.getPointerType(*this);
}

unsigned short BuiltinType::getIntegerBitwidth() const
{
   switch (getKind()) {
#define CDOT_BUILTIN_INT(Name, BW, IsUnsigned)                                 \
   case Name:                                                                  \
      return BW;
#include "cdotc/Basic/BuiltinTypes.def"

   default:
      llvm_unreachable("not an integer type!");
   }
}

bool BuiltinType::isUnsignedInteger() const
{
   switch (getKind()) {
#define CDOT_BUILTIN_INT(Name, BW, IsUnsigned)                                 \
   case Name:                                                                  \
      return IsUnsigned;
#include "cdotc/Basic/BuiltinTypes.def"

   default:
      llvm_unreachable("not an integer type!");
   }
}

unsigned short BuiltinType::getFloatingPointPrecision() const
{
   switch (getKind()) {
#define CDOT_BUILTIN_FP(Name, Prec)                                            \
   case Name:                                                                  \
      return Prec;
#include "cdotc/Basic/BuiltinTypes.def"

   default:
      llvm_unreachable("not a floating point type!");
   }
}

bool BuiltinType::isAnyIntegerType() const
{
   switch (getKind()) {
#define CDOT_BUILTIN_INT(Name, BW, IsUnsigned)                                 \
   case Name:                                                                  \
      return true;
#include "cdotc/Basic/BuiltinTypes.def"

   default:
      return false;
   }
}

bool BuiltinType::isIntNTy(unsigned short n)
{
   switch (getKind()) {
#define CDOT_BUILTIN_INT(Name, BW, IsUnsigned)                                 \
   case Name:                                                                  \
      return BW == n;
#include "cdotc/Basic/BuiltinTypes.def"

   default:
      return false;
   }
}

bool BuiltinType::isIntNTy(unsigned short n, bool isUnsigned)
{
   switch (getKind()) {
#define CDOT_BUILTIN_INT(Name, BW, IsUnsigned)                                 \
   case Name:                                                                  \
      return BW == n && isUnsigned == IsUnsigned;
#include "cdotc/Basic/BuiltinTypes.def"

   default:
      return false;
   }
}

bool BuiltinType::isAnyFloatingPointType() const
{
   switch (getKind()) {
#define CDOT_BUILTIN_FP(Name, Prec)                                            \
   case Name:                                                                  \
      return true;
#include "cdotc/Basic/BuiltinTypes.def"

   default:
      return false;
   }
}

bool Type::isFloatTy() const
{
   if (BuiltinType* BI = asBuiltinType()) {
      return BI->isf32Ty();
   }

   return false;
}

bool Type::isDoubleTy() const
{
   if (BuiltinType* BI = asBuiltinType()) {
      return BI->isf64Ty();
   }

   return false;
}

bool Type::isThinFunctionTy() const
{
   return getTypeID() == TypeID::FunctionTypeID;
}

bool Type::isStringRepresentable() const { return true; }

bool Type::isSelfComparable() const
{
   llvm_unreachable("should not be called!");
}

bool Type::isHashable() const { return true; }

bool Type::isAnyType() const
{
   if (auto *R = asRecordType()) {
      return support::isa<ProtocolDecl>(R->getRecord())
         && support::cast<ProtocolDecl>(R->getRecord())->isAny();
   }

   return false;
}

QualType Type::getPointeeType() const
{
   assert(this->isPointerType() && "not a pointer type");
   return this->uncheckedAsPointerType()->getPointeeType();
}

QualType Type::getReferencedType() const
{
   assert(this->isReferenceType() && "not a reference type");
   return this->uncheckedAsReferenceType()->getReferencedType();
}

QualType Type::getBoxedType() const
{
   assert(this->isBoxType() && "not a box type");
   return this->uncheckedAsBoxType()->getBoxedType();
}

QualType Type::removeReference() const
{
   if (auto* Ref = dyn_cast<ReferenceType>(this))
      return Ref->getReferencedType();

   return const_cast<Type*>(this);
}

QualType Type::removeMetaType() const
{
   if (auto* Meta = dyn_cast<MetaType>(this))
      return Meta->getUnderlyingType();

   return const_cast<Type*>(this);
}

Type::operator QualType() { return QualType(this); }

unsigned short Type::getAlignment() const
{
   switch (getTypeID()) {
   case TypeID::TypedefTypeID:
      return asRealTypedefType()->getAliasedType()->getAlignment();
   case TypeID::BuiltinTypeID: {
      BuiltinType* BI = asBuiltinType();
      if (BI->isAnyIntegerType())
         return BI->getIntegerBitwidth() / 8;

      return BI->getPrecision() / 8;
   }
   case TypeID::RecordTypeID:
   case TypeID::DependentRecordTypeID:
      return this->asRecordType()->getAlignment();
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
   return isRecordType() && this->asRecordType()->isRawEnum();
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

QualType Type::getDesugaredType() const
{
   switch (getTypeID()) {
   case Type::TypedefTypeID:
   case Type::DependentTypedefTypeID:
      return getCanonicalType();
   case Type::TemplateParamTypeID:
      return cast<TemplateParamType>(this)->getCovariance();
   case Type::AssociatedTypeID: {
      auto* AT = cast<AssociatedType>(this)->getDecl();
      return AT->getCovariance().getResolvedType();
   }
   default:
      break;
   }

   return const_cast<Type*>(this);
}

sema::FinalTemplateArgumentList& Type::getTemplateArgs() const
{
   switch (getTypeID()) {
   case TypeID::RecordTypeID:
      return this->uncheckedAsRecordType()->getTemplateArgs();
   case TypeID::TypedefTypeID:
      return cast<TypedefType>(this)->getTemplateArgs();
   case TypeID::DependentRecordTypeID:
      return cast<DependentRecordType>(this)->getTemplateArgs();
   case TypeID::DependentTypedefTypeID:
      return cast<DependentTypedefType>(this)->getTemplateArgs();
   default:
      llvm_unreachable("type has no template args!");
   }
}

bool Type::hasTemplateArgs() const
{
   switch (getTypeID()) {
   case TypeID::RecordTypeID:
      return this->uncheckedAsRecordType()->hasTemplateArgs();
   case TypeID::TypedefTypeID:
      return cast<TypedefType>(this)->hasTemplateArgs();
   case TypeID::DependentRecordTypeID:
   case TypeID::DependentTypedefTypeID:
      return true;
   default:
      return false;
   }
}

ast::RecordDecl* Type::getRecord() const
{
   if (auto Gen = dyn_cast<TemplateParamType>(this)) {
      return Gen->getCovariance()->getRecord();
   }

   assert(isRecordType() && "not an object");
   return this->asRecordType()->getRecord();
}

size_t Type::getSize() const
{
   switch (getTypeID()) {
   case TypeID::TypedefTypeID:
      return asRealTypedefType()->getAliasedType()->getSize();
   case TypeID::BuiltinTypeID: {
      BuiltinType* BI = asBuiltinType();
      if (BI->isAnyIntegerType())
         return BI->getIntegerBitwidth() / 8;

      if (BI->isFPType())
         return BI->getPrecision() / 8;

      llvm_unreachable("cannot calculate size of type");
   }
   case TypeID::RecordTypeID:
   case TypeID::DependentRecordTypeID:
      return this->asRecordType()->getSize();
   case TypeID::FunctionTypeID:
      return this->asFunctionType()->getSize();
   case TypeID::LambdaTypeID:
      return 2 * sizeof(void*);
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
   if (auto Obj = this->asRecordType()) {
      if (getRecord()->isClass())
         return sizeof(void*);

      return Obj->getSize();
   }

   return getSize();
}

unsigned short Type::getMemberAlignment() const
{
   if (auto Obj = this->asRecordType()) {
      if (getRecord()->isClass())
         return sizeof(void*);

      return Obj->getAlignment();
   }

   return getAlignment();
}

bool Type::isUnsigned() const
{
   if (BuiltinType const* BI = asBuiltinType())
      return BI->isUnsignedInteger();

   if (isRawEnum()) {
      return cast<EnumDecl>(getRecord())->getRawType()->isUnsigned();
   }

   llvm_unreachable("not an integer type");
}

unsigned short Type::getBitwidth() const
{
   if (BuiltinType const* BI = asBuiltinType())
      return BI->getIntegerBitwidth();

   if (isRawEnum()) {
      return cast<EnumDecl>(getRecord())->getRawType()->getBitwidth();
   }

   llvm_unreachable("not an integer type");
}

Type* Type::getSignedOfSameWidth(ast::ASTContext& Ctx) const
{
   assert(isIntegerType() && "not an integer type!");
   switch (uncheckedAsBuiltinType()->getKind()) {
   case BuiltinType::i1:
      return Ctx.getInt1Ty();
   case BuiltinType::i8:
   case BuiltinType::u8:
      return Ctx.getInt8Ty();
   case BuiltinType::i16:
   case BuiltinType::u16:
      return Ctx.getInt16Ty();
   case BuiltinType::i32:
   case BuiltinType::u32:
      return Ctx.getInt32Ty();
   case BuiltinType::i64:
   case BuiltinType::u64:
      return Ctx.getInt64Ty();
   case BuiltinType::i128:
   case BuiltinType::u128:
      return Ctx.geti128Ty();
   default:
      llvm_unreachable("bad integer kind");
   }
}

Type* Type::getUnsignedOfSameWidth(ast::ASTContext& Ctx) const
{
   assert(isIntegerType() && "not an integer type!");
   switch (uncheckedAsBuiltinType()->getKind()) {
   case BuiltinType::i1:
      return Ctx.getInt1Ty();
   case BuiltinType::i8:
   case BuiltinType::u8:
      return Ctx.getUInt8Ty();
   case BuiltinType::i16:
   case BuiltinType::u16:
      return Ctx.getUInt16Ty();
   case BuiltinType::i32:
   case BuiltinType::u32:
      return Ctx.getUInt32Ty();
   case BuiltinType::i64:
   case BuiltinType::u64:
      return Ctx.getUInt64Ty();
   case BuiltinType::i128:
   case BuiltinType::u128:
      return Ctx.getu128Ty();
   default:
      llvm_unreachable("bad integer kind");
   }
}

unsigned short Type::getPrecision() const
{
   if (BuiltinType const* BI = asBuiltinType())
      return BI->getFloatingPointPrecision();

   if (isRawEnum())
      return cast<EnumDecl>(getRecord())->getRawType()->getPrecision();

   llvm_unreachable("not an integer type");
}

bool Type::isEnum() const { return isRecordType() && getRecord()->isEnum(); }

bool Type::isUnion() const { return isRecordType() && getRecord()->isUnion(); }

bool Type::isProtocol() const
{
   return isRecordType() && getRecord()->isProtocol();
}

bool Type::isClass() const { return isRecordType() && getRecord()->isClass(); }

bool Type::isStruct() const
{
   return isRecordType() && getRecord()->isStruct();
}

bool Type::isOptionTy() const
{
   if (!isRecordType())
      return false;

   auto Rec = getRecord();
   while (Rec->getSpecializedTemplate())
      Rec = Rec->getSpecializedTemplate();

   return Rec->getName() == "Option";
}

bool Type::isRefcounted() const
{
   switch (getTypeID()) {
   case TypeID::LambdaTypeID:
   case TypeID::BoxTypeID:
      return true;
   case TypeID::RecordTypeID:
      return getRecord()->isClass();
   default:
      return false;
   }
}

bool Type::needsCleanup() const
{
   switch (getTypeID()) {
   case TypeID::TypedefTypeID:
      return asRealTypedefType()->getAliasedType()->needsCleanup();
   case TypeID::RecordTypeID:
   case TypeID::TupleTypeID:
   case TypeID::ArrayTypeID:
   case TypeID::LambdaTypeID:
      return true;
   default:
      return false;
   }
}

std::string QualType::toString() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << *this;
   return OS.str();
}

string Type::toString() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << QualType(const_cast<Type*>(this));
   return OS.str();
}

std::string Type::toDiagString() const
{
   return QualType(const_cast<Type*>(this)).toDiagString();
}

namespace {

template<class SubClass> class TypePrinterBase : public TypeVisitor<SubClass> {
protected:
   llvm::raw_ostream& OS;

   TypePrinterBase(llvm::raw_ostream& OS) : OS(OS) {}

public:
   void visitTypedefType(const TypedefType* Ty)
   {
      OS << Ty->getTypedef()->getDeclName();
   }

   void visitBuiltinType(const BuiltinType* Ty)
   {
      if (Ty->isUnknownAnyTy()) {
         OS << "?";
         return;
      }

      switch (Ty->getKind()) {
#define CDOT_BUILTIN_TYPE(Name)                                                \
   case BuiltinType::Name:                                                     \
      OS << #Name;                                                             \
      break;
#include "cdotc/Basic/BuiltinTypes.def"

      default:
         llvm_unreachable("bad builtin type");
      }
   }

   void visitTemplateArg(const sema::TemplateArgument& Arg)
   {
      if (Arg.isVariadic()) {
         OS << "(";

         unsigned i = 0;
         for (auto& VA : Arg.getVariadicArgs()) {
            if (i++ != 0)
               OS << ", ";
            visitTemplateArg(VA);
         }

         OS << ")";
      }
      else if (Arg.isType()) {
         this->visit(Arg.getNonCanonicalType());
      }
      else if (auto Val = Arg.getValue()) {
         OS << *Val;
      }
      else {
         OS << "<null>";
      }
   }

   void visitRecordType(const RecordType* Ty)
   {
      OS << Ty->getRecord()->getFullName();
   }

   void visitDependentRecordType(const DependentRecordType* Ty)
   {
      auto R = Ty->getRecord();
      OS << R->getFullName() << "<";

      auto& Args = cast<DependentRecordType>(Ty)->getTemplateArgs();
      unsigned i = 0;

      for (auto& Arg : Args) {
         if (i++ != 0)
            OS << ", ";
         visitTemplateArg(Arg);
      }

      OS << ">";
   }

   void visitDependentTypedefType(const DependentTypedefType* Ty)
   {
      auto td = Ty->getTypedef();
      OS << td->getFullName() << "<";

      auto& Args = cast<DependentTypedefType>(Ty)->getTemplateArgs();
      unsigned i = 0;

      for (auto& Arg : Args) {
         if (i++ != 0)
            OS << ", ";
         visitTemplateArg(Arg);
      }

      OS << ">";
   }

   void visitExistentialType(const ExistentialType* T)
   {
      unsigned i = 0;
      for (auto P : T->getExistentials()) {
         if (i++ != 0)
            OS << " & ";
         OS << P;
      }
   }

   void visitFunctionType(const FunctionType* Ty)
   {
      if (!isa<LambdaType>(Ty))
         OS << "@thin ";

      auto argTypes = Ty->getParamTypes();
      OS << "(";

      size_t i = 0;
      for (const auto& arg : argTypes) {
         if (i++ != 0)
            OS << ", ";
         OS << arg.toString();
      }

      OS << ") ";

      if (Ty->isUnsafe())
         OS << "unsafe ";
      if (Ty->isAsync())
         OS << "async";
      if (Ty->throws())
         OS << "throws ";

      OS << "-> " << Ty->getReturnType().toString();
   }

   void visitLambdaType(const LambdaType* Ty) { visitFunctionType(Ty); }

   void visitTupleType(const TupleType* Ty)
   {
      OS << "(";

      unsigned i = 0;
      for (auto& Cont : Ty->getContainedTypes()) {
         if (i++ != 0)
            OS << ", ";
         OS << Cont;
      }

      OS << ")";
   }

   void visitArrayType(const ArrayType* Ty)
   {
      OS << "[" << Ty->getElementType() << "; " << Ty->getNumElements() << "]";
   }

   void visitDependentSizeArrayType(const DependentSizeArrayType* Ty)
   {
      OS << "[" << Ty->getElementType() << "; ?]";
   }

   void visitInferredSizeArrayType(const InferredSizeArrayType* Ty)
   {
      OS << "[" << Ty->getElementType() << "; ?]";
   }

   void visitMetaType(const MetaType* Ty)
   {
      OS << "MetaType<" << Ty->getUnderlyingType() << ">";
   }

   void visitTemplateParamType(const TemplateParamType* Ty)
   {
      llvm::outs() << "trying to print" << Ty->getParam() << "\n";
      OS << Ty->getParam()->getDeclName();
      if (Ty->isVariadic())
         OS << "...";
   }

   void visitAssociatedType(const AssociatedType* Ty)
   {
      OS << Ty->getDecl()->getFullName();
   }

   void visitDependentNameType(const DependentNameType* Ty)
   {
      OS << Ty->getNameSpec();
   }

   void visitTypeVariableType(const TypeVariableType* Ty)
   {
      OS << "T" << Ty->getVariableID();
   }

   void visitPointerType(const PointerType* Ty)
   {
      auto pointee = Ty->getPointeeType();
      if (pointee->isVoidType())
         OS << "builtin.RawPointer";
      else
         OS << "builtin.RawPointer<" << pointee << ">";
   }

   void visitMutablePointerType(const MutablePointerType* Ty)
   {
      auto pointee = Ty->getPointeeType();
      if (pointee->isVoidType())
         OS << "builtin.RawMutablePointer";
      else
         OS << "builtin.RawMutablePointer<" << pointee << ">";
   }

   void visitReferenceType(const ReferenceType* Ty)
   {
      OS << "&" << Ty->getReferencedType();
   }

   void visitBoxType(const BoxType* Ty)
   {
      OS << "Box<" << Ty->getBoxedType() << ">";
   }

   void visitTokenType(const TokenType* Ty) { OS << "token"; }
};

class TypePrinter : public TypePrinterBase<TypePrinter> {
public:
   TypePrinter(llvm::raw_ostream& OS) : TypePrinterBase(OS) {}
};

class DiagTypePrinter : public TypePrinterBase<DiagTypePrinter> {
public:
   explicit DiagTypePrinter(llvm::raw_ostream& OS) : TypePrinterBase(OS) {}

   void visitTemplateParamType(const TemplateParamType* Ty)
   {
      OS << cast<NamedDecl>(Ty->getParam()->getDeclContext())->getFullName(false)
         << '.'
         << Ty->getTemplateParamTypeName();
   }

   void visitTypedefType(const TypedefType* Ty)
   {
      auto TD = cast<TypedefType>(Ty)->getTypedef();
      OS << TD->getDeclName();

      if (!TD->hasAttribute<_BuiltinAttr>()) {
         OS << " (aka ";
         visit(Ty->getAliasedType());
         OS << ")";
      }
   }

   void visitAssociatedType(const AssociatedType* Ty)
   {
      auto AT = Ty->getDecl();
      if (auto Outer = Ty->getOuterAT()) {
         if (Outer->isExistentialType()) {
            OS << "(" << QualType(Outer) << ")";
         }
         else {
            OS << QualType(Outer);
         }

         OS << "." << AT->getDeclName();
         return;
      }
      else {
         OS << AT->getFullName();
      }
   }

   void visitReferenceType(const ReferenceType* Ty)
   {
      OS << "&";
      visit(Ty->getReferencedType());
   }

   void visitMutableReferenceType(const MutableReferenceType* Ty)
   {
      OS << "&mut ";
      visit(Ty->getReferencedType());
   }

   void visitFunctionType(const FunctionType* Ty)
   {
      if (!isa<LambdaType>(Ty))
         OS << "@thin ";

      auto argTypes = Ty->getParamTypes();
      auto paramInfo = Ty->getParamInfo();

      OS << "(";

      size_t i = 0;
      for (const auto& arg : argTypes) {
         if (i != 0)
            OS << ", ";

         switch (paramInfo[i].getConvention()) {
         case ArgumentConvention::Owned:
            OS << "owned ";
            break;
         case ArgumentConvention::Borrowed:
            OS << "borrow ";
            break;
         case ArgumentConvention::MutableRef:
            OS << "mut ref ";
            break;
         case ArgumentConvention::ImmutableRef:
            OS << "ref ";
            break;
         case ArgumentConvention::Default:
            break;
         }

         visit(arg);
         ++i;
      }

      OS << ") ";
      if (Ty->isUnsafe())
         OS << "unsafe ";
      if (Ty->isAsync())
         OS << "async";
      if (Ty->throws())
         OS << "throws ";

      OS << "-> ";
      visit(Ty->getReturnType());
   }

   void visitLambdaType(const LambdaType* Ty) { visitFunctionType(Ty); }
};

} // anonymous namespace

llvm::raw_ostream& operator<<(llvm::raw_ostream& OS, QualType Ty)
{
   if (!Ty) {
      OS << "<null>";
   }
   else {
      DiagTypePrinter(OS).visit(Ty);
   }

   return OS;
}

llvm::raw_ostream& operator<<(llvm::raw_ostream& OS, const ast::SourceType& Ty)
{
   return OS << Ty.getResolvedType();
}

diag::DiagnosticBuilder& operator<<(diag::DiagnosticBuilder& Diag, QualType Ty)
{
   std::string str;
   llvm::raw_string_ostream OS(str);

   if (!Ty) {
      OS << "<null>";
   }
   else {
      DiagTypePrinter(OS).visit(Ty);
   }

   return Diag << OS.str();
}

diag::DiagnosticBuilder& operator<<(diag::DiagnosticBuilder& Diag,
                                    const ast::SourceType& Ty)
{
   return Diag << Ty.getResolvedType();
}

std::string QualType::toDiagString() const
{
   std::string str;
   llvm::raw_string_ostream OS(str);

   if (!*this) {
      OS << "<null>";
   }
   else {
      DiagTypePrinter(OS).visit(*this);
   }

   return OS.str();
}

BuiltinType::BuiltinType(BuiltinKind kind) : Type(TypeID::BuiltinTypeID, this)
{
   BuiltinBits.kind = kind;

   if (kind == UnknownAny) {
      Bits.Props |= TypeProperties::ContainsUnknownAny;
   }
}

PointerType::PointerType(QualType pointee, Type* CanonicalType)
    : Type(TypeID::PointerTypeID, CanonicalType), pointeeType(pointee)
{
   Bits.Props |= pointee->properties();
}

PointerType::PointerType(TypeID typeID, QualType pointee, Type* CanonicalType)
    : Type(typeID, CanonicalType), pointeeType(pointee)
{
   Bits.Props |= pointee->properties();
}

MutablePointerType::MutablePointerType(QualType pointeeType,
                                       Type* CanonicalType)
    : PointerType(MutablePointerTypeID, pointeeType, CanonicalType)
{
}

MutableReferenceType::MutableReferenceType(QualType referencedType,
                                           Type* CanonicalType)
    : ReferenceType(MutableReferenceTypeID, referencedType, CanonicalType)
{
}

MutableReferenceType::MutableReferenceType(TypeID ID, QualType referencedType,
                                           Type* CanonicalType)
    : ReferenceType(ID, referencedType, CanonicalType)
{
}

ReferenceType::ReferenceType(QualType referencedType, Type* CanonicalType)
    : Type(TypeID::ReferenceTypeID, CanonicalType),
      referencedType(referencedType)
{
   Bits.Props |= referencedType->properties();
}

ReferenceType::ReferenceType(TypeID typeID, QualType referencedType,
                             Type* CanonicalType)
    : Type(typeID, CanonicalType), referencedType(referencedType)
{
   Bits.Props |= referencedType->properties();
}

BoxType::BoxType(QualType BoxedTy, Type* CanonicalTy)
    : Type(BoxTypeID, CanonicalTy), BoxedTy(BoxedTy)
{
   Bits.Props |= BoxedTy->properties();
}

TokenType::TokenType() : Type(TokenTypeID, this) {}

RecordType::RecordType(RecordDecl* record)
    : Type(TypeID::RecordTypeID, nullptr), Rec(record)
{
   assert(!record->isTemplate() && "should be a DependentRecordType!");
}

RecordType::RecordType(TypeID typeID, ast::RecordDecl* record, bool Dependent)
    : Type(typeID, nullptr), Rec(record)
{
   if (Dependent) {
      Bits.Props |= TypeProperties::ContainsUnconstrainedGeneric;
   }
}

static void copyTemplateArgProps(TypeProperties& Props,
                                 const sema::TemplateArgument& Arg)
{
   if (!Arg.isType())
      return;

   if (Arg.isVariadic()) {
      for (auto& VA : Arg.getVariadicArgs()) {
         copyTemplateArgProps(Props, VA);
      }

      return;
   }

   Props |= Arg.getNonCanonicalType()->properties();
}

DependentRecordType::DependentRecordType(
    RecordDecl* record, sema::FinalTemplateArgumentList* templateArgs,
    QualType Parent, Type* CanonicalType)
    : RecordType(TypeID::DependentRecordTypeID, record,
                 templateArgs->isStillDependent()),
      Parent(Parent), templateArgs(templateArgs)
{
   if (CanonicalType) {
      this->CanonicalType = CanonicalType;
   }

   for (auto& Arg : *templateArgs) {
      copyTemplateArgProps(Bits.Props, Arg);
   }

   if (Parent) {
      Bits.Props |= Parent->properties();
   }
}

sema::FinalTemplateArgumentList& RecordType::getTemplateArgs() const
{
   return Rec->getTemplateArgs();
}

bool RecordType::hasTemplateArgs() const { return Rec->isInstantiation(); }

bool RecordType::isRawEnum() const
{
   auto rec = getRecord();
   return rec && rec->isRawEnum();
}

unsigned short RecordType::getAlignment() const
{
   llvm_unreachable("delete this!");
}

size_t RecordType::getSize() const { llvm_unreachable("delete this!"); }

void RecordType::setDependent(bool dep)
{
   if (dep) {
      Bits.Props |= TypeProperties::ContainsTemplateParamType;
   }
}

void DependentRecordType::Profile(llvm::FoldingSetNodeID& ID,
                                  ast::RecordDecl* R,
                                  sema::FinalTemplateArgumentList* templateArgs,
                                  QualType Parent)
{
   ID.AddPointer(R);
   ID.AddPointer(Parent.getAsOpaquePtr());
   templateArgs->Profile(ID);
}

ArrayType::ArrayType(QualType elementType, unsigned numElements,
                     Type* CanonicalType)
    : Type(TypeID::ArrayTypeID, CanonicalType), elementType(elementType),
      numElements(numElements)
{
   Bits.Props |= elementType->properties();
}

ArrayType::ArrayType(TypeID typeID, QualType elementType, Type* CanonicalType)
    : Type(typeID, CanonicalType), elementType(elementType), numElements(0)
{
   Bits.Props |= elementType->properties();
}

DependentSizeArrayType::DependentSizeArrayType(QualType elementType,
                                               ast::StaticExpr* DependentExpr,
                                               Type* CanonicalType)
    : ArrayType(TypeID::DependentSizeArrayTypeID, elementType, CanonicalType),
      DependentExpr(DependentExpr)
{
   Bits.Props |= TypeProperties::ContainsDependentSizeArrayType;
}

InferredSizeArrayType::InferredSizeArrayType(QualType elementTy,
                                             Type* CanonicalTy)
    : ArrayType(TypeID::InferredSizeArrayTypeID, elementTy, CanonicalTy)
{
}

ExistentialType::ExistentialType(ArrayRef<QualType> Existentials,
                                 Type* CanonicalType, TypeProperties Props)
    : Type(TypeID::ExistentialTypeID, CanonicalType),
      NumExistentials((unsigned)Existentials.size())
{
   Bits.Props = Props;
   std::copy(Existentials.begin(), Existentials.end(),
             reinterpret_cast<QualType*>(this + 1));
}

ArrayRef<QualType> ExistentialType::getExistentials() const
{
   return {reinterpret_cast<QualType const*>(this + 1), NumExistentials};
}

bool ExistentialType::contains(CanType T) const
{
   if (auto *otherExt = T->asExistentialType()) {
      for (CanType ext : otherExt->getExistentials()) {
         if (!contains(ext)) {
            return false;
         }
      }

      return true;
   }

   for (CanType ext : getExistentials()) {
      if (ext == T) {
         return true;
      }
   }

   return false;
}

void ExistentialType::Profile(llvm::FoldingSetNodeID& ID)
{
   Profile(ID, getExistentials());
}

void ExistentialType::Profile(llvm::FoldingSetNodeID& ID,
                              ArrayRef<QualType> Existentials)
{
   for (auto P : Existentials) {
      P.Profile(ID);
   }
}

FunctionType::FunctionType(TypeID typeID, QualType returnType,
                           llvm::ArrayRef<QualType> argTypes,
                           llvm::ArrayRef<ParamInfo> paramInfo, ExtFlags flags,
                           Type* CanonicalType, TypeProperties Props)
    : Type(typeID, CanonicalType), NumParams((unsigned)argTypes.size()),
      returnType(returnType)
{
   assert(argTypes.size() == paramInfo.size()
          && "didn't provide parameter "
             "info for every parameter!");

   FuncBits.flags = flags;
   std::copy(argTypes.begin(), argTypes.end(),
             reinterpret_cast<QualType*>(this + 1));

   assert(&this->returnType + 1 == param_begin()
          && "bad function type layout!");

   std::copy(paramInfo.begin(), paramInfo.end(),
             reinterpret_cast<ParamInfo*>(reinterpret_cast<QualType*>(this + 1)
                                          + NumParams));

   Bits.Props = Props;
}

FunctionType::FunctionType(QualType returnType,
                           llvm::ArrayRef<QualType> argTypes,
                           llvm::ArrayRef<ParamInfo> paramInfo, ExtFlags flags,
                           Type* CanonicalType, TypeProperties Props)
    : Type(TypeID::FunctionTypeID, CanonicalType),
      NumParams((unsigned)argTypes.size()), returnType(returnType)
{
   assert(argTypes.size() == paramInfo.size()
          && "didn't provide parameter "
             "info for every parameter!");

   FuncBits.flags = flags;
   std::copy(argTypes.begin(), argTypes.end(),
             reinterpret_cast<QualType*>(this + 1));

   assert(&this->returnType + 1 == param_begin()
          && "bad function type layout!");

   std::copy(paramInfo.begin(), paramInfo.end(),
             reinterpret_cast<ParamInfo*>(reinterpret_cast<QualType*>(this + 1)
                                          + NumParams));

   Bits.Props = Props;
}

FunctionType::ParamInfo::ParamInfo()
    : Conv(ArgumentConvention::Borrowed), Label(nullptr)
{
}

void FunctionType::ParamInfo::Profile(llvm::FoldingSetNodeID& ID) const
{
   ID.AddInteger((char)Conv);
   ID.AddPointer(Label);
}

bool FunctionType::ParamInfo::operator==(const ParamInfo& I) const
{
   return Conv == I.Conv && Label == I.Label;
}

LambdaType::LambdaType(QualType returnType, llvm::ArrayRef<QualType> argTypes,
                       llvm::ArrayRef<ParamInfo> paramInfo, ExtFlags flags,
                       Type* CanonicalType, TypeProperties Props)
    : FunctionType(TypeID::LambdaTypeID, returnType, argTypes, paramInfo, flags,
                   CanonicalType, Props)
{
}

TupleType::TupleType(llvm::ArrayRef<QualType> containedTypes,
                     Type* CanonicalType, TypeProperties Props)
    : Type(TupleTypeID, CanonicalType), NumTys((unsigned)containedTypes.size())
{
   std::copy(containedTypes.begin(), containedTypes.end(),
             reinterpret_cast<QualType*>(this + 1));

   Bits.Props = Props;
}

TemplateParamType::TemplateParamType(TemplateParamDecl* Param)
    : Type(TemplateParamTypeID, nullptr), P(Param)
{
   Bits.Props |= TypeProperties::ContainsTemplateParamType;

   if (Param->isVariadic()) {
      Bits.Props |= TypeProperties::ContainsUnexpandedParameterPack;
   }

   // FIXME runtime-generics
   if (Param->isUnbounded() || true) {
      Bits.Props |= TypeProperties::ContainsUnconstrainedGeneric;
   }
   else {
      Bits.Props |= TypeProperties::ContainsProtocolWithAssociatedTypes;
   }
}

llvm::StringRef TemplateParamType::getTemplateParamTypeName() const
{
   return P->getName();
}

QualType TemplateParamType::getCovariance() const { return P->getCovariance(); }

QualType TemplateParamType::getContravariance() const
{
   return P->getContravariance();
}

unsigned TemplateParamType::getIndex() const { return P->getIndex(); }

bool TemplateParamType::isVariadic() const { return P->isVariadic(); }

AssociatedType::AssociatedType(AssociatedTypeDecl* AT, QualType OuterAT)
    : Type(AssociatedTypeID, nullptr), AT(AT), OuterAT(OuterAT)
{
   Bits.Props |= TypeProperties::ContainsAssociatedType;
   if (OuterAT) {
      Bits.Props |= OuterAT->properties();
   }
}

MetaType::MetaType(QualType forType, Type* CanonicalType)
    : Type(TypeID::MetaTypeID, CanonicalType), forType(forType)
{
   Bits.Props |= forType->properties();
}

TypedefType::TypedefType(AliasDecl* td) : Type(TypedefTypeID, nullptr), td(td)
{
   if (!td->isStrong()) {
      CanonicalType = td->getType()
                          ->asMetaType()
                          ->getUnderlyingType()
                          ->getCanonicalType();
      Bits.Props |= CanonicalType->properties();
   }
}

TypedefType::TypedefType(TypeID typeID, AliasDecl* td, bool Dependent)
    : Type(typeID, nullptr), td(td)
{
}

void TypedefType::setCanonicalType(QualType CanonicalType)
{
   this->CanonicalType = CanonicalType;
   Bits.Props |= CanonicalType->properties();
}

sema::FinalTemplateArgumentList& TypedefType::getTemplateArgs() const
{
   return td->getTemplateArgs();
}

bool TypedefType::hasTemplateArgs() const { return td->isInstantiation(); }

QualType TypedefType::getAliasedType() const
{
   return td->getType()->asMetaType()->getUnderlyingType();
}

StringRef TypedefType::getAliasName() const { return td->getName(); }

DependentTypedefType::DependentTypedefType(
    AliasDecl* td, sema::FinalTemplateArgumentList* templateArgs,
    QualType Parent, Type* CanonicalType)
    : TypedefType(TypeID::DependentTypedefTypeID, td,
                  templateArgs->isStillDependent()),
      Parent(Parent), templateArgs(templateArgs)
{
   if (CanonicalType) {
      this->CanonicalType = CanonicalType;
   }

   for (auto& Arg : *templateArgs) {
      copyTemplateArgProps(Bits.Props, Arg);
   }

   if (Parent) {
      Bits.Props |= Parent->properties();
   }
}

void DependentTypedefType::Profile(
    llvm::FoldingSetNodeID& ID, AliasDecl* td,
    sema::FinalTemplateArgumentList* templateArgs, QualType Parent)
{
   ID.AddPointer(td);
   ID.AddPointer(Parent.getAsOpaquePtr());
   templateArgs->Profile(ID);
}

DependentNameType::DependentNameType(NestedNameSpecifierWithLoc* NameSpec)
    : Type(DependentNameTypeID, nullptr), NameSpec(NameSpec)
{
   Bits.Props |= TypeProperties::ContainsDependentNameType;

   auto* Name = NameSpec->getNameSpec();
   while (Name) {
      switch (Name->getKind()) {
      case NestedNameSpecifier::TemplateParam:
         Bits.Props |= TypeProperties::ContainsTemplateParamType;
         break;
      case NestedNameSpecifier::AssociatedType:
         Bits.Props |= TypeProperties::ContainsAssociatedType;
         break;
      case NestedNameSpecifier::TemplateArgList:
         for (auto& Arg : *Name->getTemplateArgs()) {
            copyTemplateArgProps(Bits.Props, Arg);
         }

         break;
      case NestedNameSpecifier::Type:
         Bits.Props |= Name->getType()->properties();
         break;
      default:
         break;
      }

      Name = Name->getPrevious();
   }
}

NestedNameSpecifier* DependentNameType::getNameSpec() const
{
   return NameSpec->getNameSpec();
}

void DependentNameType::Profile(llvm::FoldingSetNodeID& ID)
{
   Profile(ID, NameSpec);
}

void DependentNameType::Profile(llvm::FoldingSetNodeID& ID,
                                NestedNameSpecifierWithLoc* Name)
{
   ID.AddPointer(Name);

   auto SourceLocs = Name->getSourceRanges();
   for (auto Loc : SourceLocs) {
      ID.AddInteger(Loc.getStart().getOffset());
      ID.AddInteger(Loc.getEnd().getOffset());
   }
}

TypeVariableType::TypeVariableType(unsigned ID)
    : Type(TypeVariableTypeID, nullptr), ID(ID)
{
   Bits.Props |= TypeProperties::ContainsTypeVariable;
}

void ExistentialTypeBuilder::push_back(QualType T)
{
   if (T->isAnyType()) {
      return;
   }

   if (auto *Ext = T->asExistentialType()) {
      for (QualType Inner : Ext->getExistentials()) {
         push_back(Inner);
      }
   }
   else {
      _Tys.insert(T);
   }
}

void ExistentialTypeBuilder::remove(QualType T)
{
   if (auto *Ext = T->asExistentialType()) {
      for (QualType Inner : Ext->getExistentials()) {
         remove(Inner);
      }
   }
   else {
      auto it = std::find(_Tys.begin(), _Tys.end(), T);
      if (it != _Tys.end()) {
         _Tys.erase(it);
      }
   }
}

QualType ExistentialTypeBuilder::Build(ASTContext &C)
{
   if (_Tys.size() == 1) {
      return _Tys.front();
   }

   return C.getExistentialType(_Tys.takeVector());
}

} // namespace cdot