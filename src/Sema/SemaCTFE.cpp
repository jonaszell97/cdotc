#include "SemaPass.h"

#include "ILGen/ILGenPass.h"
#include "Module/Module.h"
#include "Query/QueryContext.h"

namespace cdot {
namespace ast {

bool SemaPass::prepareGlobalForCtfe(VarDecl *Decl)
{
   if (!QC.TypecheckDecl(Decl))
      return false;

   if (auto F = support::dyn_cast<FieldDecl>(Decl)) {
      ILGen->DeclareField(F);
   }

   ILGen->visit(Decl);
   return true;
}

} // namespace ast
} // namespace cdot