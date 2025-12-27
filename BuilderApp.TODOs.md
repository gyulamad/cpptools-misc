# BuilderApp TODOs: Hardcoded Values to Bubble Up

This document lists hardcoded values in `Builder.hpp` and `BuilderApp.hpp` that can be moved to configuration files or command line arguments.

---

## Builder.hpp - Hardcoded Values

### 1. Show Commands Flag
**File:** `Builder.hpp`  
**Line:** 253  
**Code:**
```cpp
const bool showcmds = true; // TODO: bubble up
```
**Function:** [`buildCmd()`](Builder.hpp:252)  
**Suggestion:** Add to command line arguments (e.g., `--show-commands`)

---

### 2. Compiler Command
**File:** `Builder.hpp`  
**Line:** 846  
**Code:**
```cpp
const string GXX = "ccache g++";
```
**Location:** Class member  
**Suggestion:** Config file option `compiler` or CLI arg `--compiler`

---

### 3. Build Folder Configuration
**File:** `Builder.hpp`  
**Lines:** 849-851  
**Code:**
```cpp
const string DIR_BASE_PATH = get_absolute_path(get_cwd());
const string DIR_BUILD_FOLDER = ".build";
const string DIR_BUILD_PATH = fix_path(DIR_BASE_PATH + "/" + DIR_BUILD_FOLDER);
```
**Location:** Class member  
**Suggestion:** Already partially configurable via `--build-folder`, but `DIR_BUILD_FOLDER` (`.build`) is hardcoded

---

### 4. Dependency Cache Folder
**File:** `Builder.hpp`  
**Line:** 852  
**Code:**
```cpp
const string DIR_DEP_FOLDER = "";
```
**Location:** Class member  
**Suggestion:** Config option `dep_cache_folder` or CLI arg `--dep-cache-folder`

---

### 5. Precompiled Header Folder
**File:** `Builder.hpp`  
**Line:** 854  
**Code:**
```cpp
const string DIR_PCH_FOLDER = "";  // Subfolder for precompiled headers
```
**Location:** Class member  
**Suggestion:** Config option `pch_folder` or CLI arg `--pch-folder`

---

### 6. Precompiled Header Extension
**File:** `Builder.hpp`  
**Line:** 855  
**Code:**
```cpp
const string EXT_GCH = ".gch";
```
**Location:** Class member  
**Suggestion:** Config option `pch_extension`

---

### 7. Regex Patterns
**File:** `Builder.hpp`  
**Lines:** 857-858  
**Code:**
```cpp
const string RGX_INCLUDE = "^\\s*#include\\s*\"([^\"]+)\"\\s*";
const string RGX_DEPENDENCY = "^\\s*//\\s*DEPENDENCY\\s*:\\s*([^\"]+)\\s*";
```
**Location:** Class member  
**Suggestion:** Config options `include_regex`, `dependency_regex`

---

### 8. Separators
**File:** `Builder.hpp`  
**Lines:** 860-861, 885-886  
**Code:**
```cpp
const string SEP_PRMS = ",";
const string SEP_MODES = "-";
const string SEP_DEPENDENCY_LIBRARY = "/";
const string SEP_DEPENDENCY_VERSION = ":";
```
**Location:** Class member  
**Suggestion:** Config options for each separator

---

### 9. File Extensions
**File:** `Builder.hpp`  
**Lines:** 863-868  
**Code:**
```cpp
const string EXT_O = ".o";
const string EXT_SO = ".so";
const string EXT_DEP = ".dep";
const vector<string> EXTS_H_HPP = { ".h", ".hpp" };
const vector<string> EXTS_C_CPP = { ".c", ".cpp" };
```
**Location:** Class member  
**Suggestion:** Config options for each extension type

---

### 10. Glob Patterns
**File:** `Builder.hpp`  
**Line:** 870  
**Code:**
```cpp
const vector<string> PTRN_EXTS_C_CPP = { "*.c", "*.cpp" };
```
**Location:** Class member  
**Suggestion:** Config option `source_patterns`

---

### 11. Cache Separators
**File:** `Builder.hpp`  
**Lines:** 872-873  
**Code:**
```cpp
const string CACHE_SEP_IMP = "\n<=== [ INCS | IMPS ] ===>\n";
const string CACHE_SEP_DEP = "\n<=== [ IMPS | DEPS ] ===>\n";
```
**Location:** Class member  
**Suggestion:** Config options for cache separators

---

