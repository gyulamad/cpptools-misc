#pragma once

class DummyLibraryInterface {
public:
    virtual ~DummyLibraryInterface() = default;
    virtual void greet() = 0;
};