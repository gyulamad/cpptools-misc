#pragma once

#include "__DIR__.hpp"
#include <vector>
#include <string>

using namespace std;


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
