//
// Created by Jonas Zell on 22.01.18.
//

#ifndef CDOT_NAMEDDECL_H
#define CDOT_NAMEDDECL_H

#include "AST/SourceType.h"
#include "Attr.h"
#include "Basic/DeclarationName.h"
#include "Basic/Precedence.h"
#include "Sema/Template.h"
#include "Support/Casting.h"

#include <llvm/ADT/DenseMap.h>
#include <llvm/ADT/PointerUnion.h>
#include <llvm/ADT/SmallPtrSet.h>
#include <llvm/Support/TrailingObjects.h>

#include <unordered_map>

namespace cdot {
namespace il {
   class Constant;
}

class FunctionType;
enum class BuiltinFn : unsigned char;
enum class KnownFunction : unsigned char;

enum class AccessSpecifier : unsigned char {
   Default = 0,
   Public,
   Private,
   Protected,
   Internal,
   FilePrivate,
};

namespace ast {

class NamedDecl;
class Statement;
class DeclStmt;
class DeclContext;
class RecordDecl;
class StaticExpr;
class TemplateParamDecl;
class ImportDecl;
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
class StaticExpr;
class AssociatedTypeDecl;
class EnumDecl;
class UnionDecl;
class ProtocolDecl;
class ExtensionDecl;

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
      DF_ExternC             = DF_CheckedAttrs << 1u,
      DF_ExternCXX           = DF_ExternC << 1u,
      DF_Instantiation       = DF_ExternCXX << 1u,
      DF_Builtin             = DF_Instantiation << 1u,

      StatusFlags            = DF_TypeDependent | DF_ValueDependent |
                               DF_IsInvalid,
   };

   void dumpFlags() const;
   void printFlags(llvm::raw_ostream &OS) const;

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

   bool isExternC() const { return declFlagSet(DF_ExternC); }
   void setExternC(bool ext) { setDeclFlag(DF_ExternC, ext); }

   bool isExternCXX() const { return declFlagSet(DF_ExternCXX); }
   void setExternCXX(bool ext) { setDeclFlag(DF_ExternCXX, ext); }

   bool isBuiltin() const { return declFlagSet(DF_Builtin); }
   void setBuiltin(bool BI) { setDeclFlag(DF_Builtin, BI); }

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

   bool isInStdNamespace() const;
   bool isInCompilerNamespace() const;

   bool isInExtension() const;

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

   DeclContext *getNonTransparentDeclContext() const;

   DeclContext *getDeclContext() const;
   DeclContext *getLexicalContext() const;

   void setLexicalContext(DeclContext *ctx);
   void setLogicalContext(DeclContext *Ctx);
   void setLexicalContextUnchecked(DeclContext *ctx);

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

   struct LLVM_ALIGNAS(8) MultipleDeclContext {
      DeclContext *LogicalDC;
      DeclContext *LexicalDC;
   };

   using DeclContextUnion = llvm::PointerUnion<DeclContext*,
                                               MultipleDeclContext*>;

   DeclContextUnion ContextUnion;
};

class NamedDecl: public Decl {
public:
   AccessSpecifier getAccess() const { return access; }
   void setAccess(AccessSpecifier AS) { access = AS; }

   SourceLocation getAccessLoc() const { return AccessLoc; }
   void setAccessLoc(SourceLocation Loc);
   SourceRange getAccessRange() const;

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
   bool inDependentContext() const;

   llvm::ArrayRef<TemplateParamDecl*> getTemplateParams() const;

   bool isInstantiation() const { return declFlagSet(DF_Instantiation) ;}
   void setIsInstantiation(bool I) { setDeclFlag(DF_Instantiation, I); }

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

   static bool classof(const Decl *T) { return classofKind(T->getKind()); }
   static bool classofKind(DeclKind kind)
   {
      return kind > _firstNamedDeclID
             && kind < _lastNamedDeclID;
   }

protected:
   NamedDecl(DeclKind typeID,
             AccessSpecifier access,
             DeclarationName DN);

   AccessSpecifier access;
   SourceLocation AccessLoc;

   DeclarationName Name;
};

class UsingDecl final: public NamedDecl,
                       llvm::TrailingObjects<UsingDecl, IdentifierInfo*> {
   UsingDecl(SourceRange Loc,
             AccessSpecifier Access,
             DeclarationName Name,
             llvm::ArrayRef<IdentifierInfo*> NestedImportName,
             bool wildCardImport);

   SourceRange Loc;
   unsigned NumSpecifierNames;
   bool IsWildCard;

public:
   static bool classofKind(DeclKind kind) { return kind == UsingDeclID; }
   static bool classof(Decl const *T) { return classofKind(T->getKind()); }

   static UsingDecl *Create(ASTContext &C,
                            SourceRange Loc,
                            AccessSpecifier Access,
                            DeclarationName Name,
                            llvm::ArrayRef<IdentifierInfo*> NestedImportName,
                            bool wildCardImport);

   friend TrailingObjects;

   SourceRange getSourceRange() const { return Loc; }

   llvm::ArrayRef<IdentifierInfo*> getNestedImportName() const
   {
      return { getTrailingObjects<IdentifierInfo*>(), NumSpecifierNames };
   }

   bool isWildCardImport() const { return IsWildCard; }
};

