//
// Created by Jonas Zell on 04.01.18.
//

#ifndef CDOT_TEMPLATE_H
#define CDOT_TEMPLATE_H

#include <llvm/ADT/ArrayRef.h>
#include <string>

#include "Variant/Type/Type.h"
#include "lex/SourceLocation.h"

namespace llvm {
   class FoldingSetNodeID;
} // namespace llvm

namespace cdot {

class QualType;
struct Variant;
class Type;

namespace diag {
   class DiagnosticBuilder;
} // namespace diag

namespace ast {
   class RecordDecl;
   class CallableDecl;
   class TypedefDecl;
   class AliasDecl;
   class SemaPass;
   class FuncArgDecl;
   class TemplateParamDecl;
   class NamedDecl;
} // namespace ast

namespace sema {

class TemplateArgListImpl;
class TemplateArgList;

struct ResolvedTemplateArg {
   ResolvedTemplateArg()
      : IsType(false), IsVariadic(false), IsNull(true),
        ManuallySpecifiedVariadicArgs(0)
   {}

   explicit ResolvedTemplateArg(QualType type, SourceLocation loc = {})
      : IsType(true), IsVariadic(false), IsNull(false),
        ManuallySpecifiedVariadicArgs(0),
        Type(type), Loc(loc)
   {}

   ResolvedTemplateArg(Variant &&V,
                       QualType valueType,
                       SourceLocation loc = {}) noexcept;

   explicit
   ResolvedTemplateArg(bool isType,
                       std::vector<ResolvedTemplateArg> &&variadicArgs = {},
                       SourceLocation loc = {})
      : IsType(isType), IsVariadic(true), IsNull(false),
        ManuallySpecifiedVariadicArgs(0),
        VariadicArgs(move(variadicArgs)), Loc(loc)
   {}

   ResolvedTemplateArg(ResolvedTemplateArg &&other) noexcept;

   ~ResolvedTemplateArg();

   ResolvedTemplateArg& operator=(ResolvedTemplateArg &&other);

   QualType getType() const
   {
      assert(isType());
      return Type;
   }

   QualType getValueType() const
   {
      assert(isValue());
      return Type;
   }

   const Variant &getValue() const;

   template<class ...Args>
   void emplace_back(Args&&... args)
   {
      assert(isVariadic());
      VariadicArgs.emplace_back(std::forward<Args&&>(args)...);
   }

   std::vector<ResolvedTemplateArg>& getVariadicArgs()
   {
      assert(isVariadic());
      return VariadicArgs;
   }

   std::vector<ResolvedTemplateArg> const& getVariadicArgs() const
   {
      assert(isVariadic());
      return VariadicArgs;
   }

   ResolvedTemplateArg clone() const;

   bool isVariadic() const { return IsVariadic; }
   bool isType()     const { return IsType; }
   bool isNull()     const { return IsNull; }
   bool isValue()    const { return !isType(); }

   bool isStillDependent() const;
   std::string toString() const;

   SourceLocation getLoc() const { return Loc; }

   void Profile(llvm::FoldingSetNodeID &ID) const;

   friend class TemplateArgListImpl; // for GivenCount, ignore

private:
   bool IsType     : 1;
   bool IsVariadic : 1;
   bool IsNull     : 1;

   // number of variadic arguments that were manually specified, only when
   // this becomes 0 we begin adding inferred ones
   unsigned ManuallySpecifiedVariadicArgs : 24;

   QualType Type;

   union {
      std::unique_ptr<Variant> V;
      std::vector<ResolvedTemplateArg> VariadicArgs;
   };

   SourceLocation Loc;

   void destroyValue();
};

enum TemplateArgListResultKind {
   TLR_CouldNotInfer = 0,     // Must be kept in this order
   TLR_ConflictingInferredArg,
   TLR_TooManyTemplateArgs,
   TLR_IncompatibleArgKind,
   TLR_IncompatibleArgVal,
   TLR_Success,
};

struct TemplateArgListResult {
   operator bool() const
   {
      return ResultKind == TLR_Success;
   }

   void setCouldNotInfer(ast::TemplateParamDecl const *Param)
   {
      ResultKind = TLR_CouldNotInfer;
      Data1 = reinterpret_cast<uintptr_t >(Param);
   }

   void setHasTooManyTemplateArgs(size_t given, size_t expected)
   {
      ResultKind = TLR_TooManyTemplateArgs;
      Data1 = given;
      Data2 = expected;
   }

   void setHasIncompatibleKind(unsigned diagSelect,
                               size_t idx)
   {
      ResultKind = TLR_IncompatibleArgKind;
      Data1 = diagSelect;
      Data2 = idx;
   }

   void setHasIncompatibleType(QualType given, size_t idx)
   {
      ResultKind = TLR_IncompatibleArgVal;
      Data1 = reinterpret_cast<uintptr_t>(given.getAsOpaquePtr());
      Data2 = idx;
   }

   void setHasConflict(QualType conflicting, size_t idx)
   {
      ResultKind = TLR_ConflictingInferredArg;
      Data1 = reinterpret_cast<uintptr_t>(conflicting.getAsOpaquePtr());
      Data2 = idx;
   }

   TemplateArgListResultKind ResultKind = TLR_Success;
   uintptr_t Data1 = 0;
   uintptr_t Data2 = 0;
};

class TemplateArgList {
public:
   using TemplateParamDecl = ast::TemplateParamDecl;
   using RawArgList        = llvm::ArrayRef<ast::Expression*>;

   TemplateArgList()
      : pImpl(nullptr)
   {}

