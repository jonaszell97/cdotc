//
// Created by Jonas Zell on 22.01.18.
//

#ifndef CDOT_NAMEDDECL_H
#define CDOT_NAMEDDECL_H

#include "ASTVector.h"
#include "Basic/Precedence.h"
#include "ContinuationPoint.h"
#include "DeclBase.h"
#include "Lex/Token.h"
#include "Sema/Template.h"
#include "SourceType.h"
#include "Support/Casting.h"

#include <llvm/ADT/SmallPtrSet.h>
#include <llvm/Support/TrailingObjects.h>

#include <unordered_map>

namespace cdot {

class CompilerInstance;
class FunctionType;
class Module;

enum class BuiltinFn : unsigned char;
enum class KnownFunction : unsigned char;

enum class ConstructorKind : unsigned char {
   Complete, Base,
};

namespace il {
   class Constant;
   class Module;
} // namespace il

namespace ast {

class Statement;
class DeclStmt;
class RecordDecl;
class StaticExpr;
class TemplateParamDecl;
class ImportDecl;
class ConstraintExpr;
class Expression;
class NamespaceDecl;
class ModuleDecl;
class CallableDecl;
class ASTContext;
class SemaPass;
class FuncArgDecl;
class CompoundStmt;
class LocalVarDecl;
class FieldDecl;
class PropDecl;
class MethodDecl;
class StructDecl;
class InitDecl;
class TypedefDecl;
class DeinitDecl;
class StaticExpr;
class AssociatedTypeDecl;
class EnumDecl;
class UnionDecl;
class ProtocolDecl;
class ExtensionDecl;
class PrecedenceGroupDecl;

template<class T>
struct InstantiationInfo {
   InstantiationInfo(const SourceLocation &instantiatedFrom,
                     sema::FinalTemplateArgumentList *templateArgs,
                     T *specializedTemplate)
      : instantiatedFrom(instantiatedFrom),
        templateArgs(std::move(templateArgs)),
        specializedTemplate(specializedTemplate)
   { }

   InstantiationInfo() = default;

   SourceLocation instantiatedFrom;
   mutable sema::FinalTemplateArgumentList *templateArgs;
   T *specializedTemplate = nullptr;
   NamedDecl *instantiatedWithin = nullptr;
};

template <class T>
struct DefaultImplementable {
   T *getProtocolDefaultImpl() const { return DefaultImpl; }
   void setProtocolDefaultImpl(T *t) { DefaultImpl = t; }

protected:
   T *DefaultImpl = nullptr;
};

class UsingDecl final: public NamedDecl,
                       llvm::TrailingObjects<UsingDecl, IdentifierInfo*> {
   UsingDecl(SourceRange Loc,
             AccessSpecifier Access,
             DeclarationName Name,
             llvm::ArrayRef<IdentifierInfo*> NestedImportName,
             bool wildCardImport);

   UsingDecl(EmptyShell, unsigned N);

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

   static UsingDecl *CreateEmpty(ASTContext &C, unsigned N);

   using TrailingObjects::getTrailingObjects;
   friend TrailingObjects;

   SourceRange getSourceRange() const { return Loc; }
   void setSourceRange(SourceRange SR) { Loc = SR; }

   llvm::ArrayRef<IdentifierInfo*> getNestedImportName() const
   {
      return { getTrailingObjects<IdentifierInfo*>(), NumSpecifierNames };
   }

   bool isWildcardImport() const { return IsWildCard; }
   void setWildcardImport(bool V) { IsWildCard = V; }
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

   static TemplateParamDecl *CreateEmpty(ASTContext &C);

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

   TemplateParamDecl(EmptyShell Empty);

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

   void setTypeName(bool typeName) { TemplateParamDecl::typeName = typeName; }
   void setIndex(unsigned int Index) { TemplateParamDecl::Index = Index; }
   void setTypeNameOrValueLoc(SourceLocation Loc) { TypeNameOrValueLoc = Loc; }
   void setNameLoc(SourceLocation Loc) { NameLoc = Loc; }
   void setEllipsisLoc(SourceLocation Loc) { EllipsisLoc = Loc; }
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
   bool IsMovedFrom  : 1;

public:
   SourceRange getSourceRange() const;
   SourceLocation getVarOrLetLoc() const { return VarOrLetLoc; }
   SourceLocation getColonLoc() const { return ColonLoc; }

   void setVarOrLetLoc(SourceLocation Loc) { VarOrLetLoc = Loc; }
   void setColonLoc(SourceLocation Loc) { ColonLoc = Loc; }

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

   bool isMovedFrom() const { return IsMovedFrom; }
   void setMovedFrom(bool Moved) { VarDecl::IsMovedFrom = Moved; }
};

class LocalVarDecl: public VarDecl {
   LocalVarDecl(AccessSpecifier access,
                SourceLocation VarOrLetLoc,
                SourceLocation ColonLoc,
                bool isConst,
                DeclarationName Name,
                SourceType type,
                Expression* value);

   LocalVarDecl(EmptyShell Empty);

   bool IsNRVOCand : 1;
   bool InitIsMove : 1;

public:
   static LocalVarDecl *Create(ASTContext &C,
                               AccessSpecifier access,
                               SourceLocation VarOrLetLoc,
                               SourceLocation ColonLoc,
                               bool isConst,
                               DeclarationName Name,
                               SourceType type,
                               Expression* value);

   static LocalVarDecl *CreateEmpty(ASTContext &C);

   bool isNRVOCandidate() const { return IsNRVOCand; }
   void setIsNRVOCandidate(bool NRVO) { IsNRVOCand = NRVO; }

   bool isUninitialized() const { return value == nullptr; }

   bool isInitMove() const { return InitIsMove; }
   void setInitIsMove(bool IsMove) { InitIsMove = IsMove; }

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

   GlobalVarDecl(EmptyShell Empty);

   size_t globalOrdering = size_t(-1);

public:
   static GlobalVarDecl *Create(ASTContext &C,
                                AccessSpecifier access,
                                SourceLocation VarOrLetLoc,
                                SourceLocation ColonLoc,
                                bool isConst,
                                DeclarationName Name,
                                SourceType type,
                                Expression* value);

   static GlobalVarDecl *CreateEmpty(ASTContext &C);

   size_t getGlobalOrdering() const { return globalOrdering; }
   void setGlobalOrdering(size_t Order) { globalOrdering = Order; }

   static bool classof(Decl const* T) { return classofKind(T->getKind()); }
   static bool classofKind(DeclKind kind) { return kind == GlobalVarDeclID; }
};

class DestructuringDecl final: public Decl, TrailingObjects<DestructuringDecl,
                                                            VarDecl*> {
public:
   enum DestructuringKind {
      Unknown, Tuple, Struct, Custom,
   };

private:
   DestructuringDecl(SourceRange Parens,
                     ArrayRef<VarDecl*> Decls,
                     SourceType Type,
                     Expression* value);

   DestructuringDecl(EmptyShell, unsigned N);

   DestructuringKind Kind;
   SourceRange Parens;
   SourceType Type;
   Expression *Val;
   unsigned NumDecls;

   CallableDecl *DestructuringOp = nullptr;

public:
   static bool classofKind(DeclKind kind){ return kind == DestructuringDeclID; }
   static bool classof(Decl const* T) { return classofKind(T->getKind()); }

   friend TrailingObjects;

   static DestructuringDecl *Create(ASTContext &C,
                                    SourceRange Parens,
                                    ArrayRef<VarDecl*> Decls,
                                    SourceType Type,
                                    Expression* value);

   static DestructuringDecl *CreateEmpty(ASTContext &C, unsigned N);

   SourceRange getSourceRange() const { return Parens; }

   DestructuringKind getDestructuringKind() const { return Kind; }
   void setDestructuringKind(DestructuringKind V) { Kind = V; }

   CallableDecl* getDestructuringOp() const { return DestructuringOp; }
   void setDestructuringOp(CallableDecl* V) { DestructuringOp = V; }

   SourceRange getParenRange() const { return Parens; }
   void setParenRange(SourceRange V) { Parens = V; }

   const SourceType &getType() const { return Type; }
   void setType(SourceType V) { Type = V; }

   Expression* getValue() const { return Val; }
   void setValue(Expression* V) { Val = V; }

   unsigned getNumDecls() const { return NumDecls; }

   ArrayRef<VarDecl*> getDecls() const
   {
      return { getTrailingObjects<VarDecl*>(), NumDecls };
   }

   MutableArrayRef<VarDecl*> getDecls()
   {
      return { getTrailingObjects<VarDecl*>(), NumDecls };
   }
};

class FuncArgDecl: public VarDecl {
   FuncArgDecl(SourceLocation OwnershipLoc,
               SourceLocation ColonLoc,
               DeclarationName Name,
               IdentifierInfo *Label,
               ArgumentConvention Conv,
               SourceType argType,
               Expression* defaultValue,
               bool variadicArgPackExpansion,
               bool cstyleVararg,
               bool isSelf);

   FuncArgDecl(EmptyShell Empty);

   IdentifierInfo *Label = nullptr;
   bool VariadicArgPackExpansion : 1;
   bool Vararg : 1;
   bool CstyleVararg : 1;
   bool IsSelf : 1;
   ArgumentConvention Conv : 4;

public:
   static FuncArgDecl *Create(ASTContext &C,
                              SourceLocation OwnershipLoc,
                              SourceLocation ColonLoc,
                              DeclarationName Name,
                              IdentifierInfo *Label,
                              ArgumentConvention Conv,
                              SourceType argType,
                              Expression* defaultValue,
                              bool variadicArgPackExpansion,
                              bool cstyleVararg = false,
                              bool isSelf = false);

   static FuncArgDecl *CreateEmpty(ASTContext &C);

   Expression* getDefaultVal() const { return value; }
   void setDefaultVal(Expression *defaultVal) { value = defaultVal; }

   bool isVariadicArgPackExpansion() const { return VariadicArgPackExpansion; }
   bool isVararg() const { return Vararg; }
   bool isCstyleVararg() const { return CstyleVararg; }
   bool isSelf() const { return IsSelf; }

