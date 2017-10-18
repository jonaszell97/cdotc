//
// Created by Jonas Zell on 06.08.17.
//

#include "TypeRef.h"
#include "../../Variant/Type/AutoType.h"

TypeRef::TypeRef()
   : kind(TypeKind::Auto)
{

}

TypeRef::TypeRef(string &className, std::vector<pair<string, TypeRef::SharedPtr>> generics)
   : kind(TypeKind::ObjectType), className(className), containedTypes(generics)
{

}

TypeRef::TypeRef(TypeRef::SharedPtr returnType, std::vector<pair<string, TypeRef::SharedPtr>> &argTypes)
   : kind(TypeKind::FunctionType), returnType(returnType), containedTypes(argTypes)
{

}

TypeRef::TypeRef(std::vector<pair<string, TypeRef::SharedPtr>> &tupleTypes)
   : kind(TypeKind::TupleType), containedTypes(tupleTypes)
{

}

string TypeRef::toString()
{
   return type->toString();
}

std::vector<AstNode::SharedPtr> TypeRef::get_children() {
   std::vector<AstNode::SharedPtr> children;

   return children;
}

void TypeRef::__dump(int depth) {
   AstNode::__tab(depth);
   std::cout << "TypeRef [" << type->toString() << "]" << std::endl;

   for (auto c : get_children()) {
      c->__dump(depth + 1);
   }
}