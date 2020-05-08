#ifndef CDOT_DECLBASE_H
#define CDOT_DECLBASE_H

#include "cdotc/AST/Attr.h"
#include "cdotc/AST/Type.h"
#include "cdotc/Basic/DeclarationName.h"

#include <llvm/ADT/DenseMap.h>
#include <llvm/ADT/PointerUnion.h>

namespace cdot {

class Module;

namespace serial {
class ModuleFile;
class LazyFunctionInfo;
} // namespace serial

enum class AccessSpecifier : unsigned char {
   /// \brief No explicit access specifier was given.
   Default = 0,

   /// \brief This declaration is visible everywhere.
   Public,

   /// \brief This declaration is only visible within the immediate surrounding
   /// lexical context.
   Private,

   /// \brief This declaration is visible within the current class as well as
   /// its subclasses
   Protected,

   /// \brief This declaration is only visible within the current module.
   Internal,

   /// \brief This declaration is only visible within the current source file.
   FilePrivate,
};

namespace ast {

class DeclContext;
class ModuleDecl;
class Statement;
class NamedDecl;
class ConstraintSet;

class LLVM_ALIGNAS(sizeof(void*)) Decl {
public:
   struct EmptyShell {
   };

   enum DeclKind : unsigned char {
      NotDecl = 0,

#define CDOT_DECL(Name) Name##ID,
#define CDOT_ABSTRACT(Name) Name##ID,
#include "cdotc/AST/Decl.def"
   };

   enum DeclFlags : uint64_t {
      DF_None = 0x0,

      /// This declaration is dependent on an unbounded template parameter.
      DF_TypeDependent = 0x1,

      /// This declaration contains a generic parameter somewhere.
      DF_ContainsGenericParam = 0x2,

      /// This declaration contains an associated type of a protocol somewhere.
      DF_ContainsAssociatedType = 0x4,

      /// This declaration contains an unexpanded parameter pack.
      DF_ContainsUnexpandedPack = 0x8,

      /// This declaration is dependent on a value template parameter.
      DF_ValueDependent = 0x10,

      /// This declaration contained an error.
      DF_IsInvalid = DF_ValueDependent << 1u,

      /// This declaration has been semantically checked.
      DF_SemanticallyChecked = DF_IsInvalid << 1u,

      /// This declaration is marked 'static'.
      DF_Static = DF_SemanticallyChecked << 1u,

      /// This declaration is marked 'default'.
      DF_Default = DF_Static << 1u,

      /// This declaration is defined, i.e. has a body.
      DF_HasDefinition = DF_Default << 1u,

      /// This declaration is externally imported.
      DF_External = DF_HasDefinition << 1u,

      /// This declaration has been declared.
      DF_WasDeclared = DF_External << 1u,

      /// This declaration was synthesized by the compiler.
      DF_Synthesized = DF_WasDeclared << 1u,

      /// This declarations attributes were semantically checked.
      DF_CheckedAttrs = DF_Synthesized << 1u,

      /// This declaration is marked @extern(C)
      DF_ExternC = DF_CheckedAttrs << 1u,

      /// This declaration is marked @extern(CXX)
      DF_ExternCXX = DF_ExternC << 1u,

      /// This declaration is a template instantiation.
      DF_Instantiation = DF_ExternCXX << 1u,

      /// This declaration is a compiler builtin.
      DF_Builtin = DF_Instantiation << 1u,

      /// This declaration is an indirect enum case.
      DF_IndirectCase = DF_Builtin << 1u,

      /// This declaration is trivially copyable.
      DF_TriviallyCopyable = DF_IndirectCase << 1u,

      /// This declaration is imported from a CDot module.
      DF_ImportedFromModule = DF_TriviallyCopyable << 1u,

      /// This declaration is an instantiation that was imported from a module.
      DF_ImportedInstantiation = DF_ImportedFromModule << 1u,

      /// This declaration is currently being evaluated.
      DF_BeingEvaluated = DF_ImportedInstantiation << 1u,

      /// This declaration was loaded from a cache file.
      DF_LoadedFromCache = DF_BeingEvaluated << 1u,

      /// This declaration is marked 'abstract'.
      DF_Abstract = DF_LoadedFromCache << 1u,

