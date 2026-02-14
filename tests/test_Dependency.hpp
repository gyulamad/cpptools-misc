#pragma once

#ifdef TEST

#include "../TEST.hpp"
#include "../Dependency.hpp"
#include "../str_contains.hpp"

using namespace std;

// Simple test class that implements all pure virtual methods
class TestDependency : public Dependency {
public:
    void install() override {
        // No-op for testing
    }
    
    bool installed() override {
        return true;
    }
    
    vector<string> flags() override {
        return {"-DTEST"};
    }
    
    vector<string> libs() override {
        return {"test"};
    }
    
    vector<string> incs() override {
        return {"/usr/include/test"};
    }
};

// Test for Dependency base class
TEST(test_Dependency_constructor) {
    TestDependency dep;
    // Constructor should work
}

TEST(test_Dependency_setVersion) {
    TestDependency dep;
    dep.setVersion("test-version");
    // Note: VERSION is protected, so we can't directly assert its value
    // The method is tested indirectly through derived classes
}

TEST(test_Dependency_virtualMethods) {
    TestDependency dep;
    vector<string> flags = dep.flags();
    vector<string> libs = dep.libs();
    vector<string> incs = dep.incs();
    // These should return the values we defined
    assert(flags.size() == 1 && "flags() should return 1 element");
    assert(flags[0] == "-DTEST" && "flags() should return -DTEST");
    assert(libs.size() == 1 && "libs() should return 1 element");
    assert(libs[0] == "test" && "libs() should return test");
    assert(incs.size() == 1 && "incs() should return 1 element");
    assert(incs[0] == "/usr/include/test" && "incs() should return /usr/include/test");
}

TEST(test_Dependency_installed) {
    TestDependency dep;
    assert(dep.installed() && "installed() should return true");
}

TEST(test_Dependency_install) {
    TestDependency dep;
    dep.install(); // Should not throw
}

#endif