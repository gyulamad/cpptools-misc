#pragma once

#include <filesystem>
#include <vector>
#include "ERROR.hpp"
#include "array_merge.hpp"

using namespace std;

namespace fs = filesystem;

// Helper function to check if a filename matches a pattern
bool readdir_matches_pattern(const string& filename, const string& pattern) {
    // Simple implementation for patterns starting with "*"
    if (pattern[0] == '*') {
        string suffix = pattern.substr(1);
        
        // Handle "*.*" - match any file with an extension
        if (suffix == ".*") {
            return filename.find('.') != string::npos;
        }
        
        // Handle "*" - match any file
        if (suffix.empty()) {
            return true;
        }
        
        // Handle patterns like "*.txt" - exact extension match
        if (suffix.find('*') == string::npos && suffix.find('?') == string::npos) {
            return filename.length() >= suffix.length() && 
                   filename.substr(filename.length() - suffix.length()) == suffix;
        }
        
        // For more complex patterns, we could implement a more sophisticated matching algorithm
        // For now, we'll use a simple approach for "*.tx*" patterns
        if (suffix.length() > 1 && suffix[1] == '*') {
            string prefix = suffix.substr(0, 1);
            return filename.find(prefix) != string::npos;
        }
    }
    
    // Default case: no match
    return false;
}

vector<string> readdir(const string& dir, const string& pattern = "", bool recursive = true, bool ignore_folders = true) {
    vector<string> result;

    if (!fs::exists(dir) || !fs::is_directory(dir)) {
        throw ERROR("Directory does not exist or is inaccessible: " + dir);
    }

    // Local lambda to process directory entries
    auto process = [&](const auto& begin, const auto& end) {
        for (auto it = begin; it != end; ++it) {
            if (!ignore_folders || it->is_regular_file()) {
                string path = it->path().string();
                if (pattern.empty()) {
                    result.push_back(path);
                }
                else if (readdir_matches_pattern(it->path().filename().string(), pattern)) {
                    result.push_back(path);
                }
                else if (pattern[0] == '*' && it->path().extension() == pattern.substr(1)) {
                    result.push_back(path);
                }
            }
        }
    };

    try {
        if (recursive) {
            fs::recursive_directory_iterator iterator(dir, fs::directory_options::skip_permission_denied);
            process(iterator, fs::recursive_directory_iterator());
        } else {
            fs::directory_iterator iterator(dir, fs::directory_options::skip_permission_denied);
            process(iterator, fs::directory_iterator());
        }
    } catch (const fs::filesystem_error& e) {
        throw ERROR("Error reading directory: " + string(e.what()));
    }

    return result;
}

vector<string> readdir(const string& dir, const vector<string>& patterns, bool recursive = true, bool ignore_folders = true) {
    vector<string> result;
    for (const string& pattern: patterns)
        result = array_merge(result, readdir(dir, pattern, recursive, ignore_folders));
    return result;
}
