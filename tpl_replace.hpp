#pragma once

#include <string>
#include <map>
#include "regx_match.hpp"
#include "ERROR.hpp"
#include "str_cut_end.hpp"
#include "str_contains.hpp"
#include "regx_match_all.hpp"
#include "array_keys.hpp"
#include "in_array.hpp"
#include "implode.hpp"
#include "str_replace.hpp"

using namespace std;


string tpl_replace(const map<string, string>& replacements, const string& template_str, const string& placeholder_ptrn = "\\{\\{[^}]+\\}\\}") {
    
    // Check if all provided replacements exist in template        
    for (const auto& pair : replacements) {
        if (!regx_match("^" + placeholder_ptrn + "$", pair.first))
            throw ERROR(
                "Replacement variable provided that does not match to the placeholder regex: " + pair.first + 
                ", pattern: ^" + placeholder_ptrn + "$" + 
                "\nTemplate: " + str_cut_end(template_str));
        if (!str_contains(template_str, pair.first))
            throw ERROR(
                "Replacement variable provided for a template that is not having this placeholder: " + pair.first + 
                "\nTemplate: " + str_cut_end(template_str));
    }
    
    // Check if all template expected variable are provided
    vector<string> matches;
    regx_match_all(placeholder_ptrn, template_str, &matches);
    vector<string> variables = array_keys(replacements);
    for (const string& match: matches)
        if (!in_array(match, variables))
            throw ERROR(
                "Replacement value is not provided for the following placeholder(s): " + 
                implode(", ", matches) + 
                "\nTemplate: " + str_cut_end(template_str));

    // If validation passes, use the existing str_replace function
    return str_replace(replacements, template_str);
}

// Overload for single replacement
string tpl_replace(const string& from, const string& to, const string& subject, const string& placeholder_ptrn = "\\{\\{[^}]+\\}\\}") {
    return tpl_replace({{from, to}}, subject, placeholder_ptrn);
}
    
