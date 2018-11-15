//
// Created by Jonas Zell on 24.08.18.
//

#include "Query.h"

#include "AST/Decl.h"
#include "AST/TypeBuilder.h"
#include "IL/Constants.h"
#include "Sema/SemaPass.h"
#include "QueryContext.h"

using namespace cdot;
using namespace cdot::ast;
using namespace cdot::diag;
using namespace cdot::sema;
using namespace cdot::support;

QueryResult IsEquatableQuery::run()
{
   Status S = Done;
   if (T->isDependentType()) {
      S = Dependent;
   }

   bool Result;
   switch (T->getTypeID()) {
   default:
      Result = true;
      break;
   case Type::LambdaTypeID:
      Result = false;
      break;
   case Type::ArrayTypeID:
   case Type::TupleTypeID:
      Result = true;

      for (auto SubTy : T->children()) {
         // Check whether or not the subtype is equatable.
         if (auto Err = QC.IsEquatable(Result, SubTy)) {
            return Query::finish(Err);
         }

         if (!Result) {
            break;
         }
      }

      break;
   case Type::ExistentialTypeID: {
      Result = false;

      auto *Eq = QC.Sema->getEquatableDecl();
      for (auto E : T->asExistentialType()->getExistentials()) {
         if (E->getRecord() == Eq) {
            Result = true;
            break;
         }
      }

      break;
   }
   case Type::RecordTypeID:
   case Type::DependentRecordTypeID:
      Result = true;

      if (!isa<ClassDecl>(T->getRecord())) {
         if (auto Err = QC.ConformsTo(Result,
                                      QC.Context.getRecordType(T->getRecord()),
                                      QC.Sema->getEquatableDecl())) {
            return Query::finish(Err);
         }
      }

      break;
   }

   return finish(Result, S);
}

QueryResult IsMoveOnlyQuery::run()
{
   Status S = Done;
   if (T->isDependentType()) {
      S = Dependent;
   }

   bool Result;
   switch (T->getTypeID()) {
   default:
      Result = false;
      break;
   case Type::LambdaTypeID:
      Result = true;
      break;
   case Type::ArrayTypeID:
   case Type::TupleTypeID:
      Result = false;

      for (auto SubTy : T->children()) {
         // Check whether or not the subtype is equatable.
         if (auto Err = QC.IsMoveOnly(Result, SubTy)) {
            return Query::finish(Err);
         }

         if (Result) {
            break;
         }
      }

      break;
   case Type::ExistentialTypeID: {
      Result = false;

      auto *MoveOnly = QC.Sema->getMoveOnlyDecl();
      for (auto E : T->asExistentialType()->getExistentials()) {
         if (E->getRecord() == MoveOnly) {
            Result = true;
            break;
         }
      }

      break;
   }
   case Type::RecordTypeID:
   case Type::DependentRecordTypeID:
      Result = false;

      if (!isa<ClassDecl>(T->getRecord())) {
         if (auto Err = QC.CheckBuiltinConformances(T->getRecord())) {
            return Query::finish(Err);
         }
         if (auto Err = QC.ConformsTo(Result,
                                      QC.Context.getRecordType(T->getRecord()),
                                      QC.Sema->getMoveOnlyDecl())) {
            return Query::finish(Err);
         }
      }

      break;
   }

   return finish(Result, S);
}

QueryResult IsCopyableQuery::run()
{
   bool Result;
   if (auto Err = QC.IsMoveOnly(Result, T)) {
      return Query::finish(Err);
   }

   return finish(!Result);
}

