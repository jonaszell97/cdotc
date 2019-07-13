//
// Created by Jonas Zell on 24.08.18.
//

#include "Query.h"

#include "AST/Decl.h"
#include "AST/TypeBuilder.h"
#include "AST/TypeVisitor.h"
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
         auto &StoredFields = cast<StructDecl>(rec)->getStoredFields();
         if (StoredFields.size() > 3) {
            Result = true;
         }
         else {
            for (auto *F : StoredFields) {
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
      auto *Arr = T->asArrayType();
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
   CanType T = this->T->getDesugaredType();

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
   CanType T = this->T->getDesugaredType();
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
   CanType T = this->T->getDesugaredType();
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

   QualType T = this->T;
   QualType Covar = this->Covar;

   if (!T->isMetaType()) {
      T = QC.Context.getMetaType(T);
   }
   if (!Covar->isMetaType()) {
      Covar = QC.Context.getMetaType(Covar);
   }

   auto ConvSeq = QC.Sema->getConversionSequence(T, Covar);
   return finish(ConvSeq.isImplicit());
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
      if (T->getDecl()->isImplementation() && !T->getDecl()->isDefault()) {
         return T;
      }

      if (!Self) {
         return T;
      }

      RecordDecl *DC;
      if (auto *Outer = T->getOuterAT()) {
         CanType OuterImpl = this->visit(Outer)->getCanonicalType();

         if (OuterImpl->isDependentType() || OuterImpl->isAssociatedType()) {
            return T;
         }

         DC = OuterImpl->getRecord();
      }
      else {
         DC = Self->getRecord();
      }

      bool conforms;
      if (this->SP.QC.ConformsTo(conforms, this->SP.Context.getRecordType(DC),
                                 cast<ProtocolDecl>(T->getDecl()->getRecord()))) {
         return T;
      }

      // Don't substitute unrelated associated types.
      if (!conforms) {
         return T;
      }

      QueryContext &QC = this->SP.QC;
      AssociatedTypeDecl *AT;

      if (QC.GetAssociatedType(AT, DC, T->getDecl()->getDeclName(),
                               DC->getExtensions())) {
         return T;
      }

      if (!AT /*|| AT->isDefault()*/) {
         return T;
      }

      auto *Inst = QC.template InstantiateTemplateMember<AssociatedTypeDecl>(
         AT, DC);

      if (QC.PrepareDeclInterface(Inst)) {
         return T;
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

   void visitGenericType(GenericType *T, SmallVectorImpl<QualType> &Types)
   {
      Types.push_back(visitGenericType(T));
   }

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
                                                 SmallVectorImpl<SourceRange>
                                                    &Locs,
                                                 unsigned i,
                                                 bool &Dependent,
                                                 FinalTemplateArgumentList
                                                      *TemplateArgs = nullptr) {
      if (!Name) {
         return nullptr;
      }

      NestedNameSpecifier *Copy = nullptr;
      switch (Name->getKind()) {
      case NestedNameSpecifier::Type: {
         Copy = NestedNameSpecifier::Create(this->Ctx.getDeclNameTable(),
                                            this->visit(Name->getType()),
                                            this->visitNestedNameSpecifier(
                                               Name->getPrevious(), Locs, i - 1,
                                               Dependent));

         break;
      }
      case NestedNameSpecifier::Identifier: {
         Copy = NestedNameSpecifier::Create(this->Ctx.getDeclNameTable(),
                                            Name->getIdentifier(),
                                            this->visitNestedNameSpecifier(
                                               Name->getPrevious(), Locs, i - 1,
                                               Dependent));

         break;
      }
      case NestedNameSpecifier::Namespace:
         Copy = NestedNameSpecifier::Create(this->Ctx.getDeclNameTable(),
                                            Name->getNamespace(),
                                            this->visitNestedNameSpecifier(
                                               Name->getPrevious(), Locs, i - 1,
                                               Dependent));

         break;
      case NestedNameSpecifier::TemplateParam: {
         if (auto Arg = this->TemplateArgs.getArgForParam(Name->getParam())) {
            if (Arg->isType()) {
               if (!Arg->isVariadic()) {
                  Copy = NestedNameSpecifier::Create(this->Ctx.getDeclNameTable(),
                                                     Arg->getType(),
                                                     visitNestedNameSpecifier(
                                                        Name->getPrevious(),
                                                        Locs, i - 1,
                                                        Dependent));
               }
            }
         }

         if (!Copy)
            Copy = NestedNameSpecifier::Create(this->Ctx.getDeclNameTable(),
                                               Name->getParam(),
                                               this->visitNestedNameSpecifier(
                                                  Name->getPrevious(),
                                                  Locs, i - 1,
                                                  Dependent));

         break;
      }
      case NestedNameSpecifier::AssociatedType: {
         Copy = NestedNameSpecifier::Create(this->Ctx.getDeclNameTable(),
                                            Name->getAssociatedType(),
                                            this->visitNestedNameSpecifier(
                                               Name->getPrevious(),
                                               Locs, i - 1,
                                               Dependent));

         break;
      }
      case NestedNameSpecifier::Alias: {
         auto *Alias = Name->getAlias();
         if (TemplateArgs) {
            AliasDecl *Inst;
            if (!this->SP.QC.InstantiateAlias(Inst, Alias, TemplateArgs,
                                              this->SR.getStart())) {
               Alias = Inst;
            }
         }

         Copy = NestedNameSpecifier::Create(this->Ctx.getDeclNameTable(),
                                            Alias,
                                            this->visitNestedNameSpecifier(
                                               Name->getPrevious(),
                                               Locs, i - 1,
                                               Dependent));

         break;
      }
      case NestedNameSpecifier::Module: {
         Copy = NestedNameSpecifier::Create(this->Ctx.getDeclNameTable(),
                                            Name->getModule(),
                                            this->visitNestedNameSpecifier(
                                               Name->getPrevious(),
                                               Locs, i - 1,
                                               Dependent));

         break;
      }
      case NestedNameSpecifier::TemplateArgList: {
         auto *TemplateArgs = copyTemplateArgs(Name->getTemplateArgs());
         if (TemplateArgs->isStillDependent()) {
            Copy = NestedNameSpecifier::Create(this->Ctx.getDeclNameTable(),
                                               TemplateArgs,
                                               this->visitNestedNameSpecifier(
                                                  Name->getPrevious(),
                                                  Locs, i - 1,
                                                  Dependent));
         }
         else {
            Locs.erase(Locs.begin() + i);
            Copy = this->visitNestedNameSpecifier(
               Name->getPrevious(),
               Locs, i - 2,
               Dependent,
               TemplateArgs);
         }
      }
      }

      Dependent |= Copy->isDependent();
      return Copy;
   }

   QualType visitDependentNameType(DependentNameType *T)
   {
      bool Dependent = false;

      auto Ranges = T->getNameSpecWithLoc()->getSourceRanges();
      SmallVector<SourceRange, 4> Locs(Ranges.begin(), Ranges.end());

      unsigned Depth = Locs.size() - 1;
      auto *Name = this->visitNestedNameSpecifier(T->getNameSpec(), Locs, Depth,
                                                  Dependent);

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
         Locs);

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

   FinalTemplateArgumentList*
   copyTemplateArgs(const FinalTemplateArgumentList *TemplateArgs)
   {
      SmallVector<sema::TemplateArgument, 0> Args;

      bool Dependent = false;
      for (auto &Arg : *TemplateArgs) {
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

         auto *ArgVal = TemplateArgs->getArgForParam(TA->getParam());
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

      return sema::FinalTemplateArgumentList::Create(
         this->SP.getContext(), Args, !Dependent);
   }

   QualType visitRecordTypeCommon(
                        QualType T, RecordDecl *R,
                        const sema::FinalTemplateArgumentList &TemplateArgs) {
      auto *FinalList = copyTemplateArgs(&TemplateArgs);
      if (FinalList->isStillDependent())
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

class TypeEquivalenceChecker: public TypeComparer<TypeEquivalenceChecker> {
public:
   bool visitGenericType(GenericType *LHS, QualType RHS)
   {
      if (auto *Param = RHS->asGenericType()) {
         return visit(LHS->getCovariance(), Param->getCovariance());
      }

      return false;
   }

   bool visitDependentNameType(DependentNameType *LHS, QualType RHS)
   {
      // FIXME
      return LHS->getCanonicalType() == RHS->getCanonicalType();
   }

   bool visitAssociatedType(AssociatedType *LHS, QualType RHS)
   {
      if (auto *AT = RHS->asAssociatedType()) {
         auto *LHSOuter = LHS->getOuterAT();
         auto *RHSOuter = AT->getOuterAT();

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
};

} // anonymous namespace

QueryResult CheckTypeEquivalenceQuery::run()
{
   AssociatedTypeSubstVisitor substVisitor(sema(), Self, {});
   RHS = substVisitor.visit(RHS);
   LHS = substVisitor.visit(LHS);

   if (Self->isRecordType() && Self->getRecord()->isInstantiation()) {
      GenericTypeSubstVisitor genericTypeSubstVisitor(
         sema(), Self->getRecord()->getTemplateArgs(), {});

      RHS = genericTypeSubstVisitor.visit(RHS);
      LHS = genericTypeSubstVisitor.visit(LHS);
   }

   return finish(TypeEquivalenceChecker().visit(LHS, RHS));
}

namespace {

struct AssociatedTypeFinder: public RecursiveTypeVisitor<AssociatedTypeFinder> {
   explicit AssociatedTypeFinder()
   {

   }

   bool FoundAssociatedType = false;

   bool visitAssociatedType(const AssociatedType *T)
   {
      if (T->getDecl()->isImplementation())
         return true;

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

QueryResult CheckTypeCapabilitiesQuery::run()
{
   auto Constraints = QC.Context.getExtConstraints(ND);
   if (Constraints.empty()) {
      return finish({});
   }

   std::vector<TypeCapability> Capabilities;
   for (auto *C : Constraints) {
      if (auto Err = QC.VerifyConstraint(C, ND)) {
         return Query::finish(Err);
      }

      QualType CT = C->getConstrainedType();

      if (C->getKind() == DeclConstraint::Concept) {
         Capabilities.emplace_back(CT, C->getConceptRefExpr()->getAlias());
         continue;
      }

      QualType T = C->getType();
      switch (C->getKind()) {
      case DeclConstraint::TypeEquality:
         Capabilities.emplace_back(CT, T, TypeCapability::Equality);
         break;
      case DeclConstraint::TypeInequality:
         Capabilities.emplace_back(CT, T, TypeCapability::Inequality);
         break;
      case DeclConstraint::TypePredicate: {
         ArrayRef<QualType> Types;
         if (T->isRecordType()) {
            Types = T;
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
               Capabilities.emplace_back(CT,cast<ProtocolDecl>(Ty->getRecord()),
                                         TypeCapability::Conformance);
            }
         }

         break;
      }
      case DeclConstraint::TypePredicateNegated: {
         ArrayRef<QualType> Types;
         if (T->isRecordType()) {
            Types = T;
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
               Capabilities.emplace_back(CT,cast<ProtocolDecl>(Ty->getRecord()),
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

   // FIXME verify capabilities.

   return finish(move(Capabilities));
}

static void applyCapabilities(QueryContext &QC,
                              ArrayRef<TypeCapability> Capabilities,
                              SmallVectorImpl<QualType> &Existentials,
                              QualType Stripped,
                              QualType &NewTy,
                              bool &done) {
   for (auto &C : Capabilities) {
      bool Applies = false;
      QualType ConstrainedTy = C.getConstrainedType();

      switch (C.getKind()) {
      case TypeCapability::Concept:
      case TypeCapability::Class:
      case TypeCapability::Struct:
      case TypeCapability::Enum:
      case TypeCapability::Conformance:
      case TypeCapability::NonConformance:
      case TypeCapability::SubClass:
      case TypeCapability::NotSubClass:
         Applies = Stripped == ConstrainedTy;
         break;
      case TypeCapability::Inequality:
         // The relationship is commutative.
         Applies |= Stripped != ConstrainedTy;
         Applies |= Stripped != C.getType();

         break;
      case TypeCapability::Equality:
         // The relationship is commutative.
         Applies |= Stripped == ConstrainedTy;
         Applies |= Stripped == C.getType();

         break;
      }

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
         Existentials.push_back(QC.Context.getRecordType(C.getProto()));
         break;
      case TypeCapability::SubClass:
         Existentials.push_back(QC.Context.getRecordType(C.getClass()));
         break;
      case TypeCapability::Equality:
         // Only one equality constraint is allowed in any given context,
         // so we know that this is the concrete type.
         NewTy = C.getType();
         done = true;

         break;
      case TypeCapability::NonConformance: {
         auto It = std::find(Existentials.begin(), Existentials.end(),
                             QC.Context.getRecordType(C.getProto()));

         if (It != Existentials.end()) {
            Existentials.erase(It);
         }

         break;
      }
      case TypeCapability::NotSubClass: {
         auto It = std::find(Existentials.begin(), Existentials.end(),
                             QC.Context.getRecordType(C.getClass()));

         if (It != Existentials.end()) {
            Existentials.erase(It);
         }

         break;
      }
      }
   }
}

QueryResult ApplyCapabilitesQuery::run()
{
   QualType Stripped;
   switch (T->getTypeID()) {
   case Type::ReferenceTypeID:
   case Type::MutableReferenceTypeID:
   case Type::MutableBorrowTypeID:
      Stripped = cast<ReferenceType>(T)->getReferencedType();
      break;
   case Type::PointerTypeID:
   case Type::MutablePointerTypeID:
      Stripped = cast<PointerType>(T)->getPointeeType();
      break;
   case Type::MetaTypeID:
      Stripped = cast<MetaType>(T)->getUnderlyingType();
      break;
   default:
      Stripped = T;
      break;
   }

   // Check if the current context adds additional conformances to this type.
   SmallVector<QualType, 2> Existentials;

   QualType NewTy;
   bool done = false;

   QualType Covariance;
   if (auto *AT = Stripped->asAssociatedType()) {
      auto *ATDecl = AT->getDecl();
      if (QC.PrepareDeclInterface(ATDecl) || ATDecl->isImplementation()) {
         return finish(T);
      }

      Covariance = ATDecl->getCovariance();

      // Get the constraints defined directly on the associated type.
      ArrayRef<TypeCapability> Capabilities;
      if (auto Err = QC.CheckTypeCapabilities(Capabilities, ATDecl)) {
         return Query::finish(Err);
      }

      applyCapabilities(QC, Capabilities, Existentials, Stripped, NewTy,
                        done);
   }
   else if (auto *GT = Stripped->asGenericType()) {
      auto *Param = GT->getParam();
      if (QC.PrepareDeclInterface(Param)) {
         return finish(T);
      }

      Covariance = Param->getCovariance();
   }
   else {
      return finish(T);
   }

   if (Covariance->isUnknownAnyType()) {
      return finish(T);
   }

   // Check if the current context adds additional conformances to this type.
   Existentials.push_back(Covariance);

   auto *DC = this->DC;
   while (DC && !done) {
      if (auto *ND = dyn_cast<NamedDecl>(DC)) {
         ArrayRef<TypeCapability> Capabilities;
         if (QC.CheckTypeCapabilities(Capabilities, ND)) {
            DC = DC->getParentCtx();
            continue;
         }

         applyCapabilities(QC, Capabilities, Existentials, Stripped, NewTy,
                           done);
      }

      DC = DC->getParentCtx();
   }

   if (!NewTy) {
      if (Existentials.empty()) {
         NewTy = QC.Context.getRecordType(QC.Sema->getAnyDecl());
      }
      else if (Existentials.size() == 1) {
         return finish(T);
      }
      else {
         NewTy = QC.Context.getExistentialType(Existentials);
      }
   }

   switch (T->getTypeID()) {
   case Type::ReferenceTypeID:
      return finish(QC.Context.getReferenceType(NewTy));
   case Type::MutableReferenceTypeID:
      return finish(QC.Context.getMutableReferenceType(NewTy));
   case Type::MutableBorrowTypeID:
      return finish(QC.Context.getMutableBorrowType(NewTy));
   case Type::PointerTypeID:
      return finish(QC.Context.getPointerType(NewTy));
   case Type::MutablePointerTypeID:
      return finish(QC.Context.getMutablePointerType(NewTy));
   case Type::MetaTypeID:
      return finish(QC.Context.getMetaType(NewTy));
   default:
      return finish(NewTy);
   }
}

namespace {

class TypeVariableSubstVisitor: public TypeBuilder<TypeVariableSubstVisitor> {
   const llvm::DenseMap<TypeVariableType*, QualType> &SubstMap;

public:
   TypeVariableSubstVisitor(
         SemaPass &SP,
         const llvm::DenseMap<TypeVariableType*, QualType> &SubstMap,
         SourceRange SR)
      : TypeBuilder(SP, SR),
        SubstMap(SubstMap)
   {}

   QualType visitTypeVariableType(TypeVariableType *T)
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
   return finish(TypeVariableSubstVisitor(*QC.Sema,
                                          ReplacementMap,
                                          SR).visit(T));
}

QueryResult IsImplicitlyConvertibleQuery::run()
{
   ConversionSequence *Seq;
   if (auto Err = QC.GetConversionSequence(Seq, From, To)) {
      return Query::finish(Err);
   }

   return finish(Seq && Seq->isImplicit());
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

class GenericTypeComparer: public TypeComparer<GenericTypeComparer> {
   SemaPass &Sema;

public:
   bool typeDependent = false;

   explicit GenericTypeComparer(SemaPass &Sema) : Sema(Sema) {}

   bool compareImpl(QualType LHS, QualType RHS)
   {
      auto Builder = Sema.getConversionSequence(LHS, RHS);
      if (Builder.isDependent()) {
         typeDependent = true;
      }

      return Builder.isValid();
   }

   bool visitGenericType(GenericType *LHS, QualType RHS)
   {
      bool covariant;
      if (Sema.QC.IsCovariant(covariant, RHS, LHS->getParam()->getCovariance())) {
         return true;
      }

      return covariant;
   }
};

} // anonymous namespace

QueryResult IsValidParameterValueQuery::run()
{
   if (!paramType->containsGenericType()) {
      bool implicitlyConvertible;
      if (auto err = QC.IsImplicitlyConvertible(implicitlyConvertible,
                                                givenType, paramType)) {
         return err;
      }

      return finish(implicitlyConvertible);
   }

   QualType givenType = this->givenType;
   QualType paramType = this->paramType;

   // Implicit lvalue -> rvalue
   if (givenType->isReferenceType() && !paramType->isReferenceType()) {
      givenType = givenType->stripReference();
   }

   // Implicit rvalue -> const reference
   if (!givenType->isReferenceType() && paramType->isNonMutableReferenceType()) {
      givenType = QC.Sema->Context.getReferenceType(givenType);
   }

   GenericTypeComparer comparer(*QC.Sema);

   bool result = comparer.visit(paramType, givenType);
   return finish(result);
}