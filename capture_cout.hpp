#pragma once

#include <string>
#include <functional>
#include <iostream>
#include <sstream>
#include "ERROR.hpp"
#include "TeeStreamBuf.hpp"

using namespace std;

string capture_cout(function<void()> func, bool show = false) {
    streambuf* original_cout_buffer = cout.rdbuf();
    stringstream buffer;

    // Create TeeStreamBuf for cout if show is true
    TeeStreamBuf tee_cout(original_cout_buffer, buffer.rdbuf());

    // Set up streams based on show parameter
    if (show) cout.rdbuf(&tee_cout);
    else cout.rdbuf(buffer.rdbuf());
    
    string ewhat;
    try {
        func();
    } catch (exception& e) {
        ewhat = e.what();
    }
    cout.rdbuf(original_cout_buffer);
    cout.clear();
    string output = buffer.str();
    if (!ewhat.empty()) 
        throw ERROR("Exception in stdout capture: " + ewhat + 
            (output.empty() ? "" : "\nOutput captured:\n" + output));
    return output;
}