      /// This declaration is ignored, e.g. because of a version attribute.
      DF_Ignored = DF_Abstract << 1u,

      /// This declaration is a protocol default implementation.
      DF_ProtoDefaultImpl = DF_Ignored << 1u,

      /// This declaration is a protocol requirement.
      DF_ProtoRequirement = DF_ProtoDefaultImpl << 1u,

      /// This declaration has been finalized, i.e. its conformance has been
      /// checked and its size calculated.
      DF_Finalized = DF_ProtoRequirement << 1u,

      /// This declaration is an instantiation of a protocol default
      /// implementation.
      DF_InstantiatedFromProtoImpl = DF_Finalized << 1u,

      /// This declaration was imported from Clang.
      DF_ImportedFromClang = DF_InstantiatedFromProtoImpl << 1u,

      /// This declaration is a template that needs to be fully instantiated
      /// instead of specialized.
      DF_UnboundedTemplate = DF_ImportedFromClang << 1u,

      /// This function should be specialized.
      DF_Specialize = DF_UnboundedTemplate << 1u,

      /// This decl is an implementation of a protocol requirement.
      DF_ImplOfProtocolRequirement = DF_Specialize << 1u,

      DF_Last = DF_ImplOfProtocolRequirement,
      StatusFlags = DF_TypeDependent | DF_ValueDependent | DF_IsInvalid
                    | DF_ContainsGenericParam
                    | DF_ContainsUnexpandedPack
                    | DF_ContainsAssociatedType,
   };

   static_assert(DF_Last < (1llu << 63llu), "too many decl flags!");

#ifndef NDEBUG
   static void verifyID(DeclKind K);
#endif

   void dumpFlags() const;
   void printFlags(llvm::raw_ostream& OS) const;

   bool isStatic() const { return declFlagSet(DF_Static); }

   bool isDefault() const { return declFlagSet(DF_Default); }
   void setDefault(bool D) { setDeclFlag(DF_Default, D); }

   bool hasDefinition() const { return declFlagSet(DF_HasDefinition); }
   void setHasDefinition(bool def) { setDeclFlag(DF_HasDefinition, def); }

   bool isExternal() const
   {
      return isImportedFromModule() || isLoadedFromCache();
   }

   bool isDependent() const { return isTypeDependent() || isValueDependent(); }

   bool isTypeDependent() const { return declFlagSet(DF_TypeDependent); }
   bool isValueDependent() const { return declFlagSet(DF_ValueDependent); }

   void setIsTypeDependent(bool typeDependant)
   {
      setDeclFlag(DF_TypeDependent, typeDependant);
   }

   void setIsValueDependent(bool valueDependant)
   {
      setDeclFlag(DF_ValueDependent, valueDependant);
   }

   bool containsGenericParam() const
   {
      return declFlagSet(DF_ContainsGenericParam);
   }

   void setContainsGenericParam(bool b)
   {
      setDeclFlag(DF_ContainsGenericParam, b);
   }

   bool containsAssociatedType() const
   {
      return declFlagSet(DF_ContainsAssociatedType);
   }

   void setContainsAssociatedType(bool b)
   {
      setDeclFlag(DF_ContainsAssociatedType, b);
   }

   bool isInvalid() const { return declFlagSet(DF_IsInvalid); }
   void setIsInvalid(bool error);

   bool isSemanticallyChecked() const
   {
      return declFlagSet(DF_SemanticallyChecked);
   }

   void setSemanticallyChecked(bool chk = true)
   {
      setDeclFlag(DF_SemanticallyChecked, chk);
   }

   bool wasDeclared() const { return declFlagSet(DF_WasDeclared); }
   void setDeclared(bool dec = true) { setDeclFlag(DF_WasDeclared, dec); }

   bool checkedAttrs() const { return declFlagSet(DF_CheckedAttrs); }
   void setCheckedAttrs(bool C) { setDeclFlag(DF_CheckedAttrs, C); }

   bool isSynthesized() const { return declFlagSet(DF_Synthesized); }
   void setSynthesized(bool synth = true)
   {
      setDeclFlag(DF_Synthesized, synth);
   }

   bool isExternC() const { return declFlagSet(DF_ExternC); }
   void setExternC(bool ext) { setDeclFlag(DF_ExternC, ext); }

