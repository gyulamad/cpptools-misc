#pragma once

#include "Logger.hpp"
#include "EWHAT.hpp"

template<typename L, typename A>
class App {
public:
    App(int argc, char* argv[]): args(argc, argv) {}

    virtual ~App() {}

    operator int() { return run(); }

protected:

    int run() {
        try {
            createLogger<L>();
            result = process();
        } catch (exception &e) {
            LOG_ERROR("Exception" + EWHAT);
            result = 1;
        }
        return result;
    }

    virtual int process() = 0;

    A args;
    int result = 0;
};
