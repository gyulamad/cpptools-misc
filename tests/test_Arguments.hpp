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

// Test get<T> with value in next argument for non-bool types (line 188)
TEST(test_Arguments_get_value_in_next_arg_non_bool) {
    Arguments args = createArgs({"program", "--verbose", "42"});
    args.addHelpByKey({ "verbose", "v" }, "Verbose description");
    int actual = args.get<int>("verbose");
    assert(actual == 42 && "get should parse value from next argument for non-bool");
}

// Test get<T> throws when no value available after flag (line 190)
TEST(test_Arguments_get_no_value_after_flag_throws_non_bool) {
    Arguments args = createArgs({"program", "--verbose"});
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
    assert(thrown && "get should throw when no value after flag for non-bool");
}

// Test getByKey throws when both keys are missing (line 198)
TEST(test_Arguments_getByKey_both_missing_throws_extra) {
    Arguments args = createArgs({"program"});
    args.addHelpByKey({ "verbose", "v" }, "Verbose description");
    bool thrown = false;
    string what;
    try {
        args.getByKey<int>(Arguments::Key("missing1", "m1"));
    } catch (exception& e) {
        thrown = true;
        what = e.what();
        assert(str_contains(what, "Error:") && "Exception should contain Error prefix");
    }
    assert(thrown && "getByKey should throw when both keys are missing");
}

// Test getopt for non-bool types (lines 203-205)
TEST(test_Arguments_getopt_non_bool) {
    Arguments args = createArgs({"program", "--count", "123"});
    args.addHelpByKey({ "count", "c" }, "Count description");
    int actual = args.getopt<int>("count", 10);
    assert(actual == 123 && "getopt should return value when argument present");
}

TEST(test_Arguments_getopt_non_bool_default) {
    Arguments args = createArgs({"program"});
    args.addHelpByKey({ "count", "c" }, "Count description");
    int actual = args.getopt<int>("count", 10);
    assert(actual == 10 && "getopt should return default when argument missing");
}

// Test getoptByKey for non-bool types
TEST(test_Arguments_getoptByKey_non_bool) {
    Arguments args = createArgs({"program", "--count", "456"});
    args.addHelpByKey({ "count", "c" }, "Count description");
    int actual = args.getoptByKey<int>(Arguments::Key("count", "c"), 10);
    assert(actual == 456 && "getoptByKey should return value when argument present");
}

TEST(test_Arguments_getoptByKey_non_bool_default) {
    Arguments args = createArgs({"program"});
    args.addHelpByKey({ "count", "c" }, "Count description");
    int actual = args.getoptByKey<int>(Arguments::Key("count", "c"), 10);
    assert(actual == 10 && "getoptByKey should return default when argument missing");
}

// Test get<size_t> positional getter (lines 213-218)
TEST(test_Arguments_get_positional) {
    Arguments args = createArgs({"program", "arg1", "arg2", "arg3"});
    args.addHelp(0, "first", "First positional argument");
    args.addHelp(1, "second", "Second positional argument");
    args.addHelp(2, "third", "Third positional argument");
    
    string actual1 = args.get<string>(0);
    assert(actual1 == "arg1" && "get positional should return first argument");
    
    string actual2 = args.get<string>(1);
    assert(actual2 == "arg2" && "get positional should return second argument");
    
    string actual3 = args.get<string>(2);
    assert(actual3 == "arg3" && "get positional should return third argument");
}

// Test getopt with positional getter (lines 221-224)
TEST(test_Arguments_getopt_positional) {
    Arguments args = createArgs({"program", "arg1", "arg2"});
    args.addHelp(0, "first", "First positional argument");
    args.addHelp(1, "second", "Second positional argument");
    args.addHelp(2, "third", "Third positional argument");
    
    string actual1 = args.getopt<string>(0, "default");
    assert(actual1 == "arg1" && "getopt positional should return value when present");
    
    string actual2 = args.getopt<string>(5, "default");
    assert(actual2 == "default" && "getopt positional should return default when missing");
}

