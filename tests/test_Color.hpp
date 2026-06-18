#pragma once

#ifdef TEST

#include "../TEST.hpp"
#include "../Color.hpp"
#include "../str_contains.hpp"

class ColorTestHelper : public Color {
public:
    using Color::Color;
    int getR() const { return r; }
    int getG() const { return g; }
    int getB() const { return b; }
};

TEST(test_Color_default_constructor) {
    ColorTestHelper color;
    assert(color.getR() == 0 && "Default R should be 0");
    assert(color.getG() == 0 && "Default G should be 0");
    assert(color.getB() == 0 && "Default B should be 0");
}

TEST(test_Color_rgb_constructor) {
    ColorTestHelper color(255, 128, 64);
    assert(color.getR() == 255 && "R should be 255");
    assert(color.getG() == 128 && "G should be 128");
    assert(color.getB() == 64 && "B should be 64");
}

TEST(test_Color_hex_3_digit) {
    ColorTestHelper color("FFF");
    assert(color.getR() == 0xFF && "R from FFF should be 255");
    assert(color.getG() == 0xFF && "G from FFF should be 255");
    assert(color.getB() == 0xFF && "B from FFF should be 255");
}

TEST(test_Color_hex_3_digit_lowercase) {
    ColorTestHelper color("abc");
    assert(color.getR() == 0xAA && "R from abc should be 170");
    assert(color.getG() == 0xBB && "G from abc should be 187");
    assert(color.getB() == 0xCC && "B from abc should be 204");
}

TEST(test_Color_hex_6_digit) {
    ColorTestHelper color("FFAABB");
    assert(color.getR() == 0xFF && "R from FFAABB should be 255");
    assert(color.getG() == 0xAA && "G from FFAABB should be 170");
    assert(color.getB() == 0xBB && "B from FFAABB should be 187");
}

TEST(test_Color_hex_6_digit_lowercase) {
    ColorTestHelper color("ffaa55");
    assert(color.getR() == 255 && "R from ffaa55 should be 255");
    assert(color.getG() == 170 && "G from ffaa55 should be 170");
    assert(color.getB() == 85 && "B from ffaa55 should be 85");
}

TEST(test_Color_hex_with_hash_3_digit) {
    ColorTestHelper color("#FFF");
    assert(color.getR() == 255 && "R from #FFF should be 255");
    assert(color.getG() == 255 && "G from #FFF should be 255");
    assert(color.getB() == 255 && "B from #FFF should be 255");
}

TEST(test_Color_hex_with_hash_6_digit) {
    ColorTestHelper color("#1A2B3C");
    assert(color.getR() == 0x1A && "R from #1A2B3C should be 26");
    assert(color.getG() == 0x2B && "G from #1A2B3C should be 43");
    assert(color.getB() == 0x3C && "B from #1A2B3C should be 60");
}

TEST(test_Color_const_char_constructor) {
    ColorTestHelper color("FF00AA");
    assert(color.getR() == 255 && "R from const char* FF00AA should be 255");
    assert(color.getG() == 0 && "G from const char* FF00AA should be 0");
    assert(color.getB() == 170 && "B from const char* FF00AA should be 170");
}

TEST(test_Color_assignment_operator) {
    ColorTestHelper color1(255, 128, 64);
    ColorTestHelper color2;
    color2 = color1;
    assert(color2.getR() == 255 && "Assigned R should be 255");
    assert(color2.getG() == 128 && "Assigned G should be 128");
    assert(color2.getB() == 64 && "Assigned B should be 64");
}

TEST(test_Color_empty_string_returns_zero) {
    ColorTestHelper color("");
    assert(color.getR() == 0 && "Empty string R should remain 0");
    assert(color.getG() == 0 && "Empty string G should remain 0");
    assert(color.getB() == 0 && "Empty string B should remain 0");
}

TEST(test_Color_throws_on_invalid_2_digit_hex) {
    bool threw = false;
    try {
        ColorTestHelper color("FF");
    } catch (exception& e) {
        assert(str_contains(e.what(), "Invalid") && "Exception should mention invalid hex code");
        threw = true;
    }
    assert(threw && "Color should throw on 2-digit hex string");
}

TEST(test_Color_throws_on_invalid_4_digit_hex) {
    bool threw = false;
    try {
        ColorTestHelper color("FFFF");
    } catch (exception& e) {
        assert(str_contains(e.what(), "Invalid") && "Exception should mention invalid hex code");
        threw = true;
    }
    assert(threw && "Color should throw on 4-digit hex string");
}

TEST(test_Color_throws_on_invalid_5_digit_hex) {
    bool threw = false;
    try {
        ColorTestHelper color("FFFFF");
    } catch (exception& e) {
        assert(str_contains(e.what(), "Invalid") && "Exception should mention invalid hex code");
        threw = true;
    }
    assert(threw && "Color should throw on 5-digit hex string");
}

TEST(test_Color_throws_on_invalid_character_in_hex) {
    bool threw = false;
    try {
        ColorTestHelper color("GGHHII");
    } catch (exception& e) {
        threw = true;
    }
    assert(threw && "Color should throw on non-hex characters in 6-digit string");
}

#endif