//
// Created by Jonas Zell on 12.08.17.
//

#include "Function.h"

#include "../../Statement/Declaration/Class/MethodDecl.h"
#include "../../SymbolTable.h"
#include "Record/Record.h"

using namespace cdot::ast;

namespace cdot {

Callable::Callable(
   string &&name,
   AccessModifier am,
   const QualType &returnType,
   std::vector<Argument> &&arguments) : name(name), accessModifier(am),
                                        returnType(returnType),
                                        arguments(arguments)
{

}

Callable::Callable(cl::CallableTemplate *Template)
   : name(string(Template->funcName)),
     Template(Template),
     is_template(true)
{

}

namespace cl {

Method::Method(
   string name,
   const QualType& ret_type,
   AccessModifier access_modifier,
   std::vector<Argument>&& args,
   bool isStatic,
   MethodDecl* declaration,
   SourceLocation loc,
   unsigned id
) : Callable(std::move(name), access_modifier, ret_type,
             std::move(args)),
   is_static(isStatic), methodID(id), loc(loc),
   hasDefinition(declaration == nullptr || declaration->hasDefinition())
{
   this->declaration = declaration;
}

Method::Method(MethodTemplate *Template) : Callable(Template)
{

}

Method::Method(
   string name,
   const QualType& ret_type,
   std::vector<Argument>&& args,
   MethodDecl*declaration,
   SourceLocation loc,
   unsigned id
) : Callable(std::move(name), AccessModifier::PUBLIC, ret_type,
             std::move(args)),
   is_static(false),
   hasDefinition(declaration == nullptr || declaration->hasDefinition()),
   loc(loc), methodID(id)
{
   this->declaration = declaration;
}

string Method::getLinkageName() const
{
   return owningClass->getName() + '.' + getMangledName();
}

Method::~Method()
{
   delete Template;
}

AstNode* Method::getTemplateOrMethodDecl()
{
   return is_template
          ? (AstNode*)static_cast<MethodTemplate*>(Template)->methodDecl
          : (AstNode*)declaration;
}

}

void Callable::copyThrows(Callable *callable)
{
   addThrownTypes(callable->thrownTypes);
}

void Callable::addThrownTypes(const std::vector<Type*> &tys)
{
   thrownTypes.insert(thrownTypes.begin(), tys.begin(), tys.end());
}

void Callable::addThrownType(Type *ty)
{
   if (throws(ty)) {
      return;
   }

   thrownTypes.push_back(ty);
}

bool Callable::throws(Type *ty)
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

const std::vector<Type*>& Callable::getThrownTypes() const
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

string Callable::getLinkageName() const
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

QualType &Callable::getReturnType()
{
   return returnType;
}

void Callable::setReturnType(const QualType &returnType)
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

namespace ast {

Function::Function(string& name, const QualType& ret_type)
   : Callable(std::move(name), AccessModifier::PUBLIC, ret_type, {})
{

}

Function::Function(cl::CallableTemplate *Template) : Callable(Template)
{

}

Function::~Function()
{
   delete Template;
}

AstNode* Function::getTemplateOrFunctionDecl() const
{
   return is_template ? (AstNode*)Template->decl
                      : (AstNode*)declaration;
}

} // namespace ast

} // namespace cdot