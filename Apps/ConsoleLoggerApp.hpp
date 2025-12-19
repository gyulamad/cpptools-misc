#pragma once

#include "../App.hpp"

template<typename A>
class ConsoleLoggerApp: public App<ConsoleLogger, A> {
public:
    using App<ConsoleLogger, A>::App;
    virtual ~ConsoleLoggerApp() {}
};
