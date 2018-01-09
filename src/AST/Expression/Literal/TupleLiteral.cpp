//
// Created by Jonas Zell on 28.08.17.
//

#include "TupleLiteral.h"
#include "../../../Variant/Type/TupleType.h"

namespace cdot {
namespace ast {

TupleLiteral::TupleLiteral(
   std::vector<pair<string, std::shared_ptr<Expression>>> &&elements)
   : Expression(TupleLiteralID), elements(std::move(elements))
{

}


TupleType *TupleLiteral::getTupleType() const
{
   return tupleType;
}

void TupleLiteral::setTupleType(TupleType *tupleType)
{
   TupleLiteral::tupleType = tupleType;
}

} // namespace ast
} // namespace cdot