class ModuleDecl final: public NamedDecl,
                        llvm::TrailingObjects<ModuleDecl, IdentifierInfo*> {
   ModuleDecl(SourceRange Loc,
              AccessSpecifier Access,
              llvm::ArrayRef<IdentifierInfo*> moduleName);

   SourceRange Loc;
   unsigned NumNameQuals;

public:
   static bool classofKind(DeclKind kind) { return kind == ModuleDeclID; }
   static bool classof(Decl const *T) { return classofKind(T->getKind()); }

   static ModuleDecl *Create(ASTContext &C,
                             SourceRange Loc,
                             AccessSpecifier Access,
                             llvm::ArrayRef<IdentifierInfo*> moduleName);

   friend TrailingObjects;

   SourceRange getSourceRange() const { return Loc; }

   llvm::ArrayRef<IdentifierInfo*> getQualifiedModuleName() const
   {
      return { getTrailingObjects<IdentifierInfo*>(), NumNameQuals };
   }
};

class ImportDecl final: public NamedDecl,
                        llvm::TrailingObjects<ImportDecl, IdentifierInfo*> {
   ImportDecl(SourceRange Loc,
              AccessSpecifier Access,
              llvm::ArrayRef<IdentifierInfo*> moduleName);

   SourceRange Loc;
   unsigned NumNameQuals;

public:
   static bool classofKind(DeclKind kind) { return kind == ImportDeclID; }
   static bool classof(Decl const *T) { return classofKind(T->getKind()); }

   static ImportDecl *Create(ASTContext &C,
                             SourceRange Loc,
                             AccessSpecifier Access,
                             llvm::ArrayRef<IdentifierInfo*> moduleName);

   friend TrailingObjects;

   SourceRange getSourceRange() const { return Loc; }

   llvm::ArrayRef<IdentifierInfo*> getQualifiedImportName() const
   {
      return { getTrailingObjects<IdentifierInfo*>(), NumNameQuals };
   }
};


class TemplateParamDecl: public NamedDecl {
public:
   static TemplateParamDecl *Create(ASTContext &C,
                                    DeclarationName Name,
                                    SourceType covariance,
                                    SourceType contravariance,
                                    Expression *defaultValue,
                                    unsigned Index,
                                    SourceLocation TypeNameOrValueLoc,
                                    SourceLocation NameLoc,
                                    SourceLocation EllipsisLoc);

   static TemplateParamDecl *Create(ASTContext &C,
                                    DeclarationName Name,
                                    SourceType valueType,
                                    Expression *defaultValue,
                                    unsigned Index,
                                    SourceLocation TypeNameOrValueLoc,
                                    SourceLocation NameLoc,
                                    SourceLocation EllipsisLoc);

   static bool classof(Decl const* T) { return classofKind(T->getKind()); }
   static bool classofKind(DeclKind kind){ return kind == TemplateParamDeclID; }
   
private:
   TemplateParamDecl(DeclarationName Name,
                     SourceType covariance,
                     SourceType contravariance,
                     Expression *defaultValue,
                     unsigned Index,
                     SourceLocation TypeNameOrValueLoc,
                     SourceLocation NameLoc,
                     SourceLocation EllipsisLoc);

   TemplateParamDecl(DeclarationName Name,
                     SourceType valueType,
                     Expression *defaultValue,
                     unsigned Index,
                     SourceLocation TypeNameOrValueLoc,
                     SourceLocation NameLoc,
                     SourceLocation EllipsisLoc);

   SourceType covariance;
   SourceType contravariance;

   bool typeName : 1;
   Expression *defaultValue;
   unsigned Index;

   SourceLocation TypeNameOrValueLoc;
   SourceLocation NameLoc;
   SourceLocation EllipsisLoc;

public:
   SourceLocation getTypeNameOrValueLoc() const { return TypeNameOrValueLoc; }
   SourceLocation getNameLoc() const { return NameLoc; }
   SourceLocation getEllipsisLoc() const { return EllipsisLoc; }
   SourceRange getSourceRange() const;

   const SourceType &getCovariance() const { return covariance; }
   const SourceType &getValueType() const { return covariance; }

   void setCovariance(SourceType cov) { covariance = cov; }

   const SourceType &getContravariance() const { return contravariance; }
   void setContravariance(SourceType con) { contravariance = con; }

   Expression *getDefaultValue() const { return defaultValue; }
   void setDefaultValue(Expression *val) { defaultValue = val; }

   bool isTypeName() const { return typeName; }
   bool isVariadic() const { return EllipsisLoc.isValid(); }
   unsigned getIndex() const { return Index; }
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
           AccessSpecifier access,
           SourceLocation VarOrLetLoc,
           SourceLocation ColonLoc,
           bool isConst,
           DeclarationName Name,
           SourceType type,
           Expression* value);

   SourceLocation VarOrLetLoc;
   SourceLocation ColonLoc;
   SourceLocation EqualsLoc;
   SourceType type;
   Expression* value = nullptr;

   bool CanElideCopy : 1;
   bool Variadic     : 1;
   bool Captured     : 1;

