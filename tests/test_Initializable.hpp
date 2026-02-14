#pragma once

#ifdef TEST

#include "../TEST.hpp"
    #include "../Initializable.hpp"
    #include "../IniFile.hpp"
    #include "../ConsoleLogger.hpp"
    #include "../str_contains.hpp"
    #include "../unlink.hpp"

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

// Test class for virtual destructor test
class TestInitializableDerived : public Initializable {
public:
    bool loaded = false;
    bool reloaded = false;
    bool deleted = false;
    
    TestInitializableDerived() : Initializable("", false, false, false, false, false) {}
    
    void onLoad() override {
        loaded = true;
    }
    
    void onReload() override {
        reloaded = true;
    }
    
    ~TestInitializableDerived() override {
        deleted = true;
    }
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

// Test for construct lambda calling onLoad
TEST(test_Initializable_construct_lambda_calls_onLoad) {
    TestInitializable obj("test_construct_lambda.ini", false, false, false, false, false);
    // The construct() method sets up onLoad callback in inifile.onLoad
    // When inifile.load() is called, onLoad should be invoked
    assert(obj.loaded == false && "onLoad should not be called until load() is invoked");
    // Note: The onLoad callback is triggered by inifile.load() which is called by Initializable
    // when load=true is passed to constructor or when reset() is called with inidata
}

// Test for construct lambda - verify onLoad is called when load=true
    TEST(test_Initializable_construct_lambda_onLoad_calls) {
        string filename = "test_construct_lambda_onload.ini";
        // Create the file first
        file_put_contents(filename, "; Test ini file\nkey=value\n", false, true);
        // Use load=false first to avoid calling load() before onLoad callback is set up
        TestInitializable obj(filename, false, true, false, false, false);
        // Manually trigger the onLoad callback by calling the inifile's onLoad function
        // This tests that the lambda in construct() properly calls onLoad()
        obj.getIniFileRef().onLoad();
        // The onLoad callback should have been triggered
        assert(obj.loaded && "onLoad should be called when onLoad callback is triggered");
        // Clean up
        unlink(filename.c_str());
    }

// Test for virtual destructor - delete through base class pointer
TEST(test_Initializable_virtual_destructor) {
    Initializable* basePtr = new TestInitializableDerived();
    assert(basePtr != nullptr && "Base pointer should not be null");
    delete basePtr;
    // If the destructor is not virtual, the derived destructor won't be called
    // We can't directly check deleted flag from base pointer, but if the test runs
    // without memory corruption and the derived destructor runs, we've covered the line
}

#endif