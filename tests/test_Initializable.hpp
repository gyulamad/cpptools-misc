#pragma once

#ifdef TEST

#include "../TEST.hpp"
#include "../Initializable.hpp"
#include "../IniFile.hpp"
#include "../ConsoleLogger.hpp"
#include "../str_contains.hpp"

using namespace std;

// Test class that implements the pure virtual methods
class TestInitializable : public Initializable {
public:
    TestInitializable(
        const string& inifname = "",
        bool load = false,
        bool createIfNotExists = false,
        bool throwsIfNotExists = false,
        bool warnsIfNotExists = false,
        bool verbose = false
    ):
        Initializable(
            inifname,
            load,
            createIfNotExists,
            throwsIfNotExists,
            warnsIfNotExists,
            verbose
        )
    {}

    void onLoad() override {
        loaded = true;
    }

    void onReload() override {
        reloaded = true;
    }

    bool loaded = false;
    bool reloaded = false;
};

// Test for Initializable constructor
TEST(test_Initializable_constructor) {
    TestInitializable obj;
    // Constructor should work without throwing
}

TEST(test_Initializable_constructor_with_filename) {
    TestInitializable obj("test.ini", false, false, false, false, false);
    // Constructor with filename should work
}

TEST(test_Initializable_getIniFile) {
    TestInitializable obj;
    const IniFile& iniRef = obj.getIniFileCRef();
    IniFile& iniRefMutable = obj.getIniFileRef();
    (void)iniRef;  // Suppress unused variable warning
    (void)iniRefMutable;  // Suppress unused variable warning
    // Should be able to get both const and mutable references
}

TEST(test_Initializable_onLoad) {
    TestInitializable obj;
    obj.onLoad();
    assert(obj.loaded && "onLoad should set loaded flag");
}

TEST(test_Initializable_onReload) {
    TestInitializable obj;
    obj.onReload();
    assert(obj.reloaded && "onReload should set reloaded flag");
}

TEST(test_Initializable_reset) {
    TestInitializable obj;
    obj.reset();
    // reset should work without throwing
}

#endif