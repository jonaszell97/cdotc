//
// Created by Jonas Zell on 13.08.17.
//

#include "VoidType.h"

namespace cdot {

VoidType* VoidType::Instance = nullptr;

VoidType* VoidType::get()
{
   if (Instance == nullptr)
      Instance = new VoidType;

   return Instance;
}

VoidType::VoidType()
{
   id = TypeID::VoidTypeID;
}

} // namespace cdot