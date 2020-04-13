#ifndef CDOT_TEMPLATE_H
#define CDOT_TEMPLATE_H

#include "cdotc/AST/Type.h"
#include "cdotc/Basic/DeclarationName.h"
#include "cdotc/Lex/SourceLocation.h"

#include <llvm/ADT/ArrayRef.h>
#include <llvm/Support/TrailingObjects.h>

#include <string>

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

namespace il {
class Constant;
} // namespace il

namespace ast {
class RecordDecl;
class CallableDecl;
class TypedefDecl;
class AliasDecl;
class SemaPass;
class FuncArgDecl;
class TemplateParamDecl;
class NamedDecl;
class StaticExpr;
} // namespace ast

namespace sema {

class TemplateArgListImpl;
class TemplateArgList;

struct TemplateArgument {
   TemplateArgument()
       : IsType(false), IsVariadic(false), IsNull(true), Dependent(false),
         Frozen(false), Runtime(false), ManuallySpecifiedVariadicArgs(0),
         Param(nullptr)
   {
   }

   explicit TemplateArgument(ast::TemplateParamDecl* Param, QualType type,
                             SourceLocation loc = {}) noexcept;

   TemplateArgument(ast::TemplateParamDecl* Param, ast::StaticExpr* Expr,
                    SourceLocation loc = {}) noexcept;

   explicit TemplateArgument(ast::TemplateParamDecl* Param, bool isType,
                             std::vector<TemplateArgument>&& variadicArgs = {},
                             SourceLocation loc = {});

   TemplateArgument(TemplateArgument&& other) noexcept;

   ~TemplateArgument();

   TemplateArgument& operator=(TemplateArgument&& other) noexcept;

   QualType getType() const
   {
      assert(isType());
      return Type.getCanonicalType();
   }

   QualType getNonCanonicalType() const
   {
      assert(isType());
      return Type;
   }

   QualType getValueType() const;
   il::Constant* getValue() const;
   ast::StaticExpr* getValueExpr() const
   {
      assert(isValue());
      return Expr;
   }

   template<class... Args> void emplace_back(Args&&... args)
   {
      assert(isVariadic());
      VariadicArgs.emplace_back(std::forward<Args&&>(args)...);
   }

   std::vector<TemplateArgument>& getVariadicArgs()
   {
      assert(isVariadic());
      return VariadicArgs;
   }

   std::vector<TemplateArgument> const& getVariadicArgs() const
   {
      assert(isVariadic());
      return VariadicArgs;
   }

   /// Mark this variadic argument as final to avoid any more types being
   // inferred for it.
   void freeze();
   bool isFrozen() const { return Frozen; }

   ast::TemplateParamDecl* getParam() const { return Param; }

   TemplateArgument clone(bool Canonicalize = false, bool Freeze = false,
                          ast::TemplateParamDecl* P = nullptr) const;

   bool operator==(const TemplateArgument& RHS) const;
   bool operator!=(const TemplateArgument& RHS) const
   {
      return !(*this == RHS);
   }

   bool isVariadic() const { return IsVariadic; }
   bool isType() const { return IsType; }
   bool isNull() const { return IsNull; }
   bool isValue() const { return !isType(); }
   bool isRuntime() const { return Runtime; }

   bool isStillDependent() const;
   std::string toString() const;

   SourceLocation getLoc() const { return Loc; }

   void Profile(llvm::FoldingSetNodeID& ID, bool Canonicalize = false) const;

   friend class TemplateArgListImpl; // for GivenCount, ignore

private:
   bool IsType : 1;
   bool IsVariadic : 1;
   bool IsNull : 1;
   bool Dependent : 1;
   bool Frozen : 1;
   bool Runtime : 1;

   // number of variadic arguments that were manually specified, only when
   // this becomes 0 we begin adding inferred ones
   unsigned ManuallySpecifiedVariadicArgs : 24;

   ast::TemplateParamDecl* Param;

   union {
      mutable QualType Type;
      ast::StaticExpr* Expr;
      std::vector<TemplateArgument> VariadicArgs;
   };

   SourceLocation Loc;

   void destroyValue();
};

enum TemplateArgListResultKind {
   TLR_CouldNotInfer = 0, // Must be kept in this order
   TLR_ConflictingInferredArg,
   TLR_TooManyTemplateArgs,
   TLR_IncompatibleArgKind,
   TLR_IncompatibleArgVal,
   TLR_CovarianceError,
   TLR_Success,
};

struct TemplateArgListResult {
   operator bool() const { return ResultKind == TLR_Success; }

