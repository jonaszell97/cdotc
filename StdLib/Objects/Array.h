//
// Created by Jonas Zell on 19.06.17.
//

#ifndef ARRAY_H
#define ARRAY_H


#include "../../Variant/Variant.h"
#include <vector>
#include "Object.h"
#include "../../AST/Expression/Expression.h"

class Array : public Object {
public:
    Array(ValueType = ANY_T, int = -1);
    Array& operator=(const Array&);
    Variant::SharedPtr at(int);
    void push(Variant::SharedPtr);
    void push(Variant);
    void set(int, Variant::SharedPtr);
    Variant::SharedPtr pop();
    std::string print();
    Variant::SharedPtr operator[](size_t);
    inline int get_length() {
        return _length;
    }
    inline bool is_var_length() {
        return _var_length;
    }
    inline ValueType get_type() {
        return _type;
    }
    inline void set_type(ValueType type) {
        _type = type;
    }
    inline void set_length(int length) {
        _length = length;
    }
    inline virtual std::string class_name() {
        return "Array";
    }

    typedef std::shared_ptr<Array> SharedPtr;

protected:
    int _length;
    bool _var_length;
    ValueType _type;
    std::vector<Variant::SharedPtr> _elements;
};


#endif //ARRAY_H