   void setVariadicArgPackExpansion(bool V) { VariadicArgPackExpansion = V; }
   void setVararg(bool V) { Vararg = V; }
   void setCstyleVararg(bool V) { CstyleVararg = V; }
   void setSelf(bool V) { IsSelf = V; }

   IdentifierInfo* getLabel() const { return Label; }
   void setLabel(IdentifierInfo* V) { Label = V; }

   SourceRange getSourceRange() const;
   SourceLocation getOwnershipLoc() const { return VarDecl::VarOrLetLoc; }

   ArgumentConvention getConvention() const { return Conv; }
   void setConvention(ArgumentConvention Conv) { FuncArgDecl::Conv = Conv; }

   static bool classofKind(DeclKind kind) { return kind == FuncArgDeclID; }
   static bool classof(Decl const* T) { return classofKind(T->getKind()); }
};

class ModuleDecl final: public NamedDecl, public DeclContext {
   ModuleDecl(SourceRange Loc,
              DeclarationName moduleName);

   ModuleDecl(EmptyShell Empty);

   /// The location this module was first encountered
   SourceRange Loc;

   /// The corresponding module instance.
   Module *Mod = nullptr;

public:
   static bool classofKind(DeclKind kind) { return kind == ModuleDeclID; }
   static bool classof(Decl const *T) { return classofKind(T->getKind()); }

   static ModuleDecl *Create(ASTContext &C,
                             SourceRange Loc,
                             DeclarationName moduleName);

   static ModuleDecl *CreateEmpty(ASTContext &C);

   friend class ASTContext;

   Module* getModule() const { return Mod; }
   void setModule(Module* V) { Mod = V; }

   ModuleDecl *getPrimaryModule() const;
   ModuleDecl *getBaseModule() const;
   ModuleDecl *getParentModule() const
   {
      return support::dyn_cast_or_null<ModuleDecl>(getParentCtx());
   }

   SourceRange getSourceRange() const { return Loc; }
   void setSourceRange(SourceRange SR) { Loc = SR; }
};

class GlobalDeclContext: public DeclContext {
   GlobalDeclContext(CompilerInstance &CI);

   CompilerInstance &CI;

public:
   static GlobalDeclContext *Create(ASTContext &C, CompilerInstance &CI);

   CompilerInstance &getCompilerInstance() const { return CI; }

   specific_decl_iterator_range<ModuleDecl> getModules()
   {
      return getDecls<ModuleDecl>();
   }

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

class ImportDecl final: public NamedDecl,
                        public DeclContext,
                        llvm::TrailingObjects<ImportDecl, IdentifierInfo*> {
   ImportDecl(SourceRange Loc,
              AccessSpecifier Access,
              llvm::ArrayRef<IdentifierInfo*> moduleName,
              llvm::ArrayRef<IdentifierInfo*> namedImports,
              bool IsWildcardImport);

   ImportDecl(EmptyShell, unsigned N);

   SourceRange Loc;
   bool WildcardImport;
   unsigned NumNameQuals;
   unsigned NumNamedImports;
   Module *ImportedModule = nullptr;

public:
   static bool classofKind(DeclKind kind) { return kind == ImportDeclID; }
   static bool classof(Decl const *T) { return classofKind(T->getKind()); }

   static ImportDecl *Create(ASTContext &C,
                             SourceRange Loc,
                             AccessSpecifier Access,
                             llvm::ArrayRef<IdentifierInfo*> moduleName,
                             llvm::ArrayRef<IdentifierInfo*> namedImports,
                             bool IsWildcardImport);

   static ImportDecl *CreateEmpty(ASTContext &C, unsigned N);

   using TrailingObjects::getTrailingObjects;
   friend TrailingObjects;

   SourceRange getSourceRange() const { return Loc; }
   void setSourceRange(SourceRange SR) { Loc = SR; }

   Module* getImportedModule() const { return ImportedModule; }
   void setImportedModule(Module* V) { ImportedModule = V; }

   bool isWildcardImport() const { return WildcardImport; }
   void setWildcardImport(bool V) { WildcardImport = V; }

   void setNumNameQuals(unsigned N) { NumNameQuals = N; }
   void setNumNamedImports(unsigned N) { NumNamedImports = N; }

   llvm::ArrayRef<IdentifierInfo*> getQualifiedImportName() const
   {
      return { getTrailingObjects<IdentifierInfo*>(), NumNameQuals };
   }

   llvm::ArrayRef<IdentifierInfo*> getNamedImports() const
   {
      return { getTrailingObjects<IdentifierInfo*>() + NumNameQuals,
         NumNamedImports };
   }
};

class NamespaceDecl: public NamedDecl, public DeclContext {
   NamespaceDecl(SourceLocation NamespaceLoc,
                 SourceLocation LBrace,
                 DeclarationName Name);

   NamespaceDecl(EmptyShell Empty);

   SourceLocation NamespaceLoc;
   SourceRange Braces;
   
public:
   static NamespaceDecl *Create(ASTContext &C,
                                SourceLocation NamespaceLoc,
                                SourceLocation LBrace,
                                DeclarationName Name);

   static NamespaceDecl *CreateEmpty(ASTContext &C);

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

   void setNamespaceLoc(SourceLocation Loc) { NamespaceLoc = Loc; }
   void setBraces(SourceRange V) { Braces = V; }

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
   CompoundDecl(EmptyShell Empty);

   SourceRange SR;
   bool Transparent : 1;

public:
   // allow stack construction
   CompoundDecl(SourceLocation LBraceLoc,
                bool Transparent);

   static CompoundDecl *Create(ASTContext &C,
                               SourceLocation LBraceLoc,
                               bool Transparent);

   static CompoundDecl *CreateEmpty(ASTContext &C);

   static bool classofKind(DeclKind kind) { return kind == CompoundDeclID; }
   static bool classof(Decl const *T) { return classofKind(T->getKind()); }

   SourceRange getSourceRange() const { return SR; }
   void setSourceRange(SourceRange V) { SR = V; }

   SourceLocation getLBraceLoc() const { return SR.getStart(); }
   SourceLocation getRBraceLoc() const { return SR.getEnd(); }

   // CompoundDecl needs to be created before its end location is known
   void setRBraceLoc(SourceLocation loc)
   {
      SR = SourceRange(SR.getStart(), loc);
   }

   bool isTransparent() const { return Transparent; }
   void setTransparent(bool V) { Transparent = V; }
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
              *instantiationInfo->templateArgs);
   }

   static void Profile(llvm::FoldingSetNodeID &ID, CallableDecl *Template,
                       sema::FinalTemplateArgumentList &list) {
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
                ASTVector<TemplateParamDecl*> &&templateParams);

   CallableDecl(EmptyShell E, DeclKind typeID, unsigned N);

   SourceLocation DefLoc;
   FunctionType *functionType = nullptr;

   SourceType returnType;
   unsigned NumArgs;
   Statement* body;

   serial::LazyFunctionInfo *LazyFnInfo = nullptr;

   ASTVector<TemplateParamDecl*> templateParams;
   InstantiationInfo<CallableDecl> *instantiationInfo = nullptr;

   LocalVarDecl* NRVOCandidate = nullptr;
   KnownFunction knownFnKind = KnownFunction(0);

   PrecedenceGroupDecl *PrecedenceGroup = nullptr;

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
      IsOperator = GlobalDtor << 1u,
      Throws     = IsOperator << 1u,
      Async      = Throws << 1u,
      Unsafe     = Async << 1u,
      ImplicitFutureReturn = Unsafe << 1u,
      CalledFromTemplate = ImplicitFutureReturn << 1u,

      // method flags
      Alias            = CalledFromTemplate << 1u,
      MutableSelf      = Alias << 1u,
      ProtoMethod      = MutableSelf << 1u,
      Virtual          = ProtoMethod << 1u,
      Override         = Virtual << 1u,
      Property         = Override << 1u,
      MemberwiseInit   = Property << 1u,
      DefaultInit      = MemberwiseInit << 1u,
      ProtoDefaultImpl = DefaultInit << 1u,
      Subscript = ProtoDefaultImpl << 1u,
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

   void setFunctionType(FunctionType *Ty) { functionType = Ty; }

   uint32_t getFunctionFlags() const { return Flags; }
   void setFunctionFlags(uint32_t Flags) { this->Flags = Flags; }

   serial::LazyFunctionInfo* getLazyFnInfo() const { return LazyFnInfo; }
   void setLazyFnInfo(serial::LazyFunctionInfo* V) { LazyFnInfo = V; }

   bool isKnownFunction();
   void checkKnownFnKind();
   KnownFunction getKnownFnKind();

   void setKnownFnKind(KnownFunction F) { knownFnKind = F; }

   bool isBaseInitializer() const;
   bool isCompleteInitializer() const;

   using arg_iterator       = FuncArgDecl**;
   using const_arg_iterator = FuncArgDecl* const*;

   arg_iterator arg_begin();
   arg_iterator arg_end() { return arg_begin() + NumArgs; }

   const_arg_iterator arg_begin() const;
   const_arg_iterator arg_end() const { return arg_begin() + NumArgs; }

   FuncArgDecl *getArgAt(unsigned idx) const
   {
      assert(idx < NumArgs && "bad argument index");
      return arg_begin()[idx];
   }

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

   PrecedenceGroupDecl *getPrecedenceGroup() const { return PrecedenceGroup; }
   void setPrecedenceGroup(PrecedenceGroupDecl *PG) { PrecedenceGroup = PG; }

   SourceRange getSourceRange() const;
   SourceLocation getDefLoc() const { return DefLoc; }

   void setDefLoc(SourceLocation Loc) { DefLoc = Loc; }

   const SourceType &getReturnType() const { return returnType; }
   void setReturnType(SourceType RetTy) { returnType = RetTy; }

   Statement* getBody() const { return body; }

   LocalVarDecl *getNRVOCandidate() const { return NRVOCandidate; }
   void setNRVOCandidate(LocalVarDecl *Cand) { NRVOCandidate = Cand; }

