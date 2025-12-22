#pragma once

#include "get_hash.hpp"
#include "JSON.hpp"
#include "JSONExts.hpp"
#include "Arguments.hpp"
#include <string>
#include <vector>
#include <map>

using namespace std;

class Settings {
public:
    Settings() {}
    Settings(JSON& conf) : conf(&conf) {}
    Settings(Arguments& args) : args(&args) {}
    Settings(JSON& conf, Arguments& args) : args(&args), conf(&conf) {}
    Settings(Arguments& args, JSON& conf) : args(&args), conf(&conf) {}

    // Add a JSON extension
    void extends(JSON ext) {
        exts.extends(ext);
    }

    // Compute a hash of the settings
    string hash() const {
        return get_hash(
            (!exts.empty() ? exts.dump() : "<noexts>") +
            (conf ? conf->dump() : "<noconf>") +
            (args ? implode(" ", args->getArgsCRef()) : "<noargs>")
        );
    }

    // Check if a key exists
    bool has(const string& key) {
        if (args && args->has(key)) return true;
        if (!exts.empty() && exts.has(key)) return true;
        if (conf && conf->has(key)) return true;
        return false;
    }

    bool has(const pair<string, string>& keys) {
        if (args && args->has(keys)) return true;
        if (!exts.empty() && exts.has(keys.first)) return true;
        if (conf && conf->has(keys.first)) return true;
        return false;
    }

    template<typename T>
    T get(const string& key) {
        if constexpr (!is_container<T>::value) if (args && args->has(key)) return args->get<T>(key);
        if (!exts.empty() && exts.has(key)) return exts.get<T>(key);
        if (conf && conf->has(key)) return conf->get<T>(key);
        throw ERROR("Settings is missing for '" + key + "'");
    }

    template<typename T>
    T get(const string& key, const T defval) {
        if constexpr (!is_container<T>::value) if (args && args->has(key)) return args->get<T>(key);
        if (!exts.empty() && exts.has(key)) return exts.get<T>(key);
        if (conf && conf->has(key)) return conf->get<T>(key);
        return defval;
    }

    template<typename T>
    T get(const pair<string, string>& keys) {
        if constexpr (!is_container<T>::value) if (args && args->has(keys)) return args->get<T>(keys);
        if (!exts.empty() && exts.has(keys.first)) return exts.get<T>(keys.first);
        if (conf && conf->has(keys.first)) return conf->get<T>(keys.first);
        throw ERROR("Settings is missing for '" + keys.first + "' (or '" + keys.second + "')");
    }

    template<typename T>
    T get(const pair<string, string>& keys, const T defval) {
        if constexpr (!is_container<T>::value) if (args && args->has(keys)) return args->get<T>(keys);
        if (!exts.empty() && exts.has(keys.first)) return exts.get<T>(keys.first);
        if (conf && conf->has(keys.first)) return conf->get<T>(keys.first);
        return defval;
    }

    Arguments* args = nullptr;
    JSON* conf = nullptr;
    JSONExts exts;

private:
    // Default case: assume types are not containers
    template<typename T>
    struct is_container : false_type {};

    // Specialization for vector
    template<typename T>
    struct is_container<vector<T>> : true_type {};

    // Specialization for map
    template<typename K, typename V>
    struct is_container<map<K, V>> : true_type {};
};
