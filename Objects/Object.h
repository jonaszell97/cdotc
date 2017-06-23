//
// Created by Jonas Zell on 18.06.17.
//

#ifndef OBJECT_H
#define OBJECT_H

#include <map>
#include <string>
#include "../Variant.h"
#include "Function.h"

enum AccessModifier {
    ACC_PUBLIC,
    ACC_PRIVATE,
    ACC_PROTECTED
};

class Object;

class ObjectProperty {
public:
    explicit ObjectProperty(std::string, VariantPtr, ValueType = ANY_T, AccessModifier = ACC_PUBLIC);
    VariantPtr get();
    void set(VariantPtr);
    void set(Variant);
    ObjectProperty set_object(Object*);
    std::string get_name();

    typedef std::shared_ptr<ObjectProperty> SharedPtr;
    friend class Object;
protected:
    Object *_object;
    ValueType _type;
    AccessModifier _access_modifier;
    std::string _property_name;
    VariantPtr _value;
};

class Object {
public:
    explicit Object(std::string = "Object", AccessModifier = ACC_PUBLIC);
    void add_property(ObjectProperty::SharedPtr);
    void add_method(Function);
    void reassign_property(std::string, VariantPtr);
    VariantPtr access_property(std::string);
    VariantPtr call_method(std::string, std::vector<VariantPtr>);
    std::string print();
    friend struct Variant;
    friend class ObjectProperty;

    typedef std::shared_ptr<Object> SharedPtr;
protected:
    std::string _class_name;
    AccessModifier _access_modifier;
    std::map<std::string, ObjectProperty::SharedPtr> _properties;
    std::map<std::string, Function> _methods;
};


#endif //OBJECT_H
