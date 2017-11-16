//
// Created by Jonas Zell on 20.06.17.
//

#include "MemberRefExpr.h"
#include "../../SymbolTable.h"
#include "../../Passes/SemanticAnalysis/Record/Class.h"
#include "../../Passes/SemanticAnalysis/Record/Enum.h"

MemberRefExpr::MemberRefExpr(string &&ident,
                             bool pointerAccess)
   : is_pointer_access(pointerAccess),
     templateArgs(new ResolvedTemplateArgList({}))
{
   this->ident = ident;
}

MemberRefExpr::MemberRefExpr(size_t index, bool pointerAccess) :
   tupleIndex(index),
   is_tuple_access(true),
   is_pointer_access(pointerAccess),
   templateArgs(new ResolvedTemplateArgList({}))
{

}

void MemberRefExpr::replaceChildWith(
   AstNode *child,
   Expression *replacement)
{
   if (memberExpr.get() == child) {
      memberExpr.reset(replacement);
   }

   llvm_unreachable("child does not exist");
}

std::vector<AstNode::SharedPtr> MemberRefExpr::get_children() {
   if (memberExpr != nullptr) {
      return { memberExpr };
   }
   else {
      return { };
   }
}

const string &MemberRefExpr::getClassName() const
{
   return className;
}

void MemberRefExpr::setClassName(const string &className)
{
   MemberRefExpr::className = className;
}

const Type &MemberRefExpr::getFieldType() const
{
   return fieldType;
}

void MemberRefExpr::setFieldType(const Type &fieldType)
{
   MemberRefExpr::fieldType = fieldType;
}

size_t MemberRefExpr::getTupleIndex() const
{
   return tupleIndex;
}

void MemberRefExpr::setTupleIndex(size_t tupleIndex)
{
   MemberRefExpr::tupleIndex = tupleIndex;
}

bool MemberRefExpr::isStatic() const
{
   return is_static;
}

void MemberRefExpr::setIsStatic(bool is_static)
{
   MemberRefExpr::is_static = is_static;
}

bool MemberRefExpr::isTupleAccess() const
{
   return is_tuple_access;
}

void MemberRefExpr::setIsTupleAccess(bool is_tuple_access)
{
   MemberRefExpr::is_tuple_access = is_tuple_access;
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

bool MemberRefExpr::isNsMember() const
{
   return is_ns_member;
}

void MemberRefExpr::setIsNsMember(bool is_ns_member)
{
   MemberRefExpr::is_ns_member = is_ns_member;
}

bool MemberRefExpr::isEnumRawValue() const
{
   return is_enum_raw_value;
}

void MemberRefExpr::setIsEnumRawValue(bool is_enum_raw_value)
{
   MemberRefExpr::is_enum_raw_value = is_enum_raw_value;
}

bool MemberRefExpr::isPointerAccess() const
{
   return is_pointer_access;
}

void MemberRefExpr::setIsPointerAccess(bool is_pointer_access)
{
   MemberRefExpr::is_pointer_access = is_pointer_access;
}

bool MemberRefExpr::isUnionAccess() const
{
   return is_union_access;
}

void MemberRefExpr::setIsUnionAccess(bool is_union_access)
{
   MemberRefExpr::is_union_access = is_union_access;
}

BuiltinType *MemberRefExpr::getMetaType() const
{
   return metaType;
}

void MemberRefExpr::setMetaType(BuiltinType *metaType)
{
   MemberRefExpr::metaType = metaType;
}

TemplateArgList *&MemberRefExpr::getTemplateArgs()
{
   return templateArgs;
}

void MemberRefExpr::setTemplateArgs(TemplateArgList *templateArgs)
{
   delete MemberRefExpr::templateArgs;
   MemberRefExpr::templateArgs = templateArgs;
}
