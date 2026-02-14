#pragma once

#ifdef TEST

#include "../TEST.hpp"
#include "../IniData.hpp"
#include "../str_contains.hpp"
#include "../Logger.hpp"
#include "../ConsoleLogger.hpp"
#include "../capture_cout_cerr.hpp"

using namespace std;

// Test for IniData default constructor
TEST(test_IniData_default_constructor) {
    IniData data;
    // Constructor should work without throwing
}

// Test for IniData setData
TEST(test_IniData_setData) {
    IniData data;
    DataT newData;
    newData["section1"]["key1"] = "value1";
    newData["section1"]["key2"] = "value2";
    newData["section2"]["key3"] = "value3";
    
    data.setData(newData);
    
    assert(data.getDataCRef().size() == 2 && "getDataCRef() should return 2 sections");
}

// Test for IniData getDataCRef
TEST(test_IniData_getDataCRef) {
    IniData data;
    const DataT& ref = data.getDataCRef();
    assert(ref.size() == 0 && "Empty IniData should have 0 sections");
}

// Test for IniData sections
TEST(test_IniData_sections) {
    IniData data;
    DataT newData;
    newData["section1"]["key1"] = "value1";
    newData["section2"]["key2"] = "value2";
    data.setData(newData);
    
    vector<string> secs = data.sections();
    assert(secs.size() == 2 && "sections() should return 2 sections");
    assert((secs[0] == "section1" || secs[0] == "section2") && "sections() should contain section1 or section2");
    assert((secs[1] == "section1" || secs[1] == "section2") && "sections() should contain the other section");
}

// Test for IniData clear
TEST(test_IniData_clear) {
    IniData data;
    DataT newData;
    newData["section1"]["key1"] = "value1";
    data.setData(newData);
    
    assert(data.size("section1") == 1 && "Before clear, size should be 1");
    data.clear();
    assert(data.getDataCRef().size() == 0 && "After clear, size should be 0");
}

// Test for IniData size
TEST(test_IniData_size) {
    IniData data;
    DataT newData;
    newData["section1"]["key1"] = "value1";
    newData["section1"]["key2"] = "value2";
    newData["section2"]["key3"] = "value3";
    data.setData(newData);
    
    assert(data.size("section1") == 2 && "size(section1) should return 2");
    assert(data.size("section2") == 1 && "size(section2) should return 1");
}

// Test for IniData has - existing key
TEST(test_IniData_has_existing_key) {
    IniData data;
    DataT newData;
    newData["section1"]["key1"] = "value1";
    data.setData(newData);
    
    assert(data.has("key1", "section1") && "has(key1, section1) should return true");
    assert(!data.has("nonexistent", "section1") && "has(nonexistent, section1) should return false");
    assert(!data.has("key1", "nonexistent") && "has(key1, nonexistent) should return false");
}

// Test for IniData keys
TEST(test_IniData_keys) {
    IniData data;
    DataT newData;
    newData["section1"]["key1"] = "value1";
    newData["section1"]["key2"] = "value2";
    data.setData(newData);
    
    vector<string> keys = data.keys("section1");
    assert(keys.size() == 2 && "keys(section1) should return 2 keys");
}

// Test for IniData values
TEST(test_IniData_values) {
    IniData data;
    DataT newData;
    newData["section1"]["key1"] = "value1";
    newData["section1"]["key2"] = "value2";
    data.setData(newData);
    
    vector<string> vals = data.values("section1");
    assert(vals.size() == 2 && "values(section1) should return 2 values");
}

// Test for IniData values - exception for non-existent section
TEST(test_IniData_values_exception) {
    IniData data;
    bool threw = false;
    try {
        data.values("nonexistent");
    } catch (exception& e) {
        threw = true;
        assert(str_contains(e.what(), "Section is not exists") && "Exception should contain 'Section is not exists'");
    }
    assert(threw && "values(nonexistent) should throw exception");
}

