#pragma once

#include <string>
#include <vector>
#include "datetime_defs.hpp"
#include "get_cwd.hpp"
#include "get_absolute_path.hpp"
#include "implode.hpp"
#include "Executor.hpp"
#include "highlight_compiler_outputs.hpp"
#include "file_exists.hpp"
#include "file_get_contents.hpp"
#include "explode.hpp"
#include "filemtime_ms.hpp"
#include "unlink.hpp"
#include "in_array.hpp"
#include "regx_match.hpp"
#include "trim.hpp"
#include "array_merge.hpp"
#include "get_path.hpp"
#include "get_extension_only.hpp"
#include "str_contains.hpp"
#include "is_dir.hpp"
#include "mkdir.hpp"
#include "file_put_contents.hpp"
#include "replace_extension.hpp"
#include "get_filename_ext.hpp"
#include "EWHAT.hpp"
#include "Logger.hpp"
#include <future>
#include "Dependency.hpp"
#include "ucfirst.hpp"
#include <dlfcn.h>
#include "array_shift.hpp"
#include "sort.hpp"
#include "array_remove.hpp"
#include "sort.hpp"
#include "array_unique.hpp"
#include "trim.hpp"
#include "QUOTEME.hpp"
#include "array_key_exists.hpp"

using namespace std;

const vector<string> 
BUILDER_DEFAULT_MODES = 
#ifdef BUILDER_MODES
    sort(array_unique(array_remove(trim(explode(",", BUILDER_MODES)), "")))
#else
    {}
#endif
;

const bool 
BUILDER_DEFAULT_VERBOSE = 
#ifdef BUILDER_VERBOSE
    true
#else
    false
#endif
;

struct DependencyArgumentPlugins {
    vector<string> dependencyFlags;
    vector<string> dependencyLibs;
    vector<string> dependencyIncs;
};

class Builder {
public:
    Builder(
        const vector<string>& modes = BUILDER_DEFAULT_MODES, 
        bool verbose = BUILDER_DEFAULT_VERBOSE
    ): 
        // DynLoader(modes, verbose)
        modes(modes), 
        verbose(verbose)
    {}

    // Destructor: Clean up all loaded objects and libraries
    virtual ~Builder() {
        // Destroy all loaded objects
        for (auto& objPair : objects)
            if (objPair.first && objPair.second)
                objPair.second(objPair.first);

        // Close all libraries
        for (auto& libPair : libraries)
            if (libPair.second.handle)
                dlclose(libPair.second.handle);
    }

