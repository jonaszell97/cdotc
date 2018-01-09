//
// Created by Jonas Zell on 19.06.17.
//

#ifndef CDOT_IDENTIFIEREXPRESSION_H
#define CDOT_IDENTIFIEREXPRESSION_H

#include "../Expression.h"


namespace cdot {
enum class BuiltinIdentifier {
   NULLPTR, FUNC, MANGLED_FUNC, FLOAT_QNAN, DOUBLE_QNAN,
   FLOAT_SNAN, DOUBLE_SNAN, __ctfe
};

extern unordered_map<string, BuiltinIdentifier> builtinIdentifiers;

class TemplateArgListBuilder;
}

namespace cdot {

struct Alias;

namespace ast {

class MemberRefExpr;

class IdentifierRefExpr : public IdentifiedExpr {
public:
   explicit IdentifierRefExpr(string &&ident);
   ~IdentifierRefExpr();

   typedef std::shared_ptr<IdentifierRefExpr> SharedPtr;

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == IdentifierRefExprID;
   }

protected:
   union {
      Type *builtinType = nullptr;
      Type *metaType;
      Statement *capturedValue;
      Callable *callable;
   };

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
   bool is_capture : 1;
   bool is_alias : 1;
   
   size_t argNo = 0;

   std::vector<TemplateArg> templateArgs;

   Variant aliasVal;

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

   bool isCaptured() const
   {
      return is_capture;
   }

   void setIsCaptured(bool captured_var)
   {
      IdentifierRefExpr::is_capture = captured_var;
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

   std::vector<TemplateArg> const& getTemplateArgs() const
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

   void setTemplateArgs(std::vector<TemplateArg> &&templateArgs)
   {
      IdentifierRefExpr::templateArgs = move(templateArgs);
   }

   const Variant &getAliasVal() const
   {
      return aliasVal;
   }

   void setAliasVal(Variant &&aliasVal)
   {
      IdentifierRefExpr::aliasVal = std::move(aliasVal);
      is_alias = true;
   }

   bool isAlias() const
   {
      return is_alias;
   }

   Statement *getCapturedValue() const
   {
      return capturedValue;
   }

   void setCapturedValue(Statement *capturedValue)
   {
      IdentifierRefExpr::capturedValue = capturedValue;
   }

   Callable *getCallable() const
   {
      return callable;
   }

   void setCallable(Callable *callable)
   {
      IdentifierRefExpr::callable = callable;
   }
};

class NonTypeTemplateArgExpr: public Expression {
public:
   explicit NonTypeTemplateArgExpr(const TemplateParameter &Param);
   ~NonTypeTemplateArgExpr();

   typedef std::shared_ptr<NonTypeTemplateArgExpr> SharedPtr;

private:
   const TemplateParameter &Param;

public:
   const TemplateParameter &getParam() const
   {
      return Param;
   }

   static bool classof(AstNode const* T)
   {
      return T->getTypeID() == NonTypeTemplateArgExprID;
   }
};

} // namespace ast
} // namespace cdot

#endif //CDOT_IDENTIFIEREXPRESSION_H
