//
// Created by Jonas Zell on 18.01.18.
//

#include "ASTContext.h"
#include "../Support/Casting.h"

using namespace cdot::support;

namespace cdot {
namespace ast {

ASTContext::ASTContext()
   : Allocator(),
     AutoTy(new (*this, TypeAlignment) AutoType),
     VoidTy(new (*this, TypeAlignment) VoidType),
     Int1Ty(new (*this, TypeAlignment) IntegerType(1)),
     Int8Ty(new (*this, TypeAlignment) IntegerType(8)),
     UInt8Ty(new (*this, TypeAlignment) IntegerType(8, true)),
     Int16Ty(new (*this, TypeAlignment) IntegerType(16)),
     UInt16Ty(new (*this, TypeAlignment) IntegerType(16, true)),
     Int32Ty(new (*this, TypeAlignment) IntegerType(32)),
     UInt32Ty(new (*this, TypeAlignment) IntegerType(32, true)),
     Int64Ty(new (*this, TypeAlignment) IntegerType(64)),
     UInt64Ty(new (*this, TypeAlignment) IntegerType(64, true)),
     Int8PtrTy(getPointerType(Int8Ty)),
     FloatTy(new (*this, TypeAlignment) FPType(32)),
     DoubleTy(new (*this, TypeAlignment) FPType(64))
{

}

PointerType* ASTContext::getPointerType(QualType pointeeType) const
{
   llvm::FoldingSetNodeID ID;
   PointerType::Profile(ID, pointeeType);

   void *insertPos = nullptr;
   if (PointerType *Ptr = PointerTypes.FindNodeOrInsertPos(ID, insertPos))
      return Ptr;

   auto New = new (*this, TypeAlignment) PointerType(pointeeType);
   PointerTypes.InsertNode(New, insertPos);

   return New;
}

FunctionType* ASTContext::getFunctionType(QualType returnType,
                                          llvm::ArrayRef<QualType> argTypes,
                                          unsigned flags) const {
   llvm::FoldingSetNodeID ID;
   FunctionType::Profile(ID, returnType, argTypes, flags);

   void *insertPos = nullptr;
   if (FunctionType *Ptr = FunctionTypes.FindNodeOrInsertPos(ID, insertPos))
      return Ptr;

   auto New = new (*this, TypeAlignment)
      FunctionType(returnType, argTypes, (FunctionType::ExtFlags)flags);

   FunctionTypes.InsertNode(New, insertPos);
   return New;
}

ArrayType* ASTContext::getArrayType(QualType elementType,
                                    size_t numElements) const {
   llvm::FoldingSetNodeID ID;
   ArrayType::Profile(ID, elementType, numElements);

   void *insertPos = nullptr;
   if (ArrayType *Ptr = ArrayTypes.FindNodeOrInsertPos(ID, insertPos))
      return Ptr;

   auto New = new (*this, TypeAlignment) ArrayType(elementType, numElements);
   ArrayTypes.InsertNode(New, insertPos);

   return New;
}

InferredArrayType* ASTContext::getValueDependentSizedArrayType(
                                               QualType elementType,
                                               Expression *DependentExpr)const {
   llvm::FoldingSetNodeID ID;
   InferredArrayType::Profile(ID, elementType, DependentExpr);

   void *insertPos = nullptr;
   if (auto *Ptr = ValueDependentArrayTypes.FindNodeOrInsertPos(ID, insertPos))
      return Ptr;

   auto New = new (*this, TypeAlignment) InferredArrayType(elementType,
                                                           DependentExpr);
   ValueDependentArrayTypes.InsertNode(New, insertPos);

   return New;
}

TupleType* ASTContext::getTupleType(llvm::ArrayRef<QualType> containedTypes)
const
{
   llvm::FoldingSetNodeID ID;
   TupleType::Profile(ID, containedTypes);

   void *insertPos = nullptr;
   if (TupleType *Ptr = TupleTypes.FindNodeOrInsertPos(ID, insertPos))
      return Ptr;

   auto New = new (*this, TypeAlignment) TupleType(containedTypes);
   TupleTypes.InsertNode(New, insertPos);

   return New;
}

ObjectType* ASTContext::getRecordType(RecordDecl *R, Type::BoxedPrimitive kind)
const
{
   llvm::FoldingSetNodeID ID;
   ObjectType::Profile(ID, R);

   void *insertPos = nullptr;
   if (auto *Ptr = RecordTypes.FindNodeOrInsertPos(ID, insertPos))
      return Ptr;

   auto New = new (*this, TypeAlignment) ObjectType(R, kind);
   RecordTypes.InsertNode(New, insertPos);

   return New;
}

InconcreteObjectType* ASTContext::getDependentRecordType(
                                             RecordDecl *R,
                                             sema::TemplateArgList *args)const {
   llvm::FoldingSetNodeID ID;
   InconcreteObjectType::Profile(ID, R, args);

   void *insertPos = nullptr;
   if (auto *Ptr = DependentRecordTypes.FindNodeOrInsertPos(ID, insertPos))
      return Ptr;

   auto New = new (*this, TypeAlignment) InconcreteObjectType(R, args);
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

   auto New = new (*this, TypeAlignment) MetaType(forType);
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

} // namespace ast
} // namespace cdot