//
// Created by Jonas Zell on 02.07.17.
//

#ifndef CDOT_MODULE_H
#define CDOT_MODULE_H


#include <string>
#include "Class/Class.h"
#include "GlobalContext.h"

class Module {
public:
    Module(std::string = "default");
    Variant::SharedPtr get_export(std::string);
    void add_export(std::string, Variant::SharedPtr);
    inline void set_name(std::string name) {
        module_name = name;
    }
    inline void set_exports(std::unordered_map<std::string, Variant::SharedPtr> exp) {
        exports = exp;
    }
    inline std::unordered_map<std::string, Variant::SharedPtr> get_exports() {
        return exports;
    };

    typedef std::unique_ptr<Module> UniquePtr;

protected:
    std::string module_name;
    std::unordered_map<std::string, Variant::SharedPtr> exports;
};


#endif //CDOT_MODULE_H
