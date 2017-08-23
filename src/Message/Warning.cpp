//
// Created by Jonas Zell on 05.07.17.
//

#include <iostream>
#include "Warning.h"
#include "../AST/AstNode.h"

void Warning::issue(std::string msg, AstNode* cause, WarningLevel severity) {
    std::string err = "\033[33mWarning";
    if (severity == WarningLevel::SEVERE) {
        err += " (Severe):";
    }
    else {
        err += ": ";
    }

    err += msg;

    if (cause != nullptr) {
        std::string program = cause->getSourceFile();
        // get line number
        int err_index = cause->getStartIndex();
        int err_end = cause->getEndIndex();
        int lines = 1;
        int last_newline = 0;
        for (int l = 0; l < err_index; ++l) {
            if (program[l] == '\n') {
                lines++;
                last_newline = l;
            }
        }

        err += " (" + std::to_string(lines) + ":" + std::to_string(err_index - last_newline) + ")\n";

        int start = err_index;
        while(program[start] != '\n' && start > 0) {
            --start;
        }

        ++start;

        int end = err_index;
        while(program[end] != '\n' && end < program.length()) {
            ++end;
        }

        std::string line_num = std::to_string(lines) + " | ";
        std::string details = line_num + program.substr(start, end - start) + "\n";
        while (details[err_index - start + line_num.length()] == ' ') {
            --start;
        }

        for (int i = 0; i < err_index - start + line_num.length(); ++i) {
            details += " ";
        }
        details += "^";

        err += details;
    }

    std::cout << err << "\033[0m" << std::endl;
}