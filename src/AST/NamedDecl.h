//
// Created by Jonas Zell on 22.01.18.
//

#ifndef CDOT_NAMEDDECL_H
#define CDOT_NAMEDDECL_H

#include "AST/Attribute/Attr.h"
#include "AST/SourceType.h"
#include "AST/Passes/SemanticAnalysis/Template.h"
#include "Basic/Precedence.h"
#include "Support/Casting.h"

#include <llvm/Support/TrailingObjects.h>
#include <llvm/ADT/PointerUnion.h>
#include <llvm/ADT/SmallPtrSet.h>

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

#  define CDOT_DECL(Name)           \
      Name##ID,
#  include "AST/Decl.def"
   };

   enum DeclFlags : unsigned {
      DF_None                = 0u,
      DF_TypeDependent       = 1u,
      DF_ValueDependent      = DF_TypeDependent << 1u,
      DF_IsInvalid           = DF_ValueDependent << 1u,
      DF_CtfeDependent       = DF_IsInvalid << 1u,
      DF_SemanticallyChecked = DF_CtfeDependent << 1u,
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

   bool isStatic() const
   {
      return declFlagSet(DF_Static);
   }

   bool isConst() const
   {
      return declFlagSet(DF_Const);
   }

   bool hasDefinition() const
   {
      return declFlagSet(DF_HasDefinition);
   }

   void setHasDefinition(bool def)
   {
      setDeclFlag(DF_HasDefinition, def);
   }

   bool isExternal() const
   {
      return declFlagSet(DF_External);
   }

   void setExternal(bool ext)
   {
      setDeclFlag(DF_External, ext);
   }

   bool isDependent() const
   {
      return isTypeDependent() || isValueDependent() || isCtfeDependent();
   }

   bool isTypeDependent() const
   {
      return declFlagSet(DF_TypeDependent);
   }

   bool isValueDependent() const
   {
      return declFlagSet(DF_ValueDependent);
   }

   void setIsTypeDependent(bool typeDependant)
   {
      setDeclFlag(DF_TypeDependent, typeDependant);
   }

   void setIsValueDependent(bool valueDependant)
   {
      setDeclFlag(DF_ValueDependent, valueDependant);
   }

   bool isCtfeDependent() const { return declFlagSet(DF_CtfeDependent); }
   void setIsCtfeDependent(bool dep = true)
   {
      setDeclFlag(DF_CtfeDependent, dep);
   }

   bool isInvalid() const
   {
      return declFlagSet(DF_IsInvalid);
   }

   void setIsInvalid(bool error)
   {
      setDeclFlag(DF_IsInvalid, error);
   }

   bool isSemanticallyChecked() const
   {
      return declFlagSet(DF_SemanticallyChecked);
   }

   void setSemanticallyChecked(bool chk = true)
   {
      setDeclFlag(DF_SemanticallyChecked, chk);
   }

   bool wasDeclared() const
   {
      return declFlagSet(DF_WasDeclared);
   }

   void setDeclared(bool dec = true)
   {
      setDeclFlag(DF_WasDeclared, dec);
   }

   bool checkedAttrs() const { return declFlagSet(DF_CheckedAttrs); }
   void setCheckedAttrs(bool C) { setDeclFlag(DF_CheckedAttrs, C); }

   bool isSynthesized() const { return declFlagSet(DF_Synthesized); }
   void setSynthesized(bool synth = true)
   {
      setDeclFlag(DF_Synthesized, synth);
   }

   bool alreadyCheckedOrIsInvalid() const
   {
      static uint32_t mask = DF_IsInvalid | DF_SemanticallyChecked;
      return (flags & mask) != 0;
   }

   void copyStatusFlags(Decl *D);
   void copyStatusFlags(Statement *D);

   bool declFlagSet(DeclFlags f) const
   {
      return (flags & f) != 0;
   }

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
   void addAttribute(Attr *A) const;

   ASTContext &getASTCtx() const;

   SourceLocation getSourceLoc() const { return sourceLoc; }
   void setSourceLoc(SourceLocation loc) { sourceLoc = loc; }

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
   explicit Decl(DeclKind kind, unsigned flags = DF_None)
      : kind(kind), flags(flags)
   {}

#ifndef NDEBUG
   virtual
#endif
   ~Decl();

   DeclKind kind  : 8;
   unsigned flags : 24;

   SourceLocation sourceLoc;

   Decl *nextDeclInContext = nullptr;
   DeclContext *declContext = nullptr;
};

class NamedDecl: public Decl {
public:
   AccessModifier getAccess() const
   {
      return access;
   }

   void setAccess(AccessModifier access)
   {
      NamedDecl::access = access;
   }

   llvm::StringRef getName() const
   {
      return name;
   }

   void setName(std::string &&name)
   {
      NamedDecl::name = move(name);
   }

   bool isOverloadable() const;

   const std::vector<StaticExpr *> &getConstraints() const
   {
      return constraints;
   }

   void setConstraints(std::vector<StaticExpr *> &&constraints)
   {
      NamedDecl::constraints = move(constraints);
   }

   bool isTemplate() const;
   bool isTemplateOrInTemplate() const;

   llvm::ArrayRef<TemplateParamDecl*> getTemplateParams() const;

   bool isInstantiation() const;
   const sema::TemplateArgList &getTemplateArgs() const;
   NamedDecl *getSpecializedTemplate() const;
   SourceLocation getInstantiatedFrom() const;
   NamedDecl *getInstantiatedWithin() const;

   llvm::StringRef getNameWithoutNamespace() const;

   std::string getFullName() const { return getJoinedName('.'); }
   std::string getJoinedName(char join, bool includeFile = false) const;

   size_t getSpecifierForDiagnostic();

   bool isExported() const;

   unsigned int getFlags() const
   {
      return flags;
   }

   void setFlags(unsigned int flags)
   {
      NamedDecl::flags = flags;
   }

   ExternKind getExternKind() const
   {
      return externKind;
   }

   void setExternKind(ExternKind externKind)
   {
      NamedDecl::externKind = externKind;
   }

   static bool classof(const Decl *T) { return classofKind(T->getKind()); }
   static bool classofKind(DeclKind kind)
   {
      switch (kind) {
#     define CDOT_NAMED_DECL(Name)      \
         case Name##ID:
#     include "AST/Decl.def"
            return true;
         default:
            return false;
      }
   }

protected:
   NamedDecl(DeclKind typeID,
             AccessModifier access,
             std::string &&ident,
             std::vector<StaticExpr*> &&constraints = {})
      : Decl(typeID), access(access),
        name(move(ident)), constraints(move(constraints))
   {}