QueryResult IsImplicitlyCopyableQuery::run()
{
   Status S = Done;
   if (T->isDependentType()) {
      S = Dependent;
   }

   bool Result;
   switch (T->getTypeID()) {
   default:
      Result = false;
      break;
   case Type::BuiltinTypeID:
   case Type::PointerTypeID:
   case Type::MutablePointerTypeID:
   case Type::ReferenceTypeID:
   case Type::MutableReferenceTypeID:
   case Type::FunctionTypeID:
   case Type::MetaTypeID:
      Result = true;
      break;
   case Type::ArrayTypeID:
   case Type::TupleTypeID:
      Result = true;

      for (auto SubTy : T->children()) {
         // Check whether or not the subtype is equatable.
         if (auto Err = QC.IsImplicitlyCopyable(Result, SubTy)) {
            return Query::finish(Err);
         }

         if (!Result) {
            break;
         }
      }

      break;
   case Type::RecordTypeID:
   case Type::DependentRecordTypeID:
      Result = true;

      if (!isa<ClassDecl>(T->getRecord())) {
         if (auto Err = QC.CheckBuiltinConformances(T->getRecord())) {
            return Query::finish(Err);
         }
         if (auto Err = QC.ConformsTo(Result,
                                      QC.Context.getRecordType(T->getRecord()),
                                      QC.Sema->getImplicitlyCopyableDecl())) {
            return Query::finish(Err);
         }
      }

      break;
   }

   return finish(Result, S);
}

QueryResult NeedsRetainOrReleaseQuery::run()
{
   Status S = Done;
   if (T->isDependentType()) {
      S = Dependent;
   }

   bool Result;
   switch (T->getTypeID()) {
   default:
      Result = false;
      break;
   case Type::ArrayTypeID:
   case Type::TupleTypeID:
      Result = false;

      for (auto SubTy : T->children()) {
         // Check whether or not the subtype is equatable.
         if (auto Err = QC.NeedsRetainOrRelease(Result, SubTy)) {
            return Query::finish(Err);
         }

         if (Result) {
            break;
         }
      }

      break;
   case Type::BoxTypeID:
   case Type::LambdaTypeID:
      Result = true;
      break;
   case Type::RecordTypeID:
   case Type::DependentRecordTypeID: {
      if (auto Err = QC.CheckBuiltinConformances(T->getRecord())) {
         return Query::finish(Err);
      }

      Result = QC.RecordMeta[T->getRecord()].NeedsRetainOrRelease;
      break;
   }
   }

   return finish(Result, S);
}

QueryResult NeedsStructReturnQuery::run()
{
   Status S = Done;
   if (T->isDependentType()) {
      S = Dependent;
   }

   bool Result;
   switch (T->getTypeID()) {
   default:
      Result = false;
      break;
   case Type::AssociatedTypeID:
   case Type::ExistentialTypeID:
      Result = true;
      break;
   case Type::RecordTypeID: {
      auto rec = T->getRecord();
      switch (rec->getKind()) {
      case Decl::EnumDeclID:
         Result = !cast<EnumDecl>(rec)->isRawEnum();
         break;
      case Decl::StructDeclID:
      case Decl::UnionDeclID:
      case Decl::ProtocolDeclID:
         Result = true;
         break;
      case Decl::ClassDeclID:
         Result = false;
         break;
      default:
         llvm_unreachable("bad record kind!");
      }

      break;
   }
   case Type::TupleTypeID:
      Result = T->asTupleType()->getArity() > 1;
      break;
   case Type::ArrayTypeID:
   case Type::MetaTypeID:
   case Type::BoxTypeID:
      Result = true;
      break;
   }

   return finish(Result, S);
}

QueryResult NeedsDeinitilizationQuery::run()
{
   Status S = Done;
   if (T->isDependentType()) {
      S = Dependent;
   }

   bool Result;
   switch (T->getTypeID()) {
   default:
      Result = false;
      break;
   case Type::ArrayTypeID:
   case Type::TupleTypeID:
      Result = false;

      for (auto SubTy : T->children()) {
         // Check whether or not the subtype is equatable.
         if (auto Err = QC.NeedsDeinitilization(Result, SubTy)) {
            return Query::finish(Err);
         }

         if (Result) {
            break;
         }
      }

      break;
   case Type::BoxTypeID:
   case Type::LambdaTypeID:
   case Type::RecordTypeID:
   case Type::ExistentialTypeID:
      Result = true;
      break;
   }

   return finish(Result, S);
}

