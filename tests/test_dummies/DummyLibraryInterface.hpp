#pragma once

#include "../../safe.hpp"

class DummyLibraryInterface {
public:
    virtual ~DummyLibraryInterface() = default;
    virtual void greet() = 0;
};