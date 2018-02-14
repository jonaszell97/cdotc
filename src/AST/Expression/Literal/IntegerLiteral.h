//
// Created by Jonas Zell on 19.06.17.
//

#ifndef CDOT_LITERALEXPRESSION_H
#define CDOT_LITERALEXPRESSION_H

#include "../Expression.h"

#include <llvm/ADT/APInt.h>
#include <llvm/ADT/APFloat.h>

namespace cdot {
namespace ast {

class IntegerLiteral : public Expression {
public:
   IntegerLiteral(Type *type, llvm::APInt &&value)
      :  Expression(IntegerLiteralID, true),
         value(std::move(value)), type(type)
   {}

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == IntegerLiteralID;
   }

   friend class TransformImpl;

protected:
   llvm::APInt value;
   Type* type = nullptr;

public:
   const llvm::APInt &getValue() const
   {
      return value;
   }

   Type *getType() const
   {
      return type;
   }

   void setType(Type *type)
   {
      IntegerLiteral::type = type;
   }
};

class FPLiteral: public Expression {
public:
   FPLiteral(Type *type, llvm::APFloat &&value)
      :  Expression(FPLiteralID, true),
         value(std::move(value)), type(type)
   {}

   static bool classof(AstNode const* T)
   {
      return T->getTypeID() == FPLiteralID;
   }

   friend class TransformImpl;

protected:
   llvm::APFloat value;
   cdot::Type *type;

public:
   const llvm::APFloat &getValue() const
   {
      return value;
   }

   Type *getType() const
   {
      return type;
   }

   void setType(Type *type)
   {
      FPLiteral::type = type;
   }
};

class BoolLiteral: public Expression {
public:
   BoolLiteral(Type *type, bool value)
      : Expression(BoolLiteralID), value(value), type(type)
   {

   }

   static bool classof(AstNode const* T)
   {
      return T->getTypeID() == BoolLiteralID;
   }

   friend class TransformImpl;

protected:
   bool value;
   cdot::Type *type;

public:
   bool getValue() const
   {
      return value;
   }

   Type *getType() const
   {
      return type;
   }

   void setType(Type *type)
   {
      BoolLiteral::type = type;
   }
};

class CharLiteral: public Expression {
public:
   CharLiteral(Type *type, char value)
      : Expression(CharLiteralID), narrow(value), is_wide(false), type(type)
   {

   }

   CharLiteral(Type *type, wchar_t value)
      : Expression(CharLiteralID), wide(value), is_wide(true), type(type)
   {

   }

   static bool classof(AstNode const* T)
   {
      return T->getTypeID() == CharLiteralID;
   }

   friend class TransformImpl;

protected:
   union {
      char narrow;
      wchar_t wide;
   };

   bool is_wide;
   Type *type;

public:
   char getNarrow() const
   {
      return narrow;
   }

   wchar_t getWide() const
   {
      return wide;
   }

   bool isIs_wide() const
   {
      return is_wide;
   }

   Type *getType() const
   {
      return type;
   }

   void setType(Type *type)
   {
      CharLiteral::type = type;
   }
};

} // namespace ast
} // namespace cdot

#endif //CDOT_LITERALEXPRESSION_H
