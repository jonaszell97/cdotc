//
// Created by Jonas Zell on 17.06.17.
//

#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>
#include <string>
#include <map>

enum ParseErrors {
    ERR_UNEXPECTED_TOKEN,
    ERR_UNEXPECTED_CHARACTER,
    ERR_UNINITIALIZED_VAR,
    ERR_EOF
};

enum RuntimeErrors {
    ERR_CONTEXT_ERROR,
    ERR_UNDECLARED_VARIABLE,
    ERR_BAD_CAST,
    ERR_BAD_ACCESS,
    ERR_OP_UNDEFINED,
    ERR_REDECLARED_VAR,
    ERR_WRONG_NUM_ARGS,
    ERR_TYPE_ERROR,
    ERR_VAL_TOO_LARGE,
    ERR_PRIVATE_PROPERTY
};

extern std::map<ParseErrors, std::string> _parse_errors;
extern std::map<RuntimeErrors, std::string> _runtime_errors;

class ParseError : public std::exception {
private:
    explicit ParseError(const std::string& message);
    std::string message_;

public:
    static void raise(ParseErrors , std::string);
    virtual const char* what() const throw() {
        return message_.c_str();
    }
};

class RuntimeError : public std::exception {
private:
    explicit RuntimeError(const std::string& message);
    std::string message_;

public:
    static void raise(RuntimeErrors , std::string);
    virtual const char* what() const throw() {
        return message_.c_str();
    }
};
#endif //EXCEPTIONS_H
