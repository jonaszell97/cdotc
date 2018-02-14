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
      ArrayType,
      FunctionType,
      TupleType,
      ObjectType,
      DeclTypeExpr,
      Pointer,
      Option
   };

   typedef std::vector<std::pair<std::string, std::vector<TemplateArgExpr*>>>
      NamespaceVec;
   friend class TransformImpl;

   TypeRef();
   ~TypeRef();

   // Object type
   explicit TypeRef(NamespaceVec &&ns);

   // function type
   TypeRef(TypeRef* returnType,
           std::vector<std::pair<std::string, TypeRef*>> &&argTypes);

   // tuple type
   explicit TypeRef(std::vector<std::pair<std::string, TypeRef*>> &&tupleTypes);

   // array type
   explicit TypeRef(TypeRef* elementTy,
                    StaticExpr* arraySize);

   // decltype(expr)
   explicit TypeRef(Expression* declTypeExpr);

   explicit TypeRef(const QualType &ty);

   TypeRef(TypeRef* subject, TypeKind kind);


   std::string toString();

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

   operator QualType() const
   {
      return type;
   }

   bool isReference() const
   {
      return is_reference;
   }

   void isReference(bool ref)
   {
      is_reference = ref;
   }

   void setType(const QualType& t);

   bool isDeclTypeExpr() const
   {
      return kind == TypeKind::DeclTypeExpr;
   }

   Expression* getDeclTypeExpr() const
   {
      return declTypeExpr;
   }

   bool isGlobalLookup() const
   {
      return globalLookup;
   }

   void setGlobalLookup(bool globalLookup)
   {
      TypeRef::globalLookup = globalLookup;
   }

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == TypeRefID;
   }

protected:
   TypeKind kind;
   QualType type;

   NamespaceVec namespaceQual;
   std::vector<std::pair<std::string, TypeRef*>> containedTypes;

   union {
      TypeRef* returnType = nullptr;
      StaticExpr* arraySize;
      TypeRef* subject;
      Expression* declTypeExpr;
   };

   bool resolved : 1;
   bool is_reference : 1;
   bool globalLookup : 1;
   bool is_meta_ty : 1;
   bool allow_unexpanded_template_args : 1;

public:
   TypeKind getKind() const;
   void setKind(TypeKind kind);

   bool isResolved() const;
   void setResolved(bool resolved);

   const QualType &getType() const;

   NamespaceVec &getNamespaceQual();

   NamespaceVec const& getNamespaceQual() const
   {
      return namespaceQual;
   }

   const std::vector<std::pair<std::string, TypeRef* >>
   &getContainedTypes() const
   {
      return containedTypes;
   };

   TypeRef* getReturnType() const { return returnType; }
   void setReturnType(TypeRef* returnType) { this->returnType =returnType; }

   bool isMetaTy() const { return is_meta_ty; }
   void isMetaTy(bool is_meta_ty) { this->is_meta_ty = is_meta_ty; }

   TypeRef* getSubject() const
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

   StaticExpr* getArraySize() const
   {
      return arraySize;
   }

   TypeRef* getElementType() const
   {
      return containedTypes.front().second;
   }
};

} // namespace ast
} // namespace cdot

#endif //CDOT_TYPEREF_H
