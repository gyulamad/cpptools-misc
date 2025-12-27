#pragma once

#include "Dependency.hpp"
#include "get_absolute_path.hpp"
#include "ConsoleLogger.hpp"
#include "Executor.hpp"

class GithubDependency: public Dependency {
public:
    using Dependency::Dependency;
    virtual ~GithubDependency() {}

    virtual void install() override {
        createLogger<ConsoleLogger>();

        // Install and build json with the selected version (if not already)
        if(!installed()) { // already instelled?
            LOG("Install dependency...");
            const string BASE = fix_path(DIR_LIBS + "/..");
            // DBG(DIR_LIBS);
            const string cmd = DIR_DEPENDENCIES + REPO + "/install.sh " + REPO + " " + BASE + " " + TARGET + " " + VERSION;
            // DBG(REPO);
            // DBG(BASE);
            // DBG(TARGET);
            // DBG(VERSION);
            LOG("Execute: " + cmd);
            Executor::execute(cmd);
        }
    }

    virtual bool installed() override {
        const string jsonf = TARGET + "/" + VERSION;
        // LOG("Checking json installed: " + jsonf);
        return file_exists(jsonf);
    }

protected:

    virtual void setRepo(string REPO) {
        this->REPO = REPO;
        this->TARGET = fix_path(DIR_LIBS + "/" + REPO + "/");
    }

    virtual string getPath() {
        return get_absolute_path(DIR_LIBS + "/" + REPO + "/" + VERSION);
    }

    string REPO;
    string TARGET;
    const string DIR_LIBS = "libs/";
    const string DIR_DEPENDENCIES = "autobuild/dependencies/"; // TODO: it goes to the scare ho.. no, to the misc/ or next to the Dependecy class
};