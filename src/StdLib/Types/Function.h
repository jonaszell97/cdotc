//
// Created by Jonas Zell on 02.07.17.
//

#ifndef CDOT_FUNCTION_H
#define CDOT_FUNCTION_H

#include "../../Variant/Variant.h"
#include "../Class/Class.h"

namespace cdot {
namespace lib {
namespace fun {

    Class::UniquePtr init();

    /******************************************/
    /*                                        */
    /*            STATIC METHODS              */
    /*                                        */
    /******************************************/




    /******************************************/
    /*                                        */
    /*          INSTANCE METHODS              */
    /*                                        */
    /******************************************/

    Variant construct(Object*, std::vector<Variant> args);
    Variant bind(Object*, std::vector<Variant> args);
    Variant call(Object*, std::vector<Variant> args);
    Variant apply(Object*, std::vector<Variant> args);


} // namespace fun
} // namespace lib
} // namespace cdot

#endif //CDOT_FUNCTION_H
