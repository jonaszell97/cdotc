#ifndef CDOT_ASTCONTEXT_H
#define CDOT_ASTCONTEXT_H

#include "cdotc/AST/ConformanceTable.h"
#include "cdotc/AST/DeclDenseMapInfo.h"
#include "cdotc/AST/Type.h"
#include "cdotc/Basic/DeclarationName.h"
#include "cdotc/Basic/TargetInfo.h"
#include "cdotc/Sema/Template.h"

#include <llvm/ADT/DenseMap.h>
#include <llvm/ADT/FoldingSet.h>
#include <llvm/ADT/MapVector.h>
#include <llvm/ADT/SetVector.h>
#include <llvm/Support/Allocator.h>

namespace cdot {

namespace sema {
class TemplateArgList;
} // namespace sema

class Attr;
class CompilerInstance;

namespace ast {

class Expression;
class StaticExpr;
class Statement;
class Decl;
class DeclContext;
class DeclConstraint;
class ConstraintSet;
class NamedDecl;
class CallableDecl;
class RecordDecl;
class AliasDecl;
class ExtensionDecl;
class PrecedenceGroupDecl;
class ParsedConstraint;

class ASTContext {
public:
   ASTContext(CompilerInstance& CI);
   ~ASTContext() = default;

   void cleanup();

   void* Allocate(size_t size, size_t alignment = 8) const
   {
      return Allocator.Allocate(size, alignment);
   }

   template<typename T> T* Allocate(size_t Num = 1) const
   {
      return static_cast<T*>(Allocate(Num * sizeof(T), alignof(T)));
   }

   void Deallocate(void* Ptr) const {}

   llvm::BumpPtrAllocator& getAllocator() const { return Allocator; }
   IdentifierTable& getIdentifiers() const { return Identifiers; }
   DeclarationNameTable& getDeclNameTable() const { return DeclNames; }
   ConformanceTable& getConformanceTable() const { return Conformances; }

   const TargetInfo& getTargetInfo() const { return TI; }

   using AttrVec = SmallVector<Attr*, 0>;
   using ConstraintVec = SmallVector<StaticExpr*, 0>;
   using ExtConstraintVec = SmallVector<DeclConstraint*, 0>;
   using ExtensionVec = SmallVector<ExtensionDecl*, 0>;
   using CovarianceVec = SmallVector<RecordDecl*, 0>;

   using ProtocolImplMapType = llvm::DenseMap<
      const RecordDecl*, llvm::MapVector<const NamedDecl*, NamedDecl*>>;

   friend sema::FinalTemplateArgumentList;

   CompilerInstance& CI;
   mutable llvm::BumpPtrAllocator Allocator;
   mutable llvm::BumpPtrAllocator TmpAllocator;

   mutable llvm::SetVector<ExtensionDecl*> UnresolvedExtensions;
   mutable llvm::FoldingSet<DeclConstraint> DeclConstraints;
   mutable llvm::FoldingSet<ConstraintSet> ConstraintSets;

   mutable ConstraintSet* EmptyConstraintSet = nullptr;

private:
   mutable ConformanceTable Conformances;
   mutable IdentifierTable Identifiers;
   mutable DeclarationNameTable DeclNames;
   mutable TargetInfo TI;

   mutable llvm::DenseMap<const Decl*, AttrVec*> AttributeMap;
   mutable llvm::DenseMap<const Decl*, ConstraintSet*> ConstraintMap;
   mutable llvm::DenseMap<const Decl*, std::vector<ParsedConstraint>>
       ParsedConstraintMap;
   mutable llvm::DenseMap<const Decl*,
                          llvm::DenseMap<const DeclConstraint*, SourceRange>>
       ConstraintLocs;
   mutable llvm::DenseMap<CanType, ExtensionVec*> ExtensionMap;
   mutable llvm::DenseMap<const AssociatedTypeDecl*, CovarianceVec*>
       CovarianceMap;

