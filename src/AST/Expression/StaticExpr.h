//
// Created by Jonas Zell on 08.12.17.
//

#ifndef CDOT_STATICEXPR_H
#define CDOT_STATICEXPR_H

#include "Expression.h"

namespace cdot {
namespace ast {

class TypeRef;

class StaticExpr: public Expression {
public:
   explicit StaticExpr(Expression* expr)
      : Expression(StaticExprID), expr(expr)
   {
      loc = expr->getSourceLoc();
   }

   StaticExpr(Variant &&V)
      : Expression(StaticExprID), expr(nullptr), evaluatedExpr(std::move(V))
   {}

   static bool classof(AstNode const *T)
   {
      return T->getTypeID() == StaticExprID;
   }

   friend class TransformImpl;

private:
   Expression* expr;
   Variant evaluatedExpr;

public:
   Expression* getExpr() const
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
      Struct = StructDeclID, Class = ClassDeclID,
      Enum = EnumDeclID, Union = UnionDeclID, Function = FunctionDeclID,
      DefaultConstructible, Pointer, Reference, Type
   };

   explicit ConstraintExpr(Kind kind)
      : Expression(ConstraintExprID), kind(kind),
        typeConstraint(nullptr)
   {}

   explicit ConstraintExpr(TypeRef* typeConstraint)
      : Expression(ConstraintExprID), kind(Type),
        typeConstraint(typeConstraint)
   {}

   Kind getKind() const
   {
      return kind;
   }

   TypeRef* getTypeConstraint() const
   {
      return typeConstraint;
   }

   static bool classof(AstNode const *T)
   {
      return T->getTypeID() == ConstraintExprID;
   }

   friend class TransformImpl;

private:
   Kind kind;
   TypeRef* typeConstraint;
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

      explicit TraitsArgument(Statement* stmt)
         : kind(Stmt), stmt(stmt)
      {}

      explicit TraitsArgument(Expression* expr)
         : kind(Expr), expr(expr)
      {}

      explicit TraitsArgument(TypeRef* type)
         : kind(Type), type(type)
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

      Statement* getStmt() const
      {
         assert(kind == Stmt);
         return stmt;
      }

      Expression* getExpr() const
      {
         assert(kind == Expr);
         return expr;
      }

      TypeRef* getType() const
      {
         assert(kind == Type);
         return type;
      }

      const string &getStr() const
      {
         assert(kind == String);
         return str;
      }

      friend class TransformImpl;

   private:
      Kind kind;

      union {
         Statement* stmt;
         Expression* expr;
         TypeRef* type;
         std::string str;
      };

      void destroyValue()
      {
         switch (kind) {
            default:
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
               expr = other.expr;
               break;
            case Stmt:
               stmt = other.stmt;
               break;
            case Type:
               type = other.type;
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

   friend class TransformImpl;

   Kind getKind() const
   {
      return kind;
   }

   const std::vector<TraitsArgument> &getArgs() const
   {
      return args;
   }

   Expression* getResultExpr() const
   {
      return resultExpr;
   }

   void setResultExpr(Expression* resultExpr)
   {
      TraitsExpr::resultExpr = resultExpr;
   }

private:
   Kind kind;
   std::vector<TraitsArgument> args;

   Expression* resultExpr = nullptr;
};

} // namespace ast
} // namespace cdot

#endif //CDOT_STATICEXPR_H
