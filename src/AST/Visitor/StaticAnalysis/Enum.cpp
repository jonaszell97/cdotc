//
// Created by Jonas Zell on 29.08.17.
//

#include "Enum.h"

namespace cdot {
namespace cl {

   Enum::Enum(string &name, std::vector<ObjectType *> &conformsTo, std::vector<ObjectType *> &generics) :
      Class(name, nullptr, conformsTo, generics, nullptr)
   {
      type->isEnum(true);
      needsTypeInfo = false;
      is_class = false;
   }

   void Enum::generateMemoryLayout(llvm::IRBuilder<> &Builder)
   {
      alignment = sizeof(int*);
      occupiedBytes = sizeof(int*);
      memoryLayout.push_back(Builder.getInt64Ty());

      for (size_t i = 0; i < maxAssociatedValues; ++i) {
         memoryLayout.push_back(Builder.getInt8PtrTy());
         occupiedBytes += sizeof(int*);
      }
   }

}
}