### 12. Compiler Flags
**File:** `Builder.hpp`  
**Lines:** 876-879  
**Code:**
```cpp
const string FLAG_COMPILE = "-c";
const string FLAG_LIBRARY = "-l";
const string FLAG_INCLDIR = "-I";
const string FLAG_OUTPUT = "-o";
```
**Location:** Class member  
**Suggestion:** Config options for each flag type

---

### 13. Dependency Defaults
**File:** `Builder.hpp`  
**Lines:** 881-883  
**Code:**
```cpp
const string DEFAULT_DEPENDENCY_CREATOR = "";
const string DEFAULT_DEPENDENCY_LIBRARY = "";
const string DEFAULT_DEPENDENCY_VERSION = "master";
```
**Location:** Class member  
**Suggestion:** Config options for default dependency values

---

### 14. Dependencies Directory
**File:** `Builder.hpp`  
**Line:** 888  
**Code:**
```cpp
const string DIR_DEPENDENCIES = get_cwd() + "/autobuild/dependencies";
```
**Location:** Class member  
**Suggestion:** Config option `dependencies_dir` or CLI arg `--dependencies-dir`

---

### 15. TODO Comments in loadLibrary()
**File:** `Builder.hpp`  
**Lines:** 754-757, 779, 782  
**Code:**
```cpp
const string buildPath = getBuildFolder(
    DIR_BUILD_PATH, // TODO: to parameter
    modes,
    SEP_MODES // TODO: to parameter
);
// ...
+ (verbose ? " --verbose" : "") // TODO: to parameter
```
**Function:** [`loadLibrary()`](Builder.hpp:739)  
**Suggestion:** Make `DIR_BUILD_PATH` and `SEP_MODES` configurable

---

## BuilderApp.hpp - Hardcoded Values

### 16. Base Compile Flags
**File:** `BuilderApp.hpp`  
**Line:** 51  
**Code:**
```cpp
const vector<string> FLAGS = { "--std=c++20" };
```
**Location:** Class member  
**Suggestion:** Config option `base_flags` or CLI arg `--std`

---

### 17. Mode-Specific Compile Flags
**File:** `BuilderApp.hpp`  
**Lines:** 52-59  
**Code:**
```cpp
const vector<string> FLAGS_TEST = { "-DTEST" };
const vector<string> FLAGS_DEBUG = { "-g", "-DDEBUG", "-fno-omit-frame-pointer" };
const vector<string> FLAGS_STRICT = { "-pedantic-errors", "-Werror", "-Wall", "-Wextra", "-Wunused", "-fno-elide-constructors" };
const vector<string> FLAGS_FAST = array_merge(FLAGS_STRICT, { "-Ofast", "-fno-fast-math" });
const vector<string> FLAGS_SAFE = array_merge(FLAGS_STRICT, { "-fsanitize-address-use-after-scope", "-fsanitize=undefined", "-fstack-protector" });
const vector<string> FLAGS_SAFE_MEMORY = array_merge(FLAGS_SAFE, { "-fsanitize=address", "-fsanitize=leak" });
const vector<string> FLAGS_SAFE_THREAD = array_merge(FLAGS_SAFE, { "-fsanitize=thread" });    
const vector<string> FLAGS_COVERAGE = { "-fprofile-arcs", "-ftest-coverage" };
```
**Location:** Class member  
**Suggestion:** Config file with sections for each mode (e.g., `[mode.debug]`, `[mode.fast]`)

---

### 18. Shared Library Flags
**File:** `BuilderApp.hpp`  
**Line:** 63  
**Code:**
```cpp
const vector<string> FLAGS_SHARED = { "-fPIC", "-shared" };
```
**Location:** Class member  
**Suggestion:** Config option `shared_flags`

---

### 19. Mode Names
**File:** `BuilderApp.hpp`  
**Lines:** 66-73  
**Code:**
```cpp
const string MODE_NONE = "";
const string MODE_DEBUG = "debug";
const string MODE_FAST = "fast";
const string MODE_TEST = "test";
const string MODE_STRICT = "strict";
const string MODE_SAFE_MEMORY = "safe_memory";
const string MODE_SAFE_THREAD = "safe_thread";
const string MODE_COVERAGE = "coverage";
```
**Location:** Class member  
**Suggestion:** Config option `available_modes` (list of mode names)

---

### 20. Coverage Settings
**File:** `BuilderApp.hpp`  
**Lines:** 88-91  
**Code:**
```cpp
const string COVERAGE_INFO_FILE = "coverage.info";
const string COVERAGE_FOLDER = ".coverage";
const bool COVERAGE_DARK_MODE = true;
const string COVERAGE_BROWSER = "brave-browser --ozone-platform-hint=x11"; // TODO: "brave-browser"..?? (add PREFERED_BROWSER?? as user preference?)
```
**Location:** Class member  
**Suggestion:** Config options `coverage_info_file`, `coverage_folder`, `coverage_dark_mode`, `coverage_browser`

