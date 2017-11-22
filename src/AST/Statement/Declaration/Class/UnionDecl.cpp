//
// Created by Jonas Zell on 10.10.17.
//

#include "UnionDecl.h"

#include "../../../Expression/TypeRef.h"
#include "../../../../Util.h"

using std::move;

namespace cdot {
namespace ast {

UnionDecl::UnionDecl(string &&name,
                     UnionTypes &&types,
                     bool isConst,
                     std::vector<std::shared_ptr<Statement>> &&methods,
                     std::vector<std::shared_ptr<TypedefDecl>> &&typedefs,
                     std::vector<std::shared_ptr<PropDecl>> &&properties,

                     std::vector<std::shared_ptr<Statement>> &&innerDecls)
   : RecordDecl(UnionDeclID, AccessModifier::PUBLIC, move(name), {},
                move(methods), move(typedefs), move(properties),
                move(innerDecls)),
     containedTypes(move(types)),
     is_const(isConst)
{

}

bool UnionDecl::isConst() const {
   return is_const;
}

void UnionDecl::isConst(bool is_const) {
   UnionDecl::is_const = is_const;
}

} // namespace ast
} // namespace cdot