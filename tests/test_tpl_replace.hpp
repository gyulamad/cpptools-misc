#pragma once

#include "../TEST.hpp"
#include "../tpl_replace.hpp"

#ifdef TEST



TEST(test_tpl_replace_single_replacement) {
    string input = "Hello {{name}}!";
    string expected = "Hello John!";
    string actual = tpl_replace("{{name}}", "John", input);
    assert(actual == expected && "test_tpl_replace_single_replacement failed");
}

TEST(test_tpl_replace_multiple_replacements) {
    string input = "{{greeting}}, {{name}}! Welcome to {{place}}.";
    map<string, string> replacements = {
        {"{{greeting}}", "Hi"},
        {"{{name}}", "Alice"},
        {"{{place}}", "Wonderland"}
    };
    string expected = "Hi, Alice! Welcome to Wonderland.";
    string actual = tpl_replace(replacements, input);
    assert(actual == expected && "test_tpl_replace_multiple_replacements failed");
}

TEST(test_tpl_replace_no_placeholder_in_template) {
    bool thrown = false;
    try {
        string input = "Hello World!";
        tpl_replace("{{name}}", "John", input);
    } catch (const exception&) {
        thrown = true;
    }
    assert(thrown && "test_tpl_replace_no_placeholder_in_template failed");
}

TEST(test_tpl_replace_missing_replacement_value) {
    bool thrown = false;
    try {
        string input = "Hello {{name}} and {{friend}}!";
        map<string, string> replacements = {{"{{name}}", "John"}};
        tpl_replace(replacements, input);
    } catch (const exception&) {
        thrown = true;
    }
    assert(thrown && "test_tpl_replace_missing_replacement_value failed");
}

TEST(test_tpl_replace_invalid_placeholder_regex) {
    bool thrown = false;
    try {
        string input = "Hello [name]!";
        tpl_replace("[name]", "John", input, "\\{\\{[^}]+\\}\\}");
    } catch (const exception&) {
        thrown = true;
    }
    assert(thrown && "test_tpl_replace_invalid_placeholder_regex failed");
}

TEST(test_tpl_replace_empty_template) {
    bool thrown = false;
    try {
        string input = "";
        string expected = "";
        string actual = tpl_replace("{{name}}", "John", input);
    } catch (const exception&) {
        thrown = true;
    }
    assert(thrown && "test_tpl_replace_empty_template failed");
}

TEST(test_tpl_replace_empty_replacement_value) {
    string input = "Hello {{name}}!";
    string expected = "Hello !";
    string actual = tpl_replace("{{name}}", "", input);
    assert(actual == expected && "test_tpl_replace_empty_replacement_value failed");
}

#endif
