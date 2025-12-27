#pragma once

#include "Initializable.hpp"

using namespace std;

class Parameters: public Initializable {
public:
    Parameters(
        const string& inifname, 
        bool createIfNotExists = false, 
        bool throwsIfNotExists = true
    ):
        Initializable() 
    {
        init(inifname, createIfNotExists, throwsIfNotExists);
    }

    virtual ~Parameters() {}

    void onLoad() {
        // TODO ...
    }

    void onReload() {
        // TODO ...
    }
};
