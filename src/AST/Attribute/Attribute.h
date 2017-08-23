//
// Created by Jonas Zell on 20.08.17.
//

#ifndef CDOT_ATTRIBUTE_H
#define CDOT_ATTRIBUTE_H

#include <string>
#include <vector>
#include <unordered_map>

using std::unordered_map;
using std::pair;
using std::string;

namespace cdot {

    enum class Attr : unsigned int {
        None,
        Inline,
        CArray,
        NoCopy,
        CString,
        Primitive
    };

    extern unordered_map<string, Attr> AttributeMap;

    struct Attribute {
        Attr kind;
        string name;
        std::vector<string> args;
    };

    string isValidAttribute(Attribute attr);

}

#endif //CDOT_ATTRIBUTE_H
