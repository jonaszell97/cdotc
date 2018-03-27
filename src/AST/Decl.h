//
// Created by Jonas Zell on 22.01.18.
//

#ifndef CDOT_NAMEDDECL_H
#define CDOT_NAMEDDECL_H

#include "AST/Attribute/Attr.h"
#include "AST/SourceType.h"
#include "AST/Passes/SemanticAnalysis/Template.h"
#include "Basic/DeclarationName.h"
#include "Basic/Precedence.h"
#include "Support/Casting.h"

#include <llvm/Support/TrailingObjects.h>
#include <llvm/ADT/PointerUnion.h>
#include <llvm/ADT/DenseMap.h>
#include <llvm/ADT/SmallPtrSet.h>

#include <unordered_map>

namespace cdot {

class FunctionType;
enum class BuiltinFn : unsigned char;
enum class KnownFunction : unsigned char;

enum class AccessModifier : unsigned char {
   DEFAULT = 0,
   PUBLIC,
   PRIVATE,
   PROTECTED
};

namespace ast {

class NamedDecl;
class Statement;
class DeclStmt;
class DeclContext;
class RecordDecl;
class StaticExpr;
class TemplateParamDecl;
class ImportStmt;
class ConstraintExpr;
class Expression;
class NamespaceDecl;
class CallableDecl;
class TranslationUnit;
class ASTContext;
class SemaPass;
class FuncArgDecl;
class CompoundStmt;
class LocalVarDecl;
class FieldDecl;
class PropDecl;
class MethodDecl;
class InitDecl;
class TypedefDecl;
class DeinitDecl;
class StaticStmt;
class StaticExpr;
class AssociatedTypeDecl;
class EnumDecl;
class UnionDecl;
class ProtocolDecl;
class ExtensionDecl;

using TemplateParamVec = std::vector<TemplateParamDecl*>;

template<class T>
struct InstantiationInfo {
   InstantiationInfo(const SourceLocation &instantiatedFrom,
                     sema::TemplateArgList &&templateArgs,
                     T *specializedTemplate,
                     NamedDecl *instantiatedWithin)
      : instantiatedFrom(instantiatedFrom),
        templateArgs(std::move(templateArgs)),
        specializedTemplate(specializedTemplate),
        instantiatedWithin(instantiatedWithin)
   { }

   InstantiationInfo() = default;

   SourceLocation instantiatedFrom;
   sema::TemplateArgList templateArgs;
   T *specializedTemplate = nullptr;
   NamedDecl *instantiatedWithin = nullptr;
};

enum class ExternKind: unsigned char {
   None,
   C,
   CXX,
   Native
};

class LLVM_ALIGNAS(sizeof(void*)) Decl {
public:
   enum DeclKind: unsigned char {
      NotDecl = 0,

#  define CDOT_DECL(Name)               \
      Name##ID,
#  define CDOT_ABSTRACT(Name)           \
      Name##ID,
#  include "AST/Decl.def"
   };

   enum DeclFlags : unsigned {
      DF_None                = 0u,
      DF_TypeDependent       = 1u,
      DF_ValueDependent      = DF_TypeDependent << 1u,
      DF_IsInvalid           = DF_ValueDependent << 1u,
      DF_SemanticallyChecked = DF_IsInvalid << 1u,
      DF_Static              = DF_SemanticallyChecked << 1u,
      DF_Const               = DF_Static << 1u,
      DF_HasDefinition       = DF_Const << 1u,
      DF_External            = DF_HasDefinition << 1u,
      DF_WasDeclared         = DF_External << 1u,
      DF_Synthesized         = DF_WasDeclared << 1u,
      DF_CheckedAttrs        = DF_Synthesized << 1u,

      StatusFlags            = DF_TypeDependent | DF_ValueDependent |
                               DF_IsInvalid,
   };

   bool isStatic() const { return declFlagSet(DF_Static); }
   bool isConst() const { return declFlagSet(DF_Const); }

   bool hasDefinition() const { return declFlagSet(DF_HasDefinition); }
   void setHasDefinition(bool def) { setDeclFlag(DF_HasDefinition, def); }

   bool isExternal() const { return declFlagSet(DF_External); }
   void setExternal(bool ext) { setDeclFlag(DF_External, ext); }

   bool isDependent() const
   {
      return isTypeDependent() || isValueDependent();
   }

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

   bool isInvalid() const { return declFlagSet(DF_IsInvalid); }
   void setIsInvalid(bool error) { setDeclFlag(DF_IsInvalid, error); }

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
   void setSynthesized(bool synth = true){ setDeclFlag(DF_Synthesized, synth); }

   bool alreadyCheckedOrIsInvalid() const
   {
      static uint32_t mask = DF_IsInvalid | DF_SemanticallyChecked;
      return (flags & mask) != 0;
   }

   void copyStatusFlags(Decl *D);
   void copyStatusFlags(Statement *D);

   bool declFlagSet(DeclFlags f) const { return (flags & f) != 0; }
   void setDeclFlag(DeclFlags f, bool val)
   {
      if (val)
         flags |= f;
      else
         flags &= ~f;
   }

   template<class T>
   bool hasAttribute() const
   {
      for (auto &Attr : getAttributes())
         if (support::isa<T>(Attr))
            return true;

      return false;
   }

   template<class T>
   T *getAttribute()
   {
      for (auto &Attr : getAttributes())
         if (auto A = support::dyn_cast<T>(Attr))
            return A;

      return nullptr;
   }

   llvm::ArrayRef<Attr*> getAttributes() const;
   void setAttributes(llvm::ArrayRef<Attr*> attrs) const;
   void addAttributes(llvm::ArrayRef<Attr*> attrs) const;
   void addAttribute(Attr *A) const;

   ASTContext &getASTCtx() const;

   SourceLocation getSourceLoc() const { return getSourceRange().getStart(); }
   SourceRange getSourceRange() const;

   DeclKind getKind() const { return kind; }
   RecordDecl *getRecord() const;

   DeclContext *getDeclContext() const { return declContext; }
   void setDeclContext(DeclContext *ctx);
   void setDeclContextUnchecked(DeclContext *ctx);

   Decl *getNextDeclInContext() const { return nextDeclInContext; }
   void setNextDeclInContext(Decl *next) { nextDeclInContext = next; }

   bool inAnonymousNamespace() const;
   bool inStdNamespace() const;
   bool isGlobalDecl() const;

   unsigned int getFlags() const { return flags; }

   static bool classofKind(DeclKind kind) { return kind != NotDecl; }
   static bool classof(const Decl *T) { return classofKind(T->getKind()); }

   static DeclContext *castToDeclContext(Decl const *D);
   static Decl *castFromDeclContext(DeclContext const *Ctx);

   friend class DeclContext;

protected:
   explicit Decl(DeclKind kind, unsigned flags = DF_None);

#ifndef NDEBUG
   virtual
#endif
   ~Decl();

   DeclKind kind  : 8;
   unsigned flags : 24;

   Decl *nextDeclInContext = nullptr;
   DeclContext *declContext = nullptr;
};

class NamedDecl: public Decl {
public:
   AccessModifier getAccess() const { return access; }
   void setAccess(AccessModifier AS) { access = AS; }

   llvm::StringRef getName() const
   {
      assert(Name.isSimpleIdentifier() && "not a simple identifier");
      return Name.getIdentifierInfo()->getIdentifier();
   }

   DeclarationName getDeclName() const { return Name; }
   IdentifierInfo *getIdentifierInfo() const
   {
      return Name.getIdentifierInfo();
   }

   bool isOverloadable() const;

   llvm::ArrayRef<StaticExpr*> getConstraints() const;

   bool isTemplate() const;
   bool isTemplateOrInTemplate() const;

   llvm::ArrayRef<TemplateParamDecl*> getTemplateParams() const;

   bool isInstantiation() const;
   const sema::TemplateArgList &getTemplateArgs() const;
   NamedDecl *getSpecializedTemplate() const;
   SourceLocation getInstantiatedFrom() const;
   NamedDecl *getInstantiatedWithin() const;

   std::string getFullName() const { return getJoinedName('.'); }
   std::string getJoinedName(char join, bool includeFile = false) const;

   void printName(llvm::raw_ostream &OS, bool Qualified = false);

   size_t getSpecifierForDiagnostic();

   bool isExported() const;

   unsigned int getFlags() const { return flags; }
   void setFlags(unsigned int flags) { NamedDecl::flags = flags; }

   ExternKind getExternKind() const { return externKind; }
   void setExternKind(ExternKind kind) { externKind = kind; }

   static bool classof(const Decl *T) { return classofKind(T->getKind()); }
   static bool classofKind(DeclKind kind)
   {
      return kind > _firstNamedDeclID
             && kind < _lastNamedDeclID;
   }

protected:
   NamedDecl(DeclKind typeID,
             AccessModifier access,
             DeclarationName DN);

   ExternKind externKind = ExternKind::None;
   AccessModifier access;
   DeclarationName Name;
};

class TemplateParamDecl: public NamedDecl {
public:
   static TemplateParamDecl *Create(ASTContext &C,
                                    IdentifierInfo *II,
                                    SourceType covariance,
                                    SourceType contravariance,
                                    Expression *defaultValue,
                                    SourceLocation TypeNameOrValueLoc,
                                    SourceLocation NameLoc,
                                    SourceLocation EllipsisLoc);

   static TemplateParamDecl *Create(ASTContext &C,
                                    IdentifierInfo *II,
                                    SourceType valueType,
                                    Expression *defaultValue,
                                    SourceLocation TypeNameOrValueLoc,
                                    SourceLocation NameLoc,
                                    SourceLocation EllipsisLoc);

   TemplateParamDecl(const TemplateParamDecl &) = default;

   static bool classof(Decl const* T) { return classofKind(T->getKind()); }
   static bool classofKind(DeclKind kind){ return kind == TemplateParamDeclID; }
   
private:
   TemplateParamDecl(IdentifierInfo *II,
                     SourceType covariance,
                     SourceType contravariance,
                     Expression *defaultValue,
                     SourceLocation TypeNameOrValueLoc,
                     SourceLocation NameLoc,
                     SourceLocation EllipsisLoc);

   TemplateParamDecl(IdentifierInfo *II,
                     SourceType valueType,
                     Expression *defaultValue,
                     SourceLocation TypeNameOrValueLoc,
                     SourceLocation NameLoc,
                     SourceLocation EllipsisLoc);

   SourceType covariance;
   SourceType contravariance;

   bool typeName : 1;

   Expression *defaultValue;

