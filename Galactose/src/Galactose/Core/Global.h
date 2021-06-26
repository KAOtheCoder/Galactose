#pragma once

#include <iostream>

#define GT_STRINGIFY(x) #x
#define GT_STRINGIFY_VALUE(x) GT_STRINGIFY(x)

#ifndef GT_ASSERT
#define GT_ASSERT(check, message) { if (!(check)) { std::cerr << (message) << std::endl; std::exit(EXIT_FAILURE); } }
#endif