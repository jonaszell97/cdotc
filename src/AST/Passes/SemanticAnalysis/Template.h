//
// Created by Jonas Zell on 04.01.18.
//

#ifndef CDOT_TEMPLATE_H
#define CDOT_TEMPLATE_H

#include <llvm/ADT/ArrayRef.h>
#include <string>
#include "../../../lex/SourceLocation.h"

namespace cdot {

class TemplateArg;
struct TemplateParameter;
class QualType;
struct Variant;
struct Argument;
class Type;
class Callable;
struct Typedef;
struct Alias;

namespace cl {
   class Record;
} // namespace cl

namespace diag {
   class DiagnosticBuilder;
} // namespace diag

namespace ast {
   class SemaPass;
} // namespace ast

namespace sema {

class TemplateArgListImpl;
class TemplateArgList;

struct ResolvedTemplateArg {
   explicit ResolvedTemplateArg(Type *type, SourceLocation loc = {})
      : is_type(true), is_variadic(false), is_inferred(false), ignore(false),
        type(type), loc(loc)
   {}

   explicit ResolvedTemplateArg(Variant &&V,
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

   Type *getType() const
   {
      assert(isType());
      return type;
   }

   const Variant &getValue() const;

   template<class ...Args>
   void emplace_back(Args&&... args)
   {
      assert(isVariadic());
      variadicArgs.emplace_back(std::forward<Args>(args)...);
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

   friend class TemplateArgListImpl; // for is_inferred, ignore

private:
   bool is_type     : 1;
   bool is_variadic : 1;
   bool is_inferred : 1;
   bool ignore      : 1;

   union {
      Type *type = nullptr;
      std::unique_ptr<Variant> V;
      std::vector<ResolvedTemplateArg> variadicArgs;
   };

   SourceLocation loc;

   void destroyValue();
};

class TemplateArgList {
public:
   TemplateArgList()
      : pImpl(nullptr)
   {}

   TemplateArgList(ast::SemaPass &S,
                   cl::Record *R,
                   llvm::ArrayRef<TemplateArg> templateArguments = {});

   TemplateArgList(ast::SemaPass &S,
                   Callable *C,
                   llvm::ArrayRef<TemplateArg> templateArguments = {});

   TemplateArgList(ast::SemaPass &S,
                   Typedef *td,
                   llvm::ArrayRef<TemplateArg> templateArguments = {});

   TemplateArgList(ast::SemaPass &S,
                   Alias *alias,
                   llvm::ArrayRef<TemplateArg> templateArguments = {});

   explicit TemplateArgList(ast::SemaPass &S,
                            llvm::ArrayRef<TemplateArg> templateArguments = {});

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

   void inferFromArgList(std::vector<QualType> const& givenArgs,
                         std::vector<Argument> const& neededArgs);

   void resolveWith(cl::Record *R);
   void resolveWith(Callable *C);
   void resolveWith(Typedef *td);
   void resolveWith(Alias *alias);

   TemplateArgList moveInitializerArgs();

   void markAsIgnored(llvm::StringRef name);
   void markAllAsNotIgnored();

   void setParameterOffsetBegin(size_t parameterOffsetBegin);
   void setParameterOffsetEnd(size_t parameterOffsetEnd);

   bool checkCompatibility() const;
   llvm::SmallVector<diag::DiagnosticBuilder, 4> &getDiagnostics();

   bool isStillDependant() const;

   TemplateParameter const* getMissingArgument() const;
   ResolvedTemplateArg const* getNamedArg(llvm::StringRef name) const;

   bool insert(llvm::StringRef name, Type *ty);
   bool insert(llvm::StringRef name, Variant &&V);
   bool insert(llvm::StringRef name, bool isType,
               std::vector<ResolvedTemplateArg> &&variadicArgs);

   bool empty() const;
   size_t size() const;

   llvm::ArrayRef<TemplateArg> getOriginalArgs() const;

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

      TemplateParameter const& getParam() const
      {
         return *it;
      }

   private:
      ResolvedTemplateArg const* arg;
      TemplateArgList const* list;
      llvm::ArrayRef<TemplateParameter>::iterator it;
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
