//
// Created by Jonas Zell on 19.06.17.
//

#include <iostream>
#include "DeclStmt.h"
#include "../CompoundStmt.h"
#include "../../../Util.h"
#include "../../Expression/Literal/ArrayLiteral.h"
#include "../../../StdLib/Objects/Array.h"


DeclStmt::DeclStmt() : declarations{}, decl_identifiers{} {

}

void DeclStmt::add_declaration(std::string ident, TypeSpecifier type, Expression::SharedPtr val) {
    if (std::find(decl_identifiers.begin(), decl_identifiers.end(), ident) != decl_identifiers.end()) {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Duplicate declaration of identifier " + ident);
    }

    declarations.push_back(std::pair<TypeSpecifier, Expression::SharedPtr>{type, val});
    decl_identifiers.push_back(ident);
}

std::vector<AstNode::SharedPtr> DeclStmt::get_children() {
    std::vector<AstNode::SharedPtr> children;
    for (auto decl : declarations) {
        if (decl.second) {
            children.push_back(decl.second);
        }
    }

    return children;
}

void DeclStmt::__dump(int depth) {
    for (int i = 0; i < decl_identifiers.size(); ++i) {
        AstNode::__tab(depth);
        std::cout << "DeclStmt [" << declarations[i].first.to_string() << " " << decl_identifiers[i] << "]" <<
                                                                                                            std::endl;
        if (declarations[i].second) {
            declarations[i].second->__dump(depth + 1);
        }
    }
}