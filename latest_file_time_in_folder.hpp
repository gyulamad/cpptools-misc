#pragma once

#include <chrono>
#include "ERROR.hpp"
#include <filesystem>
#include "datetime_defs.hpp"

using namespace std;

namespace fs = filesystem; // Alias for convenience

chrono::system_clock::duration latest_file_time_in_folder(const string& folder_path) {
    if (folder_path.empty()) 
        throw ERROR("Folder path cannot be empty");
    
    fs::path dir_path(folder_path);
    
    if (!fs::exists(dir_path))
        throw ERROR("Path does not exist: " + folder_path);
    
    if (!fs::is_directory(dir_path))
        throw ERROR("Path is not a directory: " + folder_path);
    
    chrono::system_clock::duration latest_time{};
    bool found_any_file = false;
    
    // Recursively iterate through all files
    for (const auto& entry : fs::recursive_directory_iterator(dir_path)) {
        if (fs::is_regular_file(entry)) {
            auto file_time = fs::last_write_time(entry);
            auto system_time = chrono::clock_cast<chrono::system_clock>(file_time);
            auto duration = system_time.time_since_epoch();
            
            if (!found_any_file || duration > latest_time) {
                latest_time = duration;
                found_any_file = true;
            }
        }
    }
    
    if (!found_any_file)
        throw ERROR("No files found in directory: " + folder_path);
    
    return latest_time;
}


time_ms latest_file_time_in_folder_ms(const string& folder) {
    return chrono::duration_cast<chrono::milliseconds>(latest_file_time_in_folder(folder)).count();
}

time_ms latest_file_time_in_folder_sec(const string& folder) {
    return chrono::duration_cast<chrono::seconds>(latest_file_time_in_folder(folder)).count();
}