#pragma once

#include "../TEST.hpp"
#include "../Builder.hpp"

#ifdef TEST



#include "../str_contains.hpp"
#include "test_dummies/DummyLibraryInterface.hpp"
#include "../__DIR__.hpp"
#include "../capture_cout.hpp"

// Using namespace std as per convention
using namespace std;

// Test case for DynLoader::destroy with a single object
TEST(test_Builder_destroy_singleObject) {
    //string output = 
    capture_cout([&]() {
        Builder loader({}, false);
        DummyLibraryInterface* obj = loader.load<DummyLibraryInterface>(__DIR__ + "/test_dummies/DummyLibraryImplementation1");
        assert(obj!= nullptr && "Object should be loaded.");
        obj->greet(); // Verify object is functional

        loader.destroy(obj); // Explicitly destroy the object
        // At this point, obj should be freed and removed from loader's internal list.
        // The destructor of DynLoader should not attempt to free it again.
    });
}

// Test case for DynLoader::destroy with multiple objects and selective destruction
TEST(test_Builder_destroy_multipleObjects_selective) {
    //string output = 
    capture_cout([&]() {

        Builder loader({}, false);

        DummyLibraryInterface* obj1 = nullptr;
        DummyLibraryInterface* obj2 = nullptr;
        DummyLibraryInterface* obj3 = nullptr;

        obj1 = loader.load<DummyLibraryInterface>(__DIR__ + "/test_dummies/DummyLibraryImplementation1");
        obj2 = loader.load<DummyLibraryInterface>(__DIR__ + "/test_dummies/DummyLibraryImplementation1");
        obj3 = loader.load<DummyLibraryInterface>(__DIR__ + "/test_dummies/DummyLibraryImplementation1");

        assert(obj1!= nullptr && "obj1 should be loaded.");
        assert(obj2!= nullptr && "obj2 should be loaded.");
        assert(obj3!= nullptr && "obj3 should be loaded.");

        obj1->greet();
        obj2->greet();
        obj3->greet();

        loader.destroy(obj2); // Destroy obj2
        // obj1 and obj3 should still be managed by the loader
        
        // Attempting to destroy obj2 again should throw an error
        bool threw = false;
        try {
            loader.destroy(obj2);
        } catch (const exception& e) {
            assert(str_contains(e.what(), "Attempted to destroy an object not managed by DynLoader or already destroyed.") && "Expected error message not found.");
            threw = true;
        }

        assert(threw && "destroy(obj2) should throw an error on second call.");
    });
}

// Test case for DynLoader destructor with remaining objects
TEST(test_Builder_destructor_remainingObjects) {
    //string output = 
    capture_cout([&]() {
        Builder loader({}, false);
        DummyLibraryInterface* obj1 = loader.load<DummyLibraryInterface>(__DIR__ + "/test_dummies/DummyLibraryImplementation1");
        DummyLibraryInterface* obj2 = loader.load<DummyLibraryInterface>(__DIR__ + "/test_dummies/DummyLibraryImplementation1");
        assert(obj1!= nullptr && "obj1 should be loaded.");
        assert(obj2!= nullptr && "obj2 should be loaded.");
        
        loader.destroy(obj1); // obj1 should be destroyed, obj2 remains
    }); // loader goes out of scope, obj2 should be destroyed by DynLoader's destructor
}

#endif

