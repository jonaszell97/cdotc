//
// Created by Jonas Zell on 21.06.17.
//

#ifndef CDOT_CALLEXPR_H
#define CDOT_CALLEXPR_H


#include "../Expression.h"

namespace cdot {
   class FunctionType;
namespace cl {
   class Class;
   class Method;
   struct EnumCase;
}
}

using cdot::cl::EnumCase;

enum class CallType : unsigned int {
   METHOD_CALL,
   FUNC_CALL,
   CONSTR_CALL,
   ANON_CALL
};

class CallExpr : public Expression {
public:
   CallExpr(CallType, std::vector<pair<string, Expression::SharedPtr>>, string = "");
   CallExpr(CallType, std::vector<Expression::SharedPtr>, string = "");

   void set_generics(std::vector<std::shared_ptr<TypeRef>> generics) {
      this->generics = generics;
   }

   void isPointerAccess(bool ptr) {
      isPointerAccess_ = ptr;
   }

   bool createsTemporary() override;

   typedef std::shared_ptr<CallExpr> SharedPtr;
   std::vector<AstNode::SharedPtr> get_children() override;
   void __dump(int) override;

   NodeType get_type() override {
      return NodeType::CALL_EXPR;
   }

   llvm::Value* accept(CodeGen& v) override {
      return v.visit(this);
   }

   Type accept(SemaPass& v) override {
      return v.visit(this);
   }

   void accept(AbstractPass* v) override {
      v->visit(this);
   }

   Variant accept(ConstExprPass &v) override {
      return v.visit(this);
   }

   void replaceChildWith(AstNode *child, Expression *replacement) override;

   ADD_FRIEND_PASSES

protected:
   CallType type;
   std::vector<pair<string, Expression::SharedPtr>> args;

   // codegen
   bool isCallOp = false;
   string callOpBinding;

   bool isCapturedVar = false;

   bool hasStructReturn = false;
   BuiltinType* structReturnType = nullptr;

   bool isPointerAccess_ = false;
   CallCompatability* compatibility = nullptr;

   // method call
   bool isNsMember = false;
   bool isStatic = false;
   Type returnType;
   bool is_virtual = false;

   bool isBuiltin = false;
   BuiltinFn builtinFnKind;
   size_t alignment;

   string className;
   union {
      cdot::cl::Method *method;
      Function *func;
   };

   Type genericOriginTy;
   Type genericDestTy;
   bool needsGenericCast = false;
   bool anonymous_call = false;
   size_t anonymousFieldIndex;

   std::vector<std::shared_ptr<TypeRef>> generics;
   std::vector<GenericType*> resolvedGenerics;

   std::vector<Argument> resolvedArgs;

   std::vector<Argument>* declaredArgTypes = nullptr;

   FunctionType* functionType;
   bool implicitSelfCall = false;
   string selfBinding;

   bool isProtocolCall = false;
   bool castToBase = false;

   bool unionConstr = false;

public:
   CallType getType() const;

   void setType(CallType type);

   const std::vector<pair<string, std::shared_ptr<Expression>>> &getArgs() const;

   void setArgs(const std::vector<pair<string, std::shared_ptr<Expression>>> &args);

   bool isIsCallOp() const;

   void setIsCallOp(bool isCallOp);

   const string &getCallOpBinding() const;

   void setCallOpBinding(const string &callOpBinding);

   bool isIsCapturedVar() const;

   void setIsCapturedVar(bool isCapturedVar);

   bool isHasStructReturn() const;

   void setHasStructReturn(bool hasStructReturn);

   BuiltinType *getStructReturnType() const;

   void setStructReturnType(BuiltinType *structReturnType);

   bool isIsPointerAccess_() const;

   void setIsPointerAccess_(bool isPointerAccess_);

   CallCompatability *getCompatibility() const;

   void setCompatibility(CallCompatability *compatibility);

   bool isIsNsMember() const;

   void setIsNsMember(bool isNsMember);

   bool isIsStatic() const;

   void setIsStatic(bool isStatic);

   const Type &getReturnType() const;

   void setReturnType(const Type &returnType);

   bool isIs_virtual() const;

   void setIs_virtual(bool is_virtual);

   bool isIsBuiltin() const;

   void setIsBuiltin(bool isBuiltin);

   BuiltinFn getBuiltinFnKind() const;

   void setBuiltinFnKind(BuiltinFn builtinFnKind);

   size_t getAlignment() const;

   void setAlignment(size_t alignment);

   const string &getClassName() const;

   void setClassName(const string &className);

   Method *getMethod() const;

   void setMethod(Method *method);

   const Type &getGenericOriginTy() const;

   void setGenericOriginTy(const Type &genericOriginTy);

   const Type &getGenericDestTy() const;

   void setGenericDestTy(const Type &genericDestTy);

   bool isNeedsGenericCast() const;

   void setNeedsGenericCast(bool needsGenericCast);

   bool isAnonymousCall() const;

   void setIsAnonymousCall(bool isAnonymousCall_);

   const std::vector<std::shared_ptr<TypeRef>> &getGenerics() const;

   void setGenerics(const std::vector<std::shared_ptr<TypeRef>> &generics);

   const std::vector<GenericType *> &getResolvedGenerics() const;

   void setResolvedGenerics(const std::vector<GenericType *> &resolvedGenerics);

   const std::vector<Argument> &getResolvedArgs() const;

   void setResolvedArgs(const std::vector<Argument> &resolvedArgs);

   std::vector<Argument> *getDeclaredArgTypes() const;

   void setDeclaredArgTypes(std::vector<Argument> *declaredArgTypes);

   FunctionType *getFunctionType() const;

   void setFunctionType(FunctionType *functionType);

   bool isImplicitSelfCall() const;

   void setImplicitSelfCall(bool implicitSelfCall);

   const string &getSelfBinding() const;

   void setSelfBinding(const string &selfBinding);

   bool isIsProtocolCall() const;

   void setIsProtocolCall(bool isProtocolCall);

   bool isCastToBase() const;

   void setCastToBase(bool castToBase);

   size_t getAnonymousFieldIndex() const;

   void setAnonymousFieldIndex(size_t anonymousFieldIndex);

   bool isUnionConstr() const;

   void setUnionConstr(bool unionConstr);
};


#endif //CDOT_CALLEXPR_H
