//
// Created by Jonas Zell on 07.11.17.
//

#ifndef CDOT_TYPEUTILS_H
#define CDOT_TYPEUTILS_H


#include "BuiltinType.h"

namespace cdot {
namespace util {

bool isSubTypeOf(BuiltinType *ty, BuiltinType *of);
bool operator<(const BuiltinType &ty, const BuiltinType &of);

bool isSubTypeOrEqual(BuiltinType *ty, BuiltinType *of);
bool operator<=(const BuiltinType &ty, const BuiltinType &of);

bool isSuperTypeOf(BuiltinType *ty, BuiltinType *of);
bool operator>(const BuiltinType &ty, const BuiltinType &of);

bool isSuperTypeOrEqual(BuiltinType *ty, BuiltinType *of);
bool operator>=(const BuiltinType &ty, const BuiltinType &of);

bool isSameAs(BuiltinType *ty, BuiltinType *as);
bool operator==(const BuiltinType &ty, const BuiltinType &as);

bool isDifferentThan(BuiltinType *ty, BuiltinType *than);
bool operator!=(const BuiltinType &ty, const BuiltinType &than);

} // namespace util
} // namespace cdot


#endif //CDOT_TYPEUTILS_H
