//
// Created by Jonas Zell on 30.09.17.
//

#include <sstream>
#include "Generic.h"
#include "../../AST/SymbolTable.h"
#include "../../AST/Passes/SemanticAnalysis/Record/Class.h"
#include "../../AST/Expression/TypeRef.h"

#include "FunctionType.h"
#include "TupleType.h"
#include "GenericType.h"

using cdot::lex::Token;
using std::string;

using namespace cdot::ast;

namespace cdot {

string Argument::toString() const
{
   return cstyleVararg ? "..." : type.toString();
}

TemplateArg::TemplateArg(std::shared_ptr<TypeRef> &&ty)
   : type(move(ty))
{

}

TemplateArg::TemplateArg(std::shared_ptr<StaticExpr> &&staticExpr)
   : staticExpr(move(staticExpr)),
     kind(TemplateParameter::Value)
{

}

void TemplateArg::destroy()
{
   //FIXME
//   switch (kind) {
//      case TemplateParameter::TypeName:
//         type.~shared_ptr();
//         break;
//      case TemplateParameter::Value:
//         staticExpr.~shared_ptr();
//         break;
//      default:
//         llvm_unreachable("bad template arg kind");
//   }
}

TemplateArg::TemplateArg(TemplateArg &&arg) noexcept
   : sourceLoc(arg.sourceLoc),
     kind(arg.kind)
{
   switch (kind) {
      case TemplateParameter::TypeName:
         new (&type) std::shared_ptr<TypeRef>(move(arg.type));
         break;
      case TemplateParameter::Value:
         new (&staticExpr) std::shared_ptr<StaticExpr>(move(arg.staticExpr));
         break;
   }
}

TemplateArg& TemplateArg::operator=(TemplateArg &&arg) noexcept
{
   destroy();

   kind = arg.kind;
   sourceLoc = arg.sourceLoc;

   switch (kind) {
      case TemplateParameter::TypeName:
         new (&type) std::shared_ptr<TypeRef>(std::move(arg.type));
         break;
      case TemplateParameter::Value:
         new (&staticExpr)
            std::shared_ptr<StaticExpr>(std::move(arg.staticExpr));
         break;
   }

   return *this;
}

TemplateArg::~TemplateArg()
{
   destroy();
}

bool TemplateArg::isTypeName() const
{
   return kind == TemplateParameter::TypeName;
}

bool TemplateArg::isValue() const
{
   return kind == TemplateParameter::Value;
}

TemplateParameter::Kind TemplateArg::getKind() const
{
   return kind;
}

TemplateParameter::TemplateParameter(
   Kind kind, string &&typeName,
   std::shared_ptr<TypeRef> &&unresolvedCovariance,
   std::shared_ptr<TypeRef> &&unresolvedContravariance,
   std::shared_ptr<TemplateArg> &&defaultValue,
   bool isVariadic, const SourceLocation &loc)
      : kind(kind),
        genericTypeName(typeName),
        isVariadic(isVariadic),
        sourceLoc(loc),
        unresolvedCovariance(std::move(unresolvedCovariance)),
        unresolvedContravariance(std::move(unresolvedContravariance)),
        defaultValue(std::move(defaultValue))
{

}

void TemplateParameter::copyFrom(TemplateParameter &&TP)
{
   kind = TP.kind;
   genericTypeName = std::move(TP.genericTypeName);
   isVariadic = TP.isVariadic;
   sourceLoc = TP.sourceLoc;

   new (&unresolvedCovariance)
      std::shared_ptr<TypeRef>(std::move(TP.unresolvedCovariance));

   new (&unresolvedContravariance)
      std::shared_ptr<TypeRef>(std::move(TP.unresolvedContravariance));

   covariance = TP.covariance;
   contravariance = TP.contravariance;

   new (&defaultValue)
      std::shared_ptr<TemplateArg>(std::move(TP.defaultValue));

   resolved = TP.resolved;
}

void TemplateParameter::copyFrom(TemplateParameter const& TP)
{
   kind = TP.kind;
   genericTypeName = TP.genericTypeName;
   isVariadic = TP.isVariadic;
   sourceLoc = TP.sourceLoc;

   new (&unresolvedCovariance)
      std::shared_ptr<TypeRef>(TP.unresolvedCovariance);

   new (&unresolvedContravariance)
      std::shared_ptr<TypeRef>(TP.unresolvedContravariance);

   covariance = TP.covariance;
   contravariance = TP.contravariance;

   new (&defaultValue) std::shared_ptr<TemplateArg>(TP.defaultValue);

   resolved = TP.resolved;
}

void TemplateParameter::destroyValue()
{
   unresolvedCovariance.~shared_ptr();
   unresolvedContravariance.~shared_ptr();
   defaultValue.~shared_ptr();
}

TemplateParameter::TemplateParameter(TemplateParameter &&TP) noexcept
{
   copyFrom(std::move(TP));
}

TemplateParameter& TemplateParameter::operator=(TemplateParameter &&TP) noexcept
{
   destroyValue();
   copyFrom(std::move(TP));

   return *this;
}

TemplateParameter::TemplateParameter(TemplateParameter const& TP)
{
   copyFrom(TP);
}

TemplateParameter& TemplateParameter::operator=(TemplateParameter const& TP)
{
   destroyValue();
   copyFrom(TP);

   return *this;
}

TemplateParameter::~TemplateParameter()
{
   destroyValue();
}

bool TemplateParameter::operator!=(const TemplateParameter &rhs) const
{
   return !(*this == rhs);
}

bool TemplateParameter::operator==(const TemplateParameter &rhs) const
{
   if (kind != rhs.kind) {
      return false;
   }
   if (genericTypeName != rhs.genericTypeName) {
      return false;
   }

   assert(resolved && "unresolved template constraint");
   if (covariance && covariance != rhs.covariance) {
      return false;
   }
   if (contravariance && contravariance != rhs.contravariance) {
      return false;
   }

   return true;
}

bool
TemplateParameter::effectivelyEquals(const TemplateParameter &Other) const
{
   if (kind != Other.kind || isVariadic != Other.isVariadic)
      return false;

   if (isTypeName()) {
      if (covariance && (covariance != Other.covariance))
         return false;

      if (contravariance && (contravariance != Other.contravariance))
         return false;
   }
   else {
      if (valueType != Other.valueType)
         return false;
   }

   return true;
}

} // namespace cdot