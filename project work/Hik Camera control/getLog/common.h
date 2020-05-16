/*
 * common.h - Common things.
 *
 * Authors: Jia Wang
 *
 */

#ifndef COMMON_H
#define COMMON_H

#ifndef __cplusplus
#error C++11 support is required.
#endif // __cplusplus

#ifdef _MSC_VER

#if _MSC_VER < 1900
#error MSVC19/VS2015 or newer is required.
#endif // _MSC_VER < 1900

// no deprecated function
#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE
#endif // _CRT_SECURE_NO_DEPRECATE
#ifndef _SCL_SECURE_NO_DEPRECATE
#define _SCL_SECURE_NO_DEPRECATE
#endif // _SCL_SECURE_NO_DEPRECATE

// turn off range validation in vector's []
#ifdef NDEBUG
#ifndef _SECURE_SCL
#define _SECURE_SCL 0
#endif // _SECURE_SCL
#endif // NDEBUG

#endif // _MSC_VER

#ifdef __GNUC__
#if __GNUC__*10000+__GNUC_MINOR__*100 < 40900
#error GCC 4.9 or newer is required.
#endif
#endif // __GNUC__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <time.h>
#include <assert.h>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>

#ifdef __GNUC__
#define _vsnprintf vsnprintf
#define _strnicmp strncasecmp
#define _stricmp strcasecmp
#endif // __GNUC__

#ifndef _WIN32
#define _popen popen
#define _pclose pclose
#endif // _WIN32

#include <list>
#include <set>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <vector>
#include <stdexcept>
#include <functional>
#include <algorithm>
#include <utility>
#include <numeric>
#include <memory>
#include <queue>
#include <complex>
#include <type_traits>
#include <atomic>
#include <thread>
#include <chrono>
#include <array>
#include <mutex>
#include <thread>

#include "misc_str.h"

#define STRINGIFY_(x) #x
#define STRINGIFY(x) STRINGIFY_(x)

#endif // COMMON_H
