//
// Created by Jonas Zell on 21.06.17.
//

#ifndef CDOT_CALLEXPR_H
#define CDOT_CALLEXPR_H


#include "../Expression.h"

namespace cdot {

class FunctionType;
enum class BuiltinFn : unsigned char;

} // namespace cdot

struct CallCompatability;

namespace cdot {
namespace ast {

class IdentifierRefExpr;
class CallableDecl;
class MethodDecl;
class FunctionDecl;
class UnionDecl;
class GlobalVarDecl;
class LocalVarDecl;

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
   PrimitiveInitializer,
   GlobalVariableCall,
   LocalVariableCall,
};

class CallExpr : public IdentifiedExpr {
public:
   explicit CallExpr(std::vector<Expression* > &&args,
                     string &&name = "");

   explicit CallExpr(std::vector<Expression* > &&args,
                     CallableDecl *C);

   void isPointerAccess(bool ptr)
   {
      is_pointer_access = ptr;
   }

   friend class TransformImpl;

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == CallExprID;
   }

protected:
   CallKind kind = CallKind::Unknown;
   std::vector<Expression* > args;

   bool is_pointer_access : 1;
   bool implicitSelf : 1;

   BuiltinFn builtinFnKind;
   size_t alignment;

   QualType returnType;

   union {
      MethodDecl *method = nullptr;
      FunctionDecl *func;
      GlobalVarDecl *globalVar;
      LocalVarDecl *localVar;
   };

   IdentifierRefExpr *identExpr = nullptr;

   std::vector<TemplateArgExpr*> templateArgs;

   union {
      Type *builtinArgType = nullptr;
      FunctionType* functionType;
      UnionDecl *U;
   };


public:
   std::vector<Expression* > &getArgs()
   {
      return args;
   }

   std::vector<Expression* > const& getArgs() const
   {
      return args;
   }

   MethodDecl *getMethod() const { return method; }
   void setMethod(MethodDecl *method) { this->method = method; }

   FunctionDecl *getFunc() const { return func; }
   void setFunc(FunctionDecl *func) { this->func = func; }

   std::vector<TemplateArgExpr*> &getTemplateArgs()
   {
      return templateArgs;
   }

   std::vector<TemplateArgExpr*> const& getTemplateArgs() const
   {
      return templateArgs;
   }

   void setTemplateArgs(std::vector<TemplateArgExpr*> &&templateArgs)
   {
      CallExpr::templateArgs = std::move(templateArgs);
   }

   FunctionType *getFunctionType() const { return functionType; }
   void setFunctionType(FunctionType *functionType)
   {
      this->functionType = functionType;
   }

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

   UnionDecl *getUnion() const
   {
      return U;
   }

   void setUnion(UnionDecl *U)
   {
      CallExpr::U = U;
   }

   GlobalVarDecl *getGlobalVar() const
   {
      return globalVar;
   }

   void setGlobalVar(GlobalVarDecl *globalVar)
   {
      kind = CallKind::GlobalVariableCall;
      CallExpr::globalVar = globalVar;
   }

   LocalVarDecl *getLocalVar() const
   {
      return localVar;
   }

   void setLocalVar(LocalVarDecl *localVar)
   {
      kind = CallKind::LocalVariableCall;
      CallExpr::localVar = localVar;
   }


   CallKind getKind() const
   {
      return kind;
   }

   void setKind(CallKind type)
   {
      CallExpr::kind = type;
   }

   bool isPointerAccess() const
   {
      return is_pointer_access;
   }

   void setIsPointerAccess(bool isPointerAccess_)
   {
      CallExpr::is_pointer_access = isPointerAccess_;
   }

   QualType &getReturnType()
   {
      return returnType;
   }

   void setReturnType(const QualType &returnType)
   {
      CallExpr::returnType = returnType;
   }

   BuiltinFn getBuiltinFnKind() const
   {
      return builtinFnKind;
   }

   void setBuiltinFnKind(BuiltinFn builtinFnKind)
   {
      CallExpr::builtinFnKind = builtinFnKind;
   }
};

} // namespace ast
} // namespace cdot

#endif //CDOT_CALLEXPR_H
