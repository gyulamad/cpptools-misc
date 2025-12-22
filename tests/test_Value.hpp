#pragma once

#include "../TEST.hpp"
#include "../Value.hpp"

#ifdef TEST


// Test struct for Value class tests
template<typename real = float>
struct test_Value_TestData {
    const char* name;
    real value;
    real lower;
    real upper;
    real step;
    bool constant;
    bool rounded;
    bool clips;
    bool throws;
    test_Value_TestData(
        const char* name,
        real value,
        real lower,
        real upper,
        real step,
        bool constant,
        bool rounded,
        bool clips,
        bool throws
    ):
        name(name),
        value(value),
        lower(lower),
        upper(upper),
        step(step),
        constant(constant),
        rounded(rounded),
        clips(clips),
        throws(throws)
    {

    }
};

// Test constructor initialization and constraint enforcement
TEST(test_Value_constructor_initializes_correctly) {
    test_Value_TestData data("test", 5.7f, 0.0f, 10.0f, 2.0f, true, false, false, true);
    Value v(data.name, data.value, data.lower, data.upper, data.step, data.constant, data.rounded, data.clips, data.throws);
    assert(abs(v - 6.0f) == .0f && "Value should be discretized to 6 with step 2 and rounded");
    assert(v.getLower() == data.lower && "Lower bound should be 0");
    assert(v.getUpper() == data.upper && "Upper bound should be 10");
    assert(v.getStep() == data.step && "Step should be 2");
    assert(v.isRounded() == data.rounded && "Rounded should be true");
    assert(v.isConstant() == data.constant && "Constant should be false");
    assert(v.isThrows() == data.throws && "Throws should be true");
}

// Test invalid bounds in constructor
TEST(test_Value_constructor_invalid_bounds) {
    test_Value_TestData data("test", 5.0f, 10.0f, 0.0f, NAN, false, false, false, true);
    bool threw = false;
    try {
        Value v(data.name, data.value, data.lower, data.upper, data.step, data.constant);
    } catch (exception& e) {
        assert(str_contains(e.what(), "Invalid bounds") && "Exception should contain 'Invalid bounds'");
        threw = true;
    }
    assert(threw && "Constructor should throw for invalid bounds (lower > upper)");
}

// Test invalid step in constructor
TEST(test_Value_constructor_invalid_step) {
    test_Value_TestData data("test", 5.0f, 0.0f, 10.0f, -1.0f, false, false, false, true);
    bool threw = false;
    try {
        Value v(data.name, data.value, data.lower, data.upper, data.step, data.constant);
    } catch (exception& e) {
        assert(str_contains(e.what(), "Invalid discretization step") && "Exception should contain 'Invalid step'");
        threw = true;
    }
    assert(threw && "Constructor should throw for negative step");
}

// Test setLower and getLower
TEST(test_Value_setLower_updates_bounds) {
    Value v("test", 5.0f);
    v.setLower(2.0f);
    assert(v.getLower() == 2.0f && "Lower bound should be updated to 2");
    assert(v >= 2.0f && "Value should be clipped to lower bound if necessary");
}

// Test setUpper and getUpper: Updated test for out-of-bounds throwing
TEST(test_Value_out_of_bounds_throws2) {
    test_Value_TestData data("test", 15.0f, 0.0f, 10.0f, NAN, false, false, false, true);
    bool threw = false;
    try {
        Value v(data.name, data.value, data.lower, data.upper, data.step, data.constant, data.rounded, data.clips, data.throws);
    } catch (...) {
        threw = true;
    }
    assert(threw && "Out-of-bounds value should throw when clips is false and throws is true");
}

// Test setUpper and getUpper: test for clipping behavior
TEST(test_Value_clips_out_of_bounds) {
    test_Value_TestData data("test", 15.0f, 0.0f, 10.0f, NAN, false, true, true, true);
    Value v(data.name, data.value, data.lower, data.upper, data.step, data.constant, data.rounded, data.clips, data.throws);
    assert(abs(v - 10.0f) == .0f && "Value should be clipped to upper bound 10 when clips is true");
    assert(v.getLower() == data.lower && "Lower bound should be 0");
    assert(v.getUpper() == data.upper && "Upper bound should be 10");
    assert(v.isClips() == data.clips && "Clips should be true");
}

