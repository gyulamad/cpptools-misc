#pragma once

#include "../TEST.hpp"
#include "../Values.hpp"

#ifdef TEST


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
        assert(str_contains(e.what(), "Value not found") && "Exception should contain 'Value not found'");
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
