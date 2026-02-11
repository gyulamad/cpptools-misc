#pragma once

#ifdef TEST

#include "../TEST.hpp"
#include "../str_show_diff.hpp"

TEST(test_str_show_diff_empty) {
    str_diff_t diff;
    diff.bound[0] = 0;
    diff.bound[1] = 0;
    diff.added = {};
    diff.removed = {};
    string result = str_show_diff(diff, false);
    // Just verify it doesn't crash
    (void)result;
}

TEST(test_str_show_diff_with_added) {
    str_diff_t diff;
    diff.bound[0] = 1;
    diff.bound[1] = 1;
    diff.added = {"+added"};
    diff.removed = {};
    string result = str_show_diff(diff, false);
    assert(result.size() > 0 && "Diff with added");
}

TEST(test_str_show_diff_with_removed) {
    str_diff_t diff;
    diff.bound[0] = 1;
    diff.bound[1] = 1;
    diff.added = {};
    diff.removed = {"-removed"};
    string result = str_show_diff(diff, false);
    assert(result.size() > 0 && "Diff with removed");
}

#endif