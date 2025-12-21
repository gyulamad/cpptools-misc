#pragma once

#include <functional>
#include <iostream>
#include "TeeStreamBuf.hpp"
#include <sstream>
#include "ERROR.hpp"

using namespace std;

string capture_cout_cerr(function<void()> func, bool show = false) {
    streambuf* original_cout_buffer = cout.rdbuf();
    streambuf* original_cerr_buffer = cerr.rdbuf();
    stringstream buffer;

    // Create TeeStreamBuf for cout if show is true
    TeeStreamBuf tee_cout(original_cout_buffer, buffer.rdbuf());
    TeeStreamBuf tee_cerr(original_cerr_buffer, buffer.rdbuf());

    // Set up streams based on show parameter
    if (show) {
        cout.rdbuf(&tee_cout);
        cerr.rdbuf(&tee_cerr);
    } else {
        cout.rdbuf(buffer.rdbuf());
        cerr.rdbuf(buffer.rdbuf());
    }

    string ewhat;
    try {
        func();
    } catch (exception& e) {
        ewhat = e.what();
    }
    cout.rdbuf(original_cout_buffer);
    cerr.rdbuf(original_cerr_buffer);
    cout.clear();
    cerr.clear();
    string output = buffer.str();
    if (!ewhat.empty()) 
        throw runtime_error(string("Exception in stdout and stderr capture: ") + ewhat + 
            (output.empty() ? "" : "\nOutput captured:\n" + output));
    return output;
}
