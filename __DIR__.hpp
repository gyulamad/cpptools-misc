#pragma once

#include "get_path.hpp"

// WARN: !!DEPRECATED!! DO NOT USE __DIR__, use get_cwd() instread!!!
#define __DIR__ get_path(__FILE__)