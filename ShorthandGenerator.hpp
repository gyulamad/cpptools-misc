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
