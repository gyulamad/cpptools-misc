#pragma once

#include <vector>
#include "vector_concat.hpp"

using namespace std;

template<typename S, typename T>
void Serializable_vector_serialize(vector<S>& serialized, vector<T>& vec) {
    serialized.push_back(vec.size());
    for (T& v: vec)
        serialized = vector_concat(serialized, v.serialize());
}

