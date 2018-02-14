//
// Created by Jonas Zell on 22.01.18.
//

#ifndef CDOT_NAMEDDECL_H
#define CDOT_NAMEDDECL_H

#include "AST/Expression/Expression.h"
#include "AST/Passes/SemanticAnalysis/Template.h"

#include <set>
#include <llvm/ADT/PointerUnion.h>

namespace cdot {

enum class AccessModifier : unsigned char {
   DEFAULT = 0,
   PUBLIC,
   PRIVATE,
   PROTECTED
};

namespace ast {

class DeclContext;
class RecordDecl;
class NamespaceDecl;
class TypeRef;
class StaticExpr;
class TemplateParamDecl;

using TemplateParamVec = std::vector<TemplateParamDecl*>;

template<class T>
struct InstantiationInfo {
   InstantiationInfo(const SourceLocation &instantiatedFrom,
                     sema::TemplateArgList &&templateArgs,
                     T *specializedTemplate)
      : instantiatedFrom(instantiatedFrom),
        templateArgs(std::move(templateArgs)),
        specializedTemplate(specializedTemplate)
   { }

   InstantiationInfo() = default;

   SourceLocation instantiatedFrom;
   sema::TemplateArgList templateArgs;
   T *specializedTemplate = nullptr;
};

enum class ExternKind : unsigned char {
   None,
   C,
   CXX,
   Native
};

class NamedDecl: public Statement {
public:
   enum DeclFlags : unsigned {
      DF_None          = 0,
      DF_Static        = 1,
      DF_Mutating      = DF_Static << 1,
      DF_Const         = DF_Mutating << 1,
      DF_HasDefinition = DF_Const << 1,
      DF_External      = DF_HasDefinition << 1,
   };

   bool isStatic() const
   {
      return declFlagSet(DF_Static);
   }