   void setCouldNotInfer(const ast::TemplateParamDecl* Param)
   {
      ResultKind = TLR_CouldNotInfer;
      Data1 = reinterpret_cast<uintptr_t>(Param);
   }

   void setHasTooManyTemplateArgs(size_t given, size_t expected)
   {
      ResultKind = TLR_TooManyTemplateArgs;
      Data1 = given;
      Data2 = expected;
   }

   void setHasIncompatibleKind(unsigned diagSelect,
                               const ast::TemplateParamDecl* P)
   {
      ResultKind = TLR_IncompatibleArgKind;
      Data1 = diagSelect;
      Data2 = reinterpret_cast<uintptr_t>(P);
   }

   void setHasIncompatibleType(QualType given, const ast::TemplateParamDecl* P)
   {
      ResultKind = TLR_IncompatibleArgVal;
      Data1 = reinterpret_cast<uintptr_t>(given.getAsOpaquePtr());
      Data2 = reinterpret_cast<uintptr_t>(P);
   }

   void setHasConflict(QualType conflicting, const ast::TemplateParamDecl* P)
   {
      ResultKind = TLR_ConflictingInferredArg;
      Data1 = reinterpret_cast<uintptr_t>(conflicting.getAsOpaquePtr());
      Data2 = reinterpret_cast<uintptr_t>(P);
   }

   void setCovarianceError(QualType Given, const ast::TemplateParamDecl* P)
   {
      ResultKind = TLR_CovarianceError;
      Data1 = reinterpret_cast<uintptr_t>(Given.getAsOpaquePtr());
      Data2 = reinterpret_cast<uintptr_t>(P);
   }

   TemplateArgListResultKind ResultKind = TLR_Success;
   uintptr_t Data1 = 0;
   uintptr_t Data2 = 0;
};

class TemplateArgList {
public:
   using TemplateParamDecl = ast::TemplateParamDecl;
   using RawArgList = llvm::ArrayRef<ast::Expression*>;

   TemplateArgList() : pImpl(nullptr) {}

   TemplateArgList(ast::SemaPass& S, ast::NamedDecl* Template,
                   RawArgList templateArguments = {}, SourceLocation loc = {});

   TemplateArgList(ast::SemaPass& S, QualType RecordTy,
                   RawArgList templateArguments = {}, SourceLocation loc = {});

   explicit TemplateArgList(ast::SemaPass& S, RawArgList templateArguments = {},
                            SourceLocation loc = {});

   ~TemplateArgList();

   TemplateArgList(TemplateArgList const&) = delete;
   TemplateArgList& operator=(TemplateArgList const&) = delete;

   TemplateArgList(TemplateArgList&& other) noexcept : pImpl(other.pImpl)
   {
      other.pImpl = nullptr;
   }

   TemplateArgList& operator=(TemplateArgList&& other) noexcept
   {
      pImpl = other.pImpl;
      other.pImpl = nullptr;

      return *this;
   }

   TemplateArgList copy() const;

   void Profile(llvm::FoldingSetNodeID& ID) const { Profile(ID, *this); }

   static void Profile(llvm::FoldingSetNodeID& ID, TemplateArgList const& list);

   bool setParamValue(TemplateParamDecl* Param, TemplateArgument&& Arg) const;

   bool inferFromType(QualType contextualType, QualType returnType,
                      bool IsLastVariadicParam = false) const;

   void inferFromArgList(llvm::ArrayRef<QualType> givenArgs,
                         llvm::ArrayRef<ast::FuncArgDecl*> neededArgs) const;

   bool isFullyInferred() const;
   bool isPartiallyInferred() const;

   llvm::MutableArrayRef<TemplateArgument> getMutableArgs() const;

   bool isInferred() const
   {
      return isFullyInferred() || isPartiallyInferred();
   }

   TemplateArgListResult checkCompatibility() const;
   bool isStillDependent() const;
   bool hasRuntimeParameter() const;

   TemplateArgument* getNamedArg(DeclarationName Name) const;
   TemplateArgument* getArgForParam(TemplateParamDecl* P) const;
   TemplateParamDecl* getParameter(TemplateArgument* forArg) const;

   bool insert(TemplateArgument&& arg);

   bool empty() const;
   size_t size() const;

   const TemplateArgument& operator[](size_t idx) const;
   const TemplateArgument& front() const;
   const TemplateArgument& back() const;

