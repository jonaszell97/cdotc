//
// Created by Jonas Zell on 28.11.17.
//

#include "Module.h"

namespace cdot {
namespace module {

llvm::StringMap<Module*> Module::Instances;

Module * Module::get(std::string &&Name, std::string &&fileName,
                     std::string &&md5)
{
   auto it = Instances.find(Name);
   if (it != Instances.end())
      return it->second;

   auto M = new Module(move(Name), move(fileName), move(md5));
   Instances[Name] = M;

   return M;
}

} // namespace module
} // namespace cdot