    void setModes(const vector<string>& modes) { this->modes = modes; }
    void setVerbose(bool verbose) { this->verbose = verbose; }


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
            [&obj_to_destroy](const pair<void*, void (*)(void*)>& objPair) {
                if (objPair.first == obj_to_destroy && objPair.second) {
                    objPair.second(obj_to_destroy); // Call the specific destroy function
                    obj_to_destroy = nullptr;
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

protected:

    // Structure to hold library handle and destroy function for each library
    struct LibInfo {
        void* handle;
        void (*destroy)(void*); // Generic destroy function, cast to specific type when needed
        LibInfo(void* h, void (*d)(void*)) : handle(h), destroy(d) {}
    };

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

    void buildSourceFile(
        const string& sourceFile,
        const string& outputFile, 
        const vector<string>& flags,
        const vector<string>& includeDirs,
        const vector<string>& linkObjectFiles,
        const vector<string>& libs,
        // BuildType buildType
        const bool strict,
        const bool verbose
    ) const {
        if (verbose) LOG("Building source file: " + F(F_FILE, sourceFile));
        // NOTE: known bug in gcc shows warning: #pragma once in main file
        // see: https://gcc.gnu.org/bugzilla/show_bug.cgi?id=64117
        // const string wrapperFile = 
        //     get_path(outputFile) + "/" + get_filename(sourceFile) + ".wrp";
        // if (buildType == BT_PRECOMPILED_HEADER && !file_exists(wrapperFile))
        //     file_put_contents(
        //         wrapperFile,
        //         "#include \"" + sourceFile + "\"\n", 
        //         false, true
        //     );
        const string arguments = 
            (!flags.empty() ? " " + implode(" ", flags) : "") +
            (!includeDirs.empty() ? " " + FLAG_INCLDIR + implode(" " + FLAG_INCLDIR, includeDirs) : "") + 
            // + (buildType == BT_PRECOMPILED_HEADER ? " -x c++-header" : "")
            " " + FLAG_OUTPUT + " " + outputFile + " " +
            // + (buildType == BT_PRECOMPILED_HEADER ? wrapperFile : sourceFile);
            sourceFile + 
            (!linkObjectFiles.empty() ? " " + implode(" ", linkObjectFiles) : "") +
            (!libs.empty() ? " " + implode(" ", libs) : "");

        if (strict) {
            // buildCmd("iwyu" + arguments, verbose); // TODO - make iwyu optional

            // all header files should starts with '#pragma once'
            string outputs, errors;
            Executor::execute(
                "find . -path \"./libs/*\" -prune -o "
                "\\( -name \"*.h\" -o -name \"*.hpp\" \\) "
                "! -name \"*.wrapper.h\" ! -name \"*.wrapper.hpp\" -print | "
                "while read f; do "
                    "head -n1 \"$f\" | grep -q \"#pragma once\" || echo \"$f\"; "
                "done",
                &outputs, &errors);
            if (!outputs.empty() || !errors.empty())
                throw ERROR(
                    "Some header files does not starts with '#pragma once':\n" 
                    + outputs + (errors.empty() ? "" : "\nErrors:\n" + errors));
        }
        buildCmd(GXX + arguments, verbose);
        
        // if (buildType == BT_PRECOMPILED_HEADER)
        //     file_copy(outputFile, get_path(sourceFile), true);
    }

    void buildCmd(const string& command, bool show = false) const {
        string outputs, errors;
        if (show) cout << command << endl;
        int err = Executor::execute(command, &outputs, &errors, false);
        // if (show || err) cout << command << endl;
        if (show && !outputs.empty()) cout << outputs << endl;
        if ((show || err) && !errors.empty()) cerr << highlight_compiler_outputs(errors) << endl;
        if (err)
            throw ERROR("Compile failed: " + to_string(err));
    }


    Dependency* loadDependency(const string& dependency) {
        Dependency* dependencyPtr = nullptr;
        string
            creator = DEFAULT_DEPENDENCY_CREATOR,
            library = DEFAULT_DEPENDENCY_LIBRARY,
            version = DEFAULT_DEPENDENCY_VERSION;
        const vector<string> splits =
            explode(SEP_DEPENDENCY_VERSION, dependency);
        vector<string> splits0 =
            explode(SEP_DEPENDENCY_LIBRARY, splits[0]);
        if (splits0.size() == 2) creator = array_shift(splits0);
        library = splits0[0];
        if (splits.size() == 2) version = splits[1];
        if (creator == DEFAULT_DEPENDENCY_CREATOR) creator = library;
        if (library == DEFAULT_DEPENDENCY_LIBRARY)
            throw ERROR("Unnamed library in dependency: " + dependency);
        const string libClassName = ucfirst(library) + "Dependency";
        const string libPathName = get_absolute_path (
            /*__DIR__ + "/" +*/ DIR_DEPENDENCIES + "/"
            + creator + "/" + library + "/" + libClassName,
            false
        );
        if (verbose) {
            // lock_guard<mutex> outputLock(outputMutex);
            LOG("Loading dependency: " + F(F_HIGHLIGHT, libClassName) + " from " + F(F_FILE, libPathName));
        }
        {
            lock_guard<mutex> loaderLock(loaderMutex);
            dependencyPtr = load<Dependency>(libPathName);
            dependencyPtr->setVersion(version);
            if (verbose) LOG("Checking if dependency is installed... " + libPathName + ":" + version);
            if (!dependencyPtr->installed()) {
                if (verbose) LOG("Dependency installation needed...");
                dependencyPtr->install();
            }
            if (verbose) LOG("Dependency installed.");
        }
        return dependencyPtr;
    }

    vector<vector<string>> getIncludesAndImplementationsAndDependencies(
        time_ms& lastfmtime,
        const string& basePath, 
        const string& sourceFile, 
        const string& buildPath,
        const vector<string>& flags,
        const vector<string>& includeDirs,
        vector<string>& foundImplementations,
        vector<string>& foundDependencies,
        vector<string>& visitedSourceFiles,
        unordered_map<string, vector<vector<string>>>& mappedSourceFilesToDeps,
        const bool pch,
        //const bool verbose, // TODO: fetch maxPchThreads from and optional command line arguments
        const unsigned int maxPchThreads, // = thread::hardware_concurrency() // controls PCH build parallelism, 0=auto, 1=sequential, N=limit to N
        bool throwsIfRecursion
    ) {

        // if (in_array(sourceFile, visitedSourceFiles)) {
        if (array_key_exists(sourceFile, mappedSourceFilesToDeps)) {
            visitedSourceFiles = array_remove(visitedSourceFiles, sourceFile);
            return mappedSourceFilesToDeps[sourceFile];
        }
        // visitedSourceFiles.push_back(sourceFile);
        // mappedSourceFilesToDeps[sourceFile] = {{}, {}, {}};

        if (in_array(sourceFile, visitedSourceFiles)) {
            runtime_error err = ERROR("Recursion? " + F(F_FILE, sourceFile) + "\nVisited path:\n" + implode("\n", visitedSourceFiles));
            if (throwsIfRecursion)
                throw err;
            else {
                LOG(err.what());
                visitedSourceFiles = array_remove(visitedSourceFiles, sourceFile);
                if (array_key_exists(sourceFile, mappedSourceFilesToDeps))
                    return mappedSourceFilesToDeps[sourceFile];
                else {
                    LOG(ERROR("No mapped dependency for: " + F(F_FILE, sourceFile)).what());
                    return {};
                }
            }
        }
        visitedSourceFiles.push_back(sourceFile);

        lastfmtime = max(lastfmtime, filemtime_ms(sourceFile));
        // waitFutures(pchBuilderFutures);
        // pchBuilderFutures.reserve(maxPchThreads);
        // const string cacheFile = replaceToBuildPath(sourceFile, buildPath) + EXT_DEP;
        const string cacheFile = getCachePath(sourceFile, buildPath, DIR_DEP_FOLDER, EXT_DEP);
        if (file_exists(cacheFile)) {
            if (verbose) LOG("Load dependencies from cache for " + F(F_FILE, sourceFile));
            string cache = file_get_contents(cacheFile);
            if (cache.empty()) {
                visitedSourceFiles = array_remove(visitedSourceFiles, sourceFile);
                mappedSourceFilesToDeps[sourceFile] = {{}, {}, {}};
                return mappedSourceFilesToDeps[sourceFile];
            }

            vector<string> 
            splits =  explode(CACHE_SEP_IMP, cache);
            if (splits.size() != 2)
                throw ERROR("Invalid cache format: " + F(F_FILE, cacheFile) 
                    + " - missing marker: " + CACHE_SEP_IMP);
            vector<string> incs_and_imps_and_deps = splits;
            
            splits = explode(CACHE_SEP_DEP, incs_and_imps_and_deps[1]);
            if (splits.size() != 2)
                throw ERROR("Invalid cache format: " + F(F_FILE, cacheFile) 
                    + " - missing marker: " + CACHE_SEP_DEP);
            incs_and_imps_and_deps[1] = splits[0];
            incs_and_imps_and_deps.push_back(splits[1]);

            vector<string> includes = explode("\n", incs_and_imps_and_deps[0]);
            if (includes.size() == 1 && includes[0].empty()) includes = {};
            vector<string> implementations = explode("\n", incs_and_imps_and_deps[1]);
            if (implementations.size() == 1 && implementations[0].empty()) implementations = {};
            vector<string> dependencies = explode("\n", incs_and_imps_and_deps[2]);
            if (dependencies.size() == 1 && dependencies[0].empty()) dependencies = {};
            
            time_ms local_max = filemtime_ms(sourceFile);
            for (const string& include: includes)
                local_max = max(local_max, filemtime_ms(include));                

            // TODO: ??? build may not need to depend on implementations - but I keep it for now in case if some crazy one includes the .cpp directly...
            for (const string& implementation: implementations)
                local_max = max(local_max, filemtime_ms(implementation));

            if (filemtime_ms(cacheFile) < local_max) {
                unlink(cacheFile);
                if (file_exists(cacheFile))
                    throw ERROR("Unable to delete: " + F(F_FILE, cacheFile));
                // visitedSourceFiles = array_remove(visitedSourceFiles, sourceFile);
                mappedSourceFilesToDeps.erase(sourceFile);
                if (verbose) LOG("Dependency cache invalidated, source file need to revisit...");
            } else {
                lastfmtime = max(lastfmtime, local_max);
                visitedSourceFiles = array_remove(visitedSourceFiles, sourceFile);
                mappedSourceFilesToDeps[sourceFile] = { includes, implementations, dependencies };
                return mappedSourceFilesToDeps[sourceFile];
            }
        }
        
        if (verbose) LOG("Collecting dependencies for " + F(F_FILE, sourceFile));
        

        // NOTE: it may only a warning but I keep it as an error 
        // as I do not want recursive includes in my project 
        // and I want to detect it early when it happening
        // if (/*!pch &&*/ in_array(sourceFile, visitedSourceFiles)) // TODO: It may not needed if pch because it we build them one by one...???
        //     throw ERROR("Source file already visited (possible include recursion?): " + F(F_FILE, sourceFile) + " - Note: In case you changed the #include dependencies cleanup your build folder.");
        // else // TODO throw ERROR ^^^^ Only if --no-pch
            
        // visitedSourceFiles.push_back(sourceFile);

        if (verbose) LOG("Searching includes in " + F(F_FILE, sourceFile));

        vector<string> buildCommands;
        string sourceCode = file_get_contents(sourceFile);
        vector<string> sourceLines = explode("\n", sourceCode);
        vector<string> matches;
        vector<string> includes;
        vector<string> implementations;
        vector<string> dependencies;
        int line = 0;
        try {
            for (const string& sourceLine: sourceLines) {
                line++; 
                if (regx_match(RGX_DEPENDENCY, sourceLine, &matches)) {
                    if (verbose) LOG("Dependency found: " + matches[0] + " in " + F_FILE_LINE(sourceFile, line));
                    const vector<string> splits = explode(",", matches[1]);
                    for (const string& split: splits) {
                        const string dependency = trim(split);
                        if (!in_array(dependency, dependencies)) {
                            loadDependency(dependency);
                            dependencies.push_back(dependency);
                            foundDependencies = array_merge(foundDependencies, dependencies);
                        }
                    }
                }
                if (regx_match(RGX_INCLUDE, sourceLine, &matches)) {
                    if (verbose) LOG("Include found: " + matches[0]);
                    DependencyArgumentPlugins dependencyArgumentPlugins = getDependenciesArgumentPlugins(foundDependencies);
                    const vector<string> foundIncludes = lookupFileInIncludeDirs(
                        get_path(sourceFile), matches[1], 
                        array_merge(includeDirs, dependencyArgumentPlugins.dependencyIncs), false, true
                    );
                    if (foundIncludes.empty())
                        throw ERROR("Include file not found: " + matches[0] 
                            + " at " + F_FILE_LINE(sourceFile, line));
                    if (foundIncludes.size() > 1)
                        throw ERROR("Multipe file found: " + matches[0]
                            + " at " + F_FILE_LINE(sourceFile, line));

                    const string include = foundIncludes[0];

                    if (in_array(include, includes)) 
                        continue;

                    includes.push_back(include);
                    lastfmtime = max(lastfmtime, filemtime_ms(include));

                    // === PCH PRECOMPILATION LOGIC (using wrapper to avoid #pragma once warning) ===
                    // Only precompile actual headers (.h / .hpp), skip other files
                    // string ext = "." + get_extension_only(include);                    
                    if (pch && in_array("." + get_extension_only(include), EXTS_H_HPP)) {
                        string pchFile = getPchPath(include, buildPath);
                        string wrapperFile = getPchWrapperPath(include, buildPath);
                        string pchDir = get_path(pchFile);

                        // Rebuild if PCH missing, header newer, or wrapper newer than PCH
                        bool needsRebuild = !file_exists(pchFile) ||
                                            filemtime_ms(include) > filemtime_ms(pchFile) ||
                                            filemtime_ms(wrapperFile) > filemtime_ms(pchFile);

                        if (needsRebuild) {
                            if (verbose) LOG("Needs rebuild precompiled header (via wrapper): " + F(F_FILE, include) + " -> " + pchFile);
                            
                            if (!is_dir(pchDir)) {
                                if (!mkdir(pchDir, 0777, true))
                                    throw ERROR("Failed to create PCH directory: " + pchDir);
                            }

                            // Create wrapper if missing or header changed
                            if (!file_exists(wrapperFile) || filemtime_ms(include) > filemtime_ms(wrapperFile)) {
                                string wrapperContent = "#include \"" + include + "\"\n";
                                file_put_contents(wrapperFile, wrapperContent, false, true);
                            }

                            // Build command for PCH using the wrapper (no #pragma once → no warning)
                            string pchArgs =
                                " " + implode(" ", flags) + " " +
                                FLAG_INCLDIR + implode(" " + FLAG_INCLDIR, array_merge(includeDirs, dependencyArgumentPlugins.dependencyIncs)) + " " +
                                "-x c++-header " + // TODO: once it's added to gcc use this instead wrapper files: -Wno-pragma-once-outside-header " +
                                wrapperFile + " " +
                                FLAG_OUTPUT + " " + pchFile;

                            buildCommands.push_back(GXX + pchArgs);
                            DBG("collected: " + GXX + pchArgs);
                            // if (pchBuilderFutures.size() >= maxPchThreads) waitFutures(pchBuilderFutures);
                            // pchBuilderFutures.push_back(async(launch::async, [this, wrapperFile, pchArgs, /*verbose,*/ &lastfmtime, pchFile]() {
                            //     buildCmd(GXX + pchArgs, verbose);
                            //     {
                            //         lock_guard<mutex> lock(lastPchFMTimeMutex);
                            //         lastfmtime = max(lastfmtime, filemtime_ms(pchFile));
                            //     }
                            // }));
                            // if (verbose) LOG("Building on thread(s): " + to_string(pchBuilderFutures.size()) + "...");
                            lastfmtime = max(lastfmtime, get_time_ms());
                        } else if (verbose) {
                            LOG("Using cached PCH: " + pchFile);
                            lastfmtime = max(lastfmtime, filemtime_ms(pchFile));
                        }

                        // === END PCH LOGIC ===
                    }

                    vector<vector<string>> cache = 
                        getIncludesAndImplementationsAndDependencies(
                            lastfmtime, 
                            basePath, 
                            include, 
                            buildPath,
                            flags, 
                            includeDirs,
                            foundImplementations,
                            foundDependencies,
                            visitedSourceFiles,
                            mappedSourceFilesToDeps,
                            pch,
                            //verbose,
                            maxPchThreads,
                            throwsIfRecursion
                        );
                    if (cache.empty())
                        break;
                        
                    includes = array_merge(includes, cache[0]);
                    implementations = array_merge(implementations, cache[1]);
                    dependencies = array_merge(dependencies, cache[2]);

                    // look up for implementations...
                    // if (verbose) {
                    //     for (const string& implementation: implementations)
                    //         LOG("Implementation found: " + F(F_FILE, implementation));
                    // }
                    foundImplementations = array_merge(
                        foundImplementations,
                        lookupFileInIncludeDirs(
                            get_path(sourceFile), 
                            matches[1], 
                            array_merge(includeDirs, dependencyArgumentPlugins.dependencyIncs), 
                            false, 
                            false, 
                            EXTS_C_CPP
                        )
                    );

                    foundDependencies = array_merge(
                        foundDependencies,
                        dependencies
                    );
                }
            }
        } catch (exception &e) {
            // runBuildCommands(buildCommands, maxPchThreads);
            waitFutures(pchBuilderFutures);
            throw ERROR("Include search failed at " 
                + F_FILE_LINE(sourceFile, line) + EWHAT);
        }
        const string cachePath = get_path(cacheFile);
        if (!is_dir(cachePath)) {
            if (file_exists(cachePath)) {
                // runBuildCommands(buildCommands, maxPchThreads);
                waitFutures(pchBuilderFutures);
                throw ERROR("Cache path is not a folder: " + cachePath);
            }
            else if (!mkdir(cachePath, 0777, true)) {
                // runBuildCommands(buildCommands, maxPchThreads);
                waitFutures(pchBuilderFutures);
                throw ERROR("Unable to create folder: " + cachePath);
            }
        }
        file_put_contents(
            cacheFile, 
            (!includes.empty() ? implode("\n", includes) : "") + 
            CACHE_SEP_IMP + 
            (!implementations.empty() ? implode("\n", implementations) : "") +
            CACHE_SEP_DEP + 
            (!dependencies.empty() ? implode("\n", dependencies) : ""), 
            false, true
        );
        runBuildCommands(buildCommands, maxPchThreads);
        // waitFutures(pchBuilderFutures);
        visitedSourceFiles = array_remove(visitedSourceFiles, sourceFile);
        mappedSourceFilesToDeps[sourceFile] = { includes, implementations, dependencies };
        return mappedSourceFilesToDeps[sourceFile];
    }

    DependencyArgumentPlugins getDependenciesArgumentPlugins(const vector<string>& dependencies) {
        DependencyArgumentPlugins dependencyArgumentPlugins;
        // vector<string> dependencyFlags;
        // vector<string> dependencyLibs;
        // vector<string> dependencyIncs;
        for (const string& dependency: dependencies) {
            // string
            //     creator = DEFAULT_DEPENDENCY_CREATOR,
            //     library = DEFAULT_DEPENDENCY_LIBRARY,
            //     version = DEFAULT_DEPENDENCY_VERSION;
            // const vector<string> splits =
            //     explode(SEP_DEPENDENCY_VERSION, dependency);
            // vector<string> splits0 =
            //     explode(SEP_DEPENDENCY_LIBRARY, splits[0]);
            // if (splits0.size() == 2) creator = array_shift(splits0);
            // library = splits0[0];
            // if (splits.size() == 2) version = splits[1];
            // if (creator == DEFAULT_DEPENDENCY_CREATOR) creator = library;
            // if (library == DEFAULT_DEPENDENCY_LIBRARY)
            //     throw ERROR("Unnamed library in dependency: " + dependency);
            // const string libClassName = ucfirst(library) + "Dependency";
            // const string libPathName = get_absolute_path (
            //     /*__DIR__ + "/" +*/ DIR_DEPENDENCIES + "/"
            //     + creator + "/" + library + "/" + libClassName
            // );
            // if (verbose) {
            //     lock_guard<mutex> outputLock(outputMutex);
            //     LOG("Loading dependency: " + F(F_HIGHLIGHT, libClassName) + " from " + F(F_FILE, libPathName));
            // }
            // {
            //     lock_guard<mutex> loaderLock(loaderMutex);
                // Dependency* dependency = loader.load<Dependency>(libPathName);
                // dependency->install(version);
                Dependency* deptr = loadDependency(dependency);
                dependencyArgumentPlugins.dependencyFlags = array_merge(dependencyArgumentPlugins.dependencyFlags, deptr->flags());
                dependencyArgumentPlugins.dependencyLibs = array_merge(dependencyArgumentPlugins.dependencyLibs, deptr->libs());
                dependencyArgumentPlugins.dependencyIncs = array_merge(dependencyArgumentPlugins.dependencyIncs, deptr->incs());
            // }
        }
        return dependencyArgumentPlugins;
    }

    void runBuildCommands(const vector<string>& buildCommands, unsigned int maxPchThreads) {
        waitFutures(pchBuilderFutures);
        for (const string& buildCommand: buildCommands) {
            if (pchBuilderFutures.size() >= maxPchThreads) waitFutures(pchBuilderFutures);
            pchBuilderFutures.push_back(async(launch::async, [this, buildCommand]() {
                buildCmd(buildCommand, verbose);
            }));
            if (verbose) LOG("Build command(s) running on thread " + to_string(pchBuilderFutures.size()) + " of " + to_string(maxPchThreads) + "...");
        }
        waitFutures(pchBuilderFutures);
    } 

    void waitFutures(vector<future<void>>& futures) {
        for (future<void>& f: futures) if (f.valid()) f.get();
        futures.clear();
    }

    string getPchPath(
        const string& headerFile,
        const string& buildPath
    ) const {
        return getCachePath(headerFile, buildPath, DIR_PCH_FOLDER, EXT_GCH);
        // // Mirrors header path into .build-<modes>/pch/ structure
        // string relative = str_replace(DIR_BASE_PATH + "/", "", get_absolute_path(headerFile));
        // return fix_path(buildPath + "/" + DIR_PCH_FOLDER + "/" + relative + EXT_GCH);
    }

    string getCachePath(
        const string& inputFile,
        const string& buildPath,
        const string& cachePath,
        const string& ext
    ) const {
        // Mirrors header path into .build-<modes>/xyz/ structure
        string relative = str_replace(DIR_BASE_PATH + "/", "", get_absolute_path(inputFile));
        return fix_path(buildPath + "/" + cachePath + "/" + relative + ext);
    
    }

    string getPchWrapperPath(const string& headerFile, const string& buildPath) const {
        // e.g. .build-debug/pch/core/utils.hpp.wrapper.hpp
        string relative = str_replace(DIR_BASE_PATH + "/", "", get_absolute_path(headerFile));
        return fix_path(buildPath + "/" + DIR_PCH_FOLDER + "/" + relative + ".wrapper.hpp");
    }

    vector<string> lookupFileInIncludeDirs(
        const string& basePath,
        const string& include, 
        const vector<string>& includeDirs,
        bool stopAtFirstFound,
        bool throwsIfNotFound,
        vector<string> extensions = {}
    ) const {
        bool stop = false;
        vector<string> results;
        if (extensions.empty()) { 
            string extension = get_extension_only(include);
            extensions = extension.empty() ? EXTS_H_HPP : vector({ extension });
        }
        for (const string& extension: extensions) {
            if (stop) break;
            string includePath = replace_extension(
                get_absolute_path(fix_path(basePath + "/" + include), false), 
                extension
            );
            if (file_exists(includePath)) {
                results.push_back(includePath);
                if (stopAtFirstFound) break;
                continue;
            }
            for (const string& includeDir: includeDirs) {
                if (stop) break;
                includePath = 
                    get_absolute_path(fix_path(trim(includeDir) + "/" + include), false);
                if (file_exists(includePath)) {
                    results.push_back(includePath);
                    if (stopAtFirstFound) {
                        stop = true;
                        break;
                    }
                    continue;
                }
            }
        }
        if (throwsIfNotFound && results.empty())
            throw ERROR("Include not found: " + include);
        if (verbose && !results.empty())
            LOG("Source code(s) found.");
        return results;
    }



    // Private helper: Load a shared library or get existing handle
    void* loadLibrary(
        const string& path, 
        vector<string> modes, 
        const bool verbose
    ) const {
        #ifdef DEBUG
        modes.push_back("debug");
        #endif
        modes = array_unique(modes);
        sort(modes);

        if (verbose) LOG("Attempt to load shared library: " + F(F_FILE, path) + " (modes: " + (!modes.empty() ? implode(",", modes) : "<none>") + ")");
            
        const string buildPath = getBuildFolder(
            DIR_BUILD_PATH, // TODO: to parameter
            modes,
            SEP_MODES // TODO: to parameter
        ); // TODO: to parameter

        string libPath = get_absolute_path(replace_extension(path, ".so"), false);
        libPath = replaceToBuildPath(libPath, buildPath); // fixPath(path);
        
        auto it = libraries.find(libPath);
        if (it != libraries.end()) {
            return it->second.handle;
        }

        // rebuild only once before it really have to be loaded 
        // so that it does not take time to rebuild everything when debug builds
        string pathWithCppExt = replace_extension(path, ".cpp");
        string cppPath = get_absolute_path(pathWithCppExt);
    
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

    string getBuildFolder(
        const string& buildPath,
        const vector<string>& modes,
        const string& sep
    ) const {
        return fix_path(
            buildPath + (!modes.empty() ? sep + implode(sep, modes) : "")
        );
    }

    string replaceToBuildPath(
        const string& sourceFile,
        string buildPath = ""
    ) const {
        const string sourceFileName = get_filename_ext(sourceFile);
        const string sourceFilePath = get_absolute_path(get_path(sourceFile));
        const string basePath = DIR_BASE_PATH;
        if (buildPath.empty()) buildPath = DIR_BUILD_PATH;
        return fix_path(str_replace(
            basePath, buildPath + "/", sourceFilePath
        ) + "/" + sourceFileName);
    }

    // ========= GIVEN ==========

    vector<string> modes;
    bool verbose;

    // ========= OWN ==========

    mutex loaderMutex;
    vector<future<void>> pchBuilderFutures;
    // mutable std::mutex lastPchFMTimeMutex;  // mutable if used in const methods

    // vector<string> flags = { "--strict", "--fast" }; // TODO: pass the flags from the build with command line arguments -DXXXX constant maybe?

    // Map of library path to its handle and destroy function
    unordered_map<string, LibInfo> libraries;
    // Vector to track loaded objects and their destroy functions
    vector<pair<void*, void (*)(void*)>> objects;

    // ========= CONFIG ==========

    const string GXX = "ccache g++";

    // build folder
    const string DIR_BASE_PATH = get_absolute_path(get_cwd());
    const string DIR_BUILD_FOLDER = ".build";
    const string DIR_BUILD_PATH = fix_path(DIR_BASE_PATH + "/" + DIR_BUILD_FOLDER);
    const string DIR_DEP_FOLDER = "";

    const string DIR_PCH_FOLDER = "";  // Subfolder for precompiled headers
    const string EXT_GCH = ".gch";        // Precompiled header extension

    const string RGX_INCLUDE = "^\\s*#include\\s*\"([^\"]+)\"\\s*";
    const string RGX_DEPENDENCY = "^\\s*//\\s*DEPENDENCY\\s*:\\s*([^\"]+)\\s*";

    const string SEP_PRMS = ",";
    const string SEP_MODES = "-";

    const string EXT_O = ".o";
    const string EXT_SO = ".so";
    const string EXT_DEP = ".dep";

    const vector<string> EXTS_H_HPP = { ".h", ".hpp" };
    const vector<string> EXTS_C_CPP = { ".c", ".cpp" };

    const vector<string> PTRN_EXTS_C_CPP = { "*.c", "*.cpp" };

    const string CACHE_SEP_IMP = "\n<=== [ INCS | IMPS ] ===>\n";
    const string CACHE_SEP_DEP = "\n<=== [ IMPS | DEPS ] ===>\n";


    const string FLAG_COMPILE = "-c";
    const string FLAG_LIBRARY = "-l";
    const string FLAG_INCLDIR = "-I";
    const string FLAG_OUTPUT = "-o";

    const string DEFAULT_DEPENDENCY_CREATOR = "";
    const string DEFAULT_DEPENDENCY_LIBRARY = "";
    const string DEFAULT_DEPENDENCY_VERSION = "master";

    const string SEP_DEPENDENCY_LIBRARY = "/";
    const string SEP_DEPENDENCY_VERSION = ":";

    const string DIR_DEPENDENCIES = get_cwd() + "/autobuild/dependencies";
};

