//
// Created by Jonas Zell on 05.07.17.
//

#include <iostream>
#include "Warning.h"

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
        std::string program = GlobalContext::program;
        // get line number
        int err_index = cause->get_start();
        int err_end = cause->get_end();
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

        std::string details = program.substr(start, end - start) + "\n";
        for (int i = start; i < err_index; ++i) {
            details += "~";
        }
        for (int j = err_index; j < err_end; ++j) {
            details += "^";
        }
        for (int k = err_end; k < end; ++k) {
            details += "~";
        }

        err += details;
    }

    std::cout << err << "\033[0m" << std::endl;
}