   SourceLocation TypeNameOrValueLoc;
   SourceLocation NameLoc;
   SourceLocation EllipsisLoc;

public:
   SourceLocation getTypeNameOrValueLoc() const { return TypeNameOrValueLoc; }
   SourceLocation getNameLoc() const { return NameLoc; }
   SourceLocation getEllipsisLoc() const { return EllipsisLoc; }
   SourceRange getSourceRange() const
   {
      return SourceRange(TypeNameOrValueLoc ? TypeNameOrValueLoc
                                            : NameLoc);
   }

   const SourceType &getCovariance() const { return covariance; }
   const SourceType &getValueType() const { return covariance; }

   void setCovariance(SourceType cov) { covariance = cov; }

   const SourceType &getContravariance() const { return contravariance; }
   void setContravariance(SourceType con) { contravariance = con; }

   Expression *getDefaultValue() const { return defaultValue; }
   void setDefaultValue(Expression *val) { defaultValue = val; }

   bool isTypeName() const { return typeName; }
   bool isVariadic() const { return EllipsisLoc.isValid(); }
};

class VarDecl: public NamedDecl {
public:
   static bool classof(Decl const* T) { return classofKind(T->getKind()); }
   static bool classofKind(DeclKind kind)
   {
      return kind > _firstVarDeclID && kind < _lastVarDeclID;
   }

protected:
   VarDecl(DeclKind id,
           AccessModifier access,
           SourceLocation VarOrLetLoc,
           SourceLocation ColonLoc,
           bool isConst,
           IdentifierInfo *II,
           SourceType type,
           Expression* value);

   SourceLocation VarOrLetLoc;
   SourceLocation ColonLoc;
   SourceType type;
   Expression* value = nullptr;

   bool CanElideCopy : 1;

public:
   SourceRange getSourceRange() const;
   SourceLocation getVarOrLetLoc() const { return VarOrLetLoc; }
   SourceLocation getColonLoc() const { return ColonLoc; }

   const SourceType &getType() const { return type; }
   void setType(SourceType ty) { type = ty; }

   Expression* getValue() const { return value; }
   void setValue(Expression *V) { value = V; }

   bool canElideCopy() const { return CanElideCopy; }
   void setCanElideCopy(bool CanElide) { CanElideCopy = CanElide; }
};

class LocalVarDecl: public VarDecl {
   LocalVarDecl(AccessModifier access,
                SourceLocation VarOrLetLoc,
                SourceLocation ColonLoc,
                bool isConst,
                IdentifierInfo *II,
                SourceType type,
                Expression* value);

   bool IsNRVOCand : 1;

public:
   static LocalVarDecl *Create(ASTContext &C,
                               AccessModifier access,
                               SourceLocation VarOrLetLoc,
                               SourceLocation ColonLoc,
                               bool isConst,
                               IdentifierInfo *II,
                               SourceType type,
                               Expression* value);

   bool isNRVOCandidate() const { return IsNRVOCand; }
   void setIsNRVOCandidate(bool NRVO) { IsNRVOCand = NRVO; }

   static bool classof(Decl const* T) { return classofKind(T->getKind()); }
   static bool classofKind(DeclKind kind) { return kind == LocalVarDeclID; }
};

class GlobalVarDecl: public VarDecl {
   GlobalVarDecl(AccessModifier access,
                 SourceLocation VarOrLetLoc,
                 SourceLocation ColonLoc,
                 bool isConst,
                 IdentifierInfo *II,
                 SourceType type,
                 Expression* value);

   size_t globalOrdering = 0;

public:
   static GlobalVarDecl *Create(ASTContext &C,
                                AccessModifier access,
                                SourceLocation VarOrLetLoc,
                                SourceLocation ColonLoc,
                                bool isConst,
                                IdentifierInfo *II,
                                SourceType type,
                                Expression* value);

   size_t getGlobalOrdering() const { return globalOrdering; }
   void setGlobalOrdering(size_t Order) { globalOrdering = Order; }

   static bool classof(Decl const* T) { return classofKind(T->getKind()); }
   static bool classofKind(DeclKind kind) { return kind == GlobalVarDeclID; }
};

class FuncArgDecl: public VarDecl {
   FuncArgDecl(SourceLocation VarLetOrIdentLoc,
               SourceLocation ColonLoc,
               IdentifierInfo *II,
               SourceType argType,
               Expression* defaultValue,
               bool variadicArgPackExpansion,
               bool isConst,
               bool cstyleVararg);

   bool VariadicArgPackExpansion : 1;
   bool IsConst : 1;
   bool Vararg : 1;
   bool CstyleVararg : 1;

public:
   static FuncArgDecl *Create(ASTContext &C,
                              SourceLocation VarLetOrIdentLoc,
                              SourceLocation ColonLoc,
                              IdentifierInfo *II,
                              SourceType argType,
                              Expression* defaultValue,
                              bool variadicArgPackExpansion,
                              bool isConst,
                              bool cstyleVararg = false);

   Expression* getDefaultVal() const { return value; }
   void setDefaultVal(Expression *defaultVal) { value = defaultVal; }

   bool isVariadicArgPackExpansion() const { return VariadicArgPackExpansion; }
   bool isConst() const { return IsConst; }
   bool isVararg() const { return Vararg; }
   bool isCstyleVararg() const { return CstyleVararg; }

   static bool classofKind(DeclKind kind) { return kind == FuncArgDeclID; }
   static bool classof(Decl const* T) { return classofKind(T->getKind()); }
};

template <class ToTy,
   bool IsKnownSubtype = ::std::is_base_of<DeclContext, ToTy>::value>
struct cast_convert_decl_context {
   static const ToTy *doit(const DeclContext *Val) {
      return static_cast<const ToTy*>(Decl::castFromDeclContext(Val));
   }

   static ToTy *doit(DeclContext *Val) {
      return static_cast<ToTy*>(Decl::castFromDeclContext(Val));
   }
};

// Specialization selected when ToTy is a known subclass of DeclContext.
template <class ToTy>
struct cast_convert_decl_context<ToTy, true> {
   static const ToTy *doit(const DeclContext *Val) {
      return static_cast<const ToTy*>(Val);
   }

   static ToTy *doit(DeclContext *Val) {
      return static_cast<ToTy*>(Val);
   }
};

class LookupResult {
public:
   using ArrayTy = llvm::ArrayRef<NamedDecl*>;

   LookupResult() = default;

   LookupResult(NamedDecl *Single)
      : Single(Single), Result(Single ? Single : ArrayTy())
   {}

   LookupResult(ArrayTy Result)
      : Result(Result)
   {}

private:
   NamedDecl *Single = nullptr;
   ArrayTy Result;

public:
   class iterator;

   using IteratorBase =
      llvm::iterator_adaptor_base<iterator, ArrayTy::iterator,
         std::random_access_iterator_tag,
         NamedDecl *const>;

   class iterator: public IteratorBase {
      value_type SingleElement;

   public:
      iterator() = default;

      explicit iterator(pointer Pos, value_type Single = nullptr)
         : IteratorBase(Pos), SingleElement(Single)
      {}

      reference operator*() const
      {
         return SingleElement ? SingleElement : IteratorBase::operator*();
      }
   };

   using const_iterator = iterator;
   using pointer        = iterator::pointer;
   using reference      = iterator::reference;

   iterator begin() const { return iterator(Result.begin(), Single); }
   iterator end() const { return iterator(Result.end(), Single); }

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
   using VecTy  = llvm::SmallVector<NamedDecl*, 4>;
   using DataTy = llvm::PointerUnion<NamedDecl*, VecTy*>;

   DeclList(NamedDecl *Decl)
      : Data(Decl)
   {}

   DeclList(DeclList &&DL) noexcept : Data(DL.Data)
   {
      DL.Data = (NamedDecl*)nullptr;
   }

   DeclList &operator=(DeclList &&DL) noexcept
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

   void appendDecl(NamedDecl *Decl)
   {
      if (auto Single = getAsDecl()) {
         auto Vec = new VecTy{ Single, Decl };
         Data = DataTy(Vec);
      }
      else {
         getAsVec()->push_back(Decl);
      }
   }

   bool isNull() const
   {
      return Data.isNull();
   }

   NamedDecl *getAsDecl() const
   {
      return Data.dyn_cast<NamedDecl*>();
   }

   VecTy *getAsVec() const
   {
      return Data.dyn_cast<VecTy*>();
   }

