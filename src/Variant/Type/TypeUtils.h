//
// Created by Jonas Zell on 07.11.17.
//

#ifndef CDOT_TYPEUTILS_H
#define CDOT_TYPEUTILS_H


#include "Type.h"

namespace cdot {
namespace util {

bool isSubTypeOf(Type *ty, Type *of);
bool operator<(const Type &ty, const Type &of);

bool isSubTypeOrEqual(Type *ty, Type *of);
bool operator<=(const Type &ty, const Type &of);

bool isSuperTypeOf(Type *ty, Type *of);
bool operator>(const Type &ty, const Type &of);

bool isSuperTypeOrEqual(Type *ty, Type *of);
bool operator>=(const Type &ty, const Type &of);

bool isSameAs(Type *ty, Type *as);
bool operator==(const Type &ty, const Type &as);

bool isDifferentThan(Type *ty, Type *than);
bool operator!=(const Type &ty, const Type &than);

} // namespace util
} // namespace cdot


#endif //CDOT_TYPEUTILS_H
