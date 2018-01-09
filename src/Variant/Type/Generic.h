//
// Created by Jonas Zell on 30.09.17.
//

#ifndef CDOT_GENERIC_H
#define CDOT_GENERIC_H

#include <string>
#include <vector>

#include "../Variant.h"
#include "QualType.h"
#include "../../lex/Token.h"

namespace cdot {

namespace ast {

class CompoundStmt;
class TypeRef;
class StaticExpr;
class Statement;
class Expression;

} // namespace ast

namespace lex {

struct Token;

} // namespace lex

struct Argument {
   Argument() = default;
   Argument(const std::string &label, QualType ty,
            std::shared_ptr<ast::Expression> defVal = nullptr,
            bool vararg = false,
            bool cstyleVararg = false,
            bool variadic = false)
      : label(label), type(ty), defaultVal(defVal), isVararg(vararg),
        cstyleVararg(cstyleVararg), variadic(variadic)
   {

   }

   std::string toString() const;

   const std::string &getLabel() const
   {
      return label;
   }

   const QualType &getType() const
   {
      return type;
   }

   const std::shared_ptr<ast::Expression> &getDefaultVal() const
   {
      return defaultVal;
   }

   bool isIsVararg() const
   {
      return isVararg;
   }

   bool isCstyleVararg() const
   {
      return cstyleVararg;
   }

   bool isVariadic() const
   {
      return variadic;
   }

   std::string label;
   QualType type;
   std::shared_ptr<ast::Expression> defaultVal;

   bool isVararg = false;
   bool cstyleVararg = false;
   bool variadic = false;
};


class TemplateArg;
struct TemplateParameter;

class Type;
class ObjectType;
class GenericType;

class QualType;

struct TemplateParameter {
   enum Kind : unsigned char {
      TypeName,
      Value
   };

   TemplateParameter(
      Kind kind,
      std::string &&typeName,
      std::shared_ptr<ast::TypeRef> &&unresolvedCovariance,
      std::shared_ptr<ast::TypeRef> &&unresolvedContravariance,
      std::shared_ptr<TemplateArg> &&defaultValue,
      bool isVariadic = false,
      const SourceLocation &loc = {}
   );

   TemplateParameter(llvm::StringRef typeName)
      : genericTypeName(typeName.str()), resolved(true)
   {}

   TemplateParameter(const TemplateParameter& rhs);
   TemplateParameter& operator=(const TemplateParameter& rhs);

   TemplateParameter(TemplateParameter&& rhs) noexcept;
   TemplateParameter& operator=(TemplateParameter&& rhs) noexcept;

   ~TemplateParameter();

   bool isTypeName() const { return kind == TypeName; }

   const SourceLocation &getSourceLoc() const
   {
      return sourceLoc;
   }

   void setSourceLoc(const SourceLocation &sourceLoc)
   {
      TemplateParameter::sourceLoc = sourceLoc;
   }

   bool effectivelyEquals(const TemplateParameter &Other) const;

   const std::string &getGenericTypeName() const
   {
      return genericTypeName;
   }

   Kind kind = TypeName;
   std::string genericTypeName;
   bool isVariadic = false;
   SourceLocation sourceLoc;

   std::shared_ptr<ast::TypeRef> unresolvedCovariance = nullptr;
   std::shared_ptr<ast::TypeRef> unresolvedContravariance = nullptr;

   union {
      Type *covariance = nullptr;
      Type *valueType;
   };

   Type* contravariance = nullptr;

   std::shared_ptr<TemplateArg> defaultValue = nullptr;
   bool resolved = false;

   bool operator ==(const TemplateParameter &rhs) const;
   bool operator !=(const TemplateParameter &rhs) const;

private:
   void destroyValue();
   void copyFrom(TemplateParameter &&TP);
   void copyFrom(TemplateParameter const& TP);
};

typedef std::vector<std::vector<TemplateParameter>*> TemplateParamStack;

class TemplateArg {
public:
//   TemplateArg(const TemplateArg &arg);
//   TemplateArg &operator=(const TemplateArg &arg);
   TemplateArg(TemplateArg &&arg) noexcept;
   TemplateArg &operator=(TemplateArg &&arg) noexcept;
   ~TemplateArg();

   explicit TemplateArg(std::shared_ptr<ast::TypeRef> &&ty);
   explicit TemplateArg(std::shared_ptr<ast::StaticExpr> &&staticExpr);

   bool isTypeName() const;
   bool isValue() const;

   TemplateParameter::Kind getKind() const;

   std::shared_ptr<ast::TypeRef> const& getType() const
   {
      assert(isTypeName());
      return type;
   }

   const SourceLocation &getSourceLoc() const { return sourceLoc; }
   void setSourceLoc(const SourceLocation &sourceLoc)
   {
      TemplateArg::sourceLoc = sourceLoc;
   }

   const std::shared_ptr<ast::StaticExpr> &getStaticExpr() const
   {
      return staticExpr;
   }

protected:
   union {
      std::shared_ptr<ast::TypeRef> type = nullptr;
      std::shared_ptr<ast::StaticExpr> staticExpr;
   };

   SourceLocation sourceLoc;
   TemplateParameter::Kind kind = TemplateParameter::TypeName;

   void destroy();
};

} // namespace cdot


#endif //CDOT_GENERIC_H
