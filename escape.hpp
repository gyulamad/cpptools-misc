#pragma once

#include <string>

using namespace std;

inline string escape(const string& input, const string& chars = "\\$\"'`\n\r\t", const string& esc = "\\") {
    string result;
    for (size_t i = 0; i < input.size(); ++i) {
        char c = input[i];
        bool needs_escape = (chars.find(c) != string::npos);
        bool is_escaped = false;

        // Check if the previous character in the result is an escape
        if (!result.empty() && result.back() == esc[0]) {
            // Count consecutive escape characters from the end of the result
            size_t escape_count = 0;
            for (ssize_t j = result.size() - 1; j >= 0 && result[j] == esc[0]; --j) escape_count++;

            // Odd number of escapes means this character is already escaped
            is_escaped = (escape_count % 2 == 1);
        }

        if (needs_escape && !is_escaped) result += esc;
        result += c;
    }
    return result;
}
