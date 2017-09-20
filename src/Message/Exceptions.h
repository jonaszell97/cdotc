//
// Created by Jonas Zell on 17.06.17.
//

#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>
#include <string>
#include <map>

class Token;
class AstNode;

class Lexer;
using std::string;

namespace cdot {
   namespace err {
      string prepareLine(string& src, string& fileName, int errIndex);
   }
}

class ParseError : public std::exception {
private:
    explicit ParseError(const string& message);
    string message_;

public:
    static void raise(string, Lexer *);
    virtual const char* what() const throw() {
        return message_.c_str();
    }
};

class RuntimeError : public std::exception {
private:
    explicit RuntimeError(const string& message);
    string message_;

public:
    static void raise(string, AstNode *);
    virtual const char* what() const throw() {
        return message_.c_str();
    }
};
#endif //EXCEPTIONS_H
