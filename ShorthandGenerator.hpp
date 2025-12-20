#pragma once

#include <set>
#include <string>
#include <vector>

using namespace std;

class ShorthandGenerator {
public:
    // Register an existing shorthand
    void registerShorthand(const string& shorthand) {
        if (!shorthand.empty()) {
            used_shorthands.insert(shorthand);
        }
    }

    // Generate a unique shorthand for the given key
    string generate(const string& key) {
        if (key.empty()) return "";

        // Strategy 1: First character
        string candidate = string(1, key[0]);
        if (isAvailable(candidate)) {
            used_shorthands.insert(candidate);
            return candidate;
        }

        // Strategy 2: Split at camelCase, '-', or '_' and use second character
        vector<size_t> split_positions = findSplitPositions(key);
        if (split_positions.size() > 1) {
            size_t second_pos = split_positions[1];
            if (second_pos < key.length()) {
                candidate = string(1, key[second_pos]);
                if (isAvailable(candidate)) {
                    used_shorthands.insert(candidate);
                    return candidate;
                }
            }
        }

        // Strategy 3: Combine first + second [+ third...] characters from split positions
        for (size_t i = 2; i <= split_positions.size(); i++) {
            candidate = "";
            for (size_t j = 0; j < i && j < split_positions.size(); j++) {
                if (split_positions[j] < key.length()) {
                    candidate += key[split_positions[j]];
                }
            }
            if (!candidate.empty() && isAvailable(candidate)) {
                used_shorthands.insert(candidate);
                return candidate;
            }
        }

        // Strategy 4: Add alphabetical characters one by one
        for (char c = 'a'; c <= 'z'; c++) {
            candidate = string(1, c);
            if (isAvailable(candidate)) {
                used_shorthands.insert(candidate);
                return candidate;
            }
        }
        for (char c = 'A'; c <= 'Z'; c++) {
            candidate = string(1, c);
            if (isAvailable(candidate)) {
                used_shorthands.insert(candidate);
                return candidate;
            }
        }

        // Strategy 5: Add alphanumeric characters one by one
        for (char c = '0'; c <= '9'; c++) {
            candidate = string(1, c);
            if (isAvailable(candidate)) {
                used_shorthands.insert(candidate);
                return candidate;
            }
        }

        // Strategy 6: Add all characters from the original key one by one
        for (size_t i = 0; i < key.length(); i++) {
            candidate = string(1, key[i]);
            if (isAvailable(candidate)) {
                used_shorthands.insert(candidate);
                return candidate;
            }
        }

        // Strategy 7: Add numbers after the first character
        string base = string(1, key[0]);
        for (int num = 1; num < 10000; num++) {
            candidate = base + to_string(num);
            if (isAvailable(candidate)) {
                used_shorthands.insert(candidate);
                return candidate;
            }
        }

        // Fallback: should never reach here
        return "";
    }

private:
    set<string> used_shorthands;

    bool isAvailable(const string& shorthand) const {
        return used_shorthands.find(shorthand) == used_shorthands.end();
    }

    // Find positions where words start (camelCase, '-', '_')
    vector<size_t> findSplitPositions(const string& key) const {
        vector<size_t> positions;
        positions.push_back(0); // First character

        for (size_t i = 1; i < key.length(); i++) {
            // CamelCase: lowercase followed by uppercase
            if (i > 0 && islower(key[i - 1]) && isupper(key[i])) {
                positions.push_back(i);
            }
            // After '-' or '_'
            else if (i > 0 && (key[i - 1] == '-' || key[i - 1] == '_')) {
                positions.push_back(i);
            }
        }

        return positions;
    }
};

#ifdef TEST

// Test Strategy 1: First character
TEST(test_ShorthandGenerator_first_character) {
    ShorthandGenerator gen;
    string shorthand = gen.generate("verbose");
    assert(shorthand == "v" && "Should use first character 'v'");
}

// Test Strategy 1: First character already taken
TEST(test_ShorthandGenerator_first_character_taken) {
    ShorthandGenerator gen;
    gen.registerShorthand("v");
    string shorthand = gen.generate("verbose");
    assert(shorthand != "v" && "Should not use 'v' as it's already taken");
}

// Test Strategy 2: CamelCase split - second character
TEST(test_ShorthandGenerator_camelcase_second) {
    ShorthandGenerator gen;
    gen.registerShorthand("v");
    string shorthand = gen.generate("verboseMode");
    assert(shorthand == "M" && "Should use second word start 'M' from camelCase");
}

// Test Strategy 2: Underscore split - second character
TEST(test_ShorthandGenerator_underscore_second) {
    ShorthandGenerator gen;
    gen.registerShorthand("v");
    string shorthand = gen.generate("verbose_mode");
    assert(shorthand == "m" && "Should use second word start 'm' after underscore");
}

// Test Strategy 2: Hyphen split - second character
TEST(test_ShorthandGenerator_hyphen_second) {
    ShorthandGenerator gen;
    gen.registerShorthand("v");
    string shorthand = gen.generate("verbose-mode");
    assert(shorthand == "m" && "Should use second word start 'm' after hyphen");
}