QueryResult IsTriviallyCopyableQuery::run()
{
   Status S = Done;
   if (T->isDependentType()) {
      S = Dependent;
   }

   bool Result;
   switch (T->getTypeID()) {
   case Type::BuiltinTypeID:
   case Type::PointerTypeID:
   case Type::MutablePointerTypeID:
   case Type::ReferenceTypeID:
   case Type::MutableReferenceTypeID:
   case Type::MutableBorrowTypeID:
   case Type::FunctionTypeID:
   case Type::MetaTypeID:
      Result = true;
      break;
   case Type::LambdaTypeID:
      Result = false;
      break;
   case Type::ArrayTypeID:
   case Type::TupleTypeID: {
      Result = true;

      for (auto SubTy : T->children()) {
         if (auto Err = QC.IsTriviallyCopyable(Result, SubTy)) {
            return Query::finish(Err);
         }

         if (!Result) {
            break;
         }
      }

      break;
   }
   case Type::RecordTypeID:
   case Type::DependentRecordTypeID: {
      if (T->isDependentType()) {
         return finish(true, Dependent);
      }

      if (isa<ast::ClassDecl>(T->getRecord())) {
         Result = false;
      }
      else {
         if (auto Err = QC.CheckBuiltinConformances(T->getRecord())) {
            return Query::finish(Err);
         }

         auto &Meta = QC.RecordMeta[T->getRecord()];
         if (Meta.CopyFn) {
            Result = false;
         }
         else if (T->getRecord()->isInvalid()) {
            Result = true;
         }
         else {
            Result = Meta.IsTriviallyCopyable;
         }
      }

      break;
   }
   default:
      Result = false;
      break;
   }

   return finish(Result, S);
}

QueryResult IsUnpopulatedQuery::run()
{
   Status S = Done;
   if (T->isDependentType()) {
      S = Dependent;
   }

   bool Result;
   switch (T->getTypeID()) {
   default:
      Result = false;
      break;
   case Type::PointerTypeID:
   case Type::MutablePointerTypeID:
   case Type::ReferenceTypeID:
   case Type::MutableReferenceTypeID:
   case Type::MutableBorrowTypeID:
      QC.IsUnpopulated(Result, *T->child_begin());
      break;
   case Type::ArrayTypeID:
   case Type::TupleTypeID: {
      Result = false;

      for (auto SubTy : T->children()) {
         if (auto Err = QC.IsUnpopulated(Result, SubTy)) {
            return Query::finish(Err);
         }

         if (Result) {
            break;
         }
      }

      break;
   }
   case Type::RecordTypeID:
   case Type::DependentRecordTypeID: {
      if (T->isDependentType()) {
         return finish(false, Dependent);
      }

      if (auto *E = dyn_cast<EnumDecl>(T->getRecord())) {
         if (E->isInstantiation()) {
            Result =
               cast<EnumDecl>(E->getSpecializedTemplate())->isUnpopulated();
         }
         else {
            Result = E->decl_empty<EnumCaseDecl>();
         }
      }
      else {
         Result = false;
      }

      break;
   }
   }

   return finish(Result, S);
}

QueryResult IsPersistableQuery::run()
{
   switch (T->getTypeID()) {
   default:
      return finish(true);
   case Type::PointerTypeID:
   case Type::MutablePointerTypeID: {
      if (T->getPointeeType()->isInt8Ty()) {
         return finish(true);
      }

      return finish(false);
   }
   case Type::ReferenceTypeID:
   case Type::MutableReferenceTypeID:
   case Type::MutableBorrowTypeID:
      return finish(false);
   case Type::ArrayTypeID:
   case Type::TupleTypeID: {
      bool IsPersistable = true;
      for (auto SubTy : T->children()) {
         if (!QC.IsPersistable(IsPersistable, SubTy) && !IsPersistable) {
            return finish(false);
         }
      }

      return finish(true);
   }
   case Type::RecordTypeID:
   case Type::DependentRecordTypeID: {
      auto R = T->getRecord();

      RecordDecl *Decl;
      if (QC.GetBuiltinRecord(Decl, GetBuiltinRecordQuery::String)) {
         return finish(true, DoneWithError);
      }
      if (R == Decl) {
         return finish(true);
      }

      if (QC.GetBuiltinRecord(Decl, GetBuiltinRecordQuery::Array)) {
         return finish(true, DoneWithError);
      }
      if (R == Decl) {
         return finish(true);
      }
      if (R->isInstantiation() && R->getSpecializedTemplate() == Decl) {
         return finish(true);
      }

      // FIXME Persistable protocol
      return finish(true);
   }
   }
}

