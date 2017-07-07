//
// Created by Jonas Zell on 01.07.17.
//

#include "catch/catch.h"
#include "../src/Tokenizer.h"

TEST_CASE("Variable assignment") {
    Tokenizer t("let x = 3");
    t.advance();
    REQUIRE(t.current_token.get_type() == T_TYPE);
    REQUIRE(t.s_val() == "let");
    t.advance();
    REQUIRE(t.current_token.get_type() == T_IDENT);
    REQUIRE(t.s_val() == "x");
    t.advance();
    REQUIRE(t.current_token.is_operator("="));
    t.advance();
    REQUIRE(t.current_token.get_type() == T_LITERAL);
    REQUIRE(t.current_token.get_value().get<int>() == 3);
    t.advance();
    REQUIRE(t.current_token.get_type() == T_EOF);
}

TEST_CASE("Function declaration") {
    Tokenizer t(
        "def fun(int x) {"
            "return x + 3"
        "}"
    );
    t.advance();

    REQUIRE(t.current_token.is_keyword("def")); t.advance();

    REQUIRE(t.current_token.get_type() == T_IDENT);
    REQUIRE(t.s_val() == "fun"); t.advance();

    REQUIRE(t.current_token.is_punctuator('(')); t.advance();

    REQUIRE(t.current_token.get_type() == T_TYPE);
    REQUIRE(t.s_val() == "int"); t.advance();

    REQUIRE(t.current_token.get_type() == T_IDENT);
    REQUIRE(t.s_val() == "x"); t.advance();

    REQUIRE(t.current_token.is_punctuator(')')); t.advance();
    REQUIRE(t.current_token.is_punctuator('{')); t.advance();

    REQUIRE(t.current_token.is_keyword("return")); t.advance();

    REQUIRE(t.current_token.get_type() == T_IDENT);
    REQUIRE(t.s_val() == "x"); t.advance();

    REQUIRE(t.current_token.is_operator("+")); t.advance();

    REQUIRE(t.current_token.get_type() == T_LITERAL);
    REQUIRE(t.current_token.get_value().get<int>() == 3); t.advance();

    REQUIRE(t.current_token.is_punctuator('}')); t.advance();

    REQUIRE(t.current_token.get_type() == T_EOF);
}

TEST_CASE("Numeric literals") {
    Tokenizer t(
        "3 3L 3l 0xabcA3 0721 0b10100 1.1e-3 3E2 1.1 1.13131f 1.13131F"
    );
    t.advance();

    REQUIRE(t.current_token.get_type() == T_LITERAL);
    REQUIRE(t.current_token.get_value().get<int>() == 3); t.advance();

    REQUIRE(t.current_token.get_type() == T_LITERAL);
    REQUIRE(t.current_token.get_value().get<long>() == 3l); t.advance();

    REQUIRE(t.current_token.get_type() == T_LITERAL);
    REQUIRE(t.current_token.get_value().get<long>() == 3L); t.advance();

    REQUIRE(t.current_token.get_type() == T_LITERAL);
    REQUIRE(t.current_token.get_value().get<int>() == 0xabca3); t.advance();

    REQUIRE(t.current_token.get_type() == T_LITERAL);
    REQUIRE(t.current_token.get_value().get<int>() == 0721); t.advance();

    REQUIRE(t.current_token.get_type() == T_LITERAL);
    REQUIRE(t.current_token.get_value().get<int>() == 0b10100); t.advance();

    REQUIRE(t.current_token.get_type() == T_LITERAL);
    REQUIRE(t.current_token.get_value().get<double>() == 1.1e-3); t.advance();

    REQUIRE(t.current_token.get_type() == T_LITERAL);
    REQUIRE(t.current_token.get_value().get<double>() == 3e2); t.advance();

    REQUIRE(t.current_token.get_type() == T_LITERAL);
    REQUIRE(t.current_token.get_value().get<double>() == 1.1); t.advance();

    REQUIRE(t.current_token.get_type() == T_LITERAL);
    REQUIRE(t.current_token.get_value().get<float>() == 1.13131f); t.advance();

    REQUIRE(t.current_token.get_type() == T_LITERAL);
    REQUIRE(t.current_token.get_value().get<float>() == 1.13131F); t.advance();

    REQUIRE(t.current_token.get_type() == T_EOF);
}