   ExternKind externKind = ExternKind::None;
   AccessModifier access;
   std::string name;
   std::vector<StaticExpr*> constraints;
};

class TemplateParamDecl: public NamedDecl {
public:
   TemplateParamDecl(std::string &&name,
                     SourceType covariance,
                     SourceType contravariance,
                     bool variadic,
                     Expression *defaultValue)
      : NamedDecl(TemplateParamDeclID, (AccessModifier)0, move(name)),
        covariance(covariance), contravariance(contravariance),
        typeName(true), variadic(variadic), defaultValue(defaultValue)
   {}

   TemplateParamDecl(std::string &&name,
                     SourceType valueType,
                     bool variadic,
                     Expression *defaultValue)
      : NamedDecl(TemplateParamDeclID, (AccessModifier)0, move(name)),
        covariance(valueType), contravariance(nullptr),
        typeName(false), variadic(variadic),
        defaultValue(defaultValue)
   {}

   static bool classof(Decl const* T)
   {
      return classofKind(T->getKind());
   }

   static bool classofKind(DeclKind kind)
   {
      return kind == TemplateParamDeclID;
   }


private:
   SourceType covariance;
   SourceType contravariance;

   bool typeName : 1;
   bool variadic : 1;

   Expression *defaultValue;

public:
   const SourceType &getCovariance() const { return covariance; }
   const SourceType &getValueType() const { return covariance; }

   void setCovariance(SourceType cov) { covariance = cov; }

   const SourceType &getContravariance() const { return contravariance; }
   void setContravariance(SourceType con) { contravariance = con; }

   Expression *getDefaultValue() const { return defaultValue; }
   void setDefaultValue(Expression *val) { defaultValue = val; }

   bool isTypeName() const { return typeName; }
   bool isVariadic() const { return variadic; }
};


class VarDecl: public NamedDecl {
public:
   static bool classof(Decl const* T) { return classofKind(T->getKind()); }
   static bool classofKind(DeclKind kind)
   {
      switch (kind) {
      case LocalVarDeclID:
      case GlobalVarDeclID:
      case FuncArgDeclID:
         return true;
      default:
         return false;
      }
   }

protected:
   VarDecl(DeclKind id,
           AccessModifier access,
           bool isConst,
           std::string &&ident,
           SourceType type,
           Expression* value)
      : NamedDecl(id, access, move(ident)), type(type),
        value(value), CanElideCopy(false)
   {
      setDeclFlag(DeclFlags::DF_Const, isConst);
   }

   SourceType type;
   Expression* value = nullptr;

   bool CanElideCopy : 1;

public:
   const SourceType &getTypeRef() const { return type; }
   void setType(SourceType ty) { type = ty; }

   Expression* getValue() const { return value; }
   void setValue(Expression *V) { value = V; }

   bool canElideCopy() const { return CanElideCopy; }
   void setCanElideCopy(bool CanElide) { CanElideCopy = CanElide; }
};

class LocalVarDecl: public VarDecl {
public:
   LocalVarDecl(AccessModifier access,
                bool isConst,
                std::string &&ident,
                SourceType type,
                Expression* value)
      : VarDecl(LocalVarDeclID, access, isConst, move(ident), type, value),
        IsNRVOCand(false)
   {}

   bool isNRVOCandidate() const { return IsNRVOCand; }
   void setIsNRVOCandidate(bool NRVO) { IsNRVOCand = NRVO; }

   static bool classof(Decl const* T) { return classofKind(T->getKind()); }
   static bool classofKind(DeclKind kind) { return kind == LocalVarDeclID; }

private:
   bool IsNRVOCand   : 1;
};

class GlobalVarDecl: public VarDecl {
public:
   GlobalVarDecl(AccessModifier access,
                 bool isConst,
                 std::string &&ident,
                 SourceType type,
                 Expression* value)
      : VarDecl(GlobalVarDeclID, access, isConst, move(ident), type, value)
   {}

   static bool classof(Decl const* T) { return classofKind(T->getKind()); }
   static bool classofKind(DeclKind kind) { return kind == GlobalVarDeclID; }

protected:
   size_t globalOrdering = 0;

public:
   size_t getGlobalOrdering() const { return globalOrdering; }
   void setGlobalOrdering(size_t Order) { globalOrdering = Order; }
};

class FuncArgDecl: public VarDecl {
public:
   FuncArgDecl(std::string &&argName,
               SourceType argType,
               Expression* defaultValue,
               bool variadicArgPackExpansion,
               bool isConst,
               bool cstyleVararg = false)
      : VarDecl(FuncArgDeclID, AccessModifier::PUBLIC, isConst, move(argName),
                argType, defaultValue),
        VariadicArgPackExpansion(variadicArgPackExpansion),
        IsConst(isConst), Vararg(cstyleVararg), CstyleVararg(cstyleVararg)
   {}

   static bool classofKind(DeclKind kind) { return kind == FuncArgDeclID; }
   static bool classof(Decl const* T) { return classofKind(T->getKind()); }

protected:
   bool VariadicArgPackExpansion : 1;
   bool IsConst : 1;
   bool Vararg : 1;
   bool CstyleVararg : 1;

public:
   llvm::StringRef getArgName() const { return getName(); }
   const SourceType &getArgType() const { return getTypeRef(); }

   Expression* getDefaultVal() const { return value; }
   void setDefaultVal(Expression *defaultVal) { value = defaultVal; }

   bool isVariadicArgPackExpansion() const { return VariadicArgPackExpansion; }
   bool isConst() const { return IsConst; }
   bool isVararg() const { return Vararg; }
   bool isCstyleVararg() const { return CstyleVararg; }
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

   void addDecl(Decl *decl)
   {
      decl->setDeclContext(this);
      if (auto Ctx = support::dyn_cast<DeclContext>(decl))
         Ctx->setParentCtx(this);

      if (!firstDecl)
         firstDecl = decl;

      if (lastAddedDecl) {
         assert(!lastAddedDecl->getNextDeclInContext());
         lastAddedDecl->setNextDeclInContext(decl);
      }

      lastAddedDecl = decl;
   }

   [[nodiscard]]
   AddDeclResultKind addDecl(NamedDecl *decl)
   {
      return addDecl(decl->getName(), decl);
   }

