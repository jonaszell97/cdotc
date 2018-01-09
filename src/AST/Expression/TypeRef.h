//
// Created by Jonas Zell on 06.08.17.
//

#ifndef CDOT_TYPEREF_H
#define CDOT_TYPEREF_H

#include "../Expression/Expression.h"

namespace cdot {
namespace ast {

class StaticExpr;

class TypeRef : public Expression {
public:
   enum TypeKind {
      Auto,
      Primitive,
      ArrayType,
      FunctionType,
      TupleType,
      ObjectType,
      DeclTypeExpr,
      Pointer,
      Option
   };

   typedef std::vector<pair<string, std::vector<TemplateArg>>> NamespaceVec;
   typedef std::shared_ptr<TypeRef> SharedPtr;

   TypeRef();
   ~TypeRef();

   // Object type
   explicit TypeRef(NamespaceVec &&ns);

   // function type
   TypeRef(TypeRef::SharedPtr &&returnType,
           std::vector<pair<string, TypeRef::SharedPtr>> &&argTypes);

   // tuple type
   explicit TypeRef(std::vector<pair<string, TypeRef::SharedPtr>> &&tupleTypes);

   // array type
   explicit TypeRef(std::shared_ptr<TypeRef> &&elementTy,
                    std::shared_ptr<StaticExpr> &&arraySize);

   // decltype(expr)
   explicit TypeRef(std::shared_ptr<Expression> &&declTypeExpr);

   explicit TypeRef(const QualType &ty);

   TypeRef(std::shared_ptr<TypeRef> &&subject, TypeKind kind);


   string toString();

   bool isSingularType() const
   {
      return kind == ObjectType && namespaceQual.size() == 1;
   }

   llvm::StringRef getSingularTypeName() const;

   void forEachContainedType(void (*func)(TypeRef *));

   inline QualType getType(bool force = false)
   {
      assert((force || resolved) && "Resolve type before accessing!");
      return type;
   }

   QualType &operator*()
   {
      return type;
   }

   QualType const& operator*() const
   {
      return type;
   }

   QualType &getTypeRef()
   {
      return type;
   }

   void isVararg(bool va)
   {
      vararg = va;
   }

   bool isVararg()
   {
      return vararg;
   }

   void isCStyleVararg(bool va)
   {
      cstyleVararg = va;
      vararg = va;
   }

   bool isCStyleVararg()
   {
      return cstyleVararg;
   }

   bool isReference()
   {
      return is_reference;
   }

   void isReference(bool ref)
   {
      is_reference = ref;
   }

   void setType(const QualType& t)
   {
      type = t;
   }

   bool isDeclTypeExpr() const
   {
      return kind == TypeKind::DeclTypeExpr;
   }

   const std::shared_ptr<Expression> &getDeclTypeExpr() const
   {
      return declTypeExpr;
   }

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == TypeRefID;
   }

protected:
   TypeKind kind;
   QualType type;

   NamespaceVec namespaceQual;
   std::vector<pair<string, TypeRef::SharedPtr>> containedTypes;

   union {
      std::shared_ptr<TypeRef> returnType = nullptr;
      std::shared_ptr<StaticExpr> arraySize;
      std::shared_ptr<TypeRef> subject;
      std::shared_ptr<Expression> declTypeExpr;
   };

   bool resolved : 1;
   bool is_reference : 1;
   bool is_meta_ty : 1;
   bool allow_unexpanded_template_args : 1;
   bool vararg : 1;
   bool cstyleVararg : 1;

public:
   TypeKind getKind() const;
   void setKind(TypeKind kind);

   bool isResolved() const;
   void setResolved(bool resolved);

   const QualType &getType() const;

   NamespaceVec &getNamespaceQual();

   const std::vector<pair<string, SharedPtr>> &getContainedTypes() const;
   void setContainedTypes(
      const std::vector<pair<string, SharedPtr>> &containedTypes);

   const SharedPtr &getReturnType() const;
   void setReturnType(const SharedPtr &returnType);

   void setVararg(bool vararg);

   bool isCstyleVararg() const;
   void setCstyleVararg(bool cstyleVararg);

   bool isMetaTy() const;
   void isMetaTy(bool is_meta_ty);

   const std::shared_ptr<TypeRef> &getSubject() const
   {
      return subject;
   }

   bool allowUnexpandedTemplateArgs() const
   {
      return allow_unexpanded_template_args;
   }

   void setAllowUnexpandedTemplateArgs(bool allow)
   {
      allow_unexpanded_template_args = allow;
   }

   const std::shared_ptr<StaticExpr> &getArraySize() const
   {
      return arraySize;
   }

   const std::shared_ptr<TypeRef> &getElementType() const
   {
      return containedTypes.front().second;
   }
};

} // namespace ast
} // namespace cdot

#endif //CDOT_TYPEREF_H
