#pragma once

#include "Logger.hpp"
#include "Arguments.hpp"
#include <functional>

template<typename L, typename A>
class App {
public:
    App() {}

    virtual ~App() {
        if (args) delete args;
        args = nullptr;
    }

    int run(int argc, char* argv[]) {
        try {
            createLogger<L>();
            args = new A(argc, argv);
            process();
        } catch (exception &e) {
            LOG_ERROR("Exception" + EWHAT);
            return 1;
        }
        return 0;
    }

protected:
    virtual void process() = 0;

    A* args = nullptr;
};
