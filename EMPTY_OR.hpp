#pragma once

#include "str_cut_end.hpp"
#include "F.hpp"

#define EMPTY_OR(s, ...) (F(F_DEBUG, ((s).empty() ? (std::string(__VA_ARGS__).empty() ? "<empty>" : std::string(__VA_ARGS__)) : (s).size() > 100 ? str_cut_end((s), 100) : (s))))
