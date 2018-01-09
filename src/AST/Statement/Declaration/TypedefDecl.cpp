//
// Created by Jonas Zell on 01.08.17.
//

#include "TypedefDecl.h"
#include "../../Expression/TypeRef.h"

using namespace cdot::cl;

namespace cdot {
namespace ast {

TypedefDecl::TypedefDecl(
   AccessModifier access,
   string &&alias,
   std::shared_ptr<TypeRef> &&origin,
   std::vector<TemplateParameter> &&generics)
   : Statement(TypedefDeclID),
     access(access), alias(move(alias)), origin(move(origin)),
     templateParams(move(generics))
{

}

} // namespace ast
} // namespace cdot
