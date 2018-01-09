//
// Created by Jonas Zell on 12.12.17.
//

#ifndef CDOT_PRECEDENCE_H
#define CDOT_PRECEDENCE_H

namespace cdot {

enum class FixKind : unsigned char {
   Infix = 1,
   Prefix = 1 << 1,
   Postfix = 1 << 2
};

enum class Associativity : unsigned char {
   Left,
   Right
};

class PrecedenceGroup {
public:
   PrecedenceGroup() : precedence(-1) {}
   PrecedenceGroup(int precedence, Associativity assoc)
      : precedence(precedence), associativity(assoc)
   {}

   int getPrecedence() const
   {
      return precedence;
   }

   Associativity getAssociativity() const
   {
      return associativity;
   }

   bool isValid() const
   {
      return precedence != -1;
   }

   bool isLeftAssociative() const
   {
      return associativity == Associativity::Left;
   }

   bool isRightAssociative() const { return !isLeftAssociative(); }

private:
   int precedence;
   Associativity associativity;
};

class OperatorInfo {
public:
   OperatorInfo() = default;

   OperatorInfo(const PrecedenceGroup &precedenceGroup,
                FixKind fix,
                bool implicit = false)
      : precedenceGroup(precedenceGroup), fix(fix), implicit(implicit)
   { }

   const PrecedenceGroup &getPrecedenceGroup() const
   {
      return precedenceGroup;
   }

   FixKind getFix() const
   {
      return fix;
   }

   void setPrecedenceGroup(const PrecedenceGroup &precedenceGroup)
   {
      OperatorInfo::precedenceGroup = precedenceGroup;
   }

   void setFix(FixKind fix)
   {
      OperatorInfo::fix = fix;
   }

   bool isImplicit() const
   {
      return implicit;
   }

   void setImplicit(bool implicit)
   {
      OperatorInfo::implicit = implicit;
   }

private:
   PrecedenceGroup precedenceGroup;
   FixKind fix;
   bool implicit = false;
};

} // namespace cdot

#endif //CDOT_PRECEDENCE_H
