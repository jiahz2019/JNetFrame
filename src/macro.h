
// @brief 常用宏的封装

#ifndef __JHZ_MACRO_H__
#define __JHZ_MACRO_H__

#include <string.h>
#include <assert.h>
#include "log.h"
#include "util.h"

#if defined __GNUC__ || defined __llvm__
/// LIKCLY 宏的封装, 告诉编译器优化,条件大概率成立
#define JHZ_LIKELY(x) __builtin_expect(!!(x), 1)
/// LIKCLY 宏的封装, 告诉编译器优化,条件大概率不成立
#define JHZ_UNLIKELY(x) __builtin_expect(!!(x), 0)
#else
#define JHZ_LIKELY(x) (x)
#define JHZ_UNLIKELY(x) (x)
#endif

/// 断言宏封装
#define JHZ_ASSERT(x)                                                                \
    if (JHZ_UNLIKELY(!(x))) {                                                        \
        JHZ_LOG_ERROR(JHZ_LOG_ROOT()) << "ASSERTION: " #x                          \
                                          << "\nbacktrace:\n"                          \
                                          << jhz::BacktraceToString(100, 2, "    "); \
        assert(x);                                                                     \
    }

/// 断言宏封装
#define JHZ_ASSERT2(x, w)                                                            \
    if (JHZ_UNLIKELY(!(x))) {                                                        \
        JHZ_LOG_ERROR(JHZ_LOG_ROOT()) << "ASSERTION: " #x                          \
                                          << "\n"                                      \
                                          << w                                         \
                                          << "\nbacktrace:\n"                          \
                                          << jhz::BacktraceToString(100, 2, "    "); \
        assert(x);                                                                     \
    }

#endif