   [[nodiscard]]
   AddDeclResultKind addDecl(llvm::StringRef key, NamedDecl *decl)
   {
      if (!key.empty()) {
         auto it = namedDecls.find(key);
         if (it == namedDecls.end()) {
            namedDecls.try_emplace(key, decl);
         }
         else {
            auto lookup = it->getValue().getAsLookupResult();
            assert(!lookup.empty());

            if (lookup.front()->getKind() != decl->getKind())
               return ADR_DuplicateDifferentKind;

            if (!lookup.front()->isOverloadable())
               return ADR_Duplicate;

            it->getValue().appendDecl(decl);
         }
      }

      if (isTransparent())
         parentCtx->makeDeclAvailable(key, decl);

      addDecl((Decl*)decl);
      return ADR_Success;
   }

   void addTransparentDecls();

   LookupResult lookupOwn(llvm::StringRef name) const
   {
      auto it = namedDecls.find(name);
      if (it == namedDecls.end())
         return {};

      return it->getValue().getAsLookupResult();
   }

   template<class T>
   T *lookupOwn(llvm::StringRef name) const
   {
      return support::dyn_cast_or_null<T>(lookupSingle(name));
   }

   LookupResult lookup(llvm::StringRef name) const
   {
      auto res = lookupOwn(name);
      if (res)
         return res;

      if (auto P = parentCtx) {
         res = P->lookup(name);
         if (res)
            return res;
      }

      return {};
   }

   NamedDecl *lookupSingle(llvm::StringRef name) const
   {
      auto res = lookup(name);
      if (res.size() != 1)
         return nullptr;

      return res.front();
   }

   bool hasAnyDeclNamed(llvm::StringRef name) const
   {
      return namedDecls.find(name) != namedDecls.end();
   }

   template<class T>
   T* lookupSingle(llvm::StringRef name) const
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

   void makeAllDeclsAvailable(DeclContext *Ctx)
   {
      for (auto D : Ctx->getDecls())
         if (auto ND = support::dyn_cast<NamedDecl>(D))
            makeDeclAvailable(ND);
   }

   void makeDeclAvailable(NamedDecl *decl)
   {
      return makeDeclAvailable(decl->getName(), decl);
   }

   void makeDeclAvailable(llvm::StringRef key, NamedDecl *decl)
   {
      auto it = namedDecls.find(key);
      if (it == namedDecls.end()) {
         namedDecls.try_emplace(key, decl);
      }
      else {
         it->getValue().appendDecl(decl);
      }
   }

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

   Decl::DeclKind declKind;

   llvm::StringMap<DeclList> namedDecls;

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
public:
   GlobalDeclContext();

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
                                  llvm::StringRef fileName,
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
                   std::string &&fileName,
                   size_t sourceId,
                   llvm::ArrayRef<ImportStmt*> imports);

   ASTContext &ASTCtx;
   size_t sourceId;
   unsigned numImports;

public:
   const std::string &getFileName() const
   {
      return name;
   }

   size_t getSourceId() const
   {
      return sourceId;
   }

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
public:
   NamespaceDecl(std::string &&nsName,
                 bool isAnonymous)
      : NamedDecl(NamespaceDeclID, (AccessModifier)0, move(nsName)),
        DeclContext(NamespaceDeclID),
        anonymousNamespace(isAnonymous)
   {

   }

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

protected:
   size_t namespaceId = size_t(-1);
   bool anonymousNamespace = false;

   NamespaceDecl *outerNamespace = nullptr;

public:
   bool isAnonymousNamespace() const
   {
      return anonymousNamespace;
   }

   size_t getNamespaceId() const
   {
      return namespaceId;
   }

   void setNamespaceId(size_t namespaceId)
   {
      NamespaceDecl::namespaceId = namespaceId;
   }

   NamespaceDecl *getOuterNamespace() const
   {
      return outerNamespace;
   }

   void setOuterNamespace(NamespaceDecl *outerNamespace)
   {
      this->outerNamespace = outerNamespace;
      if (outerNamespace)
         anonymousNamespace |= outerNamespace->isAnonymousNamespace();
   }
};

class CompoundDecl: public Decl, public DeclContext {
public:
   CompoundDecl(SourceLocation LBraceLoc,
                bool Transparent)
      : Decl(CompoundDeclID), DeclContext(CompoundDeclID),
        LBraceLoc(LBraceLoc), Transparent(Transparent)
   {}

   static bool classofKind(DeclKind kind) { return kind == CompoundDeclID; }
   static bool classof(Decl const *T) { return classofKind(T->getKind()); }

   SourceLocation getLBraceLoc() const { return LBraceLoc; }
   SourceLocation getRBraceLoc() const { return RBraceLoc; }
   void setRBraceLoc(SourceLocation loc) { RBraceLoc = loc;
   }
   bool isTransparent() const { return Transparent; }

private:
   SourceLocation LBraceLoc;
   SourceLocation RBraceLoc;

   bool Transparent      : 1;
};


class CallableDecl: public NamedDecl, public DeclContext,
                    public llvm::FoldingSetNode {
public:
   static bool classof(Decl const* T) { return classofKind(T->getKind()); }
   static bool classofKind(DeclKind kind)
   {
      switch (kind) {
      case FunctionDeclID:
      case MethodDeclID:
      case InitDeclID:
      case DeinitDeclID:
      case EnumCaseDeclID:
         return true;
      default:
         return false;
      }
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
                std::string &&name,
                SourceType returnType,
                std::vector<FuncArgDecl* > &&args,
                std::vector<StaticExpr* > &&Constraints,
                Statement* body,
                OperatorInfo op);

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
      NoThrow  = 1,
      ExternC  = NoThrow << 1,
      Native   = ExternC << 1,
      ConvOp   = Native  << 1,
      External = ConvOp << 1,
      Main     = External << 1,
      Vararg   = Main << 1,
      CVarArg  = Vararg << 1,
      Defined  = CVarArg << 1,

      // method flags
      Abstract         = Defined << 1,
      Alias            = Abstract << 1,
      MutableSelf      = Alias << 1,
      ProtoMethod      = MutableSelf << 1,
      Virtual          = ProtoMethod << 1,
      Override         = Virtual << 1,
      Property         = Override << 1,
      MemberwiseInit   = Property << 1,
      DefaultInit      = MemberwiseInit << 1,
      ProtoDefaultImpl = DefaultInit << 1,
   };

   uint32_t Flags;

   void setFlag(Flag F, bool val)
   {
      if (val)
         Flags |= F;
      else
         Flags &= ~F;
   }

   bool getFlag(Flag F) const
   {
      return (Flags & F) != 0;
   }