   LookupResult getAsLookupResult() const
   {
      if (isNull())
         return LookupResult();

      if (auto Single = getAsDecl())
         return LookupResult(Single);

      return LookupResult(*getAsVec());
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

   bool isTransparent() const;

   void addDecl(Decl *decl);

   [[nodiscard]]
   AddDeclResultKind addDecl(NamedDecl *decl);

   [[nodiscard]]
   AddDeclResultKind addDecl(DeclarationName Name, NamedDecl *decl);

   void addTransparentDecls();

   LookupResult lookupOwn(DeclarationName name) const
   {
      auto it = namedDecls.find(name);
      if (it == namedDecls.end())
         return {};

      return it->getSecond().getAsLookupResult();
   }

   template<class T>
   T *lookupOwn(DeclarationName name) const
   {
      return support::dyn_cast_or_null<T>(lookupSingle(name));
   }

   LookupResult lookup(DeclarationName name) const;
   NamedDecl *lookupSingle(DeclarationName name) const;

   bool hasAnyDeclNamed(DeclarationName name) const
   {
      return namedDecls.find(name) != namedDecls.end();
   }

   template<class T>
   T* lookupSingle(DeclarationName name) const
   {
      return support::dyn_cast_or_null<T>(lookupSingle(name));
   }

   template<class NodeType, class Fn>
   void forEach(Fn const& fn)
   {
      for (auto &decl : getDecls()) {
         if (auto N = support::dyn_cast<NodeType>(decl)) {
            fn(N);
         }
      }
   }

   template<class NodeType, class Fn>
   void forEachRecursive(Fn const& fn)
   {
      for (auto &decl : getDecls()) {
         if (auto N = support::dyn_cast<NodeType>(decl)) {
            fn(N);
         }
         if (auto Ctx = support::dyn_cast<DeclContext>(decl)) {
            assert(Ctx != this);
            Ctx->forEachRecursive<NodeType>(fn);
         }
      }
   }

   NamespaceDecl *getClosestNamespace() const;
   TranslationUnit *getTranslationUnit() const;

   bool isGlobalDeclContext() const;

   template<class NodeType>
   size_t count() const
   {
      size_t cnt = 0;
      for (auto &decl : getDecls()) {
         cnt += support::isa<NodeType>(decl);
      }

      return cnt;
   }

   template<class NodeType>
   size_t countRecursive() const
   {
      size_t cnt = 0;
      for (auto &decl : getDecls()) {
         cnt += support::isa<NodeType>(decl);
         if (auto Ctx = support::dyn_cast<DeclContext>(decl))
            cnt += Ctx->countRecursive<NodeType>();
      }

      return cnt;
   }

   DeclContext *getParentCtx() const { return parentCtx; }
   void setParentCtx(DeclContext *parent);
   void setParentCtxUnchecked(DeclContext *parent);

   void clear()
   {
      firstDecl = nullptr;
      lastAddedDecl = nullptr;
      namedDecls.clear();
   }

   void makeAllDeclsAvailable(DeclContext *Ctx);
   void makeDeclAvailable(NamedDecl *decl);
   void makeDeclAvailable(DeclarationName Name, NamedDecl *decl);
   void replaceDecl(Decl *Orig, Decl *Rep);

   Decl::DeclKind getDeclKind() const { return declKind; }

   static bool classofKind(Decl::DeclKind kind)
   {
      switch (kind) {
#     define CDOT_DECL_CONTEXT(Name)         \
      case Decl::Name##ID:
#     include "AST/Decl.def"
         return true;
      default:
         return false;
      }
   }

   static bool classof(Decl const *T) { return classofKind(T->getKind()); }
   static bool classof(DeclContext const* T) { return true; }

protected:
   explicit DeclContext(Decl::DeclKind typeID)
      : declKind(typeID)
   {}

   using DeclsMap = llvm::SmallDenseMap<DeclarationName, DeclList, 4>;

   Decl::DeclKind declKind;

   DeclsMap namedDecls;
   Decl *firstDecl = nullptr;
   Decl *lastAddedDecl = nullptr;

   DeclContext *parentCtx = nullptr;

   void addTransparentDecls(DeclContext &Ctx);

public:
   class decl_iterator {
   public:
      using value_type        = Decl *;
      using reference         = const value_type &;
      using pointer           = const value_type *;
      using iterator_category = std::forward_iterator_tag;
      using difference_type   = std::ptrdiff_t;

      decl_iterator() = default;
      explicit decl_iterator(value_type Ptr) : Current(Ptr) {}

      reference operator*()    const { return Current; }
      value_type operator->()  const { return Current; }

      decl_iterator &operator++()
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

      bool operator==(decl_iterator const &it) const
      {
         return Current == it.Current;
      }

      bool operator!=(decl_iterator const &it) const
      {
         return !operator==(it);
      }

   private:
      value_type Current = nullptr;
   };

   decl_iterator decl_begin() const { return decl_iterator(firstDecl); }
   decl_iterator decl_end()   const { return decl_iterator(); }

   using decl_range = llvm::iterator_range<decl_iterator>;

   decl_range getDecls() const
   {
      return decl_range(decl_begin(), decl_end());
   }

   template<class SpecificDecl>
   class specific_decl_iterator {
   public:
      using value_type        = SpecificDecl*;
      using reference         = const value_type &;
      using pointer           = const value_type *;
      using iterator_category = std::forward_iterator_tag;
      using difference_type   = std::ptrdiff_t;

      specific_decl_iterator() = default;
      explicit specific_decl_iterator(Decl *Ptr) : Current(Ptr)
      {
         skipToNext();
      }

      value_type operator*() const
      {
         return support::cast<SpecificDecl>(Current);
      }

      value_type operator->()  const { return **this; }

      specific_decl_iterator &operator++()
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

      bool operator==(specific_decl_iterator const &it) const
      {
         return Current == it.Current;
      }

      bool operator!=(specific_decl_iterator const &it) const
      {
         return !operator==(it);
      }

   private:
      Decl *Current = nullptr;

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

   template<class SpecificDecl>
   llvm::iterator_range<specific_decl_iterator<SpecificDecl>> getDecls() const
   {
      return { decl_begin<SpecificDecl>(), decl_end<SpecificDecl>() };
   }

   template<class SpecificDecl, bool (SpecificDecl::*Predicate)() const>
   class filtered_decl_iterator {
   public:
      using value_type        = SpecificDecl*;
      using reference         = const value_type &;
      using pointer           = const value_type *;
      using iterator_category = std::forward_iterator_tag;
      using difference_type   = std::ptrdiff_t;

      filtered_decl_iterator() = default;
      filtered_decl_iterator(Decl *Ptr) : Current(Ptr)
      {
         skipToNext();
      }

      value_type operator*() const
      {
         return support::cast<SpecificDecl>(Current);
      }

      value_type operator->()  const { return **this; }

      filtered_decl_iterator &operator++()
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

      bool operator==(filtered_decl_iterator const &it) const
      {
         return Current == it.Current;
      }

      bool operator!=(filtered_decl_iterator const &it) const
      {
         return !operator==(it);
      }

   private:
      Decl *Current = nullptr;

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
      return { decl_begin<SpecificDecl, Predicate>(),
         decl_end<SpecificDecl, Predicate>() };
   }
};

class GlobalDeclContext: public DeclContext {
   GlobalDeclContext();

public:
   static GlobalDeclContext *Create(ASTContext &C);

   static bool classofKind(Decl::DeclKind kind){ return kind == Decl::NotDecl; }
   static bool classof(DeclContext const *T)
   {
      return classofKind(T->getDeclKind());
   }

   static DeclContext *castToDeclContext(GlobalDeclContext const *D)
   {
      return static_cast<DeclContext*>(const_cast<GlobalDeclContext*>(D));
   }

   static GlobalDeclContext *castFromDeclContext(DeclContext const *Ctx)
   {
      return static_cast<GlobalDeclContext*>(const_cast<DeclContext*>(Ctx));
   }
};

class TranslationUnit final:
   public NamedDecl,
   public DeclContext,
   private llvm::TrailingObjects<TranslationUnit, ImportStmt*> {
public:
   static TranslationUnit *Create(ASTContext &ASTCtx,
                                  IdentifierInfo *fileName,
                                  size_t sourceId,
                                  llvm::ArrayRef<ImportStmt*> imports);

   static bool classofKind(DeclKind kind) { return kind == TranslationUnitID; }
   static bool classof(Decl const* T)
   {
      return T->getKind() == TranslationUnitID;
   }

   static DeclContext *castToDeclContext(TranslationUnit const *D)
   {
      return static_cast<DeclContext*>(const_cast<TranslationUnit*>(D));
   }

   static TranslationUnit *castFromDeclContext(DeclContext const *Ctx)
   {
      return static_cast<TranslationUnit*>(const_cast<DeclContext*>(Ctx));
   }

   friend TrailingObjects;

private:
   TranslationUnit(ASTContext &ASTCtx,
                   IdentifierInfo *fileName,
                   size_t sourceId,
                   llvm::ArrayRef<ImportStmt*> imports);

   ASTContext &ASTCtx;
   size_t sourceId;
   unsigned numImports;

public:
   llvm::StringRef getFileName() const { return getName(); }
   size_t getSourceId() const { return sourceId; }

   SourceRange getSourceRange() const { return SourceRange(); }

   using import_iterator = ImportStmt**;
   using import_range    = llvm::ArrayRef<ImportStmt*>;

   size_t import_size() const { return numImports; }

   import_iterator import_begin() { return getTrailingObjects<ImportStmt*>(); }
   import_iterator import_end() { return import_begin() + import_size(); }

   import_range getImports()
   {
      return import_range(import_begin(), import_size());
   }

   ASTContext &getASTCtx() const { return ASTCtx; }
};

class NamespaceDecl: public NamedDecl, public DeclContext {
   NamespaceDecl(SourceLocation NamespaceLoc,
                 SourceLocation LBrace,
                 IdentifierInfo *II,
                 bool isAnonymous);

   SourceLocation NamespaceLoc;
   SourceRange Braces;
   bool anonymousNamespace = false;
   
public:
   static NamespaceDecl *Create(ASTContext &C,
                                SourceLocation NamespaceLoc,
                                SourceLocation LBrace,
                                IdentifierInfo *II,
                                bool isAnonymous);

   SourceLocation getNamespaceLoc() const { return NamespaceLoc; }

   SourceRange getBraceRange() const { return Braces; }
   void setRBraceLoc(SourceLocation RBraceLoc)
   {
      Braces = SourceRange(Braces.getStart(), RBraceLoc);
   }

   SourceRange getSourceRange() const
   {
      return SourceRange(NamespaceLoc, Braces.getEnd());
   }

   bool isAnonymousNamespace() const { return anonymousNamespace; }

   static bool classofKind(DeclKind kind) { return kind == NamespaceDeclID; }
   static bool classof(Decl const* T) { return classofKind(T->getKind()); }

   static DeclContext *castToDeclContext(NamespaceDecl const *D)
   {
      return static_cast<DeclContext*>(const_cast<NamespaceDecl*>(D));
   }

   static NamespaceDecl *castFromDeclContext(DeclContext const *Ctx)
   {
      return static_cast<NamespaceDecl*>(const_cast<DeclContext*>(Ctx));
   }
};

class CompoundDecl: public Decl, public DeclContext {
   SourceRange SR;
   bool Transparent : 1;

public:
   // allow stack construction
   CompoundDecl(SourceLocation LBraceLoc,
                bool Transparent);

   static CompoundDecl *Create(ASTContext &C,
                               SourceLocation LBraceLoc,
                               bool Transparent);

   static bool classofKind(DeclKind kind) { return kind == CompoundDeclID; }
   static bool classof(Decl const *T) { return classofKind(T->getKind()); }

   SourceRange getSourceRange() const { return SR; }
   SourceLocation getLBraceLoc() const { return SR.getStart(); }
   SourceLocation getRBraceLoc() const { return SR.getEnd(); }

   // CompoundDecl needs to be created before its end location is known
   void setRBraceLoc(SourceLocation loc)
   {
      SR = SourceRange(SR.getStart(), loc);
   }

   bool isTransparent() const { return Transparent; }
};

class CallableDecl: public NamedDecl, public DeclContext,
                    public llvm::FoldingSetNode {
public:
   static bool classof(Decl const* T) { return classofKind(T->getKind()); }
   static bool classofKind(DeclKind kind)
   {
      return kind > _firstCallableDeclID && kind < _lastCallableDeclID;
   }

   void Profile(llvm::FoldingSetNodeID &ID)
   {
      Profile(ID, instantiationInfo->specializedTemplate,
              instantiationInfo->templateArgs);
   }

   static void Profile(llvm::FoldingSetNodeID &ID, CallableDecl *Template,
                       sema::TemplateArgList &list) {
      ID.AddPointer(Template);
      list.Profile(ID);
   }

protected:
   CallableDecl(DeclKind typeID,
                AccessModifier am,
                SourceLocation DefLoc,
                DeclarationName Name,
                SourceType returnType,
                std::vector<FuncArgDecl*> &&args,
                Statement* body,
                OperatorInfo op,
                std::vector<TemplateParamDecl*> &&templateParams);

   SourceLocation DefLoc;
   std::string linkageName;

   FunctionType *functionType = nullptr;

   SourceType returnType;
   std::vector<FuncArgDecl* > args;
   Statement* body;

   std::vector<TemplateParamDecl*> templateParams;
   InstantiationInfo<CallableDecl> *instantiationInfo = nullptr;

   LocalVarDecl* NRVOCandidate = nullptr;
   llvm::SmallPtrSet<Type*, 2> thrownTypes;
   size_t methodID = 0;

   KnownFunction knownFnKind = KnownFunction(0);

   OperatorInfo op;

   enum Flag : uint32_t {
      // general flags
      NoThrow  = 1u,
      ExternC  = NoThrow << 1u,
      Native   = ExternC << 1u,
      ConvOp   = Native  << 1u,
      External = ConvOp << 1u,
      Main     = External << 1u,
      Vararg   = Main << 1u,
      CVarArg  = Vararg << 1u,
      Defined  = CVarArg << 1u,
      NoReturn = Defined << 1u,

      // method flags
      Abstract         = NoReturn << 1u,
      Alias            = Abstract << 1u,
      MutableSelf      = Alias << 1u,
      ProtoMethod      = MutableSelf << 1u,
      Virtual          = ProtoMethod << 1u,
      Override         = Virtual << 1u,
      Property         = Override << 1u,
      MemberwiseInit   = Property << 1u,
      DefaultInit      = MemberwiseInit << 1u,
      ProtoDefaultImpl = DefaultInit << 1u,
   };

   uint32_t Flags;

   bool getFlag(Flag F) const { return (Flags & F) != 0; }
   void setFlag(Flag F, bool val)
   {
      if (val)
         Flags |= F;
      else
         Flags &= ~F;
   }

public:
   FunctionType *getFunctionType() const { return functionType; }
   void createFunctionType(SemaPass &SP,
                           unsigned flags = 0,
                           bool lambda = false);

   bool isKnownFunction();
   void checkKnownFnKind();
   KnownFunction getKnownFnKind();

   std::vector<FuncArgDecl* > &getArgs() { return args; }
   llvm::ArrayRef<FuncArgDecl* > getArgs() const { return args; }
   void setArgs(std::vector<FuncArgDecl* > &&args) { this->args = move(args); }

   void setLinkageName(std::string &&LN) { linkageName = move(LN); }

   void setBody(Statement* body)
   {
      setHasDefinition(body != nullptr);
      this->body = body;
   }

   SourceRange getSourceRange() const;
   SourceLocation getDefLoc() const { return DefLoc; }

   const SourceType &getReturnType() const { return returnType; }
   void setReturnType(SourceType RetTy) { returnType = RetTy; }

   llvm::StringRef getLinkageName() const { return linkageName; }

   Statement* getBody() const { return body; }

   size_t getMethodID() const { return methodID; }
   void setMethodID(size_t ID) { methodID = ID; }

   LocalVarDecl *getNRVOCandidate() const { return NRVOCandidate; }
   void setNRVOCandidate(LocalVarDecl *Cand) { NRVOCandidate = Cand; }

   OperatorInfo &getOperator() { return op; }
   OperatorInfo const& getOperator() const { return op; }

   bool isOperator() const { return op.getPrecedenceGroup().isValid(); }

   bool isConversionOp() const { return getFlag(ConvOp); }
   void setIsConversionOp(bool conversionOp) { setFlag(ConvOp, conversionOp); }

   bool isMain() const { return getFlag(Main); }
   void setIsMain(bool main) { setFlag(Main, main); }

   bool isExternal() const { return getFlag(External); }
   void setExternal(bool external) { setFlag(External, external); }

   bool isExternC() const { return getFlag(ExternC); }
   void setExternC(bool externC) { setFlag(ExternC, externC); }

   bool isNative() const { return getFlag(Native); }
   void setNative(bool native) { setFlag(Native, native); }

   bool isVararg() const { return getFlag(Vararg); }
   void setVararg(bool vararg) { setFlag(Vararg, vararg); }

   bool isCstyleVararg() const { return getFlag(CVarArg); }
   void setCstyleVararg(bool cstyleVararg) { setFlag(CVarArg, cstyleVararg); }

   bool hasMutableSelf() const { return getFlag(MutableSelf); }

   bool isNoReturn() const { return getFlag(NoReturn); }
   void setIsNoReturn(bool noRet) { setFlag(NoReturn, noRet); }

   uint32_t getFunctionFlags() const { return Flags; }
   void setFunctionFlags(uint32_t Flags) { CallableDecl::Flags = Flags; }

   bool isNonStaticMethod() const;

   bool willHaveDefinition() const;

   const llvm::SmallPtrSetImpl<Type*> &getThrownTypes() const
   {
      return thrownTypes;
   }

   void addThrownType(Type *ty) { thrownTypes.insert(ty); }

   bool throws(Type *ty) const
   {
      return thrownTypes.find(ty) != thrownTypes.end();
   }

   bool throws() const { return !thrownTypes.empty(); }

   bool isNoThrow() const { return getFlag(NoThrow); }
   void isNoThrow(bool nothrow) { setFlag(NoThrow, nothrow); }

   void setInstantiationInfo(InstantiationInfo<CallableDecl> *instantiationInfo)
   {
      CallableDecl::instantiationInfo = instantiationInfo;
   }

   llvm::ArrayRef<TemplateParamDecl *> getTemplateParams() const
   {
      return templateParams;
   }

   bool isInstantiation() const { return instantiationInfo != nullptr; }

   const SourceLocation &getInstantiatedFrom() const
   {
      return instantiationInfo->instantiatedFrom;
   }

   CallableDecl* getSpecializedTemplate() const
   {
      return instantiationInfo->specializedTemplate;
   }

   const sema::TemplateArgList &getTemplateArgs() const
   {
      return instantiationInfo->templateArgs;
   }

   NamedDecl *getInstantiatedWithin() const
   {
      return instantiationInfo->instantiatedWithin;
   }
};

class CompoundStmt;

class FunctionDecl : public CallableDecl {
   FunctionDecl(AccessModifier am,
                SourceLocation DefLoc,
                DeclarationName II,
                std::vector<FuncArgDecl* > &&args,
                SourceType returnType,
                Statement* body,
                OperatorInfo op,
                std::vector<TemplateParamDecl*> &&templateParams);

public:
   static FunctionDecl *Create(ASTContext &C,
                               AccessModifier am,
                               SourceLocation DefLoc,
                               DeclarationName II,
                               std::vector<FuncArgDecl* > &&args,
                               SourceType returnType,
                               Statement* body,
                               OperatorInfo op,
                               std::vector<TemplateParamDecl*>&&templateParams);

   static bool classof(Decl const* T) { return classofKind(T->getKind()); }
   static bool classofKind(DeclKind kind) { return kind == FunctionDeclID; }

   static DeclContext *castToDeclContext(FunctionDecl const *D)
   {
      return static_cast<DeclContext*>(const_cast<FunctionDecl*>(D));
   }

   static FunctionDecl *castFromDeclContext(DeclContext const *Ctx)
   {
      return static_cast<FunctionDecl*>(const_cast<DeclContext*>(Ctx));
   }
};

class TypedefDecl: public NamedDecl {
   TypedefDecl(AccessModifier access,
               SourceLocation Loc,
               IdentifierInfo *II,
               SourceType origin,
               std::vector<TemplateParamDecl*> &&templateParams);

   SourceLocation Loc;
   SourceType origin;
   std::vector<TemplateParamDecl*> templateParams;

public:
   static TypedefDecl *Create(ASTContext &C,
                              AccessModifier access,
                              SourceLocation Loc,
                              IdentifierInfo *II,
                              SourceType origin,
                              std::vector<TemplateParamDecl*> &&templateParams);

   SourceLocation getSourceLoc() const { return Loc; }
   SourceRange getSourceRange() const { return SourceRange(Loc); }

   const SourceType &getOriginTy() const { return origin; }
   llvm::ArrayRef<TemplateParamDecl*> getTemplateParams() const
   {
      return templateParams;
   }

   static bool classofKind(DeclKind kind) { return kind == TypedefDeclID; }
   static bool classof(Decl const* T) { return classofKind(T->getKind()); }
};

class AliasDecl: public NamedDecl,
                 public DeclContext,
                 public llvm::FoldingSetNode {
   AliasDecl(SourceLocation Loc,
             IdentifierInfo *II,
             StaticExpr* aliasExpr,
             std::vector<TemplateParamDecl*> &&templateParams);

   SourceLocation Loc;
   StaticExpr* aliasExpr;
   std::vector<TemplateParamDecl*> templateParams;
   InstantiationInfo<AliasDecl> *instantiationInfo = nullptr;

public:
   static AliasDecl *Create(ASTContext &C,
                            SourceLocation Loc,
                            IdentifierInfo *II,
                            StaticExpr* aliasExpr,
                            std::vector<TemplateParamDecl*> &&templateParams);

   static bool classofKind(DeclKind kind) { return kind == AliasDeclID; }
   static bool classof(Decl const* T) { return classofKind(T->getKind()); }

   void Profile(llvm::FoldingSetNodeID &ID)
   {
      Profile(ID, instantiationInfo->specializedTemplate,
              instantiationInfo->templateArgs);
   }

   static void Profile(llvm::FoldingSetNodeID &ID, AliasDecl *Template,
                       sema::TemplateArgList &list) {
      ID.AddPointer(Template);
      list.Profile(ID);
   }

   SourceLocation getSourceLoc() const { return Loc; }
   SourceRange getSourceRange() const { return SourceRange(Loc); }

   StaticExpr* getAliasExpr() const { return aliasExpr; }
   void setAliasExpr(StaticExpr *Expr) { aliasExpr = Expr; }

   const Variant &getVal() const;

   llvm::ArrayRef<TemplateParamDecl *> getTemplateParams() const
   {
      return templateParams;
   }

   bool isInstantiation() const { return instantiationInfo != nullptr; }
   InstantiationInfo <AliasDecl> *getInstantiationInfo() const
   {
      return instantiationInfo;
   }

   void setInstantiationInfo(InstantiationInfo <AliasDecl> *instantiationInfo)
   {
      AliasDecl::instantiationInfo = instantiationInfo;
   }

   sema::TemplateArgList const& getTemplateArgs() const
   {
      assert(isInstantiation() && "not a record instantiation!");
      return instantiationInfo->templateArgs;
   }

   SourceLocation getInstantiatedFrom() const
   {
      return instantiationInfo->instantiatedFrom;
   }

   AliasDecl* getSpecializedTemplate() const
   {
      return instantiationInfo->specializedTemplate;
   }

   NamedDecl *getInstantiatedWithin() const
   {
      return instantiationInfo->instantiatedWithin;
   }
};

enum class ImplicitConformanceKind : unsigned char {
   StringRepresentable,
   Hashable,
   Equatable
};

class RecordDecl: public NamedDecl,
                  public DeclContext,
                  public llvm::FoldingSetNode {
public:
   static bool classof(Decl const* T) { return classofKind(T->getKind()); }
   static bool classofKind(DeclKind kind)
   {
      return kind > _firstRecordDeclID && kind < _lastRecordDeclID;
   }

   static DeclContext *castToDeclContext(RecordDecl const *D)
   {
      return static_cast<DeclContext*>(const_cast<RecordDecl*>(D));
   }

   static RecordDecl *castFromDeclContext(DeclContext const *Ctx)
   {
      return static_cast<RecordDecl*>(const_cast<DeclContext*>(Ctx));
   }

   void Profile(llvm::FoldingSetNodeID &ID)
   {
      Profile(ID, instantiationInfo->specializedTemplate,
              instantiationInfo->templateArgs);
   }

   static void Profile(llvm::FoldingSetNodeID &ID, RecordDecl *Template,
                       sema::TemplateArgList &list) {
      ID.AddPointer(Template);
      list.Profile(ID);
   }

   SourceLocation getKeywordLoc() const { return KeywordLoc; }
   SourceRange getBraceRange() const { return BraceRange; }
   SourceRange getSourceRange() const
   {
      return SourceRange(KeywordLoc, BraceRange.getEnd());
   }

   AssociatedTypeDecl* getAssociatedType(DeclarationName name,
                                         ProtocolDecl *P = nullptr) const;

   MethodDecl *getConversionOperator(QualType toType) const;
   MethodDecl *getComparisonOperator(QualType withType) const;

   bool hasMethodWithName(DeclarationName name) const;
   bool hasMethodTemplate(DeclarationName name) const;

   PropDecl *getProperty(DeclarationName name) const;
   FieldDecl* getField(DeclarationName name) const;

   MethodDecl* getMethod(DeclarationName name, bool checkParent = true) const;
   MethodDecl* getMethod(size_t id) const;

   MethodDecl* getOwnMethod(DeclarationName name);
   bool hasInnerRecord(RecordDecl *R) const
   {
      return innerRecords.find(R) != innerRecords.end();
   }

   bool conformsTo(ProtocolDecl *P) const
   {
      return conformances.find(P) != conformances.end();
   }

   bool conformsToBaseTemplate(ProtocolDecl *P) const;
   void addExtension(ExtensionDecl *E);

   [[nodiscard]]
   DeclContext::AddDeclResultKind addDecl(NamedDecl *decl);

protected:
   RecordDecl(DeclKind typeID,
              AccessModifier access,
              SourceLocation KeywordLoc,
              IdentifierInfo *II,
              std::vector<SourceType> &&conformanceTypes,
              std::vector<TemplateParamDecl*> &&templateParams);

   static size_t lastRecordID;

   SourceLocation KeywordLoc;
   SourceRange BraceRange;

   size_t lastMethodID = 1;
   size_t uses = 0;
   size_t recordID;

   std::vector<SourceType> conformanceTypes;

   std::vector<TemplateParamDecl*> templateParams;
   InstantiationInfo<RecordDecl> *instantiationInfo = nullptr;

   llvm::SmallPtrSet<ProtocolDecl*, 4> conformances;
   llvm::SmallPtrSet<RecordDecl*, 4> innerRecords;

   DeinitDecl *deinitializer = nullptr;

   unsigned occupiedBytes = 0;
   unsigned short alignment = 1;

   bool manualAlignment : 1;
   bool opaque          : 1;
   bool implicitlyEquatable : 1;
   bool implicitlyHashable : 1;
   bool implicitlyStringRepresentable : 1;

   MethodDecl *operatorEquals = nullptr;
   MethodDecl *hashCodeFn = nullptr;
   MethodDecl *toStringFn = nullptr;

   std::unordered_multimap<size_t, MethodDecl*> destructuringOperators;

public:
   bool isStruct() const;
   bool isClass() const;
   bool isEnum() const;
   bool isUnion() const;
   bool isProtocol() const;
   bool isRawEnum() const;

   size_t getSpecifierForDiagnostic()
   {
      switch (kind) {
      case ClassDeclID: return 0;
      case StructDeclID: return 1;
      case EnumDeclID: return 2;
      case UnionDeclID: return 3;
      case ProtocolDeclID: return 4;
      default:
         llvm_unreachable("not a record decl");
      }
   }

   llvm::ArrayRef<SourceType> getConformanceTypes() const
   {
      return conformanceTypes;
   }

   const llvm::SmallPtrSet<ProtocolDecl *, 4> &getConformances() const
   {
      return conformances;
   }

   void addConformance(ProtocolDecl *P) { conformances.insert(P); }

   const llvm::SmallPtrSet<RecordDecl *, 4> &getInnerRecords() const
   {
      return innerRecords;
   }

   void addInnerRecord(RecordDecl *R);

   DeinitDecl *getDeinitializer() const { return deinitializer; }

   const std::unordered_multimap<size_t, MethodDecl*> &
   getDestructuringOperators() const
   {
      return destructuringOperators;
   }

   void setInstantiationInfo(InstantiationInfo<RecordDecl> *instantiationInfo)
   {
      RecordDecl::instantiationInfo = instantiationInfo;
   }

   llvm::ArrayRef<TemplateParamDecl *> getTemplateParams() const
   {
      return templateParams;
   }

   bool isInstantiation() const
   {
      return instantiationInfo != nullptr;
   }

   RecordDecl *getSpecializedTemplate() const
   {
      assert(isInstantiation() && "not a record instantiation!");
      return instantiationInfo->specializedTemplate;
   }

   const SourceLocation &getInstantiatedFrom() const
   {
      assert(isInstantiation() && "not a record instantiation!");
      return instantiationInfo->instantiatedFrom;
   }

   sema::TemplateArgList const& getTemplateArgs() const
   {
      assert(isInstantiation() && "not a record instantiation!");
      return instantiationInfo->templateArgs;
   }

   NamedDecl *getInstantiatedWithin() const
   {
      return instantiationInfo->instantiatedWithin;
   }

   sema::ResolvedTemplateArg const* getTemplateArg(llvm::StringRef name) const;

   size_t getUses() const { return uses; }
   size_t getRecordID() const { return recordID; }

   unsigned getSize() const { return occupiedBytes; }
   unsigned short getAlignment() const { return alignment; }

   void setSize(unsigned s) { occupiedBytes = s; }
   void setAlignment(unsigned short al) { alignment = al; }

   bool hasManualAlignment() const { return manualAlignment; }

   bool isOpaque() const { return opaque; }
   void setOpaque(bool opaque) { RecordDecl::opaque = opaque; }

   MethodDecl *getOperatorEquals() const { return operatorEquals; }
   void setOperatorEquals(MethodDecl *Eq) { operatorEquals = Eq; }

   MethodDecl *getHashCodeFn() const { return hashCodeFn; }
   void setHashCodeFn(MethodDecl *fn) { RecordDecl::hashCodeFn = fn; }

   MethodDecl *getToStringFn() const { return toStringFn; }
   void setToStringFn(MethodDecl *fn) { toStringFn = fn; }

   int getNameSelector() const;

   bool isImplicitlyEquatable() const { return implicitlyEquatable; }
   void setImplicitlyEquatable(bool eq) { implicitlyEquatable = eq; }

   bool isImplicitlyHashable() const { return implicitlyHashable; }

   void setImplicitlyHashable(bool hash) { implicitlyHashable = hash; }

   bool isImplicitlyStringRepresentable() const
   {
      return implicitlyStringRepresentable;
   }

   void setImplicitlyStringRepresentable(bool rep)
   {
      implicitlyStringRepresentable = rep;
   }
};

class StructDecl: public RecordDecl {
public:
   static StructDecl *Create(ASTContext &C,
                             AccessModifier access,
                             SourceLocation KeywordLoc,
                             IdentifierInfo *II,
                             std::vector<SourceType> &&conformanceTypes,
                             std::vector<TemplateParamDecl*> &&templateParams);

   InitDecl *getParameterlessConstructor() const
   {
      return parameterlessConstructor;
   }

   void setParameterlessConstructor(InitDecl *parameterlessConstructor)
   {
      StructDecl::parameterlessConstructor = parameterlessConstructor;
   }

   InitDecl *getMemberwiseInitializer() const
   {
      return memberwiseInitializer;
   }

   void setMemberwiseInitializer(InitDecl *memberwiseInitializer)
   {
      StructDecl::memberwiseInitializer = memberwiseInitializer;
   }

   MethodDecl *getDefaultInitializer() const
   {
      return defaultInitializer;
   }

   void setDefaultInitializer(MethodDecl *defaultInitializer)
   {
      StructDecl::defaultInitializer = defaultInitializer;
   }

   llvm::ArrayRef<FieldDecl*> getFields() const
   {
      return fields;
   }

   static bool classof(Decl const* T) { return classofKind(T->getKind()); }
   static bool classofKind(DeclKind kind)
   {
      switch (kind) {
      case StructDeclID:
      case ClassDeclID:
         return true;
      default:
         return false;
      }
   }

   static DeclContext *castToDeclContext(StructDecl const *D)
   {
      return static_cast<DeclContext*>(const_cast<StructDecl*>(D));
   }

   static StructDecl *castFromDeclContext(DeclContext const *Ctx)
   {
      return static_cast<StructDecl*>(const_cast<DeclContext*>(Ctx));
   }

   friend class RecordDecl; // for access to fields

private:
   StructDecl(AccessModifier access,
              SourceLocation KeywordLoc,
              IdentifierInfo *II,
              std::vector<SourceType> &&conformanceTypes,
              std::vector<TemplateParamDecl*> &&templateParams);

protected:
   StructDecl(DeclKind typeID,
              AccessModifier access,
              SourceLocation KeywordLoc,
              IdentifierInfo *II,
              std::vector<SourceType> &&conformanceTypes,
              std::vector<TemplateParamDecl*> &&templateParams);

   std::vector<FieldDecl*> fields;

   InitDecl* parameterlessConstructor = nullptr;
   InitDecl* memberwiseInitializer    = nullptr;
   MethodDecl* defaultInitializer     = nullptr;
};

class ClassDecl: public StructDecl {
public:
   static ClassDecl *Create(ASTContext &C,
                            AccessModifier access,
                            SourceLocation KeywordLoc,
                            IdentifierInfo *II,
                            std::vector<SourceType> &&conformanceTypes,
                            std::vector<TemplateParamDecl*> &&templateParams,
                            SourceType parentClass,
                            bool isAbstract);

   const SourceType &getParentType() const { return parentType; }

   ClassDecl *getParentClass() const { return parentClass; }
   void setParentClass(ClassDecl *C) { parentClass = C; }

   bool isBaseClassOf(ClassDecl const* C) const
   {
      auto Outer = C->getParentClass();
      while (Outer) {
         if (Outer == this)
            return true;

         Outer = Outer->getParentClass();
      }

      return false;
   }

   static bool classof(Decl const* T) { return classofKind(T->getKind()); }
   static bool classofKind(DeclKind kind) { return kind == ClassDeclID; }

   static DeclContext *castToDeclContext(ClassDecl const *D)
   {
      return static_cast<DeclContext*>(const_cast<ClassDecl*>(D));
   }

   static ClassDecl *castFromDeclContext(DeclContext const *Ctx)
   {
      return static_cast<ClassDecl*>(const_cast<DeclContext*>(Ctx));
   }

private:
   ClassDecl(AccessModifier access,
             SourceLocation KeywordLoc,
             IdentifierInfo *II,
             std::vector<SourceType> &&conformanceTypes,
             std::vector<TemplateParamDecl*> &&templateParams,
             SourceType parentClass,
             bool isAbstract);

   SourceType parentType;
   ClassDecl *parentClass = nullptr;
   bool IsAbstract = false;

public:
   bool isAbstract() const { return IsAbstract; }
};

class EnumCaseDecl;

class EnumDecl: public RecordDecl {
public:
   static EnumDecl *Create(ASTContext &C,
                           AccessModifier access,
                           SourceLocation KeywordLoc,
                           IdentifierInfo *II,
                           std::vector<SourceType> &&conformanceTypes,
                           std::vector<TemplateParamDecl*> &&templateParams,
                           SourceType rawType);

   EnumCaseDecl *hasCase(DeclarationName name);

   static bool classof(Decl const* T) { return classofKind(T->getKind()); }
   static bool classofKind(DeclKind kind) { return kind == EnumDeclID; }

   static DeclContext *castToDeclContext(EnumDecl const *D)
   {
      return static_cast<DeclContext*>(const_cast<EnumDecl*>(D));
   }

   static EnumDecl *castFromDeclContext(DeclContext const *Ctx)
   {
      return static_cast<EnumDecl*>(const_cast<DeclContext*>(Ctx));
   }

   friend class RecordDecl; // for maxAssociatedTypes, cases

private:
   EnumDecl(AccessModifier access,
            SourceLocation KeywordLoc,
            IdentifierInfo *II,
            std::vector<SourceType> &&conformanceTypes,
            std::vector<TemplateParamDecl*> &&templateParams,
            SourceType rawType);

   SourceType rawType;
   size_t maxAssociatedTypes = 0;
   std::vector<EnumCaseDecl*> cases;

public:
   const SourceType &getRawType() const { return rawType; }
   void setRawType(SourceType ty) { rawType = ty; }

   size_t getMaxAssociatedTypes() const { return maxAssociatedTypes; }
   llvm::ArrayRef<EnumCaseDecl *> getCases() const { return cases; }

   bool isUnpopulated() const { return cases.empty(); }
};

class UnionDecl: public RecordDecl {
public:
   static UnionDecl *Create(ASTContext &C,
                            AccessModifier access,
                            SourceLocation KeywordLoc,
                            IdentifierInfo *II,
                            std::vector<SourceType> &&conformanceTypes,
                            std::vector<TemplateParamDecl*> &&templateParams);

   bool isConst() const { return IsConst; }
   void isConst(bool is_const) { this->IsConst = is_const; }

   static bool classof(Decl const* T) { return classofKind(T->getKind()); }
   static bool classofKind(DeclKind kind)
   {
      return kind == UnionDeclID;
   }

   static DeclContext *castToDeclContext(UnionDecl const *D)
   {
      return static_cast<DeclContext*>(const_cast<UnionDecl*>(D));
   }

   static UnionDecl *castFromDeclContext(DeclContext const *Ctx)
   {
      return static_cast<UnionDecl*>(const_cast<DeclContext*>(Ctx));
   }

private:
   UnionDecl(AccessModifier access,
             SourceLocation KeywordLoc,
             IdentifierInfo *II,
             std::vector<SourceType> &&conformanceTypes,
             std::vector<TemplateParamDecl*> &&templateParams);

   bool IsConst = false;
};

class ProtocolDecl: public RecordDecl {
public:
   static ProtocolDecl *Create(ASTContext &C,
                               AccessModifier access,
                               SourceLocation KeywordLoc,
                               IdentifierInfo *II,
                               std::vector<SourceType> &&conformanceTypes,
                               std::vector<TemplateParamDecl*>&&templateParams);

   static bool classof(Decl const* T) { return classofKind(T->getKind()); }
   static bool classofKind(DeclKind kind) { return kind == ProtocolDeclID; }

   static DeclContext *castToDeclContext(ProtocolDecl const *D)
   {
      return static_cast<DeclContext*>(const_cast<ProtocolDecl*>(D));
   }

   static ProtocolDecl *castFromDeclContext(DeclContext const *Ctx)
   {
      return static_cast<ProtocolDecl*>(const_cast<DeclContext*>(Ctx));
   }

   void clearTemplateParams() { templateParams.clear(); }

private:
   ProtocolDecl(AccessModifier access,
                SourceLocation KeywordLoc,
                IdentifierInfo *II,
                std::vector<SourceType> &&conformanceTypes,
                std::vector<TemplateParamDecl*> &&templateParams);
};

class ExtensionDecl: public RecordDecl {
public:
   static ExtensionDecl *Create(ASTContext &C,
                                AccessModifier access,
                                SourceLocation KeywordLoc,
                                IdentifierInfo *II,
                                std::vector<SourceType> &&conformanceTypes,
                                std::vector<TemplateParamDecl*>
                                                              &&templateParams);

   static bool classof(Decl const* T) { return classofKind(T->getKind()); }
   static bool classofKind(DeclKind kind) { return kind == ExtensionDeclID; }

   static DeclContext *castToDeclContext(ExtensionDecl const *D)
   {
      return static_cast<DeclContext*>(const_cast<ExtensionDecl*>(D));
   }

   static ExtensionDecl *castFromDeclContext(DeclContext const *Ctx)
   {
      return static_cast<ExtensionDecl*>(const_cast<DeclContext*>(Ctx));
   }

private:
   ExtensionDecl(AccessModifier access,
                 SourceLocation KeywordLoc,
                 IdentifierInfo *II,
                 std::vector<SourceType> &&conformanceTypes,
                 std::vector<TemplateParamDecl*> &&templateParams);
};

class MethodDecl: public CallableDecl {
public:
   static MethodDecl *Create(ASTContext &C,
                             AccessModifier access,
                             SourceLocation DefLoc,
                             DeclarationName II,
                             SourceType returnType,
                             std::vector<FuncArgDecl*> &&args,
                             std::vector<TemplateParamDecl*> &&templateParams,
                             Statement* body,
                             bool isStatic);

   static MethodDecl *CreateOperator(ASTContext &C,
                                     AccessModifier access,
                                     SourceLocation DefLoc,
                                     DeclarationName OperatorName,
                                     SourceType returnType,
                                     std::vector<FuncArgDecl*> &&args,
                                     std::vector<TemplateParamDecl*>
                                                               &&templateParams,
                                     Statement* body,
                                     OperatorInfo op,
                                     bool isStatic);

   static MethodDecl *CreateConversionOp(ASTContext &C,
                                         AccessModifier access,
                                         SourceLocation DefLoc,
                                         SourceType returnType,
                                         std::vector<FuncArgDecl*> &&args,
                                         std::vector<TemplateParamDecl*>
                                                               &&templateParams,
                                         Statement* body);

   static bool classof(Decl const* T) { return classofKind(T->getKind()); }
   static bool classofKind(DeclKind kind)
   {
      switch (kind) {
      case MethodDeclID:
      case InitDeclID:
      case DeinitDeclID:
         return true;
      default:
         return false;
      }
   }

   static DeclContext *castToDeclContext(MethodDecl const *D)
   {
      return static_cast<DeclContext*>(const_cast<MethodDecl*>(D));
   }

   static MethodDecl *castFromDeclContext(DeclContext const *Ctx)
   {
      return static_cast<MethodDecl*>(const_cast<DeclContext*>(Ctx));
   }

private:
   MethodDecl(AccessModifier access,
              SourceLocation DefLoc,
              DeclarationName II,
              SourceType returnType,
              std::vector<FuncArgDecl*> &&args,
              std::vector<TemplateParamDecl*> &&templateParams,
              Statement* body,
              bool isStatic);

   MethodDecl(AccessModifier access,
              SourceLocation DefLoc,
              DeclarationName OperatorName,
              SourceType returnType,
              std::vector<FuncArgDecl*> &&args,
              std::vector<TemplateParamDecl*> &&templateParams,
              Statement* body,
              OperatorInfo op,
              bool isStatic);

   // the declaration name of a conversion operator is only known after the
   // first declaration pass
   MethodDecl(AccessModifier access,
              SourceLocation DefLoc,
              SourceType returnType,
              std::vector<FuncArgDecl*> &&args,
              std::vector<TemplateParamDecl*> &&templateParams,
              Statement* body);

protected:
   MethodDecl(DeclKind typeID,
              AccessModifier access,
              SourceLocation Loc,
              DeclarationName Name,
              SourceType returnType,
              std::vector<FuncArgDecl*> &&args,
              std::vector<TemplateParamDecl*> &&templateParams,
              Statement* body);

   size_t methodID;
   size_t protocolTableOffset = 0;

   SourceLocation BodyInstantiationLoc;
   Statement *BodyTemplate = nullptr;

public:
   void setName(DeclarationName Name)
   {
      assert(!this->Name && "name should only be set once!");
      this->Name = Name;
   }

   size_t getMethodID() const { return methodID; }

   size_t getProtocolTableOffset() const { return protocolTableOffset; }
   void setProtocolTableOffset(size_t offset) { protocolTableOffset = offset; }

   SourceLocation getBodyInstantiationLoc()const{ return BodyInstantiationLoc; }
   void setBodyInstantiationLoc(SourceLocation Loc)
   {
      BodyInstantiationLoc = Loc;
   }

   Statement *getBodyTemplate() const { return BodyTemplate; }
   void setBodyTemplate(Statement *T) { BodyTemplate = T; }

   bool isAbstract() const { return getFlag(Abstract); }
   bool isProtocolMethod() const { return getFlag(ProtoMethod); }
   bool isVirtual() const { return getFlag(Virtual); }
   bool isOverride() const { return getFlag(Override); }
   bool isProperty() const { return getFlag(Property); }

   bool isTemplatedInitializer() const;
   bool isProtocolDefaultImpl() const { return getFlag(ProtoMethod); }
   bool isHasDefinition() const { return getFlag(Defined); }

   bool isMemberwiseInitializer() const { return getFlag(MemberwiseInit); }
   bool isDefaultInitializer() const { return getFlag(DefaultInit); }
   bool isCastOp() const { return getFlag(ConvOp); }

   void setDefaultInitializer(bool DI) { setFlag(DefaultInit, DI); }
   void setMutating(bool mutating) { setFlag(MutableSelf, mutating); }
   void setIsProtocolMethod(bool PM) { setFlag(ProtoMethod, PM); }
   void setIsVirtual(bool virt) { setFlag(Virtual, virt); }
   void setProperty(bool property) { setFlag(Property, property); }
   void setProtocolDefaultImpl(bool impl) { setFlag(ProtoDefaultImpl, impl); }
   void setMemberwiseInitializer(bool init) { setFlag(MemberwiseInit, init); }
};

class InitDecl: public MethodDecl {
public:
   static InitDecl *CreateMemberwise(ASTContext &C,
                                     AccessModifier am,
                                     SourceLocation Loc,
                                     QualType InitializedType = QualType());

   static InitDecl *Create(ASTContext &C,
                           AccessModifier am,
                           SourceLocation Loc,
                           std::vector<FuncArgDecl* > &&args,
                           std::vector<TemplateParamDecl*> &&templateParams,
                           Statement* body,
                           QualType InitializedType = QualType());

   std::vector<TemplateParamDecl*> &getTemplateParamsRef()
   {
      return templateParams;
   }

   static bool classof(Decl const* T) { return classofKind(T->getKind()); }
   static bool classofKind(DeclKind kind) { return kind == InitDeclID; }

   static DeclContext *castToDeclContext(InitDecl const *D)
   {
      return static_cast<DeclContext*>(const_cast<InitDecl*>(D));
   }

   static InitDecl *castFromDeclContext(DeclContext const *Ctx)
   {
      return static_cast<InitDecl*>(const_cast<DeclContext*>(Ctx));
   }

private:
   InitDecl(AccessModifier am,
            SourceLocation Loc,
            DeclarationName Name);

   InitDecl(AccessModifier am,
            SourceLocation Loc,
            std::vector<FuncArgDecl* > &&args,
            std::vector<TemplateParamDecl*> &&templateParams,
            Statement* body,
            DeclarationName Name);

   bool ExplicitMemberwise : 1;

public:
   bool isMemberwise() const { return ExplicitMemberwise; }
};

class DeinitDecl: public MethodDecl {
   explicit DeinitDecl(SourceLocation Loc,
                       Statement* body,
                       DeclarationName Name);

public:
   static DeinitDecl *Create(ASTContext &C,
                             SourceLocation Loc,
                             Statement* body,
                             QualType DeinitializedType = QualType());

   static bool classof(Decl const* T) { return classofKind(T->getKind()); }
   static bool classofKind(DeclKind kind) { return kind == DeinitDeclID; }

   static DeclContext *castToDeclContext(DeinitDecl const *D)
   {
      return static_cast<DeclContext*>(const_cast<DeinitDecl*>(D));
   }

   static DeinitDecl *castFromDeclContext(DeclContext const *Ctx)
   {
      return static_cast<DeinitDecl*>(const_cast<DeclContext*>(Ctx));
   }
};

class FieldDecl: public VarDecl {
public:
   static FieldDecl *Create(ASTContext &C,
                            AccessModifier Access,
                            SourceLocation VarOrLetLoc,
                            SourceLocation ColonLoc,
                            IdentifierInfo *II,
                            SourceType Type,
                            bool IsStatic,
                            bool IsConst,
                            Expression* DefaultVal);

   void setDefaultVal(Expression *expr) { value = expr; }

   void addGetter(CompoundStmt* body = nullptr)
   {
      HasGetter = true;
      getterBody = body;
   }

   void addSetter(CompoundStmt* body = nullptr)
   {
      HasSetter = true;
      setterBody = body;
   }

   static bool classofKind(DeclKind kind) { return kind == FieldDeclID; }
   static bool classof(Decl const* T) { return classofKind(T->getKind()); }

private:
   FieldDecl(AccessModifier Access,
             SourceLocation VarOrLetLoc,
             SourceLocation ColonLoc,
             IdentifierInfo *II,
             SourceType Type,
             bool IsStatic,
             bool IsConst,
             Expression* DefaultVal);

   std::string linkageName;

   bool HasGetter = false;
   bool HasSetter = false;

   CompoundStmt* getterBody = nullptr;
   CompoundStmt* setterBody = nullptr;

   MethodDecl *getterMethod;
   MethodDecl *setterMethod;

   FuncArgDecl* newVal = nullptr;
   size_t globalOrdering = 0;

public:
   llvm::StringRef getLinkageName() const { return linkageName; }
   void setLinkageName(std::string &&name) { linkageName = move(name); }

   bool hasGetter() const { return HasGetter; }
   void hasGetter(bool hasGetter) { HasGetter = hasGetter; }

   bool hasSetter() const { return HasSetter; }
   void hasSetter(bool hasSetter) { HasSetter = hasSetter; }

   CompoundStmt* getGetterBody() const { return getterBody; }
   void setGetterBody(CompoundStmt* body) { getterBody = body; }

   CompoundStmt* getSetterBody() const { return setterBody; }
   void setSetterBody(CompoundStmt* body) { setterBody = body; }

   MethodDecl *getGetterMethod() const { return getterMethod; }
   void setGetterMethod(MethodDecl *method) { getterMethod = method; }

   MethodDecl *getSetterMethod() const { return setterMethod; }
   void setSetterMethod(MethodDecl *method) { setterMethod = method; }

   FuncArgDecl* getNewVal() const { return newVal; }
   void setNewVal(FuncArgDecl* Val) { newVal = Val; }

   Expression* getDefaultVal() const { return value; }

   size_t getGlobalOrdering() const { return globalOrdering; }
   void setGlobalOrdering(size_t globalOrder) { globalOrdering = globalOrder; }
};

class AssociatedTypeDecl: public NamedDecl {
public:
   static AssociatedTypeDecl *Create(ASTContext &C,
                                     SourceLocation Loc,
                                     IdentifierInfo *ProtoSpec,
                                     IdentifierInfo *Name,
                                     SourceType actualType);

   SourceLocation getSourceLoc() const { return Loc; }
   SourceRange getSourceRange() const { return SourceRange(Loc); }

   SourceType getActualType() const { return actualType; }
   void setActualType(SourceType ty) { actualType = ty; }

   IdentifierInfo *getProtoSpecInfo() const { return protocolSpecifier; }
   llvm::StringRef getProtocolSpecifier() const
   {
      return protocolSpecifier->getIdentifier();
   }

   ProtocolDecl *getProto() const { return Proto; }
   void setProto(ProtocolDecl *P) { Proto = P; }

   static bool classofKind(DeclKind kind){ return kind == AssociatedTypeDeclID;}
   static bool classof(Decl const* T) { return classofKind(T->getKind()); }

private:
   AssociatedTypeDecl(SourceLocation Loc,
                      IdentifierInfo *ProtoSpec,
                      IdentifierInfo *Name,
                      SourceType actualType);

   SourceLocation Loc;
   IdentifierInfo *protocolSpecifier;
   SourceType actualType;
   ProtocolDecl *Proto = nullptr;
};

class PropDecl: public NamedDecl {
   PropDecl(AccessModifier access,
            SourceLocation Loc,
            IdentifierInfo *propName,
            SourceType type,
            bool isStatic,
            bool hasDefinition,
            bool hasGetter,
            bool hasSetter,
            CompoundStmt* getter,
            CompoundStmt* setter,
            IdentifierInfo *newValName);

   SourceLocation Loc;
   SourceType type;

   CompoundStmt* getterBody = nullptr;
   CompoundStmt* setterBody = nullptr;

   bool HasGetter = false;
   bool HasSetter = false;

   IdentifierInfo *newValName;

   MethodDecl *getterMethod = nullptr;
   MethodDecl *setterMethod = nullptr;

   PropDecl *PropTemplate = nullptr;

public:
   static PropDecl *Create(ASTContext &C,
                           AccessModifier access,
                           SourceLocation Loc,
                           IdentifierInfo *propName,
                           SourceType type,
                           bool isStatic,
                           bool hasDefinition,
                           bool hasGetter,
                           bool hasSetter,
                           CompoundStmt* getter,
                           CompoundStmt* setter,
                           IdentifierInfo *newValName);

   static bool classof(Decl const* T) { return classofKind(T->getKind()); }
   static bool classofKind(DeclKind kind) { return kind == PropDeclID; }

   SourceLocation getSourceLoc() const { return Loc; }
   SourceRange getSourceRange() const { return SourceRange(Loc); }

   const SourceType &getType() const { return type; }

   bool hasGetter() const { return HasGetter; }
   bool hasSetter() const { return HasSetter; }

   CompoundStmt* getGetterBody() const { return getterBody; }
   CompoundStmt* getSetterBody() const { return setterBody; }

   void setGetterBody(CompoundStmt *B) { getterBody = B; }
   void setSetterBody(CompoundStmt *B) { setterBody = B; }

   IdentifierInfo *getNewValNameInfo() const { return newValName; }
   llvm::StringRef getNewValName() const { return newValName->getIdentifier(); }

   MethodDecl *getGetterMethod() const { return getterMethod; }
   void setGetterMethod(MethodDecl *M) { getterMethod = M; }

   MethodDecl *getSetterMethod() const { return setterMethod; }
   void setSetterMethod(MethodDecl *M) { setterMethod = M; }

   PropDecl *getPropTemplate() const { return PropTemplate; }
   void setPropTemplate(PropDecl *T) { PropTemplate = T; }
};

class EnumCaseDecl: public CallableDecl {
   EnumCaseDecl(AccessModifier AS,
                SourceLocation CaseLoc,
                SourceLocation IdentLoc,
                IdentifierInfo *II,
                StaticExpr* rawValue,
                std::vector<FuncArgDecl*>&& associatedTypes);

   SourceLocation CaseLoc;
   SourceLocation IdentLoc;
   StaticExpr* rawValExpr;
   long long rawValue = 0;

public:
   static EnumCaseDecl *Create(ASTContext &C,
                               AccessModifier AS,
                               SourceLocation CaseLoc,
                               SourceLocation IdentLoc,
                               IdentifierInfo *II,
                               StaticExpr* rawValue,
                               std::vector<FuncArgDecl*>&& associatedTypes);

   static bool classofKind(DeclKind kind) { return kind == EnumCaseDeclID; }
   static bool classof(Decl const* T) { return classofKind(T->getKind()); }

   SourceLocation getCaseLoc() const { return CaseLoc; }
   SourceLocation getIdentLoc() const { return IdentLoc; }
   SourceRange getSourceRange() const
   {
      return SourceRange(CaseLoc ? CaseLoc : IdentLoc, IdentLoc);
   }

   StaticExpr* getRawValExpr() const { return rawValExpr; }
   void setRawValExpr(StaticExpr *expr) { rawValExpr = expr; }

   long long getRawValue() const { return rawValue; }
   void setRawValue(long val) { rawValue = val; }

   std::vector<TemplateParamDecl*> &getTemplateParamsRef()
   {
      return templateParams;
   }
};

class StaticIfDecl: public Decl {
   StaticIfDecl(SourceLocation StaticLoc,
                SourceLocation RBRaceLoc,
                StaticExpr *condition,
                CompoundDecl *IfDecl,
                CompoundDecl *ElseDecl);

   SourceLocation StaticLoc;
   SourceLocation RBRaceLoc;

   StaticExpr *condition;
   CompoundDecl *IfDecl;
   CompoundDecl *ElseDecl;

public:
   static StaticIfDecl *Create(ASTContext &C,
                               SourceLocation StaticLoc,
                               SourceLocation RBRaceLoc,
                               StaticExpr *condition,
                               CompoundDecl *IfDecl,
                               CompoundDecl *ElseDecl);

   static bool classofKind(DeclKind kind) { return kind == StaticIfDeclID; }
   static bool classof(Decl const *T) { return classofKind(T->getKind()); }

   SourceRange getSourceRange() const
   {
      return SourceRange(StaticLoc, RBRaceLoc);
   }

   StaticExpr* getCondition() const { return condition; }
   void setCondition(StaticExpr *C) { condition = C; }

   CompoundDecl *getIfDecl() const { return IfDecl; }
   void setIfDecl(CompoundDecl *ID) { IfDecl = ID; }

   CompoundDecl *getElseDecl() const { return ElseDecl; }
   void setElseDecl(CompoundDecl *ED) { ElseDecl = ED; }

   SourceLocation getStaticLoc() const { return StaticLoc; }
   SourceLocation getRBRaceLoc() const { return RBRaceLoc; }
};

class StaticForDecl: public Decl {
   StaticForDecl(SourceLocation StaticLoc,
                 SourceLocation RBRaceLoc,
                 IdentifierInfo *elementName,
                 StaticExpr *range,
                 CompoundDecl *BodyDecl);

   SourceLocation StaticLoc;
   SourceLocation RBRaceLoc;

   IdentifierInfo *elementName;
   StaticExpr *range;
   CompoundDecl *BodyDecl;

public:
   static StaticForDecl *Create(ASTContext &C,
                                SourceLocation StaticLoc,
                                SourceLocation RBRaceLoc,
                                IdentifierInfo *elementName,
                                StaticExpr *range,
                                CompoundDecl *BodyDecl);

   static bool classofKind(DeclKind kind) { return kind == StaticForDeclID; }
   static bool classof(Decl const *T) { return classofKind(T->getKind()); }

   SourceRange getSourceRange() const
   {
      return SourceRange(StaticLoc, RBRaceLoc);
   }

   IdentifierInfo *getElementNameIdentifier() const { return elementName; }
   llvm::StringRef getElementName() const
   {
      return elementName->getIdentifier();
   }

   StaticExpr *getRange() const { return range; }
   void setRange(StaticExpr *S) { range = S; }

   CompoundDecl *getBodyDecl() const { return BodyDecl; }
   void setBodyDecl(CompoundDecl *D) { BodyDecl = D; }

   SourceLocation getStaticLoc() const { return StaticLoc; }
   SourceLocation getRBRaceLoc() const { return RBRaceLoc; }
};

class StaticAssertStmt: public Decl {
   StaticAssertStmt(SourceLocation Loc,
                    SourceRange Parens,
                    StaticExpr* expr,
                    std::string &&message);

   SourceLocation Loc;
   SourceRange Parens;
   StaticExpr* expr;
   std::string message;

public:
   static StaticAssertStmt *Create(ASTContext &C,
                                   SourceLocation Loc,
                                   SourceRange Parens,
                                   StaticExpr* expr,
                                   std::string &&message);

   static bool classofKind(DeclKind kind) { return kind == StaticAssertStmtID; }
   static bool classof(Decl const *T) { return classofKind(T->getKind()); }

   SourceLocation getStaticAssertLoc() const { return Loc; }
   SourceRange getParenRange() const { return Parens; }
   SourceRange getSourceRange() const
   {
      return SourceRange(Loc, Parens.getEnd());
   }

   StaticExpr* getExpr() const { return expr; }
   void setExpr(StaticExpr *E) { expr = E; }
   llvm::StringRef getMessage() const { return message; }
};

class StaticPrintStmt: public Decl {
   StaticPrintStmt(SourceLocation Loc,
                   SourceRange Parens,
                   StaticExpr* expr);

   SourceLocation Loc;
   SourceRange Parens;
   StaticExpr* expr;

public:
   static StaticPrintStmt *Create(ASTContext &C,
                                  SourceLocation Loc,
                                  SourceRange Parens,
                                  StaticExpr *E);

   static bool classofKind(DeclKind kind) { return kind == StaticPrintStmtID; }
   static bool classof(Decl const *T) { return classofKind(T->getKind()); }

   SourceLocation getStaticPrintLoc() const { return Loc; }
   SourceRange getParenRange() const { return Parens; }
   SourceRange getSourceRange() const
   {
      return SourceRange(Loc, Parens.getEnd());
   }

   StaticExpr* getExpr() const { return expr; }
   void setExpr(StaticExpr *E) { expr = E; }
};

class MixinDecl: public Decl {
public:
   static MixinDecl *Create(ASTContext &C,
                            SourceLocation Loc,
                            SourceRange Parens,
                            Expression *MixinExpr);

   static bool classofKind(DeclKind kind) { return kind == MixinDeclID; }
   static bool classof(Decl const *T) { return classofKind(T->getKind()); }

   Expression *getMixinExpr() const { return MixinExpr; }
   void setMixinExpr(Expression *E) { MixinExpr = E; }

   SourceLocation getMixinLoc() const { return Loc; }
   SourceRange getParenRange() const { return Parens; }
   SourceRange getSourceRange() const
   {
      return SourceRange(Loc, Parens.getEnd());
   }

private:
   MixinDecl(SourceLocation Loc,
             SourceRange Parens,
             Expression *MixinExpr);

   SourceLocation Loc;
   SourceRange Parens;
   Expression *MixinExpr;
};

} // namespace ast
} // namespace cdot

namespace llvm {

/// isa<T>(DeclContext*)
template<typename To>
struct isa_impl<To, ::cdot::ast::DeclContext> {
   static bool doit(const ::cdot::ast::DeclContext &Val)
   {
      return To::classofKind(Val.getDeclKind());
   }
};

/// cast<T>(DeclContext*)
template<class ToTy>
struct cast_convert_val<ToTy,
   const ::cdot::ast::DeclContext, const ::cdot::ast::DeclContext> {
   static const ToTy &doit(const ::cdot::ast::DeclContext &Val)
   {
      return *::cdot::ast::cast_convert_decl_context<ToTy>::doit(&Val);
   }
};

template<class ToTy>
struct cast_convert_val<ToTy, ::cdot::ast::DeclContext,
                        ::cdot::ast::DeclContext> {
   static ToTy &doit(::cdot::ast::DeclContext &Val)
   {
      return *::cdot::ast::cast_convert_decl_context<ToTy>::doit(&Val);
   }
};

template<class ToTy>
struct cast_convert_val<ToTy,
   const ::cdot::ast::DeclContext *, const ::cdot::ast::DeclContext *> {
   static const ToTy *doit(const ::cdot::ast::DeclContext *Val)
   {
      return ::cdot::ast::cast_convert_decl_context<ToTy>::doit(Val);
   }
};

template<class ToTy>
struct cast_convert_val<ToTy, ::cdot::ast::DeclContext *,
                        ::cdot::ast::DeclContext *> {
   static ToTy *doit(::cdot::ast::DeclContext *Val)
   {
      return ::cdot::ast::cast_convert_decl_context<ToTy>::doit(Val);
   }
};

template<class FromTy>
struct cast_convert_val< ::cdot::ast::DeclContext, FromTy, FromTy> {
   static ::cdot::ast::DeclContext &doit(const FromTy &Val) {
      return *FromTy::castToDeclContext(&Val);
   }
};

template<class FromTy>
struct cast_convert_val< ::cdot::ast::DeclContext, FromTy*, FromTy*> {
   static ::cdot::ast::DeclContext *doit(const FromTy *Val) {
      return FromTy::castToDeclContext(Val);
   }
};

template<class FromTy>
struct cast_convert_val< const ::cdot::ast::DeclContext, FromTy, FromTy> {
   static const ::cdot::ast::DeclContext &doit(const FromTy &Val) {
      return *FromTy::castToDeclContext(&Val);
   }
};

template<class FromTy>
struct cast_convert_val< const ::cdot::ast::DeclContext, FromTy*, FromTy*> {
   static const ::cdot::ast::DeclContext *doit(const FromTy *Val) {
      return FromTy::castToDeclContext(Val);
   }
};

} // namespace llvm

#endif //CDOT_NAMEDDECL_H
