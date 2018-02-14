//
// Created by Jonas Zell on 06.08.17.
//

#include "TypeRef.h"
#include "../Statement/Declaration/NamedDecl.h"
#include "../../Util.h"

using std::string;
using std::pair;

namespace cdot {
namespace ast {

TypeRef::TypeRef()
   : Expression(TypeRefID), kind(TypeKind::Auto), resolved(false),
     is_reference(false),
     globalLookup(false),
     is_meta_ty(false),
     allow_unexpanded_template_args(false)
{

}

TypeRef::TypeRef(NamespaceVec &&ns)
   : Expression(TypeRefID), kind(TypeKind::ObjectType),
     namespaceQual(move(ns)),
     resolved(false), is_reference(false), globalLookup(false),
     is_meta_ty(false),
     allow_unexpanded_template_args(false)
{

}

TypeRef::TypeRef(TypeRef* returnType,
                 std::vector<pair<string, TypeRef*>> &&argTypes)
   : Expression(TypeRefID),
     kind(TypeKind::FunctionType),
     containedTypes(move(argTypes)),
     returnType(returnType),
     resolved(false), is_reference(false), globalLookup(false),
     is_meta_ty(false),
     allow_unexpanded_template_args(false)
{

}

TypeRef::TypeRef(
   std::vector<pair<string, TypeRef*>> &&tupleTypes)
   : Expression(TypeRefID), kind(TypeKind::TupleType),
     containedTypes(move(tupleTypes)),
     resolved(false), is_reference(false), globalLookup(false),
     is_meta_ty(false),
     allow_unexpanded_template_args(false)
{

}

TypeRef::TypeRef(TypeRef* elementTy,
                 StaticExpr* arraySize)
   : Expression(TypeRefID), kind(TypeKind::ArrayType),
     containedTypes{{"", elementTy}},
     arraySize(arraySize),
     resolved(false), is_reference(false), globalLookup(false),
     is_meta_ty(false),
     allow_unexpanded_template_args(false)
{

}

TypeRef::TypeRef(Expression* declTypeExpr)
   : Expression(TypeRefID),
     kind(TypeKind::DeclTypeExpr),
     declTypeExpr(declTypeExpr),
     resolved(false), is_reference(false), globalLookup(false),
     is_meta_ty(false),
     allow_unexpanded_template_args(false)
{

}

TypeRef::TypeRef(const QualType &ty)
   : Expression(TypeRefID), type(ty), resolved(true),
     is_reference(false), globalLookup(false),
     is_meta_ty(false),
     allow_unexpanded_template_args(false)
{

}

TypeRef::TypeRef(TypeRef* subject, TypeKind kind)
   : Expression(TypeRefID), kind(kind), subject(subject),
     resolved(false), is_reference(false), globalLookup(false),
     is_meta_ty(false),
     allow_unexpanded_template_args(false)
{

}

TypeRef::~TypeRef()
{

}

void TypeRef::setType(const cdot::QualType &t)
{
   resolved = true;
   type = t;
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
               if (TA->isTypeName())
                  func(TA->getType());
         }

         break;
      case FunctionType:
      case TupleType:
         for (const auto &Ty : containedTypes)
            func(Ty.second);

         if (kind == FunctionType)
            func(returnType);

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

} // namespace ast
} // namespace cdot