public:
   FunctionType *getFunctionType() const
   {
      return functionType;
   }

   void createFunctionType(SemaPass &SP,
                           unsigned flags = 0,
                           bool lambda = false);

   bool isKnownFunction();
   void checkKnownFnKind();
   KnownFunction getKnownFnKind();

   std::vector<FuncArgDecl* > &getArgs()
   {
      return args;
   }

   std::vector<FuncArgDecl* > const& getArgs() const
   {
      return args;
   }

   void setArgs(std::vector<FuncArgDecl* > &&args)
   {
      CallableDecl::args = move(args);
   }

   void setLinkageName(std::string &&linkageName)
   {
      CallableDecl::linkageName = move(linkageName);
   }

   void setBody(Statement* body)
   {
      setHasDefinition(body != nullptr);
      this->body = body;
   }

   const SourceType &getReturnType() const { return returnType; }
   void setReturnType(SourceType RetTy) { returnType = RetTy; }

   llvm::StringRef getLinkageName() const { return linkageName; }

   Statement* getBody() const { return body; }

   size_t getMethodID() const
   {
      return methodID;
   }

   void setMethodID(size_t methodID)
   {
      CallableDecl::methodID = methodID;
   }

   LocalVarDecl *getNRVOCandidate() const
   {
      return NRVOCandidate;
   }

   void setNRVOCandidate(LocalVarDecl *NRVOCandidate)
   {
      CallableDecl::NRVOCandidate = NRVOCandidate;
   }

   OperatorInfo &getOperator()
   {
      return op;
   }

   OperatorInfo const& getOperator() const
   {
      return op;
   }

   bool isOperator() const
   {
      return op.getPrecedenceGroup().isValid();
   }

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

   uint32_t getFunctionFlags() const { return Flags; }
   void setFunctionFlags(uint32_t Flags) { CallableDecl::Flags = Flags; }

   bool isNonStaticMethod() const;

   bool willHaveDefinition() const;

   llvm::SmallPtrSetImpl<CallableDecl*> &getCalledFunctions()
   {
      return CalledFunctions;
   }

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

   void setTemplateParams(std::vector<TemplateParamDecl *> &&templateParams)
   {
      CallableDecl::templateParams = move(templateParams);
   }

   llvm::ArrayRef<TemplateParamDecl *> getTemplateParams() const
   {
      return templateParams;
   }

   bool isInstantiation() const
   {
      return instantiationInfo != nullptr;
   }

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

   llvm::StringRef getNameWithoutFix() const;
};

class CompoundStmt;

class FunctionDecl : public CallableDecl {
public:
   FunctionDecl(AccessModifier am,
                std::string &&funcName,
                std::vector<FuncArgDecl* > &&args,
                SourceType returnType,
                std::vector<StaticExpr* > &&Constraints,
                Statement* body,
                OperatorInfo op)
      : CallableDecl(FunctionDeclID, am, move(funcName), returnType,
                     move(args), move(Constraints), body, op)
   {

   }

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
public:
   TypedefDecl(AccessModifier access,
               std::string&& name,
               SourceType origin)
      : NamedDecl(TypedefDeclID, access, move(name), {}),
        origin(origin)
   {}

   static bool classofKind(DeclKind kind)
   {
      return kind == TypedefDeclID;
   }

   static bool classof(Decl const* T)
   {
      return classofKind(T->getKind());
   }

protected:
   SourceType origin;
   std::vector<TemplateParamDecl*> templateParams;

public:
   const SourceType &getOriginTy() const
   {
      return origin;
   }

   void setTemplateParams(std::vector<TemplateParamDecl *> &&templateParams)
   {
      TypedefDecl::templateParams = move(templateParams);
   }

   llvm::ArrayRef<TemplateParamDecl *> getTemplateParams() const
   {
      return templateParams;
   }
};

