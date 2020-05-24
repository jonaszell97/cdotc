#include "cdotc/AST/ASTContext.h"

#include "cdotc/AST/Decl.h"
#include "cdotc/Driver/Compiler.h"
#include "cdotc/Query/QueryContext.h"
#include "cdotc/Serialization/ModuleFile.h"
#include "cdotc/Support/Casting.h"
#include "cdotc/Support/Log.h"

using namespace cdot::support;

namespace cdot {
namespace ast {

ArrayRef<Attr*> ASTContext::getAttributes(const Decl* D) const
{
   auto it = AttributeMap.find(D);
   if (it == AttributeMap.end() || !it->getSecond())
      return {};

   return *it->getSecond();
}

void ASTContext::setAttributes(const Decl* D, ArrayRef<Attr*> attrs) const
{
   AttributeMap[D] = new (*this) AttrVec(attrs.begin(), attrs.end());
}

void ASTContext::addAttributes(const Decl* D, ArrayRef<Attr*> attrs) const
{
   auto it = AttributeMap.find(D);
   if (it != AttributeMap.end()) {
      it->getSecond()->append(attrs.begin(), attrs.end());
   }
   else {
      auto Vec = new (*this) AttrVec(attrs.begin(), attrs.end());
      AttributeMap[D] = Vec;
   }
}

void ASTContext::addAttribute(const Decl* D, Attr* attr) const
{
   auto it = AttributeMap.find(D);
   if (it != AttributeMap.end()) {
      it->getSecond()->push_back(attr);
   }
   else {
      auto Vec = new (*this) AttrVec;
      Vec->push_back(attr);

      AttributeMap[D] = Vec;
   }
}

void ASTContext::setParsedConstraints(const Decl* D,
                                      std::vector<ParsedConstraint>&& vec) const
{
   ParsedConstraintMap.try_emplace(D, std::move(vec));
}

ArrayRef<ParsedConstraint> ASTContext::getParsedConstraints(const Decl* D) const
{
   auto it = ParsedConstraintMap.find(D);
   if (it == ParsedConstraintMap.end()) {
      return {};
   }

   return it->getSecond();
}

ConstraintSet* ASTContext::getExtConstraints(const Decl* D) const
{
   auto it = ConstraintMap.find(D);
   if (it == ConstraintMap.end() || !it->getSecond()) {
      return EmptyConstraintSet;
   }

   return it->getSecond();
}

void ASTContext::setConstraints(const Decl* D, ConstraintSet* CS) const
{
   ConstraintMap[D] = CS;
}

void ASTContext::updateConstraintLocs(
    const Decl* D, ArrayRef<ParsedConstraint> parsedConstraints,
    ArrayRef<DeclConstraint*> declConstraints)
{
   assert(parsedConstraints.size() == declConstraints.size());

   auto &map = ConstraintLocs[D];
   for (int i = 0; i < parsedConstraints.size(); ++i) {
      map[declConstraints[i]] = parsedConstraints[i].SR;
   }
}

SourceRange ASTContext::getConstraintLoc(const Decl* D,
                                         const DeclConstraint* DC) const
{
   auto it = ConstraintLocs.find(D);
   if (it == ConstraintLocs.end()) {
      return SourceRange();
   }

   auto it2 = it->getSecond().find(DC);
   if (it2 == it->getSecond().end()) {
      return SourceRange();
   }

   return it2->getSecond();
}

ConstraintSet* ASTContext::getNearestConstraintSet(const DeclContext* DC) const
{
   while (DC) {
      if (auto* decl = dyn_cast<Decl>(DC)) {
         auto* CS = getExtConstraints(decl);
         if (!CS->empty()) {
            return CS;
         }
      }

      DC = DC->getParentCtx();
   }

   return 0;
}

ArrayRef<RecordDecl*>
ASTContext::getCovariance(const AssociatedTypeDecl* AT) const
{
   auto it = CovarianceMap.find(AT);
   if (it == CovarianceMap.end() || !it->getSecond())
      return {};

   return *it->getSecond();
}

void ASTContext::addCovariance(const AssociatedTypeDecl* AT,
                               RecordDecl* Cov) const
{
   auto it = CovarianceMap.find(AT);
   if (it != CovarianceMap.end()) {
      it->getSecond()->push_back(Cov);
   }
   else {
      auto Vec = new (*this) CovarianceVec;
      Vec->push_back(Cov);

      CovarianceMap[AT] = Vec;
   }
}

ArrayRef<ExtensionDecl*> ASTContext::getExtensions(CanType T) const
{
   auto it = ExtensionMap.find(T);
   if (it == ExtensionMap.end() || !it->getSecond())
      return {};

   return *it->getSecond();
}

void ASTContext::addExtension(CanType T, ExtensionDecl* E) const
{
   auto it = ExtensionMap.find(T);
   if (it != ExtensionMap.end()) {
      it->getSecond()->push_back(E);
   }
   else {
      auto Vec = new (*this) ExtensionVec;
      Vec->push_back(E);

      ExtensionMap[T] = Vec;
   }
}

void ASTContext::addProtocolDefaultImpl(const ProtocolDecl* P,
                                        const NamedDecl* Req, NamedDecl* Impl)
{
   ProtocolDefaultImplMap[P][Req].push_back(Impl);
}

ArrayRef<NamedDecl*> ASTContext::getProtocolDefaultImpls(const ProtocolDecl* P,
                                                         const NamedDecl* Req)
{
   auto It = ProtocolDefaultImplMap.find(P);
   if (It == ProtocolDefaultImplMap.end())
      return nullptr;

   return It->getSecond()[Req];
}

const llvm::DenseMap<const NamedDecl*, std::vector<NamedDecl*>>*
ASTContext::getProtocolDefaultImpls(const ProtocolDecl* P)
{
   auto It = ProtocolDefaultImplMap.find(P);
   if (It == ProtocolDefaultImplMap.end())
      return nullptr;

   return &It->getSecond();
}

void ASTContext::addProtocolImpl(const RecordDecl* R, const NamedDecl* Req,
                                 NamedDecl* Impl)
{
   assert(R && Req && Impl && "invalid protocol impl");
   ProtocolImplMap[R][Req] = Impl;

   LOG(ProtocolImpls, "implementation of '", Req->getFullName(), "' for '",
       R->getFullName(), "': ", Impl->getFullName(), " [",
       CI.getFileMgr().getFullSourceLoc(Impl->getSourceLoc()), "]");
}

void ASTContext::updateProtocolImpl(const RecordDecl *R,
                                    NamedDecl *OldImpl,
                                    NamedDecl *NewImpl)
{
   const NamedDecl *Req = nullptr;
   for (auto &[req, impl] : ProtocolImplMap[OldImpl->getRecord()]) {
      if (impl == OldImpl) {
         Req = req;
         break;
      }
   }

   if (!Req)
      return;

   ProtocolImplMap[R][Req] = NewImpl;
}

NamedDecl* ASTContext::getProtocolImpl(const RecordDecl* R,
                                       const NamedDecl* Req)
{
   auto It = ProtocolImplMap.find(R);
   if (It == ProtocolImplMap.end())
      return nullptr;

   return It->getSecond()[Req];
}

const llvm::MapVector<const NamedDecl*, NamedDecl*>*
ASTContext::getProtocolImpls(const RecordDecl* R)
{
   auto It = ProtocolImplMap.find(R);
   if (It == ProtocolImplMap.end())
      return nullptr;

   return &It->getSecond();
}

PointerType* ASTContext::getPointerType(QualType pointeeType) const
{
   auto It = PointerTypes.find(pointeeType);
   if (It != PointerTypes.end()) {
      return It->getSecond();
   }

   Type* CanonicalTy = nullptr;
   if (!pointeeType.isCanonical()) {
      CanonicalTy = getPointerType(pointeeType.getCanonicalType());
   }

   auto New = new (*this, TypeAlignment) PointerType(pointeeType, CanonicalTy);
   PointerTypes[pointeeType] = New;

   return New;
}

MutablePointerType*
ASTContext::getMutablePointerType(QualType pointeeType) const
{
   auto It = MutablePointerTypes.find(pointeeType);
   if (It != MutablePointerTypes.end()) {
      return It->getSecond();
   }

   Type* CanonicalTy = nullptr;
   if (!pointeeType.isCanonical()) {
      CanonicalTy = getMutablePointerType(pointeeType.getCanonicalType());
   }

   auto New = new (*this, TypeAlignment)
       MutablePointerType(pointeeType, CanonicalTy);

   MutablePointerTypes[pointeeType] = New;
   return New;
}

ReferenceType* ASTContext::getReferenceType(QualType referencedType) const
{
   assert(!referencedType->isReferenceType() && "reference to reference type!");

   auto It = ReferenceTypes.find(referencedType);
   if (It != ReferenceTypes.end()) {
      return It->getSecond();
   }

   Type* CanonicalTy = nullptr;
   if (!referencedType.isCanonical()) {
      CanonicalTy = getReferenceType(referencedType.getCanonicalType());
   }

   auto New
       = new (*this, TypeAlignment) ReferenceType(referencedType, CanonicalTy);

   ReferenceTypes[referencedType] = New;
   return New;
}

MutableReferenceType*
ASTContext::getMutableReferenceType(QualType referencedType) const
{
   assert(!referencedType->isReferenceType() && "reference to reference type!");

   auto It = MutableReferenceTypes.find(referencedType);
   if (It != MutableReferenceTypes.end()) {
      return It->getSecond();
   }

   Type* CanonicalTy = nullptr;
   if (!referencedType.isCanonical()) {
      CanonicalTy = getMutableReferenceType(referencedType.getCanonicalType());
   }

   auto New = new (*this, TypeAlignment)
       MutableReferenceType(referencedType, CanonicalTy);

   MutableReferenceTypes[referencedType] = New;
   return New;
}

BoxType* ASTContext::getBoxType(QualType BoxedTy) const
{
   auto It = BoxTypes.find(BoxedTy);
   if (It != BoxTypes.end())
      return It->getSecond();

   Type* CanonicalTy = nullptr;
   if (!BoxedTy.isCanonical()) {
      CanonicalTy = getBoxType(BoxedTy.getCanonicalType());
   }

   auto New = new (*this, TypeAlignment) BoxType(BoxedTy, CanonicalTy);
   BoxTypes.try_emplace(BoxedTy, New);

   return New;
}

QualType ASTContext::getExistentialType(ArrayRef<QualType> Existentials) const
{
   if (Existentials.size() == 1) {
      return Existentials.front();
   }

   assert(!Existentials.empty() && "invalid existential type!");

   llvm::FoldingSetNodeID ID;
   ExistentialType::Profile(ID, Existentials);

   void* InsertPos;
   if (auto* T = ExistentialTypes.FindNodeOrInsertPos(ID, InsertPos)) {
      return T;
   }

   TypeProperties Props;
   bool Canonical = true;

   for (auto& P : Existentials) {
      Canonical &= P.isCanonical();
      Props |= P->properties();

      assert(P->isRecordType() && "existential must be RecordType!");
   }

   Type* CanonicalType = nullptr;
   if (!Canonical) {
      std::vector<QualType> canonicalArgs;
      for (auto& P : Existentials)
         canonicalArgs.push_back(P.getCanonicalType());

      CanonicalType = getExistentialType(canonicalArgs);

      // We need to get the insert position again since the folding set might
      // have grown.
      auto* NewTy = ExistentialTypes.FindNodeOrInsertPos(ID, InsertPos);
      assert(!NewTy && "type shouldn't exist!");
      (void)NewTy;
   }

   void* Mem = Allocate(sizeof(ExistentialType)
                            + sizeof(QualType) * Existentials.size(),
                        TypeAlignment);

   auto* T = new (Mem) ExistentialType(Existentials, CanonicalType, Props);
   ExistentialTypes.InsertNode(T, InsertPos);

   return T;
}

FunctionType*
ASTContext::getFunctionType(QualType returnType, ArrayRef<QualType> argTypes,
                            ArrayRef<FunctionType::ParamInfo> paramInfo,
                            unsigned flags, bool lambda) const
{
   if (lambda)
      return getLambdaType(returnType, argTypes, paramInfo, flags);

   llvm::FoldingSetNodeID ID;
   FunctionType::Profile(ID, returnType, argTypes, paramInfo, flags, false);

   void* insertPos = nullptr;
   if (FunctionType* Ptr = FunctionTypes.FindNodeOrInsertPos(ID, insertPos))
      return Ptr;

   TypeProperties Props = returnType->properties();
   bool Canonical = returnType.isCanonical();

   for (auto& arg : argTypes) {
      Canonical &= arg.isCanonical();
      Props |= arg->properties();
   }

   Type* CanonicalType = nullptr;
   if (!Canonical) {
      std::vector<QualType> canonicalArgs;
      for (auto& arg : argTypes)
         canonicalArgs.push_back(arg.getCanonicalType());

      CanonicalType = getFunctionType(returnType.getCanonicalType(),
                                      canonicalArgs, paramInfo, flags, lambda);

      // We need to get the insert position again since the folding set might
      // have grown.
      auto* NewTy = FunctionTypes.FindNodeOrInsertPos(ID, insertPos);
      assert(!NewTy && "type shouldn't exist!");
      (void)NewTy;
   }

   size_t SizeToAlloc = sizeof(FunctionType)
                        + sizeof(QualType) * argTypes.size()
                        + sizeof(FunctionType::ParamInfo) * paramInfo.size();

   void* Mem = Allocate(SizeToAlloc, TypeAlignment);
   auto FnTy = new (Mem)
       FunctionType(returnType, argTypes, paramInfo,
                    (FunctionType::ExtFlags)flags, CanonicalType, Props);

   FunctionTypes.InsertNode(FnTy, insertPos);
   return FnTy;
}

FunctionType* ASTContext::getFunctionType(QualType returnType,
                                          ArrayRef<QualType> argTypes,
                                          unsigned flags, bool lambda) const
{
   llvm::SmallVector<FunctionType::ParamInfo, 4> ParamInfo;
   ParamInfo.resize(argTypes.size());

   return getFunctionType(returnType, argTypes, ParamInfo, flags, lambda);
}

LambdaType* ASTContext::getLambdaType(FunctionType* FnTy)
{
   return getLambdaType(FnTy->getReturnType(), FnTy->getParamTypes(),
                        FnTy->getParamInfo(), FnTy->getRawFlags());
}

LambdaType*
ASTContext::getLambdaType(QualType returnType, ArrayRef<QualType> argTypes,
                          ArrayRef<FunctionType::ParamInfo> paramInfo,
                          unsigned int flags) const
{
   llvm::FoldingSetNodeID ID;
   FunctionType::Profile(ID, returnType, argTypes, paramInfo, flags, true);

   void* insertPos = nullptr;
   if (LambdaType* Ptr = LambdaTypes.FindNodeOrInsertPos(ID, insertPos))
      return Ptr;

   TypeProperties Props = returnType->properties();
   bool Canonical = returnType.isCanonical();

   for (auto& arg : argTypes) {
      Canonical &= arg.isCanonical();
      Props |= arg->properties();
   }

   Type* CanonicalType = nullptr;
   if (!Canonical) {
      std::vector<QualType> canonicalArgs;
      for (auto& arg : argTypes)
         canonicalArgs.push_back(arg.getCanonicalType());

      CanonicalType = getLambdaType(returnType.getCanonicalType(),
                                    canonicalArgs, paramInfo, flags);

      // We need to get the insert position again since the folding set might
      // have grown.
      auto* NewTy = LambdaTypes.FindNodeOrInsertPos(ID, insertPos);
      assert(!NewTy && "type shouldn't exist!");
      (void)NewTy;
   }

   size_t SizeToAlloc = sizeof(LambdaType) + sizeof(QualType) * argTypes.size()
                        + sizeof(FunctionType::ParamInfo) * paramInfo.size();

   void* Mem = Allocate(SizeToAlloc, TypeAlignment);
   auto New = new (Mem)
       LambdaType(returnType, argTypes, paramInfo,
                  (FunctionType::ExtFlags)flags, CanonicalType, Props);

   LambdaTypes.InsertNode(New, insertPos);
   return New;
}

LambdaType* ASTContext::getLambdaType(QualType returnType,
                                      ArrayRef<QualType> argTypes,
                                      unsigned int flags) const
{
   llvm::SmallVector<FunctionType::ParamInfo, 4> ParamInfo;
   ParamInfo.resize(argTypes.size());

   return getLambdaType(returnType, argTypes, ParamInfo, flags);
}

ArrayType* ASTContext::getArrayType(QualType elementType,
                                    unsigned numElements) const
{
   llvm::FoldingSetNodeID ID;
   ArrayType::Profile(ID, elementType, numElements);

   void* insertPos = nullptr;
   if (ArrayType* Ptr = ArrayTypes.FindNodeOrInsertPos(ID, insertPos))
      return Ptr;

   Type* CanonicalTy = nullptr;
   if (!elementType.isCanonical()) {
      CanonicalTy = getArrayType(elementType.getCanonicalType(), numElements);

      // We need to get the insert position again since the folding set might
      // have grown.
      auto* NewTy = ArrayTypes.FindNodeOrInsertPos(ID, insertPos);
      assert(!NewTy && "type shouldn't exist!");
      (void)NewTy;
   }

   auto New = new (*this, TypeAlignment)
       ArrayType(elementType, numElements, CanonicalTy);

   ArrayTypes.InsertNode(New, insertPos);
   return New;
}

DependentSizeArrayType*
ASTContext::getValueDependentSizedArrayType(QualType elementType,
                                            StaticExpr* DependentExpr) const
{
   Type* CanonicalTy = nullptr;
   if (!elementType.isCanonical())
      CanonicalTy = getValueDependentSizedArrayType(
          elementType.getCanonicalType(), DependentExpr);

   // these aren't uniqued, because the size expressions aren't either
   return new (*this, TypeAlignment)
       DependentSizeArrayType(elementType, DependentExpr, CanonicalTy);
}

InferredSizeArrayType* ASTContext::getInferredSizeArrayType(QualType elTy) const
{
   auto It = InferredSizeArrayTypes.find(elTy);
   if (It != InferredSizeArrayTypes.end()) {
      return It->getSecond();
   }

   Type* CanonicalTy = nullptr;
   if (!elTy.isCanonical()) {
      CanonicalTy = getInferredSizeArrayType(elTy.getCanonicalType());
   }

   auto New
       = new (*this, TypeAlignment) InferredSizeArrayType(elTy, CanonicalTy);

   InferredSizeArrayTypes[elTy] = New;
   return New;
}

TupleType* ASTContext::getTupleType(ArrayRef<QualType> containedTypes) const
{
   llvm::FoldingSetNodeID ID;
   TupleType::Profile(ID, containedTypes);

   void* insertPos = nullptr;
   if (TupleType* Ptr = TupleTypes.FindNodeOrInsertPos(ID, insertPos))
      return Ptr;

   TypeProperties Props;
   bool Canonical = true;

   for (auto& arg : containedTypes) {
      Canonical &= arg.isCanonical();
      Props |= arg->properties();
   }

   Type* CanonicalType = nullptr;
   if (!Canonical) {
      std::vector<QualType> canonicalElements;
      for (auto& arg : containedTypes)
         canonicalElements.push_back(arg.getCanonicalType());

      CanonicalType = getTupleType(canonicalElements);

      // We need to get the insert position again since the folding set might
      // have grown.
      auto* NewTy = TupleTypes.FindNodeOrInsertPos(ID, insertPos);
      assert(!NewTy && "type shouldn't exist!");
      (void)NewTy;
   }

   size_t SizeToAlloc
       = sizeof(TupleType) + sizeof(QualType) * containedTypes.size();

   void* Mem = Allocate(SizeToAlloc, TypeAlignment);
   auto TupTy = new (Mem) TupleType(containedTypes, CanonicalType, Props);

   TupleTypes.InsertNode(TupTy, insertPos);
   return TupTy;
}

static DependentRecordType* createTemplateType(ASTContext& C, RecordDecl* R)
{
   SmallVector<sema::TemplateArgument, 2> templateArgs;
   for (auto* param : R->getTemplateParams()) {
      if (param->isVariadic()) {
         templateArgs.emplace_back(param, param->isTypeName(),
                                   std::vector<sema::TemplateArgument>(),
                                   param->getSourceLoc());
      }
      else if (param->isTypeName()) {
         templateArgs.emplace_back(param, C.getTemplateParamType(param),
                                   param->getSourceLoc());
      }
      else {
         llvm_unreachable("not sure how to handle this!");
      }
   }

   auto* finalList = sema::FinalTemplateArgumentList::Create(C, templateArgs);
   auto* dependentType = C.getDependentRecordType(R, finalList);

   R->setType(dependentType);
   return dependentType;
}

RecordType* ASTContext::getRecordType(RecordDecl* R) const
{
   if (R->isTemplate()) {
      if (QualType T = R->getType()) {
         return T->asDependentRecordType();
      }

      return createTemplateType(const_cast<ASTContext&>(*this), R);
   }

   auto It = RecordTypes.find(R);
   if (It != RecordTypes.end()) {
      return It->getSecond();
   }

   RecordType* New = new (*this, TypeAlignment) RecordType(R);
   RecordTypes[R] = New;

   return New;
}

static bool isCanonical(const sema::TemplateArgument& Arg)
{
   if (!Arg.isType()) {
      return true;
   }
   if (Arg.isVariadic()) {
      for (auto& VA : Arg.getVariadicArgs()) {
         if (!isCanonical(VA)) {
            return false;
         }
      }
   }

   return Arg.getNonCanonicalType()->isCanonical();
}

static sema::TemplateArgument makeCanonical(const sema::TemplateArgument& Arg)
{
   if (!Arg.isType()) {
      return Arg.clone();
   }
   if (Arg.isVariadic()) {
      std::vector<sema::TemplateArgument> VariadicArgs;
      VariadicArgs.reserve(Arg.getVariadicArgs().size());

      for (auto& VA : Arg.getVariadicArgs()) {
         VariadicArgs.emplace_back(makeCanonical(VA));
      }

      return sema::TemplateArgument(Arg.getParam(), true, move(VariadicArgs),
                                    Arg.getLoc());
   }

   return sema::TemplateArgument(Arg.getParam(),
                                 Arg.getNonCanonicalType()->getCanonicalType(),
                                 Arg.getLoc());
}

DependentRecordType* ASTContext::getDependentRecordType(
    RecordDecl* R, sema::FinalTemplateArgumentList* args, QualType Parent) const
{
   if (R->isNestedTemplate()) {
      auto *RealTemplate = cast<RecordDecl>(R->getOuterTemplate());
      Parent = getDependentRecordType(RealTemplate, args);
      args = sema::FinalTemplateArgumentList::Create(
          const_cast<ASTContext&>(*this),
          MutableArrayRef<sema::TemplateArgument>());
   }

   assert(R->isTemplateOrInTemplate() && "not a template!");

   llvm::FoldingSetNodeID ID;
   DependentRecordType::Profile(ID, R, args, Parent);

   void* insertPos = nullptr;
   if (auto* Ptr = DependentRecordTypes.FindNodeOrInsertPos(ID, insertPos)) {
      return Ptr;
   }

   bool Canonical = true;
   for (auto& Arg : *args) {
      if (!isCanonical(Arg)) {
         Canonical = false;
         break;
      }
   }

   DependentRecordType* CanonicalType = nullptr;
   if (!Canonical) {
      SmallVector<sema::TemplateArgument, 4> CanonicalArgs;
      CanonicalArgs.reserve(args->size());

      for (auto& Arg : *args) {
         CanonicalArgs.emplace_back(makeCanonical(Arg));
      }

      auto* CanonicalList = sema::FinalTemplateArgumentList::Create(
          const_cast<ASTContext&>(*this), CanonicalArgs, true);

      CanonicalType = getDependentRecordType(
          R, CanonicalList, Parent ? Parent->getCanonicalType() : QualType());

      // We need to get the insert position again since the folding set might
      // have grown.
      auto* NewTy = DependentRecordTypes.FindNodeOrInsertPos(ID, insertPos);
      assert(!NewTy && "type shouldn't exist!");
      (void)NewTy;
   }

   auto New = new (*this, TypeAlignment)
       DependentRecordType(R, args, Parent, CanonicalType);

   DependentRecordTypes.InsertNode(New, insertPos);
   return New;
}

TemplateParamType*
ASTContext::getTemplateParamType(TemplateParamDecl* Param) const
{
   auto It = TemplateParamTypes.find(Param);
   if (It != TemplateParamTypes.end()) {
      return It->getSecond();
   }

   auto New = new (*this, TypeAlignment) TemplateParamType(Param);
   TemplateParamTypes[Param] = New;

   return New;
}

AssociatedType* ASTContext::getAssociatedType(AssociatedTypeDecl* AT,
                                              QualType OuterAT) const
{
   if (!OuterAT && !AT->isSelf()) {
      CI.getQueryContext().DeclareSelfAlias(AT->getRecord());
      auto* SelfDecl = AT->getRecord()->lookupSingle<AssociatedTypeDecl>(
          Identifiers.get("Self"));

      OuterAT = getAssociatedType(SelfDecl);
   }

   llvm::FoldingSetNodeID ID;
   AssociatedType::Profile(ID, AT, OuterAT);

   void* insertPos = nullptr;
   if (auto* Ptr = AssociatedTypes.FindNodeOrInsertPos(ID, insertPos))
      return Ptr;

   auto New = new (*this, TypeAlignment) AssociatedType(AT, OuterAT);
   AssociatedTypes.InsertNode(New, insertPos);

   return New;
}

DependentNameType*
ASTContext::getDependentNameType(NestedNameSpecifierWithLoc* Name) const
{
   llvm::FoldingSetNodeID ID;
   DependentNameType::Profile(ID, Name);

   void* insertPos = nullptr;
   if (auto* Ptr = DependentNameTypes.FindNodeOrInsertPos(ID, insertPos))
      return Ptr;

   auto New = new (*this, TypeAlignment) DependentNameType(Name);
   DependentNameTypes.InsertNode(New, insertPos);

   return New;
}

MetaType* ASTContext::getMetaType(QualType forType) const
{
   auto It = MetaTypes.find(forType);
   if (It != MetaTypes.end()) {
      return It->getSecond();
   }

   Type* CanonicalTy = nullptr;
   if (!forType.isCanonical()) {
      CanonicalTy = getMetaType(forType.getCanonicalType());
   }

   auto New = new (*this, TypeAlignment) MetaType(forType, CanonicalTy);
   MetaTypes[forType] = New;

   return New;
}

TypedefType* ASTContext::getTypedefType(AliasDecl* TD) const
{
   assert(TD->getType() && "should declare alias before using it!");

   auto It = TypedefTypes.find(TD);
   if (It != TypedefTypes.end()) {
      return It->getSecond();
   }

   auto New = new (*this, TypeAlignment) TypedefType(TD);
   TypedefTypes[TD] = New;

   return New;
}

TypeVariableType* ASTContext::getTypeVariableType(unsigned ID) const
{
   auto It = TypeVariableTypes.find(ID);
   if (It != TypeVariableTypes.end()) {
      return It->getSecond();
   }

   auto* T = new (*this, TypeAlignment) TypeVariableType(ID);
   TypeVariableTypes[ID] = T;

   return T;
}

DependentTypedefType* ASTContext::getDependentTypedefType(
    AliasDecl* td, sema::FinalTemplateArgumentList* args, QualType Parent) const
{
   assert(!td->isInstantiation() && "dependent instantiation?");

   llvm::FoldingSetNodeID ID;
   DependentTypedefType::Profile(ID, td, args, Parent);

   void* insertPos = nullptr;
   if (auto* Ptr = DependentTypedefTypes.FindNodeOrInsertPos(ID, insertPos)) {
      return Ptr;
   }

   QualType CanonicalType
       = td->getType()->asMetaType()->getUnderlyingType()->getCanonicalType();
   CI.getQueryContext().SubstTemplateParamTypes(CanonicalType, CanonicalType,
                                                *args, td->getSourceRange());

   auto New = new (*this, TypeAlignment)
       DependentTypedefType(td, args, Parent, CanonicalType);

   DependentTypedefTypes.InsertNode(New, insertPos);
   return New;
}

CallableDecl* ASTContext::getFunctionTemplateInstantiation(
    CallableDecl* Template, TemplateArgList& argList, void*& insertPos)
{
   llvm::FoldingSetNodeID ID;
   CallableDecl::Profile(ID, Template, argList);

   return FunctionTemplateInstatiations.FindNodeOrInsertPos(ID, insertPos);
}

RecordDecl* ASTContext::getRecordTemplateInstantiation(RecordDecl* Template,
                                                       TemplateArgList& argList,
                                                       void*& insertPos)
{
   llvm::FoldingSetNodeID ID;
   RecordDecl::Profile(ID, Template, argList);

   return RecordTemplateInstatiations.FindNodeOrInsertPos(ID, insertPos);
}

AliasDecl* ASTContext::getAliasTemplateInstantiation(AliasDecl* Template,
                                                     TemplateArgList& argList,
                                                     void*& insertPos)
{
   llvm::FoldingSetNodeID ID;
   AliasDecl::Profile(ID, Template, argList);

   return AliasTemplateInstatiations.FindNodeOrInsertPos(ID, insertPos);
}

NamedDecl* ASTContext::getTemplateInstantiation(NamedDecl* Template,
                                                TemplateArgList& argList,
                                                void*& insertPos)
{
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

void ASTContext::insertFunctionTemplateInstantiation(CallableDecl* Inst,
                                                     void* insertPos)
{
   assert(((uintptr_t)Inst & 1) == 0);
   assert(!Inst->getNextInBucket());

   registerInstantiation(Inst->getSpecializedTemplate(), Inst);
   FunctionTemplateInstatiations.InsertNode(Inst, insertPos);
}

void ASTContext::insertRecordTemplateInstantiation(RecordDecl* Inst,
                                                   void* insertPos)
{
   assert(((uintptr_t)Inst & 1) == 0);
   assert(!Inst->getNextInBucket());

   registerInstantiation(Inst->getSpecializedTemplate(), Inst);
   RecordTemplateInstatiations.InsertNode(Inst, insertPos);
}

void ASTContext::insertAliasTemplateInstantiation(AliasDecl* Inst,
                                                  void* insertPos)
{
   assert(((uintptr_t)Inst & 1) == 0);
   assert(!Inst->getNextInBucket());

   registerInstantiation(Inst->getSpecializedTemplate(), Inst);
   AliasTemplateInstatiations.InsertNode(Inst, insertPos);
}

void ASTContext::insertTemplateInstantiation(NamedDecl* Inst, void* insertPos)
{
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

ArrayRef<NamedDecl*> ASTContext::getInstantiationsOf(NamedDecl* Template)
{
   auto It = InstMap.find(Template);
   if (It == InstMap.end())
      return {};

   return It->getSecond();
}

void ASTContext::registerInstantiation(NamedDecl* Template, NamedDecl* Inst)
{
   InstMap[Template].push_back(Inst);
}

void ASTContext::initializeOpNames() const
{
#define Infix(Spelling) registerInfixOperator(&Identifiers.get(Spelling))
#define Prefix(Spelling) registerPrefixOperator(&Identifiers.get(Spelling))
#define Postfix(Spelling) registerPostfixOperator(&Identifiers.get(Spelling))
#define CDOT_OPERATOR(Name, Spelling, Precedence, Fix) Fix(Spelling);
#include "cdotc/Basic/BuiltinOperators.def"

#undef Infix
#undef Prefix
#undef Postfix
}

void ASTContext::cleanupDeclContext(DeclContext* DC)
{
   for (auto* D : DC->getDecls())
      cleanupDecl(D);

   if (auto* Ext = DC->ExtStorage) {
      if (auto* MF = Ext->ModFile)
         MF->~ModuleFile();

      Ext->~ExternalStorage();
   }

   switch (DC->getDeclKind()) {
   case Decl::StructDeclID:
   case Decl::ClassDeclID: {
      auto* R = cast<StructDecl>(DC);
      R->StoredFields.~SmallVector();
      break;
   }
   default:
      break;
   }

   DC->namedDecls.~SmallDenseMap();
}

void ASTContext::cleanupDecl(Decl* D)
{
   if (auto* DC = dyn_cast<DeclContext>(D)) {
      cleanupDeclContext(DC);
   }
}

void ASTContext::cleanup()
{
   // Walk the AST deleting DeclContext maps.
   cleanupDeclContext(&CI.getGlobalDeclCtx());

   for (auto& AttrPair : AttributeMap) {
      AttrPair.second->~SmallVector();
   }
   for (auto& ExtensionPair : ExtensionMap) {
      ExtensionPair.second->~SmallVector();
   }
}

ASTContext::ASTContext(CompilerInstance& CI)
    : CI(CI), Allocator(), DeclNames(*this),
      TI(CI, llvm::Triple(llvm::sys::getDefaultTargetTriple())),
#define CDOT_BUILTIN_TYPE(Name) Name##Ty(BuiltinType::Name),
#include "cdotc/Basic/BuiltinTypes.def"
      PointerTypes{}
{
   EmptyTupleTy = getTupleType({});
   EmptyConstraintSet = ConstraintSet::Create(*this, {});
}

} // namespace ast
} // namespace cdot