   bool hasMutableSelf() const
   {
      return declFlagSet(DF_Mutating);
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

   AccessModifier getAccess() const
   {
      return access;
   }

   void setAccess(AccessModifier access)
   {
      NamedDecl::access = access;
   }

   const std::string &getName() const
   {
      return name;
   }

   void setName(std::string &&name)
   {
      NamedDecl::name = move(name);
   }

   RecordDecl *getRecord() const;

   DeclContext *getDeclContext() const
   {
      return declContext;
   }

   void setDeclContext(DeclContext *declContext)
   {
      NamedDecl::declContext = declContext;
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
   llvm::ArrayRef<TemplateParamDecl*> getTemplateParams() const;

   llvm::StringRef getNameWithoutNamespace() const;

   std::string getFullName() const
   {
      return getJoinedName('.');
   }

   std::string getJoinedName(char join, bool includeFile = false) const;

   bool inAnonymousNamespace() const;
   bool inStdNamespace() const;
   bool isGlobalDecl() const;

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

   NamedDecl *getNextDeclInContext() const
   {
      return nextDeclInContext;
   }

   void setNextDeclInContext(NamedDecl *nextDeclInContext)
   {
      NamedDecl::nextDeclInContext = nextDeclInContext;
   }

   static bool classof(AstNode const* T)
   {
      return classofKind(T->getTypeID());
   }

   static bool classofKind(AstNode::NodeType kind)
   {
      switch (kind) {
#     define CDOT_DECL(Name) \
         case Name##ID:
#     include "../../AstNode.def"
            return true;
         default:
            return false;
      }
   }

   static bool classof(DeclContext const* T);

   static DeclContext *castToDeclContext(NamedDecl const *D);
   static NamedDecl *castFromDeclContext(DeclContext const *Ctx);

   friend class TransformImpl;

protected:
   NamedDecl(NodeType typeID,
             AccessModifier access,
             std::string &&ident,
             std::vector<StaticExpr*> &&constraints = {})
      : Statement(typeID), access(access), flags(DF_None),
        name(move(ident)), constraints(move(constraints))
   {}

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

   ExternKind externKind = ExternKind::None;
   AccessModifier access;
   unsigned flags;
   std::string name;
   std::vector<StaticExpr*> constraints;

   NamedDecl *nextDeclInContext = nullptr;
   DeclContext *declContext = nullptr;
};

class TemplateArgExpr: public Expression {
public:
   explicit TemplateArgExpr(TypeRef *type)
      : Expression(TemplateArgExprID),
        typeName(true), type(type)
   {}

   explicit TemplateArgExpr(StaticExpr *expr)
      : Expression(TemplateArgExprID),
        typeName(false), expr(expr)
   {}

   static bool classof(AstNode const* T)
   {
      return classofKind(T->getTypeID());
   }

   static bool classofKind(AstNode::NodeType kind)
   {
      return kind == TemplateArgExprID;
   }

   friend class TransformImpl;

private:
   bool typeName;

   union {
      TypeRef *type;
      StaticExpr *expr;
   };

public:
   bool isTypeName() const
   {
      return typeName;
   }

   TypeRef *getType() const
   {
      assert(isTypeName() && "not a type");
      return type;
   }

   StaticExpr *getExpr() const
   {
      assert(!isTypeName() && "not an expression");
      return expr;
   }
};

class TemplateParamDecl: public NamedDecl {
public:
   TemplateParamDecl(std::string &&name,
                     TypeRef *covariance,
                     TypeRef *contravariance,
                     bool variadic,
                     TemplateArgExpr *defaultValue)
      : NamedDecl(TemplateParamDeclID, (AccessModifier)0, move(name)),
        covariance(covariance), contravariance(contravariance),
        typeName(true), variadic(variadic), defaultValue(defaultValue)
   {}

   TemplateParamDecl(std::string &&name,
                     TypeRef *valueType,
                     bool variadic,
                     TemplateArgExpr *defaultValue)
      : NamedDecl(TemplateParamDeclID, (AccessModifier)0, move(name)),
        covariance(valueType), contravariance(nullptr),
        typeName(false), variadic(variadic),
        defaultValue(defaultValue)
   {}

   static bool classof(AstNode const* T)
   {
      return classofKind(T->getTypeID());
   }

   static bool classofKind(AstNode::NodeType kind)
   {
      return kind == TemplateParamDeclID;
   }

   friend class TransformImpl;

private:
   TypeRef *covariance;
   TypeRef *contravariance;

   bool typeName : 1;
   bool variadic : 1;

   TemplateArgExpr *defaultValue;

public:
   TypeRef *getCovariance() const
   {
      return covariance;
   }

   TypeRef *getValueType() const
   {
      return covariance;
   }

   void setCovariance(TypeRef *covariance)
   {
      TemplateParamDecl::covariance = covariance;
   }

   TypeRef *getContravariance() const
   {
      return contravariance;
   }

   void setContravariance(TypeRef *contravariance)
   {
      TemplateParamDecl::contravariance = contravariance;
   }

   TemplateArgExpr *getDefaultValue() const
   {
      return defaultValue;
   }

   void setDefaultValue(TemplateArgExpr *defaultValue)
   {
      TemplateParamDecl::defaultValue = defaultValue;
   }

   bool isTypeName() const
   {
      return typeName;
   }

   bool isVariadic() const
   {
      return variadic;
   }
};

template <class ToTy,
   bool IsKnownSubtype = ::std::is_base_of<DeclContext, ToTy>::value>
struct cast_convert_decl_context {
   static const ToTy *doit(const DeclContext *Val) {
      return static_cast<const ToTy*>(NamedDecl::castFromDeclContext(Val));
   }

   static ToTy *doit(DeclContext *Val) {
      return static_cast<ToTy*>(NamedDecl::castFromDeclContext(Val));
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

class NamespaceDecl;
class RecordDecl;
class TranslationUnit;
class ASTContext;
class NamedDecl;
class StaticExpr;

class LookupResult {
public:
   using ArrayTy = llvm::ArrayRef<NamedDecl*>;

   LookupResult() = default;

   LookupResult(NamedDecl *Single)
      : Single(Single), Result(Single)
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

   [[nodiscard]]
   AddDeclResultKind addDecl(NamedDecl *decl)
   {
      return addDecl(decl->getName(), decl);
   }

   [[nodiscard]]
   AddDeclResultKind addDecl(llvm::StringRef key, NamedDecl *decl)
   {
      auto it = namedDecls.find(key);
      if (it == namedDecls.end()) {
         namedDecls.try_emplace(key, decl);
      }
      else {
         auto lookup = it->getValue().getAsLookupResult();
         assert(!lookup.empty());

         if (lookup.front()->getTypeID() != decl->getTypeID())
            return ADR_DuplicateDifferentKind;

         if (!lookup.front()->isOverloadable())
            return ADR_Duplicate;

         it->getValue().appendDecl(decl);
      }

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

      return ADR_Success;
   }

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
         res = P->lookupOwn(name);
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

   DeclContext *getParentCtx() const
   {
      return parentCtx;
   }

   void setParentCtx(DeclContext *parentCtx)
   {
      DeclContext::parentCtx = parentCtx;
   }

   std::string getSpecifierForDiagnostic() const;

   void makeAllDeclsAvailable(DeclContext *Ctx)
   {
      for (auto D : Ctx->getDecls())
         makeDeclAvailable(D);
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

   AstNode::NodeType getDeclKind() const
   {
      return kind;
   }

   static bool classof(AstNode const *T)
   {
      switch (T->getTypeID()) {
#     define CDOT_DECL_CONTEXT(Name) \
         case AstNode::Name##ID:
#     include "../../AstNode.def"
            return true;
         default:
            return false;
      }
   }

   static bool classof(DeclContext const* T) { return true; }

protected:
   explicit DeclContext(AstNode::NodeType typeID)
      : kind(typeID)
   {}

   AstNode::NodeType kind;

   llvm::StringMap<DeclList> namedDecls;

   NamedDecl *firstDecl = nullptr;
   NamedDecl *lastAddedDecl = nullptr;

   DeclContext *parentCtx = nullptr;

public:
   class decl_iterator {
   public:
      using value_type        = NamedDecl *;
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
      using value_type        = NamedDecl *;
      using reference         = const value_type &;
      using pointer           = const value_type *;
      using iterator_category = std::forward_iterator_tag;
      using difference_type   = std::ptrdiff_t;

      specific_decl_iterator() = default;
      explicit specific_decl_iterator(value_type Ptr) : Current(Ptr)
      {
         skipToNext();
      }

      reference operator*()    const { return Current; }
      value_type operator->()  const { return Current; }

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
      value_type Current = nullptr;

      void skipToNext()
      {
         while (Current && !support::isa<SpecificDecl>(Current))
            Current = Current->getNextDeclInContext();
      }
   };

   template<class SpecificDecl, bool (SpecificDecl::*Predicate)() const>
   class filtered_decl_iterator {
   public:
      using value_type        = NamedDecl *;
      using reference         = const value_type &;
      using pointer           = const value_type *;
      using iterator_category = std::forward_iterator_tag;
      using difference_type   = std::ptrdiff_t;

      filtered_decl_iterator() = default;
      filtered_decl_iterator(value_type Ptr) : Current(Ptr)
      {
         skipToNext();
      }

      reference operator*()    const { return Current; }
      value_type operator->()  const { return Current; }

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
      value_type Current = nullptr;

      void skipToNext()
      {
         while (Current
                && (!support::isa<SpecificDecl>(Current)
                    || !(support::cast<SpecificDecl>(Current)->*Predicate)()))
            Current = Current->getNextDeclInContext();
      }
   };
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
