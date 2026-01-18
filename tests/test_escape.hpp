#pragma once

#ifdef TEST

#include "../TEST.hpp"
#include "../escape.hpp"

TEST(test_escape_empty_input) {
    string input = "";
    string expected = "";
    string actual = escape(input);
    assert(actual == expected && "Empty input");
}

TEST(test_escape_no_special_chars) {
    string input = "hello world";
    string expected = "hello world";
    string actual = escape(input);
    assert(actual == expected && "No special chars");
}

TEST(test_escape_single_char) {
    string input = "$";
    string expected = "\\$";
    string actual = escape(input);
    assert(actual == expected && "Single char");
}

TEST(test_escape_already_escaped) {
    string input = "\\\\$";
    string expected = "\\\\\\\\\\$";
    string actual = escape(input);
    assert(actual == expected && "Already escaped characters");
}

TEST(test_escape_no_chars) {
    string input = "hello";
    string expected = "hello";
    string actual = escape(input);
    assert(actual == expected && "No characters to escape");
}

TEST(test_escape_custom_chars) {
    string input = "a%b&c";
    string expected = "a\\%b\\&c";
    string actual = escape(input, "%&");
    assert(actual == expected && "Custom characters to escape");
}

TEST(test_escape_mixed_content) {
    string input = "hello $world\\ \" \' `";
    string expected = "hello \\$world\\\\ \\\" \\' \\`";
    string actual = escape(input);
    assert(actual == expected && "Mixed content");
}

TEST(test_escape_custom_chars2) {
    string input = "abc123";
    string chars = "123";
    string expected = "abc\\1\\2\\3";
    string actual = escape(input, chars);
    assert(actual == expected && "Custom chars 2");
}

TEST(test_escape_custom_escape_sequence) {
    string input = "abc$";
    string chars = "$";
    string esc = "/";
    string expected = "abc/$";
    string actual = escape(input, chars, esc);
    assert(actual == expected && "Custom escape sequence");
}

TEST(test_escape_already_escaped_in_input) {
    string input = "\\\\$"; // Escaped backslash and $
    string expected = "\\\\\\\\\\$"; // Expect: \\\$
    string actual = escape(input);
    assert(actual == expected && "Already escaped in input");
}

TEST(test_escape_multiple_chars) {
    string input = "$\\\"'`";
    string expected = "\\$\\\\\\\"\\'\\`";
    string actual = escape(input);
    assert(actual == expected && "Multiple chars");
}

#endif
