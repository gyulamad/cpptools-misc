#pragma once

#ifdef TEST

#include "../TEST.hpp"
#include "../mkdir.hpp"
#include "../file_exists.hpp"
#include "../is_dir.hpp"
#include "../str_contains.hpp"
#include "../file_put_contents.hpp"
#include "../unlink.hpp"

TEST(test_mkdir_create_directory) {
    string test_dir = "test_mkdir_dir";
    
    bool result = mkdir(test_dir, 0777, false);
    assert(result && "Directory should be created successfully");
    assert(is_dir(test_dir) && "Created path should be a directory");
    
    // Cleanup
    rmdir(test_dir.c_str());
}

TEST(test_mkdir_directory_already_exists) {
    string test_dir = "test_mkdir_exists_dir";
    
    // Create directory first
    [[maybe_unused]] bool created = mkdir(test_dir, 0777, false);
    
    bool result = mkdir(test_dir, 0777, false);
    assert(result && "Should return true when directory already exists");
    
    // Cleanup
    rmdir(test_dir.c_str());
}

TEST(test_mkdir_recursive_create) {
    string test_dir = "test_mkdir_recursive/a/b/c";
    
    bool result = mkdir(test_dir, 0777, true);
    assert(result && "Recursive directory should be created successfully");
    assert(is_dir(test_dir) && "Created path should be a directory");
    
    // Cleanup
    rmdir(("test_mkdir_recursive/a/b/c").c_str());
    rmdir(("test_mkdir_recursive/a/b").c_str());
    rmdir(("test_mkdir_recursive/a").c_str());
    rmdir(test_dir.c_str());
}

TEST(test_mkdir_with_permissions) {
    string test_dir = "test_mkdir_permissions_dir";
    
    bool result = mkdir(test_dir, 0755, false);
    assert(result && "Directory should be created with permissions");
    
    // Cleanup
    rmdir(test_dir.c_str());
}

TEST(test_mkdir_invalid_path) {
    // Try to create directory in non-existent parent (without recursive)
    string invalid_dir = "/nonexistent_parent_dir/test_dir_xyz";
    
    bool result = mkdir(invalid_dir, 0777, false);
    assert(!result && "Should fail to create directory in non-existent parent");
}

TEST(test_mkdir_failed_to_create_directory) {
    // Try to create a directory where a file with the same name already exists
    // This triggers the "Failed to create directory" branch (line 40)
    // Note: On some systems, fs::create_directory() may return false when a file exists
    // and fs::exists() might return false in certain edge cases
    string file_as_dir = "test_mkdir_file_as_dir";
    
    // First, create a file with this name
    file_put_contents(file_as_dir, "test content");
    
    // Now try to create a directory with the same name - this should fail
    bool result = mkdir(file_as_dir, 0777, false);
    assert(!result && "Should fail to create directory when a file with same name exists");
    
    // Cleanup
    unlink(file_as_dir.c_str());
}

// Test for mkdir with second overload (different parameter order: dir, recursive, permission)
TEST(test_mkdir_with_different_parameter_order) {
    string test_dir = "test_mkdir_diff_param_dir";
    
    // Use the second overload: mkdir(const string& dir, bool recursive, int permission = 0777)
    bool result = mkdir(test_dir, false, 0755);
    assert(result && "Directory should be created successfully with different parameter order");
    assert(is_dir(test_dir) && "Created path should be a directory");
    
    // Cleanup
    rmdir(test_dir.c_str());
}

#endif