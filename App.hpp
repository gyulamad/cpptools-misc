#pragma once

#include "TEST.hpp"
#include "ConsoleLogger.hpp"

#include "Arguments.hpp"
#include <functional>

template<typename L, typename A>
class App {
public:
    App(int argc, char* argv[]): args(argc, argv) {
        createLogger<ConsoleLogger>();
        tester.run({ });
    }

    virtual ~App() {}

    operator int() { return run(); }

protected:

    int run() {
        try {
            createLogger<L>();
            result = process();
        } catch (exception &e) {
            LOG_ERROR("Exception" + EWHAT);
        }
        return result;
    }

    virtual int process() = 0;

    A args;
    int result = 1;
};