// Test setStep and discretization
TEST(test_Value_setStep_discretizes_value) {
    Value v("test", 5.7f, 0.0f, 10.0f, NAN, false);
    v.setStep(2.0f);
    assert(v.getStep() == 2.0f && "Step should be set to 2");
    assert(abs(v - 6.0f) == .0f && "Value should be discretized to 6 with step 2");
}

// Test setRounded
TEST(test_Value_setRounded_rounds_value) {
    Value v("test", 5.7f);
    v.setRounded(true);
    assert(v.isRounded() && "Rounded should be true");
    assert(abs(v - 6.0f) == .0f && "Value should be rounded to 6");
}

// Test assignment to constant
TEST(test_Value_assignment_to_constant_throws) {
    Value v("test", 5.0f, -INFINITY, INFINITY, NAN, true); // Constant
    bool threw = false;
    try {
        v = 10.0f;
    } catch (exception& e) {
        assert(str_contains(e.what(), "Cannot assign to constant") && "Exception should contain 'Cannot assign to constant'");
        threw = true;
    }
    assert(threw && "Assignment to constant should throw");
}

// Test out-of-bounds value
TEST(test_Value_out_of_bounds_throws) {
    test_Value_TestData data("test", 15.0f, 0.0f, 10.0f, NAN, false, false, false, true);
    bool threw = false;
    try {
        Value v(data.name, data.value, data.lower, data.upper, data.step, data.constant);
    } catch (exception& e) {
        assert(str_contains(e.what(), "Out of bounds") && "Exception should contain 'Out of bounds'");
        threw = true;
    }
    assert(threw && "Out-of-bounds value should throw");
}

// Test comparison with tolerance
TEST(test_Value_comparison_within_tolerance) {
    Value v1("test1", 5.0f);
    Value v2("test2", 5.0f + 1e-10f);
    assert(v1 == v2 && "Values within tolerance should be equal");
    assert(!(v1 != v2) && "Values within tolerance should not be unequal");
}

// Test arithmetic operator +
TEST(test_Value_addition_produces_correct_value) {
    Value v1("test", 5.0f, 0.0f, 10.0f, NAN, false);
    Value v2 = v1 + 3.0f;
    assert(abs(v2 - 8.0f) == .0f && "Addition should yield 8");
    assert(v2.getLower() == 0.0f && "Lower bound should be preserved");
    assert(v2.getUpper() == 10.0f && "Upper bound should be preserved");
}

// Test division by zero
TEST(test_Value_division_by_zero_throws) {
    Value v1("test1", 10.0f);
    Value v2("test2", 0.0f);
    bool threw = false;
    try {
        Value v3 = v1 / v2;
    } catch (exception& e) {
        assert(str_contains(e.what(), "Division by zero") && "Exception should contain 'Division by zero'");
        threw = true;
    }
    assert(threw && "Division by zero should throw");
}

// Test stream output
TEST(test_Value_stream_output_formats_correctly) {
    Value v("test", 5.0f);
    ostringstream oss;
    oss << v;
    assert(oss.str() == "5" && "Stream output should format value as string");
}

TEST(test_Value_copy_constructor_copies_correctly) {
    test_Value_TestData data("test", 5.7f, 0.0f, 10.0f, 2.0f, true, true, false, true);
    Value v1(data.name, data.value, data.lower, data.upper, data.step, data.constant, data.rounded, data.clips, data.throws);
    Value v2(v1);
    assert(abs(v2 - 6.0f) == 0.0f && "Copied value should be discretized and rounded to 6");
    assert(v2.getLower() == v1.getLower() && "Lower bound should be copied");
    assert(v2.getUpper() == v1.getUpper() && "Upper bound should be copied");
    assert(v2.getStep() == v1.getStep() && "Step should be copied");
    assert(v2.isRounded() == v1.isRounded() && "Rounded should be copied");
    assert(v2.isClips() == v1.isClips() && "Clips should be copied");
    assert(v2.isConstant() == v1.isConstant() && "Constant should be copied");
    assert(v2.isThrows() == v1.isThrows() && "Throws should be copied");
}