   bool isExternCXX() const { return declFlagSet(DF_ExternCXX); }
   void setExternCXX(bool ext) { setDeclFlag(DF_ExternCXX, ext); }

   bool isBuiltin() const { return declFlagSet(DF_Builtin); }
   void setBuiltin(bool BI) { setDeclFlag(DF_Builtin, BI); }

   bool isImportedFromModule() const
   {
      return declFlagSet(DF_ImportedFromModule);
   }
   void setImportedFromModule(bool b) { setDeclFlag(DF_ImportedFromModule, b); }

   bool isImportedFromClang() const
   {
      return declFlagSet(DF_ImportedFromClang);
   }
   void setImportedFromClang(bool b) { setDeclFlag(DF_ImportedFromClang, b); }

   bool isBeingEvaluated() const { return declFlagSet(DF_BeingEvaluated); }
   void setBeingEvaluated(bool b) { setDeclFlag(DF_BeingEvaluated, b); }

   bool isLoadedFromCache() const { return declFlagSet(DF_LoadedFromCache); }
   void setLoadedFromCache(bool b) { setDeclFlag(DF_LoadedFromCache, b); }

   bool isIgnored() const { return declFlagSet(DF_Ignored); }
   void setIgnored(bool b) { setDeclFlag(DF_Ignored, b); }

   bool isFinalized() const { return declFlagSet(DF_Finalized); }
   void setFinalized(bool b) { setDeclFlag(DF_Finalized, b); }

   bool isUnboundedTemplate() const
   {
      return declFlagSet(DF_UnboundedTemplate);
   }
   void setUnboundedTemplate(bool V) { setDeclFlag(DF_UnboundedTemplate, V); }

   bool shouldBeSpecialized() const { return declFlagSet(DF_Specialize); }
   void setShouldBeSpecialized(bool V) { setDeclFlag(DF_Specialize, V); }

   bool isImplOfProtocolRequirement() const { return declFlagSet(DF_ImplOfProtocolRequirement); }
   void setImplOfProtocolRequirement(bool u) { setDeclFlag(DF_ImplOfProtocolRequirement, u); }

   bool isProtocolDefaultImpl() const
   {
      return declFlagSet(DF_ProtoDefaultImpl);
   }
   void setIsProtocolDefaultImpl(bool impl)
   {
      setDeclFlag(DF_ProtoDefaultImpl, impl);
   }

   bool isProtocolRequirement() const
   {
      return declFlagSet(DF_ProtoRequirement);
   }
   void setIsProtocolRequirement(bool impl)
   {
      setDeclFlag(DF_ProtoRequirement, impl);
   }

   bool instantiatedFromProtocolDefaultImpl() const
   {
      return declFlagSet(DF_InstantiatedFromProtoImpl);
   }

   void setInstantiatedFromProtocolDefaultImpl(bool b)
   {
      setDeclFlag(DF_InstantiatedFromProtoImpl, b);
   }

   bool isAbstract() const { return declFlagSet(DF_Abstract); }

   bool isImportedInstantiation() const
   {
      return declFlagSet(DF_ImportedInstantiation);
   }

   void setImportedInstantiation(bool b)
   {
      setDeclFlag(DF_ImportedInstantiation, b);
   }

   bool isInstantiation() const { return declFlagSet(DF_Instantiation); }
   bool isNestedInstantiation() const;
   bool hasTrailingObjects() const;

   bool alreadyCheckedOrIsInvalid() const
   {
      static uint32_t mask = DF_IsInvalid | DF_SemanticallyChecked;
      return (flags & mask) != 0;
   }

   void copyStatusFlags(Decl* D);
   void copyStatusFlags(Statement* D);

   bool declFlagSet(DeclFlags f) const { return (flags & f) != 0; }
   void setDeclFlag(DeclFlags f, bool val)
   {
      if (val)
         flags |= f;
      else
         flags &= ~f;
   }

   bool isInExtension() const;

   template<class T> bool hasAttribute() const
   {
      for (auto& Attr : getAttributes())
         if (support::isa<T>(Attr))
            return true;

      return false;
   }

   template<class T> T* getAttribute()
   {
      for (auto& Attr : getAttributes())
         if (auto A = support::dyn_cast<T>(Attr))
            return A;

      return nullptr;
   }

