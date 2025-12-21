#pragma once

#include "../TEST.hpp"
#include "../str_starts_with.hpp"

#ifdef TEST

TEST(test_str_ends_with_basic_match) {
    assert(str_ends_with("hello world", "world") && "Basic match failed");
    assert(str_ends_with("test", "est") && "Suffix match failed");
    assert(str_ends_with("abc", "c") && "Single char suffix match failed");
}

TEST(test_str_ends_with_full_match) {
    assert(str_ends_with("abc", "abc") && "Full string match failed");
}

TEST(test_str_ends_with_no_match) {
    assert(!str_ends_with("abc", "abd") && "Should not match different suffix");
    assert(!str_ends_with("test", "fail") && "Should not match completely different suffix");
}

TEST(test_str_ends_with_prefix_longer_than_string) {
    assert(!str_ends_with("abc", "abcd") && "Suffix longer than string should fail");
}

TEST(test_str_ends_with_empty_string) {
    assert(str_ends_with("abc", "") && "Empty suffix should always match");
    assert(!str_ends_with("", "a") && "Non-empty suffix on empty string should fail");
    assert(str_ends_with("", "") && "Empty suffix on empty string should pass");
}

TEST(test_str_ends_with_case_sensitivity) {
    assert(!str_ends_with("Hello", "hello") && "String comparison is case-sensitive");
    assert(str_ends_with("Hello", "ello") && "Case sensitive match succeeded");
}

TEST(test_str_ends_with_complex_suffix) {
    assert(str_ends_with("file.txt", ".txt") && "Should match .txt extension");
    assert(!str_ends_with("file.txt", "txt.") && "Should not match reversed suffix");
}

#endif
