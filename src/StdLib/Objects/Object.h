//
// Created by Jonas Zell on 18.06.17.
//

#ifndef OBJECT_H
#define OBJECT_H

#include <unordered_map>
#include <string>
#include <vector>
#include "../../Variant/Variant.h"

class Class;

class Object : public std::enable_shared_from_this<Object> {
public:
    Object(bool = false);
    Object(Class* class_prototype);
    virtual void operator=(const Object& o);

    Variant::SharedPtr access_property(std::string, std::string, bool = false);
    virtual Variant call_method(std::string, std::vector<Variant>, std::string, bool = false);

    void set_property(std::string, Variant::SharedPtr = {});
    void add_property(std::string, Variant::SharedPtr = {});

    inline virtual Class* get_class() {
        return class_prototype;
    }
    inline virtual std::unordered_map<std::string, Variant::SharedPtr> get_properties() {
        return _properties;
    };

    std::string has_uninitialized_fields();

    virtual std::string print();

    typedef std::shared_ptr<Object> SharedPtr;
    friend class Class;

protected:
    Class* class_prototype;
    bool generic_obj = false;
    std::unordered_map<std::string, Variant::SharedPtr> _properties;

    std::vector<std::string> uninitialized_fields;
};

namespace cdot {
namespace lib {

    void assert_args(int size, std::vector<TypeSpecifier> types, std::vector<Variant> args, std::string name);
    void assert_constr_args(int size, std::vector<TypeSpecifier> types, std::vector<Variant> args, std::string name);

namespace obj {

    /******************************************/
    /*                                        */
    /*           INSTANCE METHODS             */
    /*                                        */
    /******************************************/

    Variant construct(Object *this_arg, std::vector<Variant> args);
    Variant toString(Object *this_arg, std::vector<Variant> args);

    /******************************************/
    /*                                        */
    /*             STATIC METHODS             */
    /*                                        */
    /******************************************/

    Variant hashCode(std::vector<Variant> args);

} // namespace obj
} // namespace lib
} // namespace cdot


#endif //OBJECT_H
