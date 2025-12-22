#pragma once

#include <unordered_map>
#include <functional>
#include <map>
#include "datetime_defs.hpp"

using namespace std;

// ===============================================================
// =========================== foreach ===========================
// ===============================================================

// TODO: foreach needs tests!!

#define FE_BREAK false
#define FE_CONTINUE true

// Trait to determine key type and iteration behavior
template<typename T, typename = void>
struct ContainerTraits {
    using KeyType = size_t;
    using ItemType = typename T::value_type&;

    static void iterate(T& data, function<void(ItemType)> callback) {
        for (auto& item : data) {
            callback(item);
        }
    }

    static void iterate(T& data, function<void(ItemType, KeyType)> callback) {
        size_t index = 0;
        for (auto& item : data) {
            callback(item, index++);
        }
    }

    static void iterate(T& data, function<bool(ItemType)> callback) {
        for (auto& item : data) {
            if (!callback(item)) {
                break;
            }
        }
    }

    static void iterate(T& data, function<bool(ItemType, KeyType)> callback) {
        size_t index = 0;
        for (auto& item : data) {
            if (!callback(item, index++)) {
                break;
            }
        }
    }
};

// Specialization for unordered_map
template<typename K, typename V>
struct ContainerTraits<unordered_map<K, V>> {
    using KeyType = K;
    using ItemType = V&;

    static void iterate(unordered_map<K, V>& data, function<void(ItemType)> callback) {
        for (auto& pair : data) {
            callback(pair.second);
        }
    }

    static void iterate(unordered_map<K, V>& data, function<void(ItemType, KeyType)> callback) {
        for (auto& pair : data) {
            callback(pair.second, pair.first);
        }
    }

    static void iterate(unordered_map<K, V>& data, function<bool(ItemType)> callback) {
        for (auto& pair : data) {
            if (!callback(pair.second)) {
                break;
            }
        }
    }

    static void iterate(unordered_map<K, V>& data, function<bool(ItemType, KeyType)> callback) {
        for (auto& pair : data) {
            if (!callback(pair.second, pair.first)) {
                break;
            }
        }
    }
};

// map ---


// Specialization for vector
template<typename T>
struct ContainerTraits<vector<T>> {
    using ItemType = T&;
    using KeyType = size_t;  // Add KeyType for consistency

    static void iterate(vector<T>& data, function<void(ItemType)> callback) {
        for (auto& item : data) {
            callback(item);
        }
    }

    // Add this new overload for iteration with index
    static void iterate(vector<T>& data, function<void(ItemType, KeyType)> callback) {
        size_t index = 0;
        for (auto& item : data) {
            callback(item, index++);
        }
    }

    static void iterate(vector<T>& data, function<bool(ItemType)> callback) {
        for (auto& item : data) {
            if (!callback(item)) {
                break;
            }
        }
    }

    // Optionally, add this for completeness (if you want breaking with index)
    static void iterate(vector<T>& data, function<bool(ItemType, KeyType)> callback) {
        size_t index = 0;
        for (auto& item : data) {
            if (!callback(item, index++)) {
                break;
            }
        }
    }
};

// Specialization for const vector
template<typename T>
struct ContainerTraits<const vector<T>> {
    using ItemType = const T&;

    static void iterate(const vector<T>& data, function<void(ItemType)> callback) {
        for (const auto& item : data) {
            callback(item);
        }
    }

    static void iterate(const vector<T>& data, function<bool(ItemType)> callback) {
        for (const auto& item : data) {
            if (!callback(item)) {
                break;
            }
        }
    }
};

// Specialization for map
template<typename K, typename V>
struct ContainerTraits<map<K, V>> {
    using KeyType = K;
    using ItemType = V&;

    static void iterate(map<K, V>& data, function<void(ItemType)> callback) {
        for (auto& pair : data) {
            callback(pair.second);
        }
    }

    static void iterate(map<K, V>& data, function<void(ItemType, KeyType)> callback) {
        for (auto& pair : data) {
            callback(pair.second, pair.first);
        }
    }

