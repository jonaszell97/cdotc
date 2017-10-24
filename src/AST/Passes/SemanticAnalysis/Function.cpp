//
// Created by Jonas Zell on 12.08.17.
//

#include "Function.h"

#include "../../Statement/Declaration/Class/MethodDecl.h"

namespace cdot {

Callable::Callable(
   string &&name,
   AccessModifier am,
   Type &&returnType,
   std::vector<Argument> &&arguments,
   std::vector<GenericConstraint> &&generics) : name(name), accessModifier(am),
                                                returnType(returnType), arguments(arguments),
                                                generics(generics)
{

}

namespace cl {

Method::Method(
   string name,
   Type& ret_type,
   AccessModifier access_modifier,
   std::vector<Argument>&& args,
   std::vector<GenericConstraint>& generics,
   bool isStatic,
   MethodDecl* declaration,
   SourceLocation loc
) : Callable(std::move(name), access_modifier, std::move(ret_type),
             std::move(args), std::move(generics)),
   isStatic(isStatic), declaration(declaration),
   hasDefinition(declaration == nullptr || declaration->hasDefinition()), loc(loc)
{

}

Method::Method(
   string name,
   Type& ret_type,
   std::vector<Argument>&& args,
   std::vector<GenericConstraint>& generics,
   MethodDecl*declaration,
   SourceLocation loc
) : Callable(std::move(name), AccessModifier::PUBLIC, std::move(ret_type),
             std::move(args), std::move(generics)),
   isStatic(false), declaration(declaration),
   hasDefinition(declaration == nullptr || declaration->hasDefinition()),
   loc(loc)
{

}

}

void Callable::copyThrows(Callable *callable)
{
   addThrownTypes(callable->thrownTypes);
}

void Callable::addThrownTypes(const std::vector<BuiltinType*> &tys)
{
   thrownTypes.insert(thrownTypes.begin(), tys.begin(), tys.end());
}

void Callable::addThrownType(BuiltinType *ty)
{
   if (throws(ty)) {
      return;
   }

   thrownTypes.push_back(ty);
}

bool Callable::throws(BuiltinType *ty)
{
   return std::find(thrownTypes.begin(), thrownTypes.end(), ty)
          != thrownTypes.end();
}

bool Callable::throws()
{
   return !thrownTypes.empty();
}

bool Callable::isNoThrow()
{
   return is_nothrow;
}

void Callable::isNoThrow(bool nothrow)
{
   is_nothrow = nothrow;
}

const std::vector<BuiltinType*>& Callable::getThrownTypes() const
{
   return thrownTypes;
}

const string &Callable::getName() const
{
   return name;
}

void Callable::setName(const string &name)
{
   Callable::name = name;
}

const string &Callable::getMangledName() const
{
   return mangledName;
}

void Callable::setMangledName(const string &mandledName)
{
   Callable::mangledName = mandledName;
}

AccessModifier Callable::getAccessModifier() const
{
   return accessModifier;
}

void Callable::setAccessModifier(AccessModifier accessModifier)
{
   Callable::accessModifier = accessModifier;
}

Type &Callable::getReturnType()
{
   return returnType;
}

void Callable::setReturnType(const Type &returnType)
{
   Callable::returnType = returnType;
}

std::vector<Argument> &Callable::getArguments()
{
   return arguments;
}

void Callable::setArguments(const std::vector<Argument> &arguments)
{
   Callable::arguments = arguments;
}

std::vector<GenericConstraint> &Callable::getGenerics()
{
   return generics;
}

void Callable::setGenerics(const std::vector<GenericConstraint> &generics)
{
   Callable::generics = generics;
}

bool Callable::hasStructReturn() const
{
   return has_struct_return;
}

void Callable::hasStructReturn(bool has_struct_return)
{
   Callable::has_struct_return = has_struct_return;
}

unsigned int Callable::getUses() const
{
   return uses;
}

void Callable::setUses(unsigned int uses)
{
   Callable::uses = uses;
}

llvm::Function *Callable::getLlvmFunc() const
{
   return llvmFunc;
}

void Callable::setLlvmFunc(llvm::Function *llvmFunc)
{
   Callable::llvmFunc = llvmFunc;
}
}

Function::Function(
   string& name,
   Type& ret_type,
   std::vector<GenericConstraint>& generics
) : Callable(std::move(name), AccessModifier::PUBLIC,
             std::move(ret_type), {}, std::move(generics))
{

}

Function::Function(
   string& name,
   Type& ret_type
) : Callable(std::move(name), AccessModifier::PUBLIC, std::move(ret_type), {}, {})
{

}