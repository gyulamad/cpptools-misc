#pragma once

#ifdef TEST

#include "../TEST.hpp"
#include "../str_diff_t.hpp"

TEST(test_str_diff_t_default_constructor) {
    str_diff_t diff = {};
    assert(diff.bound[0] == 0 && "Default bound[0]");
    assert(diff.bound[1] == 0 && "Default bound[1]");
}

TEST(test_str_diff_t_with_values) {
    vector<string> added = {"line1", "line2"};
    vector<string> removed = {"line3", "line4"};
    str_diff_t diff = {};
    diff.bound[0] = 10;
    diff.bound[1] = 12;
    diff.added = added;
    diff.removed = removed;
    assert(diff.bound[0] == 10 && "Bound[0]");
    assert(diff.bound[1] == 12 && "Bound[1]");
    assert(diff.added.size() == 2 && "Added size");
    assert(diff.removed.size() == 2 && "Removed size");
}

#endif