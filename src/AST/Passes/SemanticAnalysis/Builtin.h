//
// Created by Jonas Zell on 20.08.17.
//

#ifndef CDOT_BUILTIN_H
#define CDOT_BUILTIN_H

#include <string>
#include <vector>

using std::string;

namespace cdot {

   enum class BuiltinFn : unsigned int {
      SIZEOF,
      ALIGNOF,
      TYPEOF,
      STACK_ALLOC,
      MEMCPY,
      MEMSET,
      BITCAST,
      NULLPTR,
      ISNULL,
      UNWRAP_PROTO,

      BuiltinSizeof
   };

   extern std::vector<string> BuiltinNamespaces;

   inline bool isBuilitinNamespace(const string& name) {
      return std::find(BuiltinNamespaces.begin(),
                       BuiltinNamespaces.end(), name) != BuiltinNamespaces.end();
   }

   class Builtin {
   public:
      static void ImportBuiltin(string &name);
   };

}

#endif //CDOT_BUILTIN_H
