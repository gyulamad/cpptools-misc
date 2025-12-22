#pragma once

#include <vector>

using namespace std;

template<typename T>
void vector_remove(vector<T>& vec, const T& value) {
    vec.erase(remove(vec.begin(), vec.end(), value), vec.end());
}
