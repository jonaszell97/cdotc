//
// Created by Jonas Zell on 30.09.17.
//

#include "Generic.h"
#include "../../AST/SymbolTable.h"
#include "../../AST/Passes/SemanticAnalysis/Record/Class.h"
#include "../../AST/Expression/TypeRef.h"

#include "FunctionType.h"
#include "TupleType.h"
#include "GenericType.h"

namespace cdot {

TemplateArg::TemplateArg()
   : type(std::make_shared<TypeRef>())
{

}

TemplateArg::TemplateArg(std::shared_ptr<TypeRef> &&ty)
   : type(ty)
{

}

TemplateArg::TemplateArg(std::vector<Token> &&tokens)
   : tokens(tokens), kind(TemplateConstraint::Arbitrary)
{

}

TemplateArg::TemplateArg(Variant &&val)
   : val(val), kind(TemplateConstraint::Value)
{

}

TemplateArg::TemplateArg(GenericType *ty)
   : genericType(ty), type_resolved(true)
{

}

TemplateArg::TemplateArg(std::vector<TemplateArg> &&args)
   : is_variadic(true), variadicArgs(args)
{


}

void TemplateArg::destroy()
{
   switch (kind) {
      case TemplateConstraint::TypeName: {
         if (is_variadic) {
            variadicArgs.~vector();
         }
         else if (!type_resolved) {
            type.~shared_ptr();
         }
         break;
      }
      case TemplateConstraint::Value: {
         val.~Variant();
         break;
      }
      case TemplateConstraint::Arbitrary: {
         tokens.~vector();
         break;
      }
   }
}

TemplateArg::TemplateArg(const TemplateArg &arg)
{
   kind = arg.kind;
   type_resolved = arg.type_resolved;
   is_variadic = arg.is_variadic;

   switch (kind) {
      case TemplateConstraint::TypeName: {
         if (is_variadic) {
            new (&variadicArgs) std::vector<TemplateArg>(arg.variadicArgs);
         }
         else if (!type_resolved) {
            new (&type) std::shared_ptr<TypeRef>(arg.type);
         }
         else {
            genericType = arg.genericType;
         }

         break;
      }
      case TemplateConstraint::Value: {
         new (&val) Variant(arg.val);
         break;
      }
      case TemplateConstraint::Arbitrary: {
         new (&tokens) std::vector<Token>(arg.tokens);
         break;
      }
   }
}

TemplateArg& TemplateArg::operator=(const TemplateArg &arg)
{
   destroy();
   *this = TemplateArg(arg);

   return *this;
}

TemplateArg::~TemplateArg()
{
   destroy();
}

bool TemplateArg::isTypeName() const
{
   return kind == TemplateConstraint::TypeName;
}

bool TemplateArg::isValue() const
{
   return kind == TemplateConstraint::Value;
}

bool TemplateArg::isArbitrary() const
{
   return kind == TemplateConstraint::Arbitrary;
}

bool TemplateArg::isVariadic() const
{
   return is_variadic;
}

void TemplateArg::resolveType(GenericType *ty)
{
   assert(isTypeName());
   type = nullptr;
   genericType = ty;
   type_resolved = true;
}

bool TemplateArg::isResolved() const
{
   return type_resolved;
}

TemplateConstraint::Kind TemplateArg::getKind() const
{
   return kind;
}

const std::vector<Token>& TemplateArg::getTokens() const
{
   assert(isArbitrary());
   return tokens;
}

std::shared_ptr<TypeRef>& TemplateArg::getType()
{
   assert(!type_resolved && isTypeName());
   return type;
}

std::vector<TemplateArg>& TemplateArg::getVariadicArgs()
{
   assert(is_variadic);
   return variadicArgs;
}

const std::vector<TemplateArg>& TemplateArg::getVariadicArgs() const
{
   assert(is_variadic);
   return variadicArgs;
}

GenericType *const TemplateArg::getGenericTy() const
{
   if (!type_resolved || !isTypeName()) {
      return nullptr;
   }

   return genericType;
}

const Variant& TemplateArg::getValue() const
{
   assert(isValue());
   return val;
}

ResolvedTemplateArgList::ResolvedTemplateArgList(
   std::vector<TemplateArg> &&args) : args(args)
{

}

bool ResolvedTemplateArgList::isResolved() const
{
   return true;
}

std::vector<TemplateArg>& ResolvedTemplateArgList::get()
{
   return args;
}

void ResolvedTemplateArgList::set(std::vector<TemplateArg> &&args)
{
   ResolvedTemplateArgList::args = std::move(args);
}

std::vector<Token>&& ResolvedTemplateArgList::getTokens()
{
   llvm_unreachable("already resolved");
}

UnresolvedTemplateArgList::UnresolvedTemplateArgList(
   std::vector<Token> &&tokens,
   size_t sourceId,
   size_t line,
   size_t col)
   : tokens(tokens)
{
   loc.sourceId = sourceId;
   loc.line = line;
   loc.col = col;
}

bool UnresolvedTemplateArgList::isResolved() const
{
   return false;
}

std::vector<TemplateArg>& UnresolvedTemplateArgList::get()
{
   llvm_unreachable("resolve first");
}

void UnresolvedTemplateArgList::set(std::vector<TemplateArg> &&args)
{
   llvm_unreachable("resolve first");
}

std::vector<Token>&& UnresolvedTemplateArgList::getTokens()
{
   return std::move(tokens);
}

TemplateConstraint::TemplateConstraint(
   Kind kind, string &&typeName,
   std::shared_ptr<TypeRef> &&unresolvedCovariance,
   std::shared_ptr<TypeRef> &&unresolvedContravariance,
   std::shared_ptr<TemplateArg> &&defaultValue,
   bool isVariadic)
      : kind(kind), genericTypeName(typeName),
        unresolvedCovariance(unresolvedCovariance),
        unresolvedContravariance(unresolvedContravariance),
        defaultValue(defaultValue), isVariadic(isVariadic)
{

}

TemplateConstraint::TemplateConstraint(
   const TemplateConstraint &rhs) : unresolvedContravariance(nullptr),
                                    unresolvedCovariance(nullptr)
{
   if (rhs.resolved) {
      covariance = rhs.covariance;
      contravariance = rhs.contravariance;
   }
   else {
      if (rhs.unresolvedCovariance) {
         new(&unresolvedCovariance)
            std::shared_ptr<TypeRef>(rhs.unresolvedCovariance);
      }
      if (rhs.unresolvedContravariance) {
         new(&unresolvedContravariance)
            std::shared_ptr<TypeRef>(rhs.unresolvedContravariance);
      }
   }

   kind = rhs.kind;
   defaultValue = rhs.defaultValue;
   genericTypeName = rhs.genericTypeName;
   resolved = rhs.resolved;
   isVariadic = rhs.isVariadic;
}

TemplateConstraint& TemplateConstraint::operator=(
   const TemplateConstraint &rhs)
{
   if (!resolved) {
      if (unresolvedCovariance) {
         unresolvedCovariance.~shared_ptr();
      }
      if (unresolvedContravariance) {
         unresolvedContravariance.~shared_ptr();
      }
   }

   if (rhs.resolved) {
      covariance = rhs.covariance;
      contravariance = rhs.contravariance;
   }
   else {
      if (rhs.unresolvedCovariance) {
         new(&unresolvedCovariance)
            std::shared_ptr<TypeRef>(rhs.unresolvedCovariance);
      }
      if (rhs.unresolvedContravariance) {
         new(&unresolvedContravariance)
            std::shared_ptr<TypeRef>(rhs.unresolvedContravariance);
      }
   }

   kind = rhs.kind;
   defaultValue = rhs.defaultValue;
   genericTypeName = rhs.genericTypeName;
   resolved = rhs.resolved;
   isVariadic = rhs.isVariadic;

   return *this;
}

TemplateConstraint::~TemplateConstraint()
{

}

bool TemplateConstraint::operator!=(const TemplateConstraint &rhs) const
{
   return !(*this == rhs);
}

bool TemplateConstraint::operator==(const TemplateConstraint &rhs) const
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

bool TemplateArg::compatibleWith(const TemplateArg &other) const
{
   if (kind != other.kind) {
      return false;
   }

   switch (kind) {
      case TemplateConstraint::TypeName: {
         return genericType->implicitlyCastableTo(other.genericType);
      }
      default:
         return Variant(tokens.front()._value).applyBinaryOp(
            other.tokens.front()._value, "==").intVal != 0;
   }
}

string TemplateArg::toString() const
{
   switch (kind) {
      case TemplateConstraint::TypeName: {
         if (is_variadic) {
            return
               util::vectorToString<TemplateArg, '\0', ',', '\0'>(variadicArgs);
         }
         else if (!type_resolved) {
            return type->getType()->toUniqueString();
         }

         return genericType->getActualType()->toUniqueString();
      }
      case TemplateConstraint::Value:
         return val.toString();
      default:
         return Token::TokensToString(tokens);
   }
}

bool GenericTypesCompatible(
   GenericType* given,
   const TemplateConstraint& needed)
{
   if (needed.kind != TemplateConstraint::TypeName) {
      return false;
   }

   auto actual = given->getActualType();

   assert(needed.resolved && "unresolved template constraint");
   if (needed.covariance) {
      if (!needed.covariance->isObject()) {
         return needed.covariance == actual;
      }
      if (!actual->isObject()) {
         return false;
      }

      auto Covar = needed.covariance->asObjTy()->getRecord();
      auto Given = actual->getRecord();

      if (Covar->isProtocol()) {
         return Given->conformsTo(Covar->getName());
      }

      if (Covar->isStruct() || Covar->isEnum() || Covar->isUnion()) {
         return Given->getName() == Covar->getName();
      }

      if (!Given->isClass()) {
         return false;
      }

      auto CovarCl = Covar->getAs<Class>();
      auto GivenCl = Given->getAs<Class>();

      return CovarCl->isBaseClassOf(GivenCl->getName());
   }

   if (needed.contravariance) {
      assert(needed.covariance->isObject());

      auto Covar = needed.covariance->asObjTy()->getRecord();
      assert(Covar->isClass());

      if (!actual->isObject() && actual->getRecord()->isClass()) {
         return false;
      }

      auto CovarCl = Covar->getAs<Class>();
      auto GivenCl = actual->getRecord()->getAs<Class>();

      return GivenCl->isBaseClassOf(CovarCl->getName());
   }

   return true;
}

// --- forward declarations ---

BuiltinType *resolveGenericTy(
   GenericType *ty,
   const std::vector<TemplateArg>& generics
);

BuiltinType *resolveFunctionTy(
   FunctionType *ty,
   const std::vector<TemplateArg>& generics
);

BuiltinType *resolveTupleTy(
   TupleType *ty,
   const std::vector<TemplateArg>& generics
);

BuiltinType *resolveObjectTy(
   ObjectType *ty,
   const std::vector<TemplateArg>& generics
);

// -----------------------------

BuiltinType *resolveGenerics(
   BuiltinType *ty,
   const std::vector<TemplateArg>& generics)
{
   switch (ty->getTypeID()) {
      case TypeID::ObjectTypeID:
         return resolveObjectTy(ty->asObjTy(), generics);
      case TypeID::FunctionTypeID:
         return resolveFunctionTy(ty->asFunctionTy(), generics);
      case TypeID::TupleTypeID:
         return resolveTupleTy(ty->asTupleTy(), generics);
      case TypeID::GenericTypeID:
         return resolveGenericTy(ty->asGenericTy(), generics);
      default:
         return ty;
   }
}

void resolveGenerics(
   Type& ty,
   const std::vector<TemplateArg>& generics)
{
   *ty = resolveGenerics(*ty, generics);
}

BuiltinType *resolveGenericTy(
   GenericType *ty,
   const std::vector<TemplateArg>& generics)
{
   for (const auto& gen : generics) {
      if (gen.getKind() != TemplateConstraint::TypeName) {
         continue;
      }

      if (gen.getGenericTy()->getClassName()
          == ty->getClassName()) {
         return resolveGenerics(gen.getGenericTy()->getActualType(), generics);
      }
   }

   return ty;
}

BuiltinType *resolveObjectTy(
   ObjectType *ty,
   const std::vector<TemplateArg>& generics)
{
   std::vector<TemplateArg> concreteGenerics;
   if (ty->isDummyObject()) {
      for (const auto &gen : ty->getTemplateArgs()) {
         if (!gen.isTypeName()) {
            concreteGenerics.push_back(gen);
            continue;
         }

         auto actualTy = resolveGenerics(gen.getGenericTy()->getActualType(),
                                         generics);

         concreteGenerics.emplace_back(
            GenericType::get(gen.getGenericTy()->getClassName(),
                             actualTy));
      }
   }

   string className = ty->getClassName();
   if (!concreteGenerics.empty()) {
      className += util::TemplateArgsToString(generics);
   }

   return ObjectType::get(className);
}

BuiltinType *resolveFunctionTy(
   FunctionType *ty,
   const std::vector<TemplateArg>& generics)
{
   Type retType(ty->getReturnType());
   std::vector<Argument> argTypes;

   *retType = resolveGenerics(*ty->getReturnType(), generics);
   for (auto& arg : ty->getArgTypes()) {
      Argument newArg(arg);
      *newArg.type = resolveGenerics(*arg.type, generics);
   }

   return FunctionType::get(retType, argTypes, ty->isRawFunctionTy());
}

BuiltinType *resolveTupleTy(
   TupleType *ty,
   const std::vector<TemplateArg>& generics)
{
   std::vector<pair<string, BuiltinType*>> containedTypes;
   for (auto &cont : ty->getContainedTypes()) {
      containedTypes.emplace_back(
         cont.first,
         resolveGenerics(cont.second, generics)
      );
   }

   return TupleType::get(containedTypes);
}

}