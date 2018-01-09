//
// Created by Jonas Zell on 21.06.17.
//

#include "DictionaryLiteral.h"

#include "../TypeRef.h"
#include "../../../Variant/Type/Generic.h"

namespace cdot {
namespace ast {

DictionaryLiteral::DictionaryLiteral(
   std::vector<std::shared_ptr<Expression>> &&keys,
   std::vector<std::shared_ptr<Expression>> &&values)
   : Expression(NodeType::DictionaryLiteralID),
     keys(move(keys)), values(move(values))
{

}

ArrayLiteral::ArrayLiteral(std::vector<std::shared_ptr<Expression>> &&values)
   : Expression(ArrayLiteralID), values(move(values))
{

}

} // namespace ast
} // namespace cdot