class AliasDecl: public NamedDecl,
                 public DeclContext,
                 public llvm::FoldingSetNode {
public:
   AliasDecl(std::string &&name,
             std::vector<StaticExpr* > &&constraints,
             StaticExpr* aliasExpr)
      : NamedDecl(AliasDeclID, (AccessModifier)0, move(name),
                  move(constraints)),
        DeclContext(AliasDeclID),
        aliasExpr(aliasExpr)
   { }

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

private:
   StaticExpr* aliasExpr;

   std::vector<TemplateParamDecl*> templateParams;
   InstantiationInfo<AliasDecl> *instantiationInfo = nullptr;

public:
   StaticExpr* getAliasExpr()
   {
      return aliasExpr;
   }

   void setAliasExpr(StaticExpr *aliasExpr)
   {
      AliasDecl::aliasExpr = aliasExpr;
   }

   StaticExpr* const& getAliasExpr() const
   {
      return aliasExpr;
   }

   const Variant &getVal() const
   {
      return aliasExpr->getEvaluatedExpr();
   }

   void setTemplateParams(std::vector<TemplateParamDecl *> &&templateParams)
   {
      AliasDecl::templateParams = move(templateParams);
   }

   llvm::ArrayRef<TemplateParamDecl *> getTemplateParams() const
   {
      return templateParams;
   }

   bool isInstantiation() const
   {
      return instantiationInfo != nullptr;
   }

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
      switch (kind) {
      case StructDeclID:
      case ClassDeclID:
      case EnumDeclID:
      case UnionDeclID:
      case ProtocolDeclID:
      case ExtensionDeclID:
         return true;
      default:
         return false;
      }
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


   AssociatedTypeDecl* getAssociatedType(llvm::StringRef name,
                                         ProtocolDecl *P = nullptr) const;

   MethodDecl *getConversionOperator(QualType toType) const;
   MethodDecl *getComparisonOperator(QualType withType) const;

   bool hasMethodWithName(llvm::StringRef name) const;
   bool hasMethodTemplate(llvm::StringRef name) const;

   PropDecl *getProperty(llvm::StringRef name) const;
   FieldDecl* getField(llvm::StringRef name) const;

   MethodDecl* getMethod(llvm::StringRef name, bool checkParent = true) const;
   MethodDecl* getMethod(size_t id) const;

   MethodDecl* getOwnMethod(llvm::StringRef name);

   RecordDecl *getInnerRecord(llvm::StringRef name) const
   {
      return lookupSingle<RecordDecl>(name);
   }

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
              std::string &&recordName,
              std::vector<SourceType> &&conformanceTypes,
              std::vector<StaticExpr*> &&constraints)
      : NamedDecl(typeID, access, move(recordName), move(constraints)),
        DeclContext(typeID),
        conformanceTypes(move(conformanceTypes)),
        manualAlignment(false), opaque(false),
        implicitlyEquatable(false), implicitlyHashable(false),
        implicitlyStringRepresentable(false)
   {}

   static size_t lastRecordID;

   size_t lastMethodID = 1;
   size_t uses = 0;
   size_t recordID;

   std::vector<SourceType> conformanceTypes;

   std::vector<TemplateParamDecl*> templateParams;
   InstantiationInfo<RecordDecl> *instantiationInfo = nullptr;

   llvm::SmallPtrSet<ProtocolDecl*, 4> conformances;
   llvm::SmallPtrSet<RecordDecl*, 4> innerRecords;
   RecordDecl *outerRecord = nullptr;

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

   void addConformance(ProtocolDecl *P)
   {
      conformances.insert(P);
   }

   RecordDecl *getOuterRecord() const
   {
      return outerRecord;
   }

   void setOuterRecord(RecordDecl *outerRecord)
   {
      RecordDecl::outerRecord = outerRecord;
   }

   const llvm::SmallPtrSet<RecordDecl *, 4> &getInnerRecords() const
   {
      return innerRecords;
   }

   void addInnerRecord(RecordDecl *R);

   DeinitDecl *getDeinitializer() const
   {
      return deinitializer;
   }

   const std::unordered_multimap<size_t, MethodDecl*> &
   getDestructuringOperators() const
   {
      return destructuringOperators;
   }

   bool isTemplate() const
   {
      if (NamedDecl::isTemplate())
         return true;

      return outerRecord && outerRecord->isTemplate();
   }

   void setInstantiationInfo(InstantiationInfo<RecordDecl> *instantiationInfo)
   {
      RecordDecl::instantiationInfo = instantiationInfo;
   }

   void setTemplateParams(std::vector<TemplateParamDecl *> &&templateParams)
   {
      RecordDecl::templateParams = move(templateParams);
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

   size_t getUses() const
   {
      return uses;
   }

   size_t getRecordID() const
   {
      return recordID;
   }

   unsigned getSize() const { return occupiedBytes; }
   unsigned short getAlignment() const { return alignment; }

   void setSize(unsigned s) { occupiedBytes = s; }
   void setAlignment(unsigned short al) { alignment = al; }

   bool hasManualAlignment() const { return manualAlignment; }

   bool isOpaque() const
   {
      return opaque;
   }

   void setOpaque(bool opaque)
   {
      RecordDecl::opaque = opaque;
   }

   MethodDecl *getOperatorEquals() const
   {
      return operatorEquals;
   }

   void setOperatorEquals(MethodDecl *operatorEquals)
   {
      RecordDecl::operatorEquals = operatorEquals;
   }

   MethodDecl *getHashCodeFn() const
   {
      return hashCodeFn;
   }

   void setHashCodeFn(MethodDecl *hashCodeFn)
   {
      RecordDecl::hashCodeFn = hashCodeFn;
   }

   MethodDecl *getToStringFn() const
   {
      return toStringFn;
   }

   void setToStringFn(MethodDecl *toStringFn)
   {
      RecordDecl::toStringFn = toStringFn;
   }

   llvm::StringRef getOwnName() const;

   int getNameSelector() const;

   bool isImplicitlyEquatable() const
   {
      return implicitlyEquatable;
   }

   void setImplicitlyEquatable(bool implicitlyEquatable)
   {
      RecordDecl::implicitlyEquatable = implicitlyEquatable;
   }

   bool isImplicitlyHashable() const
   {
      return implicitlyHashable;
   }

   void setImplicitlyHashable(bool implicitlyHashable)
   {
      RecordDecl::implicitlyHashable = implicitlyHashable;
   }

   bool isImplicitlyStringRepresentable() const
   {
      return implicitlyStringRepresentable;
   }

   void setImplicitlyStringRepresentable(bool implicitlyStringRepresentable)
   {
      RecordDecl::implicitlyStringRepresentable = implicitlyStringRepresentable;
   }
};

struct FunctionTemplateInstantiation;

class StructDecl: public RecordDecl {
public:
   StructDecl(AccessModifier access,
              std::string &&recordName,
              std::vector<SourceType> &&conformanceTypes,
              std::vector<StaticExpr*> &&constraints)
      : RecordDecl(StructDeclID, access, move(recordName),
                   move(conformanceTypes), move(constraints))
   {}

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

   InitDecl *getDefaultInitializer() const
   {
      return defaultInitializer;
   }

   void setDefaultInitializer(InitDecl *defaultInitializer)
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

protected:
   StructDecl(DeclKind typeID,
              AccessModifier access,
              std::string &&recordName,
              std::vector<SourceType> &&conformanceTypes,
              std::vector<StaticExpr*> &&constraints)
      : RecordDecl(typeID, access, move(recordName),
                   move(conformanceTypes), move(constraints))
   {}

   std::vector<FieldDecl*> fields;

   InitDecl* parameterlessConstructor = nullptr;
   InitDecl* memberwiseInitializer    = nullptr;
   InitDecl* defaultInitializer       = nullptr;
};

class ClassDecl: public StructDecl {
public:
   ClassDecl(AccessModifier access,
             std::string &&recordName,
             std::vector<SourceType> &&conformanceTypes,
             std::vector<StaticExpr*> &&constraints,
             SourceType parentClass,
             bool isAbstract)
      : StructDecl(ClassDeclID, access, move(recordName),
                   move(conformanceTypes), move(constraints)),
        parentType(parentClass), IsAbstract(isAbstract)
   {}

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

protected:
   SourceType parentType;
   ClassDecl *parentClass = nullptr;

   bool IsAbstract = false;

public:
   bool isAbstract() const { return IsAbstract; }
};

class EnumCaseDecl;

class EnumDecl: public RecordDecl {
public:
   EnumDecl(AccessModifier access,
            std::string &&recordName,
            std::vector<SourceType> &&conformanceTypes,
            std::vector<StaticExpr*> &&constraints,
            SourceType rawType)
      : RecordDecl(EnumDeclID, access, move(recordName),
                   move(conformanceTypes), move(constraints)),
        rawType(rawType)
   {}

   EnumCaseDecl *hasCase(llvm::StringRef name);

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

protected:
   SourceType rawType;
   size_t maxAssociatedTypes = 0;
   std::vector<EnumCaseDecl*> cases;

public:
   const SourceType &getRawType() const { return rawType; }
   void setRawType(SourceType ty) { rawType = ty; }

   size_t getMaxAssociatedTypes() const
   {
      return maxAssociatedTypes;
   }

