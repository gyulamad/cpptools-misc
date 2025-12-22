#pragma once

#include <string>
#include <iostream>
#include "ERROR.hpp"

using namespace std;

// deprecated, use Executor instead
static string execute(const char* cmd, bool shows = false) {
    if (shows) cout << cmd << endl;
    string result = "";
    char buffer[128];
    FILE* pipe = popen(cmd, "r");
    if (!pipe) throw ERROR("popen() failed!");
    while (!feof(pipe))
        if (fgets(buffer, 128, pipe) != nullptr) result += buffer;
    int status = pclose(pipe);
    if (status == -1) throw ERROR("pclose() failed!");
    if (shows) cout << result << endl;
    return result;
}

// deprecated, use Executor instead
static string execute(const string& cmd, bool shows = false) {
    return execute(cmd.c_str(), shows);
}
