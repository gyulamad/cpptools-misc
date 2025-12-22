#pragma once

#include "remove_extension.hpp"
#include "str_starts_with.hpp"

using namespace std;

string replace_extension(const string& fname, const string& newext) {
    return remove_extension(fname) + (!newext.empty() ? str_starts_with(newext, ".") ? newext : ("." + newext) : "");
}
