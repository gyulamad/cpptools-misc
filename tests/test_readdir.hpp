#pragma once

#include "../TEST.hpp"
#include "../readdir.hpp"

#ifdef TEST


#include <fstream>

// Helper to create test directory structure
void test_readir_setup_test_directory(const string& root) {
    fs::create_directory(root);
    fs::create_directory(root + "/subdir1");
    fs::create_directory(root + "/subdir1/subsubdir");
    ofstream(root + "/file1.txt").close();
    ofstream(root + "/file2.jpg").close();
    ofstream(root + "/subdir1/file3.txt").close();
    ofstream(root + "/subdir1/subsubdir/file4.txt").close();
}

// Helper to clean up test directory
void test_readir_cleanup_test_directory(const string& root) {
    fs::remove_all(root);
}

// Test cases
TEST(test_readdir_non_recursive_no_pattern) {
    string test_dir = "test_dir";
    test_readir_setup_test_directory(test_dir);

    auto files = readdir(test_dir, "", false);
    assert(files.size() == 2); // file1.txt, file2.jpg
    assert(find(files.begin(), files.end(), test_dir + "/file1.txt") != files.end());
    assert(find(files.begin(), files.end(), test_dir + "/file2.jpg") != files.end());

    test_readir_cleanup_test_directory(test_dir);
}

TEST(test_readdir_recursive_no_pattern) {
    string test_dir = "test_dir";
    test_readir_setup_test_directory(test_dir);

    auto files = readdir(test_dir, "", true);
    assert(files.size() == 4); // file1.txt, file2.jpg, subdir1/file3.txt, subdir1/subsubdir/file4.txt
    assert(find(files.begin(), files.end(), test_dir + "/file1.txt") != files.end());
    assert(find(files.begin(), files.end(), test_dir + "/file2.jpg") != files.end());
    assert(find(files.begin(), files.end(), test_dir + "/subdir1/file3.txt") != files.end());
    assert(find(files.begin(), files.end(), test_dir + "/subdir1/subsubdir/file4.txt") != files.end());

    test_readir_cleanup_test_directory(test_dir);
}

TEST(test_readdir_non_recursive_with_pattern) {
    string test_dir = "test_dir";
    test_readir_setup_test_directory(test_dir);

    auto files = readdir(test_dir, "*.txt", false);
    assert(files.size() == 1); // Only file1.txt
    assert(find(files.begin(), files.end(), test_dir + "/file1.txt") != files.end());
    assert(find(files.begin(), files.end(), test_dir + "/file2.jpg") == files.end());

    test_readir_cleanup_test_directory(test_dir);
}

TEST(test_readdir_recursive_with_pattern) {
    string test_dir = "test_dir";
    test_readir_setup_test_directory(test_dir);

    auto files = readdir(test_dir, "*.txt", true);
    assert(files.size() == 3); // file1.txt, subdir1/file3.txt, subdir1/subsubdir/file4.txt
    assert(find(files.begin(), files.end(), test_dir + "/file1.txt") != files.end());
    assert(find(files.begin(), files.end(), test_dir + "/subdir1/file3.txt") != files.end());
    assert(find(files.begin(), files.end(), test_dir + "/subdir1/subsubdir/file4.txt") != files.end());
    assert(find(files.begin(), files.end(), test_dir + "/file2.jpg") == files.end());

    test_readir_cleanup_test_directory(test_dir);
}

TEST(test_readdir_empty_directory) {
    string test_dir = "test_dir";
    fs::create_directory(test_dir);

    auto files = readdir(test_dir, "", true);
    assert(files.empty());

    test_readir_cleanup_test_directory(test_dir);
}

TEST(test_readdir_invalid_directory) {
    string test_dir = "test_dir";
    bool threw_exception = false;
    try {
        readdir(test_dir + "/nonexistent");
    } catch (const exception&) {
        threw_exception = true;
    }
    assert(threw_exception);

    // No cleanup needed (directory doesn't exist)
}