// Test help() function (no-arg version) (lines 226-236)
TEST(test_Arguments_help_no_args) {
    Arguments args = createArgs({"program", "--verbose", "42"});
    args.addHelp(0, "command", "Command description");
    args.addHelpByKey({ "verbose", "v" }, "Verbose description");
    
    string actual = args.help();
    assert(str_contains(actual, "program") && "help() should contain program name");
    assert(str_contains(actual, "command") && "help() should contain positional help");
    assert(str_contains(actual, "--verbose") && "help() should contain option help");
}

// Test help(size_t at) function (lines 238-240)
TEST(test_Arguments_help_at) {
    Arguments args = createArgs({"program"});
    args.addHelp(0, "command", "Command description");
    
    string actual = args.help(0);
    assert(str_contains(actual, "command") && "help(at) should contain name");
    assert(str_contains(actual, "Command description") && "help(at) should contain description");
}

// Test getBool functions (lines 135-154)
TEST(test_Arguments_getBool) {
    Arguments args = createArgs({"program", "--verbose"});
    args.addHelpByKey({ "verbose", "v" }, "Verbose description");
    
    bool actual = args.getBool("verbose");
    assert(actual == true && "getBool should return true when flag present");
}

TEST(test_Arguments_getBool_default) {
    Arguments args = createArgs({"program"});
    args.addHelpByKey({ "verbose", "v" }, "Verbose description");
    
    bool actual = args.getBool("verbose", false);
    assert(actual == false && "getBool with default should return default when flag missing");
}

TEST(test_Arguments_getBool_positional) {
    Arguments args = createArgs({"program", "true"});
    args.addHelp(0, "flag", "Flag description");
    
    bool actual = args.getBool(0);
    assert(actual == true && "getBool positional should parse bool value");
}

TEST(test_Arguments_getBool_positional_default) {
    Arguments args = createArgs({"program"});
    args.addHelp(0, "flag", "Flag description");
    
    bool actual = args.getBool(0, false);
    assert(actual == false && "getBool positional with default should return default");
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

// Test getBool(size_t at) throws when positional argument is missing (line 145)
TEST(test_Arguments_getBool_positional_missing_throws) {
    Arguments args = createArgs({"program"});
    args.addHelp(0, "first", "First positional argument");
    bool thrown = false;
    string what;
    try {
        args.getBool(0);
    } catch (exception& e) {
        thrown = true;
        what = e.what();
        assert(str_contains(what, "Missing argument at: 0") && "Exception should mention missing argument");
    }
    assert(thrown && "getBool should throw when positional argument is missing");
}

// Test get<T> with value in next argument when arg is not simple flag (line 186)
TEST(test_Arguments_get_value_in_next_arg_short_flag) {
    Arguments args = createArgs({"program", "--count", "42"});
    args.addHelpByKey({ "count", "c" }, "Count description");
    int actual = args.get<int>("count");
    assert(actual == 42 && "get should parse value from next argument for short flag");
}

// Test get<T> throws when no value available after flag (line 188)
TEST(test_Arguments_get_no_value_after_flag_throws_short) {
    Arguments args = createArgs({"program", "--count"});
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
    assert(thrown && "get should throw when no value after short flag");
}

// Test getByKey throws when both keys are missing (line 196)
TEST(test_Arguments_getByKey_both_missing) {
    Arguments args = createArgs({"program"});
    args.addHelpByKey({ "verbose", "v" }, "Verbose description");
    bool thrown = false;
    string what;
    try {
        args.getByKey<int>(Arguments::Key("missing", "m"));
    } catch (exception& e) {
        thrown = true;
        what = e.what();
        assert(str_contains(what, "Missing argument:") && "Exception should mention missing argument");
    }
    assert(thrown && "getByKey should throw when both keys are missing");
}

// Test get<T> positional throws when argument is missing (line 214)
TEST(test_Arguments_get_positional_missing_throws) {
    Arguments args = createArgs({"program"});
    args.addHelp(0, "first", "First positional argument");
    bool thrown = false;
    string what;
    try {
        args.get<string>(0);
    } catch (exception& e) {
        thrown = true;
        what = e.what();
        assert(str_contains(what, "Missing argument at: 0") && "Exception should mention missing argument");
    }
    assert(thrown && "get should throw when positional argument is missing");
}

#endif
