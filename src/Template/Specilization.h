//
// Created by Jonas Zell on 07.11.17.
//

#ifndef CDOT_SPECILIZATION_H
#define CDOT_SPECILIZATION_H

#include <string>
#include <vector>
#include <memory>

class TypeRef;
class BuiltinType;
class Expression;
class Token;
class AbstractPass;

namespace cdot {

struct TemplateArg;

class TemplateSpecilizationConstraint {
public:
   enum Kind {
      // typename template args
      TSC_TypeEquality,
      TSC_SubType,
      TSC_SuperType,
      TSC_ProtocolConformance,

      // value template args
      TSC_ValueExpression,
      TSC_ValueType,

      // token template args
      TSC_TokenEquality
   };

   virtual bool appliesTo(const TemplateArg &arg) = 0;
   virtual void accept(AbstractPass *pass) = 0;

   Kind getKind() const;
   void setKind(Kind kind);

   const std::string &getTemplateArgName() const;
   void setTemplateArgName(const std::string &templateArgName);

protected:
   TemplateSpecilizationConstraint(Kind kind, std::string &&argName);

   Kind kind;
   std::string templateArgName;
};

class TypeConstraint: public TemplateSpecilizationConstraint {
public:
   const std::shared_ptr<TypeRef> &getType() const;

protected:
   TypeConstraint(Kind kind, std::string &&argName,
                  std::shared_ptr<TypeRef> &&type);

   std::shared_ptr<TypeRef> type;
};

class InheritanceConstraint: public TypeConstraint {
public:
   enum InheritanceKind {
      IK_Equal,
      IK_Sub,
      IK_Super
   };

   InheritanceConstraint(std::shared_ptr<TypeRef> &&type,
                         std::string &&argName, InheritanceKind kind);

   bool appliesTo(const TemplateArg &arg) override;
   void accept(AbstractPass *pass) override;

protected:
   InheritanceKind inheritance_kind;
};

class ConformanceConstraint: public TypeConstraint {
public:
   ConformanceConstraint(std::shared_ptr<TypeRef> &&type,
                         std::string &&argName);

   bool appliesTo(const TemplateArg &arg) override;
   void accept(AbstractPass *pass) override;
};

class ValueConstraint: public TemplateSpecilizationConstraint {
public:
   const std::shared_ptr<Expression> &getExpr() const;

protected:
   ValueConstraint(Kind kind, std::string &&argName,
                   std::shared_ptr<Expression> &&expr);

   std::shared_ptr<Expression> expr;
};

class ValueExprConstraint: public ValueConstraint {
public:
   ValueExprConstraint(std::string &&argName,
                       std::shared_ptr<Expression> &&expr);

   bool appliesTo(const TemplateArg &arg) override;
   void accept(AbstractPass *pass) override;
};

class TokenConstraint: public TemplateSpecilizationConstraint {
public:
   const std::vector<Token> &getTokens() const;

protected:
   TokenConstraint(Kind kind, std::string &&argName,
                   std::vector<Token> &&tokens);

   std::vector<Token> tokens;
};

class TokenEqualityConstraint: public TokenConstraint {
public:
   TokenEqualityConstraint(std::string &&argName,
                           std::vector<Token> &&tokens);

   bool appliesTo(const TemplateArg &arg) override;
   void accept(AbstractPass *pass) override;
};


} // namespace cdot

#endif //CDOT_SPECILIZATION_H
