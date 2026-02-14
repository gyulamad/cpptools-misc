#pragma once

#ifdef TEST

#include "../TEST.hpp"
#include "../latest_file_time_in_folder.hpp"
#include "../str_contains.hpp"
#include <fstream>
#include <thread>

using namespace std;

TEST(test_latest_file_time_in_folder_empty_directory) {
    string test_dir = "test_folder_empty_" + to_string(chrono::system_clock::now().time_since_epoch().count());
    fs::create_directory(test_dir);
    
    bool threw = false;
    try {
        latest_file_time_in_folder(test_dir);
    } catch (exception& e) {
        assert(str_contains(e.what(), "No files found") && "Exception should mention no files found");
        threw = true;
    }
    assert(threw && "Empty directory should throw exception");
    
    fs::remove(test_dir);
}

TEST(test_latest_file_time_in_folder_nonexistent_path) {
    string nonexistent = "nonexistent_folder_12345";
    
    bool threw = false;
    try {
        latest_file_time_in_folder(nonexistent);
    } catch (exception& e) {
        assert(str_contains(e.what(), "does not exist") && "Exception should mention path does not exist");
        threw = true;
    }
    assert(threw && "Nonexistent path should throw exception");
}

TEST(test_latest_file_time_in_folder_file_not_directory) {
    string test_file = "test_file_" + to_string(chrono::system_clock::now().time_since_epoch().count()) + ".txt";
    ofstream file(test_file);
    file << "test";
    file.close();
    
    bool threw = false;
    try {
        latest_file_time_in_folder(test_file);
    } catch (exception& e) {
        assert(str_contains(e.what(), "not a directory") && "Exception should mention not a directory");
        threw = true;
    }
    assert(threw && "File path should throw exception");
    
    fs::remove(test_file);
}

TEST(test_latest_file_time_in_folder_single_file) {
    string test_dir = "test_folder_single_" + to_string(chrono::system_clock::now().time_since_epoch().count());
    fs::create_directory(test_dir);
    
    string file_path = test_dir + "/file1.txt";
    ofstream file(file_path);
    file << "test content";
    file.close();
    
    auto result = latest_file_time_in_folder(test_dir);
    assert(result.count() > 0 && "Should return valid time for single file");
    
    fs::remove_all(test_dir);
}

TEST(test_latest_file_time_in_folder_multiple_files_root) {
    string test_dir = "test_folder_multi_" + to_string(chrono::system_clock::now().time_since_epoch().count());
    fs::create_directory(test_dir);
    
    // Create first file
    string file1 = test_dir + "/file1.txt";
    ofstream f1(file1);
    f1 << "content1";
    f1.close();
    
    auto time1 = latest_file_time_in_folder(test_dir);
    
    // Wait to ensure different modification time
    this_thread::sleep_for(chrono::milliseconds(10));
    
    // Create second file
    string file2 = test_dir + "/file2.txt";
    ofstream f2(file2);
    f2 << "content2";
    f2.close();
    
    auto time2 = latest_file_time_in_folder(test_dir);
    assert(time2 > time1 && "Newer file should result in later time");
    
    fs::remove_all(test_dir);
}

TEST(test_latest_file_time_in_folder_subdirectory_file) {
    string test_dir = "test_folder_subdir_" + to_string(chrono::system_clock::now().time_since_epoch().count());
    fs::create_directory(test_dir);
    
    // Create file in root
    string file1 = test_dir + "/file1.txt";
    ofstream f1(file1);
    f1 << "content1";
    f1.close();
    
    auto time1 = latest_file_time_in_folder(test_dir);
    
    // Wait and create subdirectory with file
    this_thread::sleep_for(chrono::milliseconds(10));
    fs::create_directory(test_dir + "/subdir");
    
    string file2 = test_dir + "/subdir/file2.txt";
    ofstream f2(file2);
    f2 << "content2";
    f2.close();
    
    auto time2 = latest_file_time_in_folder(test_dir);
    assert(time2 > time1 && "File in subdirectory should be detected as newest");
    
    fs::remove_all(test_dir);
}

