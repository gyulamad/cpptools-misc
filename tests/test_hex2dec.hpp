#pragma once

#ifdef TEST

#include "../TEST.hpp"
#include "../hex2dec.hpp"
#include "../str_contains.hpp"

TEST(test_hex2dec_converts_uppercase) {
    string hex = "FF";
    int value = hex2dec(hex);
    assert(value == 0xFF && "Converts uppercase FF to 255");
}

TEST(test_hex2dec_converts_lowercase) {
    string hex = "ff";
    int value = hex2dec(hex);
    assert(value == 0xff && "Converts lowercase ff to 255");
}

TEST(test_hex2dec_converts_mixed_case) {
    string hex = "fFaB";
    int value = hex2dec(hex);
    assert(value == 0xFFAB && "Converts mixed case fFaB correctly");
}

TEST(test_hex2dec_converts_single_digit_zero) {
    string hex = "0";
    int value = hex2dec(hex);
    assert(value == 0 && "Single digit 0 converts to zero");
}

TEST(test_hex2dec_converts_single_digit_a) {
    string hex = "A";
    int value = hex2dec(hex);
    assert(value == 10 && "Single digit A converts to 10");
}

TEST(test_hex2dec_converts_single_digit_f) {
    string hex = "F";
    int value = hex2dec(hex);
    assert(value == 15 && "Single digit F converts to 15");
}

TEST(test_hex2dec_converts_multi_digit_45fa3) {
    string hex = "45FA3";
    int value = hex2dec(hex);
    assert(value == 0x45FA3 && "Converts multi-digit 45FA3 to decimal");
}

TEST(test_hex2dec_converts_multi_digit_f3b) {
    string hex = "F3B";
    int value = hex2dec(hex);
    assert(value == 0xF3B && "Converts multi-digit F3B to decimal");
}

TEST(test_hex2dec_handles_leading_zeros) {
    string hex = "00FF";
    int value = hex2dec(hex);
    assert(value == 0xFF && "Leading zeros are handled correctly");
}

TEST(test_hex2dec_throws_on_empty_string) {
    bool threw = false;
    try {
        hex2dec("");
    } catch (exception& e) {
        assert(str_contains(e.what(), "Empty") && "Exception should mention empty input");
        threw = true;
    }
    assert(threw && "hex2dec should throw on empty string");
}

TEST(test_hex2dec_throws_on_invalid_character_g) {
    bool threw = false;
    try {
        hex2dec("G1A");
    } catch (exception& e) {
        assert(str_contains(e.what(), "Invalid") && "Exception should mention invalid character");
        threw = true;
    }
    assert(threw && "hex2dec should throw on non-hex character G");
}

TEST(test_hex2dec_throws_on_invalid_character_z) {
    bool threw = false;
    try {
        hex2dec("1AZ9");
    } catch (exception& e) {
        assert(str_contains(e.what(), "Invalid") && "Exception should mention invalid character");
        threw = true;
    }
    assert(threw && "hex2dec should throw on non-hex character Z");
}

TEST(test_hex2dec_throws_on_special_characters) {
    bool threw = false;
    try {
        hex2dec("1@3F");
    } catch (exception& e) {
        assert(str_contains(e.what(), "Invalid") && "Exception should mention invalid character");
        threw = true;
    }
    assert(threw && "hex2dec should throw on special characters like @");
}

#endif