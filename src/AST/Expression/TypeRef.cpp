//
// Created by Jonas Zell on 06.08.17.
//

#include "TypeRef.h"

#include "../../Variant/Type/AutoType.h"
#include "../../Variant/Type/Generic.h"
#include "../../Util.h"

namespace cdot {
namespace ast {

TypeRef::TypeRef()
   : Expression(TypeRefID), kind(TypeKind::Auto), resolved(false),
     is_reference(false), is_meta_ty(false),
     allow_unexpanded_template_args(false), vararg(false), cstyleVararg(false)
{

}

TypeRef::TypeRef(NamespaceVec &&ns)
   : Expression(TypeRefID), kind(TypeKind::ObjectType),
     namespaceQual(move(ns)),
     resolved(false),is_reference(false), is_meta_ty(false),
     allow_unexpanded_template_args(false), vararg(false), cstyleVararg(false)
{

}

TypeRef::TypeRef(TypeRef::SharedPtr &&returnType,
                 std::vector<pair<string, TypeRef::SharedPtr>> &&argTypes)
   : Expression(TypeRefID),
     kind(TypeKind::FunctionType),
     containedTypes(move(argTypes)),
     returnType(move(returnType)),
     resolved(false), is_reference(false), is_meta_ty(false),
     allow_unexpanded_template_args(false), vararg(false), cstyleVararg(false)
{

}

TypeRef::TypeRef(
   std::vector<pair<string, TypeRef::SharedPtr>> &&tupleTypes)
   : Expression(TypeRefID), kind(TypeKind::TupleType),
     containedTypes(move(tupleTypes)),
     resolved(false), is_reference(false), is_meta_ty(false),
     allow_unexpanded_template_args(false), vararg(false), cstyleVararg(false)
{

}

TypeRef::TypeRef(std::shared_ptr<TypeRef> &&elementTy,
                 std::shared_ptr<StaticExpr> &&arraySize)
   : Expression(TypeRefID), kind(TypeKind::ArrayType),
     containedTypes{{"", move(elementTy)}},
     arraySize(move(arraySize)),
     resolved(false), is_reference(false), is_meta_ty(false),
     allow_unexpanded_template_args(false), vararg(false), cstyleVararg(false)
{

}

TypeRef::TypeRef(std::shared_ptr<Expression> &&declTypeExpr)
   : Expression(TypeRefID),
     kind(TypeKind::DeclTypeExpr),
     declTypeExpr(move(declTypeExpr)),
     resolved(false), is_reference(false), is_meta_ty(false),
     allow_unexpanded_template_args(false), vararg(false), cstyleVararg(false)
{

}

TypeRef::TypeRef(const QualType &ty)
   : Expression(TypeRefID), type(ty), resolved(true),
     is_reference(false), is_meta_ty(false),
     allow_unexpanded_template_args(false), vararg(false), cstyleVararg(false)
{

}

TypeRef::TypeRef(std::shared_ptr<TypeRef> &&subject, TypeKind kind)
   : Expression(TypeRefID), kind(kind), subject(move(subject)),
     resolved(false), is_reference(false), is_meta_ty(false),
     allow_unexpanded_template_args(false), vararg(false), cstyleVararg(false)
{

}

TypeRef::~TypeRef()
{
   if (isDeclTypeExpr()) {
      declTypeExpr.~shared_ptr();
   }
   else if (kind == TypeKind::FunctionType) {
      returnType.~shared_ptr();
   }
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

llvm::StringRef TypeRef::getSingularTypeName() const
{
   assert(kind == ObjectType && namespaceQual.size() == 1);
   return namespaceQual.front().first;
}

void TypeRef::forEachContainedType(void (*func)(TypeRef *))
{
   switch (kind) {
      case ObjectType:
         for (auto &NS : namespaceQual) {
            for (auto &TA : NS.second)
               if (TA.isTypeName())
                  func(TA.getType().get());
         }

         break;
      case FunctionType:
      case TupleType:
         for (const auto &Ty : containedTypes)
            func(Ty.second.get());

         if (kind == FunctionType)
            func(returnType.get());

         break;
      default:
         break;
   }
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

const QualType &TypeRef::getType() const
{
   return type;
}

TypeRef::NamespaceVec &TypeRef::getNamespaceQual()
{
   return namespaceQual;
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

bool TypeRef::isMetaTy() const
{
   return is_meta_ty;
}

void TypeRef::isMetaTy(bool is_meta_ty)
{
   TypeRef::is_meta_ty = is_meta_ty;
}

} // namespace ast
} // namespace cdot