   llvm::ArrayRef<EnumCaseDecl *> getCases() const
   {
      return cases;
   }
};

class UnionDecl: public RecordDecl {
public:
   UnionDecl(AccessModifier access,
             std::string &&recordName,
             std::vector<SourceType> &&conformanceTypes,
             std::vector<StaticExpr*> &&constraints)
      : RecordDecl(UnionDeclID, access, move(recordName),
                   move(conformanceTypes), move(constraints))
   {}

   bool isConst() const { return is_const; }
   void isConst(bool is_const) { this->is_const = is_const; }

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


protected:
   bool is_const = false;
};

class ProtocolDecl: public RecordDecl {
public:
   ProtocolDecl(AccessModifier access,
                std::string &&recordName,
                std::vector<SourceType> &&conformanceTypes,
                std::vector<StaticExpr*> &&constraints)
      : RecordDecl(ProtocolDeclID, access, move(recordName),
                   move(conformanceTypes), move(constraints))
   {}

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

   void clearTemplateParams()
   {
      templateParams.clear();
   }

};

class ExtensionDecl: public RecordDecl {
public:
   ExtensionDecl(AccessModifier access,
                 std::string &&recordName,
                 std::vector<SourceType> &&conformanceTypes,
                 std::vector<StaticExpr*> &&constraints)
      : RecordDecl(ExtensionDeclID, access, move(recordName),
                   move(conformanceTypes), move(constraints))
   {}

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
};

class MethodDecl: public CallableDecl {
public:
   MethodDecl(std::string &&methodName,
              SourceType returnType,
              std::vector<FuncArgDecl* > &&args,
              std::vector<StaticExpr* > &&Constraints,
              Statement* body,
              AccessModifier access,
              bool isStatic = false)
      : CallableDecl(MethodDeclID, access, std::move(methodName), returnType,
                     std::move(args), move(Constraints),
                     body, {})
   {
      setFlag(Abstract, !body);
      setDeclFlag(DF_Static, isStatic);
   }

   MethodDecl(std::string &&methodName,
              SourceType returnType,
              std::vector<FuncArgDecl* > &&args,
              std::vector<StaticExpr* > &&Constraints,
              Statement* body,
              OperatorInfo op,
              bool isCastOp,
              AccessModifier access,
              bool isStatic)
      : CallableDecl(MethodDeclID, access, std::move(methodName), returnType,
                     std::move(args), move(Constraints),
                     body, op)
   {
      setFlag(Abstract, !body);
      setFlag(ConvOp, isCastOp);
      setDeclFlag(DF_Static, isStatic);
   }

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

protected:
   MethodDecl(DeclKind typeID,
              std::string &&methodName,
              SourceType returnType,
              std::vector<FuncArgDecl* > &&args,
              std::vector<StaticExpr* > &&Constraints,
              Statement* body,
              AccessModifier access,
              bool isStatic = false)
      : CallableDecl(typeID, access, std::move(methodName), returnType,
                     std::move(args), move(Constraints),
                     body, {})
   {
      setFlag(Abstract, !body);
      setDeclFlag(DF_Static, isStatic);
   }

   size_t methodID;
   size_t protocolTableOffset = 0;

   SourceLocation BodyInstantiationLoc;
   Statement *BodyTemplate = nullptr;

public:
   size_t getMethodID() const
   {
      return methodID;
   }

   size_t getProtocolTableOffset() const
   {
      return protocolTableOffset;
   }

   void setProtocolTableOffset(size_t protocolTableOffset)
   {
      MethodDecl::protocolTableOffset = protocolTableOffset;
   }

   const SourceLocation &getBodyInstantiationLoc() const
   {
      return BodyInstantiationLoc;
   }

   void setBodyInstantiationLoc(const SourceLocation &BodyInstantiationLoc)
   {
      MethodDecl::BodyInstantiationLoc = BodyInstantiationLoc;
   }

   Statement *getBodyTemplate() const
   {
      return BodyTemplate;
   }

   void setBodyTemplate(Statement *BodyTemplate)
   {
      MethodDecl::BodyTemplate = BodyTemplate;
   }

   bool isAbstract() const
   {
      return getFlag(Abstract);
   }

   bool isProtocolMethod() const
   {
      return getFlag(ProtoMethod);
   }

   bool isVirtual() const
   {
      return getFlag(Virtual);
   }

   bool isOverride() const
   {
      return getFlag(Override);
   }

   bool isProperty() const
   {
      return getFlag(Property);
   }

   bool isTemplatedInitializer() const;

   bool isProtocolDefaultImpl() const
   {
      return getFlag(ProtoMethod);
   }

   bool isHasDefinition() const
   {
      return getFlag(Defined);
   }

   bool isMemberwiseInitializer() const
   {
      return getFlag(MemberwiseInit);
   }

   bool isDefaultInitializer() const
   {
      return getFlag(DefaultInit);
   }

   void setDefaultInitializer(bool defaultInitializer)
   {
      setFlag(DefaultInit, defaultInitializer);
   }

   bool isCastOp() const
   {
      return getFlag(ConvOp);
   }

   void setMutating(bool mutating)
   {
      setFlag(MutableSelf, mutating);
   }

   void setIsProtocolMethod(bool is_protocol_method)
   {
      setFlag(ProtoMethod, is_protocol_method);
   }

   void setIsVirtual(bool is_virtual)
   {
      setFlag(Virtual, is_virtual);
   }

   void setProperty(bool property)
   {
      setFlag(Property, property);
   }

   void setProtocolDefaultImpl(bool protocolDefaultImpl)
   {
      setFlag(ProtoDefaultImpl, protocolDefaultImpl);
   }

   void setMemberwiseInitializer(bool memberwiseInitializer)
   {
      setFlag(MemberwiseInit, memberwiseInitializer);
   }
};

class InitDecl: public MethodDecl {
public:
   InitDecl()
      : MethodDecl(InitDeclID, "init", SourceType(), {}, {}, nullptr,
                   (AccessModifier)0, true),
        memberwise(true)
   {
      setMemberwiseInitializer(true);
   }

   InitDecl(std::vector<FuncArgDecl* > &&args,
            AccessModifier am,
            Statement* body,
            std::string &&name = "init")
      : MethodDecl(InitDeclID, move(name), SourceType(), std::move(args),
                   {}, body, am, true),
        memberwise(false)
   {
   }

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


protected:
   bool memberwise : 1;

public:
   bool isMemberwise() const
   {
      return memberwise;
   }
};

