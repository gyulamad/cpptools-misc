#pragma once

#include "../TEST.hpp"
#include "../array_dump.hpp"

#ifdef TEST


#include "../capture_cout.hpp"

TEST(test_array_dump_basic) {
    vector<int> vec = {1, 2, 3};
    string expected_output = "vector(3): (1, 2, 3)\n";
    string expected_result = "vector(3): (1, 2, 3)";
    
    string result;
    string output = capture_cout([&]() { result = array_dump(vec, true); });
    assert(output == expected_output && "Basic dump shows");
    assert(result == expected_result && "Basic dump returns");
}

TEST(test_array_dump_empty_vector) {
    vector<int> vec;
    string expected = "vector(0): ()\n";
    
    string output = capture_cout([&]() { array_dump(vec, true); });
    assert(output == expected && "Empty vector dump");
}

TEST(test_array_dump_strings) {
    vector<string> vec = {"hello", "world"};
    string expected = "vector(2): (hello, world)\n";
    
    string output = capture_cout([&]() { array_dump(vec, true); });
    assert(output == expected && "Strings dump");
}

TEST(test_array_dump_no_debug) {
    vector<int> vec = {1, 2, 3};
    string expected = "vector(3): (1, 2, 3)";
    
    string results;
    string output = capture_cout([&]() { results = array_dump(vec, false); });
    assert(output.empty() && "No output dump");
    assert(results == expected && "Still returns dump even if not shown");
}

#endif