---

### 21. PCH Folder Path
**File:** `BuilderApp.hpp`  
**Line:** 235  
**Code:**
```cpp
const string pchFolder = "/pch"; // TODO "/" + args.get<string>(PRM_PCH_FOLDER)
```
**Function:** [`process()`](BuilderApp.hpp:102)  
**Suggestion:** Add CLI arg `--pch-folder`

---

### 22. Lcov Fixer Script
**File:** `BuilderApp.hpp`  
**Line:** 311  
**Code:**
```cpp
"php autobuild/lcov-fixer.php " // TODO implement lcov-fixer in C++ instead depending on php 
```
**Function:** [`process()`](BuilderApp.hpp:102)  
**Suggestion:** Config option `lcov_fixer_path` or CLI arg `--lcov-fixer`

---

### 23. Artifact Extensions for Cleanup
**File:** `BuilderApp.hpp`  
**Lines:** 503-512  
**Code:**
```cpp
vector<string> artifactExtensionParts = {
    string(EXT_O).erase(0, 1),
    string(EXT_SO).erase(0, 1),
    string(EXT_DEP).erase(0, 1),
    // string(EXT_GCH).erase(0, 1),  // Add .gch cleanup
    // "wrapper.hpp".substr(1),      // cleans *.wrapper.hpp files
    // Others are not defined as constants
    "test", "gdb", "cov", "gcda", "gcno"
};
```
**Function:** [`cleanProject()`](BuilderApp.hpp:486)  
**Suggestion:** Config option `cleanup_extensions`

---

### 24. Excluded Directories for Cleanup
**File:** `BuilderApp.hpp`  
**Lines:** 494-496  
**Code:**
```cpp
if (entry == ".git") continue;
if (entry == "libs") continue;
```
**Function:** [`cleanProject()`](BuilderApp.hpp:486)  
**Suggestion:** Config option `cleanup_exclude_dirs`

---

### 25. Parallel Build Flag
**File:** `BuilderApp.hpp`  
**Lines:** 44, 279-281  
**Code:**
```cpp
const Arguments::Key PRM_PARALLEL { "parallel", "p" };
// ...
// TODO: add parallel flag to command-line arguments
// const bool parallel = true; // TODO: add it to the command line arguments, default to sequential for now?
unsigned int numThreads = 0;
```
**Function:** [`process()`](BuilderApp.hpp:102)  
**Suggestion:** Already has key defined, but not implemented. Add `--parallel` or `--threads=N`

---

## Summary Table

| Category | Hardcoded Value | File:Line | Suggested Config/CLI |
|----------|----------------|-----------|---------------------|
| Display | `showcmds = true` | Builder.hpp:253 | `--show-commands` |
| Compiler | `GXX = "ccache g++"` | Builder.hpp:846 | `--compiler` |
| Paths | `DIR_BUILD_FOLDER = ".build"` | Builder.hpp:850 | `--build-folder-name` |
| Paths | `DIR_DEP_FOLDER = ""` | Builder.hpp:852 | `--dep-cache-folder` |
| Paths | `DIR_PCH_FOLDER = ""` | Builder.hpp:854 | `--pch-folder` |
| Paths | `DIR_DEPENDENCIES` | Builder.hpp:888 | `--dependencies-dir` |
| Extensions | `EXT_GCH = ".gch"` | Builder.hpp:855 | `pch_extension` config |
| Extensions | `EXT_O, EXT_SO, EXT_DEP` | Builder.hpp:863-865 | Config file |
| Extensions | `EXTS_H_HPP, EXTS_C_CPP` | Builder.hpp:867-868 | Config file |
| Regex | `RGX_INCLUDE, RGX_DEPENDENCY` | Builder.hpp:857-858 | Config file |
| Separators | `SEP_PRMS, SEP_MODES` | Builder.hpp:860-861 | Config file |
| Flags | `FLAGS = { "--std=c++20" }` | BuilderApp.hpp:51 | `--std` |
| Flags | `FLAGS_*` (all mode flags) | BuilderApp.hpp:52-59 | Config file sections |
| Coverage | `COVERAGE_BROWSER` | BuilderApp.hpp:91 | `--coverage-browser` |
| Coverage | `COVERAGE_DARK_MODE` | BuilderApp.hpp:90 | `--coverage-dark-mode` |
| Cleanup | `artifactExtensionParts` | BuilderApp.hpp:503 | Config file |
| Cleanup | Excluded dirs (`.git`, `libs`) | BuilderApp.hpp:494-496 | Config file |
| Build | `--parallel` (not implemented) | BuilderApp.hpp:44, 279 | `--threads=N` |

