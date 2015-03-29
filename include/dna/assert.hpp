#pragma once
#include <iostream>
#include <csignal>

#ifndef SIGTRAP
#define SIGTRAP 5
#endif

#ifndef EMSCRIPTEN
#define DNA_HALT raise(SIGTRAP)
#else
#define DNA_HALT exit(1)
#endif

#ifdef _MSC_VER
#define __func__ __FUNCTION__
#endif

#ifndef NDEBUG
#   define DNA_ASSERT(condition, message) \
    do\
    { \
        if(!(condition))\
        { \
            std::cerr << "Assertion `" #condition "` failed in " << __FILE__ \
            << " function " << __func__ << " line " << __LINE__ << ": " << message << std::endl; \
            DNA_HALT; \
        } \
    } while (false)
#else
#   define DNA_ASSERT(condition, message) do { } while (false)
#endif
