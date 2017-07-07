#include <iostream>
#include "Parser.h"
#include <fstream>
#include <streambuf>
#include <codecvt>

std::string cin_readline() {
    std::string res;
    std::getline(std::cin, res);

    return res;
}

int main() {
    std::ifstream t("/Users/Jonas/CDotProjects/ex/main.dot");
    std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
    try {
        Parser x(str);
        x.run(true);
    } catch(ParseError& p) {
        std::cout << p.what() << std::endl;
    } catch(RuntimeError& r) {
        std::cout << r.what() << std::endl;
    }

    return 0;
}