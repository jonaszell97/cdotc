//
// Created by Jonas Zell on 27.06.17.
//

#include "MethodDecl.h"
#include "RecordDecl.h"

namespace cdot {
namespace ast {

bool MethodDecl::isTemplatedInitializer() const
{
   if (!support::isa<InitDecl>(this))
      return false;

   return getTemplateParams().size()
          > support::cast<RecordDecl>(declContext)->getTemplateParams().size();
}

} // namespace ast
} // namespace cdot