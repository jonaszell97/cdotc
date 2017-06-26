//
// Created by Jonas Zell on 25.06.17.
//

#include "Integer.h"

namespace cdot {
namespace lib {
namespace intgr {

    const std::unordered_map<std::string, StaticFunction> _integer_static_methods = {
            {"parseInt", parseInt}
    };

    const std::unordered_map<std::string, InstanceFunction> _integer_instance_methods = {

    };

    const std::unordered_map<std::string, Variant::SharedPtr> _integer_class_constants = {
            {"MAX_VALUE", std::make_shared<Variant>(INT32_MAX)},
            {"MIN_VALUE", std::make_shared<Variant>(INT32_MIN)}
    };

    Class::UniquePtr init() {
        Class::UniquePtr Integer = std::make_unique<Class>("Integer", std::vector<ValueType>{ INT_T },
            AccessModifier::PUBLIC);

        for (auto method : _integer_static_methods) {
            Integer->_add_static_builtin(method.first, method.second);
        }
        for (auto method : _integer_instance_methods) {
            Integer->_add_builtin(method.first, method.second);
        }
        for (auto constant : _integer_class_constants) {
            Integer->add_static_property(constant.first, constant.second);
        }

        return std::move(Integer);
    }

    /******************************************/
    /*                                        */
    /*            STATIC METHODS              */
    /*                                        */
    /******************************************/

    Variant parseInt(std::vector<Variant> args) {
        return { std::stoi(args[0].get<std::string>()) };
    }

} // namespace intgr
} // namespace lib
} // namespace cdot