//
// Created by Jonas Zell on 27.06.17.
//

#include "ClassDecl.h"

#include <iostream>

ClassDecl::ClassDecl(std::string class_name, std::vector<FieldDecl::SharedPtr> fields,
        std::vector<MethodDecl::SharedPtr> methods, ConstrDecl::SharedPtr constr, AccessModifier am,
        std::vector<std::pair<std::string, OperatorDecl::SharedPtr>> unary_operators,
        std::vector<std::pair<std::string, OperatorDecl::SharedPtr>> binary_operators, bool is_interface,
        bool is_abstract, std::string extends, std::vector<std::string> implements) :
    class_name(class_name),
    fields(fields),
    methods(methods),
    constr(constr),
    unary_operators(unary_operators),
    binary_operators(binary_operators),
    am(am),
    is_interface(is_interface),
    is_abstract(is_abstract),
    extends(extends),
    implements(implements)
{

}

std::vector<AstNode::SharedPtr> ClassDecl::get_children() {
    std::vector<AstNode::SharedPtr> children;
    if (constr != nullptr) {
        children.push_back(constr);
    }
    for (auto field : fields) {
        children.push_back(field);
    }
    for (auto method : methods) {
        children.push_back(method);
    }
    for (auto op : unary_operators) {
        children.push_back(op.second);
    }
    for (auto op : binary_operators) {
        children.push_back(op.second);
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

    std::cout << (is_interface ? "InterfaceDecl" : "ClassDecl") << " [" << (is_abstract ? "abstract " : "") <<
              class_name << extends_str << implements_str << "]" << std::endl;

    for (auto c : get_children()) {
        c->__dump(depth + 1);
    }
}