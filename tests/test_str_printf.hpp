#pragma once

#include "../TEST.hpp"
#include "../str_printf.hpp"

#ifdef TEST


#include "../str_contains.hpp"

// Test struct for cases that need multiple inputs
struct test_str_printf_TestData {
    string format;
    string expected;
    // We will pass the actual arguments in the test body
};

// Basic functionality tests

TEST(test_str_printf_empty_format) {
    string result = str_printf("");
    assert(result.empty() && "Empty format should return empty string");
}

TEST(test_str_printf_simple_string) {
    string result = str_printf("hello world");
    assert(result == "hello world" && "Simple string should be returned unchanged");
}

TEST(test_str_printf_one_int) {
    string result = str_printf("value = %d", 42);
    assert(result == "value = 42" && "Integer formatting should work");
}

TEST(test_str_printf_one_double) {
    string result = str_printf("pi ≈ %.2f", 3.14159);
    assert(result == "pi ≈ 3.14" && "Double formatting with precision should work");
}

TEST(test_str_printf_multiple_arguments) {
    string result = str_printf("Name: %s, Age: %d, Height: %.1f", "Alice", 30, 165.5);
    assert(result == "Name: Alice, Age: 30, Height: 165.5" && "Multiple arguments should format correctly");
}

TEST(test_str_printf_string_argument) {
    string name = "Bob";
    string result = str_printf("Hello, %s!", name.c_str());
    assert(result == "Hello, Bob!" && "std::string argument should work with %s");
}

TEST(test_str_printf_large_number) {
    string result = str_printf("Big number: %lld", 123456789012345LL);
    assert(result == "Big number: 123456789012345" && "Large integer should format correctly");
}

TEST(test_str_printf_negative_number) {
    string result = str_printf("Negative: %d", -123);
    assert(result == "Negative: -123" && "Negative numbers should format correctly");
}

TEST(test_str_printf_zero) {
    string result = str_printf("Zero: %d", 0);
    assert(result == "Zero: 0" && "Zero should format correctly");
}

TEST(test_str_printf_very_long_string) {
    string long_text = "This is a very long string that exceeds the typical buffer size to test allocation.";
    string result = str_printf("Text: %s", long_text.c_str());
    assert(result == "Text: " + long_text && "Long strings should be handled correctly");
}

TEST(test_str_printf_null_pointer) {
    const char* null_ptr = nullptr;
    string result = str_printf("Pointer: %p", (void*)null_ptr);
    assert(str_contains(result, "0x0") || str_contains(result, "(nil)") && "Null pointer should format as 0x0 or (nil)");
}

#endif