#pragma once

#include "../TEST.hpp"
#include "../regx_match_all.hpp"

#ifdef TEST


TEST(test_regx_match_all_basic) {
    string pattern = "\\d+"; // Matches one or more digits
    string str = "123 abc 456 def";
    vector<string> matches;
    int result = regx_match_all(pattern, str, &matches);

    // Expect 2 matches: "123" and "456"
    assert(result == 2 && "Basic match all");
    assert(matches.size() == 2 && "Match count");
    assert(matches[0] == "123" && "First match");
    assert(matches[1] == "456" && "Second match");
}

TEST(test_regx_match_all_single_match) {
    string pattern = "\\d+"; // Matches one or more digits
    string str = "123";
    vector<string> matches;
    int result = regx_match_all(pattern, str, &matches);

    assert(result == 1 && "Single match");
    assert(matches.size() == 1 && "Match count");
    assert(matches[0] == "123" && "First match");
}

TEST(test_regx_match_all_no_matches) {
    string pattern = "\\d+"; // Matches one or more digits
    string str = "abc def";
    vector<string> matches;
    int result = regx_match_all(pattern, str, &matches);

    assert(result == 0 && "No matches");
    assert(matches.empty() && "Matches vector should be empty");
}

TEST(test_regx_match_all_no_match) {
    string pattern = "\\d+"; // Matches one or more digits
    string str = "abc def";
    vector<string> matches;
    int result = regx_match_all(pattern, str, &matches);
    assert(result == 0 && "No match");
    assert(matches.empty() && "Empty matches");
}

TEST(test_regx_match_all_capture_groups) {
    string pattern = "(\\d+)(\\w+)"; // Matches digits followed by letters
    string str = "123abc 456def";
    vector<string> matches;
    int result = regx_match_all(pattern, str, &matches);

    // Expected matches: ["123abc", "123", "abc", "456def", "456", "def"]
    assert(result == 2 && "Match count incorrect");
    assert(matches.size() == 6 && "Match count (2 matches, 2 capture groups each)");
    assert(matches[0] == "123abc" && "First full match");
    assert(matches[1] == "123" && "First capture group (digits)");
    assert(matches[2] == "abc" && "Second capture group (letters)");
    assert(matches[3] == "456def" && "Second full match");
    assert(matches[4] == "456" && "Third capture group (digits)");
    assert(matches[5] == "def" && "Fourth capture group (letters)");
}

TEST(test_regx_match_all_single_match_multiple_capture_groups) {
    string pattern = "(\\d+)-(\\w+)";
    string str = "123-abc";
    vector<string> matches;
    int result = regx_match_all(pattern, str, &matches);

    assert(result == 1 && "Match count incorrect");
    assert(matches.size() == 3 && "Match count (1 match, 2 capture groups)");
    assert(matches[0] == "123-abc" && "Full match");
    assert(matches[1] == "123" && "Capture group 1 (digits)");
    assert(matches[2] == "abc" && "Capture group 2 (letters)");
}

TEST(test_regx_match_all_no_matches2) {
    string pattern = "(\\d+)-(\\w+)";
    string str = "no-match-here";
    vector<string> matches;
    int result = regx_match_all(pattern, str, &matches);

    assert(result == 0 && "Match count incorrect");
    assert(matches.empty() && "Matches vector should be empty");
}

TEST(test_regx_match_all_empty_pattern) {
    string pattern = "";
    string str = "hello world";
    vector<string> matches;
    int result = regx_match_all(pattern, str, &matches);

    // Expect 12 matches (empty pattern matches every position)
    assert(result == 12 && "Empty pattern (matches any string)");
    assert(matches.size() == 12 && "Match count (empty pattern matches every position)");

    // Verify that all matches are empty strings
    for (const string& match : matches) {
        assert(match.empty() && "Each match should be an empty string");
    }
}

TEST(test_regx_match_all_empty_string) {
    string pattern = "\\d+";
    string str = "";
    vector<string> matches;
    int result = regx_match_all(pattern, str, &matches);
    assert(result == 0 && "Empty string");
    assert(matches.empty() && "Empty matches");
}

TEST(test_regx_match_all_special_characters) {
    string pattern = "\\W+"; // Matches one or more non-word characters
    string str = "hello, world!";
    vector<string> matches;
    int result = regx_match_all(pattern, str, &matches);

    // Expect 2 matches: ", " and "!"
    assert(result == 2 && "Special characters (non-word)");
    assert(matches.size() == 2 && "Match count");
    assert(matches[0] == ", " && "First match");
    assert(matches[1] == "!" && "Second match");
}

TEST(test_regx_match_all_special_characters_no_matches) {
    string pattern = "\\W+"; // Matches one or more non-word characters
    string str = "helloworld";
    vector<string> matches;
    int result = regx_match_all(pattern, str, &matches);

    assert(result == 0 && "No special characters");
    assert(matches.empty() && "Matches vector should be empty");
}

TEST(test_regx_match_all_special_characters_with_capture_groups) {
    string pattern = "(\\W+)"; // Matches one or more non-word characters with a capture group
    string str = "hello, world!";
    vector<string> matches;
    int result = regx_match_all(pattern, str, &matches);

    // Expect 2 matches: ", " and "!", each with a capture group
    assert(result == 2 && "Special characters with capture groups");
    assert(matches.size() == 4 && "Match count (2 full matches + 2 capture groups)");
    assert(matches[0] == ", " && "First full match");
    assert(matches[1] == ", " && "First capture group");
    assert(matches[2] == "!" && "Second full match");
    assert(matches[3] == "!" && "Second capture group");
}

TEST(test_regx_match_all_case_sensitive) {
    string pattern = "HELLO";
    string str = "hello world HELLO";
    vector<string> matches;
    int result = regx_match_all(pattern, str, &matches);
    assert(result == 1 && "Case sensitive");
    assert(matches.size() == 1 && "Match count");
    assert(matches[0] == "HELLO" && "Case-sensitive match");
}

TEST(test_regx_match_all_case_insensitive) {
    string pattern = "HELLO"; // Case-insensitive match
    string str = "hello world HELLO";
    vector<string> matches;
    int result = regx_match_all(pattern, str, &matches, regex_constants::icase);

    // Expect 2 matches: "hello" and "HELLO"
    assert(result == 2 && "Case insensitive");
    assert(matches.size() == 2 && "Match count");
    assert(matches[0] == "hello" && "First case-insensitive match");
    assert(matches[1] == "HELLO" && "Second case-insensitive match");
}

TEST(test_regx_match_all_invalid_pattern) {
    string pattern = "[invalid"; // Invalid regex pattern
    string str = "hello world";
    vector<string> matches;
    int result = regx_match_all(pattern, str, &matches);

    // Expect no matches for invalid patterns
    assert(result == 0 && "Invalid pattern should return 0 matches");
    assert(matches.empty() && "Matches vector should be empty");
}

TEST(test_regx_match_all_non_empty_pattern) {
    string pattern = "\\w+"; // Matches words
    string str = "hello world";
    vector<string> matches;
    int result = regx_match_all(pattern, str, &matches);

    assert(result == 2 && "Non-empty pattern match count");
    assert(matches.size() == 2 && "Match count");
    assert(matches[0] == "hello" && "First match");
    assert(matches[1] == "world" && "Second match");
}

#endif
