//
// Created by Jonas Zell on 06.08.17.
//

#include "TypeRef.h"

#include "../../Variant/Type/AutoType.h"
#include "../../Variant/Type/Generic.h"
#include "../../Util.h"

TypeRef::TypeRef()
   : kind(TypeKind::Auto)
{

}

TypeRef::TypeRef(NamespaceVec &&ns)
   : kind(TypeKind::ObjectType), namespaceQual(ns)
{

}

TypeRef::TypeRef(TypeRef::SharedPtr &&returnType,
                 std::vector<pair<string, TypeRef::SharedPtr>> &&argTypes)
   : kind(TypeKind::FunctionType), returnType(returnType),
     containedTypes(argTypes)
{

}

TypeRef::TypeRef(
   std::vector<pair<string, TypeRef::SharedPtr>> &&tupleTypes)
   : kind(TypeKind::TupleType), containedTypes(tupleTypes)
{

}

string TypeRef::toString()
{
   string res;
   if (kind == Auto) {
      return "auto";
   }

   if (kind == ObjectType) {
      auto &ns = namespaceQual;
      bool first = true;
      for (const auto &obj : ns) {
         if (!first) {
            res += '.';
         }

         first = false;
         res += obj.first;

         if (obj.second) {
            res += util::TemplateArgsToString(obj.second->get());
         }
      }

      return res;
   }

   res += '(';

   auto &cont = containedTypes;
   auto numCont = cont.size();
   size_t i = 0;

   for (const auto &ty : cont) {
      if (!ty.first.empty()) {
         res += ty.first;
         res += ": ";
      }

      res += ty.second->toString();
      if (i < numCont + 1) {
         res += ", ";
      }
   }

   res += ')';

   if (kind == FunctionType) {
      res += " -> ";
      res += returnType->toString();
   }

   return res;
}

std::vector<AstNode::SharedPtr> TypeRef::get_children() {
   std::vector<AstNode::SharedPtr> children;

   return children;
}

TypeRef::TypeKind TypeRef::getKind() const
{
   return kind;
}

void TypeRef::setKind(TypeRef::TypeKind kind)
{
   TypeRef::kind = kind;
}

bool TypeRef::isResolved() const
{
   return resolved;
}

void TypeRef::setResolved(bool resolved)
{
   TypeRef::resolved = resolved;
}

void TypeRef::setPointerDepth(size_t pointerDepth)
{
   TypeRef::pointerDepth = pointerDepth;
}

const Type &TypeRef::getType() const
{
   return type;
}

const TypeRef::NamespaceVec &TypeRef::getNamespaceQual() const
{
   return namespaceQual;
}

void TypeRef::setNamespaceQual(const TypeRef::NamespaceVec &namespaceQual)
{
   TypeRef::namespaceQual = namespaceQual;
}

const std::vector<pair<string, TypeRef::SharedPtr>> &
TypeRef::getContainedTypes() const
{
   return containedTypes;
}

void TypeRef::setContainedTypes(
   const std::vector<pair<string, TypeRef::SharedPtr>> &containedTypes)
{
   TypeRef::containedTypes = containedTypes;
}

const TypeRef::SharedPtr &TypeRef::getReturnType() const
{
   return returnType;
}

void TypeRef::setReturnType(const TypeRef::SharedPtr &returnType)
{
   TypeRef::returnType = returnType;
}

void TypeRef::setVararg(bool vararg)
{
   TypeRef::vararg = vararg;
}

bool TypeRef::isCstyleVararg() const
{
   return cstyleVararg;
}

void TypeRef::setCstyleVararg(bool cstyleVararg)
{
   TypeRef::cstyleVararg = cstyleVararg;
}

bool TypeRef::returnDummyObjTy() const
{
   return return_dummy_obj_ty;
}

void TypeRef::setReturnDummyObjTy(bool b)
{
   return_dummy_obj_ty = b;
}

bool TypeRef::isMetaTy() const
{
   return is_meta_ty;
}

void TypeRef::isMetaTy(bool is_meta_ty)
{
   TypeRef::is_meta_ty = is_meta_ty;
}
