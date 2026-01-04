#pragma once

#include "IniData.hpp"
#include "IniFile.hpp"
#include "../misc/EXTERN.hpp"

#define EXTERN_INITIALIZABLE(clazz) EXTERN(clazz, ( \
    const string& inifname, \
    bool load, \
    bool createIfNotExists, \
    bool throwsIfNotExists, \
    bool warnsIfNotExists, \
    bool verbose \
), ( \
    inifname, \
    load, \
    createIfNotExists, \
    throwsIfNotExists, \
    warnsIfNotExists, \
    verbose \
))

class Initializable {
protected:
    void construct() {
        inifile.onLoad = [this]() { 
            onLoad(); 
        };
    }
public:
    Initializable(
        bool createIfNotExists,
        bool throwsIfNotExists,
        bool warnsIfNotExists,
        bool verbose
    ):
        inifile(
            createIfNotExists,
            throwsIfNotExists,
            warnsIfNotExists,
            verbose
        )
    {
        construct();
    }

    Initializable(
        const string& inifname, 
        bool load, // = false, 
        bool createIfNotExists,
        bool throwsIfNotExists,
        bool warnsIfNotExists,
        bool verbose
    ):
        inifile(
            inifname,
            load,
            createIfNotExists,
            throwsIfNotExists,
            warnsIfNotExists,
            verbose
        )
    {
        construct();
    }

    virtual ~Initializable() {}
    
    // virtual void init(const string& inifname, bool createIfNotExists, bool throwsIfNotExists) {
    //     inifile.load(inifname, createIfNotExists, throwsIfNotExists);
    // }

    virtual void reset(const IniData* inidata = nullptr) {
        if (inidata) inifile.setData(inidata->getDataCRef());
        onReload();
    }    

    const IniFile& getIniFileCRef() const { return inifile; }
    IniFile& getIniFileRef() { return inifile; }

    template<typename T>
    T get(const string& key, const string& section = "") const {
        try {
            return inifile.get<T>(key, section);
        } catch (exception &e) {
            throw ERROR("Unable to get init value at key " + EMPTY_OR(key) + " at section [" + EMPTY_OR(section, "<global>") + "]" + EWHAT);
        }
    }

    virtual void onLoad() = 0;

    virtual void onReload() = 0;

protected:
    IniFile inifile;
};
