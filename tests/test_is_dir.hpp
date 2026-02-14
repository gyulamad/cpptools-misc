#pragma once

#ifdef TEST

#include "../TEST.hpp"
#include "../is_dir.hpp"
#include "../mkdir.hpp"
#include "../file_exists.hpp"
#include "../str_contains.hpp"
#include "../unlink.hpp"
#include "../file_put_contents.hpp"

TEST(test_is_dir_existing_directory) {
    string test_dir = "test_is_dir_dir";
    
    // Create test directory
    [[maybe_unused]] bool created = mkdir(test_dir, 0777, false);
    
    bool result = is_dir(test_dir);
    assert(result && "Directory should exist");
    
    // Cleanup
    rmdir(test_dir.c_str());
}

TEST(test_is_dir_nonexistent_path) {
    string nonexistent_dir = "nonexistent_test_dir_xyz";
    
    bool result = is_dir(nonexistent_dir);
    assert(!result && "Nonexistent path should not be a directory");
}

TEST(test_is_dir_file_not_directory) {
    string test_file = "test_is_dir_file.txt";
    
    // Create a file
    file_put_contents(test_file, "test content");
    
    bool result = is_dir(test_file);
    assert(!result && "File should not be detected as directory");
    
    // Cleanup
    unlink(test_file);
}

#endif