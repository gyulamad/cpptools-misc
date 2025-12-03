#pragma once

#include <string>
#include <vector>

#include "explode.hpp"
#include "implode.hpp"

using namespace std;

string remove_extension(const string& fname) {
    vector<string> splits_path = explode("/", fname);
    vector<string> splits_last = explode(".", splits_path[splits_path.size() - 1]);
    if (splits_last.size() > 1) splits_last.pop_back(); // remove last element but only if it had extension
    splits_path[splits_path.size() - 1] = implode(".", splits_last);
    return implode("/", splits_path);
}

#ifdef TEST
#include "str_contains.hpp"

// Tests for remove_extension
TEST(test_remove_extension_single_extension) {
    string filename = "document.txt";
    string result = remove_extension(filename);
    assert(result == "document" && "Should remove .txt extension");
}

TEST(test_remove_extension_multiple_dots) {
    string filename = "my.file.name.txt";
    string result = remove_extension(filename);
    assert(result == "my.file.name" && "Should remove last extension .txt");
}

TEST(test_remove_extension_no_extension) {
    string filename = "document";
    string result = remove_extension(filename);
    assert(result == "document" && "Should return unchanged filename with no extension");
}

TEST(test_remove_extension_empty_string) {
    string filename = "";
    string result = remove_extension(filename);
    assert(result == "" && "Should return empty string for empty input");
}

TEST(test_remove_extension_only_extension) {
    string filename = ".txt";
    string result = remove_extension(filename);
    assert(result == "" && "Should return empty string for filename with only extension");
}

TEST(test_remove_extension_relative_path_up) {
    string filename = "../path/to/file.txt";
    string result = remove_extension(filename);
    assert(result == "../path/to/file" && "Should return empty string for filename with only extension");
}

TEST(test_remove_extension_relative_path_up_no_ext) {
    string filename = "../path/to/file";
    string result = remove_extension(filename);
    assert(result == "../path/to/file" && "Should return empty string for filename with only extension");
}

#endif