   llvm::ArrayRef<Attr*> getAttributes() const;
   void setAttributes(llvm::ArrayRef<Attr*> attrs) const;
   void addAttributes(llvm::ArrayRef<Attr*> attrs) const;
   void addAttribute(Attr* A) const;

   ASTContext& getASTCtx() const;

   SourceLocation getSourceLoc() const { return getSourceRange().getStart(); }
   SourceRange getSourceRange() const;

   DeclKind getKind() const { return kind; }
   RecordDecl* getRecord() const;

   DeclContext* getNonTransparentDeclContext() const;
   DeclContext* getDeclContext() const;
   DeclContext* getLexicalContext() const;

   void setLexicalContext(DeclContext* ctx);
   void setLogicalContext(DeclContext* Ctx);
   void setLexicalContextUnchecked(DeclContext* ctx);

   NamedDecl* getInstantiationScope() const;
   NamedDecl* getProtocolDefaultImplScope() const;

   Decl* getNextDeclInContext() const { return nextDeclInContext; }
   void setNextDeclInContext(Decl* next) { nextDeclInContext = next; }

   bool inAnonymousNamespace() const;
   bool inStdNamespace() const;
   bool isGlobalDecl() const;

   uint64_t getFlags() const { return flags; }
   void setFlags(uint64_t flags) { this->flags = flags; }

   ModuleDecl* getModule() const;

   void dumpName() const;
   void dumpSourceLine();

   std::string getNameAsString() const;
   std::string getSourceLocAsString() const;
   StringRef getFileName() const;

   FullSourceLoc getFullSourceLoc() const;

   template<class T> bool isa() const { return support::isa<T>(this); }

   template<class T> T* dyn_cast() { return support::dyn_cast<T>(this); }

   template<class T> const T* dyn_cast() const
   {
      return support::dyn_cast<T>(this);
   }

   template<class T> T* cast() { return support::cast<T>(this); }

   template<class T> const T* cast() const
   {
      return support::dyn_cast<T>(this);
   }

   static bool classofKind(DeclKind kind) { return kind != NotDecl; }
   static bool classof(const Decl* T) { return classofKind(T->getKind()); }

   static DeclContext* castToDeclContext(Decl const* D);
   static Decl* castFromDeclContext(DeclContext const* Ctx);

   friend class DeclContext;

protected:
   explicit Decl(DeclKind kind, unsigned flags = DF_None);

#ifndef NDEBUG
   virtual
#endif
       ~Decl();

   DeclKind kind;
   uint64_t flags;

   Decl* nextDeclInContext = nullptr;

   struct LLVM_ALIGNAS(8) MultipleDeclContext {
      DeclContext* LogicalDC;
      DeclContext* LexicalDC;
   };

   using DeclContextUnion
       = llvm::PointerUnion<DeclContext*, MultipleDeclContext*>;

   DeclContextUnion ContextUnion;
};

class NamedDecl : public Decl {
public:
   AccessSpecifier getAccess() const { return access; }
   void setAccess(AccessSpecifier AS) { access = AS; }

   SourceLocation getAccessLoc() const { return AccessLoc; }
   void setAccessLoc(SourceLocation Loc) { AccessLoc = Loc; }

   SourceRange getAccessRange() const;

   llvm::StringRef getName() const
   {
      assert(Name.isSimpleIdentifier() && "not a simple identifier");
      return Name.getIdentifierInfo()->getIdentifier();
   }

   void setName(DeclarationName DN);

   DeclarationName getDeclName() const { return Name; }
   IdentifierInfo* getIdentifierInfo() const
   {
      return Name.getIdentifierInfo();
   }

   bool isOverloadable() const;
   bool isRedeclarable() const;

   ConstraintSet* getConstraints() const;

   bool isTemplate() const;
   bool isNestedTemplate() const;
   bool inDependentContext() const;
   bool isTemplateOrInTemplate() const;
   bool isInUnboundedTemplate() const;
   NamedDecl *getOuterTemplate() const;

   bool isShallowInstantiation() const;

   llvm::ArrayRef<TemplateParamDecl*> getTemplateParams() const;

   void setIsInstantiation(bool I) { setDeclFlag(DF_Instantiation, I); }

   sema::FinalTemplateArgumentList& getTemplateArgs() const;
   NamedDecl* getSpecializedTemplate() const;
   SourceLocation getInstantiatedFrom() const;

