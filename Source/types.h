#pragma once

#include <stddef.h>

typedef char    int8;
typedef short   int16;
typedef int     int32;
typedef __int64 int64;

typedef unsigned char       uchar;
typedef unsigned short      ushort;
typedef unsigned int        uint;
typedef unsigned long       ulong;
typedef unsigned __int64    uint64;

typedef uchar   uint8;
typedef ushort  uint16;
typedef uint    uint32;

#ifdef _M_X64
    typedef double float_size_t;
#else
    typedef float float_size_t;
#endif

#define _CACHE_LINE  64
#ifndef _NO_CACHE_ALIGN
#   define _CACHE_ALIGN __declspec(align(_CACHE_LINE))
#else
#   define _CACHE_ALIGN
#endif

#define _THREAD_LOCAL __declspec(thread)