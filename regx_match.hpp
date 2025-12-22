#pragma once

// NOTE: Do not compiles with -Ofast + -fsanitize=address
//       or use: __attribute__((optimize("O0")))
//       or: #pragma GCC optimize("O0")
// https://gcc.gnu.org/bugzilla/show_bug.cgi?id=105562#c27

#include <regex>

using namespace std;
using namespace regex_constants;

#pragma GCC optimize("O0")

__attribute__((optimize("O0")))
int regx_match(
    const string& pattern, 
    const string& str, 
    vector<string>* matches = nullptr,
    syntax_option_type flags = ECMAScript  // Add flags parameter
) {
    regex r(pattern, flags);
    smatch m;
    if (regex_search(str, m, r)) {
        if (matches != nullptr) {
            // Clear the vector before adding more matches
            matches->clear();
            for (unsigned int i = 0; i < m.size(); i++)
                matches->push_back(m[i].str());
        }
        return 1;
    }
    return 0;
}

#pragma GCC reset_options
