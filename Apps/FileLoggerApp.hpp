#pragma once

template<typename A>
class FileLoggerApp: public App<FileLogger, A> {
public:
    using App<FileLogger, A>::App;
    virtual ~FileLoggerApp() {}
};