public:
   SourceRange getSourceRange() const;
   SourceLocation getVarOrLetLoc() const { return VarOrLetLoc; }
   SourceLocation getColonLoc() const { return ColonLoc; }

   void setEqualsLoc(SourceLocation L) { EqualsLoc = L; }
   SourceLocation getEqualsLoc() const { return EqualsLoc; }

   const SourceType &getType() const { return type; }
   void setType(SourceType ty) { type = ty; }

   Expression* getValue() const { return value; }
   void setValue(Expression *V) { value = V; }

   bool isVariadic() const { return Variadic; }
   void setVariadic(bool V) { Variadic = V; }

   bool canElideCopy() const { return CanElideCopy; }
   void setCanElideCopy(bool CanElide) { CanElideCopy = CanElide; }

   bool isCaptured() const { return Captured; }
   void setCaptured(bool capt) { Captured = capt; }
};

class LocalVarDecl: public VarDecl {
   LocalVarDecl(AccessSpecifier access,
                SourceLocation VarOrLetLoc,
                SourceLocation ColonLoc,
                bool isConst,
                DeclarationName Name,
                SourceType type,
                Expression* value);

   bool IsNRVOCand : 1;

public:
   static LocalVarDecl *Create(ASTContext &C,
                               AccessSpecifier access,
                               SourceLocation VarOrLetLoc,
                               SourceLocation ColonLoc,
                               bool isConst,
                               DeclarationName Name,
                               SourceType type,
                               Expression* value);

   bool isNRVOCandidate() const { return IsNRVOCand; }
   void setIsNRVOCandidate(bool NRVO) { IsNRVOCand = NRVO; }

   static bool classof(Decl const* T) { return classofKind(T->getKind()); }
   static bool classofKind(DeclKind kind) { return kind == LocalVarDeclID; }
};

class GlobalVarDecl: public VarDecl {
   GlobalVarDecl(AccessSpecifier access,
                 SourceLocation VarOrLetLoc,
                 SourceLocation ColonLoc,
                 bool isConst,
                 DeclarationName Name,
                 SourceType type,
                 Expression* value);

   size_t globalOrdering = 0;

public:
   static GlobalVarDecl *Create(ASTContext &C,
                                AccessSpecifier access,
                                SourceLocation VarOrLetLoc,
                                SourceLocation ColonLoc,
                                bool isConst,
                                DeclarationName Name,
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
               DeclarationName Name,
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
                              DeclarationName Name,
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

   using DeclsMap = llvm::SmallDenseMap<DeclarationName, DeclList, 4>;

   bool isTransparent() const;
   bool isAnonymousNamespace() const;

   void addDecl(Decl *decl);

   [[nodiscard]]
   AddDeclResultKind addDecl(NamedDecl *decl);

   [[nodiscard]]
   AddDeclResultKind addDecl(DeclarationName Name, NamedDecl *decl);

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

   const DeclsMap &getAllNamedDecls() const { return namedDecls; }

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

   [[nodiscard]]
   AddDeclResultKind makeDeclAvailable(NamedDecl *decl);

   [[nodiscard]]
   AddDeclResultKind makeDeclAvailable(DeclarationName Name, NamedDecl *decl);

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

   DeclsMap namedDecls;
   Decl *firstDecl = nullptr;
   Decl *lastAddedDecl = nullptr;

   DeclContext *parentCtx = nullptr;

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
   using specific_decl_iterator_range
      = llvm::iterator_range<specific_decl_iterator<SpecificDecl>>;

   template<class SpecificDecl>
   specific_decl_iterator_range<SpecificDecl> getDecls() const
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
   private llvm::TrailingObjects<TranslationUnit, ImportDecl*> {
public:
   static TranslationUnit *Create(ASTContext &ASTCtx,
                                  IdentifierInfo *fileName,
                                  size_t sourceId,
                                  llvm::ArrayRef<ImportDecl*> imports);

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
                   llvm::ArrayRef<ImportDecl*> imports);

   ASTContext &ASTCtx;
   size_t sourceId;
   unsigned numImports;

public:
   llvm::StringRef getFileName() const { return getName(); }
   size_t getSourceId() const { return sourceId; }

   SourceRange getSourceRange() const { return SourceRange(); }

   using import_iterator = ImportDecl**;
   using import_range    = llvm::ArrayRef<ImportDecl*>;

   size_t import_size() const { return numImports; }

   import_iterator import_begin() { return getTrailingObjects<ImportDecl*>(); }
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
                 DeclarationName Name);

   SourceLocation NamespaceLoc;
   SourceRange Braces;
   
public:
   static NamespaceDecl *Create(ASTContext &C,
                                SourceLocation NamespaceLoc,
                                SourceLocation LBrace,
                                DeclarationName Name);

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

