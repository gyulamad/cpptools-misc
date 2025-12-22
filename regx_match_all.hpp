#pragma once

// NOTE: Do not compiles with -Ofast + -fsanitize=address
//       or use: __attribute__((optimize("O0")))
//       or: #pragma GCC optimize("O0")
// https://gcc.gnu.org/bugzilla/show_bug.cgi?id=105562#c2

#include <regex>

using namespace std;
using namespace regex_constants;

#pragma GCC optimize("O0")

__attribute__((optimize("O0")))
int regx_match_all(
    const string& pattern,
    const string& str,
    vector<string>* matches = nullptr,
    syntax_option_type flags = ECMAScript
) {
    // Check for empty pattern
    if (pattern.empty()) {
        if (matches != nullptr) {
            matches->clear();
            for (size_t i = 0; i <= str.size(); ++i)
                matches->push_back(""); // Empty pattern matches every position
        }
        return static_cast<int>(str.size() + 1); // Return number of matches (positions)
    }

    // Regular regex matching
    try {
        regex r(pattern, flags);
        sregex_iterator begin(str.begin(), str.end(), r), end;
        int match_count = 0;

        if (matches != nullptr) {
            matches->clear();
            for (sregex_iterator it = begin; it != end; ++it) {
                smatch m = *it;
                matches->push_back(m[0].str()); // Add the full match
                for (size_t i = 1; i < m.size(); ++i)
                    matches->push_back(m[i].str()); // Add each capture group
                ++match_count;
            }
        }

        return match_count;
    } catch (const regex_error&) {
        // Handle invalid regex patterns
        if (matches != nullptr) matches->clear();
        return 0; // No matches for invalid patterns
    }
}
    
#pragma GCC reset_options
