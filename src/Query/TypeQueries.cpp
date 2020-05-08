#include "cdotc/Query/Query.h"

#include "cdotc/AST/Decl.h"
#include "cdotc/AST/TypeBuilder.h"
#include "cdotc/AST/TypeVisitor.h"
#include "cdotc/IL/Constants.h"
#include "cdotc/Query/QueryContext.h"
#include "cdotc/Sema/SemaPass.h"
#include "cdotc/Sema/TemplateInstantiator.h"
#include "cdotc/Support/Log.h"

using namespace cdot;
using namespace cdot::ast;
using namespace cdot::diag;
using namespace cdot::sema;
using namespace cdot::support;

QueryResult IsEquatableQuery::run()
{
   CanType T = this->T->getDesugaredType();

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

      auto* Eq = QC.Sema->getEquatableDecl();
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
         Result = QC.Sema->ConformsTo(QC.Context.getRecordType(T->getRecord()),
                                      QC.Sema->getEquatableDecl());
      }

      break;
   }

   return finish(Result, S);
}

QueryResult ContainsProtocolWithAssociatedTypesQuery::run()
{
   bool result = false;
   visitSpecificType<RecordType>([&](RecordType *R) {
      if (auto *Proto = dyn_cast<ProtocolDecl>(R->getRecord())) {
         if (Proto->hasAssociatedTypeConstraint()) {
            result = true;
            return false;
         }
      }

      return true;
   }, T);

   return finish(result);
}

QueryResult ContainsTemplateQuery::run()
{
   bool result = false;
   visitSpecificType<RecordType>([&](RecordType *R) {
     if (R->getRecord()->isTemplateOrInTemplate()) {
        result = true;
        return false;
     }

     return true;
   }, T);

   return finish(result);
}

QueryResult IsMoveOnlyQuery::run()
{
   CanType T = this->T->getDesugaredType();

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

      auto* MoveOnly = QC.Sema->getMoveOnlyDecl();
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

         Result = QC.Sema->ConformsTo(QC.Context.getRecordType(T->getRecord()),
                                      QC.Sema->getMoveOnlyDecl());
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
   CanType T = this->T->getDesugaredType();

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
   case Type::ExistentialTypeID:
      if (auto Err = QC.IsCopyable(Result, T)) {
         return Query::finish(Err);
      }

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

      if (T->isProtocol()) {
         Result = T->getRecord() != QC.Sema->getImplicitlyCopyableDecl();
      }
      else if (!T->isClass()) {
         if (auto Err = QC.CheckBuiltinConformances(T->getRecord())) {
            return Query::finish(Err);
         }

         Result = QC.Sema->ConformsTo(QC.Context.getRecordType(T->getRecord()),
                                      QC.Sema->getImplicitlyCopyableDecl());
      }

      break;
   }

   return finish(Result, S);
}

QueryResult NeedsRetainOrReleaseQuery::run()
{
   CanType T = this->T->getDesugaredType();

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
   CanType T = this->T->getDesugaredType();

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
      case Decl::StructDeclID: {
         auto& StoredFields = cast<StructDecl>(rec)->getStoredFields();
         if (StoredFields.size() > 3) {
            Result = true;
         }
         else {
            for (auto* F : StoredFields) {
               if (QC.PrepareDeclInterface(F)) {
                  continue;
               }

               if (QC.NeedsStructReturn(Result, F->getType())) {
                  continue;
               }

               if (Result) {
                  break;
               }
            }
         }

         break;
      }
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
   case Type::TupleTypeID: {
      auto ContainedTypes = T->asTupleType()->getContainedTypes();
      if (ContainedTypes.size() > 3) {
         Result = true;
      }
      else {
         for (auto ContainedTy : ContainedTypes) {
            if (QC.NeedsStructReturn(Result, ContainedTy)) {
               continue;
            }

            if (Result) {
               break;
            }
         }
      }

      break;
   }
   case Type::ArrayTypeID: {
      auto* Arr = T->asArrayType();
      if (Arr->getNumElements() > 3) {
         Result = true;
      }
      else {
         if (auto Err = QC.NeedsStructReturn(Result, Arr->getElementType())) {
            return Query::finish(Err);
         }
      }

      break;
   }
   case Type::MetaTypeID:
   case Type::BoxTypeID:
      Result = true;
      break;
   }

   return finish(Result, S);
}

QueryResult PassByValueQuery::run()
{
   CanType T = this->T->getDesugaredType();

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
      Result = true;
      break;
   case Type::AssociatedTypeID:
   case Type::ExistentialTypeID:
      Result = false;
      break;
   case Type::RecordTypeID: {
      auto rec = T->getRecord();
      switch (rec->getKind()) {
      case Decl::EnumDeclID:
         Result = !cast<EnumDecl>(rec)->isRawEnum();
         break;
      case Decl::StructDeclID: {
         auto& StoredFields = cast<StructDecl>(rec)->getStoredFields();
         if (StoredFields.size() > 3) {
            Result = false;
         }
         else {
            Result = true;
            for (auto* F : StoredFields) {
               if (QC.PrepareDeclInterface(F)) {
                  continue;
               }

               if (QC.PassByValue(Result, F->getType())) {
                  continue;
               }

               if (!Result) {
                  break;
               }
            }
         }

         break;
      }
      case Decl::UnionDeclID:
      case Decl::ProtocolDeclID:
      case Decl::ClassDeclID:
         Result = false;
         break;
      default:
         llvm_unreachable("bad record kind!");
      }

      break;
   }
   case Type::TupleTypeID: {
      auto ContainedTypes = T->asTupleType()->getContainedTypes();
      if (ContainedTypes.size() > 3) {
         Result = false;
      }
      else {
         Result = true;
         for (auto ContainedTy : ContainedTypes) {
            if (QC.PassByValue(Result, ContainedTy)) {
               continue;
            }

            if (!Result) {
               break;
            }
         }
      }

      break;
   }
   case Type::ArrayTypeID: {
      auto* Arr = T->asArrayType();
      if (Arr->getNumElements() > 3) {
         Result = false;
      }
      else {
         if (auto Err = QC.PassByValue(Result, Arr->getElementType())) {
            return Query::finish(Err);
         }
      }

      break;
   }
   case Type::MetaTypeID:
   case Type::BoxTypeID:
      Result = true;
      break;
   }

   return finish(Result, S);
}

