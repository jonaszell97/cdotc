//
// Created by Jonas Zell on 29.06.17.
//

#include "GotoStmt.h"

GotoStmt::GotoStmt(std::string label_name) : labelName(label_name) {

}

std::vector<AstNode::SharedPtr> GotoStmt::get_children() {
    return { };
}