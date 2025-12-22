#pragma once

// DEPENDENCY: nlohmann/json

// TODO: Use builder DEPENDENCY comment stuff for this
// # ---- ADDING DEPENDENCIES ----
// echo "Adding dependencies..."
//
// # nlohmann/json
// git clone https://github.com/nlohmann/json libs/nlohmann/json
//
// # ----------------------------
#include "../../libs/nlohmann/json/master/single_include/nlohmann/json.hpp"
#include "explode.hpp"
#include "implode.hpp"
#include "str_diffs_show.hpp"
#include "str_contains.hpp"
#include "ERROR.hpp"
#include <string>
#include <vector>
#include <stack>
#include <regex>

using namespace std;
using namespace nlohmann;

string json_last_error = "";

// Function to remove single-line and multi-line comments
string json_remove_comments(const string& json) {
    string result;
    bool inString = false;
    bool escapeNext = false;
    bool inSingleLineComment = false;
    bool inMultiLineComment = false;

    for (size_t i = 0; i < json.size(); ++i) {
        char ch = json[i];

        if (inString && ch == '\\' && !escapeNext) {
            escapeNext = true;
            result += ch;
            continue;
        }

        if (ch == '"' && !escapeNext && !inSingleLineComment && !inMultiLineComment) {
            inString = !inString;
        }

        if (!inString && !inMultiLineComment && ch == '/' && i + 1 < json.size() && json[i + 1] == '/') {
            inSingleLineComment = true;
            i++;
            continue;
        }

        if (!inString && !inSingleLineComment && ch == '/' && i + 1 < json.size() && json[i + 1] == '*') {
            inMultiLineComment = true;
            i++;
            continue;
        }

        if (inSingleLineComment && ch == '\n') {
            inSingleLineComment = false;
        }

        if (inMultiLineComment && ch == '*' && i + 1 < json.size() && json[i + 1] == '/') {
            inMultiLineComment = false;
            i++;
            continue;
        }

        if (!inSingleLineComment && !inMultiLineComment) {
            result += ch;
        }

        escapeNext = false;
    }

    return result;
}

// Function to fix JSON by removing trailing commas
string json_fix(string json) {
    json = json_remove_comments(json);
    string result;
    bool inString = false;
    bool escapeNext = false;
    stack<char> scopeStack;

    for (size_t i = 0; i < json.size(); ++i) {
        char ch = json[i];

        if (ch == '"' && !escapeNext) {
            inString = !inString;
        }

        if (ch == '\\' && !escapeNext) {
            escapeNext = true;
        } else {
            escapeNext = false;
        }

        if (!inString) {
            if (ch == '{' || ch == '[') {
                scopeStack.push(ch);
            } else if (ch == '}' || ch == ']') {
                if (!scopeStack.empty()) {
                    scopeStack.pop();
                }
            }
        }

        if (!inString && ch == ',') {
            size_t j = i + 1;
            while (j < json.size() && (json[j] == ' ' || json[j] == '\t' || json[j] == '\n' || json[j] == '\r')) {
                ++j;
            }
            if (j < json.size() && (json[j] == '}' || json[j] == ']')) {
                continue;
            }
        }

        result += ch;
    }

    return result;
}

// Enum to represent JSON value types
enum json_type {
    JSON_TYPE_UNDEFINED,
    JSON_TYPE_NULL,
    JSON_TYPE_STRING,
    JSON_TYPE_INTEGER,
    JSON_TYPE_REAL,
    JSON_TYPE_BOOLEAN,
    JSON_TYPE_ARRAY,
    JSON_TYPE_OBJECT,
};

// Global variable to store the last JSON parsing error
extern string json_last_error;

