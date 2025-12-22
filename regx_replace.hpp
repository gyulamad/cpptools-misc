#pragma once

// NOTE: Do not compiles with -Ofast + -fsanitize=address
//       or use: __attribute__((optimize("O0")))
//       or: #pragma GCC optimize("O0")
// https://gcc.gnu.org/bugzilla/show_bug.cgi?id=105562#c2

#include <regex>
#include "ERROR.hpp"

using namespace std;
using namespace regex_constants;

#pragma GCC optimize("O0")

/// @brief Replace first regex match in string.
/// @param pattern regex pattern to match 
/// @param str input string
/// @param replace string to replace matches with
/// @return string with first match replaced
__attribute__((optimize("O0")))
inline string regx_replace(const string& pattern, const string& str, const string& replace) {
    if (pattern.empty()) throw ERROR("Regex pattern can not be empty"); // Explicitly throw for empty patterns
    regex r(pattern);
    return regex_replace(str, r, replace, regex_constants::format_first_only);
}

#pragma GCC reset_options
