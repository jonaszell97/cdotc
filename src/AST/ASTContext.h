//
// Created by Jonas Zell on 18.01.18.
//

#ifndef CDOT_ASTCONTEXT_H
#define CDOT_ASTCONTEXT_H

#include <llvm/Support/Allocator.h>
#include <llvm/ADT/FoldingSet.h>

#include "Variant/Type/Type.h"
#include "Basic/IdentifierInfo.h"

#include "AST/Statement/Declaration/CallableDecl.h"
#include "AST/Statement/Declaration/Class/RecordDecl.h"
#include "AST/Statement/Declaration/TypedefDecl.h"

#include "AST/ParentMap.h"

namespace cdot {

namespace sema {
   class TemplateArgList;
} // namespace sema

namespace ast {

class Expression;
class Statement;

class CallableDecl;
class RecordDecl;
class AliasDecl;

class ASTContext {
public:
   ASTContext();
   ~ASTContext() = default;

   void *Allocate(size_t size, size_t alignment = 8) const
   {
      return Allocator.Allocate(size, alignment);
   }

   template <typename T>
   T *Allocate(size_t Num = 1) const
   {
      return static_cast<T *>(Allocate(Num * sizeof(T), alignof(T)));
   }

   void Deallocate(void *Ptr) const {}

   IdentifierTable &getIdentifiers() const
   {
      return Identifiers;
   }

   ParentMap &getParentMap()
   {
      return parentMap;
   }

private:
   mutable ParentMap parentMap;
   mutable IdentifierTable Identifiers;
   mutable llvm::BumpPtrAllocator Allocator;

   mutable llvm::FoldingSet<PointerType> PointerTypes;
   mutable llvm::FoldingSet<FunctionType> FunctionTypes;
   mutable llvm::FoldingSet<ArrayType> ArrayTypes;
   mutable llvm::FoldingSet<InferredArrayType> ValueDependentArrayTypes;
   mutable llvm::FoldingSet<TupleType> TupleTypes;
   mutable llvm::FoldingSet<GenericType> GenericTypes;
   mutable llvm::FoldingSet<InconcreteObjectType> DependentRecordTypes;
   mutable llvm::FoldingSet<MetaType> MetaTypes;
   mutable llvm::FoldingSet<NamespaceType> NamespaceTypes;
   mutable llvm::FoldingSet<TypedefType> TypedefTypes;
   mutable llvm::FoldingSet<ObjectType> RecordTypes;

   mutable AutoType *AutoTy;
   mutable VoidType *VoidTy;

   mutable IntegerType *Int1Ty;
   mutable IntegerType *Int8Ty;
   mutable IntegerType *UInt8Ty;
   mutable IntegerType *Int16Ty;
   mutable IntegerType *UInt16Ty;
   mutable IntegerType *Int32Ty;
   mutable IntegerType *UInt32Ty;
   mutable IntegerType *Int64Ty;
   mutable IntegerType *UInt64Ty;

   mutable PointerType *Int8PtrTy;

   mutable FPType *FloatTy;
   mutable FPType *DoubleTy;

   alignas(TypeAlignment) mutable UnknownAnyType UnknownAnyTy;
   
public:
   AutoType *getAutoType() const { return AutoTy; }
   VoidType *getVoidType() const { return VoidTy; }

   IntegerType *getInt1Ty() const { return Int1Ty; }
   IntegerType *getUInt1Ty() const { return Int1Ty; }

   IntegerType *getBoolTy() const { return Int1Ty; }

   IntegerType *getInt8Ty() const { return Int8Ty; }
   IntegerType *getUInt8Ty() const { return UInt8Ty; }
   IntegerType *getCharTy() const { return UInt8Ty; }

   IntegerType *getInt16Ty() const { return Int16Ty; }
   IntegerType *getUInt16Ty() const { return UInt16Ty; }

   IntegerType *getInt32Ty() const { return Int32Ty; }
   IntegerType *getUInt32Ty() const { return UInt32Ty; }

   IntegerType *getInt64Ty() const { return Int64Ty; }
   IntegerType *getUInt64Ty() const { return UInt64Ty; }

   IntegerType *getIntTy() const
   {
      if constexpr (sizeof(void*) == 8) {
         return Int64Ty;
      }
      else {
         return Int32Ty;
      }
   }

