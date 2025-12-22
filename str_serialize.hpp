#pragma once

#include <vector>
#include <string>

using namespace std;

template<typename S>
vector<S> str_serialize(const string& str) {
    vector<S> serialized;
    size_t size = str.size();
    // Serialize size as 4 bytes (big-endian)
    serialized.push_back(static_cast<S>((size >> 24) & 0xFF));
    serialized.push_back(static_cast<S>((size >> 16) & 0xFF));
    serialized.push_back(static_cast<S>((size >> 8) & 0xFF));
    serialized.push_back(static_cast<S>(size & 0xFF));
    for (char c : str)
        serialized.push_back(static_cast<S>(c));
    return serialized;
}
