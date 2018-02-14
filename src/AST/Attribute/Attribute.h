//
// Created by Jonas Zell on 20.08.17.
//

#ifndef CDOT_ATTRIBUTE_H
#define CDOT_ATTRIBUTE_H

#include <string>
#include <vector>
#include <unordered_map>

#include "../../Variant/Variant.h"

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
   Implicit,

   _builtin,
   _opaque,
   _align
};

extern std::unordered_map<std::string, Attr> AttributeMap;

struct Attribute {
   Attr kind;
   std::string name;
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

std::string isValidAttribute(Attribute attr);

} // namespace cdot

#endif //CDOT_ATTRIBUTE_H