   bool isOperator() const { return getFlag(IsOperator); }

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

   bool throws() const { return getFlag(Throws); }
   void setThrows(bool t) { setFlag(Throws, t); }

   bool isAsync() const { return getFlag(Async); }
   void setAsync(bool a) { setFlag(Async, a); }

   bool isUnsafe() const { return getFlag(Unsafe); }
   void setUnsafe(bool u) { setFlag(Unsafe, u); }

   bool isCalledFromTemplate() const { return getFlag(CalledFromTemplate); }
   void setCalledFromTemplate(bool u) { setFlag(CalledFromTemplate, u); }

   bool isImplicitFutureReturn() const { return getFlag(ImplicitFutureReturn); }
   void setImplicitFutureReturn(bool u) { setFlag(ImplicitFutureReturn, u); }

   bool isInitializerOfTemplate() const;
   bool isCaseOfTemplatedEnum() const;
   bool isFallibleInit() const;

   bool isNonStaticMethod() const;
   bool willHaveDefinition() const;

   void setInstantiationInfo(InstantiationInfo<CallableDecl> *II)
   {
      instantiationInfo = II;
   }

   InstantiationInfo<CallableDecl> *getInstantiationInfo() const
   {
      return instantiationInfo;
   }

   llvm::ArrayRef<TemplateParamDecl *> getTemplateParams() const
   {
      return templateParams;
   }

   ASTVector<TemplateParamDecl*> &getTemplateParams()
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

   sema::FinalTemplateArgumentList &getTemplateArgs() const
   {
      return *instantiationInfo->templateArgs;
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
                ASTVector<TemplateParamDecl*> &&templateParams);

   FunctionDecl(EmptyShell Empty, unsigned N);

public:
   static FunctionDecl *Create(ASTContext &C,
                               AccessSpecifier am,
                               SourceLocation DefLoc,
                               DeclarationName II,
                               llvm::ArrayRef<FuncArgDecl*> args,
                               SourceType returnType,
                               Statement* body,
                               ASTVector<TemplateParamDecl*> &&templateParams);

   static FunctionDecl *CreateEmpty(ASTContext &C, unsigned N);

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
               ASTVector<TemplateParamDecl*> &&templateParams);

   TypedefDecl(EmptyShell Empty);

   SourceLocation Loc;
   SourceType origin;
   ASTVector<TemplateParamDecl*> templateParams;

public:
   static TypedefDecl *Create(ASTContext &C,
                              AccessSpecifier access,
                              SourceLocation Loc,
                              DeclarationName Name,
                              SourceType origin,
                              ASTVector<TemplateParamDecl*> &&templateParams);

   static TypedefDecl *CreateEmpty(ASTContext &C);

   SourceLocation getSourceLoc() const { return Loc; }
   void setSourceLoc(SourceLocation V) { Loc = V; }

   SourceRange getSourceRange() const { return SourceRange(Loc); }

   const SourceType &getOriginTy() const { return origin; }
   void setOriginTy(SourceType Ty) { origin = Ty; }

   llvm::MutableArrayRef<TemplateParamDecl*> getTemplateParams()
   {
      return templateParams;
   }

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

   AliasDecl(EmptyShell, unsigned N);

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

   static AliasDecl *CreateEmpty(ASTContext &C, unsigned N);

   static bool classofKind(DeclKind kind) { return kind == AliasDeclID; }
   static bool classof(Decl const* T) { return classofKind(T->getKind()); }

   void Profile(llvm::FoldingSetNodeID &ID)
   {
      Profile(ID, instantiationInfo->specializedTemplate,
              *instantiationInfo->templateArgs);
   }

   static void Profile(llvm::FoldingSetNodeID &ID, AliasDecl *Template,
                       sema::FinalTemplateArgumentList &list) {
      ID.AddPointer(Template);
      list.Profile(ID);
   }

   using TrailingObjects::getTrailingObjects;
   friend TrailingObjects;

   SourceLocation getSourceLoc() const { return Loc; }
   void setSourceLoc(SourceLocation L) { Loc = L; }

   SourceRange getSourceRange() const;

   const SourceType &getType() const { return Type; }
   void setType(SourceType Ty) { Type = Ty; }

   StaticExpr* getAliasExpr() const { return aliasExpr; }
   void setAliasExpr(StaticExpr *Expr) { aliasExpr = Expr; }

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

   sema::FinalTemplateArgumentList &getTemplateArgs() const
   {
      assert(isInstantiation() && "not a record instantiation!");
      return *instantiationInfo->templateArgs;
   }

   SourceLocation getInstantiatedFrom() const
   {
      return instantiationInfo->instantiatedFrom;
   }

   AliasDecl* getSpecializedTemplate() const
   {
      return instantiationInfo->specializedTemplate;
   }
};

enum class ImplicitConformanceKind : unsigned char {
   StringRepresentable,
   Hashable,
   Equatable,
   Copyable,
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
              *instantiationInfo->templateArgs);
   }

   static void Profile(llvm::FoldingSetNodeID &ID, RecordDecl *Template,
                       sema::FinalTemplateArgumentList &list) {
      ID.AddPointer(Template);
      list.Profile(ID);
   }

   SourceLocation getKeywordLoc() const { return KeywordLoc; }
   void setKeywordLoc(SourceLocation Loc) { KeywordLoc = Loc; }

   SourceRange getBraceRange() const { return BraceRange; }
   SourceRange getSourceRange() const
   {
      return SourceRange(KeywordLoc, BraceRange.getEnd());
   }

   void setBraceRange(SourceRange BR) { BraceRange = BR; }

   AssociatedTypeDecl* getAssociatedType(DeclarationName name,
                                         ProtocolDecl *P = nullptr);

   MethodDecl *getConversionOperator(QualType toType);
   MethodDecl *getComparisonOperator(QualType withType);

   llvm::ArrayRef<ExtensionDecl*> getExtensions() const;
   void addExtension(ExtensionDecl *E) const;

   bool hasMethodWithName(DeclarationName name) const;
   bool hasMethodTemplate(DeclarationName name) const;

   PropDecl *getProperty(DeclarationName name);
   FieldDecl *getField(DeclarationName name);

   [[nodiscard]]
   DeclContext::AddDeclResultKind addDecl(NamedDecl *decl);

   template<class FnTy>
   typename
   std::enable_if<std::is_same_v<decltype((*(FnTy*)(0))(QualType())), bool>,
                  void>::type
   visitStoredTypes(const FnTy &F);

protected:
   RecordDecl(DeclKind typeID,
              AccessSpecifier access,
              SourceLocation KeywordLoc,
              DeclarationName Name,
              ASTVector<SourceType> &&conformanceTypes,
              ASTVector<TemplateParamDecl*> &&templateParams);

   RecordDecl(EmptyShell E, DeclKind typeID);

   SourceLocation KeywordLoc;
   SourceRange BraceRange;

   unsigned lastMethodID = 1;

   ASTVector<SourceType> conformanceTypes;
   ASTVector<TemplateParamDecl*> templateParams;

   InstantiationInfo<RecordDecl> *instantiationInfo = nullptr;

   DeinitDecl *deinitializer = nullptr;

   QualType RecordType;
   unsigned occupiedBytes = 0;
   unsigned short alignment = 1;

   bool manualAlignment : 1;
   bool opaque          : 1;
   bool implicitlyEquatable : 1;
   bool implicitlyHashable : 1;
   bool implicitlyCopyable : 1;
   bool implicitlyStringRepresentable : 1;
   bool NeedsRetainOrRelease : 1;

   MethodDecl *operatorEquals = nullptr;
   MethodDecl *hashCodeFn = nullptr;
   MethodDecl *toStringFn = nullptr;
   MethodDecl *copyFn = nullptr;

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

   ArrayRef<SourceType> getConformanceTypes() const { return conformanceTypes; }
   ASTVector<SourceType> &getConformanceTypes() { return conformanceTypes; }

   DeinitDecl *getDeinitializer() const { return deinitializer; }
   void setDeinitializer(DeinitDecl *D) { deinitializer = D; }

   void setInstantiationInfo(InstantiationInfo<RecordDecl> *II)
   {
      instantiationInfo = II;
   }

   InstantiationInfo<RecordDecl> *getInstantiationInfo() const
   {
      return instantiationInfo;
   }

   llvm::ArrayRef<TemplateParamDecl*> getTemplateParams() const
   {
      return templateParams;
   }

   ASTVector<TemplateParamDecl*> &getTemplateParams()
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

   sema::FinalTemplateArgumentList &getTemplateArgs() const
   {
      assert(isInstantiation() && "not a record instantiation!");
      return *instantiationInfo->templateArgs;
   }

   bool isNonUnionStruct() const;
   StructDecl *asNonUnionStruct() const;

   unsigned getSize() const { return occupiedBytes; }
   unsigned short getAlignment() const { return alignment; }

   unsigned getLastMethodID() const { return lastMethodID; }
   unsigned getAndIncrementLastMethodID() { return lastMethodID++; }
   void setLastMethodID(unsigned ID) { lastMethodID = ID; }

   QualType getType() const { return RecordType; }
   void setType(QualType V) { RecordType = V; }

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

   MethodDecl *getCopyFn() const { return copyFn; }
   void setCopyFn(MethodDecl *fn) { copyFn = fn; }

   bool isTriviallyCopyable() const {return declFlagSet(DF_TriviallyCopyable);}
   void setTriviallyCopyable(bool val)
   {
      setDeclFlag(DF_TriviallyCopyable, val);
   }

   int getNameSelector() const;

   bool isImplicitlyEquatable() const { return implicitlyEquatable; }
   void setImplicitlyEquatable(bool eq) { implicitlyEquatable = eq; }

   bool isImplicitlyHashable() const { return implicitlyHashable; }
   void setImplicitlyHashable(bool hash) { implicitlyHashable = hash; }

   bool isImplicitlyCopyable() const { return implicitlyCopyable; }
   void setImplicitlyCopyable(bool copyable) { implicitlyCopyable = copyable; }

   bool isImplicitlyStringRepresentable() const
   {
      return implicitlyStringRepresentable;
   }

   void setImplicitlyStringRepresentable(bool rep)
   {
      implicitlyStringRepresentable = rep;
   }

   void setNeedsRetainOrRelease(bool N) { NeedsRetainOrRelease = N; }
   bool needsRetainOrRelease() const { return NeedsRetainOrRelease; }
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

   static FieldDecl *CreateEmpty(ASTContext &C);

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

   FieldDecl(EmptyShell E);

   PropDecl *Accessor = nullptr;
   unsigned Offset = 0;