TEST(test_latest_file_time_in_folder_deeply_nested) {
    string test_dir = "test_folder_nested_" + to_string(chrono::system_clock::now().time_since_epoch().count());
    fs::create_directory(test_dir);
    
    // Create file in root
    string file1 = test_dir + "/file1.txt";
    ofstream f1(file1);
    f1 << "content1";
    f1.close();
    
    auto time1 = latest_file_time_in_folder(test_dir);
    
    // Wait and create deeply nested file
    this_thread::sleep_for(chrono::milliseconds(10));
    fs::create_directories(test_dir + "/sub1/sub2/sub3");
    
    string file2 = test_dir + "/sub1/sub2/sub3/file2.txt";
    ofstream f2(file2);
    f2 << "content2";
    f2.close();
    
    auto time2 = latest_file_time_in_folder(test_dir);
    assert(time2 > time1 && "Deeply nested file should be detected as newest");
    
    fs::remove_all(test_dir);
}

TEST(test_latest_file_time_in_folder_modified_old_file) {
    string test_dir = "test_folder_modify_" + to_string(chrono::system_clock::now().time_since_epoch().count());
    fs::create_directory(test_dir);
    
    // Create two files
    string file1 = test_dir + "/file1.txt";
    ofstream f1(file1);
    f1 << "content1";
    f1.close();
    
    this_thread::sleep_for(chrono::milliseconds(10));
    
    string file2 = test_dir + "/file2.txt";
    ofstream f2(file2);
    f2 << "content2";
    f2.close();
    
    auto time1 = latest_file_time_in_folder(test_dir);
    
    // Modify the first (older) file
    this_thread::sleep_for(chrono::milliseconds(10));
    ofstream f1_modify(file1, ios::app);
    f1_modify << " modified";
    f1_modify.close();
    
    auto time2 = latest_file_time_in_folder(test_dir);
    assert(time2 > time1 && "Modified old file should become the newest");
    
    fs::remove_all(test_dir);
}

TEST(test_latest_file_time_in_folder_ignores_directories) {
    string test_dir = "test_folder_dirs_" + to_string(chrono::system_clock::now().time_since_epoch().count());
    fs::create_directory(test_dir);
    
    // Create a file
    string file1 = test_dir + "/file1.txt";
    ofstream f1(file1);
    f1 << "content1";
    f1.close();
    
    auto time1 = latest_file_time_in_folder(test_dir);
    
    // Create empty subdirectories (should not affect result)
    this_thread::sleep_for(chrono::milliseconds(10));
    fs::create_directory(test_dir + "/emptydir1");
    fs::create_directory(test_dir + "/emptydir2");
    
    auto time2 = latest_file_time_in_folder(test_dir);
    assert(time2 == time1 && "Creating empty directories should not change latest file time");
    
    fs::remove_all(test_dir);
}

// Test for latest_file_time_in_folder_ms function
TEST(test_latest_file_time_in_folder_ms) {
    string test_dir = "test_folder_ms_" + to_string(chrono::system_clock::now().time_since_epoch().count());
    fs::create_directory(test_dir);
    
    // Create a file
    string file1 = test_dir + "/file1.txt";
    ofstream f1(file1);
    f1 << "content1";
    f1.close();
    
    time_ms result_ms = latest_file_time_in_folder_ms(test_dir);
    assert(result_ms > 0 && "Should return valid time in milliseconds");
    
    fs::remove_all(test_dir);
}

// Test for latest_file_time_in_folder_sec function
TEST(test_latest_file_time_in_folder_sec) {
    string test_dir = "test_folder_sec_" + to_string(chrono::system_clock::now().time_since_epoch().count());
    fs::create_directory(test_dir);
    
    // Create a file
    string file1 = test_dir + "/file1.txt";
    ofstream f1(file1);
    f1 << "content1";
    f1.close();
    
    time_ms result_sec = latest_file_time_in_folder_sec(test_dir);
    assert(result_sec > 0 && "Should return valid time in seconds");
    
    fs::remove_all(test_dir);
}

#endif
