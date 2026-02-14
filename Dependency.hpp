#pragma once

#include <vector>
#include <string>
#include "EXTERN.hpp"

using namespace std;

#define EXTERN_DEPENDENCY(clazz) EXTERN(clazz, (), ())

class Dependency {
public:
    Dependency() {}
    virtual ~Dependency() {}

    virtual void setVersion(string VERSION) {
        this->VERSION = VERSION;
    }

    virtual void install() = 0;
    virtual bool installed() = 0;
    virtual vector<string> flags() = 0;
    virtual vector<string> libs() = 0;
    virtual vector<string> incs() = 0;
protected:
    string VERSION;
};
