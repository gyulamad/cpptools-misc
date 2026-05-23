#pragma once

#include <string>
#include <vector>
#include "ucfirst.hpp"
#include "explode.hpp"
#include "regx_replace_all.hpp"

using namespace std;

#include <iostream>
/// @brief Convert a string to PascalCase (UpperCamelCase).
/// Replaces special characters (-, _, .) with nothing after capitalizing each word.
/// Preserves slashes for directory path contexts.
/// Examples:
///   "cpp-tui"         -> "CppTui"
///   "some_example"    -> "SomeExample"
/// @param str Input string (e.g., repo name, library name)
/// @return PascalCase formatted string suitable for general use
inline string toPascalCase(const string& str, const string& ptrn = "[-_\\.]+") {

    // Replace common separators (-, _, .) with a uniform space character
    string result = regx_replace_all(ptrn, str, " ");

    // Split by spaces into words
    vector<string> words = explode(" ", result);

    // Capitalize first letter of each non-empty word and concatenate
    string pascalCase;
    for (const string& word : words)
        pascalCase += ucfirst(word);

    return pascalCase;
}

