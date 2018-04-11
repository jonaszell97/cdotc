//
// Created by Jonas Zell on 13.08.17.
//

#include "Type.h"

#include "AST/ASTContext.h"
#include "AST/Decl.h"
#include "Basic/CastKind.h"
#include "Message/Diagnostics.h"
#include "Support/Casting.h"
#include "TypeVisitor.h"

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

#define CDOT_TYPE(Name, Parent)                                      \
bool Type::is##Name() const                                          \
{                                                                    \
   return this->isa<Name>();                                         \
}                                                                    \
                                                                     \
Name* Type::as##Name() const                                         \
{                                                                    \
   return const_cast<Name*>(getAs<Name>());                          \
}                                                                    \
                                                                     \
Name* Type::as##Name()                                               \
{                                                                    \
   return const_cast<Name*>(getAs<Name>());                          \
}                                                                    \
                                                                     \
Name* Type::uncheckedAs##Name() const                                \
{                                                                    \
   assert(Name::classof(CanonicalType) && "incompatible type!");     \
   return static_cast<Name*>(CanonicalType);                         \
}

#define CDOT_BASE_TYPE(Name) CDOT_TYPE(Name, "")
#include "Types.def"

Type::child_iterator Type::child_begin() const
{
   switch (getTypeID()) {
#  define CDOT_TYPE(SubClass, Parent)                                   \
   case TypeID::SubClass##ID:                                           \
      static_assert(&Type::child_begin != &SubClass::child_begin,       \
                    #SubClass " does not implement child_begin!");      \
      return static_cast<const SubClass*>(this)->child_begin();
#  include "Types.def"
   }
}

Type::child_iterator Type::child_end() const
{
   switch (getTypeID()) {
#  define CDOT_TYPE(SubClass, Parent)                                   \
   case TypeID::SubClass##ID:                                           \
      static_assert(&Type::child_end != &SubClass::child_end,           \
                    #SubClass " does not implement child_end!");        \
      return static_cast<const SubClass*>(this)->child_end();
#  include "Types.def"
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

PointerType* Type::getPointerTo(cdot::ast::ASTContext &Ctx) const
{
   return Ctx.getPointerType(QualType(const_cast<Type*>(this)));
}

QualType QualType::getPointerTo(cdot::ast::ASTContext &Ctx) const
{
   return Ctx.getPointerType(*this);
}

unsigned short BuiltinType::getIntegerBitwidth() const
{
   switch (getKind()) {
#  define CDOT_BUILTIN_INT(Name, BW, IsUnsigned) \
      case Name: return BW;
#  include "Basic/BuiltinTypes.def"

      default:
         llvm_unreachable("not an integer type!");
   }
}

bool BuiltinType::isUnsignedInteger() const
{
   switch (getKind()) {
#  define CDOT_BUILTIN_INT(Name, BW, IsUnsigned) \
      case Name: return IsUnsigned;
#  include "Basic/BuiltinTypes.def"

      default:
         llvm_unreachable("not an integer type!");
   }
}

unsigned short BuiltinType::getFloatingPointPrecision() const
{
   switch (getKind()) {
#  define CDOT_BUILTIN_FP(Name, Prec) \
      case Name: return Prec;
#  include "Basic/BuiltinTypes.def"

      default:
         llvm_unreachable("not a floating point type!");
   }
}

bool BuiltinType::isAnyIntegerType() const
{
   switch (getKind()) {
#  define CDOT_BUILTIN_INT(Name, BW, IsUnsigned) \
      case Name: return true;
#  include "Basic/BuiltinTypes.def"

      default:
         return false;
   }
}

bool BuiltinType::isIntNTy(unsigned short n)
{
   switch (getKind()) {
#  define CDOT_BUILTIN_INT(Name, BW, IsUnsigned) \
      case Name: return BW == n;
#  include "Basic/BuiltinTypes.def"

      default:
         return false;
   }
}

bool BuiltinType::isIntNTy(unsigned short n, bool isUnsigned)
{
   switch (getKind()) {
#  define CDOT_BUILTIN_INT(Name, BW, IsUnsigned) \
      case Name: return BW == n && isUnsigned == IsUnsigned;
#  include "Basic/BuiltinTypes.def"

      default:
         return false;
   }
}

bool BuiltinType::isAnyFloatingPointType() const
{
   switch (getKind()) {
#  define CDOT_BUILTIN_FP(Name, Prec) \
      case Name: return true;
#  include "Basic/BuiltinTypes.def"

      default:
         return false;
   }
}

bool Type::isFloatTy() const
{
   if (BuiltinType *BI = asBuiltinType()) {
      return BI->isf32Ty();
   }

   return false;
}

bool Type::isDoubleTy() const
{
   if (BuiltinType *BI = asBuiltinType()) {
      return BI->isf64Ty();
   }

   return false;
}

bool Type::isThinFunctionTy() const
{
   return getTypeID() == TypeID::FunctionTypeID;
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
      case TypeID::RecordTypeID:
         return getRecord()->getOperatorEquals() != nullptr;
   }
}

bool Type::isHashable() const
{
   return true;
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

QualType Type::stripReference() const
{
   if (isReferenceType())
      return getReferencedType();

   return const_cast<Type*>(this);
}

Type::operator QualType()
{
   return QualType(this);
}

unsigned short Type::getAlignment() const
{
   switch (getTypeID()) {
      case TypeID::TypedefTypeID:
         return asRealTypedefType()->getAliasedType()->getAlignment();
      case TypeID::BuiltinTypeID: {
         BuiltinType *BI = asBuiltinType();
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

Type* Type::getDesugaredType() const
{
   return getCanonicalType();
}

bool Type::isUnpopulatedType() const
{
   auto RecTy = asRecordType();
   if (!RecTy)
      return false;

   auto R = RecTy->getRecord();
   return support::isa<EnumDecl>(R) && cast<EnumDecl>(R)->isUnpopulated();
}

sema::TemplateArgList const& Type::getTemplateArgs() const
{
   assert(isRecordType() && "not an object");
   if (auto Inconcrete = this->asDependentRecordType())
      return Inconcrete->getTemplateArgs();

   return this->asRecordType()->getTemplateArgs();
}

bool Type::hasTemplateArgs() const
{
   if (!isRecordType())
      return false;

   if (auto Inconcrete = this->asDependentRecordType())
      return Inconcrete->hasTemplateArgs();

   return this->asRecordType()->hasTemplateArgs();
}

ast::RecordDecl *Type::getRecord() const
{
   if (auto Gen = dyn_cast<GenericType>(this)) {
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
         BuiltinType *BI = asBuiltinType();
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

Type* Type::getSignedOfSameWidth(ast::ASTContext &Ctx) const
{
   assert(isIntegerType() && "not an integer type!");
   switch (uncheckedAsBuiltinType()->getKind()) {
   case BuiltinType::i1:  return Ctx.getInt1Ty();
   case BuiltinType::i8:  case BuiltinType::u8: return Ctx.getInt8Ty();
   case BuiltinType::i16: case BuiltinType::u16: return Ctx.getInt16Ty();
   case BuiltinType::i32: case BuiltinType::u32: return Ctx.getInt32Ty();
   case BuiltinType::i64: case BuiltinType::u64: return Ctx.getInt64Ty();
   case BuiltinType::i128: case BuiltinType::u128: return Ctx.geti128Ty();
   default:
      llvm_unreachable("bad integer kind");
   }
}

Type* Type::getUnsignedOfSameWidth(ast::ASTContext &Ctx) const
{
   assert(isIntegerType() && "not an integer type!");
   switch (uncheckedAsBuiltinType()->getKind()) {
   case BuiltinType::i1:  return Ctx.getInt1Ty();
   case BuiltinType::i8:  case BuiltinType::u8: return Ctx.getUInt8Ty();
   case BuiltinType::i16: case BuiltinType::u16: return Ctx.getUInt16Ty();
   case BuiltinType::i32: case BuiltinType::u32: return Ctx.getUInt32Ty();
   case BuiltinType::i64: case BuiltinType::u64: return Ctx.getUInt64Ty();
   case BuiltinType::i128: case BuiltinType::u128: return Ctx.getu128Ty();
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

bool Type::isEnum() const
{
   return isRecordType() && getRecord()->isEnum();
}

bool Type::isUnion() const
{
   return isRecordType() && getRecord()->isUnion();
}

bool Type::isProtocol() const
{
   return isRecordType() && getRecord()->isProtocol();
}

bool Type::isClass() const
{
   return isRecordType() && getRecord()->isClass();
}

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

bool Type::needsStructReturn() const
{
   switch (getTypeID()) {
      case TypeID::TypedefTypeID:
         return asRealTypedefType()->getAliasedType()->needsStructReturn();
      case TypeID::RecordTypeID: {
         auto rec = getRecord();
         switch (rec->getKind()) {
            case Decl::EnumDeclID:
               return !cast<EnumDecl>(rec)->isRawEnum();
            case Decl::StructDeclID:
            case Decl::UnionDeclID:
            case Decl::ProtocolDeclID:
               return true;
            case Decl::ClassDeclID:
               return false;
            default:
               llvm_unreachable("bad record kind!");
         }
      }
      case TypeID::TupleTypeID:
      case TypeID::ArrayTypeID:
      case TypeID::MetaTypeID:
         return true;
      default:
         return false;
   }
}

bool Type::needsMemCpy() const
{
   return needsStructReturn();
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

namespace {

template<class SubClass>
class TypePrinterBase: public TypeVisitor<SubClass> {
protected:
   llvm::raw_ostream &OS;

   TypePrinterBase(llvm::raw_ostream &OS)
      : OS(OS)
   {}

public:
   void visitTypedefType(const TypedefType *Ty)
   {
      OS << Ty->asRealTypedefType()->getAliasName();
   }

   void visitBuiltinType(const BuiltinType *Ty)
   {
      switch (Ty->asBuiltinType()->getKind()) {
#        define CDOT_BUILTIN_TYPE(Name) \
            case BuiltinType::Name: OS << #Name; break;
#        include "Basic/BuiltinTypes.def"

      default:
         llvm_unreachable("bad builtin type");
      }
   }

   void visitTemplateArg(const sema::ResolvedTemplateArg &Arg)
   {
      if (Arg.isVariadic()) {
         OS << "(";

         unsigned i = 0;
         for (auto &VA : Arg.getVariadicArgs()) {
            if (i++ != 0) OS << ", ";
            visitTemplateArg(VA);
         }

         OS << ")";
      }
      else if (Arg.isType()) {
         this->visit(Arg.getType());
      }
      else {
         OS << Arg.getValue();
      }
   }

   void visitRecordType(const RecordType *Ty)
   {
      auto R = Ty->getRecord();
      auto *Ctx = R->getNonTransparentDeclContext();
      if (isa<NamedDecl>(Ctx)) {
         if (!isa<TranslationUnit>(Ctx)) {
            OS << cast<NamedDecl>(Ctx)->getFullName();
            OS << '.';
         }
      }

      if (R->isInstantiation()) {
         OS << R->getDeclName().getInstantiationName() << "[";

         auto &Args = *R->getDeclName().getInstantiationArgs();
         unsigned i = 0;

         for (auto &Arg : Args) {
            if (i++ != 0) OS << ", ";
            visitTemplateArg(Arg);
         }

         OS << "]";
      }
      else {
         OS << R->getDeclName();
      }
   }

   void visitDependentRecordType(const DependentRecordType *Ty)
   {
      auto R = Ty->getRecord();
      auto *Ctx = R->getNonTransparentDeclContext();
      if (isa<NamedDecl>(Ctx)) {
         if (!isa<TranslationUnit>(Ctx)) {
            OS << cast<NamedDecl>(Ctx)->getFullName();
            OS << '.';
         }
      }

      OS << R->getDeclName() << "[";

      auto &Args = cast<DependentRecordType>(Ty)->getTemplateArgs();
      unsigned i = 0;

      for (auto &Arg : Args) {
         if (i++ != 0) OS << ", ";
         visitTemplateArg(Arg);
      }

      OS << "]";
   }

   void visitFunctionType(const FunctionType *Ty)
   {
      if (!isa<LambdaType>(Ty))
         OS << "@thin ";

      auto argTypes = Ty->getParamTypes();
      OS << "(";

      size_t i = 0;
      for (const auto& arg : argTypes) {
         if (i++ != 0) OS << ", ";
         OS << arg.toString();
      }

      OS << ") -> " << Ty->getReturnType().toString();
   }

   void visitLambdaType(const LambdaType *Ty)
   {
      visitFunctionType(Ty);
   }

   void visitArrayType(const ArrayType *Ty)
   {
      ArrayType *arr = Ty->uncheckedAsArrayType();
      OS << "[" << arr->getElementType() << "; "
         << arr->getNumElements() << "]";
   }

   void visitDependentSizeArrayType(const DependentSizeArrayType *Ty)
   {
      OS << "[" << Ty->uncheckedAsDependentSizeArrayType()->getElementType()
         << "; ?]";
   }

   void visitInferredSizeArrayType(const InferredSizeArrayType *Ty)
   {
      OS << "[" << Ty->uncheckedAsInferredSizeArrayType()->getElementType()
         << "; ?]";
   }

   void visitMetaType(const MetaType *Ty)
   {
      OS << "MetaType["
         << Ty->uncheckedAsMetaType()->getUnderlyingType().toString()
         << "]";
   }

   void visitGenericType(const GenericType *Ty)
   {
      OS << Ty->getParam()->getDeclName();
   }

   void visitAssociatedType(const AssociatedType *Ty)
   {
      OS << Ty->getDecl()->getDeclName();
   }

   void visitPointerType(const PointerType *Ty)
   {
      OS << Ty->getPointeeType().toString() << "*";
   }

   void visitReferenceType(const ReferenceType *Ty)
   {
      OS << "ref " << Ty->getReferencedType();
   }
};

class TypePrinter: public TypePrinterBase<TypePrinter> {
public:
   TypePrinter(llvm::raw_ostream &OS)
      : TypePrinterBase(OS)
   { }
};

class DiagTypePrinter: public TypePrinterBase<DiagTypePrinter> {
public:
   explicit DiagTypePrinter(llvm::raw_ostream &OS)
      : TypePrinterBase(OS)
   { }

   void visitRecordType(const RecordType *Ty)
   {
      auto R = Ty->getRecord();
      auto *Ctx = R->getNonTransparentDeclContext();
      if (isa<NamedDecl>(Ctx)) {
         if (!isa<TranslationUnit>(Ctx)) {
            OS << cast<NamedDecl>(Ctx)->getFullName();
            OS << '.';
         }
      }

      if (R->isInstantiation()) {
         OS << R->getDeclName().getInstantiationName() << "[";

         auto &Args = *R->getDeclName().getInstantiationArgs();
         unsigned i = 0;

         for (auto &Arg : Args) {
            if (i++ != 0) OS << ", ";
            visitTemplateArg(Arg);
         }

         OS << "]";
      }
      else {
         OS << R->getDeclName();
      }
   }

   void visitDependentRecordType(const DependentRecordType *Ty)
   {
      auto R = Ty->getRecord();
      auto *Ctx = R->getNonTransparentDeclContext();
      if (isa<NamedDecl>(Ctx)) {
         if (!isa<TranslationUnit>(Ctx)) {
            OS << cast<NamedDecl>(Ctx)->getFullName();
            OS << '.';
         }
      }

      OS << R->getDeclName() << "[";

      auto &Args = cast<DependentRecordType>(Ty)->getTemplateArgs();
      unsigned i = 0;

      for (auto &Arg : Args) {
         if (i++ != 0) OS << ", ";
         visitTemplateArg(Arg);
      }

      OS << "]";
   }

   void visitGenericType(const GenericType *Ty)
   {
      auto TD = cast<TypedefType>(Ty)->getTypedef();
      OS << TD->getDeclName() << " (aka " << TD->getOriginTy() << ")";
   }

   void visitAssociatedType(const AssociatedType *Ty)
   {
      auto AT = cast<AssociatedType>(Ty)->getDecl();
      OS << AT->getDeclName();
      if (AT->isImplementation()) {
         OS << " (aka " << AT->getActualType() << ")";
      }
   }
};

} // anonymous namespace

llvm::raw_ostream &operator<<(llvm::raw_ostream &OS, QualType Ty)
{
   TypePrinter(OS).visit(Ty);
   return OS;
}

llvm::raw_ostream &operator<<(llvm::raw_ostream &OS, const ast::SourceType &Ty)
{
   return OS << Ty.getResolvedType();
}

diag::DiagnosticBuilder &operator<<(diag::DiagnosticBuilder &Diag, QualType Ty)
{
   std::string str;
   llvm::raw_string_ostream OS(str);

   DiagTypePrinter(OS).visit(Ty);
   return Diag << OS.str();
}

diag::DiagnosticBuilder &operator<<(diag::DiagnosticBuilder &Diag,
                                    const ast::SourceType &Ty) {
   return Diag << Ty.getResolvedType();
}

RecordType::RecordType(RecordDecl *record,
                       llvm::ArrayRef<QualType> TypeTemplateArgs)
   : Type(TypeID::RecordTypeID, nullptr, record->inDependentContext()),
     Rec(record), NumTypeTemplateArgs((unsigned)TypeTemplateArgs.size())
{
   std::copy(TypeTemplateArgs.begin(), TypeTemplateArgs.end(),
             const_cast<QualType*>(child_begin()));
}

RecordType::RecordType(TypeID typeID,
                       ast::RecordDecl *record,
                       llvm::ArrayRef<QualType> TypeTemplateArgs,
                       bool Dependent)
   : Type(typeID, nullptr, Dependent),
     Rec(record), NumTypeTemplateArgs((unsigned)TypeTemplateArgs.size())
{
   std::copy(TypeTemplateArgs.begin(), TypeTemplateArgs.end(),
             const_cast<QualType*>(child_begin()));
}

Type::child_iterator RecordType::child_begin() const
{
   if (auto Dep = asDependentRecordType()) {
      return reinterpret_cast<const QualType*>(Dep + 1);
   }

   return reinterpret_cast<const QualType*>(this + 1);
}

DependentRecordType::DependentRecordType(RecordDecl *record,
                                         sema::TemplateArgList *templateArgs,
                                         llvm::ArrayRef<QualType> TypeTemplateArgs)
   : RecordType(TypeID::DependentRecordTypeID, record,
                TypeTemplateArgs, true),
     templateArgs(templateArgs)
{

}

const sema::TemplateArgList& RecordType::getTemplateArgs() const
{
   return Rec->getTemplateArgs();
}

bool RecordType::hasTemplateArgs() const
{
   return Rec->isInstantiation();
}

bool RecordType::isRawEnum() const
{
   auto rec = getRecord();
   return rec && rec->isRawEnum();
}

unsigned short RecordType::getAlignment() const
{
   return Rec->getAlignment();
}

size_t RecordType::getSize() const
{
   return Rec->getSize();
}

void DependentRecordType::Profile(llvm::FoldingSetNodeID &ID,
                                  cdot::ast::RecordDecl *R,
                                  cdot::sema::TemplateArgList *templateArgs) {
   ID.AddPointer(R);
   ID.AddInteger(templateArgs->size());

   for (auto &arg : *templateArgs)
      arg.Profile(ID);
}

FunctionType::FunctionType(TypeID typeID,
                           QualType returnType,
                           llvm::ArrayRef<QualType> argTypes,
                           ExtFlags flags,
                           Type *CanonicalType,
                           bool Dependent)
   : Type(typeID, CanonicalType, Dependent),
     NumParams((unsigned)argTypes.size()), returnType(returnType)
{
   FuncBits.flags = flags;
   std::copy(argTypes.begin(), argTypes.end(),
             reinterpret_cast<QualType*>(this + 1));

   assert(&this->returnType + 1 == param_begin()
          && "bad function type layout!");
}

FunctionType::FunctionType(QualType returnType,
                           llvm::ArrayRef<QualType> argTypes,
                           ExtFlags flags,
                           Type *CanonicalType,
                           bool Dependent)
   : Type(TypeID::FunctionTypeID, CanonicalType, Dependent),
     NumParams((unsigned)argTypes.size()), returnType(returnType)
{
   FuncBits.flags = flags;
   std::copy(argTypes.begin(), argTypes.end(),
             reinterpret_cast<QualType*>(this + 1));

   assert(&this->returnType + 1 == param_begin()
          && "bad function type layout!");
}

TupleType::TupleType(llvm::ArrayRef<QualType> containedTypes,
                     Type *CanonicalType, bool Dependent)
   : Type(TypeID::TupleTypeID, CanonicalType, Dependent),
     NumTys((unsigned)containedTypes.size())
{
   std::copy(containedTypes.begin(), containedTypes.end(),
             reinterpret_cast<QualType*>(this + 1));
}

GenericType::GenericType(TemplateParamDecl *Param)
   : Type(GenericTypeID, nullptr, true),
     P(Param)
{

}

void GenericType::setCanonicalType(QualType CanonicalType)
{
   this->CanonicalType = CanonicalType;
}

llvm::StringRef GenericType::getGenericTypeName() const
{
   return P->getName();
}

QualType GenericType::getCovariance() const
{
   return P->getCovariance();
}

QualType GenericType::getContravariance() const
{
   return P->getContravariance();
}

unsigned GenericType::getIndex() const
{
   return P->getIndex();
}

bool GenericType::isVariadic() const
{
   return P->isVariadic();
}

AssociatedType::AssociatedType(AssociatedTypeDecl *AT)
   : Type(TypeID::AssociatedTypeID, nullptr,
          !AT->isImplementation()),
     AT(AT)
{

}

void AssociatedType::setCanonicalType(QualType CanonicalType)
{
   this->CanonicalType = CanonicalType;
   Bits.Dependent = CanonicalType->isDependentType();
}

QualType AssociatedType::getActualType() const
{
   return AT->getActualType();
}

TypedefType::TypedefType(TypedefDecl *td)
   : Type(TypedefTypeID, nullptr, false),
     td(td)
{

}

void TypedefType::setCanonicalType(QualType CanonicalType)
{
   this->CanonicalType = CanonicalType;
   Bits.Dependent = CanonicalType->isDependentType();
}

QualType TypedefType::getAliasedType() const
{
   return td->getOriginTy();
}

llvm::StringRef TypedefType::getAliasName() const
{
   return td->getName();
}

} // namespace cdot