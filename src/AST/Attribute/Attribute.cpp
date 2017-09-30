//
// Created by Jonas Zell on 20.08.17.
//

#include "Attribute.h"

namespace cdot {

   unordered_map<string, Attr> AttributeMap = {
      { "inline", Attr::Inline },
      { "carray", Attr::CArray },
      { "cstring", Attr::CString },
      { "primitive", Attr::Primitive },
      { "neverOmit", Attr::NeverOmit },
      { "_builtin", Attr::_builtin }
   };

   string isValidAttribute(Attribute attr) {
      switch (attr.kind) {
         case Attr::None:
            return "Unknown attribute " + attr.name;
         case Attr::_builtin:
            if (attr.args.size() != 1) {
               return "Attribute " + attr.name + " expects 1 argument";
            }
            break;
         case Attr::CArray:
         case Attr::CString:
         case Attr::NoCopy:
         case Attr::Primitive:
         case Attr::NeverOmit:
            if (!attr.args.empty()) {
               return "Attribute " + attr.name + " expects no arguments";
            }
            break;
         case Attr::Inline:
            if (attr.args.size() > 1) {
               return "Attribute " + attr.name + " expects 0 or 1 arguments";
            }

            if (!attr.args.empty()) {
               auto& arg = attr.args.front();
               if (arg != "always" && arg != "hint" && arg != "never") {
                  return "Invalid argument " + arg + " (Expected always, never or hint)";
               }
            }
            break;
      }

      return "";
   }

}