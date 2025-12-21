#pragma once

#include "../TEST.hpp"
#include "../fix_path.hpp"

#ifdef TEST

TEST(test_fix_path_basic) {
    string actual = fix_path("some/path/./file.txt");
    assert(actual == "some/path/file.txt" && "Basic normalization");
}

TEST(test_fix_path_backslashes) {
    string actual = fix_path("windows\\path\\to\\file.txt");
    assert(actual == "windows/path/to/file.txt" && "Backslashes should be converted to forward slashes");
}

TEST(test_fix_path_mixed_slashes) {
    string actual = fix_path("mixed/path\\to/../file.txt");
    assert(actual == "mixed/file.txt" && "Mixed slashes and parent dir should be handled");
}

TEST(test_fix_path_trailing_slash) {
    string actual = fix_path("some/directory/");
    assert(actual == "some/directory" && "Trailing slash should be removed");
}

TEST(test_fix_path_multiple_trailing_slashes) {
    // Note: fs::path normalization might handle this implicitly
    string actual = fix_path("some/directory///");
    assert(actual == "some/directory" && "Multiple trailing slashes should be removed");
}

TEST(test_fix_path_root_with_trailing_slash) {
    string actual = fix_path("/");
    assert(actual == "/" && "Root path '/' should remain unchanged");
}

TEST(test_fix_path_empty_string) {
    string actual = fix_path("");
    assert(actual == "" && "Empty string should remain empty");
}

TEST(test_fix_path_dot) {
    string actual = fix_path(".");
    assert(actual == "." && "'.' should remain '.'");
}

TEST(test_fix_path_double_dot) {
    string actual = fix_path("..");
    assert(actual == ".." && "'..' should remain '..'");
}

TEST(test_fix_path_complex_dots) {
    string actual = fix_path("/a/b/../../c/./d");
    assert(actual == "/c/d" && "Complex dot navigation should be normalized");
}

#endif
