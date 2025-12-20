#pragma once

#include "str_replace.hpp"
#include <string>

using namespace std;

string highlight_compiler_outputs(string output) {
    output = str_replace({
        { "error:", F(F_ERROR, "error:") },
        { "note:", F(F_NOTE, "note:") },
        { "warning:", F(F_WARNING, "warning:") },
    }, output);
    return output;
}
