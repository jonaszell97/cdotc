//
// Created by Jonas Zell on 28.06.17.
//

#include "LabelStmt.h"

LabelStmt::LabelStmt(std::string label_name) : labelName(label_name) {

}

std::vector<AstNode::SharedPtr> LabelStmt::get_children() {
    return {};
}