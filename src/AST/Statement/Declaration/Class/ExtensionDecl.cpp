//
// Created by Jonas Zell on 14.11.17.
//

#include "ExtensionDecl.h"

namespace cdot {
namespace ast {

ExtensionDecl::ExtensionDecl(
   AccessModifier am, string &&recordName,
   std::vector<std::shared_ptr<TypeRef>> &&conformsTo,
   std::vector<std::shared_ptr<Statement>> &&methods,
   std::vector<std::shared_ptr<TypedefDecl>> &&typedefs,
   std::vector<std::shared_ptr<PropDecl>> &&properties,
   std::vector<std::shared_ptr<ConstrDecl>> &&initializers,
   std::vector<Statement::SharedPtr> &&innerDeclarations)
   : RecordDecl(ExtensionDeclID, am, move(recordName), move(conformsTo),
                move(methods), move(typedefs), move(properties),
                move(innerDeclarations)), initializers(move(initializers))
{

}

const std::vector<std::shared_ptr<ConstrDecl>> &
ExtensionDecl::getInitializers() const
{
   return initializers;
}

void ExtensionDecl::setInitializers(
   const std::vector<std::shared_ptr<ConstrDecl>> &initializers)
{
   ExtensionDecl::initializers = initializers;
}

} // namespace ast
} // namespace cdot