    static void iterate(map<K, V>& data, function<bool(ItemType)> callback) {
        for (auto& pair : data) {
            if (!callback(pair.second)) {
                break;
            }
        }
    }

    static void iterate(map<K, V>& data, function<bool(ItemType, KeyType)> callback) {
        for (auto& pair : data) {
            if (!callback(pair.second, pair.first)) {
                break;
            }
        }
    }
};// Specialization for const map
template<typename K, typename V>
struct ContainerTraits<const map<K, V>> {
    using KeyType = const K;
    using ItemType = const V&;

    static void iterate(const map<K, V>& data, function<void(ItemType)> callback) {
        for (const auto& pair : data) {
            callback(pair.second);
        }
    }

    static void iterate(const map<K, V>& data, function<void(ItemType, KeyType)> callback) {
        for (const auto& pair : data) {
            callback(pair.second, pair.first);
        }
    }

    static void iterate(const map<K, V>& data, function<bool(ItemType)> callback) {
        for (const auto& pair : data) {
            if (!callback(pair.second)) {
                break;
            }
        }
    }

    static void iterate(const map<K, V>& data, function<bool(ItemType, KeyType)> callback) {
        for (const auto& pair : data) {
            if (!callback(pair.second, pair.first)) {
                break;
            }
        }
    }
};

// Specialization for const unordered_map
template<typename K, typename V>
struct ContainerTraits<const std::unordered_map<K, V>> {
    using KeyType = const K;
    using ItemType = const V&;

    static void iterate(const std::unordered_map<K, V>& data, std::function<void(ItemType)> callback) {
        for (const auto& pair : data) {
            callback(pair.second);
        }
    }

    static void iterate(const std::unordered_map<K, V>& data, std::function<void(ItemType, KeyType)> callback) {
        for (const auto& pair : data) {
            callback(pair.second, pair.first);
        }
    }

    static void iterate(const std::unordered_map<K, V>& data, std::function<bool(ItemType)> callback) {
        for (const auto& pair : data) {
            if (!callback(pair.second)) {
                break;
            }
        }
    }

    static void iterate(const std::unordered_map<K, V>& data, std::function<bool(ItemType, KeyType)> callback) {
        for (const auto& pair : data) {
            if (!callback(pair.second, pair.first)) {
                break;
            }
        }
    }
};

// --------

// Helper to detect if a functor returns bool
template<typename F, typename... Args>
using returns_bool = is_same<invoke_result_t<F, Args...>, bool>;

// Foreach overloads with SFINAE
template<typename T, typename F, enable_if_t<!returns_bool<F, typename ContainerTraits<T>::ItemType>::value, int> = 0>
void foreach(T& data, F callback) {
    ContainerTraits<T>::iterate(data, function<void(typename ContainerTraits<T>::ItemType)>(callback));
}

template<typename T, typename F, enable_if_t<!returns_bool<F, typename ContainerTraits<T>::ItemType, typename ContainerTraits<T>::KeyType>::value, int> = 0>
void foreach(T& data, F callback) {
    ContainerTraits<T>::iterate(data, function<void(typename ContainerTraits<T>::ItemType, typename ContainerTraits<T>::KeyType)>(callback));
}

template<typename T, typename F, enable_if_t<returns_bool<F, typename ContainerTraits<T>::ItemType>::value, int> = 0>
void foreach(T& data, F callback) {
    ContainerTraits<T>::iterate(data, function<bool(typename ContainerTraits<T>::ItemType)>(callback));
}

template<typename T, typename F, enable_if_t<returns_bool<F, typename ContainerTraits<T>::ItemType, typename ContainerTraits<T>::KeyType>::value, int> = 0>
void foreach(T& data, F callback) {
    ContainerTraits<T>::iterate(data, function<bool(typename ContainerTraits<T>::ItemType, typename ContainerTraits<T>::KeyType)>(callback));
}


// --- map

// Helper to detect if a functor returns bool
template<typename F, typename... Args>
using returns_bool = is_same<invoke_result_t<F, Args...>, bool>;

// ===============================================================
// ===============================================================
// ===============================================================

