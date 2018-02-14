////
//// Created by Jonas Zell on 30.09.17.
////
//
//#ifndef CDOT_GENERIC_H
//#define CDOT_GENERIC_H
//
//#include <string>
//
//#include "QualType.h"
//#include "../../lex/SourceLocation.h"
//
//namespace cdot {
//
//namespace ast {
//   class TransformImpl;
//   class CompoundStmt;
//   class TypeRef;
//   class StaticExpr;
//   class Statement;
//   class Expression;
//} // namespace ast
//
//struct Argument {
//   Argument() = default;
//   Argument(const std::string &label, QualType ty,
//            bool hasDefault = false,
//            bool vararg = false,
//            bool cstyleVararg = false,
//            bool variadic = false)
//      : label(label), type(ty), defaultVal(hasDefault), isVararg(vararg),
//        cstyleVararg(cstyleVararg), variadic(variadic)
//   {
//
//   }
//
//   Argument(std::string &&label,
//            QualType ty,
//            bool cstyleVararg = false,
//            bool variadic = false)
//      : label(move(label)), type(ty), defaultVal(false), isVararg(false),
//        cstyleVararg(cstyleVararg), variadic(variadic)
//   {
//
//   }
//
//   std::string toString() const;
//
//   const std::string &getLabel() const
//   {
//      return label;
//   }
//
//   const QualType &getType() const
//   {
//      return type;
//   }
//
//   bool hasDefaultVal() const
//   {
//      return defaultVal;
//   }
//
//   bool isIsVararg() const
//   {
//      return isVararg;
//   }
//
//   bool isCstyleVararg() const
//   {
//      return cstyleVararg;
//   }
//
//   bool isVariadic() const
//   {
//      return variadic;
//   }
//
//   std::string label;
//   QualType type;
//
//   bool defaultVal   : 1;
//   bool isVararg     : 1;
//   bool cstyleVararg : 1;
//   bool variadic     : 1;
//};
//
//
//class TemplateArg;
//struct TemplateParameter;
//
//class Type;
//class ObjectType;
//class GenericType;
//
//class QualType;
//
//struct TemplateParameter {
//   enum Kind : unsigned char {
//      TypeName,
//      Value
//   };
//
//   TemplateParameter(Kind kind,
//                     std::string &&typeName,
//                     ast::TypeRef* unresolvedCovariance,
//                     ast::TypeRef* unresolvedContravariance,
//                     std::unique_ptr<TemplateArg> &&defaultValue = nullptr,
//                     bool isVariadic = false,
//                     const SourceLocation &loc = {});
//
//   TemplateParameter(Kind kind,
//                     std::string &&typeName,
//                     Type *covariance,
//                     Type *contravariance,
//                     std::unique_ptr<TemplateArg> &&defaultValue = nullptr,
//                     bool isVariadic = false);
//
//   TemplateParameter(llvm::StringRef typeName)
//      : genericTypeName(typeName.str()), resolved(true)
//   {}
//
//   TemplateParameter(const TemplateParameter& rhs);
//   TemplateParameter& operator=(const TemplateParameter& rhs);
//
//   TemplateParameter(TemplateParameter&& rhs) noexcept;
//   TemplateParameter& operator=(TemplateParameter&& rhs) noexcept;
//
//   ~TemplateParameter();
//
//   bool isTypeName() const { return kind == TypeName; }
//
//   const SourceLocation &getSourceLoc() const
//   {
//      return sourceLoc;
//   }
//
//   void setSourceLoc(const SourceLocation &sourceLoc)
//   {
//      TemplateParameter::sourceLoc = sourceLoc;
//   }
//
//   bool effectivelyEquals(const TemplateParameter &Other) const;
//
//   const std::string &getGenericTypeName() const
//   {
//      return genericTypeName;
//   }
//
//   Kind kind = TypeName;
//   std::string genericTypeName;
//   bool isVariadic = false;
//   SourceLocation sourceLoc;
//
//   ast::TypeRef* unresolvedCovariance = nullptr;
//   ast::TypeRef* unresolvedContravariance = nullptr;
//
//   union {
//      mutable Type *covariance = nullptr;
//      mutable Type *valueType;
//   };
//
//   mutable Type* contravariance = nullptr;
//
//   std::unique_ptr<TemplateArg> defaultValue;
//   mutable bool resolved = false;
//
//   bool operator ==(const TemplateParameter &rhs) const;
//   bool operator !=(const TemplateParameter &rhs) const;
//
//   friend class ast::TransformImpl;
//
//private:
//   void destroyValue();
//   void copyFrom(TemplateParameter &&TP);
//   void copyFrom(TemplateParameter const& TP);
//};
//
//class TemplateArg {
//public:
//   TemplateArg(const TemplateArg &arg);
//   TemplateArg &operator=(const TemplateArg &arg);
//   TemplateArg(TemplateArg &&arg) noexcept;
//   TemplateArg &operator=(TemplateArg &&arg) noexcept;
//   ~TemplateArg();
//
//   explicit TemplateArg(ast::TypeRef* ty);
//   explicit TemplateArg(ast::StaticExpr* staticExpr);
//
//   bool isTypeName() const;
//   bool isValue() const;
//
//   TemplateParameter::Kind getKind() const;
//
//   ast::TypeRef* getType() const
//   {
//      assert(isTypeName());
//      return type;
//   }
//
//   const SourceLocation &getSourceLoc() const { return sourceLoc; }
//   void setSourceLoc(const SourceLocation &sourceLoc)
//   {
//      TemplateArg::sourceLoc = sourceLoc;
//   }
//
//   ast::StaticExpr* getStaticExpr() const
//   {
//      return staticExpr;
//   }
//
//   friend class ast::TransformImpl;
//
//protected:
//   union {
//      ast::TypeRef* type = nullptr;
//      ast::StaticExpr* staticExpr;
//   };
//
//   SourceLocation sourceLoc;
//   TemplateParameter::Kind kind = TemplateParameter::TypeName;
//
//   void destroy();
//};
//
//} // namespace cdot
//
//
//#endif //CDOT_GENERIC_H
