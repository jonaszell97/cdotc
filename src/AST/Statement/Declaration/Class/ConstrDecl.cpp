//
// Created by Jonas Zell on 27.06.17.
//

#include "ConstrDecl.h"
#include "../../Block/CompoundStmt.h"
#include "../FuncArgDecl.h"
#include "../../../../Util.h"

namespace cdot {
namespace ast {

ConstrDecl::ConstrDecl()
   : CallableDecl(ConstrDeclID, AccessModifier::PUBLIC, {}, {}, {}),
     memberwise(true)
{

}

ConstrDecl::ConstrDecl(std::vector<std::shared_ptr<FuncArgDecl>> &&args,
                       AccessModifier am)
   : CallableDecl(ConstrDeclID, am, {}, {}, std::move(args))
{

}

cl::Record *ConstrDecl::getRecord() const
{
   return record;
}

void ConstrDecl::setRecord(cl::Record *record)
{
   ConstrDecl::record = record;
}

} // namespace ast
} // namespace cdot