#pragma once

#include "FILELN.hpp"
#include "F.hpp"

using namespace std;

#define ERROR(msg) runtime_error(F(F_ERROR, "\n\tError: ") + msg + FILELN) //"\nat " + F_FILE_LINE(fix_path(__FILE__), __LINE__))