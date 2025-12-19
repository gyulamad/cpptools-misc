#pragma once

#include "../App.hpp"

template<typename A>
class NullLoggerApp: public App<NullLogger, A> {
public:
    using App<NullLogger, A>::App;
    virtual ~NullLoggerApp() {}
};
