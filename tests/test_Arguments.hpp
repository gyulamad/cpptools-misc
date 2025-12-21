#pragma once

#include "../TEST.hpp"
#include "../Arguments.hpp"

#include "../str_contains.hpp"

#ifdef TEST

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

#endif