   mutable llvm::DenseMap<
       const ProtocolDecl*,
       llvm::DenseMap<const NamedDecl*, std::vector<NamedDecl*>>>
       ProtocolDefaultImplMap;

   mutable ProtocolImplMapType ProtocolImplMap;

   mutable llvm::DenseMap<const IdentifierInfo*, PrecedenceGroupDecl*>
       InfixOperators;
   mutable llvm::DenseSet<const IdentifierInfo*> PrefixOperators;
   mutable llvm::DenseSet<const IdentifierInfo*> PostfixOperators;

#define CDOT_BUILTIN_TYPE(Name)                                                \
   alignas(TypeAlignment) mutable BuiltinType Name##Ty;
#include "cdotc/Basic/BuiltinTypes.def"

   alignas(TypeAlignment) mutable TokenType TokenTy;
   TupleType* EmptyTupleTy;

   mutable llvm::DenseMap<QualType, PointerType*> PointerTypes;
   mutable llvm::DenseMap<QualType, MutablePointerType*> MutablePointerTypes;
   mutable llvm::DenseMap<QualType, ReferenceType*> ReferenceTypes;
   mutable llvm::DenseMap<QualType, MutableReferenceType*> MutableReferenceTypes;
   mutable llvm::DenseMap<QualType, BoxType*> BoxTypes;
   mutable llvm::FoldingSet<ExistentialType> ExistentialTypes;
   mutable llvm::FoldingSet<FunctionType> FunctionTypes;
   mutable llvm::FoldingSet<LambdaType> LambdaTypes;
   mutable llvm::FoldingSet<ArrayType> ArrayTypes;
   mutable llvm::DenseMap<QualType, InferredSizeArrayType*> InferredSizeArrayTypes;
   mutable llvm::FoldingSet<TupleType> TupleTypes;
   mutable llvm::DenseMap<ast::TemplateParamDecl*, TemplateParamType*> TemplateParamTypes;
   mutable llvm::FoldingSet<AssociatedType> AssociatedTypes;
   mutable llvm::FoldingSet<DependentRecordType> DependentRecordTypes;
   mutable llvm::DenseMap<QualType, MetaType*> MetaTypes;
   mutable llvm::DenseMap<ast::AliasDecl*, TypedefType*> TypedefTypes;
   mutable llvm::FoldingSet<DependentTypedefType> DependentTypedefTypes;
   mutable llvm::DenseMap<ast::RecordDecl*, RecordType*> RecordTypes;
   mutable llvm::FoldingSet<DependentNameType> DependentNameTypes;
   mutable llvm::DenseMap<unsigned, TypeVariableType*> TypeVariableTypes;

   /// Uniqued template argument lists.
   mutable llvm::FoldingSet<sema::FinalTemplateArgumentList> TemplateArgs;

   /// Map from source IDs to declarations within that file, used for
   /// incremental compilation.
   mutable llvm::DenseMap<unsigned, SmallVector<Decl*, 0>> DeclsPerFile;

   bool OpNamesInitialized = false;

   void initializeOpNames() const;

   void cleanupDecl(Decl* D);
   void cleanupDeclContext(DeclContext* DC);

public:
   void registerInfixOperator(const IdentifierInfo* II) const
   {
      InfixOperators.try_emplace(II, nullptr);
   }

   void setInfixOperatorPrecedence(const IdentifierInfo* II,
                                   PrecedenceGroupDecl* PG) const
   {
      InfixOperators[II] = PG;
   }

   void registerPrefixOperator(const IdentifierInfo* II) const
   {
      PrefixOperators.insert(II);
   }

   void registerPostfixOperator(const IdentifierInfo* II) const
   {
      PostfixOperators.insert(II);
   }

   PrecedenceGroupDecl* getInfixOperator(const IdentifierInfo* II) const
   {
      auto It = InfixOperators.find(II);
      if (It != InfixOperators.end())
         return It->getSecond();

      return nullptr;
   }

