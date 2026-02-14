#pragma once

#ifdef TEST

#include "../TEST.hpp"
#include "../filemtime_duration.hpp"
#include "../file_put_contents.hpp"
#include "../file_exists.hpp"
#include "../str_contains.hpp"
#include "../unlink.hpp"

TEST(test_filemtime_duration_existing_file) {
    string test_file = "test_filemtime_duration.txt";
    file_put_contents(test_file, "test content");
    
    auto duration = filemtime_duration(test_file);
    
    // Verify the duration is valid (not zero)
    assert(duration.count() > 0 && "Duration should be greater than 0 for existing file");
    
    // Cleanup
    unlink(test_file);
}

TEST(test_filemtime_duration_nonexistent_file) {
    string nonexistent_file = "nonexistent_file_mtime_duration.txt";
    
    bool threw = false;
    try {
        filemtime_duration(nonexistent_file);
    } catch (exception& e) {
        threw = true;
        assert(str_contains(e.what(), "File does not exist") && "Exception should mention file does not exist");
    }
    
    assert(threw && "Should throw exception for nonexistent file");
}

TEST(test_filemtime_duration_empty_filename) {
    string empty_filename = "";
    
    bool threw = false;
    try {
        filemtime_duration(empty_filename);
    } catch (exception& e) {
        threw = true;
        assert(str_contains(e.what(), "Filename can not be empty") && "Exception should mention empty filename");
    }
    
    assert(threw && "Should throw exception for empty filename");
}

#endif