//
// Created by Jonas Zell on 04.11.17.
//

#include "CallableTemplateDecl.h"

using namespace cdot::cl;

namespace cdot {
namespace ast {

CallableTemplateDecl::CallableTemplateDecl(
   string &&name, std::unique_ptr<TokenStore> &&Store,
   std::vector<TemplateConstraint> &&constraints,
   std::vector<std::shared_ptr<FuncArgDecl>> &&args,
   std::shared_ptr<TypeRef> &&returnType)
   : Statement(CallableTemplateDeclID),  name(name), Store(move(Store)),
     args(args), constraints(constraints), returnType(returnType)
{

}

const string &CallableTemplateDecl::getName() const
{
   return name;
}

void CallableTemplateDecl::setName(const string &name)
{
   CallableTemplateDecl::name = name;
}

const std::vector<TemplateConstraint> &
CallableTemplateDecl::getConstraints() const
{
   return constraints;
}

void CallableTemplateDecl::setConstraints(
   const std::vector<TemplateConstraint> &constraints)
{
   CallableTemplateDecl::constraints = constraints;
}

std::vector<std::shared_ptr<FuncArgDecl>> &
CallableTemplateDecl::getArgs()
{
   return args;
}

void
CallableTemplateDecl::setArgs(
   const std::vector<std::shared_ptr<FuncArgDecl>> &args)
{
   CallableTemplateDecl::args = args;
}

CallableTemplate *CallableTemplateDecl::getTempl() const
{
   return templ;
}

void CallableTemplateDecl::setTempl(CallableTemplate *templ)
{
   CallableTemplateDecl::templ = templ;
}

const std::vector<std::shared_ptr<Statement>> &
CallableTemplateDecl::getInstantiations() const
{
   return Instantiations;
}

void CallableTemplateDecl::setInstantiations(
   const std::vector<std::shared_ptr<Statement>> &Instantiations)
{
   CallableTemplateDecl::Instantiations = Instantiations;
}

void CallableTemplateDecl::addInstantiation(std::shared_ptr<Statement> &&inst)
{
   Instantiations.push_back(std::move(inst));
}

std::shared_ptr<TypeRef> &CallableTemplateDecl::getReturnType()
{
   return returnType;
}

void
CallableTemplateDecl::setReturnType(const std::shared_ptr<TypeRef> &returnType)
{
   CallableTemplateDecl::returnType = returnType;
}

std::unique_ptr<TokenStore> &CallableTemplateDecl::getStore()
{
   return Store;
}

} // namespace ast
} // namespace cdot
