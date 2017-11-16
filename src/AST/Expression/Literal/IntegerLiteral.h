//
// Created by Jonas Zell on 19.06.17.
//

#ifndef CDOT_LITERALEXPRESSION_H
#define CDOT_LITERALEXPRESSION_H

#include "../Expression.h"

class IntegerLiteral : public Expression {
public:
   explicit IntegerLiteral(cdot::Variant &&lexeme);

   bool needsContextualInformation() const override
   {
      return true;
   }

   typedef std::shared_ptr<IntegerLiteral> SharedPtr;

   NodeType get_type() override
   {
      return NodeType::INTEGER_LITERAL;
   }

   bool createsTemporary() override
   {
      return true;
   }

   ASTNODE_ACCEPT_PASSES
   ADD_FRIEND_PASSES

protected:
   cdot::Variant value;
   BuiltinType* type = nullptr;

public:
   const Variant &getValue() const;
   void setValue(const Variant &value);

   BuiltinType *getType() const;
   void setType(BuiltinType *type);
};

class FPLiteral: public Expression {
public:
   explicit FPLiteral(cdot::Variant &&val);

   NodeType get_type() override
   {
      return NodeType::FLOATING_LITERAL;
   }

   typedef std::shared_ptr<FPLiteral> SharedPtr;
   ASTNODE_ACCEPT_PASSES

protected:
   cdot::Variant value;
   cdot::BuiltinType *type;

public:
   const Variant &getValue() const;
   void setValue(const Variant &val);

   BuiltinType *getType() const;
   void setType(BuiltinType *type);
};

class BoolLiteral: public Expression {
public:
   explicit BoolLiteral(bool value);

   NodeType get_type() override
   {
      return NodeType::BOOL_LITERAL;
   }

   typedef std::shared_ptr<BoolLiteral> SharedPtr;
   ASTNODE_ACCEPT_PASSES

protected:
   bool value;
   cdot::BuiltinType *type;

public:
   bool getValue() const;
   void setValue(bool value);

   BuiltinType *getType() const;
   void setType(BuiltinType *type);
};

class CharLiteral: public Expression {
public:
   explicit CharLiteral(char value);
   explicit CharLiteral(wchar_t value);

   NodeType get_type() override
   {
      return NodeType::CHAR_LITERAL;
   }

   typedef std::shared_ptr<CharLiteral> SharedPtr;
   ASTNODE_ACCEPT_PASSES

protected:
   union {
      char narrow;
      wchar_t wide;
   };

   bool is_wide;
   cdot::BuiltinType *type;

public:
   char getNarrow() const;
   void setNarrow(char narrow);

   wchar_t getWide() const;
   void setWide(wchar_t wide);

   bool isWide() const;
   void isWide(bool is_wide);

   BuiltinType *getType() const;
   void setType(BuiltinType *type);
};


#endif //CDOT_LITERALEXPRESSION_H
