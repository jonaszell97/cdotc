//
// Created by Jonas Zell on 18.01.18.
//

#include "ASTContext.h"
#include "Decl.h"
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

PointerType* ASTContext::getPointerType(QualType pointeeType) const
{
   llvm::FoldingSetNodeID ID;
   PointerType::Profile(ID, pointeeType);

   void *insertPos = nullptr;
   if (PointerType *Ptr = PointerTypes.FindNodeOrInsertPos(ID, insertPos))
      return Ptr;

   Type *CanonicalTy = nullptr;
   if (!pointeeType.isCanonical())
      CanonicalTy = getPointerType(pointeeType.getCanonicalType());

   auto New = new (*this, TypeAlignment) PointerType(pointeeType, CanonicalTy);

   PointerTypes.InsertNode(New, insertPos);
   return New;
}

ReferenceType* ASTContext::getReferenceType(QualType referencedType) const
{
   llvm::FoldingSetNodeID ID;
   ReferenceType::Profile(ID, referencedType);

   void *insertPos = nullptr;
   if (ReferenceType *Ptr = ReferenceTypes.FindNodeOrInsertPos(ID, insertPos))
      return Ptr;

   Type *CanonicalTy = nullptr;
   if (!referencedType.isCanonical())
      CanonicalTy = getReferenceType(referencedType.getCanonicalType());

   auto New = new (*this, TypeAlignment) ReferenceType(referencedType,
                                                       CanonicalTy);

   ReferenceTypes.InsertNode(New, insertPos);
   return New;
}

MovedType* ASTContext::getMovedType(QualType referencedType) const
{
   llvm::FoldingSetNodeID ID;
   MovedType::Profile(ID, referencedType);

   void *insertPos = nullptr;
   if (MovedType *Ptr = MovedTypes.FindNodeOrInsertPos(ID, insertPos))
      return Ptr;

   Type *CanonicalTy = nullptr;
   if (!referencedType.isCanonical())
      CanonicalTy = getMovedType(referencedType.getCanonicalType());

   auto New = new (*this, TypeAlignment) MovedType(referencedType,
                                                   CanonicalTy);

   MovedTypes.InsertNode(New, insertPos);
   return New;
}

FunctionType* ASTContext::getFunctionType(QualType returnType,
                                          llvm::ArrayRef<QualType> argTypes,
                                          unsigned flags,
                                          bool lambda) const {
   if (lambda)
      return getLambdaType(returnType, argTypes, flags);

   llvm::FoldingSetNodeID ID;
   FunctionType::Profile(ID, returnType, argTypes, flags, false);

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
                                      canonicalArgs, flags, lambda);
   }

   size_t SizeToAlloc = sizeof(FunctionType)
                        + sizeof(QualType) * argTypes.size();

   void *Mem = Allocate(SizeToAlloc, TypeAlignment);
   auto FnTy = new(Mem) FunctionType(returnType, argTypes,
                                     (FunctionType::ExtFlags)flags,
                                     CanonicalType, Dependent);

   FunctionTypes.InsertNode(FnTy, insertPos);
   return FnTy;
}

LambdaType* ASTContext::getLambdaType(FunctionType *FnTy)
{
   return getLambdaType(FnTy->getReturnType(), FnTy->getParamTypes(),
                        FnTy->getRawFlags());
}

LambdaType* ASTContext::getLambdaType(QualType returnType,
                                      llvm::ArrayRef<QualType> argTypes,
                                      unsigned int flags) const {
   llvm::FoldingSetNodeID ID;
   FunctionType::Profile(ID, returnType, argTypes, flags, true);

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
                                    canonicalArgs, flags);
   }

   size_t SizeToAlloc = sizeof(LambdaType)
                        + sizeof(QualType) * argTypes.size();

   void *Mem = Allocate(SizeToAlloc, TypeAlignment);
   auto New = new(Mem) LambdaType(returnType, argTypes,
                                  (FunctionType::ExtFlags)flags,
                                  CanonicalType, Dependent);

   LambdaTypes.InsertNode(New, insertPos);
   return New;
}

ArrayType* ASTContext::getArrayType(QualType elementType,
                                    size_t numElements) const {
   llvm::FoldingSetNodeID ID;
   ArrayType::Profile(ID, elementType, numElements);

   void *insertPos = nullptr;
   if (ArrayType *Ptr = ArrayTypes.FindNodeOrInsertPos(ID, insertPos))
      return Ptr;

   Type *CanonicalTy = nullptr;
   if (!elementType.isCanonical())
      CanonicalTy = getArrayType(elementType.getCanonicalType(), numElements);

   auto New = new (*this, TypeAlignment) ArrayType(elementType, numElements,
                                                   CanonicalTy);

   ArrayTypes.InsertNode(New, insertPos);
   return New;
}

