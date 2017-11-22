//
// Created by Jonas Zell on 20.08.17.
//

#ifndef CDOT_ATTRIBUTE_H
#define CDOT_ATTRIBUTE_H

#include <string>
#include <vector>
#include <unordered_map>
#include "../../Variant/Variant.h"

using std::unordered_map;
using std::pair;
using std::string;

namespace cdot {

enum class Attr : unsigned int {
   None,
   Inline,
   CArray,
   RawFunctionPtr,
   NoCopy,
   CString,
   Primitive,
   NeverOmit,
   Extern,
   Throws,
   NoThrow,

   _builtin,
   _opaque,
   _align
};

extern unordered_map<string, Attr> AttributeMap;

struct Attribute {
   Attr kind;
   string name;
   std::vector<Variant> args;
};

class AttributeOwner {
protected:
   std::vector<Attribute> attributes;

public:
   void setAttributes(std::vector<Attribute> &&attrs);
   const std::vector<Attribute> &getAttributes() const;

   bool hasAttribute(Attr kind);
   Attribute &getAttribute(Attr kind);
};

string isValidAttribute(Attribute attr);

} // namespace cdot

#endif //CDOT_ATTRIBUTE_H
