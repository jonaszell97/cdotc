//
// Created by Jonas Zell on 20.06.17.
//

#ifndef CDOT_MEMBERREFEXPR_H
#define CDOT_MEMBERREFEXPR_H

#include "../Expression.h"

class CallExpr;

class MemberRefExpr : public Expression {
public:
   explicit MemberRefExpr(string &&, bool pointerAccess = false);
   explicit MemberRefExpr(size_t, bool pointerAccess = false);

   typedef std::shared_ptr<MemberRefExpr> SharedPtr;
   std::vector<AstNode::SharedPtr> get_children() override;

   NodeType get_type() override {
      return NodeType::MEMBER_EXPR;
   }

   void replaceChildWith(AstNode *child, Expression *replacement) override;

   ASTNODE_ACCEPT_PASSES
   ADD_FRIEND_PASSES

protected:
   TemplateArgList *templateArgs;

   // codegen
   string className;
   Type fieldType;
   bool is_static = false;

   BuiltinType *metaType = nullptr;

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

   BuiltinType *getMetaType() const;
   void setMetaType(BuiltinType *metaType);

   bool isEnumRawValue() const;
   void setIsEnumRawValue(bool is_enum_raw_value);

   bool isPointerAccess() const;
   void setIsPointerAccess(bool is_pointer_access);

   bool isUnionAccess() const;
   void setIsUnionAccess(bool is_union_access);

   const string &getClassName() const;
   void setClassName(const string &className);

   const Type &getFieldType() const;
   void setFieldType(const Type &fieldType);

   size_t getTupleIndex() const;
   void setTupleIndex(size_t tupleIndex);

   TemplateArgList *&getTemplateArgs();
   void setTemplateArgs(TemplateArgList *templateArgs);
};


#endif //CDOT_MEMBERREFEXPR_H
