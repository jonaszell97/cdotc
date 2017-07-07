//
// Created by Jonas Zell on 03.07.17.
//

#include "Array.h"


namespace cdot {
namespace lib {
namespace arr {

    Class::UniquePtr init() {
        const std::vector<std::string> _array_static_method_names = {
                
        };

        const std::vector<StaticFunction> _array_static_methods = {
                
        };

        const std::vector<std::vector<TypeSpecifier>> _array_static_arg_types = {
                
        };

        const std::vector<std::string> _array_instance_method_names = {
                
        };

        const std::vector<InstanceFunction> _array_instance_methods = {
                
        };

        const std::vector<std::vector<TypeSpecifier>> _array_instance_arg_types = {
                
        };

        const std::unordered_map<std::string, std::pair<TypeSpecifier, Variant::SharedPtr>> _array_class_constants = {

        };

        const std::vector<std::pair<std::string, TypeSpecifier>> _array_instance_props = {
                
        };

        Class::UniquePtr Array = std::make_unique<Class>("Array", Function(construct), AccessModifier::PUBLIC);

        for (int i = 0; i < _array_static_method_names.size(); i++) {
            Array->_add_static_builtin(_array_static_method_names[i], _array_static_methods[i],
                    AccessModifier::PUBLIC, _array_static_arg_types[i]);
        }
        for (int i = 0; i < _array_instance_method_names.size(); ++i) {
            Array->_add_builtin(_array_instance_method_names[i], _array_instance_methods[i],
                    AccessModifier::PUBLIC, _array_instance_arg_types[i]);
        }
        for (auto constant : _array_class_constants) {
            Array->add_static_property(constant.first, constant.second.first, constant.second.second);
        }
        for (auto prop : _array_instance_props) {
            Array->add_property(prop.first, prop.second, std::make_shared<Variant>(),
                    AccessModifier::PRIVATE);
        }

        return std::move(Array);
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
     * Array constructor from a Array literal.
     * @param this_arg
     * @param args
     * @return
     */
    Variant construct(Object *this_arg, std::vector<Variant> args) {
        assert_constr_args(1, {OBJECT_T}, args, "Array");
        this_arg->set_property("value", std::make_shared<Variant>(args.front()));

        return {};
    }


} // namespace arr
} // namespace lib
} // namespace cdot