QueryResult GetTypeSizeQuery::run()
{
   auto &TI = QC.Sema->Context.getTargetInfo();
   switch (T->getTypeID()) {
   case Type::RecordTypeID: {
      auto R = T->getRecord();
      if (R->isClass()) {
         return finish(TI.getPointerSizeInBytes());
      }

      LLVM_FALLTHROUGH;
   }
   default: {
      unsigned Stride;
      if (auto Err = QC.GetTypeStride(Stride, T)) {
         return Query::finish(Err);
      }

      return finish(Stride);
   }
   }
}

QueryResult GetTypeStrideQuery::run()
{
   auto &TI = QC.Sema->Context.getTargetInfo();
   switch (T->getTypeID()) {
   case Type::BuiltinTypeID: {
      using BK = Type::BuiltinKind;
      switch (T->asBuiltinType()->getKind()) {
      case BK::i1: case BK::i8: case BK::u8: return finish(1);
      case BK::u16: case BK::i16: return finish(2);
      case BK::u32: case BK::i32: return finish(4);
      case BK::u64: case BK::i64: return finish(8);
      case BK::u128: case BK::i128: return finish(16);
      case BK::f16: return finish(2);
      case BK::f32: return finish(4);
      case BK::f64: return finish(8);
      case BK::f80: return finish(16);
      case BK::f128: return finish(16);
      case BK::Void: return finish(0);
      default:
         llvm_unreachable("bad builtin type kind!");
      }
   }
   case Type::PointerTypeID:
   case Type::ReferenceTypeID:
   case Type::MutablePointerTypeID:
   case Type::MutableReferenceTypeID:
   case Type::MutableBorrowTypeID:
   case Type::FunctionTypeID:
   case Type::LambdaTypeID:
      return finish(TI.getPointerSizeInBytes());
   case Type::ArrayTypeID: {
      auto Arr = T->uncheckedAsArrayType();
      unsigned ElementStride;
      if (auto Err = QC.GetTypeStride(ElementStride, Arr->getElementType())) {
         return Query::finish(Err);
      }

      return finish(Arr->getNumElements() * ElementStride);
   }
   case Type::TupleTypeID: {
      unsigned size = 0;
      for (auto &ElTy : T->uncheckedAsTupleType()->getContainedTypes()) {
         unsigned ElementStride;
         if (auto Err = QC.GetTypeStride(ElementStride, ElTy)) {
            return Query::finish(Err);
         }

         size += ElementStride;
      }

      return finish(size);
   }
   case Type::ExistentialTypeID:
   case Type::GenericTypeID: {
      RecordDecl *EC;
      if (auto Err = QC.GetBuiltinRecord(EC,
                                 GetBuiltinRecordQuery::ExistentialContainer)) {
         return Query::finish(Err);
      }

      unsigned Stride;
      if (auto Err = QC.GetTypeStride(Stride, QC.Context.getRecordType(EC))) {
         return Query::finish(Err);
      }

      return finish(Stride);
   }
   case Type::RecordTypeID:
   case Type::DependentRecordTypeID: {
      unsigned Size;
      if (auto Err = QC.CalculateRecordSize(Size, T->getRecord())) {
         return Query::finish(Err);
      }

      return finish(Size);
   }
   case Type::MetaTypeID:
      return finish(TI.getPointerSizeInBytes());
   case Type::BoxTypeID:
      return finish(BoxType::MemberCount * TI.getPointerSizeInBytes());
   default:
      llvm_unreachable("type does not have a size!");
   }
}