   std::string getFullName(bool includeSignatures) const
   {
      return getJoinedName('.', false, includeSignatures);
   }

   std::string getFullName() const
   {
      return getJoinedName('.', false, true);
   }

   std::string getJoinedName(char join, bool includeFile = false,
                             bool includeSignatures = false,
                             bool includeAllModules = false) const;

   void dumpName() const;

   bool isExported() const;

   unsigned int getFlags() const { return flags; }
   void setFlags(unsigned int flags) { NamedDecl::flags = flags; }

   static bool classof(const Decl* T) { return classofKind(T->getKind()); }
   static bool classofKind(DeclKind kind)
   {
      return kind > _firstNamedDeclID && kind < _lastNamedDeclID;
   }

protected:
   NamedDecl(DeclKind typeID, AccessSpecifier access, DeclarationName DN);

   AccessSpecifier access;
   SourceLocation AccessLoc;
   DeclarationName Name;
};

template<class ToTy,
         bool IsKnownSubtype = ::std::is_base_of<DeclContext, ToTy>::value>
struct cast_convert_decl_context {
   static const ToTy* doit(const DeclContext* Val)
   {
      return static_cast<const ToTy*>(Decl::castFromDeclContext(Val));
   }

   static ToTy* doit(DeclContext* Val)
   {
      return static_cast<ToTy*>(Decl::castFromDeclContext(Val));
   }
};

// Specialization selected when ToTy is a known subclass of DeclContext.
template<class ToTy> struct cast_convert_decl_context<ToTy, true> {
   static const ToTy* doit(const DeclContext* Val)
   {
      return static_cast<const ToTy*>(Val);
   }

   static ToTy* doit(DeclContext* Val) { return static_cast<ToTy*>(Val); }
};

class DeclContextLookupResult {
public:
   using ArrayTy = llvm::ArrayRef<NamedDecl*>;

   DeclContextLookupResult() = default;

   DeclContextLookupResult(NamedDecl* Single)
       : Single(Single), Result(Single ? Single : ArrayTy())
   {
   }

   DeclContextLookupResult(ArrayTy Result) : Result(Result) {}

private:
   NamedDecl* Single = nullptr;
   ArrayTy Result;

public:
   class iterator;

   using IteratorBase
       = llvm::iterator_adaptor_base<iterator, ArrayTy::iterator,
                                     std::random_access_iterator_tag,
                                     NamedDecl*>;

   class iterator : public IteratorBase {
      value_type SingleElement;

   public:
      iterator() = default;
      iterator(const iterator&) = default;
      iterator& operator=(const iterator&) = default;

      explicit iterator(pointer Pos, value_type Single = nullptr)
          : IteratorBase(Pos), SingleElement(Single)
      {
      }

      bool operator==(const iterator &RHS) const
      {
         return SingleElement
                         ? SingleElement == RHS.SingleElement
                         : IteratorBase::I == RHS.I;
      }

      bool operator!=(const iterator &RHS) const
      {
         return !(*this == RHS);
      }

      reference operator*() const
      {
         return SingleElement ? SingleElement : IteratorBase::operator*();
      }

      iterator operator++(int)
      {
         if (SingleElement) {
            SingleElement++;
         }
         else {
            IteratorBase::operator++(0);
         }

         return *this;
      }

      iterator &operator++()
      {
         if (SingleElement) {
            ++SingleElement;
         }
         else {
            IteratorBase::operator++();
         }

         return *this;
      }

      IteratorBase::difference_type operator-(const iterator &RHS) const
      {
         if (SingleElement) {
            return SingleElement - RHS.SingleElement;
         }
         else {
            return IteratorBase::operator-(RHS);
         }
      }
   };

   using const_iterator = iterator;
   using pointer = iterator::pointer;
   using reference = iterator::reference;

   iterator begin() const
   {
      if (Single)
         return iterator(nullptr, Single);

      return iterator(Result.begin(), nullptr);
   }

   iterator end() const
   {
      if (Single)
         return iterator(nullptr, Single + 1);

      return iterator(Result.end(), nullptr);
   }

