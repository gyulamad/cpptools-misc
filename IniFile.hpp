#pragma once

#include "get_absolute_path.hpp"
#include "file_get_contents.hpp"
#include "explode.hpp"
#include "trim.hpp"
#include "file_put_contents.hpp"
#include "file_exists.hpp"
#include "Logger.hpp"
#include "EWHAT.hpp"
#include <functional>
#include "IniData.hpp"
#include "ConsoleLogger.hpp"

using namespace std;

class IniFile: public IniData {    
protected:
    void construct(
        string filename,
        bool load,
        bool createIfNotExists,
        bool throwsIfNotExists,
        bool warnsIfNotExists
    ) {
        if (!logger()) createLogger<ConsoleLogger>();
        if (!filename.empty()) {
            filename = get_absolute_path(filename, false);
            if (load) this->load(filename, createIfNotExists, throwsIfNotExists, warnsIfNotExists);
            else if (!setFilename(filename, createIfNotExists, throwsIfNotExists, warnsIfNotExists))
                throw ERROR("Unknown ini file error: " + filename);
        }
    }
public:
    IniFile(
        const string& filename,
        bool load = false, 
        bool createIfNotExists = false,
        bool throwsIfNotExists = false,
        bool warnsIfNotExists = false,
        bool verbose = false
    ): 
        IniData(),
        verbose(verbose)
    {
        construct(
            filename,
            load,
            createIfNotExists,
            throwsIfNotExists,
            warnsIfNotExists
        );
    }

    IniFile(
        bool createIfNotExists = false,
        bool throwsIfNotExists = false,
        bool warnsIfNotExists = false,
        bool verbose = false
    ):
        IniData(),
        verbose(verbose)
    {
        construct(
            "",
            false,
            createIfNotExists,
            throwsIfNotExists,
            warnsIfNotExists
        );
    }

    virtual ~IniFile() {}

    function<void()> onLoad = nullptr;
    function<void()> onSave = nullptr;
    function<void()> onSaveAs = nullptr;

    const string& getFilenameCRef() const { return filename; }

    void load(bool createIfNotExists = false, bool throwsIfNotExists = true, bool warnsIfNotExists = false) {
        load(this->filename, createIfNotExists, throwsIfNotExists, warnsIfNotExists);
    }

    void load(const string& filename, bool createIfNotExists = false, bool throwsIfNotExists = true, bool warnsIfNotExists = false) {
        if (!setFilename(filename, createIfNotExists, throwsIfNotExists, warnsIfNotExists)) return;

        string section = "";
        data.clear();
        if (verbose) LOG("Loading ini file: " + F(F_FILE, this->filename));
        vector<string> lines = explode("\n", file_get_contents(this->filename));
        size_t size = lines.size();
        for (size_t ln = 0; ln < size; ln++) {
            string cleanln = trim(explode(";", explode("#", lines[ln])[0])[0]);
            if (cleanln.empty()) continue;
            if (cleanln.starts_with('[') && cleanln.ends_with(']')) {
                section = trim(cleanln, "[]");
                continue;
            }
            vector<string> splits = explode("=", cleanln);
            string key = trim(splits[0]);
            if (splits.size() < 2)
                throw ERROR("Value is missing at key '" + EMPTY_OR(key) + "' in " + EMPTY_OR(filename) + ":" + ::to_string(ln + 1));
            string val = trim(splits[1]);
            data[section][key] = val;
        }

        if (onLoad) onLoad();
        changed = false;
    }

    void save(bool createIfNotExists = false, bool throwsIfNotExists = true, bool warnsIfNotExists = false) {
        save(this->filename, false, createIfNotExists, throwsIfNotExists, warnsIfNotExists);
    }

    void save(const string& filename, bool keepFilenameChanged = true, bool createIfNotExists = false, bool throwsIfNotExists = true, bool warnsIfNotExists = false) {
        string _filename = this->filename;
        bool saveAs = setFilename(filename, createIfNotExists, throwsIfNotExists, warnsIfNotExists);

        string contents;
        for (const auto& [section, values]: data) {
            contents += section.empty() ? "" : "[" + section + "]\n";
            for (const auto& [key, val]: values)
                contents += key + "=" + val + "\n";
            contents += "\n";
        }
        
        bool error = false;
        string errmsg = "";
        try {
            file_put_contents(this->filename, contents, false, true);
        } catch (exception& e) {
            error = true;
            errmsg = "IniFile save failed: " + EWHAT;
        }

        if (!keepFilenameChanged)
            this->filename = _filename;

        if (error)
            throw ERROR(errmsg);

        if (saveAs) {
            if (onSaveAs) onSaveAs();
        } else 
            if (onSave) onSave();
        changed = false;
    }

    template<typename T>
    T get(const string& key, const string& section = "") const {
        try {
            return IniData::get<T>(key, section);
        } catch (exception &e) {
            throw ERROR("Unable to get ini file value at key " + EMPTY_OR(key) + " at section [" + EMPTY_OR(section, "<global>") + "] in file " + EMPTY_OR(filename) + "" + EWHAT);
        }
    }

    template<typename T>
    T getopt(const string& key, const T defval, const string& section = "", bool warns = true) const {
        if (warns && !has(key, section))
            LOG_WARN("Unable to get optional ini file value at key " + EMPTY_OR(key) + " at section [" + EMPTY_OR(section, "<global>") + "] in file " + EMPTY_OR(filename));
        return IniData::getopt<T>(key, defval, section, warns);
    }

    [[nodiscard]]
    bool setFilename(string filename, bool createIfNotExists, bool throwsIfNotExists, bool warnsIfNotExists) {
        filename = filename.empty() ? "" : get_absolute_path(filename, false);
        bool changed = false;

        if (!filename.empty())
            if ((changed = (filename != this->filename))) {
                this->filename = filename;
            }
            
        if (this->filename.empty())
            throw ERROR("Filename is empty or undefined.");

        if (!file_exists(this->filename)) {
            if (createIfNotExists) {
                if (verbose) LOG("Create file: " + F(F_FILE, this->filename));
                file_put_contents(this->filename, "; Autogenerated initialization file\n\n", false, true);
            } else {
                const string msg = "File not found: " + F(F_FILE, this->filename);
                if (throwsIfNotExists)
                    throw ERROR(msg);
                else if (warnsIfNotExists)
                    LOG_WARN(msg);
            }
        }

        this->changed = this->changed || changed;
        return changed;
    }

protected:
    string filename = "";
    bool verbose = false;
};
