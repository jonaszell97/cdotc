//
// Created by Jonas Zell on 04.11.17.
//

#include "MethodTemplateDecl.h"

using std::move;
using namespace cdot::cl;

MethodTemplateDecl::MethodTemplateDecl(
   string &&name, std::unique_ptr<TokenStore> &&Store,
   std::vector<TemplateConstraint> &&constraints,
   std::vector<std::shared_ptr<FuncArgDecl>> &&args,
   std::shared_ptr<TypeRef> &&returnType,
   bool is_static)
   : CallableTemplateDecl(move(name), move(Store),
                          move(constraints), move(args), move(returnType)),
     is_static(is_static)
{

}

bool MethodTemplateDecl::isStatic() const
{
   return is_static;
}

void MethodTemplateDecl::isStatic(bool is_static)
{
   MethodTemplateDecl::is_static = is_static;
}

Method *MethodTemplateDecl::getMethod() const
{
   return method;
}

void MethodTemplateDecl::setMethod(Method *method)
{
   MethodTemplateDecl::method = method;
}

RecordTemplateInstantiation *MethodTemplateDecl::getOuterRecord() const
{
   return outerRecord;
}

void MethodTemplateDecl::setOuterRecord(
   RecordTemplateInstantiation *outerRecord)
{
   MethodTemplateDecl::outerRecord = outerRecord;
}

bool MethodTemplateDecl::isOperator() const
{
   return is_operator;
}

void MethodTemplateDecl::isOperator(bool is_operator)
{
   MethodTemplateDecl::is_operator = is_operator;
}

bool MethodTemplateDecl::isMutating() const
{
   return is_mutating;
}

void MethodTemplateDecl::isMutating(bool is_mutating)
{
   MethodTemplateDecl::is_mutating = is_mutating;
}