// Function to convert jq-style or JavaScript-style selector to json_pointer
nlohmann::json::json_pointer _json_selector(string jselector) {
    if (jselector.empty()) return nlohmann::json::json_pointer("/");
    if (jselector[0] != '.') jselector = "." + jselector;

    vector<string> splits = explode(".", jselector);

    for (size_t i = 1; i < splits.size(); i++) {
        if (splits[i].empty())
            throw ERROR("Invalid json selector: " + jselector);

        regex valid_brackets("\\[\\s*(\\d+)\\s*\\]$");
        regex invalid_brackets("\\[[^\\]]+\\]$");
        smatch match;

        if (regex_search(splits[i], match, valid_brackets)) {
            splits[i] = regex_replace(splits[i], valid_brackets, "/$1");
            continue;
        }

        if (regex_search(splits[i], match, invalid_brackets)) {
            throw ERROR("Invalid json selector: " + jselector);
        }
    }

    int open_brackets = 0, close_brackets = 0;
    for (char ch : jselector) {
        if (ch == '[') open_brackets++;
        if (ch == ']') close_brackets++;
    }
    if (open_brackets != close_brackets) {
        throw ERROR("Invalid json selector: " + jselector);
    }

    return nlohmann::json::json_pointer(implode("/", splits));
}

// Check if a JSON string is valid
bool is_valid_json(string jstring) {
    json_last_error = "";
    try {
        auto json = nlohmann::json::parse(jstring);
        (void)json;
        return true;
    } catch (const nlohmann::json::parse_error& e) {
        json_last_error = e.what();
        return false;
    }
}

// Get the last JSON parsing error
string get_json_error(string jstring) {
    if (is_valid_json(jstring)) return "";
    return json_last_error;
}

// Get the type of a JSON value at a given selector
json_type get_json_value_type(string jstring, string jselector) {
    try {
        auto json = nlohmann::json::parse(jstring);
        auto ptr = _json_selector(jselector);

        if (!json.contains(ptr))
            return JSON_TYPE_UNDEFINED;

        const auto& value = json.at(ptr);
        if (value.is_null()) return JSON_TYPE_NULL;
        if (value.is_string()) return JSON_TYPE_STRING;
        if (value.is_boolean()) return JSON_TYPE_BOOLEAN;
        if (value.is_number_integer()) return JSON_TYPE_INTEGER;
        if (value.is_number_float()) return JSON_TYPE_REAL;
        if (value.is_array()) return JSON_TYPE_ARRAY;
        if (value.is_object()) return JSON_TYPE_OBJECT;

        return JSON_TYPE_UNDEFINED;
    } catch (...) {
        return JSON_TYPE_UNDEFINED;
    }
}

// Convert a json_type enum to its string representation
string json_type_to_string(json_type type) {
    switch (type) {
        case JSON_TYPE_UNDEFINED: return "undefined";
        case JSON_TYPE_NULL: return "null";
        case JSON_TYPE_STRING: return "string";
        case JSON_TYPE_INTEGER: return "integer";
        case JSON_TYPE_REAL: return "real";
        case JSON_TYPE_BOOLEAN: return "boolean";
        case JSON_TYPE_ARRAY: return "array";
        case JSON_TYPE_OBJECT: return "object";
        default:
            throw ERROR("Invalid JSON type: " + to_string(type));
    }
}

// Retrieve specific JSON value types
string json_get_string(string jstring, string jselector) {
    if (get_json_value_type(jstring, jselector) != JSON_TYPE_STRING)
        throw ERROR("Expected string type at " + jselector);
    try {
        auto json = nlohmann::json::parse(jstring);
        return json.at(_json_selector(jselector)).get<string>();
    } catch (const nlohmann::json::exception& e) {
        throw ERROR("Error retrieving string: " + string(e.what()));
    }
}

int json_get_int(string jstring, string jselector) {
    if (get_json_value_type(jstring, jselector) != JSON_TYPE_INTEGER)
        throw ERROR("Expected integer type at " + jselector);
    try {
        auto json = nlohmann::json::parse(jstring);
        return json.at(_json_selector(jselector)).get<int>();
    } catch (const nlohmann::json::exception& e) {
        throw ERROR("Error retrieving integer: " + string(e.what()));
    }
}

double json_get_double(string jstring, string jselector) {
    if (get_json_value_type(jstring, jselector) != JSON_TYPE_REAL)
        throw ERROR("Expected real type at " + jselector);
    try {
        auto json = nlohmann::json::parse(jstring);
        return json.at(_json_selector(jselector)).get<double>();
    } catch (const nlohmann::json::exception& e) {
        throw ERROR("Error retrieving double: " + string(e.what()));
    }
}

bool json_get_bool(string jstring, string jselector) {
    if (get_json_value_type(jstring, jselector) != JSON_TYPE_BOOLEAN)
        throw ERROR("Expected boolean type at " + jselector);
    try {
        auto json = nlohmann::json::parse(jstring);
        return json.at(_json_selector(jselector)).get<bool>();
    } catch (const nlohmann::json::exception& e) {
        throw ERROR("Error retrieving boolean: " + string(e.what()));
    }
}