   ast::NamedDecl* getTemplate() const;

   void print(llvm::raw_ostream& OS, char begin = '<', char end = '>',
              bool showNames = false) const;
   std::string toString(char begin = '<', char end = '>',
                        bool showNames = false) const;

   using arg_iterator = TemplateArgument*;
   using const_arg_iterator = const TemplateArgument*;

   arg_iterator begin();
   arg_iterator end();

   const_arg_iterator begin() const;
   const_arg_iterator end() const;

   friend class TemplateArgListImpl;

private:
   mutable TemplateArgListImpl* pImpl;
};

ASSERT_NOEXCEPT_MOVE_CONSTRUCTIBLE(TemplateArgList);

class LLVM_ALIGNAS(sizeof(void*)) FinalTemplateArgumentList final
    : llvm::TrailingObjects<FinalTemplateArgumentList, TemplateArgument>,
      public llvm::FoldingSetNode {
   FinalTemplateArgumentList(llvm::MutableArrayRef<TemplateArgument> Args,
                             bool Dependent, bool RuntimeParam,
                             bool Canonicalize);

   unsigned NumArgs : 28;
   bool Dependent : 1;
   bool RuntimeParam : 1;

public:
   static FinalTemplateArgumentList*
   Create(ast::ASTContext& C, llvm::MutableArrayRef<TemplateArgument> Args,
          bool Canonicalize = true);

   static FinalTemplateArgumentList* Create(ast::ASTContext& C,
                                            const TemplateArgList& list,
                                            bool Canonicalize = true);

   void Profile(llvm::FoldingSetNodeID& ID, bool Canonicalize = false) const;
   static void Profile(llvm::FoldingSetNodeID& ID,
                       ArrayRef<TemplateArgument> Args,
                       bool Canonicalize = false);

   using arg_iterator = const TemplateArgument*;

   arg_iterator begin() const { return getTrailingObjects<TemplateArgument>(); }
   arg_iterator end() const { return begin() + NumArgs; }

   llvm::ArrayRef<TemplateArgument> getArguments() const
   {
      return {begin(), NumArgs};
   }

   bool empty() const { return !NumArgs; }
   size_t size() const { return NumArgs; }

   const TemplateArgument& operator[](size_t idx) const
   {
      assert(idx < NumArgs && "index out of bounds for template arg list");
      return *(begin() + idx);
   }

   const TemplateArgument& front() const { return (*this)[0]; }
   const TemplateArgument& back() const { return (*this)[NumArgs - 1]; }

   void print(llvm::raw_ostream& OS, char begin = '<', char end = '>',
              bool showNames = false) const;

   std::string toString(char begin = '<', char end = '>',
                        bool showNames = false) const;

   const TemplateArgument* getNamedArg(DeclarationName Name) const;
   const TemplateArgument* getArgForParam(ast::TemplateParamDecl* P) const;
   ast::TemplateParamDecl* getParameter(TemplateArgument* forArg) const;

   bool isStillDependent() const { return Dependent; }
   bool hasRuntimeParameter() const { return RuntimeParam; }

   friend class ast::ASTContext;
   friend TrailingObjects;
};

class MultiLevelTemplateArgList {
public:
   using VecTy = llvm::SmallVector<const TemplateArgList*, 0>;

   MultiLevelTemplateArgList() = default;

   /*implicit*/
   MultiLevelTemplateArgList(const TemplateArgList& list) : ArgLists{&list} {}

   /*implicit*/
   MultiLevelTemplateArgList(llvm::ArrayRef<TemplateArgList> lists)
   {
      for (auto& list : lists)
         ArgLists.push_back(&list);
   }

   void Profile(llvm::FoldingSetNodeID& ID) const;

   void addOuterList(const TemplateArgList& list) { ArgLists.push_back(&list); }

   const TemplateArgList& innermost() const { return *ArgLists.front(); }
   const TemplateArgList& outermost() const { return *ArgLists.back(); }

   const TemplateArgList* pop_back_val() { return ArgLists.pop_back_val(); }

   const TemplateArgList& operator[](size_t idx) const
   {
      return *ArgLists[idx];
   }

   TemplateArgListResult checkCompatibility() const;

   bool isStillDependent() const
   {
      for (auto& list : ArgLists)
         if (list->isStillDependent())
            return true;

      return false;
   }

