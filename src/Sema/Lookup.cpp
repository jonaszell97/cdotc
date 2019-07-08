
#include "Lookup.h"

using namespace cdot;
using namespace cdot::ast;

bool SingleLevelLookupResult::unique() const
{
   NamedDecl *UniqueDecl = nullptr;
   for (auto *ND : *this) {
      if (!UniqueDecl) {
         UniqueDecl = ND;
      }
      else if (ND != UniqueDecl) {
         return false;
      }
   }

   return true;
}

bool MultiLevelLookupResult::unique() const
{
   NamedDecl *UniqueDecl = nullptr;
   for (auto *ND : allDecls()) {
      if (!UniqueDecl) {
         UniqueDecl = ND;
      }
      else if (ND != UniqueDecl) {
         return false;
      }
   }

   return true;
}