   bool isAnonymousNamespace() const { return !Name; }

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
                AccessSpecifier am,
                SourceLocation DefLoc,
                DeclarationName Name,
                SourceType returnType,
                llvm::ArrayRef<FuncArgDecl*> args,
                Statement* body,
                OperatorInfo op,
                std::vector<TemplateParamDecl*> &&templateParams);

   SourceLocation DefLoc;
   FunctionType *functionType = nullptr;

   SourceType returnType;
   unsigned NumArgs;
   Statement* body;

   std::vector<TemplateParamDecl*> templateParams;
   InstantiationInfo<CallableDecl> *instantiationInfo = nullptr;

   LocalVarDecl* NRVOCandidate = nullptr;
   llvm::SmallPtrSet<Type*, 2> thrownTypes;

   KnownFunction knownFnKind = KnownFunction(0);

   OperatorInfo op;

   enum Flag : uint32_t {
      // general flags
      NoThrow  = 1u,
      Native   = NoThrow << 1u,
      ConvOp   = Native  << 1u,
      External = ConvOp << 1u,
      Main     = External << 1u,
      Vararg   = Main << 1u,
      CVarArg  = Vararg << 1u,
      Defined  = CVarArg << 1u,
      NoReturn = Defined << 1u,
      Lambda   = NoReturn << 1u,
      GlobalCtor = Lambda << 1u,
      GlobalDtor = GlobalCtor << 1u,

      // method flags
      Abstract         = GlobalDtor << 1u,
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

   using arg_iterator       = FuncArgDecl**;
   using const_arg_iterator = FuncArgDecl* const*;

   arg_iterator arg_begin();
   arg_iterator arg_end() { return arg_begin() + NumArgs; }

   const_arg_iterator arg_begin() const;
   const_arg_iterator arg_end() const { return arg_begin() + NumArgs; }

   llvm::MutableArrayRef<FuncArgDecl*> getArgs()
   {
      return llvm::MutableArrayRef<FuncArgDecl*>(arg_begin(), NumArgs);
   }

   llvm::ArrayRef<FuncArgDecl*> getArgs() const
   {
      return llvm::ArrayRef<FuncArgDecl*>(arg_begin(), NumArgs);
   }

   void setBody(Statement* body)
   {
      setHasDefinition(body != nullptr);
      this->body = body;
   }

   SourceRange getSourceRange() const;
   SourceLocation getDefLoc() const { return DefLoc; }

   const SourceType &getReturnType() const { return returnType; }
   void setReturnType(SourceType RetTy) { returnType = RetTy; }

   Statement* getBody() const { return body; }

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

   bool isNative() const { return getFlag(Native); }
   void setNative(bool native) { setFlag(Native, native); }

   bool isVararg() const { return getFlag(Vararg); }
   void setVararg(bool vararg) { setFlag(Vararg, vararg); }

   bool isCstyleVararg() const { return getFlag(CVarArg); }
   void setCstyleVararg(bool cstyleVararg) { setFlag(CVarArg, cstyleVararg); }

   bool isLambda() const { return getFlag(Lambda); }
   void setIsLambda(bool l) { setFlag(Lambda, l); }

   bool isGlobalCtor() const { return getFlag(GlobalCtor); }
   void setGlobalCtor(bool ctor) { setFlag(GlobalCtor, ctor); }

   bool isGlobalDtor() const { return getFlag(GlobalDtor); }
   void setGlobalDtor(bool dtor) { setFlag(GlobalDtor, dtor); }

   bool hasMutableSelf() const { return getFlag(MutableSelf); }

   bool isNoReturn() const { return getFlag(NoReturn); }
   void setIsNoReturn(bool noRet) { setFlag(NoReturn, noRet); }

   bool isInitializerOfTemplate() const;
   bool isCaseOfTemplatedEnum() const;

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

   void setInstantiationInfo(InstantiationInfo<CallableDecl> *II)
   {
      instantiationInfo = II;
   }

   llvm::ArrayRef<TemplateParamDecl *> getTemplateParams() const
   {
      return templateParams;
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
};

class CompoundStmt;

class FunctionDecl : public CallableDecl {
   FunctionDecl(AccessSpecifier am,
                SourceLocation DefLoc,
                DeclarationName II,
                llvm::ArrayRef<FuncArgDecl*> args,
                SourceType returnType,
                Statement* body,
                OperatorInfo op,
                std::vector<TemplateParamDecl*> &&templateParams);

public:
   static FunctionDecl *Create(ASTContext &C,
                               AccessSpecifier am,
                               SourceLocation DefLoc,
                               DeclarationName II,
                               llvm::ArrayRef<FuncArgDecl*> args,
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
   TypedefDecl(AccessSpecifier access,
               SourceLocation Loc,
               DeclarationName Name,
               SourceType origin,
               std::vector<TemplateParamDecl*> &&templateParams);

   SourceLocation Loc;
   SourceType origin;
   std::vector<TemplateParamDecl*> templateParams;

public:
   static TypedefDecl *Create(ASTContext &C,
                              AccessSpecifier access,
                              SourceLocation Loc,
                              DeclarationName Name,
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

class AliasDecl final: public NamedDecl,
                       public DeclContext,
                       public llvm::FoldingSetNode,
                       llvm::TrailingObjects<AliasDecl, TemplateParamDecl*> {
   AliasDecl(SourceLocation Loc,
             AccessSpecifier AccessSpec,
             DeclarationName Name,
             SourceType Type,
             StaticExpr* aliasExpr,
             llvm::ArrayRef<TemplateParamDecl*> templateParams);

   SourceLocation Loc;
   SourceType Type;
   StaticExpr* aliasExpr;
   unsigned NumParams;
   InstantiationInfo<AliasDecl> *instantiationInfo = nullptr;

public:
   static AliasDecl *Create(ASTContext &C,
                            SourceLocation Loc,
                            AccessSpecifier AccessSpec,
                            DeclarationName Name,
                            SourceType Type,
                            StaticExpr* aliasExpr,
                            llvm::ArrayRef<TemplateParamDecl*> templateParams);

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

   friend TrailingObjects;

   SourceLocation getSourceLoc() const { return Loc; }
   SourceRange getSourceRange() const;

   const SourceType &getType() const { return Type; }

   StaticExpr* getAliasExpr() const { return aliasExpr; }
   void setAliasExpr(StaticExpr *Expr) { aliasExpr = Expr; }

   const Variant &getVal() const;

   llvm::ArrayRef<TemplateParamDecl *> getTemplateParams() const
   {
      return { getTrailingObjects<TemplateParamDecl*>(), NumParams };
   }

   InstantiationInfo <AliasDecl> *getInstantiationInfo() const
   {
      return instantiationInfo;
   }

   void setInstantiationInfo(InstantiationInfo <AliasDecl> *II)
   {
      instantiationInfo = II;
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

   llvm::ArrayRef<ExtensionDecl*> getExtensions() const;
   void addExtension(ExtensionDecl *E) const;

   bool hasMethodWithName(DeclarationName name) const;
   bool hasMethodTemplate(DeclarationName name) const;

   PropDecl *getProperty(DeclarationName name) const;
   FieldDecl* getField(DeclarationName name) const;

   bool hasInnerRecord(RecordDecl *R) const
   {
      return innerRecords.find(R) != innerRecords.end();
   }

   bool conformsTo(ProtocolDecl *P) const
   {
      return conformances.find(P) != conformances.end();
   }

   [[nodiscard]]
   DeclContext::AddDeclResultKind addDecl(NamedDecl *decl);

protected:
   RecordDecl(DeclKind typeID,
              AccessSpecifier access,
              SourceLocation KeywordLoc,
              DeclarationName Name,
              std::vector<SourceType> &&conformanceTypes,
              std::vector<TemplateParamDecl*> &&templateParams);

   SourceLocation KeywordLoc;
   SourceRange BraceRange;

   unsigned lastMethodID = 1;

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

   void setInstantiationInfo(InstantiationInfo<RecordDecl> *II)
   {
      instantiationInfo = II;
   }

   llvm::ArrayRef<TemplateParamDecl *> getTemplateParams() const
   {
      return templateParams;
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

   unsigned getSize() const { return occupiedBytes; }
   unsigned short getAlignment() const { return alignment; }

   unsigned getLastMethodID() const { return lastMethodID; }
   void setLastMethodID(unsigned ID) { lastMethodID = ID; }

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

class FieldDecl: public VarDecl {
public:
   static FieldDecl *Create(ASTContext &C,
                            AccessSpecifier Access,
                            SourceLocation VarOrLetLoc,
                            SourceLocation ColonLoc,
                            DeclarationName Name,
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
   FieldDecl(AccessSpecifier Access,
             SourceLocation VarOrLetLoc,
             SourceLocation ColonLoc,
             DeclarationName Name,
             SourceType Type,
             bool IsStatic,
             bool IsConst,
             Expression* DefaultVal);

   bool HasGetter = false;
   bool HasSetter = false;

   CompoundStmt* getterBody = nullptr;
   CompoundStmt* setterBody = nullptr;

   MethodDecl *getterMethod;
   MethodDecl *setterMethod;

   FuncArgDecl* newVal = nullptr;
   size_t globalOrdering = 0;

public:
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

   // used as a predicate for filtered_decl_iterator
   bool isNotStatic() const { return !isStatic(); }
};

class StructDecl: public RecordDecl {
public:
   static StructDecl *Create(ASTContext &C,
                             AccessSpecifier access,
                             SourceLocation KeywordLoc,
                             DeclarationName Name,
                             std::vector<SourceType> &&conformanceTypes,
                             std::vector<TemplateParamDecl*> &&templateParams);

   InitDecl*getParameterlessConstructor()const{return parameterlessConstructor;}
   void setParameterlessConstructor(InitDecl *C) {parameterlessConstructor = C;}

   InitDecl *getMemberwiseInitializer() const { return memberwiseInitializer; }
   void setMemberwiseInitializer(InitDecl *Init) {memberwiseInitializer = Init;}

   MethodDecl *getDefaultInitializer() const { return defaultInitializer; }
   void setDefaultInitializer(MethodDecl *Init) { defaultInitializer = Init; }

   using StoredFieldVec = llvm::SmallVector<FieldDecl*, 0>;
   using field_iterator = StoredFieldVec::const_iterator;

   field_iterator stored_field_begin() const { return StoredFields.begin(); }
   field_iterator stored_field_end() const { return StoredFields.end(); }

   const StoredFieldVec &getFields() const { return StoredFields; }

   unsigned getNumNonStaticFields() const
   { return (unsigned)StoredFields.size(); }

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
   StructDecl(AccessSpecifier access,
              SourceLocation KeywordLoc,
              DeclarationName Name,
              std::vector<SourceType> &&conformanceTypes,
              std::vector<TemplateParamDecl*> &&templateParams);

protected:
   StructDecl(DeclKind typeID,
              AccessSpecifier access,
              SourceLocation KeywordLoc,
              DeclarationName Name,
              std::vector<SourceType> &&conformanceTypes,
              std::vector<TemplateParamDecl*> &&templateParams);

   StoredFieldVec StoredFields;

   InitDecl* parameterlessConstructor = nullptr;
   InitDecl* memberwiseInitializer    = nullptr;
   MethodDecl* defaultInitializer     = nullptr;
};

class ClassDecl: public StructDecl {
public:
   static ClassDecl *Create(ASTContext &C,
                            AccessSpecifier access,
                            SourceLocation KeywordLoc,
                            DeclarationName Name,
                            std::vector<SourceType> &&conformanceTypes,
                            std::vector<TemplateParamDecl*> &&templateParams,
                            SourceType parentClass,
                            bool isAbstract);

   const SourceType &getParentType() const { return parentType; }

   ClassDecl *getParentClass() const { return parentClass; }
   void inherit(ClassDecl *C);

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

   void setNumVirtualFns(unsigned int Num) { NumVirtualFns = Num; }
   unsigned getNumVirtualFns() const { return NumVirtualFns; }

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
   ClassDecl(AccessSpecifier access,
             SourceLocation KeywordLoc,
             DeclarationName Name,
             std::vector<SourceType> &&conformanceTypes,
             std::vector<TemplateParamDecl*> &&templateParams,
             SourceType parentClass,
             bool isAbstract);

   SourceType parentType;
   ClassDecl *parentClass = nullptr;
   bool IsAbstract = false;

   unsigned NumVirtualFns = 0;

public:
   bool isAbstract() const { return IsAbstract; }
};

class EnumCaseDecl;

class EnumDecl: public RecordDecl {
public:
   static EnumDecl *Create(ASTContext &C,
                           AccessSpecifier access,
                           SourceLocation KeywordLoc,
                           DeclarationName Name,
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
   EnumDecl(AccessSpecifier access,
            SourceLocation KeywordLoc,
            DeclarationName Name,
            std::vector<SourceType> &&conformanceTypes,
            std::vector<TemplateParamDecl*> &&templateParams,
            SourceType rawType);

   SourceType rawType;
   size_t maxAssociatedTypes = 0;
   bool Unpopulated = true;

public:
   const SourceType &getRawType() const { return rawType; }
   void setRawType(SourceType ty) { rawType = ty; }

   size_t getMaxAssociatedTypes() const { return maxAssociatedTypes; }
   specific_decl_iterator_range<EnumCaseDecl> getCases() const
   {
      return getDecls<EnumCaseDecl>();
   }

   bool isUnpopulated() const { return Unpopulated; }
};

class UnionDecl: public RecordDecl {
public:
   static UnionDecl *Create(ASTContext &C,
                            AccessSpecifier access,
                            SourceLocation KeywordLoc,
                            DeclarationName Name,
                            std::vector<SourceType> &&conformanceTypes,
                            std::vector<TemplateParamDecl*> &&templateParams);

   bool isConst() const { return IsConst; }
   void isConst(bool is_const) { this->IsConst = is_const; }

   using field_iterator = filtered_decl_iterator<FieldDecl,
                                                 &FieldDecl::isNotStatic>;

   using field_iterator_range = llvm::iterator_range<field_iterator>;

   field_iterator stored_field_begin() const
   {
      return field_iterator(firstDecl);
   }

   field_iterator stored_field_end() const
   {
      return field_iterator();
   }

   field_iterator_range getFields() const
   {
      return { stored_field_begin(), stored_field_end() };
   }

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
   UnionDecl(AccessSpecifier access,
             SourceLocation KeywordLoc,
             DeclarationName Name,
             std::vector<SourceType> &&conformanceTypes,
             std::vector<TemplateParamDecl*> &&templateParams);

   bool IsConst = false;
};

class ProtocolDecl: public RecordDecl {
public:
   static ProtocolDecl *Create(ASTContext &C,
                               AccessSpecifier access,
                               SourceLocation KeywordLoc,
                               DeclarationName Name,
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
   ProtocolDecl(AccessSpecifier access,
                SourceLocation KeywordLoc,
                DeclarationName Name,
                std::vector<SourceType> &&conformanceTypes,
                std::vector<TemplateParamDecl*> &&templateParams);
};

class ExtensionDecl final: public NamedDecl,
                           public DeclContext,
                           llvm::TrailingObjects<ExtensionDecl, SourceType> {
public:
   static ExtensionDecl *Create(ASTContext &C,
                                AccessSpecifier access,
                                SourceLocation KeywordLoc,
                                SourceType ExtendedType,
                                llvm::ArrayRef<SourceType> conformanceTypes);

   static ExtensionDecl *Create(ASTContext &C,
                                AccessSpecifier access,
                                SourceLocation KeywordLoc,
                                RecordDecl *R,
                                llvm::ArrayRef<SourceType> conformanceTypes);

   SourceLocation getExtLoc() const { return ExtLoc; }
   SourceRange getBraceRange() const { return BraceRange; }
   void setBraceRange(SourceRange BR) { BraceRange = BR; }

   SourceRange getSourceRange() const
   {
      return SourceRange(ExtLoc, BraceRange.getEnd());
   }

   void setName(DeclarationName N) { Name = N; }
   const SourceType &getExtendedType() const { return ExtendedType; }

   RecordDecl *getExtendedRecord() const { return ExtendedRecord; }
   void setExtendedRecord(RecordDecl *R) { ExtendedRecord = R; }

   llvm::ArrayRef<SourceType> getConformanceTypes() const
   {
      return { getTrailingObjects<SourceType>(), NumConformances };
   }

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

   friend TrailingObjects;

private:
   ExtensionDecl(AccessSpecifier access,
                 SourceLocation KeywordLoc,
                 SourceType ExtendedType,
                 llvm::ArrayRef<SourceType> conformanceTypes);

   ExtensionDecl(AccessSpecifier access,
                 SourceLocation KeywordLoc,
                 RecordDecl *R,
                 llvm::ArrayRef<SourceType> conformanceTypes);

   SourceLocation ExtLoc;
   SourceRange BraceRange;

   SourceType ExtendedType;
   RecordDecl *ExtendedRecord = nullptr;

   unsigned NumConformances;
};

class MethodDecl: public CallableDecl {
public:
   static MethodDecl *Create(ASTContext &C,
                             AccessSpecifier access,
                             SourceLocation DefLoc,
                             DeclarationName II,
                             SourceType returnType,
                             llvm::ArrayRef<FuncArgDecl*> args,
                             std::vector<TemplateParamDecl*> &&templateParams,
                             Statement* body,
                             bool isStatic);

   static MethodDecl *CreateOperator(ASTContext &C,
                                     AccessSpecifier access,
                                     SourceLocation DefLoc,
                                     DeclarationName OperatorName,
                                     SourceType returnType,
                                     llvm::ArrayRef<FuncArgDecl*> args,
                                     std::vector<TemplateParamDecl*>
                                                               &&templateParams,
                                     Statement* body,
                                     OperatorInfo op,
                                     bool isStatic);

   static MethodDecl *CreateConversionOp(ASTContext &C,
                                         AccessSpecifier access,
                                         SourceLocation DefLoc,
                                         SourceType returnType,
                                         llvm::ArrayRef<FuncArgDecl*> args,
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
   MethodDecl(AccessSpecifier access,
              SourceLocation DefLoc,
              DeclarationName II,
              SourceType returnType,
              llvm::ArrayRef<FuncArgDecl*> args,
              std::vector<TemplateParamDecl*> &&templateParams,
              Statement* body,
              bool isStatic);

   MethodDecl(AccessSpecifier access,
              SourceLocation DefLoc,
              DeclarationName OperatorName,
              SourceType returnType,
              llvm::ArrayRef<FuncArgDecl*> args,
              std::vector<TemplateParamDecl*> &&templateParams,
              Statement* body,
              OperatorInfo op,
              bool isStatic);

   // the declaration name of a conversion operator is only known after the
   // first declaration pass
   MethodDecl(AccessSpecifier access,
              SourceLocation DefLoc,
              SourceType returnType,
              llvm::ArrayRef<FuncArgDecl*> args,
              std::vector<TemplateParamDecl*> &&templateParams,
              Statement* body);

protected:
   MethodDecl(DeclKind typeID,
              AccessSpecifier access,
              SourceLocation Loc,
              DeclarationName Name,
              SourceType returnType,
              llvm::ArrayRef<FuncArgDecl*> args,
              std::vector<TemplateParamDecl*> &&templateParams,
              Statement* body);

   unsigned methodID = 0;

   SourceLocation BodyInstantiationLoc;
   MethodDecl *BodyTemplate = nullptr;

   MethodDecl *OverridenMethod = nullptr;

public:
   void setName(DeclarationName Name)
   {
      assert(!this->Name && "name should only be set once!");
      this->Name = Name;
   }

   unsigned getMethodID() const { return methodID; }
   void setMethodID(unsigned ID) { methodID = ID; }

   SourceLocation getBodyInstantiationLoc()const{ return BodyInstantiationLoc; }
   void setBodyInstantiationLoc(SourceLocation Loc)
   {
      BodyInstantiationLoc = Loc;
   }

   MethodDecl *getBodyTemplate() const { return BodyTemplate; }
   void setBodyTemplate(MethodDecl *T) { BodyTemplate = T; }

   bool isAbstract() const { return getFlag(Abstract); }
   bool isProtocolMethod() const { return getFlag(ProtoMethod); }
   bool isVirtual() const { return getFlag(Virtual); }
   bool isOverride() const { return getFlag(Override); }
   bool isVirtualOrOverride() const { return isVirtual() || isOverride(); }
   bool isProperty() const { return getFlag(Property); }

   bool isProtocolDefaultImpl() const { return getFlag(ProtoMethod); }
   bool isHasDefinition() const { return getFlag(Defined); }

   bool isMemberwiseInitializer() const { return getFlag(MemberwiseInit); }
   bool isDefaultInitializer() const { return getFlag(DefaultInit); }
   bool isCastOp() const { return getFlag(ConvOp); }

   void setDefaultInitializer(bool DI) { setFlag(DefaultInit, DI); }
   void setMutating(bool mutating) { setFlag(MutableSelf, mutating); }
   void setIsProtocolMethod(bool PM) { setFlag(ProtoMethod, PM); }
   void setIsVirtual(bool virt) { setFlag(Virtual, virt); }
   void setIsOverride(bool ovr) { setFlag(Override, ovr); }
   void setProperty(bool property) { setFlag(Property, property); }
   void setProtocolDefaultImpl(bool impl) { setFlag(ProtoDefaultImpl, impl); }
   void setMemberwiseInitializer(bool init) { setFlag(MemberwiseInit, init); }

   MethodDecl *getOverridenMethod() const { return OverridenMethod; }
   void setOverridenMethod(MethodDecl *M) { OverridenMethod = M; }
};

class InitDecl: public MethodDecl {
public:
   static InitDecl *CreateMemberwise(ASTContext &C,
                                     AccessSpecifier am,
                                     SourceLocation Loc,
                                     DeclarationName Name = DeclarationName());

   static InitDecl *Create(ASTContext &C,
                           AccessSpecifier am,
                           SourceLocation Loc,
                           llvm::ArrayRef<FuncArgDecl*> args,
                           std::vector<TemplateParamDecl*> &&templateParams,
                           Statement* body,
                           DeclarationName Name = DeclarationName());

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
   InitDecl(AccessSpecifier am,
            SourceLocation Loc,
            DeclarationName Name);

   InitDecl(AccessSpecifier am,
            SourceLocation Loc,
            llvm::ArrayRef<FuncArgDecl*> args,
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
                             DeclarationName Name = DeclarationName());

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

class AssociatedTypeDecl: public NamedDecl {
public:
   static AssociatedTypeDecl *Create(ASTContext &C,
                                     SourceLocation Loc,
                                     IdentifierInfo *ProtoSpec,
                                     DeclarationName Name,
                                     SourceType actualType);

   SourceLocation getSourceLoc() const { return Loc; }
   SourceRange getSourceRange() const { return SourceRange(Loc); }

   const SourceType &getActualType() const { return actualType; }
   void setActualType(SourceType ty) { actualType = ty; }

   IdentifierInfo *getProtoSpecInfo() const { return protocolSpecifier; }
   llvm::StringRef getProtocolSpecifier() const
   {
      return protocolSpecifier->getIdentifier();
   }

   bool isImplementation() const;

   ProtocolDecl *getProto() const { return Proto; }
   void setProto(ProtocolDecl *P) { Proto = P; }

   static bool classofKind(DeclKind kind){ return kind == AssociatedTypeDeclID;}
   static bool classof(Decl const* T) { return classofKind(T->getKind()); }

private:
   AssociatedTypeDecl(SourceLocation Loc,
                      IdentifierInfo *ProtoSpec,
                      DeclarationName Name,
                      SourceType actualType);

   SourceLocation Loc;
   IdentifierInfo *protocolSpecifier;
   SourceType actualType;
   ProtocolDecl *Proto = nullptr;
};

class PropDecl: public NamedDecl {
   PropDecl(AccessSpecifier access,
            SourceRange Loc,
            DeclarationName Name,
            SourceType type,
            bool isStatic,
            bool hasDefinition,
            bool hasGetter,
            bool hasSetter,
            CompoundStmt* getter,
            CompoundStmt* setter,
            IdentifierInfo *newValName);

   SourceRange Loc;
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
                           AccessSpecifier access,
                           SourceRange Loc,
                           DeclarationName Name,
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

   SourceRange getSourceRange() const { return Loc; }

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
   EnumCaseDecl(AccessSpecifier AS,
                SourceLocation CaseLoc,
                SourceLocation IdentLoc,
                DeclarationName Name,
                StaticExpr* rawValue,
                llvm::ArrayRef<FuncArgDecl*> args);

   SourceLocation CaseLoc;
   SourceLocation IdentLoc;
   StaticExpr* rawValExpr;
   long long rawValue = 0;

   il::Constant *ILValue = nullptr;

public:
   static EnumCaseDecl *Create(ASTContext &C,
                               AccessSpecifier AS,
                               SourceLocation CaseLoc,
                               SourceLocation IdentLoc,
                               DeclarationName Name,
                               StaticExpr* rawValue,
                               llvm::ArrayRef<FuncArgDecl*> args);

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

   il::Constant *getILValue() const { return ILValue; }
   void setILValue(il::Constant *V) { ILValue = V; }

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

   IdentifierInfo *getElementName() const { return elementName; }

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
