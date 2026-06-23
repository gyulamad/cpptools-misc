#pragma once

#ifdef TEST

#include "../TEST.hpp"
#include "../base64.hpp"


TEST(test_base64_encode_simple) {
    assert(base64_encode("Hello") == "SGVsbG8=");
}

TEST(test_base64_encode_empty_string) {
    assert(base64_encode("").empty());
}

TEST(test_base64_encode_no_padding_needed) {
    // 3 bytes encode to exactly 4 base64 chars, no padding
    assert(base64_encode("Man") == "TWFu");
}

TEST(test_base64_encode_two_bytes_one_padding) {
    // 2 bytes produce one '=' padding
    assert(base64_encode("He") == "SGU=");
}

TEST(test_base64_encode_single_byte_two_paddings) {
    // 1 byte produces two '=' paddings
    assert(base64_encode("M") == "TQ==");
}

TEST(test_base64_decode_simple) {
    assert(base64_decode("SGVsbG8=") == "Hello");
}

TEST(test_base64_decode_empty_string) {
    assert(base64_decode("").empty());
}

TEST(test_base64_decode_no_padding_needed) {
    // 3 bytes encode to exactly 4 base64 chars, no padding
    assert(base64_decode("TWFu") == "Man");
}

TEST(test_base64_decode_two_bytes_one_padding) {
    assert(base64_decode("SGU=") == "He");
}

TEST(test_base64_decode_single_byte_two_paddings) {
    assert(base64_decode("TQ==") == "M");
}

TEST(test_base64_decode_with_newlines) {
    string encoded = "SGVsbG8g\nV29ybGQ="; // "Hello World" with newline
    assert(base64_decode(encoded) == "Hello World");
}

TEST(test_base64_decode_non_strict_ignores_invalid) {
    // Non-strict mode should silently discard invalid characters
    string dirty = "SGVs bG8="; // contains a space - removing it gives SGVsbG8= which is "Hello"
    assert(base64_decode(dirty, false) == "Hello");
}

TEST(test_base64_decode_strict_throws_on_invalid) {
    bool threw = false;
    try {
        base64_decode("SGV!sbG8=", true); // '!' is invalid in strict mode
    } catch (exception& e) {
        assert(str_contains(e.what(), "Invalid base64 character") && "Exception should mention invalid base64");
        threw = true;
    }
    assert(threw && "Strict decode should throw on invalid characters");
}

TEST(test_base64_encode_decode_roundtrip) {
    string original = "The quick brown fox jumps over the lazy dog!";
    string encoded = base64_encode(original);
    string decoded = base64_decode(encoded);
    assert(decoded == original && "Round-trip encode/decode should preserve data");
}

TEST(test_base64_encode_binary_data) {
    // Test with binary-like content (all 256 byte values)
    string binary;
    for (int i = 0; i < 256; i++) {
        binary += (char)i;
    }
    string encoded = base64_encode(binary);
    string decoded = base64_decode(encoded);
    assert(decoded == binary && "Binary data round-trip should preserve all byte values");
}

#endif
