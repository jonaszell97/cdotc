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
   None = 0,
   TYPEOF,
   MEMCPY,
   MEMSET,
   MemCmp,
   ISNULL,
   UNWRAP_PROTO,
   BuiltinSizeof,
   CtfePrintStackTrace,

   SIZEOF,
   ALIGNOF,
   NULLPTR,
   DefaultVal,

   STACK_ALLOC,
   BITCAST
};

extern std::vector<string> BuiltinNamespaces;

inline bool isBuilitinNamespace(const string& name) {
   return std::find(BuiltinNamespaces.begin(),
                    BuiltinNamespaces.end(), name) != BuiltinNamespaces.end();
}

class Builtin {
public:
   static void ImportBuiltin(const string &name);
};

}

#endif //CDOT_BUILTIN_H
