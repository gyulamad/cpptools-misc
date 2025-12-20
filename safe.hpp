#pragma once

#include "EMPTY_OR.hpp"
#include "NULLCHK.hpp"

using namespace std;

template<typename T>
T* safe(T* ptr, const string& errmsg) {
    NULLCHK(ptr, "Null pointer reference: " + EMPTY_OR(errmsg));
    return ptr;
}

#define SAFE(ptr) safe(ptr, #ptr + FILELN)
