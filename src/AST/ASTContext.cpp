//
// Created by Jonas Zell on 18.01.18.
//

#include "ASTContext.h"

#include "Decl.h"
#include "Driver/Compiler.h"
#include "Serialization/ModuleFile.h"
#include "Support/Casting.h"

using namespace cdot::support;

namespace cdot {
namespace ast {

llvm::ArrayRef<Attr*> ASTContext::getAttributes(const Decl *D) const
{
   auto it = AttributeMap.find(D);
   if (it == AttributeMap.end() || !it->getSecond())
      return {};

   return *it->getSecond();
}

void ASTContext::setAttributes(const Decl *D,
                               llvm::ArrayRef<Attr*> attrs) const {
   AttributeMap[D] = new(*this) AttrVec(attrs.begin(), attrs.end());
}

void ASTContext::addAttributes(const Decl *D,
                               llvm::ArrayRef<Attr *> attrs) const {
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

llvm::ArrayRef<StaticExpr*> ASTContext::getConstraints(const Decl *D) const
{
   auto it = ConstraintMap.find(D);
   if (it == ConstraintMap.end() || !it->getSecond())
      return {};

   return *it->getSecond();
}

void ASTContext::setConstraints(const Decl *D,
                                llvm::ArrayRef<StaticExpr *> cvec) const {
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

llvm::ArrayRef<ExtensionDecl*>
ASTContext::getExtensions(const RecordDecl *R) const
{
   auto it = ExtensionMap.find(R);
   if (it == ExtensionMap.end() || !it->getSecond())
      return {};

   return *it->getSecond();
}

void ASTContext::addExtension(const RecordDecl *R,
                              ExtensionDecl *E) const {
   auto it = ExtensionMap.find(R);
   if (it != ExtensionMap.end()) {
      it->getSecond()->push_back(E);
   }
   else {
      auto Vec = new(*this) ExtensionVec;
      Vec->push_back(E);

      ExtensionMap[R] = Vec;
   }
}

void ASTContext::addProtocolDefaultImpl(const ProtocolDecl *P,
                                        const NamedDecl *Req,
                                        NamedDecl *Impl) {
   ProtocolDefaultImplMap[P][Req] = Impl;
}

NamedDecl*
ASTContext::getProtocolDefaultImpl(const ProtocolDecl *P,const NamedDecl *Req)
{
   auto It = ProtocolDefaultImplMap.find(P);
   if (It == ProtocolDefaultImplMap.end())
      return nullptr;

   return It->getSecond()[Req];
}

const llvm::DenseMap<const NamedDecl*, NamedDecl*>*
ASTContext::getProtocolDefaultImpls(const ProtocolDecl *P)
{
   auto It = ProtocolDefaultImplMap.find(P);
   if (It == ProtocolDefaultImplMap.end())
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

FunctionType* ASTContext::getFunctionType(QualType returnType,
                              llvm::ArrayRef<QualType> argTypes,
                              llvm::ArrayRef<FunctionType::ParamInfo> paramInfo,
                              unsigned flags,
                              bool lambda) const {
   if (lambda)
      return getLambdaType(returnType, argTypes, paramInfo, flags);

   llvm::FoldingSetNodeID ID;
   FunctionType::Profile(ID, returnType, argTypes, paramInfo, flags, false);

   void *insertPos = nullptr;
   if (FunctionType *Ptr = FunctionTypes.FindNodeOrInsertPos(ID, insertPos))
      return Ptr;

   bool Dependent = returnType->isDependentType();
   bool Canonical = returnType.isCanonical();

   for (auto &arg : argTypes) {
      Canonical &= arg.isCanonical();
      Dependent |= arg->isDependentType();
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
                                     CanonicalType, Dependent);

   FunctionTypes.InsertNode(FnTy, insertPos);
   return FnTy;
}

FunctionType* ASTContext::getFunctionType(QualType returnType,
                                          llvm::ArrayRef<QualType> argTypes,
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
                             llvm::ArrayRef<QualType> argTypes,
                             llvm::ArrayRef<FunctionType::ParamInfo> paramInfo,
                             unsigned int flags) const {
   llvm::FoldingSetNodeID ID;
   FunctionType::Profile(ID, returnType, argTypes, paramInfo, flags, true);

   void *insertPos = nullptr;
   if (LambdaType *Ptr = LambdaTypes.FindNodeOrInsertPos(ID, insertPos))
      return Ptr;

   bool Dependent = returnType->isDependentType();
   bool Canonical = returnType.isCanonical();

   for (auto &arg : argTypes) {
      Canonical &= arg.isCanonical();
      Dependent |= arg->isDependentType();
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
                                  CanonicalType, Dependent);

   LambdaTypes.InsertNode(New, insertPos);
   return New;
}

LambdaType* ASTContext::getLambdaType(QualType returnType,
                                      llvm::ArrayRef<QualType> argTypes,
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
ASTContext::getTupleType(llvm::ArrayRef<QualType> containedTypes) const
{
   llvm::FoldingSetNodeID ID;
   TupleType::Profile(ID, containedTypes);

   void *insertPos = nullptr;
   if (TupleType *Ptr = TupleTypes.FindNodeOrInsertPos(ID, insertPos))
      return Ptr;

   bool Dependent = false;
   bool Canonical = true;

   for (auto &arg : containedTypes) {
      Canonical &= arg.isCanonical();
      Dependent |= arg->isDependentType();
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
                                   Dependent);

   TupleTypes.InsertNode(TupTy, insertPos);
   return TupTy;
}

static void addArg(const sema::ResolvedTemplateArg &arg,
                   std::vector<QualType> &vec) {
   if (arg.isVariadic()) {
      for (auto &VA : arg.getVariadicArgs())
         addArg(VA, vec);
   }
   else if (arg.isType()) {
      vec.push_back(arg.getType());
   }
}

static std::vector<QualType> getTypeTemplateParams(
   const sema::FinalTemplateArgumentList &list)
{
   std::vector<QualType> vec;
   for (auto &arg : list) {
      addArg(arg, vec);
   }

   return vec;
}

RecordType* ASTContext::getRecordType(RecordDecl *R) const
{
   llvm::FoldingSetNodeID ID;
   RecordType::Profile(ID, R);

   void *insertPos = nullptr;
   if (auto *Ptr = RecordTypes.FindNodeOrInsertPos(ID, insertPos))
      return Ptr;

   RecordType *New = new(*this, TypeAlignment) RecordType(R, {});
   RecordTypes.InsertNode(New, insertPos);

   return New;
}

DependentRecordType*
ASTContext::getDependentRecordType(RecordDecl *R,
                                   sema::FinalTemplateArgumentList *args)const{
   llvm::FoldingSetNodeID ID;
   DependentRecordType::Profile(ID, R, args);

   void *insertPos = nullptr;
   if (auto *Ptr = DependentRecordTypes.FindNodeOrInsertPos(ID, insertPos))
      return Ptr;

   auto typeParams = getTypeTemplateParams(*args);
   void *Mem = Allocate(sizeof(DependentRecordType)
                        + typeParams.size() * sizeof(QualType),
                        TypeAlignment);

   auto New = new(Mem) DependentRecordType(R, args, typeParams);

   DependentRecordTypes.InsertNode(New, insertPos);
   return New;
}

GenericType* ASTContext::getTemplateArgType(TemplateParamDecl *Param) const
{
   llvm::FoldingSetNodeID ID;
   GenericType::Profile(ID, Param);

   void *insertPos = nullptr;
   if (auto *Ptr = GenericTypes.FindNodeOrInsertPos(ID, insertPos))
      return Ptr;

   auto New = new (*this, TypeAlignment) GenericType(Param);
   GenericTypes.InsertNode(New, insertPos);

   return New;
}

AssociatedType* ASTContext::getAssociatedType(AssociatedTypeDecl *AT) const
{
   llvm::FoldingSetNodeID ID;
   AssociatedType::Profile(ID, AT);

   void *insertPos = nullptr;
   if (auto *Ptr = AssociatedTypes.FindNodeOrInsertPos(ID, insertPos))
      return Ptr;

   auto New = new (*this, TypeAlignment) AssociatedType(AT);
   AssociatedTypes.InsertNode(New, insertPos);

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

TypedefType* ASTContext::getTypedefType(cdot::ast::TypedefDecl *TD) const
{
   llvm::FoldingSetNodeID ID;
   TypedefType::Profile(ID, TD);

   void *insertPos = nullptr;
   if (auto *Ptr = TypedefTypes.FindNodeOrInsertPos(ID, insertPos))
      return Ptr;

   auto New = new (*this, TypeAlignment) TypedefType(TD);
   TypedefTypes.InsertNode(New, insertPos);

   return New;
}

CallableDecl*
ASTContext::getFunctionTemplateInstantiation(CallableDecl *Template,
                                             TemplateArgs &argList,
                                             void *&insertPos) {
   llvm::FoldingSetNodeID ID;
   CallableDecl::Profile(ID, Template, argList);

   return FunctionTemplateInstatiations.FindNodeOrInsertPos(ID, insertPos);
}

RecordDecl*
ASTContext::getRecordTemplateInstantiation(RecordDecl *Template,
                                           TemplateArgs &argList,
                                           void *&insertPos) {
   llvm::FoldingSetNodeID ID;
   RecordDecl::Profile(ID, Template, argList);

   return RecordTemplateInstatiations.FindNodeOrInsertPos(ID, insertPos);
}

AliasDecl*
ASTContext::getAliasTemplateInstantiation(AliasDecl *Template,
                                          TemplateArgs &argList,
                                          void *&insertPos) {
   llvm::FoldingSetNodeID ID;
   AliasDecl::Profile(ID, Template, argList);

   return AliasTemplateInstatiations.FindNodeOrInsertPos(ID, insertPos);
}

NamedDecl* ASTContext::getTemplateInstantiation(NamedDecl *Template,
                                                TemplateArgs &argList,
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

void ASTContext::cleanup(CompilerInstance &CI)
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

ASTContext::ASTContext()
   : Allocator(), DeclNames(*this),
     TI(*this, llvm::Triple(llvm::sys::getDefaultTargetTriple())),
#  define CDOT_BUILTIN_TYPE(Name)            \
     Name##Ty(BuiltinType::Name),
#  include "Basic/BuiltinTypes.def"
   PointerTypes{}
{
   EmptyTupleTy = getTupleType({});
}

} // namespace ast
} // namespace cdot
