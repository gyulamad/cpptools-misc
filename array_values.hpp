#pragma once

#include <vector>

using namespace std;

template<typename T>
auto array_values(const T& input_map) -> vector<typename T::mapped_type> {
    vector<typename T::mapped_type> result;
    result.reserve(input_map.size());
    
    for (const auto& pair : input_map) {
        result.push_back(pair.second);
    }
    
    return result;
}
