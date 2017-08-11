//
// Created by Jonas Zell on 27.06.17.
//

#include "ClassDecl.h"

#include <iostream>

ClassDecl::ClassDecl(std::string class_name, std::vector<FieldDecl::SharedPtr> fields,
        std::vector<MethodDecl::SharedPtr> methods, std::vector<ConstrDecl::SharedPtr> constr,
    std::vector<TypedefDecl::SharedPtr> typedefs, std::vector<pair<string, TypeSpecifier>> generics, AccessModifier am,
        bool is_abstract, std::string extends, std::vector<std::string> implements) :
    class_name(class_name),
    fields(fields),
    methods(methods),
    constructors(constr),
    generics(generics),
    typedefs(typedefs),
    am(am),
    is_abstract(is_abstract),
    extends(extends),
    implements(implements)
{

}

std::vector<AstNode::SharedPtr> ClassDecl::get_children() {
    std::vector<AstNode::SharedPtr> children;
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

    return children;
}

void ClassDecl::__dump(int depth) {
    AstNode::__tab(depth);
    std::string extends_str = extends != "" ? " extends " + extends : "";
    std::string implements_str = implements.size() > 0 ? " implements " : "";
    for (int i = 0; i < implements.size(); ++i) {
        implements_str += implements[i];
        if (i != implements.size() - 1) {
            implements_str += ", ";
        }
    }

    std::cout << "ClassDecl" << " [" << (is_abstract ? "abstract " : "") <<
              class_name << extends_str << implements_str << "]" << std::endl;

    for (auto c : get_children()) {
        c->__dump(depth + 1);
    }
}