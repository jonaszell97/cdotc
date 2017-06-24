//
// Created by Jonas Zell on 24.06.17.
//

#ifndef MATHPARSER_CONVERSION_H
#define MATHPARSER_CONVERSION_H


#include "Variant.h"

namespace cdot {
namespace var {

    class Converter {
    public:
        static Variant cast(Variant, ValueType);
    };

} // namespace var
} // namespace cdot


#endif //MATHPARSER_CONVERSION_H