public:
   PropDecl *getAccessor() const { return Accessor; }
   void setAccessor(PropDecl *Accessor) { FieldDecl::Accessor = Accessor; }

   Expression* getDefaultVal() const { return value; }
   unsigned getOffset() const { return Offset; }
   void setOffset(unsigned OS) { Offset = OS; }

   // used as a predicate for filtered_decl_iterator
   bool isNotStatic() const { return !isStatic(); }
};

class StructDecl: public RecordDecl {
public:
   static StructDecl *Create(ASTContext &C,
                             AccessSpecifier access,
                             SourceLocation KeywordLoc,
                             DeclarationName Name,
                             ASTVector<SourceType> &&conformanceTypes,
                             ASTVector<TemplateParamDecl*> &&templateParams);

   static StructDecl *CreateEmpty(ASTContext &C);

   friend class ASTContext;

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

   StoredFieldVec &getStoredFields() { return StoredFields; }
   const StoredFieldVec &getFields() const { return StoredFields; }

   unsigned getNumNonStaticFields() const
   { return (unsigned)StoredFields.size(); }

   bool isSingleElementStruct() const { return StoredFields.size() == 1; }

   static bool classof(Decl const* T) { return classofKind(T->getKind()); }
   static bool classofKind(DeclKind kind)
   {
      switch (kind) {
      case StructDeclID:
      case ClassDeclID:
      case UnionDeclID:
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
              ASTVector<SourceType> &&conformanceTypes,
              ASTVector<TemplateParamDecl*> &&templateParams);

protected:
   StructDecl(DeclKind typeID,
              AccessSpecifier access,
              SourceLocation KeywordLoc,
              DeclarationName Name,
              ASTVector<SourceType> &&conformanceTypes,
              ASTVector<TemplateParamDecl*> &&templateParams);

   StructDecl(EmptyShell Empty);
   StructDecl(EmptyShell Empty, DeclKind typeID);

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
                            ASTVector<SourceType> &&conformanceTypes,
                            ASTVector<TemplateParamDecl*> &&templateParams,
                            SourceType parentClass,
                            bool isAbstract);

   static ClassDecl *CreateEmpty(ASTContext &C);

   const SourceType &getParentType() const { return parentType; }
   void setParentType(SourceType Ty) { parentType = Ty; }

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

   void setAbstract(bool b) { setDeclFlag(DF_Abstract, b); }

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
             ASTVector<SourceType> &&conformanceTypes,
             ASTVector<TemplateParamDecl*> &&templateParams,
             SourceType parentClass,
             bool isAbstract);

   ClassDecl(EmptyShell Empty);

   SourceType parentType;
   ClassDecl *parentClass = nullptr;
   unsigned NumVirtualFns = 0;
};

class EnumCaseDecl;

class EnumDecl: public RecordDecl {
public:
   static EnumDecl *Create(ASTContext &C,
                           AccessSpecifier access,
                           SourceLocation KeywordLoc,
                           DeclarationName Name,
                           ASTVector<SourceType> &&conformanceTypes,
                           ASTVector<TemplateParamDecl*> &&templateParams,
                           SourceType rawType);

   static EnumDecl *CreateEmpty(ASTContext &C);

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
            ASTVector<SourceType> &&conformanceTypes,
            ASTVector<TemplateParamDecl*> &&templateParams,
            SourceType rawType);

   EnumDecl(EmptyShell Empty);

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

   EnumCaseDecl *getCase(StringRef Name);

   EnumCaseDecl *getSomeCase() { return getCase("Some"); }
   EnumCaseDecl *getNoneCase() { return getCase("None"); }

   bool isUnpopulated() const { return Unpopulated; }

   void setMaxAssociatedTypes(size_t N) { maxAssociatedTypes = N; }
   void setUnpopulated(bool B) { Unpopulated = B; }
};

class UnionDecl: public StructDecl {
public:
   static UnionDecl *Create(ASTContext &C,
                            AccessSpecifier access,
                            SourceLocation KeywordLoc,
                            DeclarationName Name,
                            ASTVector<SourceType> &&conformanceTypes,
                            ASTVector<TemplateParamDecl*> &&templateParams);

   static UnionDecl *CreateEmpty(ASTContext &C);

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
   UnionDecl(AccessSpecifier access,
             SourceLocation KeywordLoc,
             DeclarationName Name,
             ASTVector<SourceType> &&conformanceTypes,
             ASTVector<TemplateParamDecl*> &&templateParams);

   UnionDecl(EmptyShell Empty);

   bool IsConst = false;
};

class ProtocolDecl: public RecordDecl {
public:
   static ProtocolDecl *Create(ASTContext &C,
                               AccessSpecifier access,
                               SourceLocation KeywordLoc,
                               DeclarationName Name,
                               ASTVector<SourceType> &&conformanceTypes,
                               ASTVector<TemplateParamDecl*>&&templateParams);

   static ProtocolDecl *CreateEmpty(ASTContext &C);

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

   bool isAny() const { return IsAny; }
   void setIsAny(bool IsAny) { ProtocolDecl::IsAny = IsAny; }

private:
   ProtocolDecl(AccessSpecifier access,
                SourceLocation KeywordLoc,
                DeclarationName Name,
                ASTVector<SourceType> &&conformanceTypes,
                ASTVector<TemplateParamDecl*> &&templateParams);

   ProtocolDecl(EmptyShell Empty);

   bool IsAny : 1;
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

   static ExtensionDecl *CreateEmpty(ASTContext &C, unsigned N);

   SourceLocation getExtLoc() const { return ExtLoc; }
   void setExtLoc(SourceLocation V) { ExtLoc = V; }

   SourceRange getBraceRange() const { return BraceRange; }
   void setBraceRange(SourceRange BR) { BraceRange = BR; }

   SourceRange getSourceRange() const
   {
      return SourceRange(ExtLoc, BraceRange.getEnd());
   }

   void setName(DeclarationName N) { Name = N; }

   const SourceType &getExtendedType() const { return ExtendedType; }
   void setExtendedType(SourceType V) { ExtendedType = V; }

   RecordDecl *getExtendedRecord() const { return ExtendedRecord; }
   void setExtendedRecord(RecordDecl *R) { ExtendedRecord = R; }

   llvm::ArrayRef<SourceType> getConformanceTypes() const
   {
      return { getTrailingObjects<SourceType>(), NumConformances };
   }

   llvm::MutableArrayRef<SourceType> getConformanceTypes()
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

   using TrailingObjects::getTrailingObjects;
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

   ExtensionDecl(EmptyShell, unsigned N);

   SourceLocation ExtLoc;
   SourceRange BraceRange;

   SourceType ExtendedType;
   RecordDecl *ExtendedRecord = nullptr;

   unsigned NumConformances;
};

class MethodDecl: public CallableDecl, public DefaultImplementable<MethodDecl> {
public:
   static MethodDecl *Create(ASTContext &C,
                             AccessSpecifier access,
                             SourceLocation DefLoc,
                             DeclarationName II,
                             SourceType returnType,
                             llvm::ArrayRef<FuncArgDecl*> args,
                             ASTVector<TemplateParamDecl*> &&templateParams,
                             Statement* body,
                             bool isStatic);

   static MethodDecl *CreateOperator(ASTContext &C,
                               AccessSpecifier access,
                               SourceLocation DefLoc,
                               DeclarationName OperatorName,
                               SourceType returnType,
                               llvm::ArrayRef<FuncArgDecl*> args,
                               ASTVector<TemplateParamDecl*> &&templateParams,
                               Statement* body,
                               bool isStatic);

   static MethodDecl *CreateConversionOp(ASTContext &C,
                                         AccessSpecifier access,
                                         SourceLocation DefLoc,
                                         SourceType returnType,
                                         llvm::ArrayRef<FuncArgDecl*> args,
                                         ASTVector<TemplateParamDecl*>
                                                               &&templateParams,
                                         Statement* body);

   static MethodDecl *CreateEmpty(ASTContext &C, unsigned N);

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

   QualType getSelfType() const { return SelfType; }
   void setSelfType(QualType SelfType) { MethodDecl::SelfType = SelfType; }

   void setAbstract(bool b) { setDeclFlag(DF_Abstract, b); }

private:
   MethodDecl(AccessSpecifier access,
              SourceLocation DefLoc,
              DeclarationName II,
              SourceType returnType,
              llvm::ArrayRef<FuncArgDecl*> args,
              ASTVector<TemplateParamDecl*> &&templateParams,
              Statement* body,
              bool isStatic);

   // the declaration name of a conversion operator is only known after the
   // first declaration pass
   MethodDecl(AccessSpecifier access,
              SourceLocation DefLoc,
              SourceType returnType,
              llvm::ArrayRef<FuncArgDecl*> args,
              ASTVector<TemplateParamDecl*> &&templateParams,
              Statement* body);

protected:
   MethodDecl(DeclKind typeID,
              AccessSpecifier access,
              SourceLocation Loc,
              DeclarationName Name,
              SourceType returnType,
              llvm::ArrayRef<FuncArgDecl*> args,
              ASTVector<TemplateParamDecl*> &&templateParams,
              Statement* body);

   MethodDecl(EmptyShell Empty, unsigned N);
   MethodDecl(EmptyShell Empty, DeclKind typeID, unsigned N);

   QualType SelfType;
   unsigned methodID = 0;

