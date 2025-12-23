#pragma once

#include <vector>

using namespace std;

template <typename T>
void sort(vector<T>& vec) {
    sort(vec.begin(), vec.end()); // Sorts in ascending order
}

template <typename T>
vector<T> sort(const vector<T>& vec) {
    vector<T> ret = vec;
    sort(ret); // Sorts in ascending order
    return ret;
}
