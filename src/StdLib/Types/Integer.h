//
// Created by Jonas Zell on 25.06.17.
//

#ifndef CDOT_INTEGER_H
#define CDOT_INTEGER_H

#include "../Class.h"

namespace cdot {
namespace lib {
namespace intgr {

    Class::UniquePtr init();

    /******************************************/
    /*                                        */
    /*            STATIC METHODS              */
    /*                                        */
    /******************************************/

    Variant parseInt(std::vector<Variant> args);
    Variant parseLong(std::vector<Variant> args);


    /******************************************/
    /*                                        */
    /*          INSTANCE METHODS              */
    /*                                        */
    /******************************************/

    Variant construct(Object*, std::vector<Variant> args);
    Variant toString(Object*, std::vector<Variant> args);
    Variant toLong(Object*, std::vector<Variant> args);
    Variant toFloat(Object*, std::vector<Variant> args);
    Variant toDouble(Object*, std::vector<Variant> args);
    Variant toBool(Object*, std::vector<Variant> args);

} // namespace intgr
} // namespace lib
} // namespace cdot

#endif //CDOT_INTEGER_H
