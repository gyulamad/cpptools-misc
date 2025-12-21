#pragma once

#include <type_traits>
#include <utility>

using namespace std;

template <typename T>
struct is_pair : false_type {};

template <typename T1, typename T2>
struct is_pair<pair<T1, T2>> : true_type {};

template <typename Needle, typename Container>
bool in_array(const Needle& needle, const Container& container) {
    using Element = typename Container::value_type;

    if constexpr (is_pair<Element>::value) {
        // Check values in associative containers (maps, unordered_maps)
        for (const auto& element : container)
            if (element.second == needle) return true;
    } else {
        // Check elements in sequence containers (vectors, lists, etc.)
        for (const auto& element : container)
            if (element == needle) return true;
    }

    return false;
}

