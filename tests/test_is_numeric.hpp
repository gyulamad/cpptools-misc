#pragma once

#ifdef TEST

#include "../is_numeric.hpp"

TEST(test_is_numeric_integer_strings) {
    assert(is_numeric("123") && "Should accept positive integer string");
    assert(is_numeric("-456") && "Should accept negative integer string");
    assert(is_numeric("+789") && "Should accept integer with plus sign");
    assert(is_numeric("0") && "Should accept zero");
}

TEST(test_is_numeric_float_strings) {
    assert(is_numeric("3.14") && "Should accept decimal number");
    assert(is_numeric("-0.5") && "Should accept negative decimal");
    assert(is_numeric("+123.456") && "Should accept positive decimal with sign");
    assert(is_numeric("0.0") && "Should accept zero with decimal");
    assert(is_numeric(".5") && "Should accept decimal starting with dot");
    assert(is_numeric("5.") && "Should accept decimal ending with dot");
}

TEST(test_is_numeric_exponential_notation) {
    assert(is_numeric("1e10") && "Should accept exponential notation");
    assert(is_numeric("1.5e3") && "Should accept decimal with exponent");
    assert(is_numeric("2E-5") && "Should accept uppercase E with negative exponent");
    assert(is_numeric("3.14e+2") && "Should accept exponent with plus sign");
    assert(is_numeric("-1.5e-10") && "Should accept negative exponential");
}

TEST(test_is_numeric_invalid_inputs) {
    assert(!is_numeric("") && "Should reject empty string");
    assert(!is_numeric("abc") && "Should reject non-numeric string");
    assert(!is_numeric("12abc") && "Should reject string with trailing letters");
    assert(!is_numeric("abc12") && "Should reject string with leading letters");
    assert(!is_numeric("12.34.56") && "Should reject multiple decimal points");
    assert(!is_numeric("12e10e5") && "Should reject multiple exponents");
    assert(!is_numeric("+") && "Should reject sign only");
    assert(!is_numeric("-") && "Should reject sign only");
    assert(!is_numeric(".") && "Should reject dot only");
    assert(!is_numeric("e10") && "Should reject exponent without base");
    assert(!is_numeric("12e") && "Should reject exponent without power");
}

TEST(test_is_numeric_edge_cases) {
    assert(is_numeric("0.0") && "Should accept zero as decimal");
    assert(is_numeric("00123") && "Should accept leading zeros");
    assert(!is_numeric("  123") && "Should reject leading whitespace");
    assert(!is_numeric("123  ") && "Should reject trailing whitespace");
    assert(!is_numeric("1 2 3") && "Should reject spaces in number");
}

#endif
