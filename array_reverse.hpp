#pragma once

#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <type_traits>
#include "array_helpers.hpp"

using namespace std;

// ============================================================
// Internal helper: reverse sequential container (e.g., vector)
// ============================================================
template<typename TContainer>
auto array_reverse_sequential(const TContainer& input, bool /*preserve_keys*/)
{
    using ValueType = typename TContainer::value_type;
    vector<ValueType> result(input.size());

    // Copy in reverse order without relying on rbegin/rend (for unordered_map compatibility)
    ssize_t idx = static_cast<ssize_t>(input.size()) - 1;
    for (const auto& element : input) {
        if (idx >= 0) {
            result[static_cast<size_t>(idx)] = element;
            --idx;
        }
    }

    return result;
}

// ============================================================
// Internal helper: reverse string-keyed map (always preserve keys)
// ============================================================
template<typename TContainer>
auto array_reverse_string_keyed(const TContainer& input, bool /*preserve_keys*/)
{
    using KeyType = typename TContainer::key_type;
    using MappedType = typename TContainer::mapped_type;

   // Collect all pairs first (unordered_map doesn't have rbegin/rend)
    vector<pair<KeyType, MappedType>> all_pairs(input.size());
    size_t idx = 0;
    for (const auto& element : input) {
        all_pairs[idx++] = {element.first, element.second};
    }

    // Sort by key to ensure deterministic order for unordered containers
    sort(all_pairs.begin(), all_pairs.end(), [](const auto& a, const auto& b) { return a.first < b.first; });

    // Build result in reverse order - reversed pairs are inserted into sorted map
    map<KeyType, MappedType> result;
    if (idx > 0) {
        --idx; // Start from last element
        while (true) {
            result[all_pairs[idx].first] = all_pairs[idx].second;
            if (idx == 0) break;
            --idx;
        }
    }

    return result;
}

// ============================================================
// Internal helper: reverse integer-keyed map (preserve_keys matters)
// ============================================================
template<typename TContainer>
auto array_reverse_int_keyed(const TContainer& input, bool preserve_keys)
{
    using KeyType = typename TContainer::key_type;
    using MappedType = typename TContainer::mapped_type;

    // Collect all pairs first (unordered_map doesn't have rbegin/rend)
    vector<pair<KeyType, MappedType>> all_pairs(input.size());
    size_t count = 0;
    for (const auto& element : input) {
        all_pairs[count++] = {element.first, element.second};
    }

    // Sort by key to ensure deterministic order for unordered containers
    sort(all_pairs.begin(), all_pairs.end(), [](const auto& a, const auto& b) { return a.first < b.first; });

    if (!preserve_keys) {
        // Renumber keys sequentially from 0 - values in reverse order
        map<KeyType, MappedType> result;
        KeyType i = 0;
        for (size_t idx = count; idx > 0; --idx) {
            result[i] = all_pairs[idx - 1].second;
            ++i;
        }
        return result;
    }

    // Preserve original integer keys - just reverse order into new map
    map<KeyType, MappedType> result;
    for (size_t idx = count; idx > 0; --idx) {
        result[all_pairs[idx - 1].first] = all_pairs[idx - 1].second;
    }

    return result;
}

// ============================================================
// Internal helper: reverse other-keyed map (always preserve keys)
// ============================================================
template<typename TContainer>
auto array_reverse_other_keyed(const TContainer& input, bool /*preserve_keys*/)
{
    using KeyType = typename TContainer::key_type;
    using MappedType = typename TContainer::mapped_type;

    // Collect all pairs first (unordered_map doesn't have rbegin/rend)
    vector<pair<KeyType, MappedType>> all_pairs(input.size());
    size_t count = 0;
    for (const auto& element : input) {
        all_pairs[count++] = {element.first, element.second};
    }

    // Sort by key to ensure deterministic order for unordered containers
    sort(all_pairs.begin(), all_pairs.end(), [](const auto& a, const auto& b) { return a.first < b.first; });

    // Build result in reverse order - keys always preserved
    map<KeyType, MappedType> result;
    for (size_t idx = count; idx > 0; --idx) {
        result[all_pairs[idx - 1].first] = all_pairs[idx - 1].second;
    }

    return result;
}

/**
 * PHP array_reverse equivalent implementation in C++.
 * 
 * Takes an input array and returns a new array with the order of the elements reversed.
 * 
 * Parameters:
 *   array         - The input container (vector or map-like).
 *   preserve_keys  - If true, numeric keys are preserved. Non-numeric keys always preserved.
 * 
 * Return Values:
 *   Returns a new container with elements in reverse order.
 *   For vectors: returns vector<T>
 *   For maps: returns map<K, V>
 */
template<typename TContainer>
auto array_reverse(const TContainer& input, bool preserve_keys = false)
{
    using HasKey = typename has_key_type<TContainer>::type;
    
    if constexpr (!HasKey::value) {
        // Sequential container (vector, deque, etc.) - no keys to worry about
        return array_reverse_sequential(input, preserve_keys);
    } else {
        // Associative container (map-like) - dispatch based on key type at compile time
        using KeyType = typename TContainer::key_type;
        
        if constexpr (is_same<KeyType, string>::value ||
                      is_same<KeyType, const char*>::value) {
            // String keys are non-numeric, always preserved regardless of preserve_keys
            return array_reverse_string_keyed(input, preserve_keys);
        } else if (is_integral<KeyType>::value &&
                   !is_same<KeyType, bool>::value &&
                   !is_same<KeyType, char>::value) {
            // Integer keys - preserve_keys controls whether original keys are kept or renumbered from 0
            return array_reverse_int_keyed(input, preserve_keys);
        } else {
            // Other key types (e.g., long double, etc.) - always preserve keys
            return array_reverse_other_keyed(input, preserve_keys);
        }
    }
}
