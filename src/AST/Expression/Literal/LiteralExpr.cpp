//
// Created by Jonas Zell on 19.06.17.
//

#include "LiteralExpr.h"
#include "../../../Variant/Type/IntegerType.h"

LiteralExpr::LiteralExpr(Variant v) : LiteralExpr(v, v.getType()) {

}

LiteralExpr::LiteralExpr(Variant v, Type* type) : value(v), type(type) {
    switch (type->getTypeID()) {
        case TypeID::IntegerTypeID:
            switch (cast<IntegerType>(type)->getBitwidth()) {
                case 1:
                    className = "BoolLiteral"; break;
                case 8:
                    className = "CharLiteral"; break;
                case 16:
                case 32:
                case 64:
                default:
                    className = "IntegerLiteral"; break;
            }
            className = "IntegerLiteral";
            break;
        case TypeID::FPTypeID:
            className = "FloatingPointLiteral";
            break;
        case TypeID::VoidTypeID:
            className = "NullLiteral";
            break;
        default:
            assert(false && "Unknown literal type!");

    }
}

std::vector<AstNode::SharedPtr> LiteralExpr::get_children() {
    return memberExpr == nullptr ? std::vector<AstNode::SharedPtr>() : std::vector<AstNode::SharedPtr>{memberExpr };
}

void LiteralExpr::__dump(int depth) {
    for (int i = 0; i < depth; i++) {
        std::cout << "\t";
    }
    std::cout << className << " ['" << value.to_string(true) << "']" << std::endl;

    for (auto c : get_children()) {
        c->__dump(depth + 1);
    }
}