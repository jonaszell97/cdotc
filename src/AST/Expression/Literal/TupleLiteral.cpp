//
// Created by Jonas Zell on 28.08.17.
//

#include "TupleLiteral.h"
#include "../../../Variant/Type/TupleType.h"

namespace cdot {
namespace ast {

TupleLiteral::TupleLiteral(
   std::vector<pair<string, Expression::SharedPtr>> &&elements)
   : Expression(TupleLiteralID), elements(std::move(elements))
{

}

bool TupleLiteral::isMetaTy() const
{
   return is_meta_ty;
}

void TupleLiteral::isMetaTy(bool is_meta_ty)
{
   TupleLiteral::is_meta_ty = is_meta_ty;
}

std::vector<pair<string, Expression::SharedPtr>> &
TupleLiteral::getElements()
{
   return elements;
}

void TupleLiteral::setElements(
   const std::vector<pair<string, Expression::SharedPtr>> &elements)
{
   TupleLiteral::elements = elements;
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