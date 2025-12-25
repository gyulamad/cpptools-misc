#pragma once

#include <vector>
#include "array_helpers.hpp"

using namespace std;

template <typename Container>
auto array_keys(const Container& container) {
    using KeyType = typename KeyTypeTrait<Container>::type;
    vector<KeyType> keys;

    if constexpr (has_key_type<Container>::value) {
        for (const auto& element : container)
            keys.push_back(element.first);
    } else {
        for (size_t i = 0; i < container.size(); ++i)
            keys.push_back(i);
    }

    return keys;
}
