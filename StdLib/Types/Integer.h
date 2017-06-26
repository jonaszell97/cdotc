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

    Variant construct(Object*, std::vector<Variant> args);

} // namespace intgr
} // namespace lib
} // namespace cdot

#endif //CDOT_INTEGER_H
