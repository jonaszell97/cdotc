//
// Created by Jonas Zell on 12.08.17.
//

#include "Function.h"

#include "../../Statement/Declaration/Class/MethodDecl.h"
#include "../../Statement/Declaration/CallableDecl.h"

#include "../../SymbolTable.h"
#include "Record/Record.h"

using namespace cdot::ast;

namespace cdot {

Callable::Callable(
   TypeID id,
   string &&name,
   AccessModifier am,
   const QualType &returnType,
   std::vector<Argument> &&arguments,
   std::vector<TemplateParameter> &&templateParams,
   OperatorInfo op,
   Namespace *declarationNamespace)
   : typeID(id),
     name(name),
     accessModifier(am),
     returnType(returnType),
     arguments(arguments),
     is_nothrow(false),
     externC(false),
     native(false),
     op(op),
     declarationNamespace(declarationNamespace),
     templateParams(move(templateParams))
{

}

namespace cl {

Method::Method(string name, const QualType& ret_type,
               AccessModifier access_modifier,
               std::vector<Argument>&& args,
               std::vector<TemplateParameter> &&templateParams,
               OperatorInfo op,
               bool isStatic,
               CallableDecl* declaration,
               SourceLocation loc,
               Namespace *declarationNamespace,
               size_t id)
   : Callable(MethodID, std::move(name), access_modifier, ret_type,
             std::move(args), move(templateParams), op, declarationNamespace),
     methodID(id),
     is_static(isStatic),
     is_protocol_method(false),
     is_virtual(false),
     mutableSelf(false),
     property(false),
     is_initializer(false),
     protocolDefaultImpl(false),
     hasDefinition(declaration == nullptr || declaration->hasDefinition()),
     conversionOp(false),
     memberwiseInitializer(false)
{
   this->loc = loc;
   this->declaration = declaration;
}

bool Method::hasMutableSelf() const
{
   return mutableSelf || (is_initializer && !owningRecord->isClass());
}

bool Method::isTemplatedInitializer() const
{
   return templateParams.size() > owningRecord->getTemplateParams().size();
}

Method* Method::hasInstantiation(const std::string &withName) const
{
   auto fullName = name + withName;
   for (const auto &Inst : Instantiations) {
      if (Inst->getCallable()->getName() == fullName)
         return support::cast<Method>(Inst->getCallable());
   }

   return nullptr;
}

} // namespace cl

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

llvm::StringRef Callable::getNameWithoutNamespace() const
{
   if (!declarationNamespace || support::isa<Method>(this))
      return name;

   auto nslen = declarationNamespace->name.length() + 1;
   return llvm::StringRef(name.data() + nslen, name.length() - nslen);
}

llvm::StringRef Callable::getNameWithoutFix() const
{
   if (!isOperator())
      return getNameWithoutNamespace();

   size_t opLen;
   switch (op.getFix()) {
      case FixKind::Infix:
         opLen = 6; // "infix "
         break;
      case FixKind::Prefix:
         opLen = 7; // "prefix "
         break;
      case FixKind::Postfix:
         opLen = 8; // "postfix "
         break;
   }

   return getNameWithoutNamespace().substr(opLen);
}

const string &Callable::getLinkageName() const
{
   return linkageName;
}

void Callable::setLinkageName(const string &mandledName)
{
   Callable::linkageName = mandledName;
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

unsigned int Callable::getUses() const
{
   return uses;
}

void Callable::setUses(unsigned int uses)
{
   Callable::uses = uses;
}

namespace ast {

Function::Function(string &funcName, const QualType &returnType,
                   std::vector<Argument> &&args,
                   std::vector<TemplateParameter> &&templateParams,
                   OperatorInfo op,
                   Namespace *declarationNamespace)
   : Callable(FunctionID,move(funcName), AccessModifier::PUBLIC, returnType,
              move(args), move(templateParams), op, declarationNamespace)
{

}

Function::Function(llvm::StringRef funcName,
                   std::vector<TemplateParameter> &&templateParams,
                   OperatorInfo op, Namespace *declarationNamespace)

   : Callable(FunctionID, funcName.str(), AccessModifier::PUBLIC, {},
              {}, move(templateParams), op, declarationNamespace)
{

}

Function* Function::hasInstantiation(const std::string &withName) const
{
   auto fullName = name + withName;
   for (const auto &Inst : Instantiations) {
      if (Inst->getCallable()->getName() == fullName)
         return support::cast<Function>(Inst->getCallable());
   }

   return nullptr;
}

} // namespace ast

} // namespace cdot