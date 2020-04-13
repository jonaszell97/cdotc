#include "cdotc/ILGen/ILGenPass.h"
#include "cdotc/Module/Module.h"
#include "cdotc/Query/QueryContext.h"
#include "cdotc/Sema/SemaPass.h"

namespace cdot {
namespace ast {

bool SemaPass::prepareGlobalForCtfe(VarDecl* Decl)
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