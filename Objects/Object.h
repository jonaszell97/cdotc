//
// Created by Jonas Zell on 18.06.17.
//

#ifndef OBJECT_H
#define OBJECT_H

#include <map>
#include <string>
#include "../Variant/Variant.h"

class Function;

enum AccessModifier {
    ACC_PUBLIC,
    ACC_PRIVATE,
    ACC_PROTECTED
};

class Object {
public:
    explicit Object(std::string = "Object", AccessModifier = ACC_PUBLIC);
    void add_property(std::string, Variant::SharedPtr);
    void add_method(std::string, std::shared_ptr<Function>);
    void reassign_property(std::string, Variant::SharedPtr);
    Variant access_property(std::string);
    Variant call_method(std::string, std::vector<Variant>);

    std::string print();
    friend struct Variant;
    friend class ObjectProperty;

    typedef std::shared_ptr<Object> SharedPtr;

protected:
    std::string _class_name;
    AccessModifier _access_modifier;
    std::map<std::string, Variant::SharedPtr> _properties;
    std::map<std::string, std::shared_ptr<Function>> _methods;
};


#endif //OBJECT_H
