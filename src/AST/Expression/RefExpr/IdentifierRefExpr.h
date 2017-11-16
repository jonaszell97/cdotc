//
// Created by Jonas Zell on 19.06.17.
//

#ifndef CDOT_IDENTIFIEREXPRESSION_H
#define CDOT_IDENTIFIEREXPRESSION_H

#include "../Expression.h"

class MemberRefExpr;

namespace cdot {
enum class BuiltinIdentifier {
   FUNC, MANGLED_FUNC, FLOAT_QNAN, DOUBLE_QNAN, FLOAT_SNAN, DOUBLE_SNAN
};

extern unordered_map<string, BuiltinIdentifier> builtinIdentifiers;

class TemplateArgList;
}

class IdentifierRefExpr : public Expression {
public:
   explicit IdentifierRefExpr(string &&ident);
   ~IdentifierRefExpr() override;

   bool isUnderscore() const override
   {
      return ident == "_";
   }

   bool needsContextualInformation() const override
   {
      return memberExpr == nullptr;
   }

   typedef std::shared_ptr<IdentifierRefExpr> SharedPtr;
   std::vector<AstNode::SharedPtr> get_children() override;

   NodeType get_type() override {
      return NodeType::IDENTIFIER_EXPR;
   }

   ASTNODE_ACCEPT_PASSES
   ADD_FRIEND_PASSES

protected:
   // codegen
   bool captured_var = false;

   union {
      BuiltinType *builtinType = nullptr;
      BuiltinType *capturedType;
   };

   BuiltinType *metaType = nullptr;

   bool is_let_expr = false;
   bool is_var_expr = false;

   Variant builtinValue;
   BuiltinIdentifier builtinKind;

   bool is_namespace = false;
   bool is_super = false;
   bool is_function = false;
   bool is_metatype = false;

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

   BuiltinType *getCapturedType() const
   {
      return capturedType;
   }

   void setCapturedType(BuiltinType *capturedType)
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

   BuiltinType *getBuiltinType() const
   {
      return builtinType;
   }

   void setBuiltinType(BuiltinType *builtinType)
   {
      IdentifierRefExpr::builtinType = builtinType;
   }

   BuiltinType *getMetaType() const
   {
      return metaType;
   }

   void setMetaType(BuiltinType *metaType)
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

   void setTemplateArgs(TemplateArgList *templateArgs);
};


#endif //CDOT_IDENTIFIEREXPRESSION_H