   TemplateArgument* getNamedArg(DeclarationName Name) const
   {
      for (auto& list : ArgLists)
         if (auto arg = list->getNamedArg(Name))
            return arg;

      return nullptr;
   }

   TemplateArgument* getArgForParam(ast::TemplateParamDecl* P) const
   {
      for (auto& list : ArgLists)
         if (auto arg = list->getArgForParam(P))
            return arg;

      return nullptr;
   }

   ast::TemplateParamDecl* getParameter(TemplateArgument* forArg) const
   {
      for (auto& list : ArgLists)
         if (auto param = list->getParameter(forArg))
            return param;

      return nullptr;
   }

   bool setParamValue(ast::TemplateParamDecl* Param, TemplateArgument&& Arg);

   bool inferFromType(QualType contextualType, QualType returnType,
                      bool IsLastVariadicParam = false);

   size_t size() const { return ArgLists.size(); }
   bool empty() const { return ArgLists.empty(); }

   VecTy::iterator begin() { return ArgLists.begin(); }
   VecTy::iterator end() { return ArgLists.end(); }

   VecTy::const_iterator begin() const { return ArgLists.begin(); }
   VecTy::const_iterator end() const { return ArgLists.end(); }

   void print(llvm::raw_ostream& OS) const;
   std::string toString() const;

private:
   VecTy ArgLists;
};

class MultiLevelFinalTemplateArgList {
public:
   using VecTy = llvm::SmallVector<const FinalTemplateArgumentList*, 0>;

   using pointer = const FinalTemplateArgumentList*;
   using reference = const FinalTemplateArgumentList&;

   MultiLevelFinalTemplateArgList() = default;

   /*implicit*/
   MultiLevelFinalTemplateArgList(reference list) : ArgLists{&list} {}

   /*implicit*/
   MultiLevelFinalTemplateArgList(
       llvm::ArrayRef<FinalTemplateArgumentList> lists)
   {
      for (auto& list : lists)
         ArgLists.push_back(&list);
   }

   void Profile(llvm::FoldingSetNodeID& ID) const;

   void addOuterList(reference list) { ArgLists.push_back(&list); }

   reference innermost() const { return *ArgLists.front(); }
   reference outermost() const { return *ArgLists.back(); }

   pointer pop_back_val() { return ArgLists.pop_back_val(); }

   reference operator[](size_t idx) const { return *ArgLists[idx]; }

   const TemplateArgument* getNamedArg(DeclarationName Name) const
   {
      for (auto& list : ArgLists)
         if (auto arg = list->getNamedArg(Name))
            return arg;

      return nullptr;
   }

   const TemplateArgument* getArgForParam(ast::TemplateParamDecl* P) const
   {
      for (auto& list : ArgLists)
         if (auto arg = list->getArgForParam(P))
            return arg;

      return nullptr;
   }

   ast::TemplateParamDecl* getParameter(TemplateArgument* forArg) const
   {
      for (auto& list : ArgLists)
         if (auto param = list->getParameter(forArg))
            return param;

      return nullptr;
   }

   size_t size() const { return ArgLists.size(); }
   bool empty() const { return ArgLists.empty(); }
   void clear() { ArgLists.clear(); }

   void push_back(const FinalTemplateArgumentList& L)
   {
      ArgLists.push_back(&L);
   }

   void reverse();

   VecTy::iterator begin() { return ArgLists.begin(); }
   VecTy::iterator end() { return ArgLists.end(); }

   VecTy::const_iterator begin() const { return ArgLists.begin(); }
   VecTy::const_iterator end() const { return ArgLists.end(); }

   void print(llvm::raw_ostream& OS) const;
   std::string toString() const;

private:
   VecTy ArgLists;
};

inline llvm::raw_ostream& operator<<(llvm::raw_ostream& OS,
                                     const TemplateArgList& list)
{
   list.print(OS);
   return OS;
}

inline llvm::raw_ostream& operator<<(llvm::raw_ostream& OS,
                                     const FinalTemplateArgumentList& list)
{
   list.print(OS);
   return OS;
}

inline llvm::raw_ostream& operator<<(llvm::raw_ostream& OS,
                                     const MultiLevelTemplateArgList& list)
{
   list.print(OS);
   return OS;
}

inline llvm::raw_ostream& operator<<(llvm::raw_ostream& OS,
                                     const MultiLevelFinalTemplateArgList& list)
{
   list.print(OS);
   return OS;
}

} // namespace sema
} // namespace cdot

#endif // CDOT_TEMPLATE_H
