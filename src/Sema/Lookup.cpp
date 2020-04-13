
#include "cdotc/Sema/Lookup.h"

using namespace cdot;
using namespace cdot::ast;

bool SingleLevelLookupResult::unique() const
{
   NamedDecl* UniqueDecl = nullptr;
   for (auto* ND : *this) {
      if (!UniqueDecl) {
         UniqueDecl = ND;
      }
      else if (ND != UniqueDecl) {
         return false;
      }
   }

   return true;
}

ast::NamedDecl* SingleLevelLookupResult::uniqueDecl() const
{
   assert(unique() && "lookup result is not unique!");
   return front();
}

bool MultiLevelLookupResult::unique() const
{
   NamedDecl* UniqueDecl = nullptr;
   for (auto* ND : allDecls()) {
      if (!UniqueDecl) {
         UniqueDecl = ND;
      }
      else if (ND != UniqueDecl) {
         return false;
      }
   }

   return true;
}

ast::NamedDecl* MultiLevelLookupResult::uniqueDecl() const
{
   assert(unique() && "lookup result is not unique!");
   return front().front();
}