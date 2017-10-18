//
// Created by Jonas Zell on 27.06.17.
//

#include "ClassDecl.h"
#include "ConstrDecl.h"
#include "FieldDecl.h"
#include "MethodDecl.h"
#include "../TypedefDecl.h"

ClassDecl::ClassDecl(
      string class_name,
      std::vector<std::shared_ptr<FieldDecl>> &&fields,
      std::vector<std::shared_ptr<MethodDecl>> &&methods,
      std::vector<std::shared_ptr<ConstrDecl>> &&constr,
      std::vector<std::shared_ptr<TypedefDecl>> &&typedefs,
      std::vector<GenericConstraint> &&generics,
      AccessModifier am,
      bool is_abstract,
      std::shared_ptr<TypeRef> extends,
      std::vector<std::shared_ptr<TypeRef>> &&implements,
      std::shared_ptr<DestrDecl> &&destr,
      std::vector<Statement::SharedPtr> &&innerDeclarations,
      std::vector<ExtensionConstraint> &&constraints) :
   className(class_name),
   fields(fields),
   methods(methods),
   constructors(constr),
   destructor(destr),
   generics(generics),
   typedefs(typedefs),
   am(am),
   is_abstract(is_abstract),
   parentClass(extends),
   conformsTo(implements),
   innerDeclarations(innerDeclarations),
   constraints(constraints)
{

}

ClassDecl::ClassDecl(
      string className,
      std::vector<std::shared_ptr<FieldDecl>> &&fields,
      std::vector<std::shared_ptr<MethodDecl>> &&methods,
      std::vector<std::shared_ptr<ConstrDecl>> &&constructors,
      std::vector<std::shared_ptr<TypedefDecl>>&&typedefs,
      std::vector<GenericConstraint> &&generics,
      AccessModifier am,
      std::vector<std::shared_ptr<TypeRef>> &&conformsTo,
      std::shared_ptr<DestrDecl> &&destr,
      std::vector<Statement::SharedPtr> &&innerDeclarations) :
   className(className),
   fields(fields),
   methods(methods),
   constructors(constructors),
   destructor(destr),
   typedefs(typedefs),
   generics(generics),
   am(am),
   is_abstract(true),
   is_protocol(true),
   conformsTo(conformsTo),
   innerDeclarations(innerDeclarations)
{

}

std::vector<std::shared_ptr<AstNode>> ClassDecl::get_children() {
   std::vector<std::shared_ptr<AstNode>> children;
   for (const auto& td : typedefs) {
      children.push_back(td);
   }
   for (const auto& constr : constructors) {
      children.push_back(constr);
   }
   for (const auto& field : fields) {
      children.push_back(field);
   }
   for (const auto& method : methods) {
      children.push_back(method);
   }
   for (const auto& inner : innerDeclarations) {
      children.push_back(inner);
   }

   return children;
}

void ClassDecl::__dump(int depth) {

}