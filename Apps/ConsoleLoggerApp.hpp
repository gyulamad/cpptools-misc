#pragma once

#include "../App.hpp"
#include "../ConsoleLogger.hpp"

template<typename A>
class ConsoleLoggerApp: public App<ConsoleLogger, A> {
public:
    using App<ConsoleLogger, A>::App;
    virtual ~ConsoleLoggerApp() {}
};