QueryResult NeedsDeinitilizationQuery::run()
{
   CanType T = this->T->getDesugaredType();

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
   CanType T = this->T->getDesugaredType();

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

      if (T->getRecord()->isExternal()) {
         auto &Meta = QC.RecordMeta[T->getRecord()];
         return finish(Meta.IsTriviallyCopyable);
      }

      if (isa<ast::ClassDecl>(T->getRecord())) {
         Result = false;
      }
      else {
         if (auto Err = QC.CheckBuiltinConformances(T->getRecord())) {
            return Query::finish(Err);
         }

         auto& Meta = QC.RecordMeta[T->getRecord()];
         if (T->getRecord()->isInvalid()) {
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
   CanType T = this->T->getDesugaredType();

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

      if (auto* E = dyn_cast<EnumDecl>(T->getRecord())) {
         if (E->isInstantiation()) {
            Result
                = cast<EnumDecl>(E->getSpecializedTemplate())->isUnpopulated();
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
   CanType T = this->T->getDesugaredType();

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

      RecordDecl* Decl;
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
   CanType T = this->T->getDesugaredType();

   auto& TI = QC.Sema->Context.getTargetInfo();
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
   CanType T = this->T->getDesugaredType();
   auto& TI = QC.Sema->Context.getTargetInfo();
   switch (T->getTypeID()) {
   case Type::BuiltinTypeID: {
      using BK = Type::BuiltinKind;
      switch (T->asBuiltinType()->getKind()) {
      case BK::i1:
      case BK::i8:
      case BK::u8:
         return finish(1);
      case BK::u16:
      case BK::i16:
         return finish(2);
      case BK::u32:
      case BK::i32:
         return finish(4);
      case BK::u64:
      case BK::i64:
         return finish(8);
      case BK::u128:
      case BK::i128:
         return finish(16);
      case BK::f16:
         return finish(2);
      case BK::f32:
         return finish(4);
      case BK::f64:
         return finish(8);
      case BK::f80:
         return finish(16);
      case BK::f128:
         return finish(16);
      case BK::Void:
         return finish(0);
      default:
         llvm_unreachable("bad builtin type kind!");
      }
   }
   case Type::PointerTypeID:
   case Type::ReferenceTypeID:
   case Type::MutablePointerTypeID:
   case Type::MutableReferenceTypeID:
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
      for (auto& ElTy : T->uncheckedAsTupleType()->getContainedTypes()) {
         unsigned ElementStride;
         if (auto Err = QC.GetTypeStride(ElementStride, ElTy)) {
            return Query::finish(Err);
         }

         size += ElementStride;
      }

      return finish(size);
   }
   case Type::ExistentialTypeID:
   case Type::TemplateParamTypeID: {
      RecordDecl* EC;
      if (auto Err = QC.GetBuiltinRecord(
              EC, GetBuiltinRecordQuery::ExistentialContainer)) {
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
   CanType T = this->T->getDesugaredType();
   auto& TI = QC.Sema->Context.getTargetInfo();
   switch (T->getTypeID()) {
   case Type::BuiltinTypeID: {
      unsigned Size;
      if (auto Err = QC.GetTypeSize(Size, T)) {
         return Query::finish(Err);
      }

      return finish((unsigned short)Size);
   }
   case Type::PointerTypeID:
   case Type::MutablePointerTypeID:
   case Type::ReferenceTypeID:
   case Type::MutableReferenceTypeID:
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
      for (auto& ElTy : T->uncheckedAsTupleType()->getContainedTypes()) {
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
   case Type::TemplateParamTypeID: {
      RecordDecl* EC;
      if (auto Err = QC.GetBuiltinRecord(
              EC, GetBuiltinRecordQuery::ExistentialContainer)) {
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
      if (isa<ClassDecl>(T->getRecord())) {
         return finish(TI.getPointerAlignInBytes());
      }

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

   CanType T = this->T->getCanonicalType();
   CanType Covar = this->Covar->getCanonicalType();

   if (T == Covar) {
      return finish(true);
   }

   if (auto *RT = Covar->asRecordType()) {
      auto *R = RT->getRecord();
      if (auto *P = dyn_cast<ProtocolDecl>(R)) {
         return finish(QC.Sema->ConformsTo(T, P));
      }

      if (auto *C = dyn_cast<ClassDecl>(R)) {
         if (!T->isRecordType() || !isa<ClassDecl>(T->getRecord())) {
            return finish(false);
         }

         return finish(QC.Sema->IsSubClassOf(cast<ClassDecl>(T->getRecord()), C));
      }
   }
   else if (auto *Ext = Covar->asExistentialType()) {
      for (QualType Inner : Ext->getExistentials()) {
         bool IsCovariant;
         if (auto Err = QC.IsCovariant(IsCovariant, T, Inner)) {
            return Query::finish(Err);
         }

         if (!IsCovariant) {
            return finish(false);
         }
      }

      return finish(true);
   }

   return finish(false);
}

QueryResult IsContravariantQuery::run() { llvm_unreachable("TODO"); }

namespace {

template<class SubClass>
class AssociatedTypeSubstVisitorBase : public TypeBuilder<SubClass> {
   /// \brief The 'Self' type used to substitute abstract associated types.
   QualType Self;

   /// The current declaration context.
   DeclContext* CurCtx;

public:
   AssociatedTypeSubstVisitorBase(SemaPass& SP, QualType Self, SourceRange SR,
                                  DeclContext* CurCtx = nullptr)
       : TypeBuilder<SubClass>(SP, SR), Self(Self), CurCtx(CurCtx)
   {
   }

   QualType visitAssociatedType(AssociatedType* T)
   {
      RecordDecl* DC;
      QualType OuterImpl;

      if (QualType Outer = T->getOuterAT()) {
         OuterImpl = this->visit(Outer)->getCanonicalType();
         if (auto* TP = OuterImpl->asTemplateParamType()) {
            DC = TP->getCovariance()->getRecord();
         }
         else if (auto* AT = OuterImpl->asAssociatedType()) {
            DC = AT->getDecl()->getCovariance()->getRecord();
         }
         else {
            DC = OuterImpl->getRecord();
         }
      }
      else if (Self && Self->isRecordType()) {
         DC = Self->getRecord();
      }
      else {
         return T;
      }

      QualType LookupTy = this->SP.Context.getRecordType(DC);
      if (T->getDecl()->isSelf()) {
         if (this->SP.QC.DeclareSelfAlias(DC)) {
            return T;
         }
      }
      else {
         bool conforms = this->SP.ConformsTo(
            LookupTy, cast<ProtocolDecl>(T->getDecl()->getRecord()),
               this->SP.getExtensionCtx(CurCtx),
               /*AllowConditional=*/true);

         // Don't substitute unrelated associated types.
         if (!conforms) {
            return T;
         }
      }

      QueryContext& QC = this->SP.QC;
      auto Name = T->getDecl()->getDeclName();

      ConstraintSet* CS = nullptr;
      if (CurCtx) {
         CS = QC.Context.getNearestConstraintSet(CurCtx);
      }

      ArrayRef<ExtensionDecl*> extensions;
      if (!QC.FindExtensions(LookupTy)) {
         extensions = QC.Context.getExtensions(LookupTy);
      }

      AliasDecl* Impl = QC.Sema->getAssociatedTypeImpl(DC, Name);
      if (!Impl) {
         if (auto* Proto = dyn_cast<ProtocolDecl>(DC)) {
            AssociatedTypeDecl* Impl;
            if (QC.GetAssociatedTypeDecl(Impl, Proto, Name, CS)) {
               return T;
            }

            if (QC.PrepareDeclInterface(Impl)) {
               return T;
            }

            return this->Ctx.getAssociatedType(Impl, OuterImpl);
         }

         return T;
      }

      auto* Inst = QC.Sema->maybeInstantiateTemplateMember(DC, Impl);
      if (!Inst || QC.PrepareDeclInterface(Inst)) {
         return T;
      }

      auto *A = cast<AliasDecl>(Inst);
      if (!A->getType()->isMetaType()) {
         return A->getType();
      }

      return this->Ctx.getTypedefType(A);
   }
};

class AssociatedTypeSubstVisitor
    : public AssociatedTypeSubstVisitorBase<AssociatedTypeSubstVisitor> {
public:
   AssociatedTypeSubstVisitor(SemaPass& SP, QualType Self, SourceRange SR,
                              DeclContext* CurCtx = nullptr)
       : AssociatedTypeSubstVisitorBase(SP, Self, SR, CurCtx)
   {
   }
};

} // anonymous namespace

QueryResult SubstAssociatedTypesQuery::run()
{
   return finish(AssociatedTypeSubstVisitor(sema(), Self, SR).visit(T));
}

namespace {

template<class TemplateArgList>
class TemplateParamTypeSubstVisitor
    : public TypeBuilder<TemplateParamTypeSubstVisitor<TemplateArgList>> {
   /// The template arguments we are substituting with.
   const TemplateArgList& TemplateArgs;

   /// The base type of the right hand side declaration.
   QualType Self;

   /// The left hand side declaration.
   DeclContext* LHSDecl;

public:
   TemplateParamTypeSubstVisitor(SemaPass& SP,
                                 const TemplateArgList& TemplateArgs,
                                 SourceRange SR, QualType Self = {},
                                 DeclContext* LHSDecl = nullptr)
       : TypeBuilder<TemplateParamTypeSubstVisitor<TemplateArgList>>(SP, SR),
         TemplateArgs(TemplateArgs)
   {
   }

   QualType visitDependentRecordTypeAlt(DependentRecordType* T)
   {
      auto R = T->getRecord();
      auto& TemplateArgs = T->getTemplateArgs();

      return this->visitRecordTypeCommon(T, R, TemplateArgs);
   }

   QualType visitDependentRecordType(DependentRecordType* T)
   {
      if (!Self || !LHSDecl || !isa<NamedDecl>(LHSDecl)) {
         return visitDependentRecordTypeAlt(T);
      }

      auto* R = cast<NamedDecl>(LHSDecl)->getRecord();
      if (!R) {
         return visitDependentRecordTypeAlt(T);
      }

      if (T->getRecord() != R) {
         return visitDependentRecordTypeAlt(T);
      }

      if (Self->isRecordType() || !Self->getRecord()->isInstantiation()) {
         return visitDependentRecordTypeAlt(T);
      }

      if (Self->getRecord()->getSpecializedTemplate() != R) {
         return visitDependentRecordTypeAlt(T);
      }

      return Self;
   }

   void visitTemplateParamType(TemplateParamType* T,
                               SmallVectorImpl<QualType>& Types)
   {
      Types.push_back(visitTemplateParamType(T));
   }

   QualType visitTemplateParamType(TemplateParamType* T)
   {
      const TemplateArgument* Arg = TemplateArgs.getArgForParam(T->getParam());
      if (!Arg || Arg->isNull()) {
         return T;
      }

      assert(!Arg->isValue() && "should not appear in type position!");
      assert(!Arg->isVariadic() && "should have been removed!");

      return Arg->getType();
   }

   QualType visitDependentSizeArrayType(DependentSizeArrayType* T)
   {
      auto Ident = dyn_cast<IdentifierRefExpr>(T->getSizeExpr()->getExpr());
      if (!Ident || Ident->getKind() != IdentifierKind::TemplateParam)
         return T;

      auto Param = Ident->getTemplateParam();

      // have to lookup via name because the address might change if an
      // outer record is instantiated
      auto* Arg = TemplateArgs.getNamedArg(Param->getDeclName());
      if (!Arg || Arg->isNull())
         return T;

      assert(Arg->isValue() && "used type for array element size?");
      assert(isa<il::ConstantInt>(Arg->getValue()) && "invalid array size");

      return this->Ctx.getArrayType(
          this->visit(T->getElementType()),
          cast<il::ConstantInt>(Arg->getValue())->getZExtValue());
   }

   NestedNameSpecifier*
   visitNestedNameSpecifier(NestedNameSpecifier* Name,
                            SmallVectorImpl<SourceRange>& Locs, unsigned i,
                            bool& Dependent,
                            FinalTemplateArgumentList* TemplateArgs = nullptr)
   {
      if (!Name) {
         return nullptr;
      }

      NestedNameSpecifier* Copy = nullptr;
      switch (Name->getKind()) {
      case NestedNameSpecifier::Type: {
         Copy = NestedNameSpecifier::Create(
             this->Ctx.getDeclNameTable(), this->visit(Name->getType()),
             this->visitNestedNameSpecifier(Name->getPrevious(), Locs, i - 1,
                                            Dependent));

         break;
      }
      case NestedNameSpecifier::Identifier: {
         Copy = NestedNameSpecifier::Create(
             this->Ctx.getDeclNameTable(), Name->getIdentifier(),
             this->visitNestedNameSpecifier(Name->getPrevious(), Locs, i - 1,
                                            Dependent));

         break;
      }
      case NestedNameSpecifier::Namespace:
         Copy = NestedNameSpecifier::Create(
             this->Ctx.getDeclNameTable(), Name->getNamespace(),
             this->visitNestedNameSpecifier(Name->getPrevious(), Locs, i - 1,
                                            Dependent));

         break;
      case NestedNameSpecifier::TemplateParam: {
         if (auto Arg = this->TemplateArgs.getArgForParam(Name->getParam())) {
            if (Arg->isType()) {
               if (!Arg->isVariadic()) {
                  Copy = NestedNameSpecifier::Create(
                      this->Ctx.getDeclNameTable(), Arg->getType(),
                      visitNestedNameSpecifier(Name->getPrevious(), Locs, i - 1,
                                               Dependent));
               }
            }
         }

         if (!Copy)
            Copy = NestedNameSpecifier::Create(
                this->Ctx.getDeclNameTable(), Name->getParam(),
                this->visitNestedNameSpecifier(Name->getPrevious(), Locs, i - 1,
                                               Dependent));

         break;
      }
      case NestedNameSpecifier::AssociatedType: {
         Copy = NestedNameSpecifier::Create(
             this->Ctx.getDeclNameTable(), Name->getAssociatedType(),
             this->visitNestedNameSpecifier(Name->getPrevious(), Locs, i - 1,
                                            Dependent));

         break;
      }
      case NestedNameSpecifier::Alias: {
         auto* Alias = Name->getAlias();
         if (TemplateArgs) {
            AliasDecl* Inst = this->SP.getInstantiator().InstantiateAlias(
               Alias, TemplateArgs, this->SR.getStart());

            if (Inst) {
               Alias = Inst;
            }
         }

         Copy = NestedNameSpecifier::Create(
             this->Ctx.getDeclNameTable(), Alias,
             this->visitNestedNameSpecifier(Name->getPrevious(), Locs, i - 1,
                                            Dependent));

         break;
      }
      case NestedNameSpecifier::Module: {
         Copy = NestedNameSpecifier::Create(
             this->Ctx.getDeclNameTable(), Name->getModule(),
             this->visitNestedNameSpecifier(Name->getPrevious(), Locs, i - 1,
                                            Dependent));

         break;
      }
      case NestedNameSpecifier::TemplateArgList: {
         auto* TemplateArgs = copyTemplateArgs(Name->getTemplateArgs());
         if (TemplateArgs->isStillDependent()) {
            Copy = NestedNameSpecifier::Create(
                this->Ctx.getDeclNameTable(), TemplateArgs,
                this->visitNestedNameSpecifier(Name->getPrevious(), Locs, i - 1,
                                               Dependent));
         }
         else {
            Locs.erase(Locs.begin() + i);
            Copy = this->visitNestedNameSpecifier(
                Name->getPrevious(), Locs, i - 2, Dependent, TemplateArgs);
         }
      }
      }

      Dependent |= Copy->isDependent();
      return Copy;
   }

   QualType visitDependentNameType(DependentNameType* T)
   {
      bool Dependent = false;

      auto Ranges = T->getNameSpecWithLoc()->getSourceRanges();
      SmallVector<SourceRange, 4> Locs(Ranges.begin(), Ranges.end());

      unsigned Depth = Locs.size() - 1;
      auto* Name = this->visitNestedNameSpecifier(T->getNameSpec(), Locs, Depth,
                                                  Dependent);

      if (Dependent) {
         if (Name != T->getNameSpec()) {
            auto* WithLoc = NestedNameSpecifierWithLoc::Create(
                this->Ctx.getDeclNameTable(), Name,
                T->getNameSpecWithLoc()->getSourceRanges());

            return this->Ctx.getDependentNameType(WithLoc);
         }

         return T;
      }

      auto* WithLoc = NestedNameSpecifierWithLoc::Create(
          this->Ctx.getDeclNameTable(), Name, Locs);

      // Resolve the dependent name to a type.
      QualType Ty;
      if (this->SP.QC.ResolveNestedNameSpecToType(Ty, WithLoc)) {
         return T;
      }

      return Ty;
   }

   QualType visitFunctionType(FunctionType* T)
   {
      SmallVector<QualType, 4> ParamTys;
      SmallVector<FunctionType::ParamInfo, 4> Info;

      auto GivenInfo = T->getParamInfo();
      unsigned i = 0;

      for (QualType Ty : T->getParamTypes()) {
         auto* TA = Ty->asTemplateParamType();
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

         for (auto& VA : Arg->getVariadicArgs()) {
            ParamTys.push_back(this->visit(VA.getNonCanonicalType()));
            Info.push_back(GivenInfo[i]);
         }

         i++;
      }

      return this->Ctx.getFunctionType(this->visit(T->getReturnType()),
                                       ParamTys, Info, T->getRawFlags(),
                                       T->isLambdaType());
   }

   QualType visitLambdaType(LambdaType* T) { return visitFunctionType(T); }

   QualType visitTupleType(TupleType* T)
   {
      SmallVector<QualType, 4> ResolvedTys;
      for (QualType Ty : T->getContainedTypes()) {
         auto* TA = Ty->asTemplateParamType();
         if (!TA || !TA->isVariadic()) {
            ResolvedTys.push_back(this->visit(Ty));
            continue;
         }

         auto Arg = TemplateArgs.getArgForParam(TA->getParam());
         if (!Arg || !Arg->isFrozen()) {
            ResolvedTys.push_back(this->visit(Ty));
            continue;
         }

         for (auto& VA : Arg->getVariadicArgs()) {
            ResolvedTys.push_back(this->visit(VA.getNonCanonicalType()));
         }
      }

      return this->Ctx.getTupleType(ResolvedTys);
   }

   FinalTemplateArgumentList*
   copyTemplateArgs(const FinalTemplateArgumentList* TemplateArgs)
   {
      SmallVector<sema::TemplateArgument, 0> Args;

      bool Dependent = false;
      for (auto& Arg : *TemplateArgs) {
         if (!Arg.isType() || Arg.isVariadic()) {
            auto Copy = this->VisitTemplateArg(Arg);
            Dependent |= Copy.isStillDependent();
            Args.emplace_back(move(Copy));

            continue;
         }

         auto Ty = Arg.getNonCanonicalType();
         auto* TA = Ty->asTemplateParamType();
         if (!TA || !TA->isVariadic()) {
            auto Copy = this->VisitTemplateArg(Arg);
            Dependent |= Copy.isStillDependent();
            Args.emplace_back(move(Copy));

            continue;
         }

         auto* ArgVal = TemplateArgs->getArgForParam(TA->getParam());
         if (!ArgVal || !ArgVal->isFrozen()) {
            auto Copy = this->VisitTemplateArg(Arg);
            Dependent |= Copy.isStillDependent();
            Args.emplace_back(move(Copy));

            continue;
         }

         for (auto& VA : ArgVal->getVariadicArgs()) {
            auto Copy = this->VisitTemplateArg(VA);
            Dependent |= Copy.isStillDependent();
            Args.emplace_back(move(Copy));
         }
      }

      return sema::FinalTemplateArgumentList::Create(this->SP.getContext(),
                                                     Args, !Dependent);
   }

   QualType
   visitRecordTypeCommon(QualType T, RecordDecl* R,
                         const sema::FinalTemplateArgumentList& TemplateArgs)
   {
      auto* FinalList = copyTemplateArgs(&TemplateArgs);
      if (FinalList->isStillDependent())
         return this->Ctx.getDependentRecordType(R, FinalList);

      auto* Template = R->isTemplate() ? R : R->getSpecializedTemplate();
      auto Inst = this->SP.InstantiateRecord(this->SR.getStart(), Template,
                                             FinalList);

      if (Inst)
         return this->Ctx.getRecordType(Inst);

      return T;
   }

   QualType
   visitTypedefTypeCommon(QualType T, AliasDecl* td,
                          const sema::FinalTemplateArgumentList& TemplateArgs)
   {
      auto* FinalList = copyTemplateArgs(&TemplateArgs);
      if (FinalList->isStillDependent())
         return this->Ctx.getDependentTypedefType(td, FinalList);

      auto* Template = td->isTemplate() ? td : td->getSpecializedTemplate();
      auto Inst
          = this->SP.InstantiateAlias(this->SR.getStart(), Template, FinalList);

      if (Inst)
         return this->Ctx.getTypedefType(Inst);

      return T;
   }

   QualType visitRecordType(RecordType* T)
   {
      auto R = T->getRecord();
      if (R->isInstantiation() && !R->isNestedInstantiation()) {
         return this->visitRecordTypeCommon(T, R->getSpecializedTemplate(),
                                            R->getTemplateArgs());
      }
      if (R->isTemplate()) {
         SmallVector<TemplateArgument, 0> Args;

         bool Dependent = false;
         for (auto* Param : R->getTemplateParams()) {
            const TemplateArgument* Arg = TemplateArgs.getArgForParam(Param);
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

         auto* Template = R->isTemplate() ? R : R->getSpecializedTemplate();
         auto Inst = this->SP.InstantiateRecord(this->SR.getStart(), Template,
                                                FinalList);

         if (Inst)
            return this->Ctx.getRecordType(Inst);
      }

      return T;
   }

   QualType visitDependentTypedefType(DependentTypedefType* T)
   {
      auto td = T->getTypedef();
      auto& TemplateArgs = T->getTemplateArgs();

      return this->visitTypedefTypeCommon(T, td, TemplateArgs);
   }
};

} // anonymous namespace

QueryResult SubstTemplateParamTypesQuery::run()
{
   TemplateParamTypeSubstVisitor<MultiLevelFinalTemplateArgList> Visitor(
       *QC.Sema, TemplateArgs, SR);

   return finish(Visitor.visit(T));
}

QueryResult SubstTemplateParamTypesNonFinalQuery::run()
{
   TemplateParamTypeSubstVisitor<MultiLevelTemplateArgList> Visitor(
       *QC.Sema, TemplateArgs, SR);

   return finish(Visitor.visit(T));
}

namespace {

class TypeEquivalenceChecker : public TypeComparer<TypeEquivalenceChecker> {
public:
   bool visitTemplateParamType(TemplateParamType* LHS, QualType RHS)
   {
      if (auto* Param = RHS->asTemplateParamType()) {
         return visit(LHS->getCovariance(), Param->getCovariance());
      }

      return false;
   }

   bool visitDependentNameType(DependentNameType* LHS, QualType RHS)
   {
      // FIXME
      return LHS->getCanonicalType() == RHS->getCanonicalType();
   }

   bool visitAssociatedType(AssociatedType* LHS, QualType RHS)
   {
      if (auto* AT = RHS->asAssociatedType()) {
         QualType LHSOuter = LHS->getOuterAT();
         QualType RHSOuter = AT->getOuterAT();

         if ((LHSOuter == nullptr) != (RHSOuter == nullptr)) {
            return false;
         }

         if (LHSOuter && !visit(LHSOuter, RHSOuter)) {
            return false;
         }

         return LHS->getDecl()->getDeclName() == AT->getDecl()->getDeclName();
      }

      return LHS->getCanonicalType() == RHS->getCanonicalType();
   }

   bool visitRecordType(RecordType *LHS, QualType RHS)
   {
      auto *Ext = RHS->asExistentialType();
      if (!Ext) {
         return TypeComparer::visitRecordType(LHS, RHS);
      }

      return Ext->contains(LHS);
   }

   bool visitExistentialType(ExistentialType *LHS, QualType RHS)
   {
      auto *Ext = RHS->asExistentialType();
      if (!Ext) {
         return TypeComparer::visitExistentialType(LHS, RHS);
      }

      return Ext->contains(LHS->getCanonicalType());
   }
};

} // anonymous namespace

QueryResult CheckTypeEquivalenceQuery::run()
{
   AssociatedTypeSubstVisitor substVisitor(sema(), Self, {}, DeclCtx);
   CanType RHS = substVisitor.visit(this->RHS);
   CanType LHS = substVisitor.visit(this->LHS);

   if (DeclCtx) {
      if (RHS->containsAssociatedType() || RHS->containsTemplateParamType()) {
         RHS = QC.Sema->ApplyCapabilities(RHS, DeclCtx);
      }
      if (LHS->containsAssociatedType() || LHS->containsTemplateParamType()) {
         LHS = QC.Sema->ApplyCapabilities(LHS, DeclCtx);
      }
   }

   if (Self->isRecordType() && Self->getRecord()->isInstantiation()) {
      TemplateParamTypeSubstVisitor templateParamTypeSubstVisitor(
          sema(), Self->getRecord()->getTemplateArgs(), {}, Self, LHSDecl);

      RHS = templateParamTypeSubstVisitor.visit(RHS);
      LHS = templateParamTypeSubstVisitor.visit(LHS);

      // Revisit in case an outer associated type was replaced.
      RHS = substVisitor.visit(RHS);
      LHS = substVisitor.visit(LHS);
   }

   return finish(TypeEquivalenceChecker().visit(LHS, RHS));
}

namespace {

struct AssociatedTypeFinder : public RecursiveTypeVisitor<AssociatedTypeFinder> {
   explicit AssociatedTypeFinder() {}

   bool FoundAssociatedType = false;

   bool visitAssociatedType(const AssociatedType* T)
   {
      FoundAssociatedType = true;
      return false;
   }
};

} // anonymous namespace

QueryResult ContainsAssociatedTypeConstraintQuery::run()
{
   AssociatedTypeFinder Finder;
   Finder.visit(T);

   return finish(Finder.FoundAssociatedType);
}

static void FindCapabilities(std::vector<TypeCapability> &Capabilities,
                             ConstraintSet *Constraints) {
   for (auto* C : *Constraints) {
      QualType CT = C->getConstrainedType();

      if (C->getKind() == DeclConstraint::Concept) {
         Capabilities.emplace_back(CT, C->getConcept());
         continue;
      }

      SourceType T = C->getType();
      switch (C->getKind()) {
      case DeclConstraint::TypeEquality:
         Capabilities.emplace_back(CT, T, TypeCapability::Equality);
         if (T->isAssociatedType()) {
            Capabilities.emplace_back(T, CT, TypeCapability::Equality);
         }

         break;
      case DeclConstraint::TypeInequality:
         Capabilities.emplace_back(CT, T, TypeCapability::Inequality);
         if (T->isAssociatedType()) {
            Capabilities.emplace_back(T, CT, TypeCapability::Inequality);
         }

         break;
      case DeclConstraint::TypePredicate: {
         QualType SingleType = T;

         ArrayRef<QualType> Types;
         if (T->isRecordType()) {
            Types = SingleType;
         }
         else {
            Types = T->asExistentialType()->getExistentials();
         }

         for (QualType Ty : Types) {
            if (Ty->isClass()) {
               Capabilities.emplace_back(CT, cast<ClassDecl>(Ty->getRecord()),
                                         TypeCapability::SubClass);
            }
            else {
               Capabilities.emplace_back(CT,
                                         cast<ProtocolDecl>(Ty->getRecord()),
                                         TypeCapability::Conformance);
            }
         }

         break;
      }
      case DeclConstraint::TypePredicateNegated: {
         QualType SingleType = T;

         ArrayRef<QualType> Types;
         if (T->isRecordType()) {
            Types = SingleType;
         }
         else {
            Types = T->asExistentialType()->getExistentials();
         }

         for (QualType Ty : Types) {
            if (Ty->isClass()) {
               Capabilities.emplace_back(CT, cast<ClassDecl>(Ty->getRecord()),
                                         TypeCapability::NotSubClass);
            }
            else {
               Capabilities.emplace_back(CT,
                                         cast<ProtocolDecl>(Ty->getRecord()),
                                         TypeCapability::NonConformance);
            }
         }

         break;
      }
      case DeclConstraint::Class:
         Capabilities.emplace_back(CT, TypeCapability::Class);
         break;
      case DeclConstraint::Struct:
         Capabilities.emplace_back(CT, TypeCapability::Struct);
         break;
      case DeclConstraint::Enum:
         Capabilities.emplace_back(CT, TypeCapability::Enum);
         break;
      case DeclConstraint::Concept:
         llvm_unreachable("handled above");
      }
   }
}

QueryResult CheckTypeCapabilitiesQuery::run()
{
   auto* Constraints = QC.Sema->getDeclConstraints(ND);
   if (Constraints->empty()) {
      return finish({});
   }

   std::vector<TypeCapability> Capabilities;
   FindCapabilities(Capabilities, Constraints);

   // FIXME verify capabilities.
   return finish(std::move(Capabilities));
}

namespace {

class CapabilitiesTypeComparer : public TypeComparer<CapabilitiesTypeComparer> {
public:
   bool visitAssociatedType(AssociatedType *AT, QualType RHS) {
      auto *RHSAT = RHS->asAssociatedType();
      if (!RHSAT) {
         return false;
      }

      if (AT->getDecl()->isSelf()) {
         return RHSAT->getDecl()->isSelf();
      }

      if (QualType Outer = AT->getOuterAT()) {
         if (!RHSAT->getOuterAT() || !visit(Outer, RHSAT->getOuterAT())) {
            return false;
         }
      }

      return AT->getDecl()->getDeclName() == RHSAT->getDecl()->getDeclName();
   }
};

} // anonymous namespace

static void applyCapabilities(QueryContext& QC,
                              ArrayRef<TypeCapability> Capabilities,
                              ExistentialTypeBuilder& Builder,
                              QualType Stripped, QualType& NewTy)
{
   CapabilitiesTypeComparer Comparer;
   for (auto& C : Capabilities) {
      QualType ConstrainedTy = C.getConstrainedType();

      bool Applies = Comparer.visit(Stripped, ConstrainedTy);
      if (!Applies) {
         continue;
      }

      switch (C.getKind()) {
      case TypeCapability::Concept:
      case TypeCapability::Struct:
      case TypeCapability::Class:
      case TypeCapability::Enum:
      case TypeCapability::Inequality:
         // Can't reason about these yet.
         break;
      case TypeCapability::Conformance:
         Builder.push_back(QC.Context.getRecordType(C.getProto()));
         break;
      case TypeCapability::SubClass:
         Builder.push_back(QC.Context.getRecordType(C.getClass()));
         break;
      case TypeCapability::Equality:
         // Only one equality constraint is allowed in any given context,
         // so we know that this is the concrete type, unless the equality
         // is not for a concrete type.
         NewTy = C.getType();

         break;
      case TypeCapability::NonConformance: {
         Builder.remove(QC.Context.getRecordType(C.getProto()));
         break;
      }
      case TypeCapability::NotSubClass: {
         Builder.remove(QC.Context.getRecordType(C.getClass()));
         break;
      }
      }
   }
}

namespace {

class CapabilitiesApplier: public TypeBuilder<CapabilitiesApplier> {
   /// The query context.
   QueryContext &QC;

   /// The current declaration context.
   DeclContext *DC;

   QualType visitTypeCommon(QualType T, QualType Covar, NamedDecl *TypeDecl)
   {
      ExistentialTypeBuilder Builder;
      Builder.push_back(Covar);

      // Get the constraints defined directly on the associated type.
      ArrayRef<TypeCapability> Capabilities;
      if (QC.CheckTypeCapabilities(Capabilities, TypeDecl)) {
         return T;
      }

      QualType NewTy;
      applyCapabilities(QC, Capabilities, Builder, T, NewTy);

      if (NewTy && !NewTy->containsAssociatedType()) {
         return NewTy;
      }

      auto* CurDC = this->DC;
      int concreteTypes = 0;

      NewTy = QualType();
      while (CurDC) {
         if (auto* ND = dyn_cast<NamedDecl>(CurDC)) {
            if (QC.CheckTypeCapabilities(Capabilities, ND)) {
               CurDC = CurDC->getParentCtx();
               continue;
            }

            QualType ConcreteTy;
            applyCapabilities(QC, Capabilities, Builder, T, ConcreteTy);

            if (ConcreteTy) {
               if (!ConcreteTy->isAssociatedType() && ConcreteTy != NewTy) {
                  ++concreteTypes;
               }

               NewTy = ConcreteTy;
            }
         }

         CurDC = CurDC->getParentCtx();
      }

      assert(concreteTypes <= 1 && "multiple same-type constraints!");

      if (NewTy) {
         return NewTy;
      }

      if (Builder.empty()) {
         return T;
      }

      QualType Result = Builder.Build(QC.Context);
      if (Result == Covar) {
          return T;
      }

      return Result;
   }

public:
   explicit CapabilitiesApplier(QueryContext &QC, DeclContext *DC)
      : TypeBuilder(*QC.Sema, {}), QC(QC), DC(DC)
   {}

   QualType visitAssociatedType(AssociatedType *AT)
   {
      auto* ATDecl = AT->getDecl();
      if (QC.PrepareDeclInterface(ATDecl)) {
         return AT;
      }

      QualType Outer = AT->getOuterAT();
      if (Outer) {
         Outer = visit(Outer);

         // Check if the type is already concrete.
         if (!Outer->containsAssociatedType() && !Outer->containsTemplateParamType()) {
            return SP.CreateConcreteTypeFromAssociatedType(AT, Outer, AT);
         }
      }

      return visitTypeCommon(AT, ATDecl->getCovariance(), ATDecl);
   }

   void visitTemplateParamType(TemplateParamType* T,
                               SmallVectorImpl<QualType>& Types)
   {
      Types.push_back(visitTemplateParamType(T));
   }

   QualType visitTemplateParamType(TemplateParamType *T)
   {
      auto *ParamDecl = T->getParam();
      if (QC.PrepareDeclInterface(ParamDecl)) {
         return T;
      }

      return visitTypeCommon(T, ParamDecl->getCovariance(), ParamDecl);
   }
};

} // anonymous namespace

QualType SemaPass::ApplyCapabilities(QualType T, DeclContext* DeclCtx, bool force)
{
   if ((Bits.DontApplyCapabilities && !force) || !T) {
      return T;
   }

   if (!DeclCtx) {
      auto it = typeSubstitutions.find(T);
      if (it != typeSubstitutions.end()) {
         return it->getSecond();
      }

      DeclCtx = this->DeclCtx;
   }

   CapabilitiesApplier Applier(QC, DeclCtx);
   QualType Subst = Applier.visit(T);

   if (T != Subst) {
      LOG(TypeSubstitutions, T, " -> ", Subst, " [",
          cast<NamedDecl>(DeclCtx)->getFullSourceLoc(), "]");
   }

//   typeSubstitutions.try_emplace(T, Subst);
   return Subst;
}

QualType SemaPass::CreateConcreteTypeFromAssociatedType(AssociatedType *AT,
                                                        QualType Outer,
                                                        QualType Original)
{
   ExistentialTypeBuilder Builder;
   if (QualType R = Outer->asRecordType()) {
      if (auto *Proto = dyn_cast<ProtocolDecl>(R->getRecord())) {
         const MultiLevelLookupResult *Result;
         if (QC.DirectLookup(Result, Proto, AT->getDecl()->getDeclName())) {
            return QC.Sema->ErrorTy;
         }

         if (Result->empty()) {
            return QC.Sema->ErrorTy;
         }

         auto *ATDecl = cast<AssociatedTypeDecl>(Result->front().front());
         QualType Covar = ATDecl->getCovariance();

         Builder.push_back(Covar);

         // Get the constraints defined directly on the associated type.
         ArrayRef<TypeCapability> Capabilities;
         if (auto Err = QC.CheckTypeCapabilities(Capabilities, ATDecl)) {
            return QC.Sema->ErrorTy;
         }

         QualType NewTy;
         applyCapabilities(QC, Capabilities, Builder,
                           QC.Context.getAssociatedType(ATDecl), NewTy);

         assert((!NewTy || NewTy->isAssociatedType())
                && "concrete equality constraints shouldn't be allowed on a declaration!");

         if (Builder.empty()) {
            return QC.Sema->ErrorTy;
         }

         return Builder.Build(QC.Context);
      }

      auto *Impl = QC.Context.getProtocolImpl(R->getRecord(), AT->getDecl());
      QualType Result = cast<AliasDecl>(Impl)->getType()->removeMetaType();

      if (R->isDependentRecordType()) {
         if (QC.SubstTemplateParamTypes(Result, Result, R->getTemplateArgs(), {})) {
            return QC.Sema->ErrorTy;
         }
      }

      return Result;
   }

   if (auto *Ext = Outer->asExistentialType()) {
      for (QualType T : Ext->getExistentials()) {
         QualType NewTy = CreateConcreteTypeFromAssociatedType(AT, T, T);
         if (!NewTy->isErrorType()) {
            Builder.push_back(NewTy);
         }
      }
   }

   if (Builder.empty()) {
      return Original;
   }

   return Builder.Build(QC.Context);
}

namespace {

class TypeVariableSubstVisitor : public TypeBuilder<TypeVariableSubstVisitor> {
   const llvm::DenseMap<TypeVariableType*, QualType>& SubstMap;

public:
   TypeVariableSubstVisitor(
       SemaPass& SP,
       const llvm::DenseMap<TypeVariableType*, QualType>& SubstMap,
       SourceRange SR)
       : TypeBuilder(SP, SR), SubstMap(SubstMap)
   {
   }

   QualType visitTypeVariableType(TypeVariableType* T)
   {
      auto It = SubstMap.find(T);
      if (It == SubstMap.end()) {
         return T;
      }

      return It->getSecond();
   }
};

} // anonymous namespace

QueryResult SubstTypeVariablesQuery::run()
{
   return finish(
       TypeVariableSubstVisitor(*QC.Sema, ReplacementMap, SR).visit(T));
}

QueryResult IsImplicitlyConvertibleQuery::run()
{
   ConversionSequence* Seq;
   if (auto Err = QC.GetConversionSequence(Seq, From, To)) {
      return Query::finish(Err);
   }

   if (!Seq || !Seq->isImplicit()) {
      return finish({false, 0});
   }

   return finish({true, Seq->getPenalty()});
}

QueryResult GetConversionSequenceQuery::run()
{
   auto Builder = QC.Sema->getConversionSequence(From, To);
   if (Builder.isDependent()) {
      return Query::finish(Status::Dependent);
   }
   if (!Builder.isValid()) {
      return finish(nullptr);
   }

   return finish(ConversionSequence::Create(QC.Context, Builder));
}

namespace {

class TemplateParamTypeComparer
    : public TypeComparer<TemplateParamTypeComparer> {
   SemaPass& Sema;

public:
   bool typeDependent = false;
   unsigned penalty = 0;

   explicit TemplateParamTypeComparer(SemaPass& Sema) : Sema(Sema) {}

   bool compareImpl(QualType LHS, QualType RHS)
   {
      auto Builder = Sema.getConversionSequence(LHS, RHS);
      if (Builder.isDependent()) {
         typeDependent = true;
      }

      penalty += Builder.getPenalty();
      return Builder.isValid();
   }

   bool visitTemplateParamType(TemplateParamType* LHS, QualType RHS)
   {
      bool covariant;
      if (Sema.QC.IsCovariant(covariant, RHS,
                              LHS->getParam()->getCovariance())) {
         return true;
      }

      return covariant;
   }
};

} // anonymous namespace

QueryResult IsValidParameterValueQuery::run()
{
   QualType givenType = this->givenType;
   QualType paramType = this->paramType;

   if (!paramType->containsTemplateParamType()) {
      IsImplicitlyConvertibleQuery::result_type result;
      if (auto err = QC.IsImplicitlyConvertible(result, givenType, paramType)) {
         return err;
      }

      return finish({result.implicitlyConvertible, result.conversionPenalty});
   }

   // Implicit lvalue -> rvalue
   if (givenType->isReferenceType() && !paramType->isReferenceType()) {
      givenType = givenType->removeReference();
   }

   // Implicit rvalue -> const reference
   if (!givenType->isReferenceType()
       && paramType->isNonMutableReferenceType()) {
      givenType = QC.Sema->Context.getReferenceType(givenType);
   }

   TemplateParamTypeComparer comparer(*QC.Sema);

   bool result = comparer.visit(paramType, givenType);
   return finish({result, comparer.penalty});
}