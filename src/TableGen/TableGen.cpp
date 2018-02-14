//
// Created by Jonas Zell on 01.02.18.
//

#include <Message/Diagnostics.h>
#include "TableGen.h"

#include "Record.h"
#include "Value.h"
#include "Support/Casting.h"

using namespace cdot::support;

namespace cdot {
namespace tblgen {

static Value *resolveValue(Value *V,
                           Class::BaseClass const &PreviousBase,
                           llvm::ArrayRef<Value *> ConcreteTemplateArgs,
                           SourceLocation errorLoc = {}) {
   if (auto Id = dyn_cast<IdentifierVal>(V)) {
      size_t i = 0;
      for (auto &P : PreviousBase.getBase()->getParameters()) {
         if (P.getName() == Id->getVal())
            break;

         ++i;
      }

      assert(i < ConcreteTemplateArgs.size());
      return ConcreteTemplateArgs[i];
   }
   else if (auto DA = dyn_cast<DictAccessExpr>(V)) {
      auto dict = resolveValue(DA->getDict(), PreviousBase,
                               ConcreteTemplateArgs, errorLoc);
      if (!isa<DictLiteral>(dict))
         diag::err(diag::err_generic_error)
            << "value is not a dictionary"
            << errorLoc << diag::term;

      auto val = cast<DictLiteral>(dict)->getValue(DA->getKey());
      if (!val)
         diag::err(diag::err_generic_error)
            << "key does not exist in dictionary"
            << errorLoc << diag::term;

      return val;
   }
   else {
      return V;
   }
}

static void resolveValues(Class::BaseClass const &Base,
                          Class::BaseClass const &PreviousBase,
                          llvm::ArrayRef<Value *> ConcreteTemplateArgs,
                          llvm::SmallVectorImpl<Value *> &DstValues) {
   for (auto &V : Base.getTemplateArgs()) {
      DstValues.push_back(resolveValue(V, PreviousBase, ConcreteTemplateArgs));
   }
}

static RecordField const*
implementBaseForRecord(Class::BaseClass const& Base,
                       Record &R,
                       llvm::ArrayRef<Value *> BaseTemplateArgs) {
   for (auto &Field : Base.getBase()->getFields()) {
      if (auto val = R.getOwnField(Field.getName())) {
         R.setFieldValue(Field.getName(), val->getDefaultValue());
      }
      else if (auto def = Field.getDefaultValue()) {
         R.setFieldValue(Field.getName(), resolveValue(def, Base,
                                                       BaseTemplateArgs,
                                                       Field.getDeclLoc()));
      }
      else if (Field.hasAssociatedTemplateParm()) {
         size_t idx = Field.getAssociatedTemplateParm();

         assert(idx < Base.getBase()->getParameters().size()
                && "invalid template parameter index");

         if (idx < Base.getTemplateArgs().size()) {
            R.setFieldValue(Field.getName(),
                            resolveValue(Base.getTemplateArgs()[idx], Base,
                                         BaseTemplateArgs, Field.getDeclLoc()));
         }
         else {
            auto P = Base.getBase()->getParameters()[idx];
            assert (P.getDefaultValue() && "template parm not supplied!");
            R.setFieldValue(Field.getName(),
                            resolveValue(P.getDefaultValue(), Base,
                                         BaseTemplateArgs,
                                         Field.getDeclLoc()));
         }
      }
      else {
         return &Field;
      }
   }

   // propagate resolved template arguments to the next base
   llvm::SmallVector<Value*, 8> NextBaseTemplateArgs;
   for (auto &NextBase : Base.getBase()->getBases()) {
      resolveValues(NextBase, Base, BaseTemplateArgs, NextBaseTemplateArgs);

      if (auto missing = implementBaseForRecord(NextBase, R,
                                                NextBaseTemplateArgs)) {
         return missing;
      }

      NextBaseTemplateArgs.clear();
   }

   return nullptr;
}

TableGen::FinalizeResult TableGen::finalizeRecord(Record &R)
{
   for (auto &Base : R.getBases()) {
      if (auto missing = implementBaseForRecord(Base, R,
                                                Base.getTemplateArgs())) {
         return {
            RFS_MissingFieldValue, missing->getName(),
            missing->getDeclLoc()
         };
      }
   }

   return { RFS_Success };
}

} // namespace tblgen
} // namespace cdot