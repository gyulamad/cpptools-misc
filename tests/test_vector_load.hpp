#pragma once

#include "../TEST.hpp"
#include "../vector_load.hpp"

#ifdef TEST


#include "../vector_save.hpp"
#include "../vector_equal.hpp"

TEST(test_vector_load_success) {
    vector<int> vec;
    vector<int> expected = {1, 2, 3, 4, 5};
    vector_save(expected, "test.bin");
    vector_load(vec, "test.bin");
    
    // cleanup
    remove("test.bin");

    assert(vector_equal(vec, expected) && "Vector load success");
}

TEST(test_vector_load_file_open_error) {
    vector<int> vec;
    bool thrown = false;
    try {
        vector_load(vec, "non_existent_file.bin");
    } catch (exception& e) {
        thrown = true;
        string what = e.what();
        assert(str_contains(what, "Cannot open file for reading") && "File open error message");
    }
    assert(thrown && "Test case should throw but it didn't");
}

TEST(test_vector_load_data_read_error) {
    vector<int> vec;
    vector<int> expected = {1, 2, 3, 4, 5};
    vector_save(expected, "test.bin");
    // Corrupt the file
    ofstream corrupt("test.bin", ios::binary);
    corrupt.write("corrupted", 10);
    corrupt.close();
    bool thrown = false;
    try {
        vector_load(vec, "test.bin");
    } catch (exception& e) {
        thrown = true;
        //string what = e.what();
        //assert(str_contains(what, "Error reading data from file") && "Data read error message");
    }
    
    // cleanup
    remove("test.bin");

    assert(thrown && "Test case should throw but it didn't");
}

#endif
