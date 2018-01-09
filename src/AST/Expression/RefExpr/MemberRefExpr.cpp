//
// Created by Jonas Zell on 20.06.17.
//

#include "MemberRefExpr.h"

namespace cdot {
namespace ast {

MemberRefExpr::MemberRefExpr(string &&ident,
                             bool pointerAccess)
   : IdentifiedExpr(MemberRefExprID, move(ident)),
     is_pointer_access(pointerAccess)
{

}

MemberRefExpr::MemberRefExpr(size_t index, bool pointerAccess)
   : IdentifiedExpr(MemberRefExprID, ""),
     is_pointer_access(pointerAccess),
     is_tuple_access(true),
     tupleIndex(index)
{

}

MemberRefExpr::~MemberRefExpr() = default;

const QualType &MemberRefExpr::getFieldType() const
{
   return fieldType;
}

void MemberRefExpr::setFieldType(const QualType &fieldType)
{
   MemberRefExpr::fieldType = fieldType;
}

size_t MemberRefExpr::getTupleIndex() const
{
   return tupleIndex;
}

const std::shared_ptr<CallExpr> &MemberRefExpr::getGetterOrSetterCall() const
{
   return getterOrSetterCall;
}

void MemberRefExpr::setGetterOrSetterCall(
   const std::shared_ptr<CallExpr> &getterOrSetterCall)
{
   MemberRefExpr::getterOrSetterCall = getterOrSetterCall;
}

bool MemberRefExpr::isPointerAccess() const
{
   return is_pointer_access;
}

void MemberRefExpr::setIsPointerAccess(bool is_pointer_access)
{
   MemberRefExpr::is_pointer_access = is_pointer_access;
}

Type *MemberRefExpr::getMetaType() const
{
   return metaType;
}

void MemberRefExpr::setMetaType(Type *metaType)
{
   MemberRefExpr::metaType = metaType;
}

} // namespace ast
} // namespace cdot