#pragma once

#include <iostream>

#ifdef GT_STATIC_BUILD
#define GT_API
#elif GT_DYNAMIC_BUILD
#define GT_API __declspec(dllexport)
#else
#define GT_API __declspec(dllimport)
#endif

#define GT_STRINGIFY(x) #x
#define GT_STRINGIFY_VALUE(x) GT_STRINGIFY(x)
#define GT_UNMOVABLE(T) T(const T&&) = delete; T& operator=(T&&) = delete;
#define GT_UNCOPYABLE(T) T(const T&) = delete; T& operator=(T&) = delete;

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
