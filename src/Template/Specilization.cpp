//
// Created by Jonas Zell on 07.11.17.
//

#include "Specilization.h"

#include <llvm/Support/ErrorHandling.h>
#include <cassert>

#include "../AST/Passes/AbstractPass.h"
#include "../AST/Expression/TypeRef.h"

#include "../Variant/Type/GenericType.h"
#include "../Variant/Type/Generic.h"

namespace cdot {

TemplateSpecilizationConstraint::TemplateSpecilizationConstraint(
   Kind kind, std::string &&argName) : kind(kind), templateArgName(argName)
{

}

TemplateSpecilizationConstraint::Kind
TemplateSpecilizationConstraint::getKind() const
{
   return kind;
}

void TemplateSpecilizationConstraint::setKind(
   TemplateSpecilizationConstraint::Kind kind)
{
   TemplateSpecilizationConstraint::kind = kind;
}

const std::string &
TemplateSpecilizationConstraint::getTemplateArgName() const
{
   return templateArgName;
}

void TemplateSpecilizationConstraint::setTemplateArgName(
   const std::string &templateArgName)
{
   TemplateSpecilizationConstraint::templateArgName = templateArgName;
}

/// Type Constraints

TypeConstraint::TypeConstraint(
   Kind kind, std::string &&argName, std::shared_ptr<TypeRef> &&type)
   : TemplateSpecilizationConstraint(kind, std::move(argName)),
     type(type)
{

}

const std::shared_ptr<TypeRef> &TypeConstraint::getType() const
{
   return type;
}

// Inheritance Constraint

InheritanceConstraint::InheritanceConstraint(
   std::shared_ptr<TypeRef> &&type, std::string &&argName,
   InheritanceKind kind)
   : TypeConstraint(kind == IK_Sub ? TSC_SubType
                                   : kind == IK_Super ? TSC_SuperType
                                                      : TSC_TypeEquality,
                    std::move(argName), std::move(type)),
     inheritance_kind(kind)
{

}

void InheritanceConstraint::accept(AbstractPass *pass)
{
   pass->visit(this);
}

bool InheritanceConstraint::appliesTo(const TemplateArg &arg)
{
   assert(arg.isTypeName() && "expected type");
   switch (inheritance_kind) {
      case IK_Equal: {
         return *type->getType() == arg.getGenericTy()->getActualType();
      }
      case IK_Sub: {

      }
      case IK_Super: {

      }
      default:
         llvm_unreachable("unsupported constraint kind");
   }
}

// Conformance Constraint

ConformanceConstraint::ConformanceConstraint(
   std::shared_ptr<TypeRef> &&type, std::string &&argName)
   : TypeConstraint(TSC_ProtocolConformance, std::move(argName),
                    std::move(type))
{

}

void ConformanceConstraint::accept(AbstractPass *pass)
{
   pass->visit(this);
}

bool ConformanceConstraint::appliesTo(const TemplateArg &arg)
{
   return false;
}

/// Value Constraints

ValueConstraint::ValueConstraint(
   Kind kind, std::string &&argName, std::shared_ptr<Expression> &&expr)
   : TemplateSpecilizationConstraint(kind, std::move(argName)), expr(expr)
{

}

const std::shared_ptr<Expression> &ValueConstraint::getExpr() const
{
   return expr;
}

// Value Expression Constraint

ValueExprConstraint::ValueExprConstraint(
   std::string &&argName, std::shared_ptr<Expression> &&expr)
   : ValueConstraint(TSC_ValueExpression, std::move(argName), std::move(expr))
{

}

void ValueExprConstraint::accept(AbstractPass *pass)
{
   pass->visit(this);
}

bool ValueExprConstraint::appliesTo(const TemplateArg &arg)
{
   return false;
}

/// Token constraints

TokenConstraint::TokenConstraint(
   Kind kind, std::string &&argName, std::vector<Token> &&tokens)
   : TemplateSpecilizationConstraint(kind, std::move(argName)), tokens(tokens)
{

}

const std::vector<Token> &TokenConstraint::getTokens() const
{
   return tokens;
}

// Token Equality Constraint

TokenEqualityConstraint::TokenEqualityConstraint(
   std::string &&argName, std::vector<Token> &&tokens)
   : TokenConstraint(TSC_TokenEquality, std::move(argName), std::move(tokens))
{

}

void TokenEqualityConstraint::accept(AbstractPass *pass)
{
   pass->visit(this);
}

bool TokenEqualityConstraint::appliesTo(const TemplateArg &arg)
{
   return false;
}

} // namespace cdot