QueryResult GetTypeAlignmentQuery::run()
{
   auto &TI = QC.Sema->Context.getTargetInfo();
   switch (T->getTypeID()) {
   case Type::BuiltinTypeID: {
      unsigned Size;
      if (auto Err = QC.GetTypeSize(Size, T)) {
         return Query::finish(Err);
      }

      return finish((unsigned short) Size);
   }
   case Type::PointerTypeID:
   case Type::MutablePointerTypeID:
   case Type::ReferenceTypeID:
   case Type::MutableReferenceTypeID:
   case Type::MutableBorrowTypeID:
   case Type::LambdaTypeID:
   case Type::FunctionTypeID:
   case Type::MetaTypeID:
   case Type::BoxTypeID:
      return finish(TI.getPointerAlignInBytes());
   case Type::ArrayTypeID: {
      unsigned short ElementAlign;
      if (auto Err = QC.GetTypeAlignment(ElementAlign,
                                         T->asArrayType()->getElementType())) {
         return Query::finish(Err);
      }

      return finish(ElementAlign);
   }
   case Type::TupleTypeID: {
      unsigned short align = 1;
      for (auto &ElTy : T->uncheckedAsTupleType()->getContainedTypes()) {
         unsigned short ElementAlign;
         if (auto Err = QC.GetTypeAlignment(ElementAlign, ElTy)) {
            return Query::finish(Err);
         }

         if (ElementAlign > align)
            align = ElementAlign;
      }

      return finish(align);
   }
   case Type::ExistentialTypeID:
   case Type::GenericTypeID: {
      RecordDecl *EC;
      if (auto Err = QC.GetBuiltinRecord(EC,
                                 GetBuiltinRecordQuery::ExistentialContainer)) {
         return Query::finish(Err);
      }

      unsigned short Align;
      if (auto Err = QC.GetTypeAlignment(Align, QC.Context.getRecordType(EC))) {
         return Query::finish(Err);
      }

      return finish(Align);
   }
   case Type::RecordTypeID:
   case Type::DependentRecordTypeID: {
      unsigned Size;
      if (auto Err = QC.CalculateRecordSize(Size, T->getRecord())) {
         return Query::finish(Err);
      }

      return finish(QC.RecordMeta[T->getRecord()].Alignment);
   }
   default:
      llvm_unreachable("bad type kind!");
   }
}

QueryResult IsCovariantQuery::run()
{
   if (T->isDependentType()) {
      return finish(true, Dependent);
   }

   if (Covar->isUnknownAnyType()) {
      return finish(true);
   }

   if (T->getCanonicalType() == Covar->getCanonicalType()) {
      return finish(true);
   }

   auto ConvSeq = QC.Sema->getConversionSequence(T, Covar);
   return finish(ConvSeq.isValid());
}

QueryResult IsContravariantQuery::run()
{
   llvm_unreachable("TODO");
}

namespace {

template <class SubClass>
class AssociatedTypeSubstVisitorBase: public TypeBuilder<SubClass>
{
   /// \brief The 'Self' type used to substitute abstract associated types.
   QualType Self;

public:
   AssociatedTypeSubstVisitorBase(SemaPass &SP, QualType Self, SourceRange SR)
      : TypeBuilder<SubClass>(SP, SR), Self(Self)
   {}

   QualType visitAssociatedType(AssociatedType *T)
   {
      if (T->getDecl()->isImplementation()) {
         return T;
      }

      if (!Self) {
         return T;
      }

      RecordDecl *DC;
      if (auto *Outer = T->getOuterAT()) {
         auto OuterImpl = this->visit(Outer);
         DC = OuterImpl->getRecord();
      }
      else {
         DC = Self->getRecord();
      }

      QueryContext &QC = this->SP.QC;
      AssociatedTypeDecl *AT;

      if (QC.GetAssociatedType(AT, DC, T->getDecl()->getDeclName(),
                               DC->getExtensions())) {
         return T;
      }

      if (!AT || AT->isDefault()) {
         assert(isa<ProtocolDecl>(DC) && "associated type not implemented!");
         return T;
      }

      auto *Inst = QC.template InstantiateTemplateMember<AssociatedTypeDecl>(
         AT, DC);

      if (Inst != AT) {
         if (QC.PrepareDeclInterface(Inst)) {
            return T;
         }
      }

      return this->Ctx.getAssociatedType(Inst);
   }
};

class AssociatedTypeSubstVisitor:
      public AssociatedTypeSubstVisitorBase<AssociatedTypeSubstVisitor> {
public:
   AssociatedTypeSubstVisitor(SemaPass &SP, QualType Self, SourceRange SR)
      : AssociatedTypeSubstVisitorBase(SP, Self, SR)
   {}
};

} // anonymous namespace

QueryResult SubstAssociatedTypesQuery::run()
{
   return finish(AssociatedTypeSubstVisitor(sema(), Self, SR).visit(T));
}

