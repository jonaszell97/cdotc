//
// Created by Jonas Zell on 08.12.17.
//

#ifndef CDOT_STATICEXPR_H
#define CDOT_STATICEXPR_H

#include "Expression.h"

namespace cdot {
namespace ast {

class StaticExpr: public Expression {
public:
   explicit StaticExpr(std::shared_ptr<Expression> &&expr)
      : Expression(StaticExprID), expr(move(expr))
   {}

   StaticExpr(Variant &&V)
      : Expression(StaticExprID), expr(nullptr), evaluatedExpr(std::move(V))
   {}

   static bool classof(AstNode const *T)
   {
      return T->getTypeID() == StaticExprID;
   }

private:
   std::shared_ptr<Expression> expr;
   Variant evaluatedExpr;

public:
   const std::shared_ptr<Expression> &getExpr() const
   {
      return expr;
   }

   const Variant &getEvaluatedExpr() const
   {
      return evaluatedExpr;
   }

   void setEvaluatedExpr(const Variant &evaluatedExpr)
   {
      StaticExpr::evaluatedExpr = evaluatedExpr;
   }
};

class ConstraintExpr: public Expression {
public:
   enum Kind {
      Struct = 0, Class, Enum, Union, Function,
      DefaultConstructible, Pointer, Reference, Type
   };

   explicit ConstraintExpr(Kind kind)
      : Expression(ConstraintExprID), kind(kind)
   {}

   explicit ConstraintExpr(std::shared_ptr<TypeRef> &&typeConstraint)
      : Expression(ConstraintExprID), kind(Type),
        typeConstraint(move(typeConstraint))
   {}

   Kind getKind() const
   {
      return kind;
   }

   const std::shared_ptr<TypeRef> &getTypeConstraint() const
   {
      return typeConstraint;
   }

private:
   Kind kind;
   std::shared_ptr<TypeRef> typeConstraint;
};

class TraitsExpr: public Expression {
public:
   enum Kind : unsigned {
      Compiles = 0,
      CompileErrors,
      HasMember,
      HasProperty,
      HasStaticMember,
      HasStaticProperty,
      HasMethod,
      HasStaticMethod,
      ValidIdentifier,
      ValidFunction,

      IsInteger,
      IsFloat,
      IntegerBitwidth,
      IsUnsigned,
      FPPrecision,

      Arity,

      Invalid,
   };

   struct TraitsArgument {
      enum Kind {
         Stmt, Expr, Type, String
      };

      explicit TraitsArgument(std::shared_ptr<Statement> &&stmt)
         : kind(Stmt), stmt(move(stmt))
      {}

      explicit TraitsArgument(std::shared_ptr<Expression> &&expr)
         : kind(Expr), expr(move(expr))
      {}

      explicit TraitsArgument(std::shared_ptr<TypeRef> &&type)
         : kind(Type), type(move(type))
      {}

      explicit TraitsArgument(std::string &&str)
         : kind(String), str(move(str))
      {}

      TraitsArgument(TraitsArgument &&other)
         : kind(other.kind)
      {
         copyData(std::move(other));
      }

      TraitsArgument &operator=(TraitsArgument &&other)
      {
         destroyValue();
         copyData(std::move(other));

         return *this;
      }

      ~TraitsArgument()
      {
         destroyValue();
      }

      Kind getKind() const
      {
         return kind;
      }

      const std::shared_ptr<Statement> &getStmt() const
      {
         assert(kind == Stmt);
         return stmt;
      }

      const std::shared_ptr<Expression> &getExpr() const
      {
         assert(kind == Expr);
         return expr;
      }

      const std::shared_ptr<TypeRef> &getType() const
      {
         assert(kind == Type);
         return type;
      }

      const string &getStr() const
      {
         assert(kind == String);
         return str;
      }

   private:
      Kind kind;

      union {
         std::shared_ptr<Statement> stmt;
         std::shared_ptr<Expression> expr;
         std::shared_ptr<TypeRef> type;
         std::string str;
      };

      void destroyValue()
      {
         switch (kind) {
            case Stmt:
               stmt.~shared_ptr();
               break;
            case Expr:
               expr.~shared_ptr();
               break;
            case Type:
               type.~shared_ptr();
               break;
            case String:
               str.~string();
               break;
         }
      }

      void copyData(TraitsArgument &&other)
      {
         kind = other.kind;

         switch (kind) {
            case Expr:
               new (&expr) std::shared_ptr<Expression>(move(other.expr));
               break;
            case Stmt:
               new (&stmt) std::shared_ptr<Statement>(move(other.stmt));
               break;
            case Type:
               new (&type) std::shared_ptr<TypeRef>(move(other.type));
               break;
            case String:
               new (&str) std::string(move(other.str));
               break;
         }
      }
   };

   TraitsExpr(Kind kind, std::vector<TraitsArgument> &&args)
      : Expression(TraitsExprID), kind(kind), args(move(args))
   { }

   static bool classof(AstNode const *T)
   {
      return T->getTypeID() == TraitsExprID;
   }

   Kind getKind() const
   {
      return kind;
   }

   const std::vector<TraitsArgument> &getArgs() const
   {
      return args;
   }

   const std::shared_ptr<Expression> &getResultExpr() const
   {
      return resultExpr;
   }

   void setResultExpr(std::shared_ptr<Expression> &&resultExpr)
   {
      TraitsExpr::resultExpr = move(resultExpr);
   }

private:
   Kind kind;
   std::vector<TraitsArgument> args;

   std::shared_ptr<Expression> resultExpr = nullptr;
};

} // namespace ast
} // namespace cdot

#endif //CDOT_STATICEXPR_H
