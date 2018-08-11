//
// Created by Jonas Zell on 13.08.17.
//

#include "Type.h"

#include "AST/ASTContext.h"
#include "AST/Decl.h"
#include "Basic/CastKind.h"
#include "IL/Constant.h"
#include "Message/Diagnostics.h"
#include "Support/Casting.h"
#include "Support/StringSwitch.h"
#include "TypeVisitor.h"

#include <llvm/ADT/SmallString.h>
#include <sstream>

using std::string;
using namespace cdot::support;
using namespace cdot::ast;

namespace cdot {

Type::~Type()
{

}

#ifndef NDEBUG

void Type::verifyID(TypeID ID)
{
   switch (ID) {
#  define CDOT_TYPE(NAME, PARENT) case NAME##ID: break;
#  include "Types.def"

   default:
      llvm_unreachable("bad type kind");
   }
}

#endif

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

QualType Type::getBorrowedType() const
{
   assert(this->isMutableBorrowType() && "not a borrow type");
   return this->uncheckedAsMutableBorrowType()->getReferencedType();
}

QualType Type::getBoxedType() const
{
   assert(this->isBoxType() && "not a box type");
   return this->uncheckedAsBoxType()->getBoxedType();
}

QualType Type::stripReference() const
{
   if (isReferenceType())
      return getReferencedType();

   return const_cast<Type*>(this);
}

QualType Type::stripMetaType() const
{
   if (auto *Meta = asMetaType())
      return Meta->getUnderlyingType();

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
   llvm_unreachable("");
}

bool Type::isUnpopulatedType() const
{
   auto RecTy = asRecordType();
   if (!RecTy)
      return false;

   auto R = RecTy->getRecord();
   return support::isa<EnumDecl>(R) && cast<EnumDecl>(R)->isUnpopulated();
}

sema::FinalTemplateArgumentList& Type::getTemplateArgs() const
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
   case TypeID::BoxTypeID:
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

std::string Type::toDiagString() const
{
   return QualType(const_cast<Type*>(this)).toDiagString();
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
      OS << Ty->getAliasName();
   }