   bool isInfixOperator(const IdentifierInfo* II)
   {
      if (!OpNamesInitialized)
         initializeOpNames();

      return InfixOperators.find(II) != InfixOperators.end();
   }

   bool isPrefixOperator(const IdentifierInfo* II)
   {
      if (!OpNamesInitialized)
         initializeOpNames();

      return PrefixOperators.find(II) != PrefixOperators.end();
   }

   bool isPostfixOperator(const IdentifierInfo* II)
   {
      if (!OpNamesInitialized)
         initializeOpNames();

      return PostfixOperators.find(II) != PostfixOperators.end();
   }

   void addProtocolDefaultImpl(const ProtocolDecl* P, const NamedDecl* Req,
                               NamedDecl* Impl);

   ArrayRef<NamedDecl*> getProtocolDefaultImpls(const ProtocolDecl* P,
                                                const NamedDecl* Req);

   const llvm::DenseMap<const NamedDecl*, std::vector<NamedDecl*>>*
   getProtocolDefaultImpls(const ProtocolDecl* P);

   void addProtocolImpl(const RecordDecl* R, const NamedDecl* Req,
                        NamedDecl* Impl);

   void updateProtocolImpl(const RecordDecl* R, NamedDecl *OldImpl, NamedDecl* NewImpl);

   NamedDecl* getProtocolImpl(const RecordDecl* R, const NamedDecl* Req);

   const ProtocolImplMapType &getAllProtocolImpls() const { return ProtocolImplMap; }

   const llvm::MapVector<const NamedDecl*, NamedDecl*>*
   getProtocolImpls(const RecordDecl* R);

   void registerDecl(unsigned SourceID, Decl* D)
   {
      DeclsPerFile[SourceID].push_back(D);
   }

   const llvm::DenseMap<unsigned int, SmallVector<Decl*, 0>>&
   getDeclsPerFile() const
   {
      return DeclsPerFile;
   }

   BuiltinType* getAutoType() const { return &AutoTy; }
   BuiltinType* getVoidType() const { return &VoidTy; }

   BuiltinType* getInt1Ty() const { return &i1Ty; }

   BuiltinType* getBoolTy() const { return &i1Ty; }

   BuiltinType* getInt8Ty() const { return &i8Ty; }
   BuiltinType* getUInt8Ty() const { return &u8Ty; }
   BuiltinType* getCharTy() const { return &u8Ty; }

   BuiltinType* getInt16Ty() const { return &i16Ty; }
   BuiltinType* getUInt16Ty() const { return &u16Ty; }

   BuiltinType* getInt32Ty() const { return &i32Ty; }
   BuiltinType* getUInt32Ty() const { return &u32Ty; }

   BuiltinType* getInt64Ty() const { return &i64Ty; }
   BuiltinType* getUInt64Ty() const { return &u64Ty; }

   BuiltinType* getInt128Ty() const { return &i128Ty; }
   BuiltinType* getUInt128Ty() const { return &u128Ty; }

   TokenType* getTokenType() const { return &TokenTy; }
   TupleType* getEmptyTupleType() const { return EmptyTupleTy; }

#define CDOT_BUILTIN_TYPE(Name)                                                \
   BuiltinType* get##Name##Ty() const { return &Name##Ty; }
#include "cdotc/Basic/BuiltinTypes.def"

   BuiltinType* getBuiltinType(Type::BuiltinKind kind) const
   {
      switch (kind) {
#define CDOT_BUILTIN_TYPE(Name)                                                \
   case BuiltinType::Name:                                                     \
      return &Name##Ty;
#include "cdotc/Basic/BuiltinTypes.def"
      }
   }

   BuiltinType* getIntTy() const
   {
      if
         constexpr(sizeof(void*) == 8) { return &i64Ty; }
      else {
         return &i32Ty;
      }
   }

