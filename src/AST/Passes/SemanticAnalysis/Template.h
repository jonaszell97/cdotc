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
   class TemplateArgExpr;
   class TemplateParamDecl;
   class NamedDecl;
} // namespace ast

namespace sema {

class TemplateArgListImpl;
class TemplateArgList;

struct ResolvedTemplateArg {
   explicit ResolvedTemplateArg(QualType type, SourceLocation loc = {})
      : is_type(true), is_variadic(false), is_inferred(false), ignore(false),
        type(type), loc(loc)
   {}

   explicit ResolvedTemplateArg(Variant &&V,
                                QualType valueType,
                                SourceLocation loc = {}) noexcept;

   explicit ResolvedTemplateArg(
                           bool isType,
                           std::vector<ResolvedTemplateArg> &&variadicArgs = {},
                           SourceLocation loc = {})
      : is_type(isType), is_variadic(true), is_inferred(false), ignore(false),
        variadicArgs(move(variadicArgs)), loc(loc)
   {}

   ResolvedTemplateArg(ResolvedTemplateArg &&other) noexcept;

   ~ResolvedTemplateArg();

   ResolvedTemplateArg& operator=(ResolvedTemplateArg &&other);

   QualType getType() const
   {
      assert(isType());
      return type;
   }

   QualType getValueType() const
   {
      assert(isValue());
      return type;
   }

   const Variant &getValue() const;

   template<class ...Args>
   void emplace_back(Args&&... args)
   {
      assert(isVariadic());
      variadicArgs.emplace_back(std::forward<Args&&>(args)...);
   }

   std::vector<ResolvedTemplateArg> const& getVariadicArgs() const
   {
      assert(isVariadic());
      return variadicArgs;
   }

   bool isVariadic() const
   {
      return is_variadic;
   }

   bool isType() const
   {
      return is_type;
   }

   bool isValue() const { return !isType(); }
   bool isStillDependant() const;

   std::string toString() const;

   const SourceLocation &getLoc() const
   {
      return loc;
   }

   void Profile(llvm::FoldingSetNodeID &ID) const;

   friend class TemplateArgListImpl; // for is_inferred, ignore

private:
   bool is_type     : 1;
   bool is_variadic : 1;
   bool is_inferred : 1;
   bool ignore      : 1;

   QualType type;

   union {
      std::unique_ptr<Variant> V;
      std::vector<ResolvedTemplateArg> variadicArgs;
   };

   SourceLocation loc;

   void destroyValue();
};

class TemplateArgList {
public:
   using TemplateArgExpr = ast::TemplateArgExpr;
   using TemplateParamDecl = ast::TemplateParamDecl;

   TemplateArgList()
      : pImpl(nullptr)
   {}

   TemplateArgList(ast::SemaPass &S,
                   ast::NamedDecl *Template,
                   llvm::ArrayRef<TemplateArgExpr*> templateArguments = {});

   explicit TemplateArgList(ast::SemaPass &S,
                            llvm::ArrayRef<TemplateArgExpr*> templateArguments = {});

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

   void inferFromArgList(llvm::ArrayRef<QualType> givenArgs,
                         llvm::ArrayRef<ast::FuncArgDecl*> neededArgs);

   void resolveWith(ast::NamedDecl *R);

   TemplateArgList moveInitializerArgs();

   void markAsIgnored(llvm::StringRef name);
   void markAllAsNotIgnored();

   void setParameterOffsetBegin(size_t parameterOffsetBegin);
   void setParameterOffsetEnd(size_t parameterOffsetEnd);

   bool checkCompatibility() const;
   llvm::SmallVector<diag::DiagnosticBuilder, 4> &getDiagnostics();

   bool isStillDependent() const;

   TemplateParamDecl const* getMissingArgument() const;
   ResolvedTemplateArg const* getNamedArg(llvm::StringRef name) const;

   bool insert(llvm::StringRef name, Type *ty);
   bool insert(llvm::StringRef name, Variant &&V, QualType valueTy);
   bool insert(llvm::StringRef name, bool isType,
               std::vector<ResolvedTemplateArg> &&variadicArgs);

   bool insert(llvm::StringRef name, ResolvedTemplateArg &&arg);

   bool empty() const;
   size_t size() const;

   llvm::ArrayRef<TemplateArgExpr*> getOriginalArgs() const;

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

   std::string toString(char begin = '<', char end = '>',
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

} // namespace sema
} // namespace cdot

#endif //CDOT_TEMPLATE_H