TEST(test_Value_move_constructor_moves_correctly) {
    test_Value_TestData data("test", 5.7f, 0.0f, 10.0f, 2.0f, true, true, false, true);
    Value v1(data.name, data.value, data.lower, data.upper, data.step, data.constant, data.rounded, data.clips, data.throws);
    Value v2(std::move(v1));
    assert(abs(v2 - 6.0f) == 0.0f && "Moved value should be discretized and rounded to 6");
    assert(v2.getLower() == data.lower && "Lower bound should be moved");
    assert(v2.getUpper() == data.upper && "Upper bound should be moved");
    assert(v2.getStep() == data.step && "Step should be moved");
    assert(v2.isRounded() == data.rounded && "Rounded should be moved");
    assert(v2.isClips() == data.clips && "Clips should be moved");
    assert(v2.isConstant() == data.constant && "Constant should be moved");
    assert(v2.isThrows() == data.throws && "Throws should be moved");
}

TEST(test_Value_copy_assignment_assigns_correctly) {
    test_Value_TestData data("test", 5.0f, 0.0f, 10.0f, NAN, false, false, false, true);
    Value v1(data.name, data.value, data.lower, data.upper, data.step, data.constant);
    Value v2("other", 2.0f);
    v2 = v1;
    assert(abs(v2 - 5.0f) == 0.0f && "Assigned value should be 5");
    assert(v2.getLower() == v1.getLower() && "Lower bound should be assigned");
    assert(v2.getUpper() == v1.getUpper() && "Upper bound should be assigned");
}

TEST(test_Value_subtraction_produces_correct_value) {
    Value v1("test", 8.0f, 0.0f, 10.0f, NAN, false);
    Value v2 = v1 - 3.0f;
    assert(abs(v2 - 5.0f) == 0.0f && "Subtraction should yield 5");
    assert(v2.getLower() == 0.0f && "Lower bound should be preserved");
    assert(v2.getUpper() == 10.0f && "Upper bound should be preserved");
}

TEST(test_Value_multiplication_produces_correct_value) {
    Value v1("test", 5.0f, 0.0f, 50.0f, NAN, false);
    Value v2 = v1 * 2.0f;
    assert(abs(v2 - 10.0f) == 0.0f && "Multiplication should yield 10");
    assert(v2.getLower() == 0.0f && "Lower bound should be preserved");
    assert(v2.getUpper() == 50.0f && "Upper bound should be preserved");
}

TEST(test_Value_division_produces_correct_value) {
    Value v1("test", 10.0f, 0.0f, 50.0f, NAN, false);
    Value v2 = v1 / 2.0f;
    assert(abs(v2 - 5.0f) == 0.0f && "Division should yield 5");
    assert(v2.getLower() == 0.0f && "Lower bound should be preserved");
    assert(v2.getUpper() == 50.0f && "Upper bound should be preserved");
}

TEST(test_Value_modulus_produces_correct_value) {
    Value v1("test", 10.0f, 0.0f, 50.0f, NAN, false);
    Value v2 = v1 % 3.0f;
    assert(abs(v2 - 1.0f) == 0.0f && "Modulus should yield 1");
    assert(v2.getLower() == 0.0f && "Lower bound should be preserved");
    assert(v2.getUpper() == 50.0f && "Upper bound should be preserved");
}

TEST(test_Value_compound_addition_updates_correctly) {
    Value v("test", 5.0f, 0.0f, 10.0f, NAN, false);
    v += 3.0f;
    assert(abs(v - 8.0f) == 0.0f && "Compound addition should yield 8");
}

TEST(test_Value_compound_subtraction_updates_correctly) {
    Value v("test", 8.0f, 0.0f, 10.0f, NAN, false);
    v -= 3.0f;
    assert(abs(v - 5.0f) == 0.0f && "Compound subtraction should yield 5");
}

