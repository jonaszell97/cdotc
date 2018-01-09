//
// Created by Jonas Zell on 07.11.17.
//

#ifndef CDOT_SPECILIZATION_H
#define CDOT_SPECILIZATION_H

#include <string>
#include <vector>
#include <memory>

class BuiltinType;

namespace cdot {

namespace lex {

struct Token;

} // namespace lex

class TemplateArg;

namespace ast {

class Expression;
class TypeRef;
class ASTPass;

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

protected:
   InheritanceKind inheritance_kind;
};

class ConformanceConstraint: public TypeConstraint {
public:
   ConformanceConstraint(std::shared_ptr<TypeRef> &&type,
                         std::string &&argName);

   bool appliesTo(const TemplateArg &arg) override;
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
};

class TokenConstraint: public TemplateSpecilizationConstraint {
public:
   const std::vector<lex::Token> &getTokens() const;

protected:
   TokenConstraint(Kind kind, std::string &&argName,
                   std::vector<lex::Token> &&tokens);

   std::vector<lex::Token> tokens;
};

class TokenEqualityConstraint: public TokenConstraint {
public:
   TokenEqualityConstraint(std::string &&argName,
                           std::vector<lex::Token> &&tokens);

   bool appliesTo(const TemplateArg &arg) override;
};

} // namespace ast
} // namespace cdot

#endif //CDOT_SPECILIZATION_H