   BuiltinType* getUIntTy() const
   {
      if
         constexpr(sizeof(void*) == 8) { return &u64Ty; }
      else {
         return &u32Ty;
      }
   }

   BuiltinType* getIntegerTy(unsigned bits, bool isUnsigned) const
   {
      if (!isUnsigned) {
         switch (bits) {
         case 1:
            return &i1Ty;
         case 8:
            return &i8Ty;
         case 16:
            return &i16Ty;
         case 32:
            return &i32Ty;
         case 64:
            return &i64Ty;
         case 128:
            return &i128Ty;
         default:
            llvm_unreachable("bad bitwidth");
         }
      }
      else {
         switch (bits) {
         case 1:
            return &i1Ty;
         case 8:
            return &u8Ty;
         case 16:
            return &u16Ty;
         case 32:
            return &u32Ty;
         case 64:
            return &u64Ty;
         case 128:
            return &u128Ty;
         default:
            llvm_unreachable("bad bitwidth");
         }
      }
   }

   PointerType* getUInt8PtrTy() const { return getPointerType(u8Ty); }
   PointerType* getInt8PtrTy() const { return getPointerType(i8Ty); }

   BuiltinType* getFloatTy() const { return &f32Ty; }
   BuiltinType* getFloat32Ty() const { return &f32Ty; }

   BuiltinType* getDoubleTy() const { return &f64Ty; }
   BuiltinType* getFloat64Ty() const { return &f64Ty; }

   PointerType* getPointerType(QualType pointeeType) const;
   MutablePointerType* getMutablePointerType(QualType pointeeType) const;

   ReferenceType* getReferenceType(QualType referencedType) const;
   MutableReferenceType* getMutableReferenceType(QualType referencedType) const;

   BoxType* getBoxType(QualType BoxedTy) const;
   QualType getExistentialType(ArrayRef<QualType> Existentials) const;

   FunctionType*
   getFunctionType(QualType returnType, llvm::ArrayRef<QualType> argTypes,
                   llvm::ArrayRef<FunctionType::ParamInfo> paramInfo,
                   unsigned flags = 0, bool lambda = false) const;

   FunctionType* getFunctionType(QualType returnType,
                                 llvm::ArrayRef<QualType> argTypes,
                                 unsigned flags = 0, bool lambda = false) const;

   LambdaType* getLambdaType(FunctionType* FnTy);
   LambdaType* getLambdaType(QualType returnType,
                             llvm::ArrayRef<QualType> argTypes,
                             llvm::ArrayRef<FunctionType::ParamInfo> paramInfo,
                             unsigned flags = 0) const;

   LambdaType* getLambdaType(QualType returnType,
                             llvm::ArrayRef<QualType> argTypes,
                             unsigned flags = 0) const;

   ArrayType* getArrayType(QualType elementType, unsigned numElements) const;
   DependentSizeArrayType*
   getValueDependentSizedArrayType(QualType elementType,
                                   StaticExpr* DependentExpr) const;
   InferredSizeArrayType* getInferredSizeArrayType(QualType elTy) const;

   TupleType* getTupleType(llvm::ArrayRef<QualType> containedTypes) const;

   RecordType* getRecordType(RecordDecl* R) const;

   DependentRecordType*
   getDependentRecordType(RecordDecl* R, sema::FinalTemplateArgumentList* args,
                          QualType Parent = QualType()) const;

   DependentNameType*
   getDependentNameType(NestedNameSpecifierWithLoc* Name) const;

   TemplateParamType* getTemplateParamType(TemplateParamDecl* Param) const;
   AssociatedType* getAssociatedType(AssociatedTypeDecl* AT,
                                     QualType OuterAT = QualType()) const;

   MetaType* getMetaType(QualType forType) const;
   TypedefType* getTypedefType(AliasDecl* TD) const;

