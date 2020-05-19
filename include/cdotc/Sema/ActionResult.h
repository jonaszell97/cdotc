#ifndef CDOT_ACTIONRESULT_H
#define CDOT_ACTIONRESULT_H

#include "cdotc/AST/DeclDenseMapInfo.h"
#include "cdotc/AST/Type.h"

#include <llvm/ADT/PointerIntPair.h>

namespace cdot {
namespace ast {
class Statement;
class Expression;
class Decl;
} // namespace ast
} // namespace cdot

namespace cdot {

template<class T> struct IsLowBitAvailable {
   static constexpr bool value = false;
};

template<class T, bool = IsLowBitAvailable<T>::value> class ActionResult {
   T Val;
   bool Valid;

public:
   ActionResult(T Val, bool Valid) : Val(Val), Valid(Valid) {}

   ActionResult(T Val) : Val(Val), Valid(true) {}

   ActionResult() : Val(T()), Valid(false) {}

   bool isValid() const { return Valid; }
   bool hasValue() const { return Valid; }

   operator bool() const { return isValid(); }

   T get() const
   {
      assert(isValid() && "called 'get()' on invalid action result");
      return Val;
   }

   T getValue() const { return get(); }

   void set(T t) { Val = t; }
   void setValid(bool Valid) { this->Valid = Valid; }

   ActionResult& operator=(T t)
   {
      set(t);
      setValid(true);

      return *this;
   }
};

template<class T> class ActionResult<T, true> {
   llvm::PointerIntPair<T, 1, bool> Val;

public:
   ActionResult(T Val, bool Valid) : Val(Val, Valid) {}

   ActionResult(T Val) : Val(Val, true) {}

   ActionResult() : Val(T(), false) {}

   bool isValid() const { return Val.getInt(); }
   bool hasValue() const { return Val.getInt(); }
   operator bool() const { return isValid(); }

   T get() const
   {
      assert(isValid() && "called 'get()' on invalid action result");
      return Val.getPointer();
   }

   T getValue() const { return get(); }

   void set(T t) { Val.setPointer(t); }
   void setValid(bool Valid) { Val.setInt(Valid); }

   ActionResult& operator=(T t)
   {
      set(t);
      setValid(true);

      return *this;
   }
};

template<> struct IsLowBitAvailable<ast::Expression*> {
   static constexpr bool value = true;
};

template<> struct IsLowBitAvailable<ast::Statement*> {
   static constexpr bool value = true;
};

template<> struct IsLowBitAvailable<ast::Decl*> {
   static constexpr bool value = true;
};

using ExprResult = ActionResult<ast::Expression*>;
using StmtResult = ActionResult<ast::Statement*>;
using DeclResult = ActionResult<ast::Decl*>;
using TypeResult = ActionResult<QualType>;

inline ExprResult ExprError() { return ExprResult(); }

inline StmtResult StmtError() { return StmtResult(); }

inline DeclResult DeclError() { return DeclResult(); }

inline TypeResult TypeError() { return TypeResult(); }

} // namespace cdot

#endif // CDOT_ACTIONRESULT_H
