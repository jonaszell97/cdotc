//
// Created by Jonas Zell on 20.06.17.
//

#include "MemberRefExpr.h"
#include "../../../Variant/Type/Generic.h"

namespace cdot {
namespace ast {

MemberRefExpr::MemberRefExpr(string &&ident,
                             bool pointerAccess)
   : IdentifiedExpr(MemberRefExprID, move(ident)),
     pointerAccess(pointerAccess)
{

}

MemberRefExpr::MemberRefExpr(size_t index, bool pointerAccess)
   : IdentifiedExpr(MemberRefExprID, ""),
     pointerAccess(pointerAccess),
     tupleAccess(true),
     tupleIndex(index)
{

}

MemberRefExpr::~MemberRefExpr() = default;

void MemberRefExpr::setTemplateArgs(std::vector<TemplateArgExpr*> &&templateArgs)
{
   MemberRefExpr::templateArgs = std::move(templateArgs);
}

} // namespace ast
} // namespace cdot