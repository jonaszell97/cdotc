//
// Created by Jonas Zell on 24.10.17.
//

#include <sstream>

#include "SemaPass.h"
#include "TemplateInstantiator.h"
#include "Record/Record.h"

using namespace cdot::support;
using namespace cdot::sema;

namespace cdot {
namespace ast {

cl::Record* SemaPass::getRecord(llvm::StringRef name,
                                std::vector<TemplateArg> const& templateArgs,
                                const SourceLocation &loc) {
   auto rec = getRecord(name);
   if (!rec)
      return nullptr;

   if (rec->isTemplated()) {
      if (auto Rec = currentClass()) {
         // we are in an instantiation dependant context
         if (Rec->isTemplated())
            return rec;
      }

      TemplateArgList list(*this, rec, templateArgs);

      if (!list.checkCompatibility()) {
         for (auto &diag : list.getDiagnostics())
            diag << diag::cont;

         encounteredError = true;
         return rec;
      }

      if (list.isStillDependant())
         return rec;

      auto Inst = TemplateInstantiator::InstantiateRecord(*this, loc, rec,
                                                          std::move(list));

      rec = Inst;
   }

   return rec;
}

cl::Record* SemaPass::InstantiateRecord(cl::Record *R,
                                        sema::TemplateArgList &&TAs,
                                        SourceLocation loc) {
   auto Inst = TemplateInstantiator::InstantiateRecord(*this, loc, R,
                                                       std::move(TAs));

   return Inst;
}

} // namespace ast
} // namespace cdot