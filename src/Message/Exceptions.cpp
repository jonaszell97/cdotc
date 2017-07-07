//
// Created by Jonas Zell on 17.06.17.
//

#include <iostream>
#include "Exceptions.h"
#include "../StdLib/GlobalContext.h"
#include "../Tokenizer.h"

std::map<ParseErrors, std::string> _parse_errors = {
        {ERR_UNEXPECTED_TOKEN, "Unexpected Token"},
        {ERR_UNEXPECTED_CHARACTER, "Unexpected Character"},
        {ERR_EOF, "Unexpected EOF"},
        {ERR_UNINITIALIZED_VAR, "Uninitialized Variable"}
};

std::map<RuntimeErrors, std::string> _runtime_errors = {
        {ERR_CONTEXT_ERROR, "Context Error"},
         {ERR_UNDECLARED_VARIABLE, "Undeclared Variable"},
        {ERR_BAD_CAST, "Bad Cast"},
        {ERR_BAD_ACCESS, "Bad Access"},
        {ERR_OP_UNDEFINED, "Undefined Operation"},
        {ERR_REDECLARED_VAR, "Variable Redeclaration"},
        {ERR_WRONG_NUM_ARGS, "Incompatible argument count"},
        {ERR_TYPE_ERROR, "Type Error"},
        {ERR_PRIVATE_PROPERTY, "Private Property"},
        {ERR_NULL_POINTER_EXC, "Null Pointer Exception"}
};

ParseError::ParseError(const std::string& message) : message_(message) {

}

void ParseError::raise(ParseErrors error, std::string msg, Tokenizer* tokenizer) {
    std::string err = "\033[21;31m" + _parse_errors[error] + ": " + msg;
    if (tokenizer != nullptr) {
        std::string program = tokenizer->_program;
        // get line number
        int err_index = tokenizer->current_token.get_start();
        int err_end = tokenizer->current_token.get_end();
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

        std::string str_line = program.substr(start, end - start);
        while (str_line[0] == ' ') {
            start++;
            str_line = str_line.substr(1);
        }
        while (str_line[str_line.length() - 1] == ' ') {
            end--;
            str_line = str_line.substr(0, str_line.length() - 1);
        }

        std::string details = str_line + "\n";
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

    throw ParseError(err + "\033[0m");
}

RuntimeError::RuntimeError(const std::string& message) : message_(message) {

}

void RuntimeError::raise(RuntimeErrors error, std::string msg, AstNode* cause) {
    std::string err = "\033[21;31m" + _runtime_errors[error] + ": " + msg;
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
        const int max_length = int(details.length());
        bool non_ws = false;
        for (int i = start; i < err_index; ++i) {
            non_ws = non_ws || details[i - start] != ' ';
            if (non_ws) {
                details += "~";
            } else {
                details += " ";
            }
        }
        for (int j = err_index; j < std::min(err_end, err_index + max_length); ++j) {
            details += "^";
        }
        for (int k = err_end; k < max_length; ++k) {
            details += "~";
        }

        err += details;
    }

    throw RuntimeError(err + "\033[0m");
}