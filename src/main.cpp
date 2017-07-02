#include <iostream>
#include "Parser.h"
#include <fstream>
#include <streambuf>

std::string cin_readline() {
    std::string res;
    std::getline(std::cin, res);

    return res;
}

int main() {
    std::ifstream t("../src/src.dot");
    std::string str((std::istreambuf_iterator<char>(t)),
                    std::istreambuf_iterator<char>());
    Parser x(str);
    x.run(true);

    return 0;
}