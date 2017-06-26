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

    const std::vector<std::string> _integer_instance_props = {
            {"value"}
    };

    Class::UniquePtr init() {
        Method int_construct = Method("construct", construct);
        Class::UniquePtr Integer = std::make_unique<Class>("Integer", int_construct,
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
        for (auto prop : _integer_instance_props) {
            Integer->add_property(prop, {}, AccessModifier::PRIVATE);
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

    /******************************************/
    /*                                        */
    /*            INSTANCE METHODS            */
    /*                                        */
    /******************************************/

    Variant construct(Object* this_arg, std::vector<Variant> args) {
        assert_constr_args(1, { INT_T }, args, "Integer");
        this_arg->set_property("value", std::make_shared<Variant>(args.front()));

        return { };
    }

} // namespace intgr
} // namespace lib
} // namespace cdot