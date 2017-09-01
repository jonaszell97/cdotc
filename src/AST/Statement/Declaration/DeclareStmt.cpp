//
// Created by Jonas Zell on 13.08.17.
//

#include "DeclareStmt.h"
#include "../../Expression/TypeRef.h"
#include "FuncArgDecl.h"

DeclareStmt::DeclareStmt(string var_name, TypeRef::SharedPtr var_type, bool is_const) :
    declKind(DeclarationType::VAR_DECL),
    declaredName(var_name),
    type(var_type),
    isConst(is_const)
{

}

DeclareStmt::DeclareStmt(AccessModifier access, string class_name, ObjectType *extends, std::vector<ObjectType*> with,
        bool is_abstract, std::vector<ObjectType*> generics) :
    declKind(DeclarationType::CLASS_DECL),
    declaredName(class_name),
    generics(generics),
    am(access),
    extends(extends),
    conformsTo(with),
    is_abstract(is_abstract)
{

}

DeclareStmt::DeclareStmt(string func_name, TypeRef::SharedPtr ret_type, std::vector<FuncArgDecl::SharedPtr> args,
    std::vector<ObjectType*> generics) :
    declKind(DeclarationType::FUNC_DECL),
    declaredName(func_name),
    generics(generics),
    args(args),
    type(ret_type)
{

}

std::vector<AstNode::SharedPtr> DeclareStmt::get_children() {
    std::vector<AstNode::SharedPtr> children;

    return children;
}

void DeclareStmt::__dump(int depth) {
    AstNode::__tab(depth);
    std::cout << "DeclareStmt []" << std::endl;
}