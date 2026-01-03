#pragma once

#include "array_keys.hpp"
#include "array_values.hpp"
#include <unordered_map>
#include "in_array.hpp"
#include "array_key_exists.hpp"
#include "EMPTY_OR.hpp"
#include "ERROR.hpp"
#include "parse.hpp"
#include "str_replace.hpp"
#include "Logger.hpp"

using namespace std;

class IniData {
public:
    IniData() {}
    virtual ~IniData() {}

    // IniData& operator=(const IniData& other) {
    //     setData(other);
    //     return *this;
    // }

    void setData(const IniData& other) {
        if (this != &other) {
            changed = data != other.data;
            data = other.data;
        }
        // return *this;
        // *this = other;
    }

    vector<string> sections() const {
        return array_keys(data);
    }

    void clear() {
        if (!data.empty()) changed = true;
        data.clear();
    }

    size_t size(const string& section = "") const {
        return data.at(section).size();
    }

    bool has(const string& key, const string& section = "") const {
        return in_array(section, sections()) && array_key_exists(key, data.at(section));
    }

    vector<string> keys(const string& section = "") const {
        return array_keys(data.at(section));
    }

    vector<string> values(const string& section = "") const {
        if (!array_key_exists(section, data))
            throw ERROR("Section is not exists: " + EMPTY_OR(section, "<global>"));
        return array_values(data.at(section));
    }

    template<typename T>
    unordered_map<string, T> list(const string& section = "") const {
        unordered_map<string, T> typed;
        for (const auto& [key, value]: data.at(section))
            typed[key] = parse<T>(value);
        return typed;
    }

    const string ERR_MISSING_KEY = "Key is not exists: {{key}} in section [{{section}}]";

    template<typename T>
    T get(const string& key, const string& section = "") const {
        if (!has(key, section)) 
            throw ERROR(str_replace({
                { "{{key}}", { EMPTY_OR(key) } },
                { "{{section}}", { EMPTY_OR(section, "<global>") } },
            }, ERR_MISSING_KEY));
        return parse<T>(data.at(section).at(key));
    }

    template<typename T>
    T getopt(const string& key, const T defval, const string& section = "", bool warns = true) const {
        if (!has(key, section)) {
            if (warns)
                LOG_WARN(str_replace({
                    { "{{key}}", { EMPTY_OR(key) } },
                    { "{{section}}", { EMPTY_OR(section, "<global>") } },
                }, ERR_MISSING_KEY + ", using default value instead."));
            return defval;
        }
        return parse<T>(data.at(section).at(key));
    }

    template<typename T>
    void set(const string& key, T value, const string& section = "") {
        string newval = this->as_string(value);
        if (newval != data[section][key]) {
            data[section][key] = newval;
            changed = true;
        }
    }

    [[nodiscard]]
    bool isChanged() const { return this->changed; }
    void setChanged(bool changed) { this->changed = changed; }

protected:

    unordered_map<
        string, 
        unordered_map<string, string>
    > data;
    bool changed = false;
    
private:
    inline string as_string(const string& str) { return str; }
    inline string as_string(const char* str) { return str; }

    template<typename T>
    inline string as_string(T str) { return std::to_string(str); }
};
