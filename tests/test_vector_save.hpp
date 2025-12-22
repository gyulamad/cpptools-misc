#pragma once

#include "../TEST.hpp"
#include "../vector_save.hpp"

#ifdef TEST


TEST(test_vector_save_empty) {
    vector<int> vec;
    vector_save(vec, "test_empty.bin");
    ifstream file("test_empty.bin", ios::binary);
    assert(file.is_open());
    size_t size;
    file.read(reinterpret_cast<char*>(&size), sizeof(size));
    assert(size == 0);
    file.close();
    
    // cleanup
    remove("test_empty.bin");

}

TEST(test_vector_save_with_elements) {
    vector<int> vec = {1, 2, 3, 4, 5};
    vector_save(vec, "test_with_elements.bin");
    ifstream file("test_with_elements.bin", ios::binary);
    assert(file.is_open());
    size_t size;
    file.read(reinterpret_cast<char*>(&size), sizeof(size));
    assert(size == 5);
    int data;
    for (int i = 0; i < 5; ++i) {
        file.read(reinterpret_cast<char*>(&data), sizeof(data));
        assert(data == i + 1);
    }
    file.close();
    
    // cleanup
    remove("test_with_elements.bin");

}

TEST(test_vector_save_error) {
    vector<int> vec = {1, 2, 3};
    bool t = false;
    try {
        vector_save(vec, "/invalid/path/test_error.bin");
    } catch (const exception& e) {
        t = true;
    }
    assert(t && "Expected an exception");
}

#endif
