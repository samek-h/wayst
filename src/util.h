/* See LICENSE for license information. */

#pragma once

#define _GNU_SOURCE

#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define TERMCOLOR_RESET "\e[m"

#define TERMCOLOR_DEFAULT       "\e[39m"
#define TERMCOLOR_BLACK         "\e[30m"
#define TERMCOLOR_RED           "\e[31m"
#define TERMCOLOR_GREEN         "\e[32m"
#define TERMCOLOR_YELLOW        "\e[33m"
#define TERMCOLOR_BLUE          "\e[34m"
#define TERMCOLOR_MAGENTA       "\e[35m"
#define TERMCOLOR_CYAN          "\e[36m"
#define TERMCOLOR_GRAY          "\e[37m"
#define TERMCOLOR_GRAY_DARK     "\e[90m"
#define TERMCOLOR_RED_LIGHT     "\e[91m"
#define TERMCOLOR_GREEN_LIGHT   "\e[92m"
#define TERMCOLOR_YELLOW_LIGHT  "\e[93m"
#define TERMCOLOR_BLUE_LIGHT    "\e[94m"
#define TERMCOLOR_MAGENTA_LIGHT "\e[95m"
#define TERMCOLOR_CYAN_LIGHT    "\e[96m"
#define TERMCOLOR_WHITE         "\e[97m"

#define TERMCOLOR_BG_DEFAULT       "\e[49m"
#define TERMCOLOR_BG_BLACK         "\e[40m"
#define TERMCOLOR_BG_RED           "\e[41m"
#define TERMCOLOR_BG_GREEN         "\e[42m"
#define TERMCOLOR_BG_YELLOW        "\e[43m"
#define TERMCOLOR_BG_BLUE          "\e[44m"
#define TERMCOLOR_BG_MAGENTA       "\e[45m"
#define TERMCOLOR_BG_CYAN          "\e[46m"
#define TERMCOLOR_BG_GRAY          "\e[47m"
#define TERMCOLOR_BG_GRAY_DARK     "\e[100m"
#define TERMCOLOR_BG_RED_LIGHT     "\e[101m"
#define TERMCOLOR_BG_GREEN_LIGHT   "\e[102m"
#define TERMCOLOR_BG_YELLOW_LIGHT  "\e[103m"
#define TERMCOLOR_BG_BLUE_LIGHT    "\e[104m"
#define TERMCOLOR_BG_MAGENTA_LIGHT "\e[105m"
#define TERMCOLOR_BG_CYAN_LIGHT    "\e[106m"
#define TERMCOLOR_BG_WHITE         "\e[107m"

#define likely(_x)     __builtin_expect((bool)(_x), 1)
#define unlikely(_x)   __builtin_expect((bool)(_x), 0)
#define expect(_x, _y) __builtin_expect((_x), (_y))

#define POW2(_x)            ((_x) * (_x))
#define POW3(_x)            ((_x) * (_x) * (_X))
#define ABS(_x)             ((_x) < 0 ? -(_x) : (_x))
#define MAX(_a, _b)         ((_a) > (_b) ? (_a) : (_b))
#define MIN(_a, _b)         ((_a) < (_b) ? (_a) : (_b))
#define CLAMP(_v, _lo, _hi) ((_v) > (_hi) ? (_hi) : (_v) < (_lo) ? (_lo) : (_v))

#define ARRAY_SIZE(_array) (sizeof((_array)) / sizeof((_array[0])))
#define ARRAY_LAST(_array) (_array[(sizeof((_array)) / sizeof((_array[0]))) - 1])

#define STATIC_ASSERT(cond, msg) typedef char static_assertion_##msg[(cond) ? 1 : -1]

#define WRN(...)                                                                                   \
    {                                                                                              \
        fputs("[\e[33mwarning\e[m] ", stderr);                                                     \
        fprintf(stderr, __VA_ARGS__);                                                              \
    }

#ifdef DEBUG

#define ERR(...)                                                                                   \
    {                                                                                              \
        fputs("[\e[31merror\e[m] ", stderr);                                                       \
        fprintf(stderr, __VA_ARGS__);                                                              \
        fprintf(stderr, "\nIn file: \"%s\" function: \"%s\" line: %d\n", __FILE__, __func__,       \
                __LINE__);                                                                         \
        exit(EXIT_FAILURE);                                                                        \
    }
