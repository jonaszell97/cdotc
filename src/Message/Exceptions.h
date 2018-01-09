//
// Created by Jonas Zell on 17.06.17.
//

#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>
#include <string>
#include <map>

using std::string;

namespace cdot {
namespace err {
string prepareLine(string& src, string& fileName, int errIndex, int length);
}

namespace ast {
class AstNode;
}

namespace lex {
struct LexerTraits;

template<class Traits>
class Lexer;
}

}

class ParseError : public std::exception {
private:
    explicit ParseError(const string& message);
    string message_;

public:
    static void raise(string, cdot::lex::Lexer<cdot::lex::LexerTraits> *);
    virtual const char* what() const throw() {
        return message_.c_str();
    }
};

class RuntimeError : public std::exception {
private:
    string message_;

public:
    explicit RuntimeError(const string& message);
    static void raise(string, cdot::ast::AstNode *);
    virtual const char* what() const throw() {
        return message_.c_str();
    }
};
#endif //EXCEPTIONS_H
