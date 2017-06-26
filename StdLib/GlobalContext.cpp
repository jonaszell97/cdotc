//
// Created by Jonas Zell on 25.06.17.
//

#include "GlobalContext.h"
#include "Types/Integer.h"

GlobalContext::GlobalContext() {}

bool GlobalContext::initialized = false;
std::unordered_map<std::string, std::unique_ptr<Class>> GlobalContext::classes =
        std::unordered_map<std::string, std::unique_ptr<Class>>();

void GlobalContext::init() {
    if (initialized) {
        return;
    }

    initialized = true;

    Class::UniquePtr Object = std::make_unique<Class>("Object", Method("construct", cdot::lib::obj::construct),
        AccessModifier::PUBLIC);

    declare_class(std::move(Object));

    Class::UniquePtr Integer = cdot::lib::intgr::init();
    declare_class(std::move(Integer));
}

bool GlobalContext::is_declared_class(std::string class_name) {
    return classes.find(class_name) != classes.end();
}

void GlobalContext::declare_class(Class::UniquePtr class_) {
    classes.insert(std::pair<std::string, Class::UniquePtr>(class_->class_name(), std::move(class_)));
}

Class* GlobalContext::get_class(std::string class_name) {
    if (!is_declared_class(class_name)) {
        RuntimeError::raise(ERR_UNDECLARED_VARIABLE, "Class " + class_name + " does not exist in current context");
    }

    return classes[class_name].get();
}