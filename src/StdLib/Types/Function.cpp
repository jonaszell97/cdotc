//
// Created by Jonas Zell on 02.07.17.
//

#include "Function.h"


namespace cdot {
namespace lib {
namespace fun {


    Class::UniquePtr init() {
        const std::unordered_map<std::string, std::pair<StaticFunction, std::vector<TypeSpecifier>>>
                _fun_static_methods = {

        };

        const std::unordered_map<std::string, std::pair<InstanceFunction, std::vector<TypeSpecifier>>>
                _fun_instance_methods = {
                {"bind", {bind, {TypeSpecifier(ANY_T)}}}
        };

        const std::unordered_map<std::string, std::pair<TypeSpecifier, Variant::SharedPtr>> _fun_class_constants = {

        };

        const std::vector<std::pair<std::string, TypeSpecifier>> _fun_instance_props = {

        };

        Class::UniquePtr _Function = std::make_unique<Class>("Function", Function(construct), AccessModifier::PUBLIC);

        for (auto method : _fun_static_methods) {
            _Function->_add_static_builtin(method.first, method.second.first, AccessModifier::PUBLIC,
                    method.second.second);
        }
        for (auto method : _fun_instance_methods) {
            _Function->_add_builtin(method.first, method.second.first, AccessModifier::PUBLIC, method.second.second);
        }
        for (auto constant : _fun_class_constants) {
            _Function->add_static_property(constant.first, constant.second.first, constant.second.second);
        }
        for (auto prop : _fun_instance_props) {
            _Function->add_property(prop.first, prop.second, std::make_shared<Variant>(),
                    AccessModifier::PRIVATE);
        }

        return std::move(_Function);
    }

    /******************************************/
    /*                                        */
    /*            STATIC METHODS              */
    /*                                        */
    /******************************************/



    /******************************************/
    /*                                        */
    /*            INSTANCE METHODS            */
    /*                                        */
    /******************************************/

    /**
     * Function constructor
     * @param this_arg
     * @param args
     * @return
     */
    Variant construct(Object *this_arg, std::vector<Variant> args) {
        assert_constr_args(0, {}, args, "Function");
        return {};
    }

    /**
     * Binds the function to the given argument
     * @param this_arg
     * @param args
     * @return
     */
    Variant bind(Object *this_arg, std::vector<Variant> args) {
        assert_args(1, { ANY_T }, args, "bind");

        Function* func = static_cast<Function*>(this_arg);

        func->bind(std::make_shared<Variant>(args[0]));

        return {};
    }


} // namespace fun
} // namespace lib
} // namespace cdot