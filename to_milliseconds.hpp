#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include "trim.hpp"
#include "ERROR.hpp"

using namespace std;

static const unordered_map<string, double> to_milliseconds_unit_map = {
    {"year", 365 * 24 * 3600 * 1000.0},
    {"years", 365 * 24 * 3600 * 1000.0},
    {"y", 365 * 24 * 3600 * 1000.0},
    {"week", 7 * 24 * 3600 * 1000.0},
    {"weeks", 7 * 24 * 3600 * 1000.0},
    {"w", 7 * 24 * 3600 * 1000.0},
    {"day", 24 * 3600 * 1000.0},
    {"days", 24 * 3600 * 1000.0},
    {"d", 24 * 3600 * 1000.0},
    {"hour", 3600 * 1000.0},
    {"hours", 3600 * 1000.0},
    {"h", 3600 * 1000.0},
    {"minute", 60 * 1000.0},
    {"minutes", 60 * 1000.0},
    {"min", 60 * 1000.0},
    {"mins", 60 * 1000.0},
    {"m", 60 * 1000.0},
    {"second", 1000.0},
    {"seconds", 1000.0},
    {"s", 1000.0},
    {"millisecond", 1.0},
    {"milliseconds", 1.0},
    {"millis", 1.0},
    {"ms", 1.0}
};

vector<string> to_milliseconds_tokenize_period_string(const string& input) {
    vector<string> tokens;
    string buffer;
    
    for (char c : input) {
        if (isspace(c)) {
            if (!buffer.empty()) {
                tokens.push_back(buffer);
                buffer.clear();
            }
        }
        // Split when switching between digit/non-digit
        else if (!buffer.empty() && 
                (isdigit(c) != isdigit(static_cast<unsigned char>(buffer.back())))) {
            tokens.push_back(buffer);
            buffer = c;
        }
        else {
            buffer += c;
        }
    }
    
    if (!buffer.empty()) {
        tokens.push_back(buffer);
    }
    
    return tokens;
}

double to_milliseconds(const string& period_string) {
    string period_str = trim(period_string);
    if (period_str.empty()) return 0.0;

    vector<string> tokens = to_milliseconds_tokenize_period_string(period_str);
    if (tokens.size() % 2 != 0)
        throw ERROR("Input must have pairs of numbers and units");

    double total_ms = 0.0;
    for (size_t i = 0; i < tokens.size(); i += 2) {
        double value;
        try {
            value = stod(tokens[i]);
        } catch (...) {
            throw ERROR("Invalid number: " + tokens[i]);
        }

        string unit = tokens[i+1];
        transform(unit.begin(), unit.end(), unit.begin(), 
                  [](unsigned char c){ return tolower(c); });

        auto it = to_milliseconds_unit_map.find(unit);
        if (it == to_milliseconds_unit_map.end())
            throw ERROR("Invalid unit: " + unit);

        total_ms += value * it->second;
    }
    return total_ms;
}
