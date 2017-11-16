//
// Created by Jonas Zell on 30.09.17.
//

#ifndef CDOT_GENERIC_H
#define CDOT_GENERIC_H

#include <string>
#include <unordered_map>
#include <vector>

#include "../Variant.h"
#include "../Type/Type.h"
#include "../../Token.h"
#include "../../Template/TokenStore.h"

using std::string;
using std::unordered_map;

class TypeRef;
class Token;
class CompoundStmt;
class RecordTemplateDecl;
class CallableTemplateDecl;
class MethodTemplateDecl;
class Statement;
enum class RecordTemplateKind : unsigned char;

namespace cdot {

struct Argument {
   Argument() = default;
   Argument(const string &label, Type ty,
            std::shared_ptr<Expression> defVal = nullptr, bool vararg = false,
            bool cstyleVararg = false)
      : label(label), type(ty), defaultVal(defVal), isVararg(vararg),
        cstyleVararg(cstyleVararg)
   {

   }

   string toString() const
   {
      return type.toString();
   }

   string label;
   Type type;
   std::shared_ptr<Expression> defaultVal;

   bool isVararg = false;
   bool cstyleVararg = false;
   bool ref = false;
};


struct TemplateArg;
struct TemplateConstraint;
class TokenStore;

namespace cl {

class Record;

struct TemplateLoc {
   size_t startIndex;
   size_t line;
   size_t col;
   size_t length;
   size_t sourceId;
};

struct RecordTemplateInitializer {
   std::vector<std::shared_ptr<TypeRef>> args;
};

struct RecordTemplate;

struct RecordTemplateInstantiation {
   RecordTemplateInstantiation(
      RecordTemplate *Template,
      const std::vector<TemplateArg> &args
   ) : Template(Template), args(args) {}

   RecordTemplate *Template;
   std::vector<TemplateArg> args;
};

struct Template {
   std::unique_ptr<TokenStore> Store;
   std::vector<TemplateConstraint> constraints;
   RecordTemplateInstantiation *outerTemplate = nullptr;
};

struct RecordTemplate: Template {
   RecordTemplateKind kind;

   string recordName;
   std::vector<RecordTemplateInitializer> initializers;

   RecordTemplateDecl *decl;
   std::vector<RecordTemplate> extensions;

   void addInstantiation(std::shared_ptr<Statement> &&inst);
};

struct CallableTemplate: Template {
   string funcName;

   std::vector<std::shared_ptr<TypeRef>> args;
   std::shared_ptr<TypeRef> returnType;

   CallableTemplateDecl *decl;
};

struct CallableTemplateInstantiation {
   CallableTemplateInstantiation(
      CallableTemplate *Template,
      const std::vector<TemplateArg> &args
   ) : Template(Template), args(args) {}

   CallableTemplate *Template;
   std::vector<TemplateArg> args;
};

struct MethodTemplate: public CallableTemplate {
   bool isStatic;
   bool isOperator;
   bool isMutating;

   MethodTemplateDecl *methodDecl;
   Record *record;
};

} // namespace cl

class BuiltinType;
class ObjectType;
class GenericType;

struct Type;

struct TemplateConstraint {
   enum Kind : unsigned char {
      TypeName,
      Value,
      Arbitrary
   };

   TemplateConstraint(
      Kind kind,
      string &&typeName,
      std::shared_ptr<TypeRef> &&unresolvedCovariance,
      std::shared_ptr<TypeRef> &&unresolvedContravariance,
      std::shared_ptr<TemplateArg> &&defaultValue,
      bool isVariadic = false
   );

   TemplateConstraint(const TemplateConstraint& rhs);

   TemplateConstraint& operator=(const TemplateConstraint& rhs);
   ~TemplateConstraint();

   Kind kind = TypeName;
   string genericTypeName;
   bool isVariadic = false;

   union {
      std::shared_ptr<TypeRef> unresolvedCovariance;
      BuiltinType *covariance;
      BuiltinType *valueType;
   };

   union {
      std::shared_ptr<TypeRef> unresolvedContravariance;
      BuiltinType* contravariance;
   };

   std::shared_ptr<TemplateArg> defaultValue;
   bool resolved = false;

   bool operator ==(const TemplateConstraint &rhs) const;
   bool operator !=(const TemplateConstraint &rhs) const;
};

class TemplateArg {
public:
   TemplateArg();
   TemplateArg(const TemplateArg &arg);
   TemplateArg &operator=(const TemplateArg &arg);
   ~TemplateArg();

   explicit TemplateArg(std::shared_ptr<TypeRef> &&ty);
   explicit TemplateArg(Variant &&val);
   explicit TemplateArg(std::vector<Token> &&tokens);
   explicit TemplateArg(GenericType *ty);
   explicit TemplateArg(std::vector<TemplateArg> &&args);

   bool isTypeName() const;
   bool isValue() const;
   bool isArbitrary() const;
   bool isVariadic() const;

   bool compatibleWith(const TemplateArg &other) const;
   string toString() const;

   TemplateConstraint::Kind getKind() const;

   std::shared_ptr<TypeRef> &getType();
   GenericType *const getGenericTy() const;

   std::vector<TemplateArg> &getVariadicArgs();
   const std::vector<TemplateArg> &getVariadicArgs() const;

   const std::vector<Token> &getTokens() const;

   const Variant &getValue() const;

   void resolveType(GenericType *ty);
   bool isResolved() const;

protected:
   union {
      std::shared_ptr<TypeRef> type = nullptr;
      GenericType *genericType;
      std::vector<Token> tokens;
      std::vector<TemplateArg> variadicArgs;
      Variant val;
   };

   TemplateConstraint::Kind kind = TemplateConstraint::TypeName;
   bool type_resolved = false;
   bool is_variadic = false;

   void destroy();
};

class TemplateArgList {
public:
   virtual ~TemplateArgList() = default;
   virtual bool isResolved() const = 0;
   virtual std::vector<TemplateArg> &get() = 0;
   virtual void set(std::vector<TemplateArg> &&args) = 0;
   virtual std::vector<Token> &&getTokens() = 0;

protected:
   TemplateArgList() = default;
};

class ResolvedTemplateArgList: public TemplateArgList {
public:
   explicit ResolvedTemplateArgList(std::vector<TemplateArg> && args);

   bool isResolved() const override;
   std::vector<TemplateArg> &get() override;
   void set(std::vector<TemplateArg> &&args) override;
   std::vector<Token> &&getTokens() override;

protected:
   std::vector<TemplateArg> args;
};

class UnresolvedTemplateArgList: public TemplateArgList {
public:
   explicit UnresolvedTemplateArgList(
      std::vector<Token> &&tokens,
      size_t sourceId,
      size_t line,
      size_t col
   );

   bool isResolved() const override;
   std::vector<TemplateArg> &get() override;
   void set(std::vector<TemplateArg> &&args) override;
   std::vector<Token> &&getTokens() override;

protected:
   std::vector<Token> tokens;
   struct Loc {
      size_t sourceId;
      size_t line;
      size_t col;
   } loc;

public:
   const Loc &getLoc() const
   {
      return loc;
   }
};

bool GenericTypesCompatible(GenericType* given,
                            const TemplateConstraint& needed);

void resolveGenerics(Type& ty, Type& obj);
void resolveGenerics(Type& ty, ObjectType*& obj);
void resolveGenerics(Type& ty, const std::vector<TemplateArg>& generics);

} // namespace cdot


#endif //CDOT_GENERIC_H
