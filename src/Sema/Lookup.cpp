
#include "cdotc/Sema/Lookup.h"

#include "cdotc/AST/Decl.h"
#include "cdotc/Basic/NestedNameSpecifier.h"
#include "cdotc/Parse/Parser.h"
#include "cdotc/Query/QueryContext.h"
#include "cdotc/Sema/SemaPass.h"
#include "cdotc/Support/Log.h"

#include <unordered_set>

using namespace cdot;
using namespace cdot::ast;
using namespace cdot::diag;
using namespace cdot::sema;
using namespace cdot::support;

LookupLevel SemaPass::getLookupLevel(DeclContext *DC) const
{
   auto it = lookupLevels.find(DC);
   if (it == lookupLevels.end()) {
      return LookupLevel::None;
   }

   return it->getSecond();
}

void SemaPass::updateLookupLevel(DeclContext *DC, LookupLevel newLevel)
{
   auto &ref = lookupLevels[DC];
   ref = (LookupLevel)((int)ref | (int)newLevel);
}

bool SemaPass::hasLookupLevel(DeclContext *DC, LookupLevel level) const
{
   if (isa<Decl>(DC) && cast<Decl>(DC)->isExternal()) {
      return true;
   }

   auto it = lookupLevels.find(DC);
   if (it == lookupLevels.end()) {
      return false;
   }

   return ((int)it->getSecond() & (int)level) != 0;
}

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

std::vector<ast::NamedDecl *> MultiLevelLookupResult::allDeclsStable() const
{
   std::vector<ast::NamedDecl *> result;
   result.reserve(Vec.size());

   for (auto *Decl : allDecls()) {
      result.push_back(Decl);
   }

   return result;
}