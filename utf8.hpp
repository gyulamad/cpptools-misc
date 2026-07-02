#pragma once

#include <cstddef>
#include <cwchar>
#include <string>
#include <vector>

using namespace std;

// Returns the number of terminal columns needed to display a UTF-8 string.
// Uses wcswidth() for accurate measurement, handling multibyte and wide (CJK) chars.
inline int utf8_display_width(const string& str) {
    if (str.empty()) return 0;

    // Convert UTF-8 to wide string
    const size_t len = mbstowcs(nullptr, str.c_str(), 0);
    if (len == static_cast<size_t>(-1)) {
        // Fallback: treat each byte as one column (ASCII or invalid encoding)
        return static_cast<int>(str.size());
    }

    wstring wstr(len, L'\0');
    mbstowcs(&wstr[0], str.c_str(), len);

    const int width = wcswidth(wstr.c_str(), wstr.size());
    if (width < 0) {
        // Fallback: treat each byte as one column for non-displayable chars
        return static_cast<int>(str.size());
    }

    return width;
}

// Returns the display width of a single UTF-8 character starting at str[pos].
// Also sets `bytes_consumed` to the number of bytes that make up this character.
inline int utf8_char_display_width_at(const string& str, size_t pos, size_t& bytes_consumed) {
    if (pos >= str.size()) {
        bytes_consumed = 0;
        return 0;
    }

    // Determine byte length of the UTF-8 character at position pos
    unsigned char ch = static_cast<unsigned char>(str[pos]);
    if (ch < 0x80) {
        bytes_consumed = 1;
    } else if ((ch & 0xE0) == 0xC0) {
        bytes_consumed = 2;
    } else if ((ch & 0xF0) == 0xE0) {
        bytes_consumed = 3;
    } else if ((ch & 0xF8) == 0xF0) {
        bytes_consumed = 4;
    } else {
        // Invalid UTF-8 start byte — skip one byte
        bytes_consumed = 1;
    }

    // Extract the character and measure its display width
    if (pos + bytes_consumed > str.size()) {
        bytes_consumed = str.size() - pos;
    }

    string charStr = str.substr(pos, bytes_consumed);
    return utf8_display_width(charStr);
}

// Advances pos by one display character and returns its display width.
inline int utf8_nextch(const string& str, size_t& pos) {
    if (pos >= str.size()) return 0;

    size_t bytes_consumed = 0;
    int w = utf8_char_display_width_at(str, pos, bytes_consumed);
    pos += bytes_consumed;
    return w;
}

// Returns the display width of a UTF-8 substring [start, start + byteLen).
inline int utf8_substr_display_width(const string& str, size_t start, size_t byteLen) {
    if (start >= str.size()) return 0;
    if (start + byteLen > str.size()) {
        byteLen = str.size() - start;
    }
    return utf8_display_width(str.substr(start, byteLen));
}

// Finds the byte offset where a UTF-8 string should be truncated so that its
// display width does not exceed `max_width`. Returns the byte length (from pos).
inline size_t utf8_trunc_to_display_width(const string& str, size_t pos, int max_width) {
    if (pos >= str.size() || max_width <= 0) return 0;

    int accumulated = 0;
    size_t end = pos;
    while (end < str.size()) {
        size_t bytes_consumed = 0;
        int w = utf8_char_display_width_at(str, end, bytes_consumed);
        if (accumulated + w > max_width) break;
        accumulated += w;
        end += bytes_consumed;
    }

    return end - pos;
}

// Splits a UTF-8 string into chunks that fit within `max_display_width` columns.
inline vector<string> utf8_split_by_display_width(const string& str, int max_display_width) {
    vector<string> result;
    if (str.empty()) return result;

    size_t start = 0;
    while (start < str.size()) {
        size_t byteLen = utf8_trunc_to_display_width(str, start, max_display_width);
        if (byteLen == 0) byteLen = 1; // At least consume one character to avoid infinite loop
        result.push_back(str.substr(start, byteLen));
        start += byteLen;
    }

    return result;
}
