//
// Created by Jonas Zell on 21.06.17.
//

#ifndef CDOT_CALLEXPR_H
#define CDOT_CALLEXPR_H


#include "../Expression.h"

namespace cdot {
   class FunctionType;
   enum class BuiltinFn : unsigned int;
   struct Argument;
   class TemplateArgList;

   namespace cl {
      class Class;
      class Method;
      struct EnumCase;
   }

}

struct CallCompatability;

using cdot::cl::EnumCase;

enum class CallType : unsigned int {
   METHOD_CALL,
   FUNC_CALL,
   CONSTR_CALL,
   ANON_CALL
};

class CallExpr : public Expression {
public:
   CallExpr(CallType kind,
            std::vector<pair<string, Expression::SharedPtr>> &&args,
            string &&name = "");
   CallExpr(CallType kind,
            std::vector<Expression::SharedPtr> &&args,
            string &&name = "");

   void isPointerAccess(bool ptr)
   {
      is_pointer_access = ptr;
   }

   bool createsTemporary() override;

   typedef std::shared_ptr<CallExpr> SharedPtr;
   std::vector<AstNode::SharedPtr> get_children() override;

   NodeType get_type() override {
      return NodeType::CALL_EXPR;
   }

   void replaceChildWith(AstNode *child, Expression *replacement) override;

   ASTNODE_ACCEPT_PASSES
   ADD_FRIEND_PASSES

protected:
   CallType type;
   std::vector<pair<string, Expression::SharedPtr>> args;

   // codegen
   bool is_call_op = false;
   bool load_before_call = false;
   string callOpBinding;

   bool is_captured_var = false;
   bool has_struct_return = false;

   bool is_pointer_access = false;
   CallCompatability* compatibility = nullptr;

   // method call
   bool is_ns_member = false;
   bool is_static = false;
   Type returnType;
   bool is_virtual = false;

   bool is_builtin = false;
   BuiltinFn builtinFnKind;
   size_t alignment;

   string className;
   union {
      cdot::cl::Method *method;
      Function *func;
   };

   bool anonymous_call = false;
   size_t anonymousFieldIndex;

   TemplateArgList *templateArgs;

   std::vector<Argument> resolvedArgs;
   std::vector<Argument> declaredArgTypes;

   FunctionType* functionType;
   bool implicitSelfCall = false;
   string selfBinding;

   bool is_protocol_call = false;
   bool castToBase = false;

   bool unionConstr = false;

public:
   CallType getType() const;
   void setType(CallType type);

   std::vector<pair<string, std::shared_ptr<Expression>>> &getArgs();
   void setArgs(
      const std::vector<pair<string, std::shared_ptr<Expression>>> &args);

   bool isCallOp() const;
   void isCallOp(bool isCallOp);

   bool loadBeforeCall() const;
   void loadBeforeCall(bool load_before_call);

   const string &getCallOpBinding() const;
   void setCallOpBinding(const string &callOpBinding);

   bool isCapturedVar() const;
   void setIsCapturedVar(bool isCapturedVar);

   bool hasStructReturn() const;
   void setHasStructReturn(bool hasStructReturn);

   bool isPointerAccess_() const;
   void setIsPointerAccess_(bool isPointerAccess_);

   CallCompatability *getCompatibility() const;
   void setCompatibility(CallCompatability *compatibility);

   bool isNsMember() const;
   void setIsNsMember(bool isNsMember);

   bool isStatic() const;
   void setIsStatic(bool isStatic);

   const Type &getReturnType() const;
   void setReturnType(const Type &returnType);

   bool isVirtual() const;
   void setIsVirtual(bool is_virtual);

   bool isBuiltin() const;
   void setIsBuiltin(bool isBuiltin);

   BuiltinFn getBuiltinFnKind() const;
   void setBuiltinFnKind(BuiltinFn builtinFnKind);

   size_t getAlignment() const;
   void setAlignment(size_t alignment);

   const string &getClassName() const;
   void setClassName(const string &className);

   cl::Method *getMethod() const;
   void setMethod(cl::Method *method);

   bool isAnonymousCall() const;
   void setIsAnonymousCall(bool isAnonymousCall_);

   TemplateArgList *&getTemplateArgs();
   void setTemplateArgs(TemplateArgList *templateArgs);

   std::vector<Argument> &getResolvedArgs();
   void setResolvedArgs(const std::vector<Argument> &resolvedArgs);

   std::vector<Argument> &getDeclaredArgTypes();
   void setDeclaredArgTypes(std::vector<Argument> &&declaredArgTypes);

   FunctionType *getFunctionType() const;
   void setFunctionType(FunctionType *functionType);

   bool isImplicitSelfCall() const;
   void setImplicitSelfCall(bool implicitSelfCall);

   const string &getSelfBinding() const;
   void setSelfBinding(const string &selfBinding);

   bool isProtocolCall() const;
   void setIsProtocolCall(bool isProtocolCall);

   bool isCastToBase() const;
   void setCastToBase(bool castToBase);

   size_t getAnonymousFieldIndex() const;
   void setAnonymousFieldIndex(size_t anonymousFieldIndex);

   bool isUnionConstr() const;
   void setUnionConstr(bool unionConstr);
};


#endif //CDOT_CALLEXPR_H