// Test for IniData list
TEST(test_IniData_list) {
    IniData data;
    DataT newData;
    newData["section1"]["key1"] = "123";
    newData["section1"]["key2"] = "456";
    data.setData(newData);
    
    auto result = data.list<int>("section1");
    assert(result.size() == 2 && "list<int>() should return 2 elements");
    assert(result["key1"] == 123 && "list<int> key1 should be 123");
    assert(result["key2"] == 456 && "list<int> key2 should be 456");
}

// Test for IniData get - existing key
TEST(test_IniData_get) {
    IniData data;
    DataT newData;
    newData["section1"]["key1"] = "123";
    newData["section1"]["key2"] = "hello";
    data.setData(newData);
    
    assert(data.get<int>("key1", "section1") == 123 && "get<int>(key1) should return 123");
    assert(data.get<string>("key2", "section1") == "hello" && "get<string>(key2) should return hello");
}

// Test for IniData get - exception for missing key
TEST(test_IniData_get_exception) {
    IniData data;
    bool threw = false;
    try {
        data.get<int>("nonexistent", "section1");
    } catch (exception& e) {
        threw = true;
        assert(str_contains(e.what(), "Key is not exists") && "Exception should contain 'Key is not exists'");
    }
    assert(threw && "get(int, nonexistent) should throw exception");
}

// Test for IniData getopt - existing key
TEST(test_IniData_getopt_existing) {
    IniData data;
    DataT newData;
    newData["section1"]["key1"] = "42";
    data.setData(newData);
    
    assert(data.getopt<int>("key1", 0, "section1", true) == 42 && "getopt(int) should return 42");
}

// Test for IniData getopt - missing key with warns=true
TEST(test_IniData_getopt_missing_warns) {
    IniData data;
    // Set up logger to capture warnings
    createLogger<ConsoleLogger>();
    
    // Capture the warning output
    string output = capture_cout_cerr([&data]() {
        int result = data.getopt<int>("nonexistent", 99, "section1", true);
        assert(result == 99 && "getopt should return default value");
    }, false);
    
    // Assert that the warning output contains the expected message
    // Note: The warning includes file:line info from FILELN macro and ANSI color codes
    // FILELN adds "\n\tat " before the file path
    assert(str_contains(output, "Key is not exists") && "Warning should contain 'Key is not exists'");
    assert(str_contains(output, "nonexistent") && "Warning should contain 'nonexistent'");
    assert(str_contains(output, "section1") && "Warning should contain 'section1'");
    assert(str_contains(output, "using default value instead") && "Warning should contain 'using default value instead'");
}

// Test for IniData getopt - missing key with warns=false
TEST(test_IniData_getopt_missing_no_warns) {
    IniData data;
    
    int result = data.getopt<int>("nonexistent", 99, "section1", false);
    assert(result == 99 && "getopt should return default value");
}

// Test for IniData set - new value
TEST(test_IniData_set_new) {
    IniData data;
    DataT newData;
    newData["section1"]["key1"] = "value1";
    data.setData(newData);
    
    data.set("newkey", 123, "section1");
    assert(data.get<int>("newkey", "section1") == 123 && "set should add new key");
}

// Test for IniData set - existing value (changed = false case)
TEST(test_IniData_set_existing_same) {
    IniData data;
    DataT newData;
    newData["section1"]["key1"] = "value1";
    data.setData(newData);
    
    data.set("key1", "value1", "section1");
    assert(data.get<string>("key1", "section1") == "value1" && "set should update existing key");
}

// Test for IniData as_string overloads
TEST(test_IniData_as_string_overloads) {
    IniData data;
    
    // These are protected/private, so we test indirectly through set
    DataT newData;
    newData["section1"]["intkey"] = "123";
    data.setData(newData);
    
    data.set("intkey", 456, "section1");
    assert(data.get<int>("intkey", "section1") == 456 && "set should convert int to string");
    
    data.set("stringkey", string("hello"), "section1");
    assert(data.get<string>("stringkey", "section1") == "hello" && "set should convert string to string");
    
    data.set("charkey", "world", "section1");
    assert(data.get<string>("charkey", "section1") == "world" && "set should convert char* to string");
}

#endif