   bool empty() const { return Result.empty(); }
   pointer data() const { return Single ? &Single : Result.data(); }
   size_t size() const { return Single ? 1 : Result.size(); }
   reference front() const { return Single ? Single : Result.front(); }
   reference back() const { return Single ? Single : Result.back(); }
   reference operator[](size_t N) const { return Single ? Single : Result[N]; }

   operator bool() const { return !empty(); }
};

class DeclList {
public:
   using VecTy = llvm::SmallVector<NamedDecl*, 4>;
   using DataTy = llvm::PointerUnion<NamedDecl*, VecTy*>;

   DeclList(NamedDecl* Decl) : Data(Decl) {}

   DeclList(DeclList&& DL) noexcept : Data(DL.Data)
   {
      DL.Data = (NamedDecl*)nullptr;
   }

   DeclList& operator=(DeclList&& DL) noexcept
   {
      if (auto V = getAsVec())
         delete V;

      Data = DL.Data;
      DL.Data = (NamedDecl*)nullptr;

      return *this;
   }

   ~DeclList()
   {
      if (auto V = getAsVec())
         delete V;
   }

   void appendDecl(NamedDecl* Decl);
   void removeDecl(NamedDecl* Decl);

   bool isNull() const { return Data.isNull(); }

   NamedDecl* getAsDecl() const { return Data.dyn_cast<NamedDecl*>(); }
   VecTy* getAsVec() const { return Data.dyn_cast<VecTy*>(); }

   DeclContextLookupResult getAsLookupResult() const
   {
      if (isNull())
         return DeclContextLookupResult();

      if (auto Single = getAsDecl())
         return DeclContextLookupResult(Single);

      return DeclContextLookupResult(*getAsVec());
   }

private:
   DataTy Data;
};

class DeclContext {
public:
   enum AddDeclResultKind {
      ADR_Success,
      ADR_Duplicate,
      ADR_DuplicateDifferentKind,
   };

   using DeclsMap = llvm::SmallDenseMap<DeclarationName, DeclList, 4>;

   bool isTransparent() const;
   bool isAnonymousNamespace() const;

   void addDecl(Decl* decl);

   [[nodiscard]] AddDeclResultKind addDecl(NamedDecl* decl);

   [[nodiscard]] AddDeclResultKind addDecl(DeclarationName Name,
                                           NamedDecl* decl);

   DeclContextLookupResult lookup(DeclarationName name);
   NamedDecl* lookupSingle(DeclarationName name);

   template<class T> T* lookupSingle(DeclarationName name)
   {
      return support::dyn_cast_or_null<T>(lookupSingle(name));
   }

   const DeclsMap& getAllNamedDecls() const { return primaryCtx->namedDecls; }

   const DeclsMap& getOwnNamedDecls() const { return namedDecls; }

   /// Remove a declaration from the visible declarations in this context. This
   /// does *not* remove the declaration from the lexical declarations in the
   /// context.
   void removeVisibleDecl(NamedDecl* D);
   void removeVisibleDecl(NamedDecl* D, DeclarationName VisibleName);

   bool hasExternalStorage() const;

   bool hasAnyDeclNamed(DeclarationName name) const
   {
      return namedDecls.find(name) != namedDecls.end();
   }

   template<class NodeType, class Fn> void forEach(Fn const& fn)
   {
      for (auto& decl : getDecls()) {
         if (auto N = support::dyn_cast<NodeType>(decl)) {
            fn(N);
         }
      }
   }

   template<class NodeType, class Fn> void forEachRecursive(Fn const& fn)
   {
      for (auto& decl : getDecls()) {
         if (auto N = support::dyn_cast<NodeType>(decl)) {
            fn(N);
         }
         if (auto Ctx = support::dyn_cast<DeclContext>(decl)) {
            assert(Ctx != this);
            Ctx->forEachRecursive<NodeType>(fn);
         }
      }
   }

   NamespaceDecl* getClosestNamespace() const;
   ModuleDecl* getDeclModule() const;

   bool isGlobalDeclContext() const;

   template<class NodeType> size_t count() const
   {
      size_t cnt = 0;
      for (auto& decl : getDecls()) {
         cnt += support::isa<NodeType>(decl);
      }

      return cnt;
   }