TEST_CASE("String literals") {
    Tokenizer t(
        "\"hello there, good sir\" "
    );
    t.advance();

    REQUIRE(t.current_token.get_type() == T_LITERAL);
    REQUIRE(t.s_val() == "hello there, good sir"); t.advance();

    REQUIRE(t.current_token.get_type() == T_EOF);
}

TEST_CASE("Char literals") {
    Tokenizer t(
        "'c' '0' '+' '\n' 'xxx'"
    );
    t.advance();

    REQUIRE(t.current_token.get_type() == T_LITERAL);
    REQUIRE(t.c_val() == 'c'); t.advance();

    REQUIRE(t.current_token.get_type() == T_LITERAL);
    REQUIRE(t.c_val() == '0'); t.advance();

    REQUIRE(t.current_token.get_type() == T_LITERAL);
    REQUIRE(t.c_val() == '+'); t.advance();

    REQUIRE(t.current_token.get_type() == T_LITERAL);
    REQUIRE(t.c_val() == '\n');

    REQUIRE_THROWS(t.advance());
}

TEST_CASE("Bool literals") {
    Tokenizer t(
        "true false"
    );
    t.advance();

    REQUIRE(t.current_token.get_type() == T_LITERAL);
    REQUIRE(t.b_val()); t.advance();

    REQUIRE(t.current_token.get_type() == T_LITERAL);
    REQUIRE(!t.b_val()); t.advance();

    REQUIRE(t.current_token.get_type() == T_EOF);
}

TEST_CASE("Null literal") {
    Tokenizer t(
        "null"
    );
    t.advance();

    REQUIRE(t.current_token.get_type() == T_LITERAL);
    REQUIRE(t.current_token.get_value().get_type() == VOID_T); t.advance();

    REQUIRE(t.current_token.get_type() == T_EOF);
}

TEST_CASE("Operators") {
    Tokenizer t(
        "!! &&& +++ - +!+ ^|new**??:typeof?"
    );
    t.advance();

    REQUIRE(t.current_token.is_operator("!")); t.advance();
    REQUIRE(t.current_token.is_operator("!")); t.advance();
    REQUIRE(t.current_token.is_operator("&&")); t.advance();
    REQUIRE(t.current_token.is_operator("&")); t.advance();
    REQUIRE(t.current_token.is_operator("++")); t.advance();
    REQUIRE(t.current_token.is_operator("+")); t.advance();
    REQUIRE(t.current_token.is_operator("-")); t.advance();
    REQUIRE(t.current_token.is_operator("+")); t.advance();
    REQUIRE(t.current_token.is_operator("!")); t.advance();
    REQUIRE(t.current_token.is_operator("+")); t.advance();
    REQUIRE(t.current_token.is_operator("^")); t.advance();
    REQUIRE(t.current_token.is_operator("|")); t.advance();
    REQUIRE(t.current_token.is_operator("new")); t.advance();
    REQUIRE(t.current_token.is_operator("**")); t.advance();
    REQUIRE(t.current_token.is_operator("??")); t.advance();
    REQUIRE(t.current_token.is_operator(":")); t.advance();
    REQUIRE(t.current_token.is_operator("typeof")); t.advance();
    REQUIRE(t.current_token.is_operator("?")); t.advance();

    REQUIRE(t.current_token.get_type() == T_EOF);
}

TEST_CASE("Punctuators") {
    Tokenizer t(
            ",;(} [."
    );
    t.advance();

    REQUIRE(t.current_token.is_punctuator(',')); t.advance();
    REQUIRE(t.current_token.is_punctuator(';')); t.advance();
    REQUIRE(t.current_token.is_punctuator('(')); t.advance();
    REQUIRE(t.current_token.is_punctuator('}')); t.advance();
    REQUIRE(t.current_token.is_punctuator('[')); t.advance();
    REQUIRE(t.current_token.is_punctuator('.')); t.advance();

    REQUIRE(t.current_token.get_type() == T_EOF);
}