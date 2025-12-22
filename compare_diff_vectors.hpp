#pragma once

#include <vector>
#include "str_diff_t.hpp"

using namespace std;

// Helper to compare str_diff_t vectors (if vector_equal doesn't work directly)
bool compare_diff_vectors(const vector<str_diff_t>& actual, const vector<str_diff_t>& expected) {
    if (actual.size() != expected.size()) return false;
    for (size_t i = 0; i < actual.size(); ++i) {
        if (actual[i].bound[0] != expected[i].bound[0] ||
            actual[i].bound[1] != expected[i].bound[1] ||
            actual[i].added != expected[i].added ||       // Assumes vector<string> comparison works
            actual[i].removed != expected[i].removed) {   // Assumes vector<string> comparison works
            return false;
        }
    }
    return true;
}
