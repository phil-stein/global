#ifndef GLOBAL_GLOBAL_TYPES_H
#define GLOBAL_GLOBAL_TYPES_H


// include all files like this once and the only use global.h
// // order is important, io_util & str_util before global
// 
// #define IO_UTIL_IMPLEMENTATION       // only define once
// #include "global/io_util.h"          // only need to include here, normally included via global.h
// 
// #define STR_UTIL_IMPLEMENTATION      // only define once
// #include "global/str_util.h"         // only need to include here, normally included via global.h
// 
// for printing macros, define globally:
//  GLOBAL_DEBUG          (-DGLOBAL_DEBUG)                  : compile in/out P/PF/P_ macros
//  PF_PRINT_LOCATION     (-DPF_PRINT_LOCATION)             : make P/PF/P_ macros print location
// for tracing, define globally:
//  TRACE_PRINT_LOCATION  (-DTRACE_PRINT_LOCATION)          : compile in TRACE macros
//  TRACE_LOG_PATH        (-DTRACE_LOG_PATH=\"trace.log\")  : path/name of trace log file
//  TRACE_LOG_MAX_LINES   (-DTRACE_LOG_MAX_LINES=20)        : max lines output to log file
// globally define GLOBAL_DEFINE_BOOL to reassign bool (-DGLOBAL_DEFINE_BOOL)
// #define GLOBAL_BOOL_TYPE int/u8/etc. // optional is char by default
// #include "global/global.h"


// other headers in this repo, included at end of file, 
// bc. they need the defines made in global.h

// libs needed basically everywhere
#include <stdbool.h>
#include <inttypes.h>


#ifdef __cplusplus
extern "C" {
#endif




// -- null --
#ifndef NULL
	#define NULL (void*)0
#endif

// -- bool --
#ifndef GLOBAL_BOOL_TYPE
  #define GLOBAL_BOOL_TYPE char
#endif

// #define GLOBAL_DEFINE_BOOL 
#ifdef GLOBAL_DEFINE_BOOL 
  #define bool GLOBAL_BOOL_TYPE
	#define true 1
	#define false 0
#endif


// ---- typedefs ----

// @DOC: int, long, etc. not guaranteed to be excactly 32/64 bit
//       int32_t/uint64_t etc is
// typedef char			          s8;
// typedef short			          s16;
// typedef int			            s32;
// typedef long long		        s64;
// 
// typedef unsigned char		    u8;
// typedef unsigned short		  u16;
// typedef unsigned int 		    u32;
// typedef unsigned long	long  u64;
typedef int8_t      s8;
typedef int16_t     s16;
typedef int32_t     s32;
typedef int64_t 		s64;

typedef uint8_t     u8;
typedef uint16_t    u16;
typedef uint32_t    u32;
typedef uint64_t    u64;

typedef float 			f32;
typedef double			f64;

typedef void (empty_callback)(void);

// @DOC: inlining works different in msvc and gcc
#if defined(_MSC_VER)
#  define INLINE __forceinline
#else
#  define INLINE static inline __attribute((always_inline))
#endif

// ---- helper ----

// @DOC: make number with bit a set 
//       1 << 0 0000
//       1 << 1 0001
//       1 << 2 0010
//       1 << 3 0100
//       1 << 4 1000
//       aka. 1<<a == 2^a, 0, 2, 4, 8, ...
#define FLAG(a) (1 << (a)) 

// @DOC: check if flag a and b have a set bit in common
//       a: 00010010
//       b: 00000010
//       &: 00000010 true bc. >1
#define HAS_FLAG(a, b)   ((a) & (b))

// @DOC: set all shared bits between a and b to 0 in a 
#define REMOVE_FLAG(a, b) ((a) &= ~(b))




#ifdef __cplusplus
} // extern C
#endif

#endif // GLOBAL_GLOBAL_TYPES_H