   DependentTypedefType*
   getDependentTypedefType(AliasDecl* td, sema::FinalTemplateArgumentList* args,
                           QualType Parent = QualType()) const;

   TypeVariableType* getTypeVariableType(unsigned ID) const;

private:
   mutable llvm::DenseMap<NamedDecl*, SmallVector<NamedDecl*, 0>> InstMap;

public:
   mutable llvm::FoldingSet<CallableDecl> FunctionTemplateInstatiations;
   mutable llvm::FoldingSet<RecordDecl> RecordTemplateInstatiations;
   mutable llvm::FoldingSet<AliasDecl> AliasTemplateInstatiations;

   using TemplateArgList = sema::FinalTemplateArgumentList;

   CallableDecl* getFunctionTemplateInstantiation(CallableDecl* Template,
                                                  TemplateArgList& argList,
                                                  void*& insertPos);

   RecordDecl* getRecordTemplateInstantiation(RecordDecl* Template,
                                              TemplateArgList& argList,
                                              void*& insertPos);

   AliasDecl* getAliasTemplateInstantiation(AliasDecl* Template,
                                            TemplateArgList& argList,
                                            void*& insertPos);

   NamedDecl* getTemplateInstantiation(NamedDecl* Template,
                                       TemplateArgList& argList,
                                       void*& insertPos);

   ArrayRef<NamedDecl*> getInstantiationsOf(NamedDecl* Template);
   void registerInstantiation(NamedDecl* Template, NamedDecl* Inst);

   void insertFunctionTemplateInstantiation(CallableDecl* Inst,
                                            void* insertPos);

   void insertRecordTemplateInstantiation(RecordDecl* Inst, void* insertPos);

   void insertAliasTemplateInstantiation(AliasDecl* Inst, void* insertPos);

   void insertTemplateInstantiation(NamedDecl* Inst, void* insertPos);

   ArrayRef<Attr*> getAttributes(const Decl* D) const;
   void setAttributes(const Decl* D, ArrayRef<Attr*> attrs) const;
   void addAttribute(const Decl* D, Attr* attr) const;
   void addAttributes(const Decl* D, ArrayRef<Attr*> attrs) const;

   ArrayRef<ParsedConstraint> getParsedConstraints(const Decl* D) const;
   void setParsedConstraints(const Decl* D,
                             std::vector<ParsedConstraint>&& vec) const;

   ConstraintSet* getExtConstraints(const Decl* D) const;
   void setConstraints(const Decl* D, ConstraintSet* CS) const;

   ConstraintSet* getNearestConstraintSet(const DeclContext* DC) const;

   void updateConstraintLocs(const Decl* D,
                             ArrayRef<ParsedConstraint> parsedConstraints,
                             ArrayRef<DeclConstraint*> declConstraints);

   SourceRange getConstraintLoc(const Decl* D, const DeclConstraint* DC) const;

   ArrayRef<RecordDecl*> getCovariance(const AssociatedTypeDecl* AT) const;
   void addCovariance(const AssociatedTypeDecl* AT, RecordDecl* Cov) const;

   ArrayRef<ExtensionDecl*> getExtensions(CanType T) const;
   void addExtension(CanType T, ExtensionDecl* E) const;
};

} // namespace ast
} // namespace cdot

inline void* operator new(size_t size, cdot::ast::ASTContext const& Ctx,
                          size_t alignment = 8)
{
   return Ctx.Allocate(size, alignment);
}

inline void operator delete(void* ptr, cdot::ast::ASTContext const& Ctx, size_t)
{
   return Ctx.Deallocate(ptr);
}

inline void* operator new[](size_t size, cdot::ast::ASTContext const& Ctx,
                            size_t alignment = 8)
{
   return Ctx.Allocate(size, alignment);
}

inline void operator delete[](void* ptr, cdot::ast::ASTContext const& Ctx,
                              size_t)
{
   return Ctx.Deallocate(ptr);
}

#endif // CDOT_ASTCONTEXT_H
