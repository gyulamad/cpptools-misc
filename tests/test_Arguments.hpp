#pragma once

#ifdef TEST

#include "../TEST.hpp"
#include "../Arguments.hpp"

#include "../str_contains.hpp"


TEST(test_Arguments_has_found) {
    vector<string> arg_strings = { "program", "--flag" };
    vector<char*> argv;
    for (string& str : arg_strings) {
        argv.push_back(const_cast<char*>(str.c_str()));
    }
    Arguments args(static_cast<int>(argv.size()), argv.data());
    args.addHelpByKey({ "flag", "" }, "Flag description");
    bool actual = args.has("flag");
    bool expected = true;
    assert(actual == expected && "Flag should be found");
}

TEST(test_Arguments_has_not_found) {
    vector<string> arg_strings = { "program", "--flag" };
    vector<char*> argv;
    for (string& str : arg_strings) {
        argv.push_back(const_cast<char*>(str.c_str()));
    }
    Arguments args(static_cast<int>(argv.size()), argv.data());
    args.addHelpByKey({ "flag", "" }, "Flag description");
    args.addHelpByKey({"missing", ""}, "We expect but user is not providing this");
    bool actual = args.has("missing");
    bool expected = false;
    assert(actual == expected && "Missing flag should not be found");
}

TEST(test_Arguments_indexOf_found) {
    vector<string> arg_strings = { "program", "--flag" };
    vector<char*> argv;
    for (string& str : arg_strings) {
        argv.push_back(const_cast<char*>(str.c_str()));
    }
    Arguments args(static_cast<int>(argv.size()), argv.data());
    args.addHelpByKey({ "flag", "" }, "Flag description");
    long int actual = args.indexOf("flag");
    long int expected = 1;
    assert(actual == expected && "Index of flag should be 1");
}

TEST(test_Arguments_indexOf_not_found) {
    vector<string> arg_strings = { "program", "--flag" };
    vector<char*> argv;
    for (string& str : arg_strings) {
        argv.push_back(const_cast<char*>(str.c_str()));
    }
    Arguments args(static_cast<int>(argv.size()), argv.data());
    args.addHelpByKey({ "flag", "" }, "Flag description");
    long int actual = args.indexOf("missing");
    long int expected = -1;
    assert(actual == expected && "Index of missing flag should be -1");
}

// ----- Tests for short flags -----

// Helper to create Arguments from a vector of strings
Arguments createArgs(const vector<string>& input) {
    vector<char*> argv;
    for (auto& s : input) argv.push_back(const_cast<char*>(s.c_str()));
    return Arguments(static_cast<int>(argv.size()), argv.data());
}

// Test has() with short flag
TEST(test_Arguments_has_short_flag) {
    Arguments args = createArgs({"program", "-v"});
    args.addHelpByKey({ "verbose", "v" }, "Verbose description");
    bool actual = args.has(pair("verbose", "v"));
    assert(actual == true && "Short flag -v should be detected");
}

// Test has() with missing short flag
TEST(test_Arguments_has_short_flag_missing) {
    Arguments args = createArgs({"program"});
    args.addHelpByKey({ "verbose", "v" }, "Verbose description");
    bool actual = args.has(pair("verbose", "v"));
    assert(actual == false && "Missing short flag -v should return false");
}

// Test get<bool> with short flag
TEST(test_Arguments_get_bool_short_flag) {
    Arguments args = createArgs({"program", "-v"});
    args.addHelpByKey({ "verbose", "v" }, "Verbose description");
    bool actual = args.getByKey<bool>(pair("verbose", "v"));
    assert(actual == true && "Short flag -v should return true for bool");
}

// Test get<bool> with short flag and default value
TEST(test_Arguments_get_bool_short_flag_default) {
    Arguments args = createArgs({"program"});
    args.addHelpByKey({ "verbose", "v" }, "Verbose description");
    bool actual = args.getoptByKey<bool>(Arguments::Key("verbose", "v"), false);
    assert(actual == false && "Missing short flag -v should return default false");
}

