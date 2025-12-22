#pragma once

// NOTE: Do not compiles with -Ofast + -fsanitize=address
//       or use: __attribute__((optimize("O0")))
//       or: #pragma GCC optimize("O0")
// https://gcc.gnu.org/bugzilla/show_bug.cgi?id=105562#c2

#include <regex>
#include "ERROR.hpp"

using namespace std;

#pragma GCC optimize("O0")

/// @brief Replace all regex matches in string.
/// @param pattern regex pattern to match
/// @param str input string 
/// @param replace string to replace matches with 
/// @return string with all matches replaced
__attribute__((optimize("O0")))
inline string regx_replace_all(const string& pattern, const string& str, const string& replace) {
    if (pattern.empty()) throw ERROR("Regex pattern can not be empty"); // Explicitly throw for empty patterns
    regex r(pattern);
    return regex_replace(str, r, replace);
}
    
#pragma GCC reset_options