   IntegerType *getUIntTy() const
   {
      if constexpr (sizeof(void*) == 8) {
         return UInt64Ty;
      }
      else {
         return UInt32Ty;
      }
   }

   IntegerType *getIntegerTy(unsigned bits, bool isUnsigned) const
   {
      if (!isUnsigned) {
         switch (bits) {
            case 1: return Int1Ty;
            case 8: return Int8Ty;
            case 16: return Int16Ty;
            case 32: return Int32Ty;
            case 64: return Int64Ty;
            default: llvm_unreachable("bad bitwidth");
         }
      }
      else {
         switch (bits) {
            case 1: return Int1Ty;
            case 8: return UInt8Ty;
            case 16: return UInt16Ty;
            case 32: return UInt32Ty;
            case 64: return UInt64Ty;
            default: llvm_unreachable("bad bitwidth");
         }
      }
   }

   PointerType *getInt8PtrTy() const { return Int8PtrTy; }

   FPType *getFloatTy() const { return FloatTy; }
   FPType *getDoubleTy() const { return DoubleTy; }

   PointerType *getPointerType(QualType pointeeType) const;
   FunctionType *getFunctionType(QualType returnType,
                                 llvm::ArrayRef<QualType> argTypes,
                                 unsigned flags = 0) const;

   ArrayType *getArrayType(QualType elementType, size_t numElements) const;
   InferredArrayType *getValueDependentSizedArrayType(
                                             QualType elementType,
                                             Expression*DependentExpr) const;

   TupleType *getTupleType(llvm::ArrayRef<QualType> containedTypes) const;

   ObjectType *getRecordType(
            RecordDecl *R,
            Type::BoxedPrimitive kind = Type::BoxedPrimitive::BP_None) const;

   InconcreteObjectType *getDependentRecordType(
                                          RecordDecl *R,
                                          sema::TemplateArgList *args) const;

   GenericType *getTemplateArgType(QualType covariance,
                                   llvm::StringRef typeName) const;

   NamespaceType *getNamespaceType(NamespaceDecl *NS) const;
   MetaType *getMetaType(QualType forType) const;
   TypedefType *getTypedefType(TypedefDecl *TD) const;

   UnknownAnyType *getUnknownAnyTy() const { return &UnknownAnyTy; }

public:
   mutable llvm::FoldingSet<CallableDecl> FunctionTemplateInstatiations;
   mutable llvm::FoldingSet<RecordDecl>   RecordTemplateInstatiations;
   mutable llvm::FoldingSet<AliasDecl>    AliasTemplateInstatiations;

   CallableDecl *getFunctionTemplateInstantiation(CallableDecl *Template,
                                                  sema::TemplateArgList&argList,
                                                  void *&insertPos);

   RecordDecl *getRecordTemplateInstantiation(RecordDecl *Template,
                                                sema::TemplateArgList &argList,
                                                void *&insertPos);

   AliasDecl *getAliasTemplateInstantiation(AliasDecl *Template,
                                            sema::TemplateArgList &argList,
                                            void *&insertPos);

   void insertFunctionTemplateInstantiation(CallableDecl *Inst,
                                            void *insertPos) {
      FunctionTemplateInstatiations.InsertNode(Inst, insertPos);
   }

   void insertRecordTemplateInstantiation(RecordDecl *Inst,
                                          void *insertPos) {
      RecordTemplateInstatiations.InsertNode(Inst, insertPos);
   }

   void insertAliasTemplateInstantiation(AliasDecl *Inst,
                                         void *insertPos) {
      AliasTemplateInstatiations.InsertNode(Inst, insertPos);
   }
};

} // namespace ast
} // namespace cdot

inline void *operator new(size_t size, cdot::ast::ASTContext const& Ctx,
                          size_t alignment = 8) {
   return Ctx.Allocate(size, alignment);
}

inline void operator delete(void *ptr, cdot::ast::ASTContext const& Ctx,
                            size_t) {
   return Ctx.Deallocate(ptr);
}

inline void *operator new[](size_t size, cdot::ast::ASTContext const& Ctx,
                            size_t alignment = 8) {
   return Ctx.Allocate(size, alignment);
}

inline void operator delete[](void *ptr, cdot::ast::ASTContext const& Ctx,
                              size_t) {
   return Ctx.Deallocate(ptr);
}

#endif //CDOT_ASTCONTEXT_H
