#ifndef __khrplatform_h_
#define __khrplatform_h_

/*
** Copyright (c) 2008-2018 The Khronos Group Inc.
*/

#include <stdint.h>

#ifndef KHRONOS_SUPPORT_INT64
#define KHRONOS_SUPPORT_INT64 1
#endif

#ifndef KHRONOS_SUPPORT_FLOAT
#define KHRONOS_SUPPORT_FLOAT 1
#endif

typedef int8_t khronos_int8_t;
typedef uint8_t khronos_uint8_t;
typedef int16_t khronos_int16_t;
typedef uint16_t khronos_uint16_t;
typedef int32_t khronos_int32_t;
typedef uint32_t khronos_uint32_t;
typedef int64_t khronos_int64_t;
typedef uint64_t khronos_uint64_t;
typedef signed long int khronos_intptr_t;
typedef unsigned long int khronos_uintptr_t;
typedef signed long int khronos_ssize_t;
typedef unsigned long int khronos_usize_t;

#if KHRONOS_SUPPORT_FLOAT
typedef float khronos_float_t;
#endif

typedef unsigned long khronos_utime_nanoseconds_t;
typedef signed long long khronos_stime_nanoseconds_t;

#define KHRONOS_MAX_ENUM 0x7FFFFFFF

typedef enum {
    KHRONOS_FALSE = 0,
    KHRONOS_TRUE  = 1,
    KHRONOS_BOOLEAN_ENUM_FORCE_SIZE = KHRONOS_MAX_ENUM
} khronos_boolean_enum_t;

#endif /* __khrplatform_h_ */