   SourceLocation BodyInstantiationLoc;
   MethodDecl *BodyTemplate = nullptr;

   MethodDecl *OverridenMethod = nullptr;

public:
   unsigned getMethodID() const { return methodID; }
   void setMethodID(unsigned ID) { methodID = ID; }

   SourceLocation getBodyInstantiationLoc()const{ return BodyInstantiationLoc; }
   void setBodyInstantiationLoc(SourceLocation Loc)
   {
      BodyInstantiationLoc = Loc;
   }

   MethodDecl *getBodyTemplate() const { return BodyTemplate; }
   void setBodyTemplate(MethodDecl *T) { BodyTemplate = T; }

   bool isVirtual() const { return getFlag(Virtual); }
   bool isOverride() const { return getFlag(Override); }
   bool isVirtualOrOverride() const { return isVirtual() || isOverride(); }
   bool isProperty() const { return getFlag(Property); }
   bool isSubscript() const { return getFlag(Subscript); }
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
   void setSubscript(bool sub) { setFlag(Subscript, sub); }
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
                           ASTVector<TemplateParamDecl*> &&templateParams,
                           Statement* body,
                           DeclarationName Name = DeclarationName(),
                           bool IsFallible = false);

   static InitDecl *CreateEmpty(ASTContext &C, unsigned N);

   ASTVector<TemplateParamDecl*> &getTemplateParamsRef()
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
            ASTVector<TemplateParamDecl*> &&templateParams,
            Statement* body,
            DeclarationName Name,
            bool IsFallible);

   InitDecl(EmptyShell Empty, unsigned N);

   ConstructorKind Kind    : 7;
   bool ExplicitMemberwise : 1;
   bool IsFallible         : 1;

   QualType OptionTy;

   union {
      InitDecl *BaseInit = nullptr;
      InitDecl *CompleteInit;
   };

public:
   bool isMemberwise() const { return ExplicitMemberwise; }

   ConstructorKind getCtorKind() const { return Kind; }
   void setCtorKind(ConstructorKind K) { Kind = K; }

   bool isBaseInitializer() const { return Kind == ConstructorKind::Base; }
   bool isCompleteInitializer() const{return Kind == ConstructorKind::Complete;}

   InitDecl *getBaseInit() const { return BaseInit; }
   void setBaseInit(InitDecl *BI) { BaseInit = BI; }

   InitDecl *getCompleteInit() const { return CompleteInit; }
   void setCompleteInit(InitDecl *CI) { CompleteInit = CI; }

   bool isFallible() const { return IsFallible; }
   QualType getOptionTy() const { return OptionTy; }
   void setOptionTy(QualType Ty) { OptionTy = Ty; }

   void setExplicitMemberwise(bool B) { ExplicitMemberwise = B; }
   void setIsFallible(bool B) { IsFallible = B; }
};

class DeinitDecl: public MethodDecl {
   explicit DeinitDecl(SourceLocation Loc,
                       Statement* body,
                       llvm::ArrayRef<FuncArgDecl*> args,
                       DeclarationName Name);

   DeinitDecl(EmptyShell Empty, unsigned N);

public:
   static DeinitDecl *Create(ASTContext &C,
                             SourceLocation Loc,
                             Statement* body,
                             llvm::ArrayRef<FuncArgDecl*> args,
                             DeclarationName Name = DeclarationName());

   static DeinitDecl *CreateEmpty(ASTContext &C, unsigned N);

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

class AssociatedTypeDecl: public NamedDecl,
                          public DefaultImplementable<AssociatedTypeDecl> {
public:
   static AssociatedTypeDecl *Create(ASTContext &C,
                                     SourceLocation Loc,
                                     IdentifierInfo *ProtoSpec,
                                     DeclarationName Name,
                                     SourceType actualType,
                                     bool Implementation);

   static AssociatedTypeDecl *CreateEmpty(ASTContext &C);

   SourceLocation getSourceLoc() const { return Loc; }
   void setSourceLoc(SourceLocation L) { Loc = L; }

   SourceRange getSourceRange() const { return SourceRange(Loc); }

   const SourceType &getActualType() const { return actualType; }
   void setActualType(SourceType ty) { actualType = ty; }

   IdentifierInfo *getProtoSpecInfo() const { return protocolSpecifier; }
   llvm::StringRef getProtocolSpecifier() const
   {
      return protocolSpecifier->getIdentifier();
   }

   void setProtocolSpecifier(IdentifierInfo *II) { protocolSpecifier = II; }

   bool isImplementation() const { return Implementation; }
   void setImplementation(bool V) { Implementation = V; }

   ProtocolDecl *getProto() const { return Proto; }
   void setProto(ProtocolDecl *P) { Proto = P; }

   static bool classofKind(DeclKind kind){ return kind == AssociatedTypeDeclID;}
   static bool classof(Decl const* T) { return classofKind(T->getKind()); }

private:
   AssociatedTypeDecl(SourceLocation Loc,
                      IdentifierInfo *ProtoSpec,
                      DeclarationName Name,
                      SourceType actualType,
                      bool Implementation);

   AssociatedTypeDecl(EmptyShell Empty);

   SourceLocation Loc;
   IdentifierInfo *protocolSpecifier;
   SourceType actualType;
   ProtocolDecl *Proto = nullptr;
   bool Implementation;
};

class PropDecl: public NamedDecl, public DefaultImplementable<PropDecl> {
   PropDecl(AccessSpecifier access,
            SourceRange Loc,
            DeclarationName Name,
            SourceType type,
            bool isStatic,
            bool hasGetter,
            bool hasSetter,
            AccessSpecifier GetterAccess,
            AccessSpecifier SetterAccess,
            CompoundStmt* getter,
            CompoundStmt* setter,
            IdentifierInfo *newValName);

   PropDecl(EmptyShell Empty);

   SourceRange Loc;
   SourceType type;

   AccessSpecifier GetterAccess;
   AccessSpecifier SetterAccess;

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
                           bool hasGetter,
                           bool hasSetter,
                           AccessSpecifier GetterAccess,
                           AccessSpecifier SetterAccess,
                           CompoundStmt* getter,
                           CompoundStmt* setter,
                           IdentifierInfo *newValName);

   static PropDecl *CreateEmpty(ASTContext &C);

   static bool classof(Decl const* T) { return classofKind(T->getKind()); }
   static bool classofKind(DeclKind kind) { return kind == PropDeclID; }

   SourceRange getSourceRange() const { return Loc; }
   const SourceType &getType() const { return type; }

   bool hasGetter() const { return HasGetter; }
   bool hasSetter() const { return HasSetter; }

   AccessSpecifier getGetterAccess() const { return GetterAccess; }
   AccessSpecifier getSetterAccess() const { return SetterAccess; }

   CompoundStmt* getGetterBody() const { return getterBody; }
   CompoundStmt* getSetterBody() const { return setterBody; }

   void setGetterBody(CompoundStmt *B) { getterBody = B; }
   void setSetterBody(CompoundStmt *B) { setterBody = B; }

   IdentifierInfo *getNewValNameInfo() const { return newValName; }
   llvm::StringRef getNewValName() const { return newValName->getIdentifier(); }

   void setLoc(const SourceRange &Loc) { PropDecl::Loc = Loc; }
   void setType(const SourceType &type) { PropDecl::type = type; }

   void setGetterAccess(AccessSpecifier Acc) { GetterAccess = Acc; }
   void setSetterAccess(AccessSpecifier Acc) { SetterAccess = Acc; }

   void setHasGetter(bool HasGetter) { PropDecl::HasGetter = HasGetter; }
   void setHasSetter(bool HasSetter) { PropDecl::HasSetter = HasSetter; }

   void setNewValName(IdentifierInfo *N) { newValName = N; }

   MethodDecl *getGetterMethod() const { return getterMethod; }
   void setGetterMethod(MethodDecl *M) { getterMethod = M; }

   MethodDecl *getSetterMethod() const { return setterMethod; }
   void setSetterMethod(MethodDecl *M) { setterMethod = M; }

   PropDecl *getPropTemplate() const { return PropTemplate; }
   void setPropTemplate(PropDecl *T) { PropTemplate = T; }
};

