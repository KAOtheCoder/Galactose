#pragma once

#include <iostream>

#define GT_STRINGIFY(x) #x
#define GT_STRINGIFY_VALUE(x) GT_STRINGIFY(x)

#if defined(GT_WINDOWS)
#define GT_DEBUGBREAK() __debugbreak()
#elif defined(GT_LINUX)
#define GT_DEBUGBREAK() raise(SIGTRAP)
#endif

#ifndef GT_ASSERT
	#if defined(GT_DEBUG)
		#define GT_ASSERT(check, message) { if (!(check)) { std::cerr << (message) << std::endl; GT_DEBUGBREAK(); } }
	#elif defined(GT_RELEASE)
		#define GT_ASSERT(check, message)
	#endif
#endif