namespace {

template<class TemplateArgList>
class GenericTypeSubstVisitor:
      public TypeBuilder<GenericTypeSubstVisitor<TemplateArgList>> {
   /// The template arguments we are substituting with.
   const TemplateArgList &TemplateArgs;

public:
   GenericTypeSubstVisitor(SemaPass &SP,
                           const TemplateArgList &TemplateArgs,
                           SourceRange SR)
      : TypeBuilder<GenericTypeSubstVisitor<TemplateArgList>>(SP, SR),
        TemplateArgs(TemplateArgs)
   {}

   QualType visitGenericType(GenericType *T)
   {
      const TemplateArgument *Arg = TemplateArgs.getArgForParam(T->getParam());
      if (!Arg || Arg->isNull()) {
         return T;
      }

      assert(!Arg->isValue() && "should not appear in type position!");
      if (Arg->isVariadic()) {
         return T;
      }

      return Arg->getType();
   }

   QualType visitDependentSizeArrayType(DependentSizeArrayType *T)
   {
      auto Ident = dyn_cast<IdentifierRefExpr>(T->getSizeExpr()->getExpr());
      if (!Ident || Ident->getKind() != IdentifierKind::TemplateParam)
         return T;

      auto Param = Ident->getTemplateParam();

      // have to lookup via name because the address might change if an
      // outer record is instantiated
      auto *Arg = TemplateArgs.getNamedArg(Param->getDeclName());
      if (!Arg || Arg->isNull())
         return T;

      assert(Arg->isValue() && "used type for array element size?");
      assert(isa<il::ConstantInt>(Arg->getValue()) && "invalid array size");

      return this->Ctx.getArrayType(this->visit(T->getElementType()),
                                    cast<il::ConstantInt>(Arg->getValue())
                                       ->getZExtValue());
   }

   NestedNameSpecifier *visitNestedNameSpecifier(NestedNameSpecifier *Name,
                                                 bool &Dependent) {
      if (!Name)
         return nullptr;

      NestedNameSpecifier *Copy = nullptr;
      switch (Name->getKind()) {
      case NestedNameSpecifier::Type:
         Copy = NestedNameSpecifier::Create(this->Ctx.getDeclNameTable(),
                                            this->visit(Name->getType()),
                                            this->visitNestedNameSpecifier(
                                               Name->getPrevious(),
                                               Dependent));

         break;
      case NestedNameSpecifier::Identifier:
         Copy = NestedNameSpecifier::Create(this->Ctx.getDeclNameTable(),
                                            Name->getIdentifier(),
                                            this->visitNestedNameSpecifier(
                                               Name->getPrevious(),
                                               Dependent));

         break;
      case NestedNameSpecifier::Namespace:
         Copy = NestedNameSpecifier::Create(this->Ctx.getDeclNameTable(),
                                            Name->getNamespace(),
                                            this->visitNestedNameSpecifier(
                                               Name->getPrevious(),
                                               Dependent));

         break;
      case NestedNameSpecifier::TemplateParam: {
         if (auto Arg = TemplateArgs.getArgForParam(Name->getParam())) {
            if (Arg->isType()) {
               if (!Arg->isVariadic()) {
                  Copy = NestedNameSpecifier::Create(this->Ctx.getDeclNameTable(),
                                                     Arg->getType(),
                                                     visitNestedNameSpecifier(
                                                        Name->getPrevious(),
                                                        Dependent));
               }
            }
         }

         if (!Copy)
            Copy = NestedNameSpecifier::Create(this->Ctx.getDeclNameTable(),
                                               Name->getParam(),
                                               this->visitNestedNameSpecifier(
                                                  Name->getPrevious(),
                                                  Dependent));

         break;
      }
      case NestedNameSpecifier::AssociatedType: {
         Copy = NestedNameSpecifier::Create(this->Ctx.getDeclNameTable(),
                                            Name->getAssociatedType(),
                                            this->visitNestedNameSpecifier(
                                               Name->getPrevious(),
                                               Dependent));

         break;
      }
      case NestedNameSpecifier::Module: {
         Copy = NestedNameSpecifier::Create(this->Ctx.getDeclNameTable(),
                                            Name->getModule(),
                                            this->visitNestedNameSpecifier(
                                               Name->getPrevious(),
                                               Dependent));

         break;
      }
      }

      Dependent |= Copy->isDependent();
      return Copy;
   }

   QualType visitDependentNameType(DependentNameType *T)
   {
      bool Dependent = false;
      auto *Name = this->visitNestedNameSpecifier(T->getNameSpec(), Dependent);

      if (Dependent) {
         if (Name != T->getNameSpec()) {
            auto *WithLoc = NestedNameSpecifierWithLoc::Create(
               this->Ctx.getDeclNameTable(), Name,
               T->getNameSpecWithLoc()->getSourceRanges());

            return this->Ctx.getDependentNameType(WithLoc);
         }

         return T;
      }

      auto *WithLoc = NestedNameSpecifierWithLoc::Create(
         this->Ctx.getDeclNameTable(), Name,
         T->getNameSpecWithLoc()->getSourceRanges());

      // Resolve the dependent name to a type.
      QualType Ty;
      if (this->SP.QC.ResolveNestedNameSpecToType(Ty, WithLoc)) {
         return T;
      }

      return Ty;
   }

   QualType visitFunctionType(FunctionType *T)
   {
      SmallVector<QualType, 4> ParamTys;
      SmallVector<FunctionType::ParamInfo, 4> Info;

      auto GivenInfo = T->getParamInfo();
      unsigned i = 0;

      for (QualType Ty : T->getParamTypes()) {
         auto *TA = Ty->asGenericType();
         if (!TA || !TA->isVariadic()) {
            ParamTys.push_back(this->visit(Ty));
            Info.push_back(GivenInfo[i++]);
            continue;
         }

         auto Arg = TemplateArgs.getArgForParam(TA->getParam());
         if (!Arg || !Arg->isFrozen()) {
            ParamTys.push_back(this->visit(Ty));
            Info.push_back(GivenInfo[i++]);
            continue;
         }

         for (auto &VA : Arg->getVariadicArgs()) {
            ParamTys.push_back(this->visit(VA.getNonCanonicalType()));
            Info.push_back(GivenInfo[i]);
         }

         i++;
      }

      return this->Ctx.getFunctionType(this->visit(T->getReturnType()),
                                       ParamTys, Info, T->getRawFlags(),
                                       T->isLambdaType());
   }

   QualType visitLambdaType(LambdaType *T)
   {
      return visitFunctionType(T);
   }

   QualType visitTupleType(TupleType *T)
   {
      SmallVector<QualType, 4> ResolvedTys;
      for (QualType Ty : T->getContainedTypes()) {
         auto *TA = Ty->asGenericType();
         if (!TA || !TA->isVariadic()) {
            ResolvedTys.push_back(this->visit(Ty));
            continue;
         }

         auto Arg = TemplateArgs.getArgForParam(TA->getParam());
         if (!Arg || !Arg->isFrozen()) {
            ResolvedTys.push_back(this->visit(Ty));
            continue;
         }

         for (auto &VA : Arg->getVariadicArgs()) {
            ResolvedTys.push_back(this->visit(VA.getNonCanonicalType()));
         }
      }

      return this->Ctx.getTupleType(ResolvedTys);
   }

   QualType visitRecordTypeCommon(QualType T, RecordDecl *R,
                                  const sema::FinalTemplateArgumentList &TemplateArgs) {
      SmallVector<sema::TemplateArgument, 0> Args;

      bool Dependent = false;
      for (auto &Arg : TemplateArgs) {
         if (!Arg.isType() || Arg.isVariadic()) {
            auto Copy = this->VisitTemplateArg(Arg);
            Dependent |= Copy.isStillDependent();
            Args.emplace_back(move(Copy));

            continue;
         }

         auto Ty = Arg.getNonCanonicalType();
         auto *TA = Ty->asGenericType();
         if (!TA || !TA->isVariadic()) {
            auto Copy = this->VisitTemplateArg(Arg);
            Dependent |= Copy.isStillDependent();
            Args.emplace_back(move(Copy));

            continue;
         }

         auto *ArgVal = TemplateArgs.getArgForParam(TA->getParam());
         if (!ArgVal || !ArgVal->isFrozen()) {
            auto Copy = this->VisitTemplateArg(Arg);
            Dependent |= Copy.isStillDependent();
            Args.emplace_back(move(Copy));

            continue;
         }

         for (auto &VA : ArgVal->getVariadicArgs()) {
            auto Copy = this->VisitTemplateArg(VA);
            Dependent |= Copy.isStillDependent();
            Args.emplace_back(move(Copy));
         }
      }

      auto FinalList = sema::FinalTemplateArgumentList::Create(
         this->SP.getContext(), Args, !Dependent);

      if (Dependent)
         return this->Ctx.getDependentRecordType(R, FinalList);

      auto *Template = R->isTemplate() ? R : R->getSpecializedTemplate();
      auto Inst = this->SP.InstantiateRecord(this->SR.getStart(), Template,
                                             FinalList);

      if (Inst)
         return this->Ctx.getRecordType(Inst);

      return T;
   }

   QualType visitRecordType(RecordType *T)
   {
      auto  R = T->getRecord();
      if (R->isInstantiation()) {
         return this->visitRecordTypeCommon(T, R->getSpecializedTemplate(),
                                            R->getTemplateArgs());
      }
      if (R->isTemplate()) {
         SmallVector<TemplateArgument, 0> Args;

         bool Dependent = false;
         for (auto *Param : R->getTemplateParams()) {
            const TemplateArgument *Arg = TemplateArgs.getArgForParam(Param);
            if (!Arg) {
               Dependent = true;
               Args.emplace_back(Param, this->Ctx.getTemplateArgType(Param),
                                 Param->getSourceLoc());

               continue;
            }

            Args.emplace_back(this->VisitTemplateArg(*Arg));
            Dependent |= Args.back().isStillDependent();
         }

         auto FinalList = sema::FinalTemplateArgumentList::Create(
            this->SP.getContext(), Args, !Dependent);

         if (Dependent)
            return this->Ctx.getDependentRecordType(R, FinalList);

         auto *Template = R->isTemplate() ? R : R->getSpecializedTemplate();
         auto Inst = this->SP.InstantiateRecord(
            this->SR.getStart(), Template, FinalList);

         if (Inst)
            return this->Ctx.getRecordType(Inst);
      }

      return T;
   }

   QualType visitDependentRecordType(DependentRecordType *T)
   {
      auto  R = T->getRecord();
      auto &TemplateArgs = T->getTemplateArgs();

      return this->visitRecordTypeCommon(T, R, TemplateArgs);
   }

   QualType visitAssociatedType(AssociatedType *T)
   {
      if (T->getDecl()->isImplementation())
         return this->visit(T->getActualType());

      return T;
   }
};

} // anonymous namespace

