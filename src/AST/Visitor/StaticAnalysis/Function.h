//
// Created by Jonas Zell on 12.08.17.
//

#ifndef CDOT_FUNCTION_H
#define CDOT_FUNCTION_H

#include "../../../Variant/Variant.h"
#include "../../../Variant/Type/Type.h"

using std::string;
using namespace cdot;

class Function {
public:
    Function(string&, Type*, std::vector<GenericType*>&, bool = false);

    inline void addArgument(Type* type, std::shared_ptr<Expression> def_val = nullptr) {
        arg_types.push_back(type);
        arg_defaults.push_back(def_val);
    }

    inline string& getName() {
        return name;
    }

    inline Type*& getReturnType() {
        return returnType;
    }

    inline std::vector<Type*>& getArgTypes() {
        return arg_types;
    }

    inline std::vector<std::shared_ptr<Expression>>& getArgDefaults() {
        return arg_defaults;
    }

    void hasHiddenParam(bool b) {
        hasHiddenParam_ = b;
    }

    bool hasHiddenParam() {
        return hasHiddenParam_;
    }

    inline std::vector<GenericType*>& getGenerics() {
        return generics;
    }

    inline bool isLambda() {
        return isLambda_;
    }

    typedef std::unique_ptr<Function> UniquePtr;

protected:
    string name;

    Type* returnType;
    std::vector<Type*> arg_types;
    std::vector<std::shared_ptr<Expression>> arg_defaults;
    std::vector<GenericType*>& generics;

    bool isLambda_;
    bool hasHiddenParam_ = false;
};


#endif //CDOT_FUNCTION_H
