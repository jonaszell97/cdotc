//
// Created by Jonas Zell on 20.06.17.
//

#ifndef CDOT_MEMBERREFEXPR_H
#define CDOT_MEMBERREFEXPR_H

#include "../Expression.h"

namespace cdot {
namespace ast {

class CallExpr;

class MemberRefExpr : public Expression {
public:
   explicit MemberRefExpr(string &&, bool pointerAccess = false);
   explicit MemberRefExpr(size_t, bool pointerAccess = false);

   typedef std::shared_ptr<MemberRefExpr> SharedPtr;

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == MemberRefExprID;
   }

protected:
   TemplateArgList *templateArgs;

   // codegen
   string className;
   QualType fieldType;
   bool is_static = false;

   Type *metaType = nullptr;

   bool is_tuple_access = false;
   size_t tupleIndex;

   std::shared_ptr<CallExpr> getterOrSetterCall = nullptr;

   bool is_ns_member = false;
   bool is_enum_raw_value = false;

   bool is_pointer_access = false;
   bool is_union_access = false;

public:
   bool isStatic() const;
   void setIsStatic(bool is_static);

   bool isTupleAccess() const;
   void setIsTupleAccess(bool is_tuple_access);

   const std::shared_ptr<CallExpr> &getGetterOrSetterCall() const;
   void setGetterOrSetterCall(
      const std::shared_ptr<CallExpr> &getterOrSetterCall);

   bool isNsMember() const;
   void setIsNsMember(bool is_ns_member);

   Type *getMetaType() const;
   void setMetaType(Type *metaType);

   bool isEnumRawValue() const;
   void setIsEnumRawValue(bool is_enum_raw_value);

   bool isPointerAccess() const;
   void setIsPointerAccess(bool is_pointer_access);

   bool isUnionAccess() const;
   void setIsUnionAccess(bool is_union_access);

   const string &getClassName() const;
   void setClassName(const string &className);

   const QualType &getFieldType() const;
   void setFieldType(const QualType &fieldType);

   size_t getTupleIndex() const;
   void setTupleIndex(size_t tupleIndex);

   TemplateArgList *&getTemplateArgs();
   void setTemplateArgs(TemplateArgList *templateArgs);
};

} // namespace ast
} // namespace cdot

#endif //CDOT_MEMBERREFEXPR_H
