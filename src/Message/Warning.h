//
// Created by Jonas Zell on 05.07.17.
//

#ifndef CDOT_WARNING_H
#define CDOT_WARNING_H

#include <string>
#include "../AST/AstNode.h"

enum class WarningLevel {
    SEVERE,
    NOT_SEVERE
};

class Warning {
public:
    static void issue(std::string, AstNode* = nullptr, WarningLevel = WarningLevel::NOT_SEVERE);
};


#endif //CDOT_WARNING_H
