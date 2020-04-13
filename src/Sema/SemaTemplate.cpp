#include "cdotc/ILGen/ILGenPass.h"
#include "cdotc/Sema/SemaPass.h"
#include "cdotc/Sema/TemplateInstantiator.h"

using namespace cdot;
using namespace cdot::ast;
using namespace cdot::sema;
using namespace cdot::support;

bool SemaPass::inTemplate()
{
   for (auto Ctx = &getDeclContext(); Ctx; Ctx = Ctx->getParentCtx()) {
      if (auto ND = dyn_cast<NamedDecl>(Ctx->lookThroughExtension())) {
         if (ND->isTemplate())
            return true;
      }
   }

   return false;
}

bool SemaPass::inUnboundedTemplate()
{
   for (auto Ctx = &getDeclContext(); Ctx; Ctx = Ctx->getParentCtx()) {
      if (auto ND = dyn_cast<NamedDecl>(Ctx->lookThroughExtension())) {
         if (ND->isUnboundedTemplate())
            return true;
      }
   }

   return false;
}

bool SemaPass::isInDependentContext()
{
   for (auto Ctx = &getDeclContext(); Ctx; Ctx = Ctx->getParentCtx()) {
      if (auto ND = dyn_cast<NamedDecl>(Ctx->lookThroughExtension())) {
         if (ND->inDependentContext() || isa<ProtocolDecl>(ND))
            return true;
      }
   }

   return false;
}