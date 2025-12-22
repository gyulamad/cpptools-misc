#pragma once

#include <vector>

using namespace std;

template <typename T>
void rsort(vector<T>& vec) {
    sort(vec.begin(), vec.end(), greater<T>()); // Sorts in descending order
}
