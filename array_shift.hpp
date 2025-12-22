#pragma once

#include "ERROR.hpp"
#include <vector>

using namespace std;

template <typename T>
T array_shift(vector<T>& vec) {
    if (vec.empty())
        throw ERROR("Cannot shift from an empty vector");

    // Save the first element (to return it)
    T firstElement = std::move(vec.front());

    // Remove the first element
    vec.erase(vec.begin());

    // Return the shifted element
    return firstElement;
}
    
