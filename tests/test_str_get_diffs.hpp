#pragma once

#ifdef TEST

#include "../TEST.hpp"
#include "../str_get_diffs.hpp"

TEST(test_str_get_diffs_no_diff) {
    string s1 = "hello\nworld";
    string s2 = "hello\nworld";
    vector<str_diff_t> diffs = str_get_diffs(s1, s2);
    assert(diffs.size() == 0 && "No differences");
}

TEST(test_str_get_diffs_single_line_diff) {
    string s1 = "hello\nworld";
    string s2 = "hello\nworld2";
    vector<str_diff_t> diffs = str_get_diffs(s1, s2);
    assert(diffs.size() == 1 && "Single difference");
}

TEST(test_str_get_diffs_multiple_lines_diff) {
    string s1 = "line1\nline2\nline3";
    string s2 = "line1\nmodified\nline3";
    vector<str_diff_t> diffs = str_get_diffs(s1, s2);
    assert(diffs.size() == 1 && "Multiple lines difference");
}

#endif
