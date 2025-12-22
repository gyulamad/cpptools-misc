#pragma once

#include <stdint.h>
#include "Value.hpp"
#include "Loadable.hpp"
#include "Saveable.hpp"
#include "file_get_contents.hpp"
#include "file_put_contents.hpp"
#include "vector_load.hpp"
#include "vector_save.hpp"
#include "vector_concat.hpp"
#include "safe.hpp"
#include "IniData.hpp"
#include "Initializable.hpp"
#include "VECTOR_WRAPPER.hpp"

template<typename real = float> // , typename S = uint32_t>
class ValuesT: public Initializable {
public:
    ValuesT(): Initializable() {}

    virtual ~ValuesT() {}

    VECTOR_WRAPPER(ValueT<real>, values)

    void onLoad() override {
        LOG_DEBUG("ValuesT are loaded, converts..");
        convert();
    }

    void onReload() override {
        LOG_DEBUG("ValuesT are reloaded, converts...");
        convert();
    }

    // // TODO: depricate or fix, name is ini file section not a key 'name=...'
    // template<typename S>
    // void load(const string& filename, Filemode mode) {
    //     vector<string> valueStrs;
    //     vector<S> bin;
    //     ValueT<real> value;
    //     size_t nxt = 0;

    //     switch (mode) {
    //         case TEXT:
    //             valueStrs = explode("\n\n", file_get_contents(filename));
    //             values.clear();
    //             for (const string& valueStr: valueStrs) {
    //                 ValueT<real> value;
    //                 value.fromString(valueStr);
    //                 values.push_back(value);
    //             }
    //             break;

    //         case BINARY:
    //             vector_load(bin, filename);
    //             values.clear();
                
    //             while (value.deserialize(bin, nxt))
    //                 values.push_back(value);
    //             break;

    //         default:
    //             throw ERROR("Invalid file mode");
    //     }
    // }

    // // TODO: depricate or fix, name is ini file section not a key 'name=...'
    // template<typename S>
    // void save(const string& filename, Filemode mode) {
    //     vector<string> valueStrs;
    //     vector<S> bin;

    //     switch (mode) {
    //         case TEXT:
    //             for (const ValueT<real>& value: values)
    //                 valueStrs.push_back(value.toString());
    //             file_put_contents(filename, implode("\n\n", valueStrs), false, true);
    //             break;

    //         case BINARY:
    //             for (const ValueT<real>& value: values)
    //                 bin = vector_concat<S>(bin, value.serialize());
    //             vector_save(bin, filename);
    //             break;

    //         default:
    //             throw ERROR("Invalid file mode");
    //     }
    // }


    // void add(const ValueT<real>& value) {
    //     values.push_back(value);
    // }

    // -------------

    template<typename T>
    T add(const string& name, T value) {
        if (has(name))
            throw ERROR("Already has: " + EMPTY_OR(name));
        Value v(name, (real)value);
        values.push_back(v);
        return v.getValue();
    }

    bool has(const string& name) {
        size_t size = values.size();
        for (size_t i = 0; i < size; i++)
            if (values[i].getNameCRef() == name) return true;
        return false;
    }

    template<typename T>
    T set(const string& name, T value) {
        size_t size = values.size();
        for (size_t i = 0; i < size; i++)
            if (values[i].getNameCRef() == name) {
                return values[i] = value;
            }
        throw ERROR("Not found: " + EMPTY_OR(name));
    }

    template<typename T>
    T get(const string& name) {
        size_t size = values.size();
        for (size_t i = 0; i < size; i++)
            if (values[i].getNameCRef() == name) return values[i];
        throw ERROR("Not found: " + EMPTY_OR(name));
    }
    
    // ValueT<real>& operator[](size_t index) {
    //     return values[index];
    // }
    
    // const ValueT<real>& operator[](size_t index) const {
    //     return values[index];
    // }
    
    ValueT<real>& operator[](const string& name) {
        size_t i = getIndexByName(name);
        return values[i];
    }
    
    const ValueT<real>& operator[](const string& name) const {
        size_t i = getIndexByName(name);
        return values[i];
    }
    
    ValueT<real>& getValueByName(const string& name) {
        for (ValueT<real>& value: values)
            if (value.getNameCRef() == name) return value;
        throw ERROR("Value not found");
    }
    
    size_t getIndexByName(const string& name, bool create = true) {
        size_t size = values.size();
        for (size_t i = 0; i < size; i++)
            if (values[i].getNameCRef() == name) return i;
        if (create) {
            values.push_back(ValueT<real>(name, .0f));
            return values.size() - 1;
        }
        throw ERROR("Value not found");
    }
    
    vector<real> getVariables() const {
        vector<real> reals;
        for (const ValueT<real>& value: values)
            if (!value.isConstant()) reals.push_back(value);
        return reals;
    }
    
    void setVariables(const vector<real>& variables) {
        size_t activeIndex = 0;
        size_t variablesSize = variables.size();
        for (ValueT<real>& value: values)
            if (!value.isConstant() && activeIndex < variablesSize)
                value = variables[activeIndex++];
        if (variablesSize != activeIndex)
            throw ERROR("Values size mismatch, " + to_string(variablesSize) + " != " + to_string(activeIndex));
    }
    
    vector<real> getLowerBounds() const {
        vector<real> bounds;
        for (const ValueT<real>& value: values)
            if (!value.isConstant()) bounds.push_back(value.getLower());
        return bounds;
    }
    
    vector<real> getUpperBounds() const {
        vector<real> bounds;
        for (const ValueT<real>& value: values)
            if (!value.isConstant()) bounds.push_back(value.getUpper());
        return bounds;
    }

    IniData getAsIniData() const {
        IniData iniData;
        for (const ValueT<real>& value: values) {
            string section = value.getNameCRef();
            unordered_map<string, string> map = value.getAsUMapStr();
            for (const auto& [key, val]: map)
                iniData.set(key, val, section);
        }
        return iniData;
    }
    
    // size_t size() const { return values.size(); }

    // void clear() { values.clear(); }
    
    string dump(bool show = false) const {
        vector<string> dumps;
        for (const ValueT<real>& value: values)
            dumps.push_back(value.dump());
        string output = implode("\n", dumps);
        if (show) cout << output << endl;
        return output;
    }

private:
    vector<ValueT<real>> values;

    void convert() {
        static const string sectionKey = "name";
        static const string ksep = "=";
        static const string vsep = "\n";

        values.clear();
        for (const string& section: inifile.sections()) {
            if (section.empty())
                // ... (globals in the ini file are not a part of a ValueT)
                continue;

            string valueStr = sectionKey + ksep + section;
            for (const auto& [key, value]: inifile.list<string>(section))
                valueStr += vsep + key + ksep + value;
            
            ValueT<real> value;
            value.fromString(valueStr, ksep, vsep);
            values.push_back(value);
        }
    }
};

// template<typename... Ts>
// ValuesT(...) -> ValuesT<float, uint32_t>;

using Values = ValuesT<float>; //, uint32_t>;

// using real = float;
