//
// Created by Jonas Zell on 19.06.17.
//

#include "UnaryOperator.h"

namespace cdot {
namespace ast {

UnaryOperator::UnaryOperator(string &&op, const string &fix)
   : Expression(UnaryOperatorID), op(move(op)), prefix(fix == "prefix")
{

}

UnaryOperator::UnaryOperator(string &&op, std::shared_ptr<Expression> &&target,
                             const string &fix)
   : Expression(UnaryOperatorID),
     target(move(target)),
     op(move(op)),
     prefix(fix == "prefix")
{

}

} // namespace ast
} // namespace cdot