#pragma once

#include "fix_path.hpp"

#define FILELN (std::string("\n\tat ") + F_FILE_LINE(fix_path(__FILE__), __LINE__))