   template<class NodeType> size_t countRecursive() const
   {
      size_t cnt = 0;
      for (auto& decl : getDecls()) {
         cnt += support::isa<NodeType>(decl);
         if (auto Ctx = support::dyn_cast<DeclContext>(decl))
            cnt += Ctx->countRecursive<NodeType>();
      }

      return cnt;
   }

   DeclContext* getParentCtx() const { return parentCtx; }
   void setParentCtx(DeclContext* parent);
   void setParentCtxUnchecked(DeclContext* parent);

   DeclContext* lookThroughExtension() const;
   bool isLocalContext() const;
   bool inLocalContext() const;

   DeclContext* getPrimaryCtx() const { return primaryCtx; }
   void setPrimaryCtx(DeclContext* V);
   bool isPrimaryCtx() const { return this == primaryCtx; }

   serial::ModuleFile* getModFile() const;
   void setModFile(serial::ModuleFile* V);

   ArrayRef<Module*> getImportedModules() const;
   void addImportedModule(Module* Mod);

   struct ExternalStorage;

   ExternalStorage* getExtStorage() const { return ExtStorage; }
   void setExtStorage(ExternalStorage* V) { ExtStorage = V; }

   void clear()
   {
      firstDecl = nullptr;
      lastAddedDecl = nullptr;
      namedDecls.clear();
   }

   [[nodiscard]] AddDeclResultKind makeDeclAvailable(NamedDecl* decl);

   [[nodiscard]] AddDeclResultKind makeDeclAvailable(DeclarationName Name,
                                                     NamedDecl* decl);

   void replaceDecl(Decl* Orig, Decl* Rep);

   ASTContext& getDeclASTContext() const;

   Decl::DeclKind getDeclKind() const { return declKind; }

   void dumpName() const;
   void dumpSourceLine();
   std::string getNameAsString() const;

   static bool classofKind(Decl::DeclKind kind)
   {
      switch (kind) {
#define CDOT_DECL_CONTEXT(Name) case Decl::Name##ID:
#include "cdotc/AST/Decl.def"
         return true;
      default:
         return false;
      }
   }

   static bool classof(Decl const* T) { return classofKind(T->getKind()); }
   static bool classof(DeclContext const* T) { return true; }

   // For cleaning up namedDecls.
   friend class ASTContext;

protected:
   explicit DeclContext(Decl::DeclKind typeID)
       : declKind(typeID), primaryCtx(this)
   {
   }

   Decl::DeclKind declKind;

   DeclsMap namedDecls;
   Decl* firstDecl = nullptr;
   Decl* lastAddedDecl = nullptr;

   DeclContext* primaryCtx = nullptr;
   DeclContext* parentCtx = nullptr;

public:
   struct ExternalStorage {
      /// The lookup table for external declarations.
      serial::ModuleFile* ModFile = nullptr;

      /// Modules that are fully imported in this context.
      SmallVector<Module*, 0> ImportedModules;
   };

private:
   /// Storage for possible external declarations.
   ExternalStorage* ExtStorage = nullptr;

public:
   class decl_iterator {
   public:
      using value_type = Decl*;
      using reference = const value_type&;
      using pointer = const value_type*;
      using iterator_category = std::forward_iterator_tag;
      using difference_type = std::ptrdiff_t;

      decl_iterator() = default;
      explicit decl_iterator(value_type Ptr) : Current(Ptr) {}

      reference operator*() const { return Current; }
      value_type operator->() const { return Current; }

      decl_iterator& operator++()
      {
         Current = Current->getNextDeclInContext();
         return *this;
      }

      decl_iterator operator++(int)
      {
         auto tmp = decl_iterator(Current);
         ++(*this);

         return tmp;
      }

      bool operator==(decl_iterator const& it) const
      {
         return Current == it.Current;
      }

      bool operator!=(decl_iterator const& it) const { return !operator==(it); }

   private:
      value_type Current = nullptr;
   };

   decl_iterator decl_begin() const { return decl_iterator(firstDecl); }
   decl_iterator decl_end() const { return decl_iterator(); }

   bool decl_empty() const { return decl_begin() == decl_end(); }

   using decl_range = llvm::iterator_range<decl_iterator>;

   decl_range getDecls() const { return decl_range(decl_begin(), decl_end()); }

