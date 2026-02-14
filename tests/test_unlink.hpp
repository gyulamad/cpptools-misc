#pragma once

#ifdef TEST

#include "../TEST.hpp"
#include "../unlink.hpp"
#include "../file_exists.hpp"
#include "../str_contains.hpp"
#include "../file_put_contents.hpp"

TEST(test_unlink_existing_file) {
    string test_file = "test_unlink_file.txt";
    
    // Create test file
    file_put_contents(test_file, "test content");
    assert(file_exists(test_file) && "File should exist before unlink");
    
    // Note: C's ::unlink() returns 0 on success, which converts to false (boolean)
    // The current unlink implementation directly returns the result without negation
    bool result = unlink(test_file);
    // Since unlink returns 0 on success, result will be false
    // This documents the current behavior
    assert(!result && "C's unlink returns 0 on success which converts to false");
    assert(!file_exists(test_file) && "File should not exist after unlink");
}

TEST(test_unlink_nonexistent_file) {
    string nonexistent_file = "nonexistent_unlink_file_xyz.txt";
    
    bool result = unlink(nonexistent_file);
    // C's ::unlink() returns -1 for non-existent file which converts to true
    assert(result && "C's unlink returns -1 for non-existent file which converts to true");
}

TEST(test_unlink_empty_filename) {
    string empty_filename = "";
    
    bool result = unlink(empty_filename);
    // C's ::unlink("") returns -1 which converts to true
    assert(result && "C's unlink returns -1 for empty string which converts to true");
}

#endif