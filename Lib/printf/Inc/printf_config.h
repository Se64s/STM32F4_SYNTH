/**
 * @file    printf_config.h
 * @author  Sebastián Del Moral Gallardo.
 * @brief   Config file for printf library.
 * @version 0.1
 * @date    2021-10-25
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef _PRINTF_CONFIG_H_
#define _PRINTF_CONFIG_H_

// 'ntoa' conversion buffer size, this must be big enough to hold one converted
// numeric number including padded zeros (dynamically created on stack)
// default: 32 byte
#define PRINTF_NTOA_BUFFER_SIZE    (32U)

// 'ftoa' conversion buffer size, this must be big enough to hold one converted
// float number including padded zeros (dynamically created on stack)
// default: 32 byte
#define PRINTF_FTOA_BUFFER_SIZE    (32U)

// support for the floating point type (%f)
#define PRINTF_DISABLE_SUPPORT_FLOAT

// support for exponential floating point notation (%e/%g)
// #define PRINTF_DISABLE_SUPPORT_EXPONENTIAL

// define the default floating point precision
#define PRINTF_DEFAULT_FLOAT_PRECISION  6U

// define the largest float suitable to print with %f
#define PRINTF_MAX_FLOAT  1e9

// support for the long long types (%llu or %p)
// #define PRINTF_DISABLE_SUPPORT_LONG_LONG

// support for the ptrdiff_t type (%t)
// ptrdiff_t is normally defined in <stddef.h> as long or long long type
// #define PRINTF_DISABLE_SUPPORT_PTRDIFF_T

#endif // _PRINTF_CONFIG_H_