#pragma once

#include "ERROR.hpp"
#include "Logger.hpp"
#include <fstream>

using namespace std;

#define DEFAULT_LOGFILE "app.log"

class FileLogger: public Logger {
public:
    FileLogger() : FileLogger(DEFAULT_LOGFILE, "Log filename is not specified, using default log: " DEFAULT_LOGFILE) {}
    FileLogger(const string& filename, const string& openmsg = "") : filename(filename) {
        file.open(filename, ios::app);
        if (!file.is_open())
            throw ERROR("Unable to open log file: " + filename);
        if (!openmsg.empty())
            write(openmsg);
    }

    ~FileLogger() {
        if (file.is_open()) file.close();
    }

    void write(const string& output) override {
        if (file.is_open()) {
            file << output << endl;
            file.flush();
        }
    }

private:
    ofstream file;
    string filename;
};