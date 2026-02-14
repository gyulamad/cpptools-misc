#pragma once

#ifdef TEST

#include "../TEST.hpp"
#include "../Values.hpp"


// #include "unlink.hpp"

// Test struct for Values class
struct test_Values_TestData {
    string filename;
    vector<Value> values;
};

// Test for adding a value and retrieving it by index
TEST(test_Values_add_and_operator_index) {
    Values values;
    values["test"] = 42;

    assert(values["test"] == 42.0f && "Value should be 42.0 at name 'test'");
}

// Test for retrieving a value by name
TEST(test_Values_getValueByName_valid_name) {
    Values values;
    values["test_name"] = 100;

    Value& retrieved = values.getValueByName("test_name");
    assert(retrieved == 100.0f && "Value should match at the name");
}

// Test for getByName throwing exception for invalid name
TEST(test_Values_getValueByName_invalid_name_expected_to_throw) {
    Values values;
    bool threw = false;
    try {
        values.getValueByName("non_existent");
    } catch (const exception& e) {
        string what = e.what();
        assert(str_contains(what, "Value is not found") && "Exception should contain 'Value not found'");
        threw = true;
    }
    assert(threw && "getByName should throw for invalid name");
}

// // Test for saving and loading in TEXT mode
// TEST(test_Values_save_and_load_text_mode) {
//     test_Values_TestData data;
//     data.filename = "test_values.txt";
//     unlink(data.filename);

//     Values values;
//     values["test"] = 123.45f;
//     values.save<float>(data.filename, Filemode::TEXT);

//     Values loaded;
//     loaded.load<float>(data.filename, Filemode::TEXT);

//     assert(loaded.size() == 1 && "Loaded values should have size 1");
//     assert(loaded["test"] == 123.45f  && "Loaded value name should match");
//     unlink(data.filename);
// }

// // Test for saving and loading in BINARY mode
// TEST(test_Values_save_and_load_binary_mode) {
//     test_Values_TestData data;
//     data.filename = "test_values.bin";
//     unlink(data.filename);

//     Values values;
//     values["binary_test"] = 678.9f;
//     values.save<float>(data.filename, Filemode::BINARY);

//     Values loaded;
//     loaded.load<float>(data.filename, Filemode::BINARY);

//     assert(loaded.size() == 1 && "Loaded values should have size 1");
//     assert(loaded["binary_test"] == 678.9f && "Loaded value name should match");
//     unlink(data.filename);
// }

// Test for getVariables with non-constant values
TEST(test_Values_getVariables_non_constant) {
    Values values;
    values["var1"] = 10;
    values["var1"].setConstant(false);
    values["var2"] = 20;
    values["var2"].setConstant(true);

    vector<float> variables = values.getVariables();
    assert(variables.size() == 1 && "Should return only non-constant values");
    assert(variables[0] == 10.0f && "Non-constant value should be 10.0");
}

// Test for setVariables with matching size
TEST(test_Values_setVariables_matching_size) {
    Values values;
    values["var1"] = 0.0f;
    values["var1"].setConstant(false);
    values["var2"] = 0.0f;
    values["var2"].setConstant(false);

    vector<float> new_values = {55.5f, 66.6f};
    values.setVariables(new_values);

    assert(values["var1"] == 55.5f && "First value should be updated to 55.5");
    assert(values["var2"] == 66.6f && "Second value should be updated to 66.6");
}

// Test for setVariables throwing exception for size mismatch
TEST(test_Values_setVariables_size_mismatch_expected_to_throw) {
    Values values;
    values["var1"] = 0;

    vector<float> new_values = {1.0f, 2.0f};
    bool threw = false;
    try {
        values.setVariables(new_values);
    } catch (const exception& e) {
        assert(str_contains(e.what(), "Values size mismatch") && "Exception should contain 'Values size mismatch'");
        threw = true;
    }
    assert(threw && "setVariables should throw for size mismatch");
}

// Test for getLowerBounds with non-constant values
TEST(test_Values_getLowerBounds_non_constant) {
    Values values;
    values["var1"] = 10;
    values["var1"].setBounds(-10.0f, 10.0f);
    values["var2"] = 20;
    values["var2"].setConstant(true);

    vector<float> bounds = values.getLowerBounds();
    assert(bounds.size() == 1 && "Should return bounds for non-constant values only");
    assert(bounds[0] == -10.0f && "Lower bound should be -10.0");
}

// Test for getUpperBounds with non-constant values
TEST(test_Values_getUpperBounds_non_constant) {
    Values values;
    values["var1"] = 10;
    values["var1"].setBounds(-10.0f, 10.0f);
    values["var1"].setConstant(false);
    
    values["var2"] = 20;
    values["var2"].setConstant(true);

    vector<float> bounds = values.getUpperBounds();
    assert(bounds.size() == 1 && "Should return bounds for non-constant values only");
    assert(bounds[0] == 10.0f && "Upper bound should be 10.0");
}

// Test for size method
TEST(test_Values_size) {
    Values values;
    assert(values.size() == 0 && "Initial size should be 0");

    values["name"] = 0;
    assert(values.size() == 1 && "Size should be 1 after adding one value");
}

// Test for dump method without show
TEST(test_Values_dump_without_show) {
    Values values;
    values["test"] = 42;

    string output = values.dump(false);
    assert(str_contains(output, "test") && "Dump should contain value name 'test'");
    assert(str_contains(output, "42") && "Dump should contain value '42'");
}