class DeinitDecl: public MethodDecl {
public:
   explicit DeinitDecl(Statement* body = nullptr)
      : MethodDecl(DeinitDeclID, "deinit", SourceType(), {}, {}, body,
                   (AccessModifier)0)
   {

   }

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

class FieldDecl: public NamedDecl {
public:
   FieldDecl(std::string &&name,
             SourceType type,
             AccessModifier am,
             bool isStatic = false,
             bool isConst = false,
             Expression* defaultVal = {})
      : NamedDecl(FieldDeclID, am, move(name)),
        type(type), defaultVal(defaultVal)
   {
      setDeclFlag(DF_Static, isStatic);
      setDeclFlag(DF_Const, isConst);
   }

   inline void setDefault(Expression* expr)
   {
      defaultVal = expr;
   }

   void addGetter(CompoundStmt* body = nullptr)
   {
      has_getter = true;
      getterBody = body;
   }

   void addSetter(CompoundStmt* body = nullptr)
   {
      has_setter = true;
      setterBody = body;
   }

   static bool classofKind(DeclKind kind)
   {
      return kind == FieldDeclID;
   }

   static bool classof(Decl const* T)
   {
      return classofKind(T->getKind());
   }

protected:
   std::string linkageName;

   bool has_getter = false;
   bool has_setter = false;

   CompoundStmt* getterBody = nullptr;
   CompoundStmt* setterBody = nullptr;

   MethodDecl *getterMethod;
   MethodDecl *setterMethod;

   FuncArgDecl* newVal = nullptr;

   SourceType type;
   Expression* defaultVal;

   size_t globalOrdering = 0;

public:
   const std::string &getLinkageName() const
   {
      return linkageName;
   }

   void setLinkageName(std::string &&linkageName)
   {
      FieldDecl::linkageName = move(linkageName);
   }

   bool hasGetter() const
   {
      return has_getter;
   }

   void hasGetter(bool hasGetter)
   {
      FieldDecl::has_getter = hasGetter;
   }

   bool hasSetter() const
   {
      return has_setter;
   }

   void hasSetter(bool hasSetter)
   {
      FieldDecl::has_setter = hasSetter;
   }

   CompoundStmt* getGetterBody() const
   {
      return getterBody;
   }

   void setGetterBody(CompoundStmt* getterBody)
   {
      FieldDecl::getterBody = getterBody;
   }

   CompoundStmt* getSetterBody() const
   {
      return setterBody;
   }

   void setSetterBody(CompoundStmt* setterBody)
   {
      FieldDecl::setterBody = setterBody;
   }

   MethodDecl *getGetterMethod() const
   {
      return getterMethod;
   }

   void setGetterMethod(MethodDecl *getterMethod)
   {
      FieldDecl::getterMethod = getterMethod;
   }

   MethodDecl *getSetterMethod() const
   {
      return setterMethod;
   }

   void setSetterMethod(MethodDecl *setterMethod)
   {
      FieldDecl::setterMethod = setterMethod;
   }

   FuncArgDecl* getNewVal() const
   {
      return newVal;
   }

   void setNewVal(FuncArgDecl* newVal)
   {
      FieldDecl::newVal = newVal;
   }

   const SourceType &getType() const
   {
      return type;
   }

   void setType(SourceType type)
   {
      FieldDecl::type = type;
   }

   Expression* getDefaultVal() const
   {
      return defaultVal;
   }

   size_t getGlobalOrdering() const
   {
      return globalOrdering;
   }

   void setGlobalOrdering(size_t globalOrder)
   {
      FieldDecl::globalOrdering = globalOrder;
   }
};

class AssociatedTypeDecl: public NamedDecl {
public:
   AssociatedTypeDecl(std::string &&protoSpec,
                      std::string &&name,
                      std::vector<StaticExpr* > &&constraints,
                      SourceType actualType)
      : NamedDecl(AssociatedTypeDeclID, (AccessModifier)0, move(name)),
        protocolSpecifier(move(protoSpec)),
        constraints(move(constraints)), actualType(actualType)
   {}

   const std::vector<StaticExpr* > &getConstraints() const
   {
      return constraints;
   }

   SourceType getActualType() const
   {
      return actualType;
   }

   void setActualType(SourceType actualType)
   {
      AssociatedTypeDecl::actualType = actualType;
   }

   llvm::StringRef getProtocolSpecifier() const
   {
      return protocolSpecifier;
   }

   ProtocolDecl *getProto() const { return Proto; }

   void setProto(ProtocolDecl *Proto)
   {
      AssociatedTypeDecl::Proto = Proto;
   }

   static bool classofKind(DeclKind kind)
   {
      return kind == AssociatedTypeDeclID;
   }

   static bool classof(Decl const* T)
   {
      return classofKind(T->getKind());
   }

private:
   std::string protocolSpecifier;
   std::vector<StaticExpr* > constraints;
   SourceType actualType;

   ProtocolDecl *Proto = nullptr;
};

class PropDecl: public NamedDecl {
public:
   PropDecl(std::string &&propName,
            SourceType type,
            AccessModifier access,
            bool isStatic,
            bool hasDefinition,
            bool hasGetter,
            bool hasSetter,
            CompoundStmt* getter,
            CompoundStmt* setter,
            std::string &&newValName)
      : NamedDecl(PropDeclID, access, std::move(propName)),
        type(type),
        getterBody(getter), setterBody(setter),
        HasGetter(hasGetter), HasSetter(hasSetter), newValName(move(newValName))
   {
      setDeclFlag(DF_Static, isStatic);
      setDeclFlag(DF_HasDefinition, hasDefinition);
   }

   static bool classof(Decl const* T) { return classofKind(T->getKind()); }
   static bool classofKind(DeclKind kind) { return kind == PropDeclID; }

protected:
   SourceType type;

   CompoundStmt* getterBody = nullptr;
   CompoundStmt* setterBody = nullptr;

   bool HasGetter = false;
   bool HasSetter = false;

   std::string newValName;

   MethodDecl *getterMethod = nullptr;
   MethodDecl *setterMethod = nullptr;

   PropDecl *PropTemplate = nullptr;

public:
   const SourceType &getType() const { return type; }

   bool hasGetter() const { return HasGetter; }
   bool hasSetter() const { return HasSetter; }

   CompoundStmt* getGetterBody() const { return getterBody; }
   CompoundStmt* getSetterBody() const { return setterBody; }

   void setGetterBody(CompoundStmt *B) { getterBody = B; }
   void setSetterBody(CompoundStmt *B) { setterBody = B; }

   std::string &getNewValName() { return newValName; }
   std::string const& getNewValName() const { return newValName; }

