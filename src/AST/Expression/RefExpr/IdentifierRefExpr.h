//
// Created by Jonas Zell on 19.06.17.
//

#ifndef CDOT_IDENTIFIEREXPRESSION_H
#define CDOT_IDENTIFIEREXPRESSION_H

#include "../Expression.h"


namespace cdot {
enum class BuiltinIdentifier {
   FUNC, MANGLED_FUNC, FLOAT_QNAN, DOUBLE_QNAN, FLOAT_SNAN, DOUBLE_SNAN
};

extern unordered_map<string, BuiltinIdentifier> builtinIdentifiers;

class TemplateArgList;
}

namespace cdot {
namespace ast {

class MemberRefExpr;

class IdentifierRefExpr : public Expression {
public:
   explicit IdentifierRefExpr(string &&ident);

   typedef std::shared_ptr<IdentifierRefExpr> SharedPtr;

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == IdentifierRefExprID;
   }

protected:
   // codegen
   bool captured_var = false;

   union {
      Type *builtinType = nullptr;
      Type *capturedType;
   };

   Type *metaType = nullptr;

   Variant builtinValue;
   BuiltinIdentifier builtinKind;

   bool is_let_expr : 1;
   bool is_var_expr : 1;
   bool is_namespace : 1;
   bool is_super : 1;
   bool is_self : 1;
   bool is_function : 1;
   bool is_metatype : 1;
   bool functionArg : 1;
   
   size_t argNo = 0;

   bool wrap_lambda = true;
   string superClassName;

   TemplateArgList *templateArgs;

public:
   bool isLetExpr()
   {
      return is_let_expr;
   }

   void isLetExpr(bool letExpr)
   {
      is_let_expr = letExpr;
   }

   bool isVarExpr()
   {
      return is_var_expr;
   }

   void isVarExpr(bool varExpr)
   {
      is_var_expr = varExpr;
   }

   bool isCapturedVar() const
   {
      return captured_var;
   }

   void setCapturedVar(bool captured_var)
   {
      IdentifierRefExpr::captured_var = captured_var;
   }

   Type *getCapturedType() const
   {
      return capturedType;
   }

   void setCapturedType(Type *capturedType)
   {
      IdentifierRefExpr::capturedType = capturedType;
   }

   const Variant &getBuiltinValue() const
   {
      return builtinValue;
   }

   void setBuiltinValue(const Variant &builtinValue)
   {
      IdentifierRefExpr::builtinValue = builtinValue;
   }

   Type *getBuiltinType() const
   {
      return builtinType;
   }

   void setBuiltinType(Type *builtinType)
   {
      IdentifierRefExpr::builtinType = builtinType;
   }

   Type *getMetaType() const
   {
      return metaType;
   }

   void setMetaType(Type *metaType)
   {
      IdentifierRefExpr::metaType = metaType;
   }

   BuiltinIdentifier getBuiltinKind() const
   {
      return builtinKind;
   }

   void setBuiltinKind(BuiltinIdentifier builtinKind)
   {
      IdentifierRefExpr::builtinKind = builtinKind;
   }

   bool isNamespace() const
   {
      return is_namespace;
   }

   void isNamespace(bool is_namespace)
   {
      IdentifierRefExpr::is_namespace = is_namespace;
   }

   bool isSuper() const
   {
      return is_super;
   }

   void isSuper(bool is_super)
   {
      IdentifierRefExpr::is_super = is_super;
   }

   bool isFunction() const
   {
      return is_function;
   }

   void isFunction(bool is_function)
   {
      IdentifierRefExpr::is_function = is_function;
   }

   const string &getSuperClassName() const
   {
      return superClassName;
   }

   void setSuperClassName(const string &superClassName)
   {
      IdentifierRefExpr::superClassName = superClassName;
   }

   bool wrapLambda() const
   {
      return wrap_lambda;
   }

   void wrapLambda(bool b)
   {
      wrap_lambda = b;
   }

   TemplateArgList *&getTemplateArgs()
   {
      return templateArgs;
   }

   bool isSelf() const
   {
      return is_self;
   }

   void setIsSelf(bool is_self)
   {
      IdentifierRefExpr::is_self = is_self;
   }

   bool isFunctionArg() const
   {
      return functionArg;
   }

   void setFunctionArg(bool functionArg)
   {
      IdentifierRefExpr::functionArg = functionArg;
   }

   size_t getArgNo() const
   {
      return argNo;
   }

   void setArgNo(size_t argNo)
   {
      IdentifierRefExpr::argNo = argNo;
   }

   void setTemplateArgs(TemplateArgList *templateArgs);
};

} // namespace ast
} // namespace cdot

#endif //CDOT_IDENTIFIEREXPRESSION_H
