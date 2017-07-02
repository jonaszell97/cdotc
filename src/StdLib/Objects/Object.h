//
// Created by Jonas Zell on 18.06.17.
//

#ifndef OBJECT_H
#define OBJECT_H

#include <map>
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
    Variant call_method(std::string, std::vector<Variant>, std::string, bool = false);

    void set_property(std::string, Variant::SharedPtr = {});
    void add_property(std::string, Variant::SharedPtr = {});

    inline virtual Class* get_class() {
        return class_prototype;
    }

    bool has_uninitialized_fields();

    virtual std::string print();

    typedef std::shared_ptr<Object> SharedPtr;
    friend class Class;

protected:
    Class* class_prototype;
    bool generic_obj = false;
    std::map<std::string, Variant::SharedPtr> _properties;

    std::vector<std::string> uninitialized_fields;
};



#endif //OBJECT_H
