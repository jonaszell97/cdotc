//
// Created by Jonas Zell on 06.08.17.
//

#ifndef CDOT_TYPEREF_H
#define CDOT_TYPEREF_H


#include "../Expression/Expression.h"

namespace cdot {
class TemplateArgList;
}

namespace cdot {
namespace ast {

class TypeRef : public Expression {
public:
   enum TypeKind {
      Auto,
      Primitive,
      FunctionType,
      TupleType,
      ObjectType
   };

   typedef std::vector<pair<string, TemplateArgList*>> NamespaceVec;
   typedef std::shared_ptr<TypeRef> SharedPtr;

   TypeRef();

   // Object type
   TypeRef(
      NamespaceVec &&ns
   );

   // function type
   TypeRef(
      TypeRef::SharedPtr &&returnType,
      std::vector<pair<string, TypeRef::SharedPtr>> &&argTypes
   );

   // tuple type
   explicit TypeRef(
      std::vector<pair<string, TypeRef::SharedPtr>> &&tupleTypes
   );

   TypeRef(const QualType &ty) : Expression(TypeRefID), type(ty), resolved(true)
   {

   }

   string toString();

   inline QualType getType(bool force = false)
   {
      assert((force || resolved) && "Resolve type before accessing!");
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

   bool isOption()
   {
      return is_option;
   }

   void isOption(bool opt)
   {
      is_option = opt;
   }

   size_t getPointerDepth() const
   {
      return pointerDepth;
   }

   void incrementPointerDepth()
   {
      ++pointerDepth;
   }

   void setType(const QualType& t)
   {
      type = t;
   }

   QualType &operator*()
   {
      assert(resolved && "resolve first");
      return type;
   }

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == TypeRefID;
   }

protected:
   TypeKind kind;
   bool resolved = false;

   bool is_option = false;
   bool is_reference = false;
   bool return_dummy_obj_ty = false;
   bool is_meta_ty = false;
   size_t pointerDepth = 0;

   QualType type;

   NamespaceVec namespaceQual;
   std::vector<pair<string, TypeRef::SharedPtr>> containedTypes;

   TypeRef::SharedPtr returnType = nullptr;

   bool vararg = false;
   bool cstyleVararg = false;

public:
   TypeKind getKind() const;
   void setKind(TypeKind kind);

   bool isResolved() const;
   void setResolved(bool resolved);

   void setPointerDepth(size_t pointerDepth);

   const QualType &getType() const;

   const NamespaceVec &getNamespaceQual() const;
   void setNamespaceQual(const NamespaceVec &namespaceQual);

   const std::vector<pair<string, SharedPtr>> &getContainedTypes() const;
   void setContainedTypes(
      const std::vector<pair<string, SharedPtr>> &containedTypes);

   const SharedPtr &getReturnType() const;
   void setReturnType(const SharedPtr &returnType);

   void setVararg(bool vararg);

   bool isCstyleVararg() const;
   void setCstyleVararg(bool cstyleVararg);

   bool returnDummyObjTy() const;
   void setReturnDummyObjTy(bool b);

   bool isMetaTy() const;
   void isMetaTy(bool is_meta_ty);
};

} // namespace ast
} // namespace cdot

#endif //CDOT_TYPEREF_H
