#pragma once

#include "JSON.hpp"

class JSONExts {
public:

    void extends(JSON ext) {
        exts.emplace(exts.begin(), ext);
    }

    template<typename T>
    T get(string jselector) const {
        for (const JSON& ext: exts)
            if (ext.has(jselector))
                return ext.get<T>(jselector);
        throw ERROR("No value at '" + jselector + "'");
    }

    bool has(string jselector) const {
        for (const JSON& ext: exts)
            if (ext.has(jselector))
                return true;
        return false;
    }

    template<typename T>
    void set(string jselector, T value) {
        if (exts.empty()) exts.push_back(JSON("{}"));
        exts[0].set(jselector, value);
    }

    bool empty() const {
        return exts.empty();
    }

    string dump(const int indent = -1, const char indent_char = ' ') const {
        vector<string> dumps;
        for (const JSON& ext: exts)
            dumps.push_back(ext.dump(indent, indent_char));
        return "[" + implode(",", dumps) + "]";
    }

private:
    vector<JSON> exts;
};

