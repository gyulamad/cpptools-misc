#pragma once

#include <iostream>
#include "../../EXTERN.hpp"
#include "DummyLibraryInterface.hpp"

using namespace std;

// Class name in UpperCaseFirst
class DummyLibraryImplementation1: public DummyLibraryInterface {
public:
    // gamelCase method names
    DummyLibraryImplementation1() { cout << "DummyObject constructed." << endl; }
    ~DummyLibraryImplementation1() { cout << "DummyObject destroyed." << endl; }
    void greet() { cout << "Hello from DummyObject!" << endl; }
};

// Use EXTERN macro for create and destroy functions
EXTERN(DummyLibraryImplementation1, (), ());