//
// Created by Jonas Zell on 01.07.17.
//

#include <iostream>
#include "../catch.h"
#include "../../src/Parser.h"

TEST_CASE("If statement") {
    Parser p(
        "if (true) {"
            "outln \"true!\""
        "}"
    );
    Parser p2(
        "let b = 0"
        "if ((bool)b) {"
            "outln \"false!\""
        "}"
    );

    std::ostringstream oss;
    std::streambuf* p_cout_streambuf = std::cout.rdbuf();
    std::cout.rdbuf(oss.rdbuf());

    p.run(false);
    p2.run(false);

    std::cout.rdbuf(p_cout_streambuf);

    REQUIRE(oss);
    REQUIRE(oss.str() == "true!\n");
}

TEST_CASE("If/else statement") {
    Parser p(
        "if (1 + 1 == 3) {"
            "outln \"if!\""
        "} else {"
            "outln \"else!\""
        "}"
    );

    std::ostringstream oss;
    std::streambuf* p_cout_streambuf = std::cout.rdbuf();
    std::cout.rdbuf(oss.rdbuf());

    p.run(false);

    std::cout.rdbuf(p_cout_streambuf);

    REQUIRE(oss);
    REQUIRE(oss.str() == "else!\n");
}

TEST_CASE("If/else if/else statement") {
    Parser p(
        "if (1 + 1 == 3) {"
            "outln \"if!\""
        "} else if (1 + 1 == 2) {"
            "outln \"else if!\""
        "} else {"
        "}"
    );

    std::ostringstream oss;
    std::streambuf* p_cout_streambuf = std::cout.rdbuf();
    std::cout.rdbuf(oss.rdbuf());

    p.run(false);

    std::cout.rdbuf(p_cout_streambuf);

    REQUIRE(oss);
    REQUIRE(oss.str() == "else if!\n");
}

TEST_CASE("Switch statement") {
    Parser p(
        "let x = 3"
        "switch (3 + x) {"
            "case 5:"
            "case 6:"
                "outln 6"
            "case 8:"
                "outln 8"
                "break"
            "default:"
                "break"
        "}"
    );

    std::ostringstream oss;
    std::streambuf* p_cout_streambuf = std::cout.rdbuf();
    std::cout.rdbuf(oss.rdbuf());

    p.run(false);

    std::cout.rdbuf(p_cout_streambuf);

    REQUIRE(oss);
    REQUIRE(oss.str() == "6\n8\n");
}

TEST_CASE("Switch default statement") {
    Parser p(
        "switch ('c') { "
            "case 1: break "
            "case 'd': break "
            "default: outln 12 break "
        "}"
    );

    std::ostringstream oss;
    std::streambuf* p_cout_streambuf = std::cout.rdbuf();
    std::cout.rdbuf(oss.rdbuf());

    p.run(false);

    std::cout.rdbuf(p_cout_streambuf);

    REQUIRE(oss);
    REQUIRE(oss.str() == "12\n");
}

TEST_CASE("For loop") {
    Parser p(
        "for (int i = 0; i < 5; i++) { "
            "if (i == 3) continue "
            "outln i "
        "}"
    );

    std::ostringstream oss;
    std::streambuf* p_cout_streambuf = std::cout.rdbuf();
    std::cout.rdbuf(oss.rdbuf());

    p.run(false);

    std::cout.rdbuf(p_cout_streambuf);

    REQUIRE(oss);
    REQUIRE(oss.str() == "0\n1\n2\n4\n");
}

TEST_CASE("For in loop") {
    Parser p(
        "for(i in [1, 71, 15]) { "
            "outln i "
        "}"
    );
    Parser p2(
        "for(i in 5..3) { "
            "outln i "
        "}"
    );

    std::ostringstream oss;
    std::streambuf* p_cout_streambuf = std::cout.rdbuf();
    std::cout.rdbuf(oss.rdbuf());

    p.run(false);
    p2.run(false);

    std::cout.rdbuf(p_cout_streambuf);

    REQUIRE(oss);
    REQUIRE(oss.str() == "1\n71\n15\n5\n4\n3\n");
}

TEST_CASE("while statement") {
    Parser p(
        "let x = 0 "
        "while (x++ < 5) {"
            "outln x "
        "} "
    );

    std::ostringstream oss;
    std::streambuf* p_cout_streambuf = std::cout.rdbuf();
    std::cout.rdbuf(oss.rdbuf());

    p.run(false);

    std::cout.rdbuf(p_cout_streambuf);

    REQUIRE(oss);
    REQUIRE(oss.str() == "1\n2\n3\n4\n5\n");
}