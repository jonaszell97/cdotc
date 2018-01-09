//
// Created by Jonas Zell on 19.06.17.
//

#ifndef CDOT_LITERALEXPRESSION_H
#define CDOT_LITERALEXPRESSION_H

#include "../Expression.h"

namespace cdot {
namespace ast {

class IntegerLiteral : public Expression {
public:
   explicit IntegerLiteral(cdot::Variant &&lexeme);
   IntegerLiteral(Type *type, cdot::Variant &&lexeme);

   typedef std::shared_ptr<IntegerLiteral> SharedPtr;

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == IntegerLiteralID;
   }

protected:
   cdot::Variant value;
   Type* type = nullptr;

public:
   const Variant &getValue() const;
   void setValue(const Variant &value);

   Type *getType() const;
   void setType(Type *type);
};

class FPLiteral: public Expression {
public:
   explicit FPLiteral(cdot::Variant &&val);
   FPLiteral(Type *type, cdot::Variant &&val);

   typedef std::shared_ptr<FPLiteral> SharedPtr;

   static bool classof(AstNode const* T)
   {
      return T->getTypeID() == FPLiteralID;
   }

protected:
   cdot::Variant value;
   cdot::Type *type;

public:
   const Variant &getValue() const;
   void setValue(const Variant &val);

   Type *getType() const;
   void setType(Type *type);
};

class BoolLiteral: public Expression {
public:
   explicit BoolLiteral(bool value);
   BoolLiteral(Type *type, bool value);

   typedef std::shared_ptr<BoolLiteral> SharedPtr;

   static bool classof(AstNode const* T)
   {
      return T->getTypeID() == BoolLiteralID;
   }

protected:
   bool value;
   cdot::Type *type;

public:
   bool getValue() const;
   void setValue(bool value);

   Type *getType() const;
   void setType(Type *type);
};

class CharLiteral: public Expression {
public:
   explicit CharLiteral(char value);
   explicit CharLiteral(wchar_t value);

   CharLiteral(Type *type, char value);

   typedef std::shared_ptr<CharLiteral> SharedPtr;

   static bool classof(AstNode const* T)
   {
      return T->getTypeID() == CharLiteralID;
   }

protected:
   union {
      char narrow;
      wchar_t wide;
   };

   bool is_wide;
   cdot::Type *type;

public:
   char getNarrow() const;
   void setNarrow(char narrow);

   wchar_t getWide() const;
   void setWide(wchar_t wide);

   bool isWide() const;
   void isWide(bool is_wide);

   Type *getType() const;
   void setType(Type *type);
};

} // namespace ast
} // namespace cdot

#endif //CDOT_LITERALEXPRESSION_H
