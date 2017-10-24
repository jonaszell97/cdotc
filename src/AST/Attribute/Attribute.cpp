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
      { "extern", Attr::Extern },
      { "rawPtr", Attr::RawFunctionPtr },
      { "throws", Attr::Throws },
      { "nothrow", Attr::NoThrow },

      { "_builtin", Attr::_builtin },
      { "_opaque", Attr::_opaque },
      { "_align", Attr::_align }
   };

   string isValidAttribute(Attribute attr) {
      switch (attr.kind) {
         case Attr::None:
            return "Unknown attribute " + attr.name;
         case Attr::Extern:
         case Attr::_align:
            if (attr.args.size() != 1) {
               return "Attribute " + attr.name + " expects 1 argument";
            }
            break;
         case Attr::Throws:
            if (attr.args.empty()) {
               return "attribute throws expects at least 1 argument";
            }
            break;
         case Attr::CArray:
         case Attr::CString:
         case Attr::NoCopy:
         case Attr::Primitive:
         case Attr::NeverOmit:
         case Attr::_opaque:
         case Attr::RawFunctionPtr:
         case Attr::NoThrow:
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
               if (arg.type != VariantType::STRING) {
                  return "attribute inline expects string argument";
               }
               auto &str = arg.strVal;
               if (str != "always" && str != "hint" && str != "never") {
                  return "invalid argument " + str + " (expected always, never or hint)";
               }
            }
            break;
         default:
            break;
      }

      return "";
   }

}