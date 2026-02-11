#pragma once

#ifdef TEST

#include "../TEST.hpp"
#include "../str_replace.hpp"

TEST(test_str_replace_single_replacement) {
    string from = "foo";
    string to = "bar";
    string subject = "foo baz";
    string expected = "bar baz";
    string actual = str_replace(from, to, subject);
    assert(actual == expected && "Single replacement");
}

TEST(test_str_replace_multiple_replacements) {
    map<string, string> replacements = {
        {"foo", "bar"},
        {"baz", "qux"}
    };
    string subject = "foo baz foo";
    string expected = "bar qux bar";
    string actual = str_replace(replacements, subject);
    assert(actual == expected && "Multiple replacements");
}

TEST(test_str_replace_empty_string) {
    map<string, string> replacements = {{"foo", "bar"}};
    string subject = "";
    string expected = "";
    string actual = str_replace(replacements, subject);
    assert(actual == expected && "Empty string");
}

TEST(test_str_replace_no_match) {
    map<string, string> replacements = {{"foo", "bar"}};
    string subject = "baz qux";
    string expected = "baz qux";
    string actual = str_replace(replacements, subject);
    assert(actual == expected && "No match");
}

TEST(test_str_replace_empty_from) {
    map<string, string> replacements = {{"", "bar"}};
    string subject = "foo";
    bool threw = false;
    try {
        str_replace(replacements, subject);
    } catch (exception& e) {
        threw = true;
    }
    assert(threw && "Empty from should throw");
}

TEST(test_str_replace_empty_to) {
    map<string, string> replacements = {{"foo", ""}};
    string subject = "foo baz";
    string expected = " baz";
    string actual = str_replace(replacements, subject);
    assert(actual == expected && "Empty to");
}

TEST(test_str_replace_overlapping) {
    map<string, string> replacements = {{"aa", "a"}};
    string subject = "aaaa";
    string expected = "aa";
    string actual = str_replace(replacements, subject);
    assert(actual == expected && "Overlapping replacements");
}

#endif
