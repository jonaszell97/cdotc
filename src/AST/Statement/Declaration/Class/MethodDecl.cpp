//
// Created by Jonas Zell on 27.06.17.
//

#include "MethodDecl.h"
#include "../../../Expression/TypeRef.h"
#include "../../Block/CompoundStmt.h"
#include "../../Declaration/FuncArgDecl.h"
#include "../../../../Util.h"
#include "../../../Passes/SemanticAnalysis/Function.h"

namespace cdot {
namespace ast {

MethodDecl::MethodDecl(
   string &&name,
   TypeRef::SharedPtr &&returnType,
   std::vector<FuncArgDecl::SharedPtr> &&args,
   std::vector<TemplateParameter> &&templateParams,
   std::vector<std::shared_ptr<StaticExpr>> &&Constraints,
   CompoundStmt::SharedPtr &&body,
   AccessModifier am,
   bool is_static
) : CallableDecl(MethodDeclID, am, std::move(name), std::move(returnType),
                 std::move(args), move(templateParams), move(Constraints),
                 move(body), {}),
    isStatic(is_static),
    isAbstract(false),
    hasDefinition_(body != nullptr)
{

}

MethodDecl::MethodDecl(
   string &&name,
   TypeRef::SharedPtr &&returnType,
   std::vector<FuncArgDecl::SharedPtr> &&args,
   std::vector<TemplateParameter> &&templateParams,
   std::vector<std::shared_ptr<StaticExpr>> &&Constraints,
   CompoundStmt::SharedPtr &&body,
   OperatorInfo op,
   bool isCastOp,
   AccessModifier am,
   bool is_static
) : CallableDecl(MethodDeclID, am, std::move(name), std::move(returnType),
                 std::move(args), move(templateParams), move(Constraints),
                 move(body), op),
    isStatic(is_static),
    isAbstract(false),
    isCastOp_(isCastOp),
    hasDefinition_(body != nullptr)
{

}

MethodDecl::MethodDecl(
   string &&name,
   TypeRef::SharedPtr &&returnType,
   std::vector<FuncArgDecl::SharedPtr> &&args,
   std::vector<TemplateParameter> &&templateParams,
   std::vector<std::shared_ptr<StaticExpr>> &&Constraints,
   AccessModifier am,
   bool is_static
) : CallableDecl(MethodDeclID, am, std::move(name), std::move(returnType),
                 std::move(args), move(templateParams), move(Constraints),
                 nullptr, {}),
   isStatic(is_static),
   isAbstract(true)
{

}

MethodDecl::MethodDecl(
   string &&alias,
   string &&originMethod,
   std::vector<std::shared_ptr<FuncArgDecl>> &&args
) : CallableDecl(MethodDeclID, AccessModifier::PUBLIC,
                 std::move(originMethod),
                 {}, std::move(args), {}, {}, {}, {}),
    isAlias(true),
    alias(alias)
{

}

cl::Record *MethodDecl::getRecord() const
{
   return record;
}

void MethodDecl::setRecord(cl::Record *record)
{
   MethodDecl::record = record;
}

cdot::cl::Method* MethodDecl::getMethod() const
{
   return support::cast<cl::Method>(callable);
}

} // namespace ast
} // namespace cdot