QueryResult SubstGenericTypesQuery::run()
{
   GenericTypeSubstVisitor<MultiLevelFinalTemplateArgList> Visitor(*QC.Sema,
                                                                   TemplateArgs,
                                                                   SR);

   return finish(Visitor.visit(T));
}

QueryResult SubstGenericTypesNonFinalQuery::run()
{
   GenericTypeSubstVisitor<MultiLevelTemplateArgList> Visitor(*QC.Sema,
                                                              TemplateArgs,
                                                              SR);

   return finish(Visitor.visit(T));
}

namespace {

class TypeEquivalenceBuilder:
      public AssociatedTypeSubstVisitorBase<TypeEquivalenceBuilder> {
   /// The left hand side declaration.
   DeclContext *LHSDecl;

public:
   TypeEquivalenceBuilder(SemaPass &SP, DeclContext *LHSDecl, QualType Self,
                          SourceRange SR)
      : AssociatedTypeSubstVisitorBase(SP, Self, SR),
        LHSDecl(LHSDecl)
   {}

   QualType visitGenericType(GenericType *T)
   {
      if (!LHSDecl)
         return T;

      auto *Param = this->SP.QC.LookupSingleAs<TemplateParamDecl>(
         LHSDecl, T->getParam()->getDeclName());

      if (!Param)
         return T;

      return this->SP.Context.getTemplateArgType(Param);
   }
};

} // anonymous namespace

QueryResult CheckTypeEquivalenceQuery::run()
{
   RHS = TypeEquivalenceBuilder(sema(), LHSDecl, Self, {}).visit(RHS);
   return finish(LHS == RHS);
}