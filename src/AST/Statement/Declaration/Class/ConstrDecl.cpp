//
// Created by Jonas Zell on 27.06.17.
//

#include "ConstrDecl.h"

#include "../../../Passes/SemanticAnalysis/Function.h"

namespace cdot {
namespace ast {

ConstrDecl::ConstrDecl()
   : CallableDecl(ConstrDeclID, AccessModifier::PUBLIC, {}, {}, {}, {}, {},
                  {}, {}),
     memberwise(true)
{

}

ConstrDecl::ConstrDecl(std::vector<std::shared_ptr<FuncArgDecl>> &&args,
                       AccessModifier am,
                       std::vector<TemplateParameter> &&TPs,
                       std::shared_ptr<CompoundStmt> &&body,
                       string &&name)
   : CallableDecl(ConstrDeclID, am, move(name), {}, std::move(args), move(TPs),
                  {}, move(body), {})
{

}

cdot::cl::Method* ConstrDecl::getMethod() const
{
   return !callable ? nullptr : support::cast<cl::Method>(callable);
}

} // namespace ast
} // namespace cdot