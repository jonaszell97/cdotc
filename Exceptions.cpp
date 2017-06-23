//
// Created by Jonas Zell on 17.06.17.
//

#include "Exceptions.h"

std::map<ParseErrors, std::string> _parse_errors = {
        {ERR_UNEXPECTED_TOKEN, "Unexpected Token"},
        {ERR_UNEXPECTED_CHARACTER, "Unexpected Character"},
        {ERR_EOF, "Unexpected EOF"}
};

std::map<RuntimeErrors, std::string> _runtime_errors = {
        {ERR_MISSING_CONTEXT, "Missing Context"},
         {ERR_UNDECLARED_VARIABLE, "Undeclared Variable"},
        {ERR_BAD_CAST, "Bad Cast"},
        {ERR_BAD_ACCESS, "Bad Access"},
        {ERR_OP_UNDEFINED, "Undefined Operation"},
        {ERR_REDECLARED_VAR, "Variable Redeclaration"},
        {ERR_WRONG_NUM_ARGS, "Incompatible argument count"},
        {ERR_TYPE_ERROR, "Type Error"}
};

ParseError::ParseError(const std::string& message) : message_(message) {

}

void ParseError::raise(ParseErrors error, std::string msg) {
    throw ParseError(_parse_errors[error] + ": " + msg);
}

RuntimeError::RuntimeError(const std::string& message) : message_(message) {

}

void RuntimeError::raise(RuntimeErrors error, std::string msg) {
    throw RuntimeError(_runtime_errors[error] + ": " + msg);
}