class SubscriptDecl final: public NamedDecl,
                           llvm::TrailingObjects<SubscriptDecl, FuncArgDecl*>,
                           public DefaultImplementable<SubscriptDecl> {
   SubscriptDecl(AccessSpecifier access,
                 SourceRange Loc,
                 DeclarationName Name,
                 llvm::ArrayRef<FuncArgDecl*> Args,
                 SourceType type,
                 bool hasGetter,
                 bool hasSetter,
                 AccessSpecifier GetterAccess,
                 AccessSpecifier SetterAccess,
                 CompoundStmt* getter,
                 CompoundStmt* setter,
                 IdentifierInfo *newValName);

   SubscriptDecl(EmptyShell, unsigned N);

   SourceRange Loc;
   SourceType type;

   AccessSpecifier GetterAccess;
   AccessSpecifier SetterAccess;

   CompoundStmt* getterBody = nullptr;
   CompoundStmt* setterBody = nullptr;

   bool HasGetter = false;
   bool HasSetter = false;

   IdentifierInfo *newValName;

   MethodDecl *getterMethod = nullptr;
   MethodDecl *setterMethod = nullptr;

   unsigned NumArgs;

   SubscriptDecl *Template = nullptr;

public:
   static SubscriptDecl *Create(ASTContext &C,
                                AccessSpecifier access,
                                SourceRange Loc,
                                llvm::ArrayRef<FuncArgDecl*> Args,
                                SourceType type,
                                bool hasGetter,
                                bool hasSetter,
                                AccessSpecifier GetterAccess,
                                AccessSpecifier SetterAccess,
                                CompoundStmt* getter,
                                CompoundStmt* setter,
                                IdentifierInfo *newValName);

   static SubscriptDecl *CreateEmpty(ASTContext &C, unsigned N);

   static bool classof(Decl const* T) { return classofKind(T->getKind()); }
   static bool classofKind(DeclKind kind) { return kind == SubscriptDeclID; }

   using TrailingObjects::getTrailingObjects;
   friend TrailingObjects;

   SourceRange getSourceRange() const { return Loc; }

   const SourceType &getType() const { return type; }

   bool hasGetter() const { return HasGetter; }
   bool hasSetter() const { return HasSetter; }

   AccessSpecifier getGetterAccess() const { return GetterAccess; }
   AccessSpecifier getSetterAccess() const { return SetterAccess; }

   CompoundStmt* getGetterBody() const { return getterBody; }
   CompoundStmt* getSetterBody() const { return setterBody; }

   void setGetterBody(CompoundStmt *B) { getterBody = B; }
   void setSetterBody(CompoundStmt *B) { setterBody = B; }

   IdentifierInfo *getNewValNameInfo() const { return newValName; }
   llvm::StringRef getNewValName() const { return newValName->getIdentifier(); }

   void setLoc(const SourceRange &Loc) { this->Loc = Loc; }
   void setType(const SourceType &type) { this->type = type; }

   void setGetterAccess(AccessSpecifier Acc) { GetterAccess = Acc; }
   void setSetterAccess(AccessSpecifier Acc) { SetterAccess = Acc; }

   void setHasGetter(bool HasGetter) { this->HasGetter = HasGetter; }
   void setHasSetter(bool HasSetter) { this->HasSetter = HasSetter; }

   void setNewValName(IdentifierInfo *N) { newValName = N; }

   MethodDecl *getGetterMethod() const { return getterMethod; }
   void setGetterMethod(MethodDecl *M) { getterMethod = M; }

   MethodDecl *getSetterMethod() const { return setterMethod; }
   void setSetterMethod(MethodDecl *M) { setterMethod = M; }

   SubscriptDecl *getTemplate() const { return Template; }
   void setTemplate(SubscriptDecl *T) { Template = T; }

   llvm::MutableArrayRef<FuncArgDecl*> getArgs()
   {
      return { getTrailingObjects<FuncArgDecl*>(), NumArgs };
   }
};

class EnumCaseDecl: public CallableDecl {
   EnumCaseDecl(AccessSpecifier AS,
                SourceLocation CaseLoc,
                SourceLocation IdentLoc,
                DeclarationName Name,
                StaticExpr* rawValue,
                llvm::ArrayRef<FuncArgDecl*> args);

   EnumCaseDecl(EmptyShell Empty, unsigned N);

   SourceLocation CaseLoc;
   SourceLocation IdentLoc;

   StaticExpr* rawValExpr;
   long long rawValue = 0;

   unsigned size = 0;
   unsigned short alignment = 1;

   il::Constant *ILValue = nullptr;

public:
   static EnumCaseDecl *Create(ASTContext &C,
                               AccessSpecifier AS,
                               SourceLocation CaseLoc,
                               SourceLocation IdentLoc,
                               DeclarationName Name,
                               StaticExpr* rawValue,
                               llvm::ArrayRef<FuncArgDecl*> args);

   static EnumCaseDecl *CreateEmpty(ASTContext &C, unsigned N);

   static bool classofKind(DeclKind kind) { return kind == EnumCaseDeclID; }
   static bool classof(Decl const* T) { return classofKind(T->getKind()); }

   SourceLocation getCaseLoc() const { return CaseLoc; }
   SourceLocation getIdentLoc() const { return IdentLoc; }
   SourceRange getSourceRange() const
   {
      return SourceRange(CaseLoc ? CaseLoc : IdentLoc, IdentLoc);
   }

   void setCaseLoc(SourceLocation Loc) { CaseLoc = Loc; }
   void setIdentLoc(SourceLocation Loc) { IdentLoc = Loc; }

   StaticExpr* getRawValExpr() const { return rawValExpr; }
   void setRawValExpr(StaticExpr *expr) { rawValExpr = expr; }

   long long getRawValue() const { return rawValue; }
   void setRawValue(long val) { rawValue = val; }

   il::Constant *getILValue() const { return ILValue; }
   void setILValue(il::Constant *V) { ILValue = V; }

   bool isIndirect() const { return declFlagSet(DF_IndirectCase); }
   void setIndirect(bool I) { setDeclFlag(DF_IndirectCase, I); }

   unsigned int getSize() const { return size; }
   void setSize(unsigned int size) { EnumCaseDecl::size = size; }

   unsigned short getAlignment() const { return alignment; }
   void setAlignment(unsigned short align) { alignment = align; }

   ASTVector<TemplateParamDecl*> &getTemplateParamsRef()
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

   StaticIfDecl(SourceLocation StaticLoc,
                SourceLocation RBRaceLoc,
                StaticExpr *condition,
                StaticIfDecl *Template);

   StaticIfDecl(EmptyShell Empty);

   SourceLocation StaticLoc;
   SourceLocation RBRaceLoc;

   StaticExpr *condition;
   CompoundDecl *IfDecl;
   CompoundDecl *ElseDecl;

   StaticIfDecl *Template;

public:
   static StaticIfDecl *Create(ASTContext &C,
                               SourceLocation StaticLoc,
                               SourceLocation RBRaceLoc,
                               StaticExpr *condition,
                               CompoundDecl *IfDecl,
                               CompoundDecl *ElseDecl);

   static StaticIfDecl *Create(ASTContext &C,
                               SourceLocation StaticLoc,
                               SourceLocation RBRaceLoc,
                               StaticExpr *condition,
                               StaticIfDecl *Template);

   static StaticIfDecl *CreateEmpty(ASTContext &C);

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

   void setStaticLoc(SourceLocation Loc) { StaticLoc = Loc; }
   void setRBRaceLoc(SourceLocation Loc) { RBRaceLoc = Loc; }

   StaticIfDecl *getTemplate() const { return Template; }
};

class StaticForDecl: public Decl {
   StaticForDecl(SourceLocation StaticLoc,
                 SourceLocation RBRaceLoc,
                 IdentifierInfo *elementName,
                 StaticExpr *range,
                 CompoundDecl *BodyDecl);

   StaticForDecl(EmptyShell Empty);

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

   static StaticForDecl *CreateEmpty(ASTContext &C);

   static bool classofKind(DeclKind kind) { return kind == StaticForDeclID; }
   static bool classof(Decl const *T) { return classofKind(T->getKind()); }

   SourceRange getSourceRange() const
   {
      return SourceRange(StaticLoc, RBRaceLoc);
   }

   IdentifierInfo *getElementName() const { return elementName; }
   void setElementName(IdentifierInfo *II) { elementName = II; }

   StaticExpr *getRange() const { return range; }
   void setRange(StaticExpr *S) { range = S; }

   CompoundDecl *getBodyDecl() const { return BodyDecl; }
   void setBodyDecl(CompoundDecl *D) { BodyDecl = D; }

   void setStaticLoc(SourceLocation Loc) { StaticLoc = Loc; }
   void setRBRaceLoc(SourceLocation Loc) { RBRaceLoc = Loc; }

   SourceLocation getStaticLoc() const { return StaticLoc; }
   SourceLocation getRBRaceLoc() const { return RBRaceLoc; }
};

class StaticAssertStmt: public Decl {
   StaticAssertStmt(SourceLocation Loc,
                    SourceRange Parens,
                    StaticExpr* expr,
                    StringRef message);

   StaticAssertStmt(EmptyShell Empty);

   SourceLocation Loc;
   SourceRange Parens;
   StaticExpr* expr;
   StringRef message;

public:
   static StaticAssertStmt *Create(ASTContext &C,
                                   SourceLocation Loc,
                                   SourceRange Parens,
                                   StaticExpr* expr,
                                   StringRef message);

   static StaticAssertStmt *CreateEmpty(ASTContext &C);

   static bool classofKind(DeclKind kind) { return kind == StaticAssertStmtID; }
   static bool classof(Decl const *T) { return classofKind(T->getKind()); }

   SourceLocation getStaticAssertLoc() const { return Loc; }
   SourceRange getParenRange() const { return Parens; }
   SourceRange getSourceRange() const
   {
      return SourceRange(Loc, Parens.getEnd());
   }

   void setLoc(SourceLocation Loc) { StaticAssertStmt::Loc = Loc; }
   void setParens(SourceRange Parens) { StaticAssertStmt::Parens = Parens; }
   void setMessage(StringRef msg) { message = msg; }

   StaticExpr* getExpr() const { return expr; }
   void setExpr(StaticExpr *E) { expr = E; }
   StringRef getMessage() const { return message; }
};

class StaticPrintStmt: public Decl {
   StaticPrintStmt(SourceLocation Loc,
                   SourceRange Parens,
                   Expression* expr);

   StaticPrintStmt(EmptyShell Empty);

   SourceLocation Loc;
   SourceRange Parens;
   Expression* expr;

public:
   static StaticPrintStmt *Create(ASTContext &C,
                                  SourceLocation Loc,
                                  SourceRange Parens,
                                  Expression *E);

   static StaticPrintStmt *CreateEmpty(ASTContext &C);

   static bool classofKind(DeclKind kind) { return kind == StaticPrintStmtID; }
   static bool classof(Decl const *T) { return classofKind(T->getKind()); }

   SourceLocation getStaticPrintLoc() const { return Loc; }
   SourceRange getParenRange() const { return Parens; }
   SourceRange getSourceRange() const
   {
      return SourceRange(Loc, Parens.getEnd());
   }

   void setLoc(SourceLocation Loc) { this->Loc = Loc; }
   void setParens(SourceRange Parens) { this->Parens = Parens; }

   Expression* getExpr() const { return expr; }
   void setExpr(Expression *E) { expr = E; }
};

class MixinDecl: public Decl {
public:
   static MixinDecl *Create(ASTContext &C,
                            SourceLocation Loc,
                            SourceRange Parens,
                            Expression *MixinExpr);

   static MixinDecl *CreateEmpty(ASTContext &C);

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

   void setLoc(SourceLocation Loc) { this->Loc = Loc; }
   void setParens(SourceRange Parens) { this->Parens = Parens; }

private:
   MixinDecl(SourceLocation Loc,
             SourceRange Parens,
             Expression *MixinExpr);

