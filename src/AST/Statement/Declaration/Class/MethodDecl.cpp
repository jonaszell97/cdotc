//
// Created by Jonas Zell on 27.06.17.
//

#include "MethodDecl.h"
#include "../../../Expression/TypeRef.h"
#include "../../Block/CompoundStmt.h"
#include "../../Declaration/FuncArgDecl.h"
#include "../../../../Util.h"

namespace cdot {
namespace ast {

MethodDecl::MethodDecl(
   string &&name,
   TypeRef::SharedPtr &&returnType,
   std::vector<FuncArgDecl::SharedPtr> &&args,
   CompoundStmt::SharedPtr &&body,
   AccessModifier am,
   bool is_static
) : CallableDecl(MethodDeclID, am, std::move(name), std::move(returnType),
                 std::move(args)),
    isStatic(is_static),
    isAbstract(false),
    hasDefinition_(body != nullptr)
{
   this->body = body;
}

MethodDecl::MethodDecl(
   string &&name,
   TypeRef::SharedPtr &&returnType,
   std::vector<FuncArgDecl::SharedPtr> &&args,
   AccessModifier am,
   bool is_static
) : CallableDecl(MethodDeclID, am, std::move(name), std::move(returnType),
                 std::move(args)),
   isStatic(is_static),
   isAbstract(true),
   hasDefinition_(body != nullptr)
{

}

MethodDecl::MethodDecl(
   string &&alias,
   string &&originMethod,
   std::vector<std::shared_ptr<FuncArgDecl>> &&args
) : CallableDecl(MethodDeclID, AccessModifier::PUBLIC, std::move(originMethod),
                 {}, std::move(args)),
   alias(alias),
   isAlias(true),
   hasDefinition_(body != nullptr)
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

} // namespace ast
} // namespace cdot