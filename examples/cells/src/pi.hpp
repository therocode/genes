#pragma once 
#include <cmath>
#ifdef __EMSCRIPTEN__
float pi() { return std::atan(1)*4.0f; }
#else
constexpr float pi() { return std::atan(1)*4.0f; }
#endif