   void visitBuiltinType(const BuiltinType *Ty)
   {
      switch (Ty->getKind()) {
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
      else if (auto Val = Arg.getValue()) {
         OS << *Val;
      }
      else {
         OS << "<null>";
      }
   }

   void visitRecordType(const RecordType *Ty)
   {
      OS << Ty->getRecord()->getDeclName();
   }

   void visitDependentRecordType(const DependentRecordType *Ty)
   {
      auto R = Ty->getRecord();
      OS << R->getDeclName() << "<";

      auto &Args = cast<DependentRecordType>(Ty)->getTemplateArgs();
      unsigned i = 0;

      for (auto &Arg : Args) {
         if (i++ != 0) OS << ", ";
         visitTemplateArg(Arg);
      }

      OS << ">";
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


      OS << ") ";

      if (Ty->isUnsafe())
         OS << "unsafe ";
      if (Ty->isAsync())
         OS << "async";
      if (Ty->throws())
         OS << "throws ";

      OS << "-> " << Ty->getReturnType().toString();
   }

   void visitLambdaType(const LambdaType *Ty)
   {
      visitFunctionType(Ty);
   }

   void visitTupleType(const TupleType *Ty)
   {
      OS << "(";

      unsigned i = 0;
      for (auto &Cont : Ty->getContainedTypes()) {
         if (i++ != 0) OS << ", ";
         OS << Cont;
      }

      OS << ")";
   }

   void visitArrayType(const ArrayType *Ty)
   {
      OS << "[" << Ty->getElementType() << "; "
         << Ty->getNumElements() << "]";
   }

   void visitDependentSizeArrayType(const DependentSizeArrayType *Ty)
   {
      OS << "[" << Ty->getElementType() << "; ?]";
   }

   void visitInferredSizeArrayType(const InferredSizeArrayType *Ty)
   {
      OS << "[" << Ty->getElementType() << "; ?]";
   }

   void visitMetaType(const MetaType *Ty)
   {
      OS << "MetaType<" << Ty->getUnderlyingType() << ">";
   }

   void visitGenericType(const GenericType *Ty)
   {
      OS << Ty->getParam()->getDeclName();
      if (Ty->isVariadic())
         OS << "...";
   }

   void visitAssociatedType(const AssociatedType *Ty)
   {
      OS << Ty->getDecl()->getDeclName();
   }

   void visitDependentNameType(const DependentNameType *Ty)
   {
      OS << Ty->getNameSpec();
   }

   void visitPointerType(const PointerType *Ty)
   {
      auto pointee = Ty->getPointeeType();
      if (pointee->isVoidType())
         OS << "UnsafeRawPtr";
      else
         OS << "UnsafePtr<" << pointee << ">";
   }

   void visitMutablePointerType(const MutablePointerType *Ty)
   {
      auto pointee = Ty->getPointeeType();
      if (pointee->isVoidType())
         OS << "UnsafeMutableRawPtr";
      else
         OS << "UnsafeMutablePtr<" << pointee << ">";
   }

   void visitReferenceType(const ReferenceType *Ty)
   {
      OS << "&" << Ty->getReferencedType();
   }

   void visitMutableBorrowType(const MutableBorrowType *Ty)
   {
      OS << "&mut " << Ty->getBorrowedType();
   }

   void visitBoxType(const BoxType *Ty)
   {
      OS << "Box<" << Ty->getBoxedType() << ">";
   }

   void visitTokenType(const TokenType *Ty)
   {
      OS << "token";
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

   void visitGenericType(const GenericType *Ty)
   {
      auto Cov = cast<GenericType>(Ty)->getCovariance();
      OS << Ty->getGenericTypeName();

      if (!Cov->isUnknownAnyType()) {
         OS << ": ";
         visit(Cov);
      }
   }

   void visitTypedefType(const TypedefType *Ty)
   {
      auto TD = cast<TypedefType>(Ty)->getTypedef();
      OS << TD->getDeclName() << " (aka ";
      visit(Ty->getAliasedType());
      OS << ")";
   }

   void visitAssociatedType(const AssociatedType *Ty)
   {
      auto AT = cast<AssociatedType>(Ty)->getDecl();
      OS << AT->getDeclName();

      if (AT->isImplementation()) {
         OS << " (aka ";
         visit(AT->getActualType().getResolvedType());
         OS << ")";
      }
   }

   void visitReferenceType(const ReferenceType *Ty)
   {
      OS << "&";
      visit(Ty->getReferencedType());
   }

   void visitMutableReferenceType(const MutableReferenceType *Ty)
   {
      OS << "&mut ";
      visit(Ty->getReferencedType());
   }

   void visitFunctionType(const FunctionType *Ty)
   {
      if (!isa<LambdaType>(Ty))
         OS << "@thin ";

      auto argTypes = Ty->getParamTypes();
      auto paramInfo = Ty->getParamInfo();

      OS << "(";

      size_t i = 0;
      for (const auto& arg : argTypes) {
         if (i != 0) OS << ", ";

         switch (paramInfo[i].getConvention()) {
         case ArgumentConvention::Owned: OS << "owned "; break;
         case ArgumentConvention::Borrowed: OS << "borrow "; break;
         case ArgumentConvention::MutableRef: OS << "mut ref "; break;
         case ArgumentConvention::ImmutableRef: OS << "ref "; break;
         default:
            llvm_unreachable("bad argument convention");
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

   void visitLambdaType(const LambdaType *Ty)
   {
      visitFunctionType(Ty);
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

std::string QualType::toDiagString() const
{
   std::string str;
   llvm::raw_string_ostream OS(str);

   DiagTypePrinter(OS).visit(*this);
   return OS.str();
}

MutablePointerType::MutablePointerType(QualType pointeeType,
                                       Type *CanonicalType)
   : PointerType(MutablePointerTypeID, pointeeType, CanonicalType)
{

}

MutableReferenceType::MutableReferenceType(QualType referencedType,
                                           Type *CanonicalType)
   : ReferenceType(MutableReferenceTypeID, referencedType, CanonicalType)
{

}

MutableReferenceType::MutableReferenceType(TypeID ID,
                                           QualType referencedType,
                                           Type *CanonicalType)
   : ReferenceType(ID, referencedType, CanonicalType)
{

}

MutableBorrowType::MutableBorrowType(QualType borrowedType,
                                     Type *CanonicalType)
   : MutableReferenceType(MutableBorrowTypeID, borrowedType, CanonicalType)
{

}

BoxType::BoxType(QualType BoxedTy, Type *CanonicalTy)
   : Type(BoxTypeID, CanonicalTy, BoxedTy->isDependentType()),
     BoxedTy(BoxedTy)
{

}

RecordType::RecordType(RecordDecl *record,
                       llvm::ArrayRef<QualType> TypeTemplateArgs)
   : Type(TypeID::RecordTypeID, nullptr,
          record->isTemplateOrInTemplate()),
     Rec(record), NumTypeTemplateArgs((unsigned)TypeTemplateArgs.size())
{
   std::copy(TypeTemplateArgs.begin(), TypeTemplateArgs.end(),
             const_cast<QualType*>(child_begin()));
}

RecordType::RecordType(TypeID typeID,
                       ast::RecordDecl *record,
                       bool Dependent)
   : Type(typeID, nullptr, Dependent),
     Rec(record), NumTypeTemplateArgs(0)
{

}

Type::child_iterator RecordType::child_begin() const
{
   if (auto Dep = asDependentRecordType()) {
      return reinterpret_cast<const QualType*>(Dep + 1);
   }

   return reinterpret_cast<const QualType*>(this + 1);
}

DependentRecordType::DependentRecordType(RecordDecl *record,
                                         sema::FinalTemplateArgumentList
                                                                  *templateArgs,
                                         llvm::ArrayRef<QualType>
                                                               TypeTemplateArgs)
   : RecordType(TypeID::DependentRecordTypeID, record, true),
     templateArgs(templateArgs)
{
   NumTypeTemplateArgs = (unsigned)TypeTemplateArgs.size();
   std::copy(TypeTemplateArgs.begin(), TypeTemplateArgs.end(),
             reinterpret_cast<QualType*>(this + 1));
}

sema::FinalTemplateArgumentList& RecordType::getTemplateArgs() const
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
                                  ast::RecordDecl *R,
                                  sema::FinalTemplateArgumentList
                                                               *templateArgs) {
   ID.AddPointer(R);
   templateArgs->Profile(ID);
}

FunctionType::FunctionType(TypeID typeID,
                           QualType returnType,
                           llvm::ArrayRef<QualType> argTypes,
                           llvm::ArrayRef<ParamInfo> paramInfo,
                           ExtFlags flags,
                           Type *CanonicalType,
                           bool Dependent)
   : Type(typeID, CanonicalType, Dependent),
     NumParams((unsigned)argTypes.size()), returnType(returnType)
{
   assert(argTypes.size() == paramInfo.size() && "didn't provide parameter "
                                                 "info for every parameter!");

   FuncBits.flags = flags;
   std::copy(argTypes.begin(), argTypes.end(),
             reinterpret_cast<QualType*>(this + 1));

   assert(&this->returnType + 1 == param_begin()
          && "bad function type layout!");

   std::copy(paramInfo.begin(), paramInfo.end(),
             reinterpret_cast<ParamInfo*>(
                reinterpret_cast<QualType*>(this + 1) + NumParams));
}

FunctionType::FunctionType(QualType returnType,
                           llvm::ArrayRef<QualType> argTypes,
                           llvm::ArrayRef<ParamInfo> paramInfo,
                           ExtFlags flags,
                           Type *CanonicalType,
                           bool Dependent)
   : Type(TypeID::FunctionTypeID, CanonicalType, Dependent),
     NumParams((unsigned)argTypes.size()), returnType(returnType)
{
   assert(argTypes.size() == paramInfo.size() && "didn't provide parameter "
                                                 "info for every parameter!");

   FuncBits.flags = flags;
   std::copy(argTypes.begin(), argTypes.end(),
             reinterpret_cast<QualType*>(this + 1));

   assert(&this->returnType + 1 == param_begin()
          && "bad function type layout!");

   std::copy(paramInfo.begin(), paramInfo.end(),
             reinterpret_cast<ParamInfo*>(
                reinterpret_cast<QualType*>(this + 1) + NumParams));
}

FunctionType::ParamInfo::ParamInfo()
   : Conv(ArgumentConvention::Borrowed)
{

}

void FunctionType::ParamInfo::Profile(llvm::FoldingSetNodeID &ID) const
{
   ID.AddInteger((char)Conv);
}

bool FunctionType::ParamInfo::operator==(const ParamInfo &I) const
{
   return Conv == I.Conv;
}

TupleType::TupleType(llvm::ArrayRef<QualType> containedTypes,
                     Type *CanonicalType, bool Dependent)
   : Type(TupleTypeID, CanonicalType, Dependent),
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
   this->CanonicalType = CanonicalType->getCanonicalType();
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
   : Type(AssociatedTypeID, nullptr, !AT->isImplementation()),
     AT(AT)
{
   if (AT->isImplementation() && AT->getActualType().isResolved()) {
      Bits.Dependent = AT->getActualType()->isDependentType();
      this->CanonicalType = AT->getActualType().operator QualType();
   }
}

void AssociatedType::setCanonicalType(QualType CanonicalType)
{
   this->CanonicalType = CanonicalType->getCanonicalType();
   Bits.Dependent = CanonicalType->isDependentType();
}

QualType AssociatedType::getActualType() const
{
   if (!AT->isImplementation())
      return QualType();

   return AT->getActualType();
}

TypedefType::TypedefType(AliasDecl *td)
   : Type(TypedefTypeID, nullptr, false),
     td(td)
{
   CanonicalType = td->getType()->asMetaType()->getUnderlyingType();
   Bits.Dependent = CanonicalType->isDependentType();
}

void TypedefType::setCanonicalType(QualType CanonicalType)
{
   this->CanonicalType = CanonicalType;
   Bits.Dependent = CanonicalType->isDependentType();
}

QualType TypedefType::getAliasedType() const
{
   return td->getType()->asMetaType()->getUnderlyingType();
}

StringRef TypedefType::getAliasName() const
{
   return td->getName();
}

DependentNameType::DependentNameType(NestedNameSpecifierWithLoc *NameSpec)
   : Type(DependentNameTypeID, nullptr, true), NameSpec(NameSpec)
{

}

NestedNameSpecifier* DependentNameType::getNameSpec() const
{
   return NameSpec->getNameSpec();
}

void DependentNameType::Profile(llvm::FoldingSetNodeID &ID)
{
   Profile(ID, NameSpec);
}

void DependentNameType::Profile(llvm::FoldingSetNodeID &ID,
                                NestedNameSpecifierWithLoc *Name) {
   ID.AddPointer(Name);

   auto SourceLocs = Name->getSourceRanges();
   for (auto Loc : SourceLocs) {
      ID.AddInteger(Loc.getStart().getOffset());
      ID.AddInteger(Loc.getEnd().getOffset());
   }
}

} // namespace cdot