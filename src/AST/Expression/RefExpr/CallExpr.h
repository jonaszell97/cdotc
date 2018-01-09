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
class TemplateArgListBuilder;

namespace cl {

class Class;
struct Method;
struct EnumCase;

} // namespace cl

} // namespace cdot

struct CallCompatability;

using cdot::cl::EnumCase;

namespace cdot {
namespace ast {

class IdentifierRefExpr;
class Function;

enum class CallKind : unsigned {
   Unknown,
   Builtin,
   NamedFunctionCall,
   MethodCall,
   UnionInitializer,
   InitializerCall,
   StaticMethodCall,
   CallOperator,
   AnonymousCall,
   UnsafeTupleGet,
   VariadicSizeof,
   PrimitiveInitializer
};

class CallExpr : public IdentifiedExpr {
public:
   explicit CallExpr(std::vector<std::shared_ptr<Expression>> &&args,
                     string &&name = "");

   void destroyValueImpl();

   void isPointerAccess(bool ptr)
   {
      is_pointer_access = ptr;
   }

   typedef std::shared_ptr<CallExpr> SharedPtr;

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == CallExprID;
   }

protected:
   CallKind kind = CallKind::Unknown;
   std::vector<std::shared_ptr<Expression>> args;

   bool is_pointer_access : 1;
   bool implicitSelf : 1;

   BuiltinFn builtinFnKind;
   size_t alignment;

   QualType returnType;

   union {
      cdot::cl::Method *method = nullptr;
      ast::Function *func;
   };

   IdentifierRefExpr *identExpr;

   std::vector<TemplateArg> templateArgs;
   std::vector<Argument> resolvedArgs;

   llvm::ArrayRef<Argument> declaredArgTypes;

   union {
      Type *builtinArgType = nullptr;
      FunctionType* functionType;
      cl::Union *U;
   };


public:
   CallKind getKind() const;
   void setKind(CallKind type);

   std::vector<std::shared_ptr<Expression>> &getArgs()
   {
      return args;
   }

   std::vector<std::shared_ptr<Expression>> const& getArgs() const
   {
      return args;
   }

   bool isPointerAccess_() const;
   void setIsPointerAccess_(bool isPointerAccess_);

   QualType &getReturnType();
   void setReturnType(const QualType &returnType);

   BuiltinFn getBuiltinFnKind() const;
   void setBuiltinFnKind(BuiltinFn builtinFnKind);

   cl::Method *getMethod() const;
   void setMethod(cl::Method *method);

   Function *getFunc() const;
   void setFunc(Function *func);

   std::vector<TemplateArg> &getTemplateArgs()
   {
      return templateArgs;
   }

   void setTemplateArgs(std::vector<TemplateArg> &&templateArgs)
   {
      CallExpr::templateArgs = std::move(templateArgs);
   }

   std::vector<Argument> &getResolvedArgs();
   void setResolvedArgs(const std::vector<Argument> &resolvedArgs);

   llvm::ArrayRef<Argument> getDeclaredArgTypes() const
   {
      return declaredArgTypes;
   }

   FunctionType *getFunctionType() const;
   void setFunctionType(FunctionType *functionType);

   Type *getBuiltinArgType() const
   {
      return builtinArgType;
   }

   void setBuiltinArgType(Type *BuiltinArgType)
   {
      CallExpr::builtinArgType = BuiltinArgType;
   }

   IdentifierRefExpr *getIdentExpr() const
   {
      return identExpr;
   }

   void setIdentExpr(IdentifierRefExpr *identExpr)
   {
      CallExpr::identExpr = identExpr;
   }

   bool isImplicitSelf() const
   {
      return implicitSelf;
   }

   void setImplicitSelf(bool implicitSelf)
   {
      CallExpr::implicitSelf = implicitSelf;
   }

   cl::Union *getUnion() const
   {
      return U;
   }

   void setUnion(cl::Union *U)
   {
      CallExpr::U = U;
   }
};

} // namespace ast
} // namespace cdot

#endif //CDOT_CALLEXPR_H