   MixinDecl(EmptyShell Empty);

   SourceLocation Loc;
   SourceRange Parens;
   Expression *MixinExpr;
};

class PrecedenceGroupDecl: public NamedDecl {
   PrecedenceGroupDecl(SourceRange SR,
                       AccessSpecifier AS,
                       DeclarationName DN,
                       Associativity Assoc,
                       const IdentifierInfo *HigherThanIdent,
                       const IdentifierInfo *LowerThanIdent,
                       bool IsAssignment);

   PrecedenceGroupDecl(EmptyShell Empty);

   SourceRange SR;
   Associativity Assoc : 7;
   bool IsAssignment : 1;

   const IdentifierInfo *HigherThanIdent;
   PrecedenceGroupDecl *HigherThan = nullptr;

   const IdentifierInfo *LowerThanIdent;
   PrecedenceGroupDecl *LowerThan = nullptr;

public:
   static PrecedenceGroupDecl *Create(ASTContext &C,
                                      SourceRange SR,
                                      AccessSpecifier AS,
                                      DeclarationName DN,
                                      Associativity Assoc,
                                      const IdentifierInfo *HigherThanIdent,
                                      const IdentifierInfo *LowerThanIdent,
                                      bool IsAssignment);

   static PrecedenceGroupDecl *CreateEmpty(ASTContext &C);

   static bool classofKind(DeclKind kind){return kind == PrecedenceGroupDeclID;}
   static bool classof(Decl const *T) { return classofKind(T->getKind()); }

   SourceRange getSourceRange() const { return SR; }
   void setSourceRange(SourceRange V) { SR = V; }

   Associativity getAssociativity() const { return Assoc; }
   const IdentifierInfo *getHigherThanIdent() const { return HigherThanIdent; }
   const IdentifierInfo *getLowerThanIdent() const { return LowerThanIdent; }

   void setAssoc(Associativity Assoc) { PrecedenceGroupDecl::Assoc = Assoc; }
   void setIsAssignment(bool B) { IsAssignment = B; }

   void setHigherThanIdent(const IdentifierInfo *ID) { HigherThanIdent = ID; }
   void setLowerThanIdent(const IdentifierInfo *ID) { LowerThanIdent = ID; }

   PrecedenceGroupDecl *getHigherThan() const { return HigherThan; }
   void setHigherThan(PrecedenceGroupDecl *HT) { HigherThan = HT; }

   PrecedenceGroupDecl *getLowerThan() const { return LowerThan; }
   void setLowerThan(PrecedenceGroupDecl *LT) { LowerThan = LT; }

   bool isAssignment() const { return IsAssignment; }

   enum Relationship {
      Equal, Higher, Lower, Undefined,
   };

   Relationship getRelationTo(PrecedenceGroupDecl *Other,
                              bool CheckReverse = true);
};

class OperatorDecl: public NamedDecl {
   OperatorDecl(SourceRange SR,
                AccessSpecifier AS,
                DeclarationName Name,
                const IdentifierInfo *PGIdent);

   OperatorDecl(EmptyShell Empty);

   SourceRange SR;
   DeclarationName OperatorName;
   const IdentifierInfo *PrecedenceGroupIdent;
   PrecedenceGroupDecl *PrecedenceGroup = nullptr;

public:
   static OperatorDecl *Create(ASTContext &C,
                               SourceRange SR,
                               AccessSpecifier AS,
                               DeclarationName OperatorName,
                               const IdentifierInfo *PGIdent);

   static OperatorDecl *CreateEmpty(ASTContext &C);

   static bool classofKind(DeclKind kind){return kind == OperatorDeclID;}
   static bool classof(Decl const *T) { return classofKind(T->getKind()); }

   SourceRange getSourceRange() const { return SR; }
   void setSourceRange(SourceRange V) { SR = V; }

   DeclarationName getOperatorName() const { return OperatorName; }
   const IdentifierInfo *getPrecedenceGroupIdent() const
   {
      return PrecedenceGroupIdent;
   }

   PrecedenceGroupDecl *getPrecedenceGroup() const { return PrecedenceGroup; }
   void setPrecedenceGroup(PrecedenceGroupDecl *PG) { PrecedenceGroup = PG; }

   void setOperatorName(DeclarationName N) { OperatorName = N; }
   void setPrecedenceGroupIdent(const IdentifierInfo *ID)
   {
      PrecedenceGroupIdent = ID;
   }
};

class PatternFragment;

struct StateTransition {
   StateTransition(const lex::Token &Tok, PatternFragment *Next)
      : Tok(Tok), Next(Next)
   { }

   StateTransition() = default;

   operator bool() const
   {
      return Tok.getKind() != lex::tok::sentinel || Next != nullptr;
   }

   bool isUnconditional() const
   {
      return Tok.getKind() == lex::tok::sentinel && Next != nullptr;
   }

   bool isEOF() const
   {
      return Tok.getKind() == lex::tok::eof;
   }

   lex::Token Tok;
   PatternFragment *Next = nullptr;
};

class PatternFragment {
public:
   enum FragmentKind {
      Empty, Error, Tokens, Star, Plus, Question, Variable,
      Expansion,
   };

   enum VariableKind {
      Any, Expr, Stmt, Decl, Type, Ident, Tok,
   };

private:
   PatternFragment(FragmentKind Kind);

   PatternFragment(SourceLocation Loc,
                   VariableKind VarKind,
                   const IdentifierInfo *VarName);

   PatternFragment(SourceLocation Loc,
                   FragmentKind RepetitionKind,
                   PatternFragment *BeginState,
                   PatternFragment *EndState,
                   const lex::Token &Delimiter);

   PatternFragment(SourceLocation Loc, llvm::ArrayRef<lex::Token> Tokens);

   FragmentKind Kind    : 7;
   bool HasDelimiterTok : 1;

   SourceLocation Loc;

   struct TokenData {
      unsigned NumTokens;
   };

   struct VariableData {
      VariableKind Kind;
      const IdentifierInfo *VarName;
   };

   struct RepetitionData {
      PatternFragment *BeginState = nullptr;
      PatternFragment *EndState = nullptr;
   };

   union {
      TokenData TokData;
      VariableData VarData;
      RepetitionData RepData;
   };

   StateTransition Transitions[2];

   static PatternFragment ErrorState;

public:
   static PatternFragment *Create(ASTContext &C);

   static PatternFragment *Create(ASTContext &C,
                                  SourceLocation Loc,
                                  VariableKind VarKind,
                                  const IdentifierInfo *VarName);

   static PatternFragment *Create(ASTContext &C,
                                  SourceLocation Loc,
                                  FragmentKind RepetitionKind,
                                  PatternFragment *BeginState,
                                  PatternFragment *EndState,
                                  const lex::Token &Delimiter);

   static PatternFragment *Create(ASTContext &C,
                                  SourceLocation Loc,
                                  llvm::ArrayRef<lex::Token> Tokens);

   static PatternFragment *GetErrorState()
   {
      return &ErrorState;
   }

   PatternFragment(const PatternFragment&) = delete;
   PatternFragment(PatternFragment&&)      = delete;

   PatternFragment &operator=(const PatternFragment&) = delete;
   PatternFragment &operator=(PatternFragment&&)      = delete;

   FragmentKind getKind() const { return Kind; }
   SourceLocation getLoc() const { return Loc; }

   llvm::ArrayRef<StateTransition> getTransitions() const
   {
      return Transitions;
   }

   llvm::MutableArrayRef<StateTransition> getTransitions()
   {
      return Transitions;
   }

   bool isEndState() const
   {
      return !Transitions[0] && !Transitions[1];
   }

   bool isErrorState() const { return Kind == Error; }

   bool isToken() const { return Kind == Tokens; }
   llvm::ArrayRef<lex::Token> getTokens() const
   {
      assert(isToken());
      return { reinterpret_cast<lex::Token const*>(this + 1),
         TokData.NumTokens };
   }

   bool isVariable() const { return Kind == Variable; }
   VariableKind getVarKind() const { assert(isVariable()); return VarData.Kind;}
   const IdentifierInfo *getVariableName() const
   {
      assert(isVariable());
      return VarData.VarName;
   }

   bool isRepetition() const
   {
      switch (Kind) {
      case Star: case Plus: case Question: case Expansion:
         return true;
      default:
         return false;
      }
   }

   bool hasDelimiterTok() const
   {
      assert(isRepetition());
      return HasDelimiterTok;
   }

   const lex::Token &getDelimiter() const
   {
      assert(hasDelimiterTok());
      return *(reinterpret_cast<lex::Token const*>(this + 1));
   }

   PatternFragment *getRepetitionBeginState() const
   {
      assert(isRepetition());
      return RepData.BeginState;
   }

   PatternFragment *getRepetitionEndState() const
   {
      assert(isRepetition());
      return RepData.EndState;
   }

   PatternFragment *&getRepetitionBeginState()
   {
      assert(isRepetition());
      return RepData.BeginState;
   }

   PatternFragment *&getRepetitionEndState()
   {
      assert(isRepetition());
      return RepData.EndState;
   }

   void addTransition(const lex::Token &Tok, PatternFragment *Next)
   {
      if (!Transitions[0]) {
         Transitions[0] = StateTransition(Tok, Next);
      }
      else {
         assert(!Transitions[1] && "more than 2 transitions needed!");
         Transitions[1] = StateTransition(Tok, Next);
      }
   }
};

class ExpansionFragment {
public:
   enum FragmentKind {
      Tokens, Variable, Expansion, ConcatExpr,
   };

private:
   ExpansionFragment(SourceLocation Loc,
                     const IdentifierInfo *VarName);

   ExpansionFragment(SourceLocation Loc,
                     llvm::ArrayRef<ExpansionFragment*> Fragments,
                     const IdentifierInfo *ExpandedVariable);

   ExpansionFragment(SourceLocation Loc,
                     llvm::ArrayRef<ExpansionFragment*> Fragments);

