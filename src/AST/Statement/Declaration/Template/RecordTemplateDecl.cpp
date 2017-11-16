//
// Created by Jonas Zell on 03.11.17.
//

#include "RecordTemplateDecl.h"

#include "../../../../Variant/Type/Generic.h"

using std::move;

RecordTemplateDecl::RecordTemplateDecl(
   RecordTemplateKind kind, string &&name,
   std::unique_ptr<TokenStore> &&Store,
   std::vector<TemplateConstraint> &&constraints,
   std::vector<Initializer> &&initializers) : kind(kind), Store(move(Store)),
                                              name(name),
                                              constraints(move(constraints)),
                                              initializers(move(initializers))
{

}

std::vector<TemplateConstraint> &RecordTemplateDecl::getConstraints()
{
   return constraints;
}

void RecordTemplateDecl::setConstraints(
   const std::vector<TemplateConstraint> &constraints)
{
   RecordTemplateDecl::constraints = constraints;
}

std::vector<RecordTemplateDecl::Initializer> &
RecordTemplateDecl::getInitializers()
{
   return initializers;
}

void RecordTemplateDecl::setInitializers(
   const std::vector<RecordTemplateDecl::Initializer> &initializers)
{
   RecordTemplateDecl::initializers = initializers;
}

RecordTemplateKind RecordTemplateDecl::getKind() const
{
   return kind;
}

void RecordTemplateDecl::setKind(RecordTemplateKind kind)
{
   RecordTemplateDecl::kind = kind;
}

const string &RecordTemplateDecl::getName() const
{
   return name;
}

void RecordTemplateDecl::setName(const string &name)
{
   RecordTemplateDecl::name = name;
}

const std::vector<std::shared_ptr<Statement>> &
RecordTemplateDecl::getInstantiations() const
{
   return Instantiations;
}

void RecordTemplateDecl::setInstantiations(
   const std::vector<std::shared_ptr<Statement>> &Instantiations)
{
   RecordTemplateDecl::Instantiations = Instantiations;
}

void RecordTemplateDecl::addInstantiation(std::shared_ptr<Statement> &&inst)
{
   Instantiations.push_back(std::move(inst));
}

cl::RecordTemplate *RecordTemplateDecl::getTempl() const
{
   return templ;
}

void RecordTemplateDecl::setTempl(cl::RecordTemplate *Template)
{
   RecordTemplateDecl::templ = Template;
}

cl::RecordTemplateInstantiation *RecordTemplateDecl::getOuterTemplate() const
{
   return outerTemplate;
}

void RecordTemplateDecl::setOuterTemplate(
   cl::RecordTemplateInstantiation *outerTemplate)
{
   RecordTemplateDecl::outerTemplate = outerTemplate;
}

std::unique_ptr<TokenStore> &RecordTemplateDecl::getStore()
{
   return Store;
}