string json_get_array(string jstring, string jselector) {
    if (get_json_value_type(jstring, jselector) != JSON_TYPE_ARRAY)
        throw ERROR("Expected array type at " + jselector);
    try {
        auto json = nlohmann::json::parse(jstring);
        return json.at(_json_selector(jselector)).dump();
    } catch (const nlohmann::json::exception& e) {
        throw ERROR("Error retrieving array: " + string(e.what()));
    }
}

string json_get_object(string jstring, string jselector) {
    if (get_json_value_type(jstring, jselector) != JSON_TYPE_OBJECT)
        throw ERROR("Expected object type at " + jselector);
    try {
        auto json = nlohmann::json::parse(jstring);
        return json.at(_json_selector(jselector)).dump();
    } catch (const nlohmann::json::exception& e) {
        throw ERROR("Error retrieving object: " + string(e.what()));
    }
}

// JSON class to manage JSON data
class JSON {
protected:
    string* _error = nullptr;
    nlohmann::json j;

public:
    JSON(const nlohmann::json& j) : j(j) {}
    JSON(const char* j) : JSON(string(j)) {}
    JSON(string jstring = "{}") {
        try {
            j = nlohmann::json::parse(jstring.empty() ? "{}" : json_fix(jstring));
        } catch (const exception& e) {
            _error = new string(e.what());
        }
    }

    virtual ~JSON() {
        if (_error) { delete _error; _error = nullptr; }
    }

    nlohmann::json get_json() const {
        return j;
    }

    bool isValid(string* error = nullptr) {
        if (error) *error = _error ? *_error : "";
        return !_error;
    }

    string dump(int indent = -1, char indent_char = ' ') const {
        try {
            return j.dump(indent, indent_char);
        } catch (const exception& e) {
            throw ERROR("JSON dump error: " + string(e.what()));
        }
    }
   
    bool isDefined(string jselector) const {
        try {
            nlohmann::json::json_pointer ptr = _json_selector(jselector);
            return j.contains(ptr);
        } catch (...) {
            return false;
        }
    }

    bool has(string jselector) const {
        return isDefined(jselector);
    }

    bool isNull(string jselector) {
        try {
            nlohmann::json::json_pointer ptr = _json_selector(jselector);
            return j.at(ptr).is_null();
        } catch (...) {
            return false;
        }
    }

    bool isArray(string jselector) {
        try {
            nlohmann::json::json_pointer ptr = _json_selector(jselector);
            return j.at(ptr).is_array();
        } catch (...) {
            return false;
        }
    }

    bool isObject(string jselector) {
        try {
            nlohmann::json::json_pointer ptr = _json_selector(jselector);
            return j.at(ptr).is_object();
        } catch (...) {
            return false;
        }
    }
    

    template<typename T>
    T get(string jselector) const {
        try {
            json::json_pointer ptr = _json_selector(jselector);  // Convert selector to pointer
            if constexpr (is_same_v<T, JSON>) return JSON(j.at(ptr));
            return j.at(ptr).get<T>();
        } catch (const exception& e) {
            // DEBUG(j.dump());
            throw ERROR("JSON Error at: " + jselector + ", reason: " + string(e.what()));
        }
    }

    // void set(string value) {
    //     jstring = value;
    // }

    template<typename T>
    void set(string jselector, T value) {
        try {
            // json j = json::parse(jstring);
            json::json_pointer ptr = _json_selector(jselector);
            // j[ptr] = value;
            // jstring = j.dump();
            j[ptr] = value;
        } catch (const json::exception& e) {
            //DEBUG(j.dump());
            throw ERROR("JSON Error at: " + jselector + ", reason: " + string(e.what()));
        }
    }

    // Commented-out validation helpers (retained for future use)
    // void need(const string& field) const;
    // void need(const vector<string>& fields) const;
};

namespace nlohmann {
    template<>
    struct adl_serializer<JSON> {
        static JSON from_json(const json& j) {
            return JSON(j);
        }
        
        static void to_json(json& j, const JSON& jsonObj) {
            j = jsonObj.get_json();
        }
    };
}

