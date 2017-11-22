//
// Created by Jonas Zell on 29.08.17.
//

#include "Enum.h"
#include "../../../../Variant/Type/IntegerType.h"
#include "../../../Statement/Declaration/Class/EnumDecl.h"
#include "../../CodeGen/CodeGen.h"

namespace cdot {
namespace cl {

Enum::Enum(
   AccessModifier am,
   const string &name,
   const SourceLocation &loc,
   ast::EnumDecl *decl)
   : Class(am, name, decl, loc, false, false, false)
{
   needsTypeInfo = false;
   is_class = false;

   this->rawType = IntegerType::get(sizeof(int*) * 8);
}

void Enum::addCase(const string &name, EnumCase &&case_, SourceLocation loc)
{
   cases.emplace(name, case_);
   if (case_.associatedValues.size() > maxAssociatedValues) {
      maxAssociatedValues = case_.associatedValues.size();
   }

   if (!case_.associatedValues.empty()) {
      hasAssociatedValues_ = true;

      std::vector<Argument> args;
      for (const auto& assoc : case_.associatedValues) {
         args.emplace_back(assoc.first, assoc.second);
      }

      QualType retTy;
      declareMethod(name, retTy, AccessModifier::PUBLIC,
                    std::move(args), false, nullptr, loc);
   }
}

llvm::Type* Enum::getSelfType() const
{
   if (isRawEnum()) {
      return rawType->getLlvmType();
   }

   return Record::getSelfType();
}

void Enum::generateMemoryLayout(CodeGen &CGM)
{
   alignment = sizeof(int*);
   occupiedBytes = sizeof(int*);
   memoryLayout.push_back(rawType->getLlvmType());

   for (size_t i = 0; i < maxAssociatedValues; ++i) {
      memoryLayout.push_back(CodeGen::Int8PtrTy);
      occupiedBytes += sizeof(int*);
   }
}

}
}