DependentSizeArrayType*
ASTContext::getValueDependentSizedArrayType(QualType elementType,
                                            Expression *DependentExpr) const {
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
   if (!elTy.isCanonical())
      CanonicalTy = getInferredSizeArrayType(elTy.getCanonicalType());

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
   }

   size_t SizeToAlloc = sizeof(TupleType)
                        + sizeof(QualType) * containedTypes.size();

   void *Mem = Allocate(SizeToAlloc, TypeAlignment);
   auto TupTy = new(Mem) TupleType(containedTypes, CanonicalType,
                                   Dependent);

   TupleTypes.InsertNode(TupTy, insertPos);
   return TupTy;
}

RecordType* ASTContext::getRecordType(RecordDecl *R) const
{
   llvm::FoldingSetNodeID ID;
   RecordType::Profile(ID, R);

   void *insertPos = nullptr;
   if (auto *Ptr = RecordTypes.FindNodeOrInsertPos(ID, insertPos))
      return Ptr;

   auto New = new (*this, TypeAlignment) RecordType(R);
   RecordTypes.InsertNode(New, insertPos);

   return New;
}

DependentRecordType*
ASTContext::getDependentRecordType(RecordDecl *R,
                                   sema::TemplateArgList &&args) const {
   llvm::FoldingSetNodeID ID;
   DependentRecordType::Profile(ID, R, &args);

   void *insertPos = nullptr;
   if (auto *Ptr = DependentRecordTypes.FindNodeOrInsertPos(ID, insertPos))
      return Ptr;

   auto New = new(*this, TypeAlignment)
      DependentRecordType(R, new(*this) sema::TemplateArgList(std::move(args)));

   DependentRecordTypes.InsertNode(New, insertPos);
   return New;
}

GenericType* ASTContext::getTemplateArgType(QualType covariance,
                                            llvm::StringRef typeName) const {
   llvm::FoldingSetNodeID ID;
   GenericType::Profile(ID, covariance, typeName);

   void *insertPos = nullptr;
   if (auto *Ptr = GenericTypes.FindNodeOrInsertPos(ID, insertPos))
      return Ptr;

   auto New = new (*this, TypeAlignment) GenericType(covariance, typeName);
   GenericTypes.InsertNode(New, insertPos);

   return New;
}

NamespaceType* ASTContext::getNamespaceType(NamespaceDecl *NS) const
{
   llvm::FoldingSetNodeID ID;
   NamespaceType::Profile(ID, NS);

   void *insertPos = nullptr;
   if (auto *Ptr = NamespaceTypes.FindNodeOrInsertPos(ID, insertPos))
      return Ptr;

   auto New = new (*this, TypeAlignment) NamespaceType(NS);
   NamespaceTypes.InsertNode(New, insertPos);

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
   if (!forType.isCanonical())
      CanonicalTy = getMetaType(forType.getCanonicalType());

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
                                             sema::TemplateArgList &argList,
                                             void *&insertPos) {
   llvm::FoldingSetNodeID ID;
   CallableDecl::Profile(ID, Template, argList);

   return FunctionTemplateInstatiations.FindNodeOrInsertPos(ID, insertPos);
}

RecordDecl*
ASTContext::getRecordTemplateInstantiation(RecordDecl *Template,
                                           sema::TemplateArgList &argList,
                                           void *&insertPos) {
   llvm::FoldingSetNodeID ID;
   RecordDecl::Profile(ID, Template, argList);

   return RecordTemplateInstatiations.FindNodeOrInsertPos(ID, insertPos);
}

AliasDecl*
ASTContext::getAliasTemplateInstantiation(AliasDecl *Template,
                                           sema::TemplateArgList &argList,
                                           void *&insertPos) {
   llvm::FoldingSetNodeID ID;
   AliasDecl::Profile(ID, Template, argList);

   return AliasTemplateInstatiations.FindNodeOrInsertPos(ID, insertPos);
}

void ASTContext::insertFunctionTemplateInstantiation(CallableDecl *Inst,
                                                     void *insertPos) {
   FunctionTemplateInstatiations.InsertNode(Inst, insertPos);
}

void ASTContext::insertRecordTemplateInstantiation(RecordDecl *Inst,
                                                   void *insertPos) {
   RecordTemplateInstatiations.InsertNode(Inst, insertPos);
}

void ASTContext::insertAliasTemplateInstantiation(AliasDecl *Inst,
                                                  void *insertPos) {
   AliasTemplateInstatiations.InsertNode(Inst, insertPos);
}

ASTContext::ASTContext()
   : Allocator(), DeclNames(*this),
     TI(*this, llvm::Triple(llvm::sys::getDefaultTargetTriple())),
#  define CDOT_BUILTIN_TYPE(Name)            \
     Name##Ty(BuiltinType::Name),
#  include "Basic/BuiltinTypes.def"
   PointerTypes{}
{

}

} // namespace ast
} // namespace cdot