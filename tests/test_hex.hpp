#pragma once

#ifdef TEST

#include "../TEST.hpp"
#include "../hex.hpp"
#include "../str_contains.hpp"

TEST(test_hex2uint_converts_uppercase) {
    string hex = "FF";
    int value = hex2uint(hex);
    assert(value == 0xFF && "Converts uppercase FF to 255");
}

TEST(test_hex2uint_converts_lowercase) {
    string hex = "ff";
    int value = hex2uint(hex);
    assert(value == 0xff && "Converts lowercase ff to 255");
}

TEST(test_hex2uint_converts_mixed_case) {
    string hex = "fFaB";
    int value = hex2uint(hex);
    assert(value == 0xFFAB && "Converts mixed case fFaB correctly");
}

TEST(test_hex2uint_converts_single_digit_zero) {
    string hex = "0";
    int value = hex2uint(hex);
    assert(value == 0 && "Single digit 0 converts to zero");
}

TEST(test_hex2uint_converts_single_digit_a) {
    string hex = "A";
    int value = hex2uint(hex);
    assert(value == 10 && "Single digit A converts to 10");
}

TEST(test_hex2uint_converts_single_digit_f) {
    string hex = "F";
    int value = hex2uint(hex);
    assert(value == 15 && "Single digit F converts to 15");
}

TEST(test_hex2uint_converts_multi_digit_45fa3) {
    string hex = "45FA3";
    int value = hex2uint(hex);
    assert(value == 0x45FA3 && "Converts multi-digit 45FA3 to decimal");
}

TEST(test_hex2uint_converts_multi_digit_f3b) {
    string hex = "F3B";
    int value = hex2uint(hex);
    assert(value == 0xF3B && "Converts multi-digit F3B to decimal");
}

TEST(test_hex2uint_handles_leading_zeros) {
    string hex = "00FF";
    int value = hex2uint(hex);
    assert(value == 0xFF && "Leading zeros are handled correctly");
}

TEST(test_hex2uint_throws_on_empty_string) {
    bool threw = false;
    try {
        hex2uint("");
    } catch (exception& e) {
        assert(str_contains(e.what(), "Empty") && "Exception should mention empty input");
        threw = true;
    }
    assert(threw && "hex2uint should throw on empty string");
}

TEST(test_hex2uint_throws_on_invalid_character_g) {
    bool threw = false;
    try {
        hex2uint("G1A");
    } catch (exception& e) {
        assert(str_contains(e.what(), "Invalid") && "Exception should mention invalid character");
        threw = true;
    }
    assert(threw && "hex2uint should throw on non-hex character G");
}

TEST(test_hex2uint_throws_on_invalid_character_z) {
    bool threw = false;
    try {
        hex2uint("1AZ9");
    } catch (exception& e) {
        assert(str_contains(e.what(), "Invalid") && "Exception should mention invalid character");
        threw = true;
    }
    assert(threw && "hex2uint should throw on non-hex character Z");
}

TEST(test_hex2uint_throws_on_special_characters) {
    bool threw = false;
    try {
        hex2uint("1@3F");
    } catch (exception& e) {
        assert(str_contains(e.what(), "Invalid") && "Exception should mention invalid character");
        threw = true;
    }
    assert(threw && "hex2uint should throw on special characters like @");
}

TEST(test_uint2hex_converts_zero) {
    string hex = uint2hex(0);
    assert(hex == "0" && "Zero converts to 0");
}

TEST(test_uint2hex_converts_single_digit_a) {
    string hex = uint2hex(10);
    assert(hex == "A" && "10 converts to A");
}

TEST(test_uint2hex_converts_single_digit_f) {
    string hex = uint2hex(15);
    assert(hex == "F" && "15 converts to F");
}

TEST(test_uint2hex_converts_255_to_ff) {
    string hex = uint2hex(255);
    assert(hex == "FF" && "255 converts to FF");
}

TEST(test_uint2hex_converts_multi_digit_45fa3) {
    string hex = uint2hex(0x45FA3);
    assert(hex == "45FA3" && "0x45FA3 converts to 45FA3");
}

TEST(test_uint2hex_returns_uppercase) {
    string hex = uint2hex(26);
    assert(hex == "1A" && "Returns uppercase A, not lowercase a");
}

TEST(test_uint2hex_converts_max_byte_255) {
    string hex = uint2hex(0xFF);
    assert(hex == "FF" && "Max byte 255 converts to FF");
}

TEST(test_uint2hex_converts_max_word_65535) {
    string hex = uint2hex(0xFFFF);
    assert(hex == "FFFF" && "Max word 65535 converts to FFFF");
}

TEST(test_uint2hex_roundtrip_with_hex2uint) {
    for (int v : {0, 1, 10, 15, 255, 4096, 65535}) {
        string hex = uint2hex(v);
        int back = hex2uint(hex);
        assert(back == v && "Roundtrip int -> hex -> int preserves value");
    }
}

#endif