// Test get<int> with short flag and value
TEST(test_Arguments_get_int_short_flag_value) {
    Arguments args = createArgs({"program", "-c", "42"});
    args.addHelpByKey({ "count", "c" }, "Count description");
    int actual = args.getByKey<int>(pair("count", "c"));
    assert(actual == 42 && "Short flag -c with value 42 should return 42");
}

// Test get<int> with short flag missing value
TEST(test_Arguments_get_int_short_flag_missing_value) {
    Arguments args = createArgs({"program", "-c"});
    args.addHelpByKey({ "count", "c" }, "Test argument");
    bool thrown = false;
    string what;
    try {
        args.getByKey<int>(pair("count", "c"));
    } catch (exception& e) {
        thrown = true;
        what = e.what();
        assert(str_contains(what, "Missing value for argument: c") && "Exception message should indicate missing value for -c");
    }
    assert(thrown && "Short flag -c without value should throw");
}

// Test get<string> with short flag and value
TEST(test_Arguments_get_string_short_flag_value) {
    Arguments args = createArgs({"program", "-f", "data.txt"});
    args.addHelpByKey({ "file", "f" }, "File description");
    string actual = args.getByKey<string>(pair("file", "f"));
    assert(actual == "data.txt" && "Short flag -f with value data.txt should return data.txt");
}

// Test get<int> with short flag and default value
TEST(test_Arguments_get_int_short_flag_default) {
    Arguments args = createArgs({"program"});
    args.addHelpByKey({ "count", "c" }, "Count description");
    int actual = args.getoptByKey<int>(Arguments::Key("count", "c"), 10);
    assert(actual == 10 && "Missing short flag -c should return default 10");
}

// Test get<int> with both long and short flags present (long takes precedence)
TEST(test_Arguments_get_int_short_and_long_flags) {
    Arguments args = createArgs({"program", "--count", "100", "-c", "42"});
    args.addHelpByKey({ "count", "c" }, "Count description");
    int actual = args.getByKey<int>(pair("count", "c"));
    assert(actual == 100 && "Long flag --count should take precedence over -c");
}

// Test hasHelp when key is NOT found (line 147: return false)
TEST(test_Arguments_hasHelp_not_found) {
    Arguments args = createArgs({"program", "--verbose"});
    args.addHelpByKey({ "verbose", "v" }, "Verbose description");
    bool actual = args.hasHelp("nonexistent");
    assert(actual == false && "hasHelp should return false for non-existent key");
}

// Test help(const Key& k) method
TEST(test_Arguments_help_key) {
    Arguments args = createArgs({"program"});
    args.addHelpByKey({ "verbose", "v" }, "Verbose description");
    args.addHelpByKey({ "count", "c" }, "Count description");
    string actual = args.help(Arguments::Key("verbose", "v"));
    assert(str_contains(actual, "--verbose") && "Help should contain --verbose");
    assert(str_contains(actual, "-v") && "Help should contain -v");
    assert(str_contains(actual, "Verbose description") && "Help should contain description");
}

// Test help(const Key& k) with empty key (should show all)
TEST(test_Arguments_help_key_empty) {
    Arguments args = createArgs({"program"});
    args.addHelpByKey({ "verbose", "v" }, "Verbose description");
    args.addHelpByKey({ "count", "c" }, "Count description");
    string actual = args.help(Arguments::Key("", ""));
    assert(str_contains(actual, "--verbose") && "Help should contain --verbose");
    assert(str_contains(actual, "--count") && "Help should contain --count");
}

// Test checkHelp throws when help is not provided
TEST(test_Arguments_checkHelp_throws) {
    Arguments args = createArgs({"program", "--unknown"});
    bool thrown = false;
    string what;
    try {
        args.get<string>("unknown");
    } catch (exception& e) {
        thrown = true;
        what = e.what();
        assert(str_contains(what, "Help is not provided for argument '--unknown'") && "Exception should mention missing help");
    }
    assert(thrown && "get should throw when help is not provided");
}

// Test get<T> with '=' character (prefixed_key += equal_to branch)
TEST(test_Arguments_get_with_equals) {
    Arguments args = createArgs({"program", "--count=42"});
    args.addHelpByKey({ "count", "c" }, "Count description");
    int actual = args.get<int>("count");
    assert(actual == 42 && "get should parse value after =");
}

