//
// Created by Jonas Zell on 19.06.17.
//

#ifndef ARRAY_H
#define ARRAY_H


#include "../Variant.h"
#include <vector>

class Array {
public:
    Array(ValueType = ANY_T, int = -1);
    VariantPtr at(int);
    void push(VariantPtr);
    void push(Variant);
    void set(int, Variant);
    VariantPtr pop();
    std::string print();
    VariantPtr operator[](size_t);
    inline int get_length() {
        return _length;
    }
    inline bool is_var_length() {
        return _var_length;
    }
    inline ValueType get_type() {
        return _type;
    }

    typedef std::shared_ptr<Array> SharedPtr;
protected:
    int _length;
    bool _var_length;
    ValueType _type;
    std::vector<VariantPtr> _elements;
};


#endif //ARRAY_H
