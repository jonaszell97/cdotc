//
// Created by Jonas Zell on 14.08.17.
//

#include "AutoType.h"

namespace cdot {

AutoType *AutoType::Instance = nullptr;

AutoType* AutoType::get()
{
   if (Instance == nullptr) {
      Instance = new AutoType;
   }

   return Instance;
}

AutoType::AutoType()
{
   id = TypeID::AutoTypeID;
}

std::string AutoType::toString() const
{
   return "auto";
}
}