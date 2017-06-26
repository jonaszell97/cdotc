//
// Created by Jonas Zell on 25.06.17.
//

#ifndef CDOT_GLOBALCONTEXT_H
#define CDOT_GLOBALCONTEXT_H


#include <string>
#include "Class.h"

class GlobalContext {
public:
    static void init();
    static bool is_declared_class(std::string);
    static Class* get_class(std::string);
    static void declare_class(std::unique_ptr<Class>);

    typedef std::shared_ptr<GlobalContext> SharedPtr;

private:
    GlobalContext();
    static bool initialized;
    static std::unordered_map<std::string, std::unique_ptr<Class>> classes;
};


#endif //CDOT_GLOBALCONTEXT_H