   template<class SpecificDecl> class specific_decl_iterator {
   public:
      using value_type = SpecificDecl*;
      using reference = const value_type&;
      using pointer = const value_type*;
      using iterator_category = std::forward_iterator_tag;
      using difference_type = std::ptrdiff_t;

      specific_decl_iterator() = default;
      explicit specific_decl_iterator(Decl* Ptr) : Current(Ptr)
      {
         skipToNext();
      }

      value_type operator*() const
      {
         return support::cast<SpecificDecl>(Current);
      }

      value_type operator->() const { return **this; }

      specific_decl_iterator& operator++()
      {
         Current = Current->getNextDeclInContext();
         skipToNext();

         return *this;
      }

      specific_decl_iterator operator++(int)
      {
         auto tmp = specific_decl_iterator(Current);
         ++(*this);

         return tmp;
      }

      bool operator==(specific_decl_iterator const& it) const
      {
         return Current == it.Current;
      }

      bool operator!=(specific_decl_iterator const& it) const
      {
         return !operator==(it);
      }

   private:
      Decl* Current = nullptr;

      void skipToNext()
      {
         while (Current && !support::isa<SpecificDecl>(Current))
            Current = Current->getNextDeclInContext();
      }
   };

   template<class SpecificDecl>
   specific_decl_iterator<SpecificDecl> decl_begin() const
   {
      return specific_decl_iterator<SpecificDecl>(firstDecl);
   }

   template<class SpecificDecl>
   specific_decl_iterator<SpecificDecl> decl_end() const
   {
      return specific_decl_iterator<SpecificDecl>();
   }

   template<class SpecificDecl> bool decl_empty() const
   {
      return decl_begin<SpecificDecl>() == decl_end<SpecificDecl>();
   }

   template<class SpecificDecl>
   using specific_decl_iterator_range
       = llvm::iterator_range<specific_decl_iterator<SpecificDecl>>;

   template<class SpecificDecl>
   specific_decl_iterator_range<SpecificDecl> getDecls() const
   {
      return {decl_begin<SpecificDecl>(), decl_end<SpecificDecl>()};
   }

   template<class SpecificDecl, bool (SpecificDecl::*Predicate)() const>
   class filtered_decl_iterator {
   public:
      using value_type = SpecificDecl*;
      using reference = const value_type&;
      using pointer = const value_type*;
      using iterator_category = std::forward_iterator_tag;
      using difference_type = std::ptrdiff_t;

      filtered_decl_iterator() = default;
      filtered_decl_iterator(Decl* Ptr) : Current(Ptr) { skipToNext(); }

      value_type operator*() const
      {
         return support::cast<SpecificDecl>(Current);
      }

      value_type operator->() const { return **this; }

      filtered_decl_iterator& operator++()
      {
         Current = Current->getNextDeclInContext();
         skipToNext();

         return *this;
      }

      filtered_decl_iterator operator++(int)
      {
         auto tmp = filtered_decl_iterator(Current);
         ++(*this);

         return tmp;
      }

      bool operator==(filtered_decl_iterator const& it) const
      {
         return Current == it.Current;
      }

      bool operator!=(filtered_decl_iterator const& it) const
      {
         return !operator==(it);
      }

   private:
      Decl* Current = nullptr;

      void skipToNext()
      {
         while (Current
                && (!support::isa<SpecificDecl>(Current)
                    || !(support::cast<SpecificDecl>(Current)->*Predicate)()))
            Current = Current->getNextDeclInContext();
      }
   };

   template<class SpecificDecl, bool (SpecificDecl::*Predicate)() const>
   filtered_decl_iterator<SpecificDecl, Predicate> decl_begin() const
   {
      return filtered_decl_iterator<SpecificDecl, Predicate>(firstDecl);
   }

   template<class SpecificDecl, bool (SpecificDecl::*Predicate)() const>
   filtered_decl_iterator<SpecificDecl, Predicate> decl_end() const
   {
      return filtered_decl_iterator<SpecificDecl, Predicate>();
   }

   template<class SpecificDecl, bool (SpecificDecl::*Predicate)() const>
   llvm::iterator_range<filtered_decl_iterator<SpecificDecl, Predicate>>
   getDecls() const
   {
      return {decl_begin<SpecificDecl, Predicate>(),
              decl_end<SpecificDecl, Predicate>()};
   }
};

} // namespace ast
} // namespace cdot

#endif // CDOT_DECLBASE_H
