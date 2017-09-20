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
   struct MethodResult;
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

   ~CallExpr() override;

   void set_generics(std::vector<Type*> generics) {
      this->generics = generics;
   }

   void isPointerAccess(bool ptr) {
      isPointerAccess_ = ptr;
   }

   typedef std::shared_ptr<CallExpr> SharedPtr;
   std::vector<AstNode::SharedPtr> get_children() override;
   void __dump(int) override;

   NodeType get_type() override {
      return NodeType::CALL_EXPR;
   }

   llvm::Value* accept(CodeGen& v) override {
      return v.visit(this);
   }

   Type* accept(TypeCheckPass& v) override {
      return v.visit(this);
   }

   void accept(DeclPass &v) override {
      v.visit(this);
   }

   Variant accept(ConstExprPass &v) override {
      return v.visit(this);
   }

   friend class ConstExprPass;
   friend class CodeGen;
   friend class TypeCheckPass;
   friend class DeclPass;

protected:
   CallType type;
   std::vector<pair<string, Expression::SharedPtr>> args;

   // codegen
   bool isCallOp = false;
   string callOpBinding;

   bool isCapturedVar = false;
   Type* capturedType; // unowned

   bool hasHiddenParamReturn = false;
   Type* hiddenParamType = nullptr;

   bool isPointerAccess_ = false;

   union {
      cdot::cl::MethodResult *methodRes = nullptr;
      FunctionResult *functionRes;
   };

   // method call
   bool isNsMember = false;
   bool isStatic = false;
   Type* returnType = nullptr;
   bool is_virtual = false;

   bool isBuiltin = false;
   BuiltinFn builtinType;
   size_t alignment;

   string className;
   cdot::cl::Method* method;

   Type* genericOriginTy = nullptr;
   Type* genericDestTy = nullptr;
   bool needsGenericCast = false;

   std::vector<Type*> generics;
   std::vector<Argument> resolvedArgs;

   std::vector<Argument>* declaredArgTypes = nullptr;

   FunctionType* functionType; // unowned
   bool implicitSelfCall = false;
   string selfBinding;

   bool reverseProtoShift = false;
   bool castToBase = false;
};


#endif //CDOT_CALLEXPR_H
