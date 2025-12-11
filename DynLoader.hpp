#pragma once

#include <dlfcn.h>
#include <string>
#include <stdexcept>
#include <unordered_map>
#include <vector>
#include <tuple>

#include "ERROR.hpp"
#include "get_absolute_path.hpp"
#include "Logger.hpp"
#include "F.hpp"
#include "replace_extension.hpp"
#include "remove_extension.hpp"
// #include "execute.hpp"
#include "file_exists.hpp"
#include "highlight_compiler_outputs.hpp"
#include "Executor.hpp"
#include "__DIR__.hpp"
#include "get_filename_only.hpp"
#include "get_extension_only.hpp"
#include "sort.hpp"

#include "../../autobuild/Builder.hpp"
#include "../../autobuild/Dependency.hpp"

using namespace std;

class DynLoader: public Builder {
public:
    // Constructor
    DynLoader(vector<string> modes = {}, bool verbose = false): Builder(modes, verbose) {};

    // Destructor: Clean up all loaded objects and libraries
    virtual ~DynLoader() {
        // Destroy all loaded objects
        for (auto& objPair : objects)
            if (objPair.first && objPair.second)
                objPair.second(objPair.first);

        // Close all libraries
        for (auto& libPair : libraries)
            if (libPair.second.handle)
                dlclose(libPair.second.handle);
    }

    // Load a shared library and create an instance of type T with no arguments
    template <typename T, typename... Args>
    T* load(const string& path, Args&&... args) {
        void* handle = loadLibrary(path, modes, verbose);
        // Select the appropriate CreateFunc type based on whether arguments are provided
        using CreateFunc = typename conditional<
            sizeof...(Args) == 0,
            T* (*)(),
            T* (*)(Args...)
        >::type;
        using DestroyFunc = void (*)(T*);
        
        CreateFunc create = nullptr;
        DestroyFunc destroy = nullptr;

        // Load symbols
        loadSymbols(path, handle, create, destroy);

        // Create the object based on whether arguments are provided
        T* obj;
        if constexpr (sizeof...(Args) == 0) {
            obj = create();
        } else {
            obj = create(forward<Args>(args)...);
        }
        
        if (!obj)
            throw ERROR("Failed to create object from library: " + path);

        // Store object and its destroy function
        objects.emplace_back(obj, reinterpret_cast<void (*)(void*)>(destroy));
        return obj;
    }

    // Destroy a specific object created by the loader
    template <typename T>
    void destroy(T* obj_to_destroy) {
        // Find the object in the vector
        auto it = remove_if(objects.begin(), objects.end(),
            [obj_to_destroy](const pair<void*, void (*)(void*)>& objPair) {
                if (objPair.first == obj_to_destroy && objPair.second) {
                    objPair.second(objPair.first); // Call the specific destroy function
                    return true; // Mark for removal
                }
                return false;
            });

        if (it == objects.end()) {
            // Object not found, might be an error or already destroyed externally
            // For now, let's just log a warning or throw, depending on desired strictness.
            // For this implementation, we'll throw an error.
            throw ERROR("Attempted to destroy an object not managed by DynLoader or already destroyed.");
        }
        objects.erase(it, objects.end()); // Remove the destroyed object from the vector
    }

private:
    // vector<string> flags = { "--strict", "--fast" }; // TODO: pass the flags from the build with command line arguments -DXXXX constant maybe?

    // Structure to hold library handle and destroy function for each library
    struct LibInfo {
        void* handle;
        void (*destroy)(void*); // Generic destroy function, cast to specific type when needed
        LibInfo(void* h, void (*d)(void*)) : handle(h), destroy(d) {}
    };

    // Map of library path to its handle and destroy function
    unordered_map<string, LibInfo> libraries;
    // Vector to track loaded objects and their destroy functions
    vector<pair<void*, void (*)(void*)>> objects;