   TemplateArgList(ast::SemaPass &S,
                   ast::NamedDecl *Template,
                   RawArgList templateArguments = {},
                   SourceLocation loc = {});

   TemplateArgList(ast::SemaPass &S,
                   QualType RecordTy,
                   RawArgList templateArguments = {},
                   SourceLocation loc = {});

   explicit TemplateArgList(ast::SemaPass &S,
                            RawArgList templateArguments = {},
                            SourceLocation loc = {});

   ~TemplateArgList();

   TemplateArgList(TemplateArgList const&) = delete;
   TemplateArgList& operator=(TemplateArgList const&) = delete;

   TemplateArgList(TemplateArgList &&other) noexcept
      : pImpl(other.pImpl)
   {
      other.pImpl = nullptr;
   }

   TemplateArgList& operator=(TemplateArgList &&other) noexcept
   {
      pImpl = other.pImpl;
      other.pImpl = nullptr;

      return *this;
   }

   void Profile(llvm::FoldingSetNodeID &ID) const
   {
      Profile(ID, *this);
   }

   static void Profile(llvm::FoldingSetNodeID &ID,
                       TemplateArgList const& list);

   void inferFromReturnType(QualType contextualType, QualType returnType);
   void inferFromArgList(llvm::ArrayRef<QualType> givenArgs,
                         llvm::ArrayRef<ast::FuncArgDecl*> neededArgs);

   void resolveWith(ast::NamedDecl *R);

   bool isFullyInferred() const;
   bool isPartiallyInferred() const;

   bool isInferred() const
   {
      return isFullyInferred() || isPartiallyInferred();
   }

   TemplateArgListResult checkCompatibility() const;
   bool isStillDependent() const;

   ResolvedTemplateArg* getNamedArg(llvm::StringRef name) const;
   TemplateParamDecl* getParameter(ResolvedTemplateArg *forArg) const;

   bool insert(llvm::StringRef name, QualType ty);
   bool insert(llvm::StringRef name, Variant &&V, QualType valueTy);
   bool insert(llvm::StringRef name, bool isType,
               std::vector<ResolvedTemplateArg> &&variadicArgs);

   bool insert(llvm::StringRef name, ResolvedTemplateArg &&arg);

   bool empty() const;
   size_t size() const;

   llvm::ArrayRef<ast::Expression*> getOriginalArgs() const;

   struct arg_iterator: public std::iterator<std::forward_iterator_tag,
                                             const ResolvedTemplateArg> {
      explicit arg_iterator(TemplateArgList const &list);
      arg_iterator()
         : arg(nullptr), list(nullptr), it{}
      {}

      void operator++();

      ResolvedTemplateArg const& operator*()
      {
         return *arg;
      }

      ResolvedTemplateArg const* operator->()
      {
         return arg;
      }

      bool operator==(arg_iterator const &rhs)
      {
         return arg == rhs.arg
              && list == rhs.list;
      }

      bool operator!=(arg_iterator const &rhs)
      {
         return !operator==(rhs);
      }

      TemplateParamDecl* const& getParam() const
      {
         return *it;
      }

   private:
      ResolvedTemplateArg const* arg;
      TemplateArgList const* list;
      llvm::ArrayRef<TemplateParamDecl*>::iterator it;
   };

   std::string toString(char begin = '[', char end = ']',
                        bool showNames = false) const;

   arg_iterator begin() const
   {
      return arg_iterator(*this);
   }

   arg_iterator end() const
   {
      return arg_iterator();
   }

   friend class TemplateArgListImpl;

private:
   TemplateArgListImpl *pImpl;
};

class MultiLevelTemplateArgList {
public:
   using VecTy = llvm::SmallVector<const TemplateArgList*, 4>;

   MultiLevelTemplateArgList() = default;

   /*implicit*/
   MultiLevelTemplateArgList(const TemplateArgList &list)
      : ArgLists{ &list }
   {}

   /*implicit*/
   MultiLevelTemplateArgList(llvm::ArrayRef<TemplateArgList> lists)
   {
      for (auto &list : lists)
         ArgLists.push_back(&list);
   }

   void addOuterList(const TemplateArgList &list)
   {
      ArgLists.push_back(&list);
   }

   const TemplateArgList &innermost() const { return *ArgLists.front(); }
   const TemplateArgList &outermost() const { return *ArgLists.back(); }

   const TemplateArgList &operator[](size_t idx) const
   {
      return *ArgLists[idx];
   }

   bool isStillDependent() const
   {
      for (auto &list : ArgLists)
         if (list->isStillDependent())
            return true;

      return false;
   }

   ResolvedTemplateArg* getNamedArg(llvm::StringRef name) const
   {
      for (auto &list : ArgLists)
         if (auto arg = list->getNamedArg(name))
            return arg;

      return nullptr;
   }

   ast::TemplateParamDecl* getParameter(ResolvedTemplateArg *forArg) const
   {
      for (auto &list : ArgLists)
         if (auto param = list->getParameter(forArg))
            return param;

      return nullptr;
   }

   size_t getNumLevels() const { return ArgLists.size(); }

   VecTy::iterator begin() { return ArgLists.begin(); }
   VecTy::iterator end()   { return ArgLists.end(); }

   VecTy::const_iterator begin() const { return ArgLists.begin(); }
   VecTy::const_iterator end()   const { return ArgLists.begin(); }

private:
   VecTy ArgLists;
};

} // namespace sema
} // namespace cdot

#endif //CDOT_TEMPLATE_H
