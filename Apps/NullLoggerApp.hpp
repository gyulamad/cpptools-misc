#pragma once

template<typename A>
class NullLoggerApp: public App<NullLogger, A> {
public:
    using App<NullLogger, A>::App;
    virtual ~NullLoggerApp() {}
};
