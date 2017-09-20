//
// Created by Jonas Zell on 29.08.17.
//

#include "Enum.h"
#include "../../../Variant/Type/IntegerType.h"

namespace cdot {
namespace cl {

   Enum::Enum(AccessModifier am, string &name, std::vector<ObjectType *> &conformsTo,
      std::vector<ObjectType *> &generics) :
      Class(am, name, nullptr, conformsTo, generics, nullptr)
   {
      type->isEnum(true);
      needsTypeInfo = false;
      is_class = false;

      this->rawType = IntegerType::get(sizeof(int*) * 8);
   }

   void Enum::generateMemoryLayout(llvm::IRBuilder<> &Builder)
   {
      alignment = sizeof(int*);
      occupiedBytes = sizeof(int*);
      memoryLayout.push_back(rawType->getLlvmType());

      for (size_t i = 0; i < maxAssociatedValues; ++i) {
         memoryLayout.push_back(Builder.getInt8PtrTy());
         occupiedBytes += sizeof(int*);
      }
   }

}
}