    // Private helper: Load a shared library or get existing handle
    void* loadLibrary(
        const string& path, 
        vector<string> modes, 
        const bool verbose
    ) const {
        #ifdef DEBUG
        modes.push_back("debug");
        #endif
        sort(modes);

        if (verbose) LOG("Attempt to load shared library: " + F(F_FILE, path) + " (modes: " + (!modes.empty() ? implode(",", modes) : "<none>") + ")");
            
        const string buildPath = getBuildFolder(
            DIR_BUILD_PATH, // TODO: to parameter
            modes,
            SEP_MODES // TODO: to parameter
        ); // TODO: to parameter

        string libPath = get_absolute_path(replace_extension(path, ".so"));
        libPath = replaceToBuildPath(libPath, buildPath); // fixPath(path);
        
        auto it = libraries.find(libPath);
        if (it != libraries.end()) {
            return it->second.handle;
        }

        // rebuild only once before it really have to be loaded 
        // so that it does not take time to rebuild everything when debug builds
        
        string cppPath = get_absolute_path(replace_extension(path, ".cpp"));
    
        if (!file_exists(cppPath)) 
            throw ERROR("Cound not (re)build shared library, file not found: " 
                + F(F_FILE, cppPath));

        if (verbose) LOG("Build shared library: " + F(F_FILE, cppPath) + " (mode flags: " + (!modes.empty() ? implode(",", modes) : "<none>") + ")");

        // TODO: test buildPath and libPath
        string cmd = "./builder " + cppPath // + " " + libPath 
            + (!modes.empty() ? " --mode=" + implode(",", modes) : "")
            //+ (!buildPath.empty() ? " --build-folder=" + buildPath : "") 
            + (verbose ? " --verbose" : "") // TODO: to parameter
            + " --shared"// 2>&1"
        ;

        string errs;
        if (verbose) LOG("Execute: " + cmd);
        int err = Executor::execute(cmd, nullptr, &errs, false);
        if (err)
            throw ERROR("Library build failed (" + to_string(err) + "):\n" 
                + highlight_compiler_outputs("\nError:\n" + errs));
        

        // Load the shared library
        if (verbose) LOG("Loading shared library: " + F(F_FILE, libPath));
        void* handle = dlopen(libPath.c_str(), RTLD_LAZY | RTLD_GLOBAL);
        if (!handle)
            throw ERROR("Failed to load library: " + string(dlerror()));
        
        return handle;
    }

    // Private helper: Load create and destroy symbols, store library info if needed
    template <typename T, typename... Args>
    void loadSymbols(
        const string& libPath, 
        void* handle, 
        T* (*&create)(Args...), 
        void (*&destroy)(T*)
    ) {
        // Reset dlerror
        dlerror();

        // Load the create function
        create = reinterpret_cast<T* (*)(Args...)>(dlsym(handle, "create"));
        const char* dlsym_error = dlerror();
        if (dlsym_error) {
            dlclose(handle);
            throw ERROR("Failed to load create function: " + string(dlsym_error));
        }

        // Load the destroy function
        destroy = reinterpret_cast<void (*)(T*)>(dlsym(handle, "destroy"));
        dlsym_error = dlerror();
        if (dlsym_error) {
            dlclose(handle);
            throw ERROR("Failed to load destroy function: " + string(dlsym_error));
        }

        // Store library info if not already present
        if (libraries.find(libPath) == libraries.end()) {
            libraries.emplace(libPath, LibInfo(handle, reinterpret_cast<void (*)(void*)>(destroy)));
        }
    }
};


#ifdef TEST

#include "TEST.hpp" // For the TEST macro and assert
#include "str_contains.hpp" // For str_contains in exception testing
#include "test_dummies/DummyLibraryInterface.hpp"

// Using namespace std as per convention
using namespace std;

// Test case for DynLoader::destroy with a single object
TEST(test_DynLoader_destroy_singleObject) {
    //string output = 
    capture_cout([&]() {
        DynLoader loader;
        DummyLibraryInterface* obj = loader.load<DummyLibraryInterface>(__DIR__ + "/test_dummies/DummyLibraryImplementation1");
        assert(obj!= nullptr && "Object should be loaded.");
        obj->greet(); // Verify object is functional

        loader.destroy(obj); // Explicitly destroy the object
        // At this point, obj should be freed and removed from loader's internal list.
        // The destructor of DynLoader should not attempt to free it again.
    });
}

// Test case for DynLoader::destroy with multiple objects and selective destruction
TEST(test_DynLoader_destroy_multipleObjects_selective) {
    //string output = 
    capture_cout([&]() {

        DynLoader loader;

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
TEST(test_DynLoader_destructor_remainingObjects) {
    //string output = 
    capture_cout([&]() {
        DynLoader loader;
        DummyLibraryInterface* obj1 = loader.load<DummyLibraryInterface>(__DIR__ + "/test_dummies/DummyLibraryImplementation1");
        DummyLibraryInterface* obj2 = loader.load<DummyLibraryInterface>(__DIR__ + "/test_dummies/DummyLibraryImplementation1");
        assert(obj1!= nullptr && "obj1 should be loaded.");
        assert(obj2!= nullptr && "obj2 should be loaded.");
        
        loader.destroy(obj1); // obj1 should be destroyed, obj2 remains
    }); // loader goes out of scope, obj2 should be destroyed by DynLoader's destructor
}

#endif