TEST(test_Value_compound_multiplication_updates_correctly) {
    Value v("test", 5.0f, 0.0f, 50.0f, NAN, false);
    v *= 2.0f;
    assert(abs(v - 10.0f) == 0.0f && "Compound multiplication should yield 10");
}

TEST(test_Value_compound_division_updates_correctly) {
    Value v("test", 10.0f, 0.0f, 50.0f, NAN, false);
    v /= 2.0f;
    assert(abs(v - 5.0f) == 0.0f && "Compound division should yield 5");
}

TEST(test_Value_compound_modulus_updates_correctly) {
    Value v("test", 10.0f, 0.0f, 50.0f, NAN, false);
    v %= 3.0f;
    assert(abs(v - 1.0f) == 0.0f && "Compound modulus should yield 1");
}

TEST(test_Value_comparison_operators_work_correctly) {
    Value v1("test1", 5.0f);
    Value v2("test2", 6.0f);
    assert(v1 < v2 && "5 should be less than 6");
    assert(v1 <= v2 && "5 should be less than or equal to 6");
    assert(v2 > v1 && "6 should be greater than 5");
    assert(v2 >= v1 && "6 should be greater than or equal to 5");
    assert(!(v1 > v2) && "5 should not be greater than 6");
    assert(!(v2 < v1) && "6 should not be less than 5");
}

TEST(test_Value_serialize_deserialize_preserves_state) {
    test_Value_TestData data("test", 5.7f, 0.0f, 10.0f, 2.0f, true, true, false, true);
    Value v1(data.name, data.value, data.lower, data.upper, data.step, data.constant, data.rounded, data.clips, data.throws);
    auto serialized = v1.serialize();
    Value v2;
    size_t nxt = 0;
    v2.deserialize(serialized, nxt);
    assert(abs(v2 - 6.0f) == 0.0f && "Deserialized value should be 6");
    assert(v2.getLower() == v1.getLower() && "Lower bound should be preserved");
    assert(v2.getUpper() == v1.getUpper() && "Upper bound should be preserved");
    assert(v2.getStep() == v1.getStep() && "Step should be preserved");
    assert(v2.isConstant() == v1.isConstant() && "Constant should be preserved");
    assert(v2.isRounded() == v1.isRounded() && "Rounded should be preserved");
    assert(v2.isClips() == v1.isClips() && "Clips should be preserved");
    assert(v2.isThrows() == v1.isThrows() && "Throws should be preserved");
}

TEST(test_Value_toString_fromString_preserves_state) {
    test_Value_TestData data("test", 5.0f, 0.0f, 10.0f, 2.0f, true, true, false, true);
    Value v1(data.name, data.value, data.lower, data.upper, data.step, data.constant, data.rounded, data.clips, data.throws);
    string str = v1.toString();
    Value v2;
    v2.fromString(str);
    assert(v2 == 6 && "Parsed value should be 6 instead 5 (cause step = 2)");
    assert(v2.getLower() == v1.getLower() && "Lower bound should be preserved");
    assert(v2.getUpper() == v1.getUpper() && "Upper bound should be preserved");
    assert(v2.getStep() == v1.getStep() && "Step should be preserved");
    assert(v2.isRounded() == v1.isRounded() && "Rounded should be preserved");
    assert(v2.isClips() == v1.isClips() && "Clips should be preserved");
    assert(v2.isConstant() == v1.isConstant() && "Constant should be preserved");
    assert(v2.isThrows() == v1.isThrows() && "Throws should be preserved");
}

TEST(test_Value_fromString_invalid_line_throws) {
    Value v;
    bool threw = false;
    try {
        v.fromString("invalid_line");
    } catch (exception& e) {
        assert(str_contains(e.what(), "Parse error: Line is invalid") && "Exception should contain parse error");
        threw = true;
    }
    assert(threw && "Invalid line should throw");
}

