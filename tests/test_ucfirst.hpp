#pragma once

#ifdef TEST

#include "../TEST.hpp"
#include "../ucfirst.hpp"

// Test cases for ucfirst function
TEST(test_ucfirst_empty_string) {
    string input = "";
    string result = ucfirst(input);
    assert(result.empty() && "ucfirst of empty string should be empty");
}

TEST(test_ucfirst_single_char_lowercase) {
    string input = "a";
    string result = ucfirst(input);
    assert(result == "A" && "ucfirst('a') should return 'A'");
}

TEST(test_ucfirst_single_char_uppercase) {
    string input = "A";
    string result = ucfirst(input);
    assert(result == "A" && "ucfirst('A') should remain 'A'");
}

TEST(test_ucfirst_lowercase_word) {
    string input = "hello";
    string result = ucfirst(input);
    assert(result == "Hello" && "ucfirst('hello') should return 'Hello'");
}

TEST(test_ucfirst_already_capitalized) {
    string input = "Hello";
    string result = ucfirst(input);
    assert(result == "Hello" && "ucfirst('Hello') should remain 'Hello'");
}

TEST(test_ucfirst_with_numbers) {
    string input = "123abc";
    string result = ucfirst(input);
    assert(result == "123abc" && "ucfirst('123abc') should remain unchanged (digit not affected by toupper)");
}

TEST(test_ucfirst_mixed_case_word) {
    string input = "hELLO WORLD";
    string result = ucfirst(input);
    assert(result == "HELLO WORLD" && "ucfirst only affects first character");
}

#endif // TEST
