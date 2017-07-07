//
// Created by Jonas Zell on 03.07.17.
//

#ifndef CDOT_ARRAY_H
#define CDOT_ARRAY_H


#include "../Class/Class.h"

namespace cdot {
namespace lib {
namespace arr {

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
    Variant map(Object*, std::vector<Variant> args);
    Variant reduce(Object*, std::vector<Variant> args);

} // namespace arr
} // namespace lib
} // namespace cdot



#endif //CDOT_ARRAY_H