// Test get<T> with '=' character for short flag
TEST(test_Arguments_get_short_with_equals) {
    Arguments args = createArgs({"program", "-c=42"});
    args.addHelpByKey({ "count", "c" }, "Count description");
    int actual = args.getByKey<int>(Arguments::Key("count", "c"));
    assert(actual == 42 && "getBykey should parse value after = for short flag");
}

// Test get<T> throws when value is missing after '='
TEST(test_Arguments_get_equals_missing_value) {
    Arguments args = createArgs({"program", "--count="});
    args.addHelpByKey({ "count", "c" }, "Count description");
    bool thrown = false;
    string what;
    try {
        args.get<int>("count");
    } catch (exception& e) {
        thrown = true;
        what = e.what();
        assert(str_contains(what, "Missing value for argument: count") && "Exception should mention missing value");
    }
    assert(thrown && "get should throw when value is empty after =");
}

// Test addHelpByKey throws when help already exists (line 125)
TEST(test_Arguments_addHelpByKey_duplicate_throws) {
    Arguments args = createArgs({"program", "--verbose"});
    args.addHelpByKey({ "verbose", "v" }, "Verbose description");
    bool thrown = false;
    string what;
    try {
        args.addHelpByKey({ "verbose", "v" }, "Duplicate description");
    } catch (exception& e) {
        thrown = true;
        what = e.what();
        assert(str_contains(what, "Help already set for key: verbose") && "Exception should mention duplicate key");
    }
    assert(thrown && "addHelpByKey should throw when help already exists");
}

// Test getByKey throws when both keys missing (line 260)
TEST(test_Arguments_getByKey_both_missing_throws) {
    Arguments args = createArgs({"program"});
    args.addHelpByKey({ "verbose", "v" }, "Verbose description");
    bool thrown = false;
    string what;
    try {
        args.getByKey<int>(Arguments::Key("missing", "m"));
    } catch (exception& e) {
        thrown = true;
        what = e.what();
        assert(str_contains(what, "Error:") && "Exception should contain Error prefix");
    }
    assert(thrown && "getByKey should throw when both keys are missing");
}

// Test get<T> throws when argument missing (line 164)
TEST(test_Arguments_get_missing_argument_throws) {
    Arguments args = createArgs({"program"});
    args.addHelpByKey({ "verbose", "v" }, "Verbose description");
    bool thrown = false;
    string what;
    try {
        args.get<int>("missing");
    } catch (exception& e) {
        thrown = true;
        what = e.what();
        assert(str_contains(what, "Error:") && "Exception should contain Error prefix");
    }
    assert(thrown && "get should throw when argument is missing");
}

// Test get<T> with value in next argument (line 188)
TEST(test_Arguments_get_value_in_next_arg) {
    Arguments args = createArgs({"program", "--count", "42"});
    args.addHelpByKey({ "count", "c" }, "Count description");
    int actual = args.get<int>("count");
    assert(actual == 42 && "get should parse value from next argument");
}

// Test get<T> throws when no value available after flag (line 190)
TEST(test_Arguments_get_no_value_after_flag_throws) {
    Arguments args = createArgs({"program", "--count"});
    args.addHelpByKey({ "count", "c" }, "Count description");
    bool thrown = false;
    string what;
    try {
        args.get<int>("count");
    } catch (exception& e) {
        thrown = true;
        what = e.what();
        assert(str_contains(what, "Error:") && "Exception should contain Error prefix");
    }
    assert(thrown && "get should throw when no value after flag");
}

// Test get<T> throws when help provided but argument not found (line 164)
TEST(test_Arguments_get_help_provided_arg_not_found) {
    Arguments args = createArgs({"program"});
    args.addHelpByKey({ "verbose", "v" }, "Verbose description");
    bool thrown = false;
    string what;
    try {
        args.get<int>("verbose");
    } catch (exception& e) {
        thrown = true;
        what = e.what();
        assert(str_contains(what, "Error:") && "Exception should contain Error prefix");
    }
    assert(thrown && "get should throw when argument is not found even though help is provided");
}

#endif
