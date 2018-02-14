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

extern std::unordered_map<std::string, BuiltinIdentifier> builtinIdentifiers;

namespace ast {

class MemberRefExpr;
class CallableDecl;
class LocalVarDecl;
class GlobalVarDecl;
class AssociatedTypeDecl;
class NamespaceDecl;
class AliasDecl;
class FuncArgDecl;

class IdentifierRefExpr : public IdentifiedExpr {
public:
   enum class IdentifierKind {
      Unknown,
      LocalVar,
      GlobalVar,
      FunctionArg,
      Function,
      TemplateArg,
      TemplateParam,
      AssociatedType,
      Namespace,
      MetaType,
      Alias,
      Self,
      Super,
      BuiltinValue,
   };

   explicit IdentifierRefExpr(std::string &&ident)
      : IdentifiedExpr(IdentifierRefExprID, move(ident)),
        captured(false)
   {}

   IdentifierRefExpr(std::string &&ident,
                     std::vector<TemplateArgExpr*> &&templateArgs)
      : IdentifiedExpr(IdentifierRefExprID, move(ident)),
        captured(false),
        templateArgs(move(templateArgs))
   {}

   IdentifierRefExpr(IdentifierRefExpr &&expr) = default;

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == IdentifierRefExprID;
   }

   friend class TransformImpl;

protected:
   IdentifierKind kind = IdentifierKind::Unknown;

   union {
      Type *builtinType = nullptr;
      MetaType *metaType;
      Statement *capturedValue;
      CallableDecl *callable;
      LocalVarDecl *localVar;
      GlobalVarDecl *globalVar;
      FuncArgDecl *funcArg;
      NamespaceDecl *namespaceDecl;
      Variant *aliasVal;
   };

   BuiltinIdentifier builtinKind;

   bool captured : 1;
   std::vector<TemplateArgExpr*> templateArgs;

public:
   bool isCaptured() const
   {
      return captured;
   }

   void setIsCaptured(bool captured_var)
   {
      IdentifierRefExpr::captured = captured_var;
   }

   const Variant &getBuiltinValue() const
   {
      assert(aliasVal && "not an alias");
      return *aliasVal;
   }

   void setBuiltinValue(Variant *builtinValue)
   {
      IdentifierRefExpr::aliasVal = builtinValue;
   }

   Type *getBuiltinType() const
   {
      return builtinType;
   }

   void setBuiltinType(Type *builtinType)
   {
      IdentifierRefExpr::builtinType = builtinType;
   }

   MetaType *getMetaType() const
   {
      return metaType;
   }

   void setMetaType(MetaType *metaType)
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

   std::vector<TemplateArgExpr*> const& getTemplateArgs() const
   {
      return templateArgs;
   }

   std::vector<TemplateArgExpr*> &getTemplateArgRef()
   {
      return templateArgs;
   }

   void setTemplateArgs(std::vector<TemplateArgExpr*> &&templateArgs)
   {
      IdentifierRefExpr::templateArgs = move(templateArgs);
   }

   const Variant &getAliasVal() const
   {
      return *aliasVal;
   }

   void setAliasVal(Variant *aliasVal)
   {
      IdentifierRefExpr::aliasVal = aliasVal;
      kind = IdentifierKind ::Alias;
   }

   Statement *getCapturedValue() const
   {
      return capturedValue;
   }

   void setCapturedValue(Statement *capturedValue)
   {
      IdentifierRefExpr::capturedValue = capturedValue;
   }

   CallableDecl *getCallable() const
   {
      return callable;
   }

   void setCallable(CallableDecl *callable)
   {
      IdentifierRefExpr::callable = callable;
   }

   IdentifierKind getKind() const
   {
      return kind;
   }

   void setKind(IdentifierKind kind)
   {
      IdentifierRefExpr::kind = kind;
   }

   LocalVarDecl *getLocalVar() const
   {
      return localVar;
   }

   void setLocalVar(LocalVarDecl *localVar)
   {
      IdentifierRefExpr::localVar = localVar;
   }

   GlobalVarDecl *getGlobalVar() const
   {
      return globalVar;
   }

   void setGlobalVar(GlobalVarDecl *globalVar)
   {
      IdentifierRefExpr::globalVar = globalVar;
   }

   FuncArgDecl *getFuncArg() const
   {
      return funcArg;
   }

   void setFuncArg(FuncArgDecl *funcArg)
   {
      IdentifierRefExpr::funcArg = funcArg;
   }

   NamespaceDecl *getNamespaceDecl() const
   {
      return namespaceDecl;
   }

   void setNamespaceDecl(NamespaceDecl *namespaceDecl)
   {
      IdentifierRefExpr::namespaceDecl = namespaceDecl;
   }
};

class BuiltinExpr: public Expression {
public:
   explicit BuiltinExpr(QualType type)
      : Expression(BuiltinExprID)
   {
      exprType = type;
   }

   static bool classof(AstNode const* T)
   {
      return T->getTypeID() == BuiltinExprID;
   }

   friend class TransformImpl;
};

} // namespace ast
} // namespace cdot

#endif //CDOT_IDENTIFIEREXPRESSION_H
