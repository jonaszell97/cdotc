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

class Object {
public:
    Object();
    Object(Class* class_prototype);

    Variant access_property(std::string);
    Variant call_method(std::string, std::vector<Variant>);

    void set_property(std::string, Variant::SharedPtr = {});
    void add_property(std::string, Variant::SharedPtr = {});

    inline Class* get_class() {
        return class_prototype;
    }

    virtual std::string print();

    typedef std::shared_ptr<Object> SharedPtr;
    friend class Class;

protected:
    Class* class_prototype;
    bool generic_obj = false;
    std::map<std::string, Variant::SharedPtr> _properties;
};



#endif //OBJECT_H
