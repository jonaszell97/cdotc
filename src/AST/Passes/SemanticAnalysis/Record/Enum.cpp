//
// Created by Jonas Zell on 29.08.17.
//

#include "Enum.h"
#include "../../../../Variant/Type/IntegerType.h"
#include "../../CodeGen/CodeGen.h"

#include "../../../Statement/Declaration/Class/RecordDecl.h"

namespace cdot {
namespace cl {

Enum::Enum(AccessModifier am,
           const string &name,
           Namespace *NS,
           std::vector<TemplateParameter> &&templateParams,
           const SourceLocation &loc,
           ast::EnumDecl *decl)
   : Record(EnumID, name, NS, am, move(templateParams), loc, decl),
     rawType(IntegerType::get())
{

}

void Enum::addCase(const string &name, EnumCase &&case_, SourceLocation loc)
{
   case_.loc = loc;
   cases.emplace(name, case_);
   if (case_.associatedValues.size() > maxAssociatedValues) {
      maxAssociatedValues = case_.associatedValues.size();
   }

   if (!case_.associatedValues.empty()) {
      hasAssociatedValues_ = true;
   }
}

void Enum::calculateSizeImpl()
{
   occupiedBytes = rawType->getSize();
   occupiedBytes += sizeof(void*) * maxAssociatedValues;
   alignment = sizeof(void*);
}

}
}