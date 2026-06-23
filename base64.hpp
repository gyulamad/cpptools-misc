#pragma once

#include <string>
#include "ERROR.hpp"

using namespace std;

static const string _base64_table = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

inline bool _is_base64(char c) {
    return (c >= 'A' && c <= 'Z') ||
           (c >= 'a' && c <= 'z') ||
           (c >= '0' && c <= '9') ||
           (c == '+') || (c == '/') || (c == '=');
}

inline char _base64_value(char c) {
    if (c >= 'A' && c <= 'Z') return (char)(c - 'A');
    if (c >= 'a' && c <= 'z') return (char)(26 + c - 'a');
    if (c >= '0' && c <= '9') return (char)(52 + c - '0');
    if (c == '+') return 62;
    if (c == '/') return 63;
    return 0;
}

inline string base64_encode(const string& data) {
    // PHP-like base64_encode: encodes binary data to Base64 (RFC 2045).
    // Returns the encoded string with '=' padding. No line wrapping by default.
    if (data.empty()) return "";

    string result;
    size_t len = data.size();
    size_t i = 0;

    while (i + 3 <= len) {
        unsigned char b0 = (unsigned char)data[i++];
        unsigned char b1 = (unsigned char)data[i++];
        unsigned char b2 = (unsigned char)data[i++];

        result += _base64_table[(b0 >> 2) & 0x3F];
        result += _base64_table[((b0 << 4) | (b1 >> 4)) & 0x3F];
        result += _base64_table[((b1 << 2) | (b2 >> 6)) & 0x3F];
        result += _base64_table[b2 & 0x3F];
    }

    if (i < len) {
        unsigned char b0 = (unsigned char)data[i++];

        if (i < len) {
            unsigned char b1 = (unsigned char)data[i++];
            result += _base64_table[(b0 >> 2) & 0x3F];
            result += _base64_table[((b0 << 4) | (b1 >> 4)) & 0x3F];
            result += _base64_table[((b1 << 2)) & 0x3F];
        } else {
            result += _base64_table[(b0 >> 2) & 0x3F];
            result += _base64_table[(b0 << 4) & 0x3F];
            result += '=';
        }

        result += '=';
    }

    return result;
}

inline string base64_decode(const string& encoded, bool strict = false) {
    // PHP-like base64_decode: decodes MIME Base64 (RFC 2045) data.
    // In strict mode, throws on invalid characters outside the Base64 alphabet.
    if (encoded.empty()) return "";

    string result;
    int val = 0;
    int bits_collected = 0;

    for (char c : encoded) {
        if (c == '\n' || c == '\r') continue; // skip line breaks

        if (!strict && !_is_base64(c)) continue; // non-strict: silently discard invalid chars

        if (!_is_base64(c)) {
            throw ERROR("Invalid base64 character: " + string(1, c));
        }

        if (c == '=') break; // padding marker - stop processing

        val = (val << 6) | _base64_value(c);
        bits_collected += 6;

        if (bits_collected >= 8) {
            bits_collected -= 8;
            result += (char)((val >> bits_collected) & 0xFF);
        }
    }

    return result;
}
