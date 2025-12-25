#pragma once

#include <type_traits>

using namespace std;

template <typename T, typename = void>
struct has_key_type : false_type {};

template <typename T>
struct has_key_type<T, void_t<typename T::key_type>> : true_type {};

template <typename Container, typename = void>
struct KeyTypeTrait {
    using type = size_t;
};

template <typename Container>
struct KeyTypeTrait<Container, void_t<typename Container::key_type>> {
    using type = typename Container::key_type;
};