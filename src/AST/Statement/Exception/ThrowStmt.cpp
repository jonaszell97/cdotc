//
// Created by Jonas Zell on 05.10.17.
//

#include "ThrowStmt.h"
#include "../../Expression/Expression.h"

ThrowStmt::ThrowStmt(std::shared_ptr<Expression> &&thrownVal)
   : thrownVal(thrownVal)
{

}

std::vector<AstNode::SharedPtr> ThrowStmt::get_children()
{
   return { thrownVal };
}

void ThrowStmt::__dump(int depth)
{

}

const std::shared_ptr<Expression> &ThrowStmt::getThrownVal() const {
   return thrownVal;
}

void ThrowStmt::setThrownVal(const std::shared_ptr<Expression> &thrownVal) {
   ThrowStmt::thrownVal = thrownVal;
}

BuiltinType *ThrowStmt::getThrownType() const {
   return thrownType;
}

void ThrowStmt::setThrownType(BuiltinType *thrownType) {
   ThrowStmt::thrownType = thrownType;
}
