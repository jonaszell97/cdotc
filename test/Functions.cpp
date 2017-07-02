//
// Created by Jonas Zell on 01.07.17.
//

#include <iostream>
#include "catch.h"
#include "../src/Parser.h"

TEST_CASE("Recursion test") {
    Parser p(
        "def fib(int n) -> int { "
            "if (n < 0) return 0 "
            "return n == 1 ? 1 : fib(n-1) + fib(n-2) "
        "}"
        "outln fib(17)"
    );

    std::ostringstream oss;
    std::streambuf* p_cout_streambuf = std::cout.rdbuf();
    std::cout.rdbuf(oss.rdbuf());

    p.run(false);

    std::cout.rdbuf(p_cout_streambuf);

    REQUIRE(oss);
    REQUIRE(oss.str() == "1597\n");
}

TEST_CASE("Variable capture") {
    Parser p(
        "def outer() -> object { "
            "let x = 3"
            "def inner() { "
                "outln x"
            "} "
            "return inner "
        "} "

        "outer()()"
    );
    Parser p2(
        "def outer() -> object { "
            "let x = 3"
            "def inner() { "
                "outln x"
            "} "
            "return inner "
        "} "
        "outln x"
    );

    std::ostringstream oss;
    std::streambuf* p_cout_streambuf = std::cout.rdbuf();
    std::cout.rdbuf(oss.rdbuf());

    p.run(false);

    std::cout.rdbuf(p_cout_streambuf);

    REQUIRE(oss);
    REQUIRE(oss.str() == "3\n");

    REQUIRE_THROWS(p2.run(false));
}