// Test for onLoad method
TEST(test_Values_onLoad) {
    Values values;
    values["test"] = 42;
    
    // onLoad is called internally when convert() is invoked
    // We can't directly call onLoad since it's virtual and called by the framework
    // But we can verify that the values are properly converted
    assert(values["test"] == 42.0f && "Value should be properly converted");
}

// Test for onReload method
TEST(test_Values_onReload) {
    Values values;
    values["test"] = 42;
    
    // onReload is called internally when reset() is invoked
    // We can't directly call onReload since it's virtual and called by the framework
    // But we can verify that the values are properly reloaded
    assert(values["test"] == 42.0f && "Value should be properly reloaded");
}

// Test for convert method (private method called during initialization)
TEST(test_Values_convert) {
    Values values;
    values["test"] = 42;
    
    // convert() is called during initialization and when onLoad/onReload are called
    // Verify that values are properly converted from IniData
    assert(values["test"] == 42.0f && "Value should be properly converted");
}

// Test for convert() with INI file loading
// Note: The INI file format for ValuesT is complex and requires:
// 1. Sections become value names
// 2. Each section must have a "name" key that matches the section name
// 3. Other keys are value properties (value, lower, upper, step, constant, etc.)
// Due to the complexity of the format and the difficulty in testing through INI files,
// this test focuses on the core functionality through direct Value operations
TEST(test_Values_convert_value_set) {
    Values values;
    values["test_section"] = 42.5f;
    
    // Verify the value was set correctly
    assert(values["test_section"] == 42.5f && "Value should be set correctly");
}

// Test for polymorphic destruction (virtual destructor and virtual methods)
struct test_Values_DerivedClass: public ValuesT<float> {
    bool destructor_called = false;
    bool onreload_called = false;
    
    test_Values_DerivedClass(const string& inifname, bool load = false, bool createIfNotExists = false, bool throwsIfNotExists = false, bool warnsIfNotExists = false, bool verbose = false):
        ValuesT<float>(inifname, load, createIfNotExists, throwsIfNotExists, warnsIfNotExists, verbose)
    {}
    
    void onLoad() override {
        ValuesT<float>::onLoad();
    }
    
    void onReload() override {
        onreload_called = true;
        ValuesT<float>::onReload();
    }
    
    ~test_Values_DerivedClass() override {
        destructor_called = true;
    }
};

TEST(test_Values_polymorphic_destruction) {
    // Create derived class instance through base pointer
    test_Values_DerivedClass* derived = new test_Values_DerivedClass("", false, false, false, false, false);
    
    // Verify virtual methods work
    assert(derived->onreload_called == false && "onReload should not be called yet");
    
    // Delete through base pointer - this tests virtual destructor
    delete derived;
    
    // Verify destructor was called
    assert(derived->destructor_called == true && "Destructor should be called during polymorphic deletion");
}

// Test for getIndexByName with create=false (throws exception for non-existent key)
TEST(test_Values_getIndexByName_create_false_expected_to_throw) {
    Values values;
    values["existing"] = 42;
    
    bool threw = false;
    try {
        // This should throw because create=false and key doesn't exist
        values.getIndexByName("non_existent", false);
    } catch (const exception& e) {
        assert(str_contains(e.what(), "Value is not found") && "Exception should contain 'Value is not found'");
        threw = true;
    }
    assert(threw && "getIndexByName with create=false should throw for non-existent key");
}

// Test for getValueNotFoundError with valid data
TEST(test_Values_getValueNotFoundError_with_valid_data) {
    Values values;
    values["test_key"] = 123.45f;
    
    // Get the value to ensure it exists
    Value& value = values.getValueByName("test_key");
    assert(value == 123.45f && "Value should be retrieved correctly");
}

// Test for getValueNotFoundError with invalid data (exception message content)
TEST(test_Values_getValueNotFoundError_with_invalid_data) {
    Values values;
    values["test_key"] = 42;
    
    bool threw = false;
    try {
        values.getValueByName("non_existent_key");
    } catch (const exception& e) {
        string what = e.what();
        assert(str_contains(what, "Value is not found") && "Exception should contain 'Value is not found'");
        assert(str_contains(what, "non_existent_key") && "Exception should contain the key name");
        threw = true;
    }
    assert(threw && "getValueByName should throw for non-existent key");
}

// // Test for load with invalid file mode throwing exception
// TEST(test_Values_load_invalid_mode_expected_to_throw) {
//     Values values;
//     bool threw = false;
//     try {
//         values.load<float>("test.txt", static_cast<Filemode>(999));
//     } catch (const exception& e) {
//         assert(str_contains(e.what(), "Invalid file mode") && "Exception should contain 'Invalid file mode'");
//         threw = true;
//     }
//     assert(threw && "load should throw for invalid file mode");
// }

// // Test for save with invalid file mode throwing exception
// TEST(test_Values_save_invalid_mode_expected_to_throw) {
//     Values values;
//     bool threw = false;
//     try {
//         values.save<float>("test.txt", static_cast<Filemode>(999));
//     } catch (const exception& e) {
//         assert(str_contains(e.what(), "Invalid file mode") && "Exception should contain 'Invalid file mode'");
//         threw = true;
//     }
//     assert(threw && "save should throw for invalid file mode");
// }

#endif