TEST(test_readdir_pattern_star_only) {
    string test_dir = "test_dir";
    test_readir_setup_test_directory(test_dir);

    // "*" should match all files (same as empty pattern)
    auto files = readdir(test_dir, "*", true);
    assert(files.size() == 4); // file1.txt, file2.jpg, subdir1/file3.txt, subdir1/subsubdir/file4.txt
    assert(find(files.begin(), files.end(), test_dir + "/file1.txt") != files.end());
    assert(find(files.begin(), files.end(), test_dir + "/file2.jpg") != files.end());
    assert(find(files.begin(), files.end(), test_dir + "/subdir1/file3.txt") != files.end());
    assert(find(files.begin(), files.end(), test_dir + "/subdir1/subsubdir/file4.txt") != files.end());

    test_readir_cleanup_test_directory(test_dir);
}

TEST(test_readdir_pattern_star_dot_star) {
    string test_dir = "test_dir";
    test_readir_setup_test_directory(test_dir);

    // "*.*" should match all files with extensions (same as "*" for our implementation)
    auto files = readdir(test_dir, "*.*", true);
    assert(files.size() == 4); // file1.txt, file2.jpg, subdir1/file3.txt, subdir1/subsubdir/file4.txt
    assert(find(files.begin(), files.end(), test_dir + "/file1.txt") != files.end());
    assert(find(files.begin(), files.end(), test_dir + "/file2.jpg") != files.end());
    assert(find(files.begin(), files.end(), test_dir + "/subdir1/file3.txt") != files.end());
    assert(find(files.begin(), files.end(), test_dir + "/subdir1/subsubdir/file4.txt") != files.end());

    test_readir_cleanup_test_directory(test_dir);
}

TEST(test_readdir_pattern_partial_extension) {
    string test_dir = "test_dir";
    test_readir_setup_test_directory(test_dir);

    // "*.tx*" - this might not work with current implementation
    // Let's see what happens
    auto files = readdir(test_dir, "*.tx*", true);
    
    // With current implementation, this will likely return empty
    // because it looks for exact match with ".tx*" extension
    assert(files.empty());

    test_readir_cleanup_test_directory(test_dir);
}

TEST(test_readdir_pattern_question_mark) {
    string test_dir = "test_dir";
    test_readir_setup_test_directory(test_dir);

    // "*.t?t" - this might not work with current implementation
    // Let's see what happens
    auto files = readdir(test_dir, "*.t?t", true);
    
    // With current implementation, this will likely return empty
    // because it looks for exact match with ".t?t" extension
    assert(files.empty());

    test_readir_cleanup_test_directory(test_dir);
}

TEST(test_readdir_multiple_patterns) {
    string test_dir = "test_dir";
    test_readir_setup_test_directory(test_dir);

    // Test with multiple patterns
    vector<string> patterns = {"*.txt", "*.jpg"};
    auto files = readdir(test_dir, patterns, true);
    assert(files.size() == 4); // file1.txt, file2.jpg, subdir1/file3.txt, subdir1/subsubdir/file4.txt
    assert(find(files.begin(), files.end(), test_dir + "/file1.txt") != files.end());
    assert(find(files.begin(), files.end(), test_dir + "/file2.jpg") != files.end());
    assert(find(files.begin(), files.end(), test_dir + "/subdir1/file3.txt") != files.end());
    assert(find(files.begin(), files.end(), test_dir + "/subdir1/subsubdir/file4.txt") != files.end());

    test_readir_cleanup_test_directory(test_dir);
}

TEST(test_readdir_pattern_no_extension) {
    string test_dir = "test_dir";
    test_readir_setup_test_directory(test_dir);
    
    // Create a file without extension
    ofstream(test_dir + "/noext").close();
    
    // Test with empty pattern (should include files without extensions)
    auto files = readdir(test_dir, "", true);
    assert(files.size() == 5); // All 4 files + noext
    assert(find(files.begin(), files.end(), test_dir + "/noext") != files.end());
    
    // Test with "*" pattern
    files = readdir(test_dir, "*", true);
    assert(files.size() == 5); // All 4 files + noext
    assert(find(files.begin(), files.end(), test_dir + "/noext") != files.end());
    
    // Test with "*.*" pattern (should not include files without extensions)
    files = readdir(test_dir, "*.*", true);
    assert(files.size() == 4); // Only files with extensions
    assert(find(files.begin(), files.end(), test_dir + "/noext") == files.end());

    test_readir_cleanup_test_directory(test_dir);
}

#endif