// Test Strategy 3: Combine first + second characters
TEST(test_ShorthandGenerator_combine_two) {
    ShorthandGenerator gen;
    gen.registerShorthand("v");
    gen.registerShorthand("M");
    string shorthand = gen.generate("verboseMode");
    assert(shorthand == "vM" && "Should combine 'v' and 'M'");
}

// Test Strategy 3: Combine first + second + third characters
TEST(test_ShorthandGenerator_combine_three) {
    ShorthandGenerator gen;
    gen.registerShorthand("v");
    gen.registerShorthand("M");
    gen.registerShorthand("vM");
    string shorthand = gen.generate("verboseModeExtra");
    assert(shorthand == "vME" && "Should combine 'v', 'M', and 'E'");
}

// Test Strategy 4: Alphabetical characters (lowercase)
TEST(test_ShorthandGenerator_alphabetical_lowercase) {
    ShorthandGenerator gen;
    // Register all except 'z'
    for (char c = 'a'; c < 'z'; c++) {
        gen.registerShorthand(string(1, c));
    }
    string shorthand = gen.generate("anything");
    assert(shorthand == "z" && "Should use last available lowercase letter 'z'");
}

// Test Strategy 4: Alphabetical characters (uppercase)
TEST(test_ShorthandGenerator_alphabetical_uppercase) {
    ShorthandGenerator gen;
    // Register all lowercase and uppercase except 'Z'
    for (char c = 'a'; c <= 'z'; c++) {
        gen.registerShorthand(string(1, c));
    }
    for (char c = 'A'; c < 'Z'; c++) {
        gen.registerShorthand(string(1, c));
    }
    string shorthand = gen.generate("anything");
    assert(shorthand == "Z" && "Should use last available uppercase letter 'Z'");
}

// Test Strategy 5: Alphanumeric characters (digits)
TEST(test_ShorthandGenerator_numeric) {
    ShorthandGenerator gen;
    // Register all letters
    for (char c = 'a'; c <= 'z'; c++) {
        gen.registerShorthand(string(1, c));
    }
    for (char c = 'A'; c <= 'Z'; c++) {
        gen.registerShorthand(string(1, c));
    }
    string shorthand = gen.generate("anything");
    assert(shorthand == "0" && "Should use first available digit '0'");
}

// Test Strategy 5: Last digit
TEST(test_ShorthandGenerator_last_digit) {
    ShorthandGenerator gen;
    // Register all letters and digits except '9'
    for (char c = 'a'; c <= 'z'; c++) {
        gen.registerShorthand(string(1, c));
    }
    for (char c = 'A'; c <= 'Z'; c++) {
        gen.registerShorthand(string(1, c));
    }
    for (char c = '0'; c < '9'; c++) {
        gen.registerShorthand(string(1, c));
    }
    string shorthand = gen.generate("anything");
    assert(shorthand == "9" && "Should use last available digit '9'");
}

// Test Strategy 6: Characters from original key
TEST(test_ShorthandGenerator_from_key) {
    ShorthandGenerator gen;
    // Register all alphanumeric except '@'
    for (char c = 'a'; c <= 'z'; c++) {
        gen.registerShorthand(string(1, c));
    }
    for (char c = 'A'; c <= 'Z'; c++) {
        gen.registerShorthand(string(1, c));
    }
    for (char c = '0'; c <= '9'; c++) {
        gen.registerShorthand(string(1, c));
    }
    string shorthand = gen.generate("test@key");
    assert(shorthand == "@" && "Should use '@' from the original key");
}

// Test Strategy 7: Add number suffix
TEST(test_ShorthandGenerator_number_suffix) {
    ShorthandGenerator gen;
    gen.registerShorthand("v");
    // Register v1, v2, v3 and block other strategies
    gen.registerShorthand("v1");
    gen.registerShorthand("v2");
    gen.registerShorthand("v3");
    // Block all other single chars to force number suffix
    for (char c = 'a'; c <= 'z'; c++) {
        if (c != 'v') gen.registerShorthand(string(1, c));
    }
    for (char c = 'A'; c <= 'Z'; c++) {
        gen.registerShorthand(string(1, c));
    }
    for (char c = '0'; c <= '9'; c++) {
        gen.registerShorthand(string(1, c));
    }
    string shorthand = gen.generate("value");
    assert(shorthand == "v4" && "Should use 'v4' with number suffix");
}

// Test Strategy 7: Number suffix when key starts with taken character
TEST(test_ShorthandGenerator_number_suffix_simple) {
    ShorthandGenerator gen;
    // Register v and all other single char options that "value" would try
    gen.registerShorthand("v");
    // Assuming "value" has no camelCase/splits, it would try alphabetical next
    // So we need to block enough to force v1
    for (char c = 'a'; c <= 'z'; c++) {
        if (c != 'v') gen.registerShorthand(string(1, c));
    }
    for (char c = 'A'; c <= 'Z'; c++) {
        gen.registerShorthand(string(1, c));
    }
    for (char c = '0'; c <= '9'; c++) {
        gen.registerShorthand(string(1, c));
    }
    string shorthand = gen.generate("value");
    assert(shorthand == "v1" && "Should use 'v1' as first numbered suffix");
}

