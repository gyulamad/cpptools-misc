#pragma once

#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include <optional>
#include <typeinfo>
#include "array_helpers.hpp"

using namespace std;

// Trait to determine the return type based on the container category
template <typename Container, typename = void>
struct ArraySearchResult {
    // Default: vector-like, returns ssize_t (-1 for not found)
    using type = ssize_t;
};

template <typename Container>
struct ArraySearchResult<Container, void_t<typename Container::key_type>> {
    // Map-like, returns optional<key_type> (nullopt for not found)
    using type = optional<typename Container::key_type>;
};

/**
 * PHP array_search equivalent implementation in C++.
 * 
 * Searches the haystack for needle. Returns the key if it is found in the array,
 * false (nullopt or -1) otherwise.
 * 
 * @tparam TNeedle The type of the value to search for.
 * @tparam TContainer The type of the container.
 * @param needle The value to search for.
 * @param haystack The container to search in.
 * @return For maps: optional<key_type>. For vectors: ssize_t (-1 if not found).
 */
template <typename TNeedle, typename TContainer>
auto array_search(TNeedle needle, const TContainer& haystack) -> typename ArraySearchResult<TContainer>::type {
    
    if constexpr (has_key_type<TContainer>::value) {
        // Associative container (map-like) iteration
        for (const auto& element : haystack)
            if (element.second == needle) return optional<typename TContainer::key_type>(element.first);
        
        return nullopt;
    } else {
        // Sequential container (vector-like) iteration
        for (size_t i = 0; i < haystack.size(); ++i)
            if (haystack[i] == needle) return (ssize_t)i;

        return -1;
    }
}