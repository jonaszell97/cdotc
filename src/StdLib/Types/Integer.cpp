//
// Created by Jonas Zell on 25.06.17.
//

#include "Integer.h"

namespace cdot {
namespace lib {
namespace intgr {

    const std::unordered_map<std::string, std::pair<StaticFunction, std::vector<TypeSpecifier>>>
                _integer_static_methods = {
            {"parseInt", {parseInt, {TypeSpecifier(STRING_T)}}},
            {"parseLong", {parseLong, {TypeSpecifier(STRING_T)}}}
    };

    const std::unordered_map<std::string, std::pair<InstanceFunction, std::vector<TypeSpecifier>>>
                _integer_instance_methods = {
            {"toString", {toString, {}}},
            {"toLong", {toLong, {}}},
            {"toDouble", {toDouble, {}}},
            {"toFloat", {toFloat, {}}},
            {"toBool", {toBool, {}}},
    };

    const std::unordered_map<std::string, std::pair<TypeSpecifier, Variant::SharedPtr>> _integer_class_constants = {
            {"MAX_VALUE", {TypeSpecifier(INT_T), std::make_shared<Variant>(INT32_MAX)}},
            {"MIN_VALUE", {TypeSpecifier(INT_T), std::make_shared<Variant>(INT32_MIN)}}
    };

    const std::vector<std::pair<std::string, TypeSpecifier>> _integer_instance_props = {
            {"value", TypeSpecifier(INT_T)}
    };

    Class::UniquePtr init() {
        Class::UniquePtr Integer = std::make_unique<Class>("Integer", Function(construct),
            AccessModifier::PUBLIC);

        for (auto method : _integer_static_methods) {
            Integer->_add_static_builtin(method.first, method.second.first, AccessModifier::PUBLIC,
                    method.second.second);
        }
        for (auto method : _integer_instance_methods) {
            Integer->_add_builtin(method.first, method.second.first, AccessModifier::PUBLIC, method.second.second);
        }
        for (auto constant : _integer_class_constants) {
            Integer->add_static_property(constant.first, constant.second.first, constant.second.second);
        }
        for (auto prop : _integer_instance_props) {
            Integer->add_property(prop.first, prop.second, std::make_shared<Variant>(), AccessModifier::PRIVATE);
        }

        return std::move(Integer);
    }

    /******************************************/
    /*                                        */
    /*            STATIC METHODS              */
    /*                                        */
    /******************************************/

    Variant parseInt(std::vector<Variant> args) {
        assert_args(1, { STRING_T }, args, "Integer");
        return { std::stoi(args[0].get<std::string>()) };
    }

    Variant parseLong(std::vector<Variant> args) {
        assert_args(1, { STRING_T }, args, "Integer");
        return { std::stol(args[0].get<std::string>()) };
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

    Variant toString(Object* this_arg, std::vector<Variant> args) {
        assert_constr_args(0, { }, args, "Integer");

        return this_arg->access_property("value", "Integer")->to_string();
    }

    Variant toLong(Object* this_arg, std::vector<Variant> args) {
        assert_constr_args(0, { }, args, "Integer");

        return this_arg->access_property("value", "Integer")->cast_to(LONG_T);
    }

    Variant toDouble(Object* this_arg, std::vector<Variant> args) {
        assert_constr_args(0, { }, args, "Integer");

        return this_arg->access_property("value", "Integer")->cast_to(DOUBLE_T);
    }

    Variant toFloat(Object* this_arg, std::vector<Variant> args) {
        assert_constr_args(0, { }, args, "Integer");

        return this_arg->access_property("value", "Integer")->cast_to(FLOAT_T);
    }

    Variant toBool(Object* this_arg, std::vector<Variant> args) {
        assert_constr_args(0, { }, args, "Integer");

        return this_arg->access_property("value", "Integer")->cast_to(BOOL_T);
    }

} // namespace intgr
} // namespace lib
} // namespace cdot