#pragma once

#include "ShorthandGenerator.hpp"
#include "array_key_exists.hpp"
#include "parse.hpp"
#include "str_starts_with.hpp"

using namespace std;

class Arguments {
public:
    using Key = pair<string, string>;
    struct Helps {
        vector<pair<string, string>> at;
        map<Key, string> key;
    };

    // Constructor that initializes from command-line arguments
    Arguments(
        int argc, char* argv[],
        const string& prefix = "--", 
        const string& prefix_short = "-",
        const string& equal_to = "="
    ):
        prefix(prefix),
        prefix_short(prefix_short),
        equal_to(equal_to)
    {
        for (int i = 0; i < argc; i++) 
            args.push_back(string(argv[i]));
    }

    virtual ~Arguments() {}

    void addHelp(const size_t& at, const string& name, const string& description) {
        if (helps.at.size() <= at) helps.at.resize(at + 1);
        helps.at[at] = { name, description };
    }

    void addHelpByKey(const Key& key, const string& description) {
        // validate that the key is already there?
        if (array_key_exists(key, helps.key)) // TODO: also check if shorthand already exists!
            throw ERROR("Help already set for key: " + key.first);
        
        Key final_key = key;
        if (key.second.empty()) {
            // Generate automatic shorthand
            final_key.second = shorthand_gen.generate(key.first);
        } else {
            // Register the provided shorthand
            shorthand_gen.registerShorthand(key.second);
        }

        helps.key[key] = description;
    }

    void addHelp(const string& k, const string& description) {
        addHelpByKey({ k, "" }, description);
    }

    bool hasHelp(const string& key) const {
        for (const pair<Key, string> hlp: helps.key)
            if (hlp.first.first == key || hlp.first.second == key)
                return true;
        return false;
    }

    bool hasHelp(const size_t at) const {
        return at <= helps.at.size();
    }

    const vector<string>& getArgsCRef() const {
        return args;
    }

    // Check if an argument exists
    bool has(const string& key, const string& prefix = "--") const {
        checkHelp(key, prefix);
        string prefixed_key = prefix + key;
        for (const auto& arg : args) 
            if (arg == prefixed_key || str_starts_with(arg, prefixed_key + equal_to)) 
                return true;
        return false;
    }

    bool has(const Key& keys) const {
        return has(keys.first, prefix) || has(keys.second, prefix_short);
    }

    // Check if an argument exists
    bool has(size_t at) const {
        if (!hasHelp(at))
            throw ERROR("Help is not provided for argument at " + to_string(at));
        return at < args.size();
    }

    // Get the index of a specific argument
    long int indexOf(const string& arg, const string& prefix = "--") const {
        string prefixed_key = prefix + arg;
        for (size_t i = 0; i < args.size(); ++i)
            if (args[i] == prefixed_key || str_starts_with(args[i], prefixed_key + equal_to)) 
                return (long int)(i);
        return -1;
    }
    
    long int indexOf(const Key& keys) const {
        long int longIdx = indexOf(keys.first);
        if (longIdx != -1) return longIdx;
        return indexOf(keys.second);
    }

    //  =================================== Bool (deprecated) ===================================

    // Get a boolean value based on the presence of a flag
    bool getBool(const string& key) const {
        return has(key);
    }

    // Get a boolean value based on the presence of a flag (with default value)
    bool getBool(const string& key, bool defval) const {
        return has(key) ? true : defval;
    }

    // Get a boolean value at position
    bool getBool(size_t at) const {
        if (has(at)) 
            throw ERROR("Missing argument at: " + to_string(at) + "\n" + help());
        return parse<bool>(args[at]);
    }

    // Get a boolean value at position (with default value)
    bool getBool(size_t at, bool defval) const {
        return has(at) ? defval : parse<bool>(args[at]);
    }

    //  =================================== Templated getters ===================================

    // Templated getter for key-based lookup
    template<typename T>
    T get(const string& key, const string& prefix = "--") const {
        checkHelp(key, prefix);
        long int idx = indexOf(key, prefix);
        if (idx == -1)
            throw ERROR("Missing argument: " + key + "\n" + help());

        string arg = args[idx];
        string prefixed_key = prefix + key + equal_to;
        if (str_starts_with(arg, prefixed_key)) {
            // Extract value after "="
            string value = arg.substr(prefixed_key.length());
            if (value.empty()) 
                throw ERROR("Missing value for argument: " + key + "\n" + help());
            return parse<T>(value);
        } else if (idx + 1 < (long int)(args.size()))
            // Value is in the next argument
            return parse<T>(args[idx + 1]);
        else
            throw ERROR("Missing value for argument: " + key + "\n" + help());
    }
    
    template<typename T>
    T getByKey(const Key& keys) const {
        if (has(keys.first, prefix)) return get<T>(keys.first, prefix);
        else if (has(keys.second, prefix_short)) return get<T>(keys.second, prefix_short);
        else 
            throw ERROR("Missing argument: " + prefix + keys.first + " (or " + prefix_short + keys.second + ")" + "\n" + help());
    }

    // Templated getter with default value for key-based lookup
    template<typename T>
    T getopt(const string& key, const T& defval, const string& prefix = "--") const {
        return has(key, prefix) ? get<T>(key, prefix) : defval;
    }

    template<typename T>
    T getoptByKey(const Key& keys, const T& defval) const {
        return has(keys) ? getByKey<T>(keys) : defval;
    }

    // Templated getter for positional lookup
    template<typename T>
    T get(size_t at) const {
        if (!has(at)) 
            throw ERROR("Missing argument at: " + to_string(at) + "\n" + help());
        return parse<T>(args[at]);
    }

    // Templated getter with default value for positional lookup
    template<typename T>
    T getopt(size_t at, const T& defval) const {
        return !has(at) ? defval : parse<T>(args[at]);
    }

    string help() const {
        string output = "Usages:\n" + args[0];
        for (size_t i = 0; i < helps.at.size(); i++)
            output += " <" + helps.at[i].first + ">";
        if (!helps.key.empty()) output += " <options>\n\n";
        for (size_t i = 0; i < helps.at.size(); i++)
            output += helps.at[i].first + ": " + helps.at[i].second + "\n";
        if (!helps.key.empty()) output += "options:\n";
        for (const auto& [key, description] : helps.key) {
            output += "\n" + prefix + key.first 
                + (key.second.empty() ? "" : ", " + prefix_short + key.second) + "\n" 
                + description + "\n";
        }
        return output;
    }

protected:
    void checkHelp(const string& key, const string& prefix) const {
        if (!hasHelp(key))
            throw ERROR("Help is not provided for argument '" + prefix + key + "'");
    }

    ShorthandGenerator shorthand_gen;
    vector<string> args;
    Helps helps;
    string prefix = "--";
    string prefix_short = "-";
    string equal_to = "=";
};
    
template<>
inline bool Arguments::get<bool>(const string& key, const string& prefix) const {
    return has(key, prefix);
}

template<>
inline bool Arguments::getopt<bool>(const string& key, const bool& defval, const string& prefix) const {
    return has(key, prefix) ? true : defval;
}

template<>
inline bool Arguments::getByKey<bool>(const Key& keys) const {
    return has(keys);
}

template<>
inline bool Arguments::getoptByKey<bool>(const Key& keys, const bool& defval) const {
    return has(keys) ? true : defval;
}