#define ASSERT(cond, msg)                                                                          \
    {                                                                                              \
        if ((cond) == false) {                                                                     \
            ERR("Assertion failed: \'%s\', %s", #cond, msg);                                       \
        }                                                                                          \
    }
#define ASSERT_UNREACHABLE                                                                         \
    {                                                                                              \
        ERR("got to section declared unreachable. file: %s func: %s line: %d", __FILE__, __func__, \
            __LINE__);                                                                             \
        __builtin_unreachable();                                                                   \
    }

#define LOG(...) fprintf(stderr, __VA_ARGS__)

static inline void* _call_fp_helper(const char* const msg,
                                    const char* const fname,
                                    const char* const func,
                                    const int         line)
{
    fprintf(stderr, "\e[31m%s In File: \"%s\" function: \"%s\" line: %d \e[m\n", msg, fname, func,
            line);
    exit(EXIT_FAILURE);
}

// call function pointer matching T(*)(void*, ...), error if NULL
#define CALL_FP(_func, _void_ptr, ...)                                                             \
    (_func)((_func)                                                                                \
              ? (_void_ptr)                                                                        \
              : _call_fp_helper("function \'" #_func "\' is NULL.", __FILE__, __func__, __LINE__), \
            ##__VA_ARGS__)

#else

#define ASSERT(...)                                                                                \
    {                                                                                              \
        ;                                                                                          \
    }
#define ASSERT_UNREACHABLE                                                                         \
    {                                                                                              \
        __builtin_unreachable();                                                                   \
    }
#define LOG(...)                                                                                   \
    {                                                                                              \
        ;                                                                                          \
    }
#define ERR(...)                                                                                   \
    {                                                                                              \
        fputs("[\e[31merror\e[m] ", stderr);                                                       \
        fprintf(stderr, __VA_ARGS__);                                                              \
        fputs("\n", stderr);                                                                       \
        exit(EXIT_FAILURE);                                                                        \
    }

#define CALL_FP(_func, _void_ptr, ...) ((_func)((_void_ptr), ##__VA_ARGS__))

#endif

#ifndef asprintf
#define asprintf(...) _asprintf(__VA_ARGS__)
#endif

static inline char* _asprintf(const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    char* buf = malloc(1 + vsnprintf(NULL, 0, fmt, ap));
    va_end(ap);
    va_start(ap, fmt);
    vsprintf(buf, fmt, ap);
    va_end(ap);

    return buf;
}

#define FLAG_SET(fld, flg)    (fld |= (flg))
#define FLAG_UNSET(fld, flg)  (fld &= ~(flg))
#define FLAG_FLIP(fld, flg)   (fld ^= (flg))
#define FLAG_IS_SET(fld, flg) (fld & (flg))
#define BIT_SET(fld, bit)     (fld |= (1 << bit))
#define BIT_UNSET(fld, bit)   (fld &= ~(1 << bit))
#define BIT_FLIP(fld, bit)    (fld ^= (1 << bit))
#define BIT_IS_SET(fld, bit)  (fld & (1 << bit))

#define BIN_8_FMT "%c%c%c%c%c%c%c%c"
#define BIN_8_AP(byte)                                                                             \
    (byte & 0b10000000 ? '1' : '0'), (byte & 0b01000000 ? '1' : '0'),                              \
      (byte & 0b00100000 ? '1' : '0'), (byte & 0b00010000 ? '1' : '0'),                            \
      (byte & 0b00001000 ? '1' : '0'), (byte & 0b00000100 ? '1' : '0'),                            \
      (byte & 0b00000010 ? '1' : '0'), (byte & 0b00000001 ? '1' : '0')

#define BIN_16_FMT BIN_8_FMT " " BIN_8_FMT
#define BIN_16_AP(byte)                                                                            \
    (byte & 0b1000000000000000 ? '1' : '0'), (byte & 0b0100000000000000 ? '1' : '0'),              \
      (byte & 0b0010000000000000 ? '1' : '0'), (byte & 0b0001000000000000 ? '1' : '0'),            \
      (byte & 0b0000100000000000 ? '1' : '0'), (byte & 0b0000010000000000 ? '1' : '0'),            \
      (byte & 0b0000001000000000 ? '1' : '0'), (byte & 0b0000000100000000 ? '1' : '0'),            \
      (byte & 0b0000000010000000 ? '1' : '0'), (byte & 0b0000000001000000 ? '1' : '0'),            \
      (byte & 0b0000000000100000 ? '1' : '0'), (byte & 0b0000000000010000 ? '1' : '0'),            \
      (byte & 0b0000000000001000 ? '1' : '0'), (byte & 0b0000000000000100 ? '1' : '0'),            \
      (byte & 0b0000000000000010 ? '1' : '0'), (byte & 0b0000000000000001 ? '1' : '0')

#define BIN_32_FMT BIN_16_FMT " " BIN_16_FMT
#define BIN_32_AP(byte)                                                                            \
    (byte & 0b10000000000000000000000000000000 ? '1' : '0'),                                       \
      (byte & 0b01000000000000000000000000000000 ? '1' : '0'),                                     \
      (byte & 0b00100000000000000000000000000000 ? '1' : '0'),                                     \
      (byte & 0b00010000000000000000000000000000 ? '1' : '0'),                                     \
      (byte & 0b00001000000000000000000000000000 ? '1' : '0'),                                     \
      (byte & 0b00000100000000000000000000000000 ? '1' : '0'),                                     \
      (byte & 0b00000010000000000000000000000000 ? '1' : '0'),                                     \
      (byte & 0b00000001000000000000000000000000 ? '1' : '0'),                                     \
      (byte & 0b00000000100000000000000000000000 ? '1' : '0'),                                     \
      (byte & 0b00000000010000000000000000000000 ? '1' : '0'),                                     \
      (byte & 0b00000000001000000000000000000000 ? '1' : '0'),                                     \
      (byte & 0b00000000000100000000000000000000 ? '1' : '0'),                                     \
      (byte & 0b00000000000010000000000000000000 ? '1' : '0'),                                     \
      (byte & 0b00000000000001000000000000000000 ? '1' : '0'),                                     \
      (byte & 0b00000000000000100000000000000000 ? '1' : '0'),                                     \
      (byte & 0b00000000000000010000000000000000 ? '1' : '0'),                                     \
      (byte & 0b00000000000000001000000000000000 ? '1' : '0'),                                     \
      (byte & 0b00000000000000000100000000000000 ? '1' : '0'),                                     \
      (byte & 0b00000000000000000010000000000000 ? '1' : '0'),                                     \
      (byte & 0b00000000000000000001000000000000 ? '1' : '0'),                                     \
      (byte & 0b00000000000000000000100000000000 ? '1' : '0'),                                     \
      (byte & 0b00000000000000000000010000000000 ? '1' : '0'),                                     \
      (byte & 0b00000000000000000000001000000000 ? '1' : '0'),                                     \
      (byte & 0b00000000000000000000000100000000 ? '1' : '0'),                                     \
      (byte & 0b00000000000000000000000010000000 ? '1' : '0'),                                     \
      (byte & 0b00000000000000000000000001000000 ? '1' : '0'),                                     \
      (byte & 0b00000000000000000000000000100000 ? '1' : '0'),                                     \
      (byte & 0b00000000000000000000000000010000 ? '1' : '0'),                                     \
      (byte & 0b00000000000000000000000000001000 ? '1' : '0'),                                     \
      (byte & 0b00000000000000000000000000000100 ? '1' : '0'),                                     \
      (byte & 0b00000000000000000000000000000010 ? '1' : '0'),                                     \
      (byte & 0b00000000000000000000000000000001 ? '1' : '0')

/* Pair struct */
#define DEF_PAIR(a)                                                                                \
    typedef struct                                                                                 \
    {                                                                                              \
        a first;                                                                                   \
        a second;                                                                                  \
    } Pair_##a

DEF_PAIR(uint8_t);
DEF_PAIR(uint16_t);
DEF_PAIR(uint32_t);
DEF_PAIR(uint64_t);
DEF_PAIR(int8_t);
DEF_PAIR(int16_t);
DEF_PAIR(int32_t);
DEF_PAIR(int64_t);
DEF_PAIR(char);
DEF_PAIR(int);
DEF_PAIR(unsigned);
DEF_PAIR(short);
DEF_PAIR(long);
DEF_PAIR(float);
DEF_PAIR(double);
DEF_PAIR(wchar_t);
DEF_PAIR(size_t);

/** check string equality case insensitive */
static inline bool strneqci(const char* restrict s1, const char* restrict s2, const size_t n)
{
    for (size_t i = 0; i < n; ++i)
        if (tolower(s1[i]) != tolower(s2[i]))
            return false;
    return true;
}

/** match string against wildcard pattern */
static bool streq_wildcard(const char* restrict str, const char* restrict pattern)
{
    while (*pattern && *str)
        switch (*pattern) {
            default:
                if (*pattern != *str)
                    return false;
                /* fall through */
            case '?':
                ++pattern;
                ++str;
                break;
            case '*':
                if (pattern[1] == '\0' || pattern[1] == '*')
                    return true;
                ++pattern;
                while (*pattern && *str)
                    if (streq_wildcard(str++, pattern))
                        return true;
                return false;
        }

    size_t num_stars = 0;
    for (const char* i = pattern; *i; ++i)
        if (*i == '*')
            ++num_stars;
    return strlen(str) == (strlen(pattern) - num_stars);
}

/** convert string to bool (false if fails) */
static inline bool strtob(const char* restrict str)
{
    if (!str) {
        return false;
    }
    return strneqci("true", str, 4) || strneqci("1", str, 1);
}
