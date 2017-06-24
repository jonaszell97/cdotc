//
// Created by Jonas Zell on 24.06.17.
//

#ifndef MATHPARSER_ARITHMETIC_H
#define MATHPARSER_ARITHMETIC_H

#include "Variant.h"

namespace cdot {
namespace var {

class Arithmetic {
public:
    // binary math operators
    static Variant add(Variant v1, Variant v2);
    static Variant minus(Variant v1, Variant v2);
    static Variant multiply(Variant v1, Variant v2);
    static Variant divide(Variant v1, Variant v2);
    static Variant modulo(Variant v1, Variant v2);
    static Variant pow(Variant v1, Variant v2);

    // unary operators
    static Variant unary_minus(Variant v1);
    static Variant unary_not(Variant v1);
    static Variant bitwise_negate(Variant v1);

    // equality operators
    static Variant equal(Variant v1, Variant v2);
    static Variant not_equal(Variant v1, Variant v2);
    static Variant greater(Variant v1, Variant v2);
    static Variant greater_equal(Variant v1, Variant v2);
    static Variant lower(Variant v1, Variant v2);
    static Variant lower_equal(Variant v1, Variant v2);

    // logical operators
    static Variant logical_and(Variant v1, Variant v2);
    static Variant logical_or(Variant v1, Variant v2);

    // bitwise operators
    static Variant bitwise_and(Variant v1, Variant v2);
    static Variant bitwise_or(Variant v1, Variant v2);
    static Variant bitwise_xor(Variant v1, Variant v2);
    static Variant bitshift_left(Variant v1, Variant v2);
    static Variant bitshift_right(Variant v1, Variant v2);

};

} // namespace var
} // namespace cdot

#endif //MATHPARSER_ARITHMETIC_H