   explicit ExpansionFragment(SourceLocation Loc,
                              llvm::ArrayRef<lex::Token> Tokens);

   FragmentKind Kind;
   SourceLocation Loc;

   struct TokenData {
      unsigned NumTokens;
   };

   struct VariableData {
      const IdentifierInfo *VarName;
   };

   struct RepetitionData {
      unsigned NumFragments;
      const IdentifierInfo *ExpandedVariable;
   };

   union {
      TokenData TokData;
      VariableData VarData;
      RepetitionData RepData;
   };

public:
   static ExpansionFragment *Create(ASTContext &C,
                                    SourceLocation Loc,
                                    const IdentifierInfo *VarNam);

   static ExpansionFragment *Create(ASTContext &C,
                                    SourceLocation Loc,
                                   llvm::ArrayRef<ExpansionFragment*> Fragments,
                                    const IdentifierInfo *ExpandedVariable);

   static ExpansionFragment *Create(ASTContext &C,
                                    SourceLocation Loc,
                                    llvm::ArrayRef<ExpansionFragment*> Fragments);

   static ExpansionFragment *Create(ASTContext &C,
                                    SourceLocation Loc,
                                    llvm::ArrayRef<lex::Token> Tokens);

   ExpansionFragment(const ExpansionFragment&) = delete;
   ExpansionFragment(ExpansionFragment&&)      = delete;

   ExpansionFragment &operator=(const ExpansionFragment&) = delete;
   ExpansionFragment &operator=(ExpansionFragment&&)      = delete;

   FragmentKind getKind() const { return Kind; }
   SourceLocation getLoc() const { return Loc; }

   bool isToken() const { return Kind == Tokens; }
   llvm::ArrayRef<lex::Token> getTokens() const
   {
      assert(isToken());
      return { reinterpret_cast<lex::Token const*>(this + 1),
         TokData.NumTokens };
   }

   bool isVariable() const { return Kind == Variable; }
   const IdentifierInfo *getVariableName() const
   {
      assert(isVariable());
      return VarData.VarName;
   }

   bool isConcatExpr() const { return Kind == ConcatExpr; }
   bool isRepetition() const { return !isToken() && !isVariable(); }

   llvm::ArrayRef<ExpansionFragment*> getRepetitionFragments() const
   {
      assert(isRepetition() || isConcatExpr());
      return { reinterpret_cast<ExpansionFragment* const*>(this + 1),
         RepData.NumFragments };
   }

   llvm::MutableArrayRef<ExpansionFragment*> getRepetitionFragments()
   {
      assert(isRepetition() || isConcatExpr());
      return { reinterpret_cast<ExpansionFragment**>(this + 1),
         RepData.NumFragments };
   }

   const IdentifierInfo *getExpandedVariable() const
   {
      assert(isRepetition());
      return RepData.ExpandedVariable;
   }
};

class MacroPattern final: llvm::TrailingObjects<MacroPattern,
                                                ExpansionFragment*> {
   MacroPattern(SourceLocation Loc,
                PatternFragment* Pattern,
                llvm::ArrayRef<ExpansionFragment*> Expansion,
                unsigned SourceLength);

   SourceLocation Loc;
   PatternFragment* Pattern;

   /// Length of the parsed expansion pattern in bytes.
   unsigned SourceLength;
   unsigned NumExpansionFragments;

public:
   static MacroPattern *Create(ASTContext &C,
                               SourceLocation Loc,
                               PatternFragment* Pattern,
                               llvm::ArrayRef<ExpansionFragment*> Expansion,
                               unsigned SourceLength);

   SourceLocation getSourceLoc() const { return Loc; }
   SourceRange getSourceRange() const;

   PatternFragment* getPattern() const { return Pattern; }
   llvm::ArrayRef<ExpansionFragment*> getExpansion() const
   {
      return { getTrailingObjects<ExpansionFragment*>(),
         NumExpansionFragments };
   }

   unsigned getSourceLength() const { return SourceLength; }
   void setSourceLength(unsigned V) { SourceLength = V; }

   void setLoc(const SourceLocation &Loc) { MacroPattern::Loc = Loc; }
   void setPattern(PatternFragment *Pat) { Pattern = Pat; }

   friend TrailingObjects;
};

class MacroDecl final: public NamedDecl,
                       llvm::TrailingObjects<MacroDecl, MacroPattern*> {
public:
   enum Delimiter {
      Brace, Square, Paren,
   };

   friend TrailingObjects;

private:
   MacroDecl(SourceRange SR,
             AccessSpecifier AS,
             DeclarationName Name,
             Delimiter Delim,
             llvm::ArrayRef<MacroPattern*> Patterns);

   MacroDecl(EmptyShell, unsigned N);

   SourceRange SR;
   Delimiter Delim;
   unsigned NumPatterns;

public:
   static MacroDecl *Create(ASTContext &C,
                            SourceRange SR,
                            AccessSpecifier AS,
                            DeclarationName Name,
                            Delimiter Delim,
                            llvm::ArrayRef<MacroPattern*> Patterns);

   static MacroDecl *CreateEmpty(ASTContext &C, unsigned N);

   SourceRange getSourceRange() const { return SR; }
   Delimiter getDelim() const { return Delim; }

   void setSourceRange(const SourceRange &SR) { MacroDecl::SR = SR; }
   void setDelim(Delimiter Delim) { MacroDecl::Delim = Delim; }

   unsigned int getNumPatterns() const { return NumPatterns; }

   llvm::ArrayRef<MacroPattern*> getPatterns() const
   {
      return { getTrailingObjects<MacroPattern*>(), NumPatterns };
   }

   using TrailingObjects::getTrailingObjects;
};

class MacroExpansionDecl final:
   public Decl,
   llvm::TrailingObjects<MacroExpansionDecl, lex::Token>{
public:
   enum Delimiter {
      Brace, Square, Paren,
   };

   friend TrailingObjects;

private:
   MacroExpansionDecl(SourceRange SR,
                      DeclarationName MacroName,
                      Delimiter Delim,
                      llvm::ArrayRef<lex::Token> Toks);

   MacroExpansionDecl(EmptyShell, unsigned N);

   SourceRange SR;
   Delimiter Delim;
   DeclarationName MacroName;
   unsigned NumTokens;

public:
   static MacroExpansionDecl *Create(ASTContext &C,
                                     SourceRange SR,
                                     DeclarationName MacroName,
                                     Delimiter Delim,
                                     llvm::ArrayRef<lex::Token> Toks);

   static MacroExpansionDecl *CreateEmpty(ASTContext &C, unsigned N);

   static bool classofKind(DeclKind kind) {return kind == MacroExpansionDeclID;}
   static bool classof(Decl const *T) { return classofKind(T->getKind()); }

   SourceRange getSourceRange() const { return SR; }
   DeclarationName getMacroName() const { return MacroName; }
   Delimiter getDelim() const { return Delim; }

   void setSourceRange(SourceRange R) { SR = R; }
   void setDelim(Delimiter Delim) { MacroExpansionDecl::Delim = Delim; }
   void setMacroName(DeclarationName N) { MacroName = N; }

   llvm::ArrayRef<lex::Token> getTokens() const
   {
      return { getTrailingObjects<lex::Token>(), NumTokens };
   }

   using TrailingObjects::getTrailingObjects;
};

class UnittestDecl: public Decl, public DeclContext {
   UnittestDecl(SourceLocation KeywordLoc,
                SourceRange BraceRange,
                IdentifierInfo *Name,
                Statement *Body);

   UnittestDecl();

   SourceLocation KeywordLoc;
   SourceRange BraceRange;

   IdentifierInfo *Name;
   Statement *Body;

   ClassDecl *TestClass = nullptr;

public:
   static bool classofKind(DeclKind kind) { return kind == UnittestDeclID; }
   static bool classof(Decl const *T) { return classofKind(T->getKind()); }

   static UnittestDecl *Create(ASTContext &C,
                               SourceLocation KeywordLoc,
                               SourceRange BraceRange,
                               IdentifierInfo *Name,
                               Statement *Body);

   static UnittestDecl *CreateEmpty(ASTContext &C);

   SourceRange getSourceRange() const;

   SourceLocation getKeywordLoc() const { return KeywordLoc; }
   void setKeywordLoc(SourceLocation V) { KeywordLoc = V; }

   SourceRange getBraceRange() const { return BraceRange; }
   void setBraceRange(SourceRange V) { BraceRange = V; }

   IdentifierInfo* getName() const { return Name; }
   void setName(IdentifierInfo* V) { Name = V; }

   Statement* getBody() const { return Body; }
   void setBody(Statement* V) { Body = V; }

   ClassDecl* getTestClass() const { return TestClass; }
   void setTestClass(ClassDecl* V) { TestClass = V; }
};

class DebugDecl: public Decl {
   DebugDecl(SourceLocation Loc);

   SourceLocation Loc;

public:
   static bool classofKind(DeclKind kind) { return kind == DebugDeclID; }
   static bool classof(Decl const *T) { return classofKind(T->getKind()); }

   static DebugDecl *Create(ASTContext &C, SourceLocation Loc);
   static DebugDecl *CreateEmpty(ASTContext &C);

   SourceRange getSourceRange() const;

   SourceLocation getLoc() const { return Loc; }
   void setLoc(SourceLocation V) { Loc = V; }
};

template<class FnTy>
typename
std::enable_if<std::is_same_v<decltype((*(FnTy*)(0))(QualType())), bool>,
   void>::type
RecordDecl::visitStoredTypes(const FnTy &Fn)
{
   if (auto S = support::dyn_cast<StructDecl>(this)) {
      for (auto &F : S->getFields()) {
         if (!Fn(F->getType().getResolvedType()))
            break;
      }
   }
   else if (auto E = support::dyn_cast<EnumDecl>(this)) {
      for (auto Case : E->getCases()) {
         if (Case->isIndirect())
            continue;

         for (auto &Val : Case->getArgs()) {
            if (!Fn(Val->getType().getResolvedType()))
               break;
         }
      }
   }
}

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