TEST(test_Value_fromString_invalid_key_throws) {
    Value v;
    bool threw = false;
    try {
        v.fromString("unknown_key=value");
    } catch (exception& e) {
        assert(str_contains(e.what(), "Parse error: Unrecognised key") && "Exception should contain parse error");
        threw = true;
    }
    assert(threw && "Invalid key should throw");
}

TEST(test_Value_stream_input_updates_correctly) {
    Value v("test", 0.0f);
    istringstream iss("5.5");
    iss >> v;
    assert(abs(v - 5.5f) == 0.0f && "Stream input should update value to 5.5");
}

TEST(test_Value_setConstant_updates_correctly) {
    Value v("test", 5.0f);
    v.setConstant(true);
    assert(v.isConstant() && "Constant should be true");
    bool threw = false;
    try {
        v = 10.0f;
    } catch (exception& e) {
        threw = true;
    }
    assert(threw && "Assignment should throw after setting constant");
}

TEST(test_Value_setClips_updates_correctly) {
    Value v("test", 15.0f, 0.0f, 10.0f, NAN, false, false, false, false);
    v.setClips(true);
    assert(v.isClips() && "Clips should be true");
    assert(abs(v - 10.0f) == 0.0f && "Value should be clipped to 10");
}

TEST(test_Value_setThrows_updates_correctly) {
    Value v("test", 15.0f, 0.0f, 10.0f, NAN, false, false, true, true);
    v.setThrows(false);
    assert(!v.isThrows() && "Throws should be false");
    assert(abs(v - 15.0f) == 0.0f && "Value should not throw when out of bounds");
}

TEST(test_Value_setBounds_updates_correctly) {
    Value v("test", 5.0f);
    v.setBounds(2.0f, 8.0f);
    assert(v.getLower() == 2.0f && "Lower bound should be 2");
    assert(v.getUpper() == 8.0f && "Upper bound should be 8");
    bool threw = false;
    try {
        v.setBounds(10.0f, 0.0f);
    } catch (exception& e) {
        threw = true;
    }
    assert(threw && "Invalid bounds should throw");
}

TEST(test_Value_out_of_bounds_no_throw) {
    test_Value_TestData data("test", 15.0f, 0.0f, 10.0f, NAN, false, false, false, false);
    Value v(data.name, data.value, data.lower, data.upper, data.step, data.constant, data.rounded, data.clips, data.throws);
    assert(abs(v - 15.0f) == 0.0f && "Value should remain 15 when throws is false");
}

TEST(test_Value_infinite_bounds_allows_any_value) {
    test_Value_TestData data("test", 1e20f, -INFINITY, INFINITY, NAN, false, false, false, true);
    Value v(data.name, data.value, data.lower, data.upper, data.step, data.constant);
    assert(abs(v - 1e20f) == 0.0f && "Value should be allowed with infinite bounds");
}

TEST(test_Value_no_discretization_with_nan_step) {
    test_Value_TestData data("test", 5.7f, 0.0f, 10.0f, NAN, false, false, false, true);
    Value v(data.name, data.value, data.lower, data.upper, data.step, data.constant);
    assert(abs(v - 5.7f) == 0.0f && "Value should remain 5.7 with NAN step");
}

TEST(test_Value_clips_rounded_discretize_interaction) {
    test_Value_TestData data("test", 5.7f, 0.0f, 10.0f, 2.0f, true, true, false, true);
    Value v(data.name, data.value, data.lower, data.upper, data.step, data.constant, data.rounded, data.clips, data.throws);
    assert(abs(v - 6.0f) == 0.0f && "Value should be discretized to 6, rounded, and clipped if necessary");
}

// TODO: take back this test!
// TEST(test_Value_double_type_works_correctly) {
//     test_Value_TestData<double> data("test", 5.7, 0.0, 10.0, 2.0, true, false, false, true);
//     ValueT<double> v(data.name, data.value, data.lower, data.upper, data.step, data.constant, data.rounded, data.clips, data.throws);
//     assert(abs(v - 6.0) < 1e-10 && "Value should be discretized to 6 with double type");
// }

#endif
