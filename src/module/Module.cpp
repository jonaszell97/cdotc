//
// Created by Jonas Zell on 28.11.17.
//

#include <llvm/ADT/SmallString.h>

#include "Module.h"
#include "IdentifierTable.h"

#include "../Files/FileUtils.h"
#include "../AST/Statement/Declaration/NamedDecl.h"

namespace cdot {
namespace module {

void Module::addDecl(ast::NamedDecl *decl)
{
   declarations.try_emplace(decl->getName(), decl);
}

std::string Module::getJoinedName(char seperator) const
{
   llvm::SmallVector<Module const*, 4> modules{ this };

   auto M = this;
   while ((M = M->getParentModule()))
      modules.push_back(M);

   llvm::SmallString<128> fullName;
   auto end_it = modules.rend();
   size_t i = 0;

   for (auto it = modules.rbegin(); it != end_it; ++it, ++i) {
      if (i != 0) fullName += seperator;
      fullName += (*it)->getName();
   }

   return fullName.str();
}

void Module::setModuleMapFile(std::string &&moduleMapFile)
{
   this->moduleMapFile = std::move(moduleMapFile);

   baseFile = fs::getPath(this->moduleMapFile);
   baseFile += getJoinedName(fs::PathSeperator);
   baseFile += fs::PathSeperator;
   baseFile += name;
}

} // namespace module
} // namespace cdot