   MethodDecl *getGetterMethod() const { return getterMethod; }
   void setGetterMethod(MethodDecl *M) { getterMethod = M; }

   MethodDecl *getSetterMethod() const { return setterMethod; }
   void setSetterMethod(MethodDecl *M) { setterMethod = M; }

   PropDecl *getPropTemplate() const { return PropTemplate; }
   void setPropTemplate(PropDecl *T) { PropTemplate = T; }
};

class EnumCaseDecl: public CallableDecl {
public:
   typedef std::vector<FuncArgDecl*> AssociatedTypeVec;
   EnumCaseDecl(std::string&& name, StaticExpr* rawValue,
                AssociatedTypeVec&& associatedTypes)
      : CallableDecl(EnumCaseDeclID, (AccessModifier)0, move(name),
                     SourceType(), move(associatedTypes), {}, nullptr,
                     OperatorInfo()),
        rawValExpr(rawValue)
   {}

   EnumCaseDecl(std::string&& name, AssociatedTypeVec&& associatedTypes)
      : CallableDecl(EnumCaseDeclID, (AccessModifier)0, move(name),
                     SourceType(), move(associatedTypes), {}, nullptr,
                     OperatorInfo()),
        rawValExpr(nullptr)
   {}

   static bool classofKind(DeclKind kind)
   {
      return kind == EnumCaseDeclID;
   }

   static bool classof(Decl const* T)
   {
      return classofKind(T->getKind());
   }

protected:
   StaticExpr* rawValExpr;

   long long rawValue = 0;

public:
   StaticExpr* getRawValExpr() const
   {
      return rawValExpr;
   }

   void setRawValExpr(StaticExpr *rawValExpr)
   {
      EnumCaseDecl::rawValExpr = rawValExpr;
   }

   long long getRawValue() const
   {
      return rawValue;
   }

   void setRawValue(long rawValue)
   {
      EnumCaseDecl::rawValue = rawValue;
   }

   bool hasRawValue() const
   {
      return rawValExpr != nullptr;
   }

   std::vector<TemplateParamDecl*> &getTemplateParamsRef()
   {
      return templateParams;
   }
};

class StaticIfDecl: public Decl {
public:
   StaticIfDecl(SourceLocation StaticLoc,
                SourceLocation RBRaceLoc,
                StaticExpr *condition,
                CompoundDecl *IfDecl,
                CompoundDecl *ElseDecl)
      : Decl(StaticIfDeclID),
        StaticLoc(StaticLoc), RBRaceLoc(RBRaceLoc),
        condition(condition), IfDecl(IfDecl), ElseDecl(ElseDecl)
   {
      sourceLoc = StaticLoc;
   }

   static bool classofKind(DeclKind kind) { return kind == StaticIfDeclID; }
   static bool classof(Decl const *T) { return classofKind(T->getKind()); }

private:
   SourceLocation StaticLoc;
   SourceLocation RBRaceLoc;

   StaticExpr *condition;
   CompoundDecl *IfDecl;
   CompoundDecl *ElseDecl;

public:
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
public:
   StaticForDecl(SourceLocation StaticLoc,
                 SourceLocation RBRaceLoc,
                 std::string &&elementName,
                 StaticExpr *range,
                 CompoundDecl *BodyDecl)
      : Decl(StaticForDeclID),
        StaticLoc(StaticLoc), RBRaceLoc(RBRaceLoc),
        elementName(move(elementName)), range(range), BodyDecl(BodyDecl)
   {
      sourceLoc = StaticLoc;
   }

   static bool classofKind(DeclKind kind) { return kind == StaticForDeclID; }
   static bool classof(Decl const *T) { return classofKind(T->getKind()); }

private:
   SourceLocation StaticLoc;
   SourceLocation RBRaceLoc;

   std::string elementName;
   StaticExpr *range;
   CompoundDecl *BodyDecl;

public:
   llvm::StringRef getElementName() const { return elementName; }

   StaticExpr *getRange() const { return range; }
   void setRange(StaticExpr *S) { range = S; }

   CompoundDecl *getBodyDecl() const { return BodyDecl; }
   void setBodyDecl(CompoundDecl *D) { BodyDecl = D; }

   SourceLocation getStaticLoc() const { return StaticLoc; }
   SourceLocation getRBRaceLoc() const { return RBRaceLoc; }
};

class StaticAssertStmt: public Decl {
public:
   explicit StaticAssertStmt(StaticExpr* expr,
                             std::string &&message)
      : Decl(StaticAssertStmtID), expr(expr), message(move(message))
   {}

   static bool classofKind(DeclKind kind) { return kind == StaticAssertStmtID; }
   static bool classof(Decl const *T) { return classofKind(T->getKind()); }

private:
   StaticExpr* expr;
   std::string message;

   bool evaluated = false;

public:
   StaticExpr* getExpr() const { return expr; }
   void setExpr(StaticExpr *E) { expr = E; }

   const std::string &getMessage() const
   {
      return message;
   }

   bool isEvaluated() const
   {
      return evaluated;
   }

   void setEvaluated(bool evaluated)
   {
      StaticAssertStmt::evaluated = evaluated;
   }
};

class StaticPrintStmt: public Decl {
public:
   explicit StaticPrintStmt(StaticExpr* expr)
      : Decl(StaticPrintStmtID), expr(expr)
   {}

   static bool classofKind(DeclKind kind) { return kind == StaticPrintStmtID; }
   static bool classof(Decl const *T) { return classofKind(T->getKind()); }


private:
   StaticExpr* expr;

public:
   StaticExpr* getExpr() const { return expr; }
   void setExpr(StaticExpr *E) { expr = E; }
};

class MixinDecl: public Decl {
public:
   static MixinDecl *Create(ASTContext &C,
                            SourceRange Parens,
                            Expression *MixinExpr);

   static bool classofKind(DeclKind kind) { return kind == MixinDeclID; }
   static bool classof(Decl const *T) { return classofKind(T->getKind()); }

   Expression *getMixinExpr() const { return MixinExpr; }
   void setMixinExpr(Expression *E) { MixinExpr = E; }

   SourceRange getSourceRange() const { return Parens; }

private:
   MixinDecl(SourceRange Parens, Expression *MixinExpr);

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

}

namespace llvm {

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
struct cast_convert_val<ToTy, ::cdot::ast::DeclContext, ::cdot::ast::DeclContext> {
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
struct cast_convert_val<ToTy, ::cdot::ast::DeclContext *, ::cdot::ast::DeclContext *> {
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