---

## Recommended Config File Structure

```ini
[compiler]
command = ccache g++
base_flags = --std=c++20
flags_compile = -c
flags_library = -l
flags_includedir = -I
flags_output = -o
flags_shared = -fPIC -shared

[paths]
build_folder = .build
dep_cache_folder = 
pch_folder = 
dependencies_dir = autobuild/dependencies

[extensions]
object = .o
shared = .so
dep = .dep
pch = .gch
headers = .h,.hpp
sources = .c,.cpp

[regex]
include = ^\\s*#include\\s*"([^"]+)"\\s*
dependency = ^\\s*//\\s*DEPENDENCY\\s*:\\s*([^"]+)\\s*

[separators]
params = ,
modes = -
dependency_library = /
dependency_version = :

[cache]
separator_imps = \n<=== [ INCS | IMPS ] ===>\n
separator_deps = \n<=== [ IMPS | DEPS ] ===>\n

[modes]
available = debug,fast,test,strict,safe_memory,safe_thread,coverage

[mode.debug]
flags = -g,-DDEBUG,-fno-omit-frame-pointer

[mode.fast]
flags = -pedantic-errors,-Werror,-Wall,-Wextra,-Wunused,-fno-elide-constructors,-Ofast,-fno-fast-math

[mode.strict]
flags = -pedantic-errors,-Werror,-Wall,-Wextra,-Wunused,-fno-elide-constructors

[mode.test]
flags = -DTEST

[mode.safe_memory]
flags = -pedantic-errors,-Werror,-Wall,-Wextra,-Wunused,-fno-elide-constructors,-fsanitize-address-use-after-scope,-fsanitize=undefined,-fstack-protector,-fsanitize=address,-fsanitize=leak

[mode.safe_thread]
flags = -pedantic-errors,-Werror,-Wall,-Wextra,-Wunused,-fno-elide-constructors,-fsanitize-address-use-after-scope,-fsanitize=undefined,-fstack-protector,-fsanitize=thread

[mode.coverage]
flags = -fprofile-arcs,-ftest-coverage

[coverage]
info_file = coverage.info
folder = .coverage
dark_mode = true
browser = brave-browser --ozone-platform-hint=x11
lcov_fixer = php autobuild/lcov-fixer.php

[cleanup]
exclude_dirs = .git,libs
extensions = o,so,dep,test,gdb,cov,gcda,gcno

[display]
show_commands = true

[build]
parallel_threads = 0
```

---

## Dependency Cache Invalidation Issue

### Problem
The builder's cache (`.dep` files) does not track changes to dependency plugin files (e.g., `NloptDependency.cpp`). When a dependency's `libs()`, `flags()`, or `incs()` methods are modified, the cache is not automatically invalidated, requiring manual cache cleanup.

### Example
When `NloptDependency.cpp` was modified to return library linking flags, the existing cache for `NloptOptimizer.cpp` was not invalidated, causing the old build to be reused.

### Root Cause
The cache only tracks:
- Source files (`.cpp`, `.c`)
- Include files (`.h`, `.hpp`)
- Implementation files
- Dependency names (e.g., `stevengj/nlopt:v2.10.1`)

It does NOT track:
- Dependency plugin files (e.g., `autobuild/dependencies/stevengj/nlopt/NloptDependency.cpp`)
- Output of dependency methods (`libs()`, `flags()`, `incs()`)

### Solutions

To fix this limitation, you could:

TODO: implement one or more of these:

1. **Track dependency plugin files** - Add dependency plugin files to the cache tracking mechanism
   - Modify `getIncludesAndImplementationsAndDependencies()` to include dependency plugin paths
   - Add their modification times to `local_max` for cache invalidation

2. **Hash the dependency outputs** - Include a hash of `libs()`, `flags()`, `incs()` outputs in the cache
   - Store a hash of dependency outputs in the `.dep` file
   - Compare current hash with cached hash to detect changes

3. **Add a manual `--clean-deps` flag** - Force rebuild when dependencies change
   - Add CLI argument to invalidate all dependency-related caches
   - Useful when dependency plugins are modified

4. **Touch dependency-using source files** - Use `touch` to update modification times of files using changed dependencies
   - When a dependency plugin changes, find all source files using it
   - Update their modification times to trigger cache invalidation
```
