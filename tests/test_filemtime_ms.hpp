#pragma once

#ifdef TEST

#include "../TEST.hpp"
#include "../filemtime_ms.hpp"
#include "../file_put_contents.hpp"
#include "../file_exists.hpp"
#include "../unlink.hpp"

TEST(test_filemtime_ms_existing_file) {
    string test_file = "test_filemtime_ms.txt";
    file_put_contents(test_file, "test content");
    
    auto ms = filemtime_ms(test_file);
    
    // Verify the timestamp is valid (not zero)
    assert(ms > 0 && "Timestamp should be greater than 0 for existing file");
    
    // Cleanup
    unlink(test_file);
}

TEST(test_filemtime_ms_nonexistent_file) {
    string nonexistent_file = "nonexistent_file_mtime_ms.txt";
    
    bool threw = false;
    try {
        filemtime_ms(nonexistent_file);
    } catch (exception& e) {
        threw = true;
        assert(str_contains(e.what(), "File does not exist") && "Exception should mention file does not exist");
    }
    
    assert(threw && "Should throw exception for nonexistent file");
}

TEST(test_filemtime_ms_empty_filename) {
    string empty_filename = "";
    
    bool threw = false;
    try {
        filemtime_ms(empty_filename);
    } catch (exception& e) {
        threw = true;
        assert(str_contains(e.what(), "Filename can not be empty") && "Exception should mention empty filename");
    }
    
    assert(threw && "Should throw exception for empty filename");
}

#endif