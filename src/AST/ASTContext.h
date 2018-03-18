//
// Created by Jonas Zell on 18.01.18.
//

#ifndef CDOT_ASTCONTEXT_H
#define CDOT_ASTCONTEXT_H

#include "AST/ParentMap.h"
#include "Basic/IdentifierInfo.h"
#include "Basic/TargetInfo.h"

#include "Variant/Type/Type.h"

#include <llvm/Support/Allocator.h>
#include <llvm/ADT/FoldingSet.h>
#include <llvm/ADT/DenseMap.h>

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

   const TargetInfo &getTargetInfo() const { return TI; }

   using AttrVec = llvm::SmallVector<Attr*, 0>;

private:
   mutable ParentMap parentMap;
   mutable IdentifierTable Identifiers;
   mutable TargetInfo TI;
   mutable llvm::BumpPtrAllocator Allocator;
   mutable llvm::DenseMap<const Decl*, AttrVec*> AttributeMap;

#  define CDOT_BUILTIN_TYPE(Name)                              \
   alignas(TypeAlignment) mutable BuiltinType Name##Ty;
#  include "Basic/BuiltinTypes.def"

   mutable llvm::FoldingSet<PointerType> PointerTypes;
   mutable llvm::FoldingSet<ReferenceType> ReferenceTypes;
   mutable llvm::FoldingSet<MovedType> MovedTypes;
   mutable llvm::FoldingSet<FunctionType> FunctionTypes;
   mutable llvm::FoldingSet<LambdaType> LambdaTypes;
   mutable llvm::FoldingSet<ArrayType> ArrayTypes;
   mutable llvm::FoldingSet<InferredArrayType> ValueDependentArrayTypes;
   mutable llvm::FoldingSet<TupleType> TupleTypes;
   mutable llvm::FoldingSet<GenericType> GenericTypes;
   mutable llvm::FoldingSet<InconcreteObjectType> DependentRecordTypes;
   mutable llvm::FoldingSet<MetaType> MetaTypes;
   mutable llvm::FoldingSet<NamespaceType> NamespaceTypes;
   mutable llvm::FoldingSet<TypedefType> TypedefTypes;
   mutable llvm::FoldingSet<ObjectType> RecordTypes;

public:
   BuiltinType *getAutoType() const { return &AutoTy; }
   BuiltinType *getVoidType() const { return &VoidTy; }

   BuiltinType *getInt1Ty() const { return &i1Ty; }
   BuiltinType *getUInt1Ty() const { return &i1Ty; }

   BuiltinType *getBoolTy() const { return &i1Ty; }

   BuiltinType *getInt8Ty() const { return &i8Ty; }
   BuiltinType *getUInt8Ty() const { return &u8Ty; }
   BuiltinType *getCharTy() const { return &u8Ty; }

   BuiltinType *getInt16Ty() const { return &i16Ty; }
   BuiltinType *getUInt16Ty() const { return &u16Ty; }

   BuiltinType *getInt32Ty() const { return &i32Ty; }
   BuiltinType *getUInt32Ty() const { return &u32Ty; }

   BuiltinType *getInt64Ty() const { return &i64Ty; }
   BuiltinType *getUInt64Ty() const { return &u64Ty; }

#  define CDOT_BUILTIN_TYPE(Name)                           \
   BuiltinType *get##Name##Ty() const { return &Name##Ty; }
#  include "Basic/BuiltinTypes.def"

   BuiltinType *getBuiltinType(BuiltinType::Kind kind) const
   {
      switch (kind) {
#     define CDOT_BUILTIN_TYPE(Name)                           \
         case BuiltinType::Name: return &Name##Ty;
#     include "Basic/BuiltinTypes.def"
      }
   }

   BuiltinType *getIntTy() const
   {
      if constexpr (sizeof(void*) == 8) {
         return &i64Ty;
      }
      else {
         return &i32Ty;
      }
   }

   BuiltinType *getUIntTy() const
   {
      if constexpr (sizeof(void*) == 8) {
         return &u64Ty;
      }
      else {
         return &u32Ty;
      }
   }

   BuiltinType *getIntegerTy(unsigned bits, bool isUnsigned) const
   {
      if (!isUnsigned) {
         switch (bits) {
            case 1: return &i1Ty;
            case 8: return &i8Ty;
            case 16: return &i16Ty;
            case 32: return &i32Ty;
            case 64: return &i64Ty;
            default: llvm_unreachable("bad bitwidth");
         }
      }
      else {
         switch (bits) {
            case 1: return &i1Ty;
            case 8: return &u8Ty;
            case 16: return &u16Ty;
            case 32: return &u32Ty;
            case 64: return &u64Ty;
            default: llvm_unreachable("bad bitwidth");
         }
      }
   }

   PointerType *getUInt8PtrTy() const { return getPointerType(u8Ty); }
   PointerType *getInt8PtrTy() const { return getPointerType(i8Ty); }

   BuiltinType *getFloatTy() const { return &f32Ty; }
   BuiltinType *getDoubleTy() const { return &f64Ty; }

   PointerType *getPointerType(QualType pointeeType) const;
   ReferenceType *getReferenceType(QualType referencedType) const;
   MovedType *getMovedType(QualType referencedType) const;

   FunctionType *getFunctionType(QualType returnType,
                                 llvm::ArrayRef<QualType> argTypes,
                                 unsigned flags = 0,
                                 bool lambda = false) const;

   LambdaType *getLambdaType(FunctionType *FnTy);
   LambdaType *getLambdaType(QualType returnType,
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
                                          sema::TemplateArgList &&args) const;

   GenericType *getTemplateArgType(QualType covariance,
                                   llvm::StringRef typeName) const;

   NamespaceType *getNamespaceType(NamespaceDecl *NS) const;
   MetaType *getMetaType(QualType forType) const;
   TypedefType *getTypedefType(TypedefDecl *TD) const;

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

   llvm::ArrayRef<Attr*> getAttributes(const Decl *D) const;
   void setAttributes(const Decl *D, llvm::ArrayRef<Attr*> attrs) const;
   void addAttribute(const Decl *D, Attr* attr) const;
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
