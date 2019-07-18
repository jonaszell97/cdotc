//
// Created by Jonas Zell on 18.01.18.
//

#include "ASTContext.h"

#include "Decl.h"
#include "Driver/Compiler.h"
#include "Query/QueryContext.h"
#include "Serialization/ModuleFile.h"
#include "Support/Casting.h"

using namespace cdot::support;

namespace cdot {
namespace ast {

ArrayRef<Attr*> ASTContext::getAttributes(const Decl *D) const
{
   auto it = AttributeMap.find(D);
   if (it == AttributeMap.end() || !it->getSecond())
      return {};

   return *it->getSecond();
}

void ASTContext::setAttributes(const Decl *D,
                               ArrayRef<Attr*> attrs) const {
   AttributeMap[D] = new(*this) AttrVec(attrs.begin(), attrs.end());
}

void ASTContext::addAttributes(const Decl *D,
                               ArrayRef<Attr *> attrs) const {
   auto it = AttributeMap.find(D);
   if (it != AttributeMap.end()) {
      it->getSecond()->append(attrs.begin(), attrs.end());
   }
   else {
      auto Vec = new(*this) AttrVec(attrs.begin(), attrs.end());
      AttributeMap[D] = Vec;
   }
}

void ASTContext::addAttribute(const Decl *D, Attr *attr) const
{
   auto it = AttributeMap.find(D);
   if (it != AttributeMap.end()) {
      it->getSecond()->push_back(attr);
   }
   else {
      auto Vec = new(*this) AttrVec;
      Vec->push_back(attr);

      AttributeMap[D] = Vec;
   }
}

ArrayRef<StaticExpr*> ASTContext::getConstraints(const Decl *D) const
{
   auto it = ConstraintMap.find(D);
   if (it == ConstraintMap.end() || !it->getSecond())
      return {};

   return *it->getSecond();
}

void ASTContext::setConstraints(const Decl *D,
                                ArrayRef<StaticExpr *> cvec) const {
   ConstraintMap[D] = new(*this) ConstraintVec(cvec.begin(), cvec.end());
}

void ASTContext::addConstraint(const Decl *D,
                               StaticExpr *C) const {
   auto it = ConstraintMap.find(D);
   if (it != ConstraintMap.end()) {
      it->getSecond()->push_back(C);
   }
   else {
      auto Vec = new(*this) ConstraintVec;
      Vec->push_back(C);

      ConstraintMap[D] = Vec;
   }
}

ArrayRef<DeclConstraint*> ASTContext::getExtConstraints(const Decl *D) const
{
   auto it = ExtConstraintMap.find(D);
   if (it == ExtConstraintMap.end() || !it->getSecond())
      return {};

   return *it->getSecond();
}

void ASTContext::setConstraints(const Decl *D,
                                ArrayRef<DeclConstraint *> cvec) const {
   ExtConstraintMap[D] = new(*this) ExtConstraintVec(cvec.begin(), cvec.end());
}

void ASTContext::addConstraint(const Decl *D,
                               DeclConstraint *C) const {
   auto it = ExtConstraintMap.find(D);
   if (it != ExtConstraintMap.end()) {
      it->getSecond()->push_back(C);
   }
   else {
      auto Vec = new(*this) ExtConstraintVec;
      Vec->push_back(C);

      ExtConstraintMap[D] = Vec;
   }
}

ArrayRef<RecordDecl*>
ASTContext::getCovariance(const AssociatedTypeDecl *AT) const
{
   auto it = CovarianceMap.find(AT);
   if (it == CovarianceMap.end() || !it->getSecond())
      return {};

   return *it->getSecond();
}

void ASTContext::addCovariance(const AssociatedTypeDecl *AT,
                               RecordDecl *Cov) const {
   auto it = CovarianceMap.find(AT);
   if (it != CovarianceMap.end()) {
      it->getSecond()->push_back(Cov);
   }
   else {
      auto Vec = new(*this) CovarianceVec;
      Vec->push_back(Cov);

      CovarianceMap[AT] = Vec;
   }
}

ArrayRef<ExtensionDecl*>
ASTContext::getExtensions(QualType T) const
{
   auto it = ExtensionMap.find(T);
   if (it == ExtensionMap.end() || !it->getSecond())
      return {};

   return *it->getSecond();
}

void ASTContext::addExtension(QualType T,
                              ExtensionDecl *E) const {
   auto it = ExtensionMap.find(T);
   if (it != ExtensionMap.end()) {
      it->getSecond()->push_back(E);
   }
   else {
      auto Vec = new(*this) ExtensionVec;
      Vec->push_back(E);

      ExtensionMap[T] = Vec;
   }
}

void ASTContext::addProtocolDefaultImpl(const ProtocolDecl *P,
                                        const NamedDecl *Req,
                                        NamedDecl *Impl) {
   ProtocolDefaultImplMap[P][Req].push_back(Impl);
}

ArrayRef<NamedDecl*>
ASTContext::getProtocolDefaultImpls(const ProtocolDecl *P, const NamedDecl *Req)
{
   auto It = ProtocolDefaultImplMap.find(P);
   if (It == ProtocolDefaultImplMap.end())
      return nullptr;

   return It->getSecond()[Req];
}

const llvm::DenseMap<const NamedDecl*, std::vector<NamedDecl*>>*
ASTContext::getProtocolDefaultImpls(const ProtocolDecl *P)
{
   auto It = ProtocolDefaultImplMap.find(P);
   if (It == ProtocolDefaultImplMap.end())
      return nullptr;

   return &It->getSecond();
}

void ASTContext::addProtocolImpl(const RecordDecl *R,
                                 const NamedDecl *Req,
                                 NamedDecl *Impl) {
   ProtocolImplMap[R][Req] = Impl;
}

NamedDecl*
ASTContext::getProtocolImpl(const RecordDecl *R, const NamedDecl *Req)
{
   auto It = ProtocolImplMap.find(R);
   if (It == ProtocolImplMap.end())
      return nullptr;

   return It->getSecond()[Req];
}

const llvm::DenseMap<const NamedDecl*, NamedDecl*>*
ASTContext::getProtocolImpls(const RecordDecl *R)
{
   auto It = ProtocolImplMap.find(R);
   if (It == ProtocolImplMap.end())
      return nullptr;

   return &It->getSecond();
}

PointerType* ASTContext::getPointerType(QualType pointeeType) const
{
   llvm::FoldingSetNodeID ID;
   PointerType::Profile(ID, pointeeType);

   void *insertPos = nullptr;
   if (PointerType *Ptr = PointerTypes.FindNodeOrInsertPos(ID, insertPos))
      return Ptr;

   Type *CanonicalTy = nullptr;
   if (!pointeeType.isCanonical()) {
      CanonicalTy = getPointerType(pointeeType.getCanonicalType());

      // We need to get the insert position again since the folding set might
      // have grown.
      auto *NewTy = PointerTypes.FindNodeOrInsertPos(ID, insertPos);
      assert(!NewTy && "type shouldn't exist!"); (void) NewTy;
   }

   auto New = new (*this, TypeAlignment) PointerType(pointeeType, CanonicalTy);

   PointerTypes.InsertNode(New, insertPos);
   return New;
}

MutablePointerType*
ASTContext::getMutablePointerType(QualType pointeeType) const
{
   llvm::FoldingSetNodeID ID;
   MutablePointerType::Profile(ID, pointeeType);

   void *insertPos = nullptr;
   if (auto *Ptr = MutablePointerTypes.FindNodeOrInsertPos(ID, insertPos))
      return Ptr;

   Type *CanonicalTy = nullptr;
   if (!pointeeType.isCanonical()) {
      CanonicalTy = getMutablePointerType(pointeeType.getCanonicalType());

      // We need to get the insert position again since the folding set might
      // have grown.
      auto *NewTy = MutablePointerTypes.FindNodeOrInsertPos(ID, insertPos);
      assert(!NewTy && "type shouldn't exist!"); (void) NewTy;
   }

   auto New = new (*this, TypeAlignment)
      MutablePointerType(pointeeType, CanonicalTy);

   MutablePointerTypes.InsertNode(New, insertPos);
   return New;
}

ReferenceType* ASTContext::getReferenceType(QualType referencedType) const
{
   assert(!referencedType->isReferenceType() && "reference to reference type!");

   llvm::FoldingSetNodeID ID;
   ReferenceType::Profile(ID, referencedType);

   void *insertPos = nullptr;
   if (ReferenceType *Ptr = ReferenceTypes.FindNodeOrInsertPos(ID, insertPos))
      return Ptr;

   Type *CanonicalTy = nullptr;
   if (!referencedType.isCanonical()) {
      CanonicalTy = getReferenceType(referencedType.getCanonicalType());

      // We need to get the insert position again since the folding set might
      // have grown.
      auto *NewTy = ReferenceTypes.FindNodeOrInsertPos(ID, insertPos);
      assert(!NewTy && "type shouldn't exist!"); (void) NewTy;
   }

   auto New = new (*this, TypeAlignment) ReferenceType(referencedType,
                                                       CanonicalTy);

   ReferenceTypes.InsertNode(New, insertPos);
   return New;
}

MutableReferenceType*
ASTContext::getMutableReferenceType(QualType referencedType) const
{
   assert(!referencedType->isReferenceType() && "reference to reference type!");

   llvm::FoldingSetNodeID ID;
   MutableReferenceType::Profile(ID, referencedType);

   void *insertPos = nullptr;
   if (auto *Ptr = MutableReferenceTypes.FindNodeOrInsertPos(ID, insertPos))
      return Ptr;

   Type *CanonicalTy = nullptr;
   if (!referencedType.isCanonical()) {
      CanonicalTy = getMutableReferenceType(referencedType.getCanonicalType());

      // We need to get the insert position again since the folding set might
      // have grown.
      auto *NewTy = MutableReferenceTypes.FindNodeOrInsertPos(ID, insertPos);
      assert(!NewTy && "type shouldn't exist!"); (void) NewTy;
   }

   auto New = new (*this, TypeAlignment) MutableReferenceType(referencedType,
                                                              CanonicalTy);

   MutableReferenceTypes.InsertNode(New, insertPos);

#ifndef NDEBUG
   for (auto &Ty : MutableReferenceTypes) {
      Type::verifyID(Ty.getTypeID());
   }
#endif

   return New;
}

MutableBorrowType*
ASTContext::getMutableBorrowType(QualType referencedType) const
{
   assert(!referencedType->isReferenceType() && "reference to reference type!");

   llvm::FoldingSetNodeID ID;
   MutableBorrowType::Profile(ID, referencedType);

   void *insertPos = nullptr;
   if (auto *Ptr = MutableBorrowTypes.FindNodeOrInsertPos(ID, insertPos))
      return Ptr;

   Type *CanonicalTy = nullptr;
   if (!referencedType.isCanonical()) {
      CanonicalTy = getMutableBorrowType(referencedType.getCanonicalType());

      // We need to get the insert position again since the folding set might
      // have grown.
      auto *NewTy = MutableBorrowTypes.FindNodeOrInsertPos(ID, insertPos);
      assert(!NewTy && "type shouldn't exist!"); (void) NewTy;
   }

   auto New = new (*this, TypeAlignment) MutableBorrowType(referencedType,
                                                           CanonicalTy);

   MutableBorrowTypes.InsertNode(New, insertPos);
   return New;
}

BoxType* ASTContext::getBoxType(QualType BoxedTy) const
{
   auto It = BoxTypes.find(BoxedTy);
   if (It != BoxTypes.end())
      return It->getSecond();

   Type *CanonicalTy = nullptr;
   if (!BoxedTy.isCanonical()) {
      CanonicalTy = getBoxType(BoxedTy.getCanonicalType());
   }

   auto New = new(*this, TypeAlignment) BoxType(BoxedTy, CanonicalTy);
   BoxTypes.try_emplace(BoxedTy, New);

   return New;
}

QualType
ASTContext::getExistentialType(ArrayRef<QualType> Existentials) const
{
   if (Existentials.size() == 1) {
      return Existentials.front();
   }

   llvm::FoldingSetNodeID ID;
   ExistentialType::Profile(ID, Existentials);

   void *InsertPos;
   if (auto *T = ExistentialTypes.FindNodeOrInsertPos(ID, InsertPos)) {
      return T;
   }

   TypeProperties Props;
   bool Canonical = true;

   for (auto &P : Existentials) {
      Canonical &= P.isCanonical();
      Props |= P->properties();

      assert(P->isRecordType() && "existential must be RecordType!");
   }

   Type *CanonicalType = nullptr;
   if (!Canonical) {
      std::vector<QualType> canonicalArgs;
      for (auto &P : Existentials)
         canonicalArgs.push_back(P.getCanonicalType());

      CanonicalType = getExistentialType(canonicalArgs);

      // We need to get the insert position again since the folding set might
      // have grown.
      auto *NewTy = ExistentialTypes.FindNodeOrInsertPos(ID, InsertPos);
      assert(!NewTy && "type shouldn't exist!"); (void) NewTy;
   }

   void *Mem = Allocate(sizeof(ExistentialType)
      + sizeof(QualType) * Existentials.size(), TypeAlignment);

   auto *T = new(Mem) ExistentialType(Existentials, CanonicalType, Props);
   ExistentialTypes.InsertNode(T, InsertPos);

   return T;
}

FunctionType* ASTContext::getFunctionType(QualType returnType,
                              ArrayRef<QualType> argTypes,
                              ArrayRef<FunctionType::ParamInfo> paramInfo,
                              unsigned flags,
                              bool lambda) const {
   if (lambda)
      return getLambdaType(returnType, argTypes, paramInfo, flags);

   llvm::FoldingSetNodeID ID;
   FunctionType::Profile(ID, returnType, argTypes, paramInfo, flags, false);

   void *insertPos = nullptr;
   if (FunctionType *Ptr = FunctionTypes.FindNodeOrInsertPos(ID, insertPos))
      return Ptr;

   TypeProperties Props = returnType->properties();
   bool Canonical = returnType.isCanonical();

   for (auto &arg : argTypes) {
      Canonical &= arg.isCanonical();
      Props |= arg->properties();
   }

   Type *CanonicalType = nullptr;
   if (!Canonical) {
      std::vector<QualType> canonicalArgs;
      for (auto &arg : argTypes)
         canonicalArgs.push_back(arg.getCanonicalType());

      CanonicalType = getFunctionType(returnType.getCanonicalType(),
                                      canonicalArgs, paramInfo, flags,
                                      lambda);

      // We need to get the insert position again since the folding set might
      // have grown.
      auto *NewTy = FunctionTypes.FindNodeOrInsertPos(ID, insertPos);
      assert(!NewTy && "type shouldn't exist!"); (void) NewTy;
   }

   size_t SizeToAlloc = sizeof(FunctionType)
                        + sizeof(QualType) * argTypes.size()
                        + sizeof(FunctionType::ParamInfo) * paramInfo.size();

   void *Mem = Allocate(SizeToAlloc, TypeAlignment);
   auto FnTy = new(Mem) FunctionType(returnType, argTypes, paramInfo,
                                     (FunctionType::ExtFlags)flags,
                                     CanonicalType, Props);

   FunctionTypes.InsertNode(FnTy, insertPos);
   return FnTy;
}

FunctionType* ASTContext::getFunctionType(QualType returnType,
                                          ArrayRef<QualType> argTypes,
                                          unsigned flags,
                                          bool lambda) const {
   llvm::SmallVector<FunctionType::ParamInfo, 4> ParamInfo;
   ParamInfo.resize(argTypes.size());

   return getFunctionType(returnType, argTypes, ParamInfo, flags, lambda);
}

LambdaType* ASTContext::getLambdaType(FunctionType *FnTy)
{
   return getLambdaType(FnTy->getReturnType(), FnTy->getParamTypes(),
                        FnTy->getParamInfo(), FnTy->getRawFlags());
}

LambdaType* ASTContext::getLambdaType(QualType returnType,
                             ArrayRef<QualType> argTypes,
                             ArrayRef<FunctionType::ParamInfo> paramInfo,
                             unsigned int flags) const {
   llvm::FoldingSetNodeID ID;
   FunctionType::Profile(ID, returnType, argTypes, paramInfo, flags, true);

   void *insertPos = nullptr;
   if (LambdaType *Ptr = LambdaTypes.FindNodeOrInsertPos(ID, insertPos))
      return Ptr;

   TypeProperties Props = returnType->properties();
   bool Canonical = returnType.isCanonical();

   for (auto &arg : argTypes) {
      Canonical &= arg.isCanonical();
      Props |= arg->properties();
   }

   Type *CanonicalType = nullptr;
   if (!Canonical) {
      std::vector<QualType> canonicalArgs;
      for (auto &arg : argTypes)
         canonicalArgs.push_back(arg.getCanonicalType());

      CanonicalType = getLambdaType(returnType.getCanonicalType(),
                                    canonicalArgs, paramInfo, flags);

      // We need to get the insert position again since the folding set might
      // have grown.
      auto *NewTy = LambdaTypes.FindNodeOrInsertPos(ID, insertPos);
      assert(!NewTy && "type shouldn't exist!"); (void) NewTy;
   }

   size_t SizeToAlloc = sizeof(LambdaType)
                        + sizeof(QualType) * argTypes.size()
                        + sizeof(FunctionType::ParamInfo) * paramInfo.size();

   void *Mem = Allocate(SizeToAlloc, TypeAlignment);
   auto New = new(Mem) LambdaType(returnType, argTypes, paramInfo,
                                  (FunctionType::ExtFlags)flags,
                                  CanonicalType, Props);

   LambdaTypes.InsertNode(New, insertPos);
   return New;
}

LambdaType* ASTContext::getLambdaType(QualType returnType,
                                      ArrayRef<QualType> argTypes,
                                      unsigned int flags) const {
   llvm::SmallVector<FunctionType::ParamInfo, 4> ParamInfo;
   ParamInfo.resize(argTypes.size());

   return getLambdaType(returnType, argTypes, ParamInfo, flags);
}

ArrayType* ASTContext::getArrayType(QualType elementType,
                                    unsigned numElements) const {
   llvm::FoldingSetNodeID ID;
   ArrayType::Profile(ID, elementType, numElements);

   void *insertPos = nullptr;
   if (ArrayType *Ptr = ArrayTypes.FindNodeOrInsertPos(ID, insertPos))
      return Ptr;

   Type *CanonicalTy = nullptr;
   if (!elementType.isCanonical()) {
      CanonicalTy = getArrayType(elementType.getCanonicalType(), numElements);

      // We need to get the insert position again since the folding set might
      // have grown.
      auto *NewTy = ArrayTypes.FindNodeOrInsertPos(ID, insertPos);
      assert(!NewTy && "type shouldn't exist!"); (void) NewTy;
   }

   auto New = new (*this, TypeAlignment) ArrayType(elementType, numElements,
                                                   CanonicalTy);

   ArrayTypes.InsertNode(New, insertPos);
   return New;
}

DependentSizeArrayType*
ASTContext::getValueDependentSizedArrayType(QualType elementType,
                                            StaticExpr *DependentExpr) const {
   Type *CanonicalTy = nullptr;
   if (!elementType.isCanonical())
      CanonicalTy = getValueDependentSizedArrayType(
         elementType.getCanonicalType(), DependentExpr);

   // these aren't uniqued, because the size expressions aren't either
   return new (*this, TypeAlignment) DependentSizeArrayType(elementType,
                                                            DependentExpr,
                                                            CanonicalTy);
}

InferredSizeArrayType* ASTContext::getInferredSizeArrayType(QualType elTy) const
{
   llvm::FoldingSetNodeID ID;
   InferredSizeArrayType::Profile(ID, elTy);

   void *insertPos = nullptr;
   if (auto *Ptr = InferredSizeArrayTypes.FindNodeOrInsertPos(ID, insertPos))
      return Ptr;

   Type *CanonicalTy = nullptr;
   if (!elTy.isCanonical()) {
      CanonicalTy = getInferredSizeArrayType(elTy.getCanonicalType());

      // We need to get the insert position again since the folding set might
      // have grown.
      auto *NewTy = InferredSizeArrayTypes.FindNodeOrInsertPos(ID, insertPos);
      assert(!NewTy && "type shouldn't exist!"); (void) NewTy;
   }

   auto New = new (*this, TypeAlignment) InferredSizeArrayType(elTy,
                                                               CanonicalTy);

   InferredSizeArrayTypes.InsertNode(New, insertPos);
   return New;
}

TupleType*
ASTContext::getTupleType(ArrayRef<QualType> containedTypes) const
{
   llvm::FoldingSetNodeID ID;
   TupleType::Profile(ID, containedTypes);

   void *insertPos = nullptr;
   if (TupleType *Ptr = TupleTypes.FindNodeOrInsertPos(ID, insertPos))
      return Ptr;

   TypeProperties Props;
   bool Canonical = true;

   for (auto &arg : containedTypes) {
      Canonical &= arg.isCanonical();
      Props |= arg->properties();
   }

   Type *CanonicalType = nullptr;
   if (!Canonical) {
      std::vector<QualType> canonicalElements;
      for (auto &arg : containedTypes)
         canonicalElements.push_back(arg.getCanonicalType());

      CanonicalType = getTupleType(canonicalElements);

      // We need to get the insert position again since the folding set might
      // have grown.
      auto *NewTy = TupleTypes.FindNodeOrInsertPos(ID, insertPos);
      assert(!NewTy && "type shouldn't exist!"); (void) NewTy;
   }

   size_t SizeToAlloc = sizeof(TupleType)
                        + sizeof(QualType) * containedTypes.size();

   void *Mem = Allocate(SizeToAlloc, TypeAlignment);
   auto TupTy = new(Mem) TupleType(containedTypes, CanonicalType,
                                   Props);

   TupleTypes.InsertNode(TupTy, insertPos);
   return TupTy;
}

static DependentRecordType *createTemplateType(ASTContext &C, RecordDecl *R)
{
   SmallVector<sema::TemplateArgument, 2> templateArgs;
   for (auto *param : R->getTemplateParams()) {
      if (param->isVariadic()) {
         templateArgs.emplace_back(param, param->isTypeName(),
                                   std::vector<sema::TemplateArgument>(),
                                   param->getSourceLoc());
      }
      else if (param->isTypeName()) {
         templateArgs.emplace_back(param, C.getTemplateArgType(param),
                                   param->getSourceLoc());
      }
      else {
         llvm_unreachable("not sure how to handle this!");
      }
   }

   auto *finalList = sema::FinalTemplateArgumentList::Create(C, templateArgs);
   auto *dependentType = C.getDependentRecordType(R, finalList);

   R->setType(dependentType);
   return dependentType;
}

RecordType* ASTContext::getRecordType(RecordDecl *R) const
{
   if (R->isTemplate()) {
      if (QualType T = R->getType()) {
         return T->asDependentRecordType();
      }

      return createTemplateType(const_cast<ASTContext&>(*this), R);
   }

   llvm::FoldingSetNodeID ID;
   RecordType::Profile(ID, R);

   void *insertPos = nullptr;
   if (auto *Ptr = RecordTypes.FindNodeOrInsertPos(ID, insertPos))
      return Ptr;

   RecordType *New = new(*this, TypeAlignment) RecordType(R);
   RecordTypes.InsertNode(New, insertPos);

   return New;
}

static bool isCanonical(const sema::TemplateArgument &Arg)
{
   if (!Arg.isType()) {
      return true;
   }
   if (Arg.isVariadic()) {
      for (auto &VA : Arg.getVariadicArgs()) {
         if (!isCanonical(VA)) {
            return false;
         }
      }
   }

   return Arg.getNonCanonicalType()->isCanonical();
}

static sema::TemplateArgument makeCanonical(const sema::TemplateArgument &Arg)
{
   if (!Arg.isType()) {
      return Arg.clone();
   }
   if (Arg.isVariadic()) {
      std::vector<sema::TemplateArgument> VariadicArgs;
      VariadicArgs.reserve(Arg.getVariadicArgs().size());

      for (auto &VA : Arg.getVariadicArgs()) {
         VariadicArgs.emplace_back(makeCanonical(VA));
      }

      return sema::TemplateArgument(Arg.getParam(), true, move(VariadicArgs),
                                    Arg.getLoc());
   }

   return sema::TemplateArgument(Arg.getParam(),
                                 Arg.getNonCanonicalType()->getCanonicalType(),
                                 Arg.getLoc());
}

DependentRecordType*
ASTContext::getDependentRecordType(RecordDecl *R,
                                   sema::FinalTemplateArgumentList *args,
                                   QualType Parent) const {
   assert(!R->isInstantiation() && "dependent instantiation?");

   llvm::FoldingSetNodeID ID;
   DependentRecordType::Profile(ID, R, args, Parent);

   void *insertPos = nullptr;
   if (auto *Ptr = DependentRecordTypes.FindNodeOrInsertPos(ID, insertPos)) {
      return Ptr;
   }

   bool Canonical = true;
   for (auto &Arg : *args) {
      if (!isCanonical(Arg)) {
         Canonical = false;
         break;
      }
   }

   DependentRecordType *CanonicalType = nullptr;
   if (!Canonical) {
      SmallVector<sema::TemplateArgument, 4> CanonicalArgs;
      CanonicalArgs.reserve(args->size());

      for (auto &Arg : *args) {
         CanonicalArgs.emplace_back(makeCanonical(Arg));
      }

      auto *CanonicalList = sema::FinalTemplateArgumentList::Create(
         const_cast<ASTContext&>(*this), CanonicalArgs, true);

      CanonicalType = getDependentRecordType(R, CanonicalList,
                                             Parent
                                                ? Parent->getCanonicalType()
                                                : QualType());

      // We need to get the insert position again since the folding set might
      // have grown.
      auto *NewTy = DependentRecordTypes.FindNodeOrInsertPos(ID, insertPos);
      assert(!NewTy && "type shouldn't exist!"); (void) NewTy;
   }

   auto New = new(*this, TypeAlignment) DependentRecordType(R, args, Parent,
                                                            CanonicalType);

   DependentRecordTypes.InsertNode(New, insertPos);
   return New;
}

TemplateParamType* ASTContext::getTemplateArgType(TemplateParamDecl *Param) const
{
   llvm::FoldingSetNodeID ID;
   TemplateParamType::Profile(ID, Param);

   void *insertPos = nullptr;
   if (auto *Ptr = TemplateParamTypes.FindNodeOrInsertPos(ID, insertPos))
      return Ptr;

   auto New = new (*this, TypeAlignment) TemplateParamType(Param);
   TemplateParamTypes.InsertNode(New, insertPos);

   return New;
}

AssociatedType* ASTContext::getAssociatedType(AssociatedTypeDecl *AT,
                                              AssociatedType *OuterAT) const {
   llvm::FoldingSetNodeID ID;
   AssociatedType::Profile(ID, AT, OuterAT);

   void *insertPos = nullptr;
   if (auto *Ptr = AssociatedTypes.FindNodeOrInsertPos(ID, insertPos))
      return Ptr;

   auto New = new (*this, TypeAlignment) AssociatedType(AT, OuterAT);
   AssociatedTypes.InsertNode(New, insertPos);

   return New;
}

DependentNameType*
ASTContext::getDependentNameType(NestedNameSpecifierWithLoc *Name) const
{
   llvm::FoldingSetNodeID ID;
   DependentNameType::Profile(ID, Name);

   void *insertPos = nullptr;
   if (auto *Ptr = DependentNameTypes.FindNodeOrInsertPos(ID, insertPos))
      return Ptr;

   auto New = new (*this, TypeAlignment) DependentNameType(Name);
   DependentNameTypes.InsertNode(New, insertPos);

   return New;
}

MetaType* ASTContext::getMetaType(QualType forType) const
{
   llvm::FoldingSetNodeID ID;
   MetaType::Profile(ID, forType);

   void *insertPos = nullptr;
   if (auto *Ptr = MetaTypes.FindNodeOrInsertPos(ID, insertPos))
      return Ptr;

   Type *CanonicalTy = nullptr;
   if (!forType.isCanonical()) {
      CanonicalTy = getMetaType(forType.getCanonicalType());

      // We need to get the insert position again since the folding set might
      // have grown.
      auto *NewTy = MetaTypes.FindNodeOrInsertPos(ID, insertPos);
      assert(!NewTy && "type shouldn't exist!"); (void) NewTy;
   }

   auto New = new (*this, TypeAlignment) MetaType(forType, CanonicalTy);
   MetaTypes.InsertNode(New, insertPos);

   return New;
}

TypedefType* ASTContext::getTypedefType(AliasDecl *TD) const
{
   assert(TD->getType()
          && "should declare alias before using it!");

   llvm::FoldingSetNodeID ID;
   TypedefType::Profile(ID, TD);

   void *insertPos = nullptr;
   if (auto *Ptr = TypedefTypes.FindNodeOrInsertPos(ID, insertPos))
      return Ptr;

   auto New = new (*this, TypeAlignment) TypedefType(TD);
   TypedefTypes.InsertNode(New, insertPos);

   return New;
}

TypeVariableType* ASTContext::getTypeVariableType(unsigned ID) const
{
   auto It = TypeVariableTypes.find(ID);
   if (It != TypeVariableTypes.end()) {
      return It->getSecond();
   }

   auto *T = new(*this, TypeAlignment) TypeVariableType(ID);
   TypeVariableTypes[ID] = T;

   return T;
}

DependentTypedefType* ASTContext::getDependentTypedefType(
                                          AliasDecl *td,
                                          sema::FinalTemplateArgumentList *args,
                                          QualType Parent) const {
   assert(!td->isInstantiation() && "dependent instantiation?");

   llvm::FoldingSetNodeID ID;
   DependentTypedefType::Profile(ID, td, args, Parent);

   void *insertPos = nullptr;
   if (auto *Ptr = DependentTypedefTypes.FindNodeOrInsertPos(ID, insertPos)) {
      return Ptr;
   }

   QualType CanonicalType = td->getType()->asMetaType()->getUnderlyingType()->getCanonicalType();
   CI.getQueryContext().SubstTemplateParamTypes(CanonicalType, CanonicalType, *args, td->getSourceRange());

//   bool Canonical = true;
//   for (auto &Arg : *args) {
//      if (!isCanonical(Arg)) {
//         Canonical = false;
//         break;
//      }
//   }
//
//   DependentTypedefType *CanonicalType = nullptr;
//   if (!Canonical) {
//      SmallVector<sema::TemplateArgument, 4> CanonicalArgs;
//      CanonicalArgs.reserve(args->size());
//
//      for (auto &Arg : *args) {
//         CanonicalArgs.emplace_back(makeCanonical(Arg));
//      }
//
//      auto *CanonicalList = sema::FinalTemplateArgumentList::Create(
//         const_cast<ASTContext&>(*this), CanonicalArgs, true);
//
//      CanonicalType = getDependentTypedefType(td, CanonicalList,
//                                             Parent
//                                             ? Parent->getCanonicalType()
//                                             : QualType());
//
//      // We need to get the insert position again since the folding set might
//      // have grown.
//      auto *NewTy = DependentTypedefTypes.FindNodeOrInsertPos(ID, insertPos);
//      assert(!NewTy && "type shouldn't exist!"); (void) NewTy;
//   }

   auto New = new(*this, TypeAlignment) DependentTypedefType(td, args, Parent,
                                                             CanonicalType);

   DependentTypedefTypes.InsertNode(New, insertPos);
   return New;
}

CallableDecl*
ASTContext::getFunctionTemplateInstantiation(CallableDecl *Template,
                                             TemplateArgList &argList,
                                             void *&insertPos) {
   llvm::FoldingSetNodeID ID;
   CallableDecl::Profile(ID, Template, argList);

   return FunctionTemplateInstatiations.FindNodeOrInsertPos(ID, insertPos);
}

RecordDecl*
ASTContext::getRecordTemplateInstantiation(RecordDecl *Template,
                                           TemplateArgList &argList,
                                           void *&insertPos) {
   llvm::FoldingSetNodeID ID;
   RecordDecl::Profile(ID, Template, argList);

   return RecordTemplateInstatiations.FindNodeOrInsertPos(ID, insertPos);
}

AliasDecl*
ASTContext::getAliasTemplateInstantiation(AliasDecl *Template,
                                          TemplateArgList &argList,
                                          void *&insertPos) {
   llvm::FoldingSetNodeID ID;
   AliasDecl::Profile(ID, Template, argList);

   return AliasTemplateInstatiations.FindNodeOrInsertPos(ID, insertPos);
}

NamedDecl* ASTContext::getTemplateInstantiation(NamedDecl *Template,
                                                TemplateArgList &argList,
                                                void *&insertPos) {
   if (auto C = dyn_cast<CallableDecl>(Template)) {
      return getFunctionTemplateInstantiation(C, argList, insertPos);
   }
   if (auto R = dyn_cast<RecordDecl>(Template)) {
      return getRecordTemplateInstantiation(R, argList, insertPos);
   }
   if (auto A = dyn_cast<AliasDecl>(Template)) {
      return getAliasTemplateInstantiation(A, argList, insertPos);
   }

   llvm_unreachable("not a template!");
}

void ASTContext::insertFunctionTemplateInstantiation(CallableDecl *Inst,
                                                     void *insertPos) {
   assert(((uintptr_t)Inst & 1) == 0);
   assert(!Inst->getNextInBucket());

   registerInstantiation(Inst->getSpecializedTemplate(), Inst);
   FunctionTemplateInstatiations.InsertNode(Inst, insertPos);
}

void ASTContext::insertRecordTemplateInstantiation(RecordDecl *Inst,
                                                   void *insertPos) {
   assert(((uintptr_t)Inst & 1) == 0);
   assert(!Inst->getNextInBucket());

   registerInstantiation(Inst->getSpecializedTemplate(), Inst);
   RecordTemplateInstatiations.InsertNode(Inst, insertPos);
}

void ASTContext::insertAliasTemplateInstantiation(AliasDecl *Inst,
                                                  void *insertPos) {
   assert(((uintptr_t)Inst & 1) == 0);
   assert(!Inst->getNextInBucket());

   registerInstantiation(Inst->getSpecializedTemplate(), Inst);
   AliasTemplateInstatiations.InsertNode(Inst, insertPos);
}

void ASTContext::insertTemplateInstantiation(NamedDecl *Inst,
                                             void *insertPos) {
   if (auto C = dyn_cast<CallableDecl>(Inst)) {
      FunctionTemplateInstatiations.InsertNode(C, insertPos);
   }
   else if (auto R = dyn_cast<RecordDecl>(Inst)) {
      RecordTemplateInstatiations.InsertNode(R, insertPos);
   }
   else if (auto A = dyn_cast<AliasDecl>(Inst)) {
      AliasTemplateInstatiations.InsertNode(A, insertPos);
   }
}

ArrayRef<NamedDecl*> ASTContext::getInstantiationsOf(NamedDecl *Template)
{
   auto It = InstMap.find(Template);
   if (It == InstMap.end())
      return {};

   return It->getSecond();
}

void ASTContext::registerInstantiation(NamedDecl *Template, NamedDecl *Inst)
{
   InstMap[Template].push_back(Inst);
}

void ASTContext::initializeOpNames() const
{
#  define Infix(Spelling) registerInfixOperator(&Identifiers.get(Spelling))
#  define Prefix(Spelling) registerPrefixOperator(&Identifiers.get(Spelling))
#  define Postfix(Spelling) registerPostfixOperator(&Identifiers.get(Spelling))
#  define CDOT_OPERATOR(Name, Spelling, Precedence, Fix) Fix(Spelling);
#  include "Basic/BuiltinOperators.def"

#  undef Infix
#  undef Prefix
#  undef Postfix
}

void ASTContext::cleanupDeclContext(DeclContext *DC)
{
   for (auto *D : DC->getDecls())
      cleanupDecl(D);

   if (auto *Ext = DC->ExtStorage) {
      if (auto *MF = Ext->ModFile)
         MF->~ModuleFile();

      Ext->~ExternalStorage();
   }

   switch (DC->getDeclKind()) {
   case Decl::StructDeclID:
   case Decl::ClassDeclID: {
      auto *R = cast<StructDecl>(DC);
      R->StoredFields.~SmallVector();
      break;
   }
   default:
      break;
   }

   DC->namedDecls.~SmallDenseMap();
}

void ASTContext::cleanupDecl(Decl *D)
{
   if (auto *DC = dyn_cast<DeclContext>(D)) {
      cleanupDeclContext(DC);
   }
}

void ASTContext::cleanup()
{
   // Walk the AST deleting DeclContext maps.
   cleanupDeclContext(&CI.getGlobalDeclCtx());

   for (auto &AttrPair : AttributeMap) {
      AttrPair.second->~SmallVector();
   }
   for (auto &ConstraintPair : ConstraintMap) {
      ConstraintPair.second->~SmallVector();
   }
   for (auto &ExtensionPair : ExtensionMap) {
      ExtensionPair.second->~SmallVector();
   }
}

ASTContext::ASTContext(CompilerInstance &CI)
   : CI(CI), Allocator(), DeclNames(*this),
     TI(CI, llvm::Triple(llvm::sys::getDefaultTargetTriple())),
#  define CDOT_BUILTIN_TYPE(Name)            \
     Name##Ty(BuiltinType::Name),
#  include "Basic/BuiltinTypes.def"
   PointerTypes{}
{
   EmptyTupleTy = getTupleType({});
}

} // namespace ast
} // namespace cdot