// Test Strategy 7: First available number
TEST(test_ShorthandGenerator_first_number_suffix) {
    ShorthandGenerator gen;
    gen.registerShorthand("v");
    // Need to block all single characters to force number suffix strategy
    for (char c = 'a'; c <= 'z'; c++) {
        gen.registerShorthand(string(1, c));
    }
    for (char c = 'A'; c <= 'Z'; c++) {
        gen.registerShorthand(string(1, c));
    }
    for (char c = '0'; c <= '9'; c++) {
        gen.registerShorthand(string(1, c));
    }
    string shorthand = gen.generate("value");
    assert(shorthand == "v1" && "Should use 'v1' as first numbered suffix");
}

// Test empty key
TEST(test_ShorthandGenerator_empty_key) {
    ShorthandGenerator gen;
    string shorthand = gen.generate("");
    assert(shorthand == "" && "Empty key should return empty shorthand");
}

// Test multiple keys with same starting letter
TEST(test_ShorthandGenerator_multiple_keys_same_start) {
    ShorthandGenerator gen;
    
    // Block all single characters except 'v' to force number suffixes
    for (char c = 'a'; c <= 'z'; c++) {
        if (c != 'v') gen.registerShorthand(string(1, c));
    }
    for (char c = 'A'; c <= 'Z'; c++) {
        gen.registerShorthand(string(1, c));
    }
    for (char c = '0'; c <= '9'; c++) {
        gen.registerShorthand(string(1, c));
    }
    
    string s1 = gen.generate("verbose");
    string s2 = gen.generate("value");
    string s3 = gen.generate("version");
    
    assert(s1 == "v" && "First key should get 'v'");
    assert(s2 == "v1" && "Second key should get 'v1'");
    assert(s3 == "v2" && "Third key should get 'v2'");
}

// Test multiple keys sequentially
TEST(test_ShorthandGenerator_multiple_keys) {
    ShorthandGenerator gen;
    string s1 = gen.generate("verbose");
    string s2 = gen.generate("value");
    string s3 = gen.generate("version");
    
    assert(s1 == "v" && "First key should get 'v'");
    assert(s2 == "a" && "Second key should get 'a' (first available letter)");
    assert(s3 == "b" && "Third key should get 'b' (next available letter)");
}

// Test with pre-registered shorthands
TEST(test_ShorthandGenerator_preregistered) {
    ShorthandGenerator gen;
    gen.registerShorthand("v");
    gen.registerShorthand("h");
    
    string s1 = gen.generate("verbose");
    string s2 = gen.generate("help");
    
    assert(s1 != "v" && "Should not use pre-registered 'v'");
    assert(s2 != "h" && "Should not use pre-registered 'h'");
}

// Test complex camelCase
TEST(test_ShorthandGenerator_complex_camelcase) {
    ShorthandGenerator gen;
    string shorthand = gen.generate("thisIsAVeryLongCamelCaseKey");
    assert(shorthand == "t" && "Should use first character 't'");
    
    gen.registerShorthand("t");
    shorthand = gen.generate("thisIsAVeryLongCamelCaseKey");
    assert(shorthand == "I" && "Should use second word 'I' from camelCase");
}

// Test mixed delimiters
TEST(test_ShorthandGenerator_mixed_delimiters) {
    ShorthandGenerator gen;
    gen.registerShorthand("m");
    string shorthand = gen.generate("my-long_keyName");
    assert(shorthand == "l" && "Should find second word start after hyphen");
}

// Test all strategies exhausted scenario
TEST(test_ShorthandGenerator_stress_test) {
    ShorthandGenerator gen;
    
    // Generate 100 shorthands - should handle all cases
    vector<string> shorthands;
    for (int i = 0; i < 100; i++) {
        string key = "key" + to_string(i);
        string shorthand = gen.generate(key);
        assert(!shorthand.empty() && "Should always generate a shorthand");
        shorthands.push_back(shorthand);
    }
    
    // Verify all are unique
    set<string> unique(shorthands.begin(), shorthands.end());
    assert(unique.size() == shorthands.size() && "All shorthands should be unique");
}

// Test case sensitivity
TEST(test_ShorthandGenerator_case_sensitive) {
    ShorthandGenerator gen;
    gen.registerShorthand("v");
    string shorthand = gen.generate("Value");
    assert(shorthand == "V" && "Should use uppercase 'V' as lowercase is taken");
}

// Test single character key
TEST(test_ShorthandGenerator_single_char) {
    ShorthandGenerator gen;
    string shorthand = gen.generate("x");
    assert(shorthand == "x" && "Single character key should use itself");
}

// Test numeric key
TEST(test_ShorthandGenerator_numeric_key) {
    ShorthandGenerator gen;
    string shorthand = gen.generate("123abc");
    assert(shorthand == "1" && "Should use first character '1'");
}

#endif