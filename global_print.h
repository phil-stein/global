#ifndef GLOBAL_GLOBAL_PRINT_H
#define GLOBAL_GLOBAL_PRINT_H


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
#include <math.h>
#include "global_types.h"

#ifdef __cplusplus
extern "C" {
#endif

// always have these macros

// @DOC: print an error with location, without stopping the execution
#define P_ERR(...)	PF_COLOR(PF_RED); _PF("[ERROR] "); PF_MODE_RESET(); _PF(__VA_ARGS__); P_LOCATION()
// @DOC: print an error with location if the condition c if false, stopping the execution
#define ASSERT(c)   if(!(c)) { PF_COLOR(PF_RED); _PF("[ASSERT]" ); PF_MODE_RESET(); _PF("'%s'\n", #c); P_LOCATION(); abort(); }
// @DOC: print an error with location, stopping the execution
#define ERR(...)  P_ERR(__VA_ARGS__); abort();
// @DOC: print an error with location, and custom message if the condition c if false, stopping the execution
#define ERR_CHECK(c, ...) if(!(c)) { ERR(__VA_ARGS__); }
// @DOC: print an error with location, and custom message if the condition c if false, without stopping the execution
#define P_ERR_CHECK(c, ...) if(!(c)) { P_ERR(__VA_ARGS__); }

// -- static assert --
// STATIC_ASSERT(MACRO >= 3);
// STATIC_ASSERT(MACRO >= 3, "error message");
#ifdef __STDC_VERSION__
    #if __STDC_VERSION__ > 201710L  // c23
    #define STATIC_ASSERT(...) static_assert(__VA_ARGS__)
    #elif __STDC_VERSION__ >= 201112L // c11, c18
    #define STATIC_ASSERT(...) _Static_assert(__VA_ARGS__)
    #else  // c99 or c89, no static_assert
    #define STATIC_ASSERT(...) P_ERR("c99/c89 doesnt support static assertion.\n") 
    #endif
#else   //  __STDC_VERSION__
#define STATIC_ASSERT(...) P_ERR("compiler doesnt support static assertion.\n") 
#endif  //  __STDC_VERSION__

// @DOC: turn macro input to string
#define TO_STR(v)         #v
#define EXPAND_TO_STR(v)  TO_STR(v)
// @DOC: turn bool to string
#define STR_BOOL(v) ((v) ? "true" : "false")
// @DOC: paste, aka. expand and combine macros
#define PASTE(a, b)                       a##b
#define PASTE_2(a, b)                     PASTE(a, b)
#define PASTE_3(a, b, c)                  PASTE_2(PASTE_2(a, b), c)
#define PASTE_4(a, b, c, d)               PASTE_2(PASTE_3(a, b, c), d)
#define PASTE_5(a, b, c, d, e)            PASTE_2(PASTE_4(a, b, c, d), e)
#define PASTE_6(a, b, c, d, e, f)         PASTE_2(PASTE_5(a, b, c, d, e), f)
#define PASTE_7(a, b, c, d, e, f, g)      PASTE_2(PASTE_6(a, b, c, d, e, f), g)
#define PASTE_8(a, b, c, d, e, f, g, h)   PASTE_2(PASTE_7(a, b, c, d, e, f, g), h)

// --- compile time: warning / message / error ---
#define __PRAGMA_LOC__ __FILE__ ", line: "EXPAND_TO_STR(__LINE__)" " 
#if defined(__GNUC__) || defined(__clang__)
#define DO_PRAGMA(x) _Pragma (#x)
#define _PRAGMA_MESSAGE(x)  DO_PRAGMA(message "[MESSAGE] " #x " -> "__PRAGMA_LOC__)
#define PRAGMA_MESSAGE(x)   _PRAGMA_MESSAGE(x)
#define _PRAGMA_WARNING(x)  DO_PRAGMA(GCC warning "[WARNING] " #x " -> "__PRAGMA_LOC__)  
#define PRAGMA_WARNING(x)   _PRAGMA_WARNING(x)  
#define _PRAGMA_ERROR(x)    DO_PRAGMA(GCC error "[ERROR] " #x " -> "__PRAGMA_LOC__)  
#define PRAGMA_ERROR(x)     _PRAGMA_ERROR(x)  
#endif // __GNUC__
#if !defined(__clang__) && defined(_MSC_VER)
#define _PRAGMA_WARNING(x)  __pragma(message("[WARNING] " #x " -> "__PRAGMA_LOC__))
#define PRAGMA_WARNING(x)   _PRAGMA_WARNING(x)
#define _PRAGMA_MESSAGE(x)  __pragma(message("[MESSAGE] " #x " -> "__PRAGMA_LOC__))
#define PRAGMA_MESSAGE(x)   _PRAGMA_MESSAGE(x) 
#define _PRAGMA_ERROR(x)    __pragma(message("[ERROR] " #x " -> "__PRAGMA_LOC__))
#define PRAGMA_ERROR(x)     _PRAGMA_ERROR(x)
#endif  // __clang__ || _MSC_VER



// @DOC: ifdef activates P... macros, ASSERT, ERR..., etc.
// #define GLOBAL_DEBUG
#ifdef GLOBAL_DEBUG

// -- style --

// @DOC: used for setting terminal output to a specific mode, using PF_MODE(), PF_STYLE, etc.
typedef enum pf_mode
{
  PF_NORMAL     = 0,
  PF_BOLD       = 1,    // bright
  PF_DIM        = 2,
  PF_ITALIC     = 3,
  PF_UNDERLINE  = 4,
  PF_REVERSE    = 5,
  PF_HIDDEN     = 6
}pf_mode;
// @DOC: used for setting terminal output to a specific text color, using PF_MODE(), PF_STYLE, etc.
typedef enum pf_fg
{
  PF_BLACK    = 30,
  PF_RED      = 31,
  PF_GREEN    = 32,
  PF_YELLOW   = 33,
  PF_BLUE     = 34,
  PF_PURPLE   = 35,
  PF_CYAN     = 36,
  PF_WHITE    = 37 
}pf_color;
// @DOC: used for setting terminal output to a specific background color, using PF_MODE(), PF_STYLE, etc.
typedef enum pf_bg
{
  PF_BG_BLACK    = 40,
  PF_BG_RED      = 41,
  PF_BG_GREEN    = 42,
  PF_BG_YELLOW   = 43,
  PF_BG_BLUE     = 44,
  PF_BG_PURPLE   = 45,
  PF_BG_CYAN     = 46,
  PF_BG_WHITE    = 47 
}pf_bg;

// @DOC: doesnt print location, just printf
#define _PF(...)		printf(__VA_ARGS__)

// @DOC: setting terminal output to a specific mode, text and background color
#define PF_MODE(style, fg, bg)   _PF("\033[%d;%d;%dm", style, fg, bg)
// @DOC: setting terminal output to a specific mode and text color
#define PF_STYLE(style, color)   _PF("\033[%d;%dm", style, color)
// @DOC: setting terminal output to a specific text color
#define PF_COLOR(color)          PF_STYLE(PF_NORMAL, color)
// @DOC: setting terminal output to default mode, text and background color
#define PF_MODE_RESET()          PF_MODE(PF_NORMAL, PF_WHITE, PF_BG_BLACK)
// @DOC: setting terminal output to default mode and text
#define PF_STYLE_RESET()         PF_STYLE(PF_NORMAL, PF_WHITE)

// -- print --

// @DOC: print location, as in file and line, append anywhere that info is usefull
#define _P_LOCATION(_file, _func, _line) PF_STYLE(PF_DIM, PF_WHITE); PF_STYLE(PF_ITALIC, PF_WHITE); _PF(" -> file: %s\n -> func: %s, line: %d\n", _file, _func, _line); PF_STYLE_RESET()
#define P_LOCATION() _P_LOCATION(__FILE__, __func__, __LINE__)
// @DOC: print location on all P_ macros or not
//       PF_IF_LOC() used in P/PF macros
// #define PF_PRINT_LOCATION
#ifdef PF_PRINT_LOCATION
  #define PF_IF_LOC() P_LOCATION() 
  #define _PF_IF_LOC(_file, _func, _line) _P_LOCATION(_file, _func, _line) 
#else
  #define PF_IF_LOC()
  #define _PF_IF_LOC(_file, _func, _line)  
#endif




#define PF(...)		  printf(__VA_ARGS__); PF_IF_LOC()                                                      // @DOC: printf
#define P(msg)		  _PF("%s\n", msg); PF_IF_LOC()                                                         // @DOC: pritnf with automatic \n
// #define P_INFO(msg) PF_COLOR(PF_YELLOW); _PF("[INFO] "); PF_STYLE_RESET(); _PF("%s\n", msg); P_LOCATION() // @DOC: P(), but always prints location
#define P_INFO(...) PF_COLOR(PF_YELLOW); _PF("[INFO] "); PF_STYLE_RESET(); _PF(__VA_ARGS__); P_LOCATION() // @DOC: P(), but always prints location

// @DOC: draw --- line as long as the current console is wide, only works on windows
#if defined( _WIN32 )
#define P_LINE()    { int w, h; io_util_get_console_size_win(&w, &h); for (int i = 0; i < w -1; ++i) { _PF("-"); } _PF("\n"); }

// @DOC: draw formatted string followed by line as wide as console
//       example: P_LINE_STR("hello"); P_LINE_STR("str: %s", str);
#define P_LINE_STR(...)                                                   \
                        {                                                 \
                          char buf[248];                                  \
                          SPRINTF(248, buf, __VA_ARGS__);                 \
                          int w, h; io_util_get_console_size_win(&w, &h); \
                          _PF("%s", buf); /* no location */               \
                          int i = strlen(buf) +2;                         \
                          while( i < w -1) { _PF("-"); i++; }             \
                          PF("\n");                                       \
                        }

#else
#define P_LINE()          P_ERR("P_LINE() currently only supported in windows")
#define P_LINE_STR(_str)  P_ERR("P_LINE_STR() currently only supported in windows")                                                
#endif

// -- print variables --

// @DOC: print the different types, e.g. P_INT(variable), highlights variable name cyan
#define P_SIGNED(v) 	PF_COLOR(PF_CYAN); _PF("%s", #v); PF_STYLE_RESET(); _PF(": %d\n", (v)); PF_IF_LOC()
#define P_INT(v) 	    P_SIGNED(v)
#define P_S32(v) 	    P_SIGNED(v) 
#define P_S16(v) 	    P_SIGNED(v) 
#define P_S8(v) 	    P_SIGNED(v) 
#define P_UNSIGNED(v) PF_COLOR(PF_CYAN); _PF("%s", #v); PF_STYLE_RESET(); _PF(": %u\n", (v)); PF_IF_LOC()
// #define P_U64(u)      printf("|%s| %"PRId64"\n", #u, u)
#define P_U64(v)      PF_COLOR(PF_CYAN); _PF("%s", #v); PF_STYLE_RESET(); _PF(": %"PRId64"\n", (v)); PF_IF_LOC()
#define P_U32(v)      P_UNSIGNED(v)
#define P_U16(v)      P_UNSIGNED(v)
#define P_U8(v)       P_UNSIGNED(v)

#define P_F32(v) 	    PF_COLOR(PF_CYAN); _PF("%s", #v); PF_STYLE_RESET(); _PF(": %f\n", (v)); PF_IF_LOC()
#define P_F64(v) 	    P_F32(v) 
// #define P_U64(u)   printf("|%s| %llu\n", #u, u)

#define P_BOOL(v) 	  PF_COLOR(PF_CYAN); _PF("%s", #v); PF_STYLE_RESET(); _PF(": %s\n", STR_BOOL(v)); PF_IF_LOC()

#define P_CHAR(v) 	  PF_COLOR(PF_CYAN); _PF("%s", #v); PF_STYLE_RESET(); _PF(": '%c'\n", (char)(v)); PF_IF_LOC()
#define P_STR(v) 	    PF_COLOR(PF_CYAN); _PF("%s", #v); PF_STYLE_RESET(); _PF(": \"%s\"\n", (v)); PF_IF_LOC() 
#define P_TXT(v)      PF_COLOR(PF_CYAN); _PF("%s", #v); PF_STYLE_RESET(); _PF(":\n%s\n", (v)); PF_IF_LOC()    

#define P_PTR(v) 	    PF_COLOR(PF_CYAN); _PF("%s", #v); PF_STYLE_RESET(); _PF(": %p\n", (v)); PF_IF_LOC()

// #define P_V(v) _Generic((v),                              \x
//         _Bool: "bool",                                    \x
//         char: "char",                                     \x
//         signed char: "signed char",                       \x
//         unsigned char: "unsigned char",                   \x
//         short int: "short int",                           \x
//         unsigned short int: "unsigned short int",         \x
//         int: "int",                                       \x
//         unsigned int: "unsigned int",                     \x
//         long int: "long int",                             \x
//         unsigned long int: "unsigned long int",           \x
//         long long int: "long long int",                   \x
//         unsigned long long int: "unsigned long long int", \x
//         float: "float",                                   \x
//         double: "double",                                 \x
//         long double: "long double",                       \x
//         char *: "pointer to char",                        \x
//         void *: "pointer to void",                        \x
//         int *: "pointer to int",                          \x
//         default: "other")

#define STR_TYPE(v) _Generic((v),                \
        bool:           "bool",                  \
        u8:             "u8",                    \
        u16:            "u16",                   \
        u32:            "u32",                   \
        u64:            "u64",                   \
        s8:             "s8",                    \
        s16:            "s16",                   \
        s32:            "s32",                   \
        s64:            "s64",                   \
        f32:            "f32",                   \
        f64:            "f64",                   \
        long:           "long",                  \
        unsigned long:  "unsigned long",         \
        void*:          "void*",                 \
        bool*:          "bool*",                 \
        u8*:            "u8*",                   \
        u16*:           "u16*",                  \
        u32*:           "u32*",                  \
        u64*:           "u64*",                  \
        s8*:            "s8*",                   \
        s16*:           "s16*",                  \
        s32*:           "s32*",                  \
        s64*:           "s64*",                  \
        f32*:           "f32*",                  \
        f64*:           "f64*",                  \
        long*:          "long*",                 \
        unsigned long*: "unsigned long*",        \
        default:        "other")

// @DOC: P_V(var) prints variable type-generic
//       s32 i = 123; P_V(i);
//       -> s32|i: 123
#define __P_FUNC(_name, _type, _pf_type)                                                                                              \
INLINE void _name(const char* _file, const char* _func, const int _line, const char* name, const char* type, _type v)                 \
{PF_COLOR(PF_YELLOW); _PF("%s", type); PF_COLOR(PF_WHITE); _PF("|"); PF_COLOR(PF_CYAN); _PF("%s", name); PF_STYLE_RESET(); _PF(": %"_pf_type"\n", v); _PF_IF_LOC(_file, _func, _line); }

INLINE void __P_UNKNOWN(const char* _file, const char* _func, const int _line, const char* name, const char* type, ...)     {PF_COLOR(PF_YELLOW); _PF("%s", type); PF_COLOR(PF_WHITE); _PF("|"); PF_COLOR(PF_CYAN); _PF("%s", name); PF_STYLE_RESET(); _PF(": UNKNOWN TYPE\n");             _PF_IF_LOC(_file, _func, _line); }
INLINE void __P_BOOL(   const char* _file, const char* _func, const int _line, const char* name, const char* type, bool v)  {PF_COLOR(PF_YELLOW); _PF("%s", type); PF_COLOR(PF_WHITE); _PF("|"); PF_COLOR(PF_CYAN); _PF("%s", name); PF_STYLE_RESET(); _PF(": %s\n", v ? "true" : "false"); _PF_IF_LOC(_file, _func, _line); }
__P_FUNC(__P_UNSIGNED,  u32,            "u")
__P_FUNC(__P_U64,       u64,            PRId64)
__P_FUNC(__P_SIGNED,    s32,            "d")
__P_FUNC(__P_S64,       s64,            "lld")
__P_FUNC(__P_LONG,      long,           "ld")
__P_FUNC(__P_ULONG,     unsigned long,  "lu")
__P_FUNC(__P_FLOAT,     f64,            "f")
__P_FUNC(__P_POINTER,   void*,          "p")

// INLINE void __P_UNSIGNED(const char* _file, const char* _func, const int _line, const char* name, const char* type, u32 v)   {PF_COLOR(PF_YELLOW); _PF("%s", type); PF_COLOR(PF_WHITE); _PF("|"); PF_COLOR(PF_CYAN); _PF("%s", name); PF_STYLE_RESET(); _PF(": %u\n", v);                    _PF_IF_LOC(_file, _func, _line); }
// INLINE void __P_U64(    const char* _file, const char* _func, const int _line, const char* name, const char* type, u64 v)   {PF_COLOR(PF_YELLOW); _PF("%s", type); PF_COLOR(PF_WHITE); _PF("|"); PF_COLOR(PF_CYAN); _PF("%s", name); PF_STYLE_RESET(); _PF(": %"PRId64"\n", v);             _PF_IF_LOC(_file, _func, _line); }
// INLINE void __P_SIGNED( const char* _file, const char* _func, const int _line, const char* name, const char* type, s32 v)   {PF_COLOR(PF_YELLOW); _PF("%s", type); PF_COLOR(PF_WHITE); _PF("|"); PF_COLOR(PF_CYAN); _PF("%s", name); PF_STYLE_RESET(); _PF(": %d\n", v);                    _PF_IF_LOC(_file, _func, _line); }
// INLINE void __P_LONG(   const char* _file, const char* _func, const int _line, const char* name, const char* type, long v)   {PF_COLOR(PF_YELLOW); _PF("%s", type); PF_COLOR(PF_WHITE); _PF("|"); PF_COLOR(PF_CYAN); _PF("%s", name); PF_STYLE_RESET(); _PF(": %l\n", v);                    _PF_IF_LOC(_file, _func, _line); }
// INLINE void __P_ULONG(   const char* _file, const char* _func, const int _line, const char* name, const char* type, unsigned long v)   {PF_COLOR(PF_YELLOW); _PF("%s", type); PF_COLOR(PF_WHITE); _PF("|"); PF_COLOR(PF_CYAN); _PF("%s", name); PF_STYLE_RESET(); _PF(": %lu\n", v);                    _PF_IF_LOC(_file, _func, _line); }
// INLINE void __P_S64(    const char* _file, const char* _func, const int _line, const char* name, const char* type, s64 v)   {PF_COLOR(PF_YELLOW); _PF("%s", type); PF_COLOR(PF_WHITE); _PF("|"); PF_COLOR(PF_CYAN); _PF("%s", name); PF_STYLE_RESET(); _PF(": %lld\n", v);                  _PF_IF_LOC(_file, _func, _line); }
// INLINE void __P_FLOAT(  const char* _file, const char* _func, const int _line, const char* name, const char* type, f64 v) 	{PF_COLOR(PF_YELLOW); _PF("%s", type); PF_COLOR(PF_WHITE); _PF("|"); PF_COLOR(PF_CYAN); _PF("%s", name); PF_STYLE_RESET(); _PF(": %f\n", v);                    _PF_IF_LOC(_file, _func, _line); }
// INLINE void __P_POINTER(const char* _file, const char* _func, const int _line, const char* name, const char* type, void* v) {PF_COLOR(PF_YELLOW); _PF("%s", type); PF_COLOR(PF_WHITE); _PF("|"); PF_COLOR(PF_CYAN); _PF("%s", name); PF_STYLE_RESET(); _PF(": %p\n", v);                    _PF_IF_LOC(_file, _func, _line); }
#define P_V(v) _Generic((v),                        \
        bool:           __P_BOOL,                   \
        u8:             __P_UNSIGNED,               \
        u16:            __P_UNSIGNED,               \
        u32:            __P_UNSIGNED,               \
        u64:            __P_U64,                    \
        s8:             __P_SIGNED,                 \
        s16:            __P_SIGNED,                 \
        s32:            __P_SIGNED,                 \
        s64:            __P_S64,                    \
        f32:            __P_FLOAT,                  \
        f64:            __P_FLOAT,                  \
        long:           __P_LONG,                   \
        unsigned long:  __P_ULONG,                  \
        void*:          __P_POINTER,                \
        bool*:          __P_POINTER,                \
        u8*:            __P_POINTER,                \
        u16*:           __P_POINTER,                \
        u32*:           __P_POINTER,                \
        u64*:           __P_POINTER,                \
        s8*:            __P_POINTER,                \
        s16*:           __P_POINTER,                \
        s32*:           __P_POINTER,                \
        s64*:           __P_POINTER,                \
        f32*:           __P_POINTER,                \
        f64*:           __P_POINTER,                \
        long*:          __P_POINTER,                \
        unsigned long*: __P_POINTER,                \
        default:        __P_UNKNOWN)(__FILE__, __func__, __LINE__, #v, STR_TYPE(v), v)

// @DOC: test P_V() and STR_TYPE() macros
INLINE void TEST_P_V()
{
    u8  _u8  = 128; u16 _u16 = 12345;
  u32 _u32 = 12345678; u64 _u64 = 123456778910111213;
  s8  _s8  = -126; s16 _s16 = -12345;
  s32 _s32 = -12345678; s64 _s64 = -123456778910111213;
  f32 _f32 = 1.23456f; f64 _f64 = 123.4567891011;
  bool  _bool  = true; _Bool __Bool = true;
  
  void* _voidp = NULL; u8*   _u8p   = &_u8; 
  u16*  _u16p  = &_u16; u32*  _u32p  = &_u32;
  u64*  _u64p  = &_u64; s8*   _s8p   = &_s8;
  s16*  _s16p  = &_s16; s32*  _s32p  = &_s32;
  s64*  _s64p  = &_s64; f32*  _f32p  = &_f32;
  f64*  _f64p  = &_f64; bool* _boolp = &_bool;

  long l = 0; long long ll = 0; 
  unsigned long ul = 0; unsigned long long ull = 0;

  P_STR(STR_TYPE(_u8)); P_STR(STR_TYPE(_u16)); P_STR(STR_TYPE(_u32)); P_STR(STR_TYPE(_u64));
  P_STR(STR_TYPE(_s8)); P_STR(STR_TYPE(_s16)); P_STR(STR_TYPE(_s32)); P_STR(STR_TYPE(_s64));
  P_STR(STR_TYPE(_f32));  P_STR(STR_TYPE(_f64)); P_STR(STR_TYPE(_bool)); P_STR(STR_TYPE(__Bool));

  P_STR(STR_TYPE(_voidp)); P_STR(STR_TYPE(_u8p)); P_STR(STR_TYPE(_u16p)); P_STR(STR_TYPE(_u32p));
  P_STR(STR_TYPE(_u64p)); P_STR(STR_TYPE(_s8p));  P_STR(STR_TYPE(_s16p)); P_STR(STR_TYPE(_s32p));
  P_STR(STR_TYPE(_s64p)); P_STR(STR_TYPE(_f32p)); P_STR(STR_TYPE(_f64p)); P_STR(STR_TYPE(_boolp));
  
  P_STR(STR_TYPE(l)); P_STR(STR_TYPE(ll)); P_STR(STR_TYPE(ul)); P_STR(STR_TYPE(ull));

  P_V(_bool); P_V(_u8); P_V(_u16); P_V(_u32); P_V(_u64); 
  P_V(_s8); P_V(_s16); P_V(_s32); P_V(_s64); P_V(_f32); P_V(_f64);

  P_V(_voidp); P_V(_boolp); P_V(_u8p); P_V(_u16p); P_V(_u32p); P_V(_u64p); 
  P_V(_s8p); P_V(_s16p); P_V(_s32p); P_V(_s64p); P_V(_f32p); P_V(_f64p);
  int _int = 12434; P_V(_int); float _float = 123.456f; P_V(_float);
  P_V(l); P_V(ll); P_V(ul); P_V(ull);
  P_V(&l); P_V(&ll); P_V(&ul); P_V(&ull);
}

// always print location

#define P_LOC_INT(v)  P_INT(v);  P_LOCATION()
#define P_LOC_S32(v) 	P_S32(v);  P_LOCATION() 
#define P_LOC_S16(v) 	P_S16(v);  P_LOCATION() 
#define P_LOC_S8(v) 	P_S8(v);   P_LOCATION()
#define P_LOC_U8(v)   P_U8(v);   P_LOCATION()
#define P_LOC_U32(v)  P_U32(v);  P_LOCATION()
#define P_LOC_U16(v)  P_U16(v);  P_LOCATION() 
#define P_LOC_F32(v) 	P_F32(v);  P_LOCATION()
#define P_LOC_BOOL(v) P_BOOL(v); P_LOCATION()
#define P_LOC_CHAR(v) P_CHAR(v); P_LOCATION()
#define P_LOC_STR(v) 	P_STR(v);  P_LOCATION() 
#define P_LOC_TXT(v)  P_TXT(v);  P_LOCATION()    
#define P_LOC_PTR(v)  P_PTR(v);  P_LOCATION()

// --- binary ---

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)   \
  ((byte) & 0x80 ? '1' : '0'), \
  ((byte) & 0x40 ? '1' : '0'), \
  ((byte) & 0x20 ? '1' : '0'), \
  ((byte) & 0x10 ? '1' : '0'), \
  ((byte) & 0x08 ? '1' : '0'), \
  ((byte) & 0x04 ? '1' : '0'), \
  ((byte) & 0x02 ? '1' : '0'), \
  ((byte) & 0x01 ? '1' : '0') 

// @DOC: print u8/s8 as binary without name 
#define PF_BIN8(v)  printf(""BYTE_TO_BINARY_PATTERN"\n", BYTE_TO_BINARY(v)); 

// @DOC: print u16/s16 as binary with name 
#define P_BIN8(v)   PF_COLOR(PF_CYAN); printf("%s", #v); PF_STYLE_RESET(); printf(": "); PF_BIN8(v); PF_IF_LOC();
// @DOC: print u16/s16 as binary without name 
#define PF_BIN16(v)  printf(""BYTE_TO_BINARY_PATTERN"."BYTE_TO_BINARY_PATTERN"\n", \
                     BYTE_TO_BINARY(v>>8), BYTE_TO_BINARY(v)); 

// @DOC: print u16/s16 as binary with name 
#define P_BIN16(v)   PF_COLOR(PF_CYAN); printf("%s", #v); PF_STYLE_RESET(); printf(": "); PF_BIN16(v); PF_IF_LOC();
// @DOC: print u32/s32 as binary without name 
#define PF_BIN32(v)  printf(""BYTE_TO_BINARY_PATTERN"."BYTE_TO_BINARY_PATTERN"."BYTE_TO_BINARY_PATTERN"."BYTE_TO_BINARY_PATTERN"\n", \
                     BYTE_TO_BINARY(v>>24), BYTE_TO_BINARY(v>>16), BYTE_TO_BINARY(v>>8), BYTE_TO_BINARY(v)); 
// @DOC: print u32/s32 as binary with name 
#define P_BIN32(v)   PF_COLOR(PF_CYAN); printf("%s", #v); PF_STYLE_RESET(); printf(": "); PF_BIN32(v); PF_IF_LOC();

// -- debug --

// @DOC: check f32/float is nan
#define F32_NAN(v)  (isnan(v) != 0)
// @DOC: print if f32/float is nan
#define P_NAN(v)    { if (F32_NAN(v)) { _PF("%s is nan\n", #v); PF_IF_LOC(); } }

// @DOC: print out all macros once for testing
#define GLOBAL_TEST_P_MACROS() { s32 int_32 = 0; s16 int_16 = 0; s8 int_8 = 0; u32 uint_32 = 0; u16 uint_16 = 0; u8 uint_8  = 0; f32 floating_point = 0.5f; bool b = true; s8 s_byte = '?'; char* str = "hello, there"; char* txt = "this is very textual\nmhhh yess\nlicrictically intricate"; \
P_INT(int_32); P_S32(int_32); P_S16(int_16); P_S8(int_8); P_U32(uint_32); P_U16(uint_16); P_U8(uint_8); P_F32(floating_point); P_BOOL(b); P_CHAR(s_byte); P_STR(str); P_TXT(txt); P_INFO("info"); P_ERR("hello\n"); /*ASSERT(0);*/ }


// @DOC: print the currently in use version of c
#ifdef __STDC_VERSION__ // legacy msvc doesnt define this for some reason

  #if __STDC_VERSION__ >  201710L
    #define C_VERSION_STR "c23"
  #elif __STDC_VERSION__ >=  201710L
    #define C_VERSION_STR "c18"
  #elif __STDC_VERSION__ >= 201112L
    #define C_VERSION_STR "c11"
  #elif __STDC_VERSION__ >= 199901L
    #define C_VERSION_STR "c99"
  #else
    #define C_VERSION_STR "c89"
  #endif // __STDC_VERSION__ > 201710L

  #ifndef __clang__
    #define P_C_VERSION() PF("using %s\n", C_VERSION_STR); 
  #else // __clang
    #define P_C_VERSION() PF("using %s\n", C_VERSION_STR); PRAGMA_MESSAGE(c version: C_VERSION_STR)
  #endif // __clang__

#else  // __STDC_VERSION__
#define P_C_VERSION()   P_ERR("compiler doesnt define __STDC_VERSION__\nprobably legacy msvc, define /std:11 or /std:17\n");
#endif // __STDC_VERSION__

// @DOC: print the currently used compiler and version
#ifdef __GNUC__
  #ifdef __MINGW64__
  #define P_COMPILER_VERSION()   PF("using mingw64 gcc version: %d.%d.%d\n", __GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__)
  #elif __MINGW32__
  #define P_COMPILER_VERSION()   PF("using mingw32 gcc version: %d.%d.%d\n", __GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__)
  #else 
  #define P_COMPILER_VERSION()   PF("using gcc version: %d.%d.%d\n", __GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__)
                                /* PRAGMA_MESSAGE(gcc version: __GNUC__ . __GNUC_MINOR__ . __GNUC_PATCHLEVEL__) */
  #endif // __MINGW__
#elif __clang__
#define P_COMPILER_VERSION()   PF("using clang version: %d.%d.%d\n", __clang_major__, __clang_minor__, __clang_patchlevel__); \
                               __pragma(message("clang version: " EXPAND_TO_STR(__clang_major__) "." EXPAND_TO_STR(__clang_minor__) "." EXPAND_TO_STR(__clang_patchlevel__) ))
#elif _MSC_VER
#if _MSC_VER <= 800
  #define MSC_VER_STR "1"
#elif _MSC_VER <= 900
  #define MSC_VER_STR "3"
#elif _MSC_VER <= 1020
  #define MSC_VER_STR "4"
#elif _MSC_VER <= 1100
  #define MSC_VER_STR "5"
#elif _MSC_VER <= 1200
  #define MSC_VER_STR "6"
#elif _MSC_VER <= 1300
  #define MSC_VER_STR "7"
#elif _MSC_VER <= 1310
  #define MSC_VER_STR "7.1 (2003)"
#elif _MSC_VER <= 1400
  #define MSC_VER_STR "8 (2005)"
#elif _MSC_VER <= 1500
  #define MSC_VER_STR "9 (2008)"
#elif _MSC_VER <= 1600
  #define MSC_VER_STR "10 (2010)"
#elif _MSC_VER <= 1700
  #define MSC_VER_STR "11 (2012)"
#elif _MSC_VER <= 1800
  #define MSC_VER_STR "12 (2013)"
#elif _MSC_VER <= 1900
  #define MSC_VER_STR "14 (2015)"
#elif _MSC_VER <=  	1916  
  #define MSC_VER_STR "15 (2017)"
#elif _MSC_VER <= 1929
  #define MSC_VER_STR "16 (2019)"
#elif _MSC_VER <= 1930
  #define MSC_VER_STR "17.0 (2022)"
#elif _MSC_VER <= 1931
  #define MSC_VER_STR "17.1 (2022)"
#elif _MSC_VER <= 1932
  #define MSC_VER_STR "17.2 (2022)"
#elif _MSC_VER <= 1933
  #define MSC_VER_STR "17.3 (2022)"
#elif _MSC_VER <= 1934
  #define MSC_VER_STR "17.4 (2022)"
#elif _MSC_VER <= 1935
  #define MSC_VER_STR "17.5 (2022)"
#elif _MSC_VER <= 1936
  #define MSC_VER_STR "17.6 (2022)"
#elif _MSC_VER <= 1937
  #define MSC_VER_STR "17.7 (2022)"
#elif _MSC_VER <= 1938
  #define MSC_VER_STR "17.8 (2022)"
#elif _MSC_VER <= 1939
  #define MSC_VER_STR "17.9 (2022)"
#elif _MSC_VER <= 1940
  #define MSC_VER_STR "17.10 (2022)"
#elif _MSC_VER > 1940
  #define MSC_VER_STR "newer than 17.10 (2022), which is _MSC_VER: 1940"
#else
  #define MSC_VER_STR "unknown"
#endif
#define P_COMPILER_VERSION()   PF("using msvc version: %s | _MSC_VER: %d | _MSC_FULL_VER: %d\n", MSC_VER_STR, _MSC_VER, _MSC_FULL_VER)
#else // no compiler macro defined
#define P_COMPILER_VERSION()   P_ERR("compiler doesnt define __STDC_VERSION__\n");
#endif // __STDC_VERSION__

#else // GLOBAL_DEBUG --------------------------------------------------------------------------------------

// @DOC: compile out all GLOBAL_DEBUG macros

#define _PF(...)	                	
#define PF_MODE(style, fg, bg)   
#define PF_STYLE(style, color)   
#define PF_COLOR(color)          
#define PF_MODE_RESET()          
#define PF_STYLE_RESET()         
#define P_LOCATION()             
#define PF_IF_LOC()
#define PF(...)		                	
#define P(msg)
#define P_INFO(...)   
#define P_LINE()

#define P_SIGNED(v) 	
#define P_INT(v) 	    
#define P_S32(v) 	    
#define P_S16(v) 	    
#define P_S8(v) 	    
#define P_UNSIGNED(v) 
#define P_U32(v)      
#define P_U16(v)      
#define P_U8(v)       
#define P_F32(v) 	    
#define P_BOOL(v) 	  
#define P_CHAR(v) 	  
#define P_STR(v) 	    
#define P_TXT(v)      

#define F32_NAN(v)  
#define P_NAN(v)    

#define GLOBAL_TEST_P_MACROS()
#define P_C_VERSION()                     

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)   \
  ((byte) & 0x80 ? '1' : '0'), \
  ((byte) & 0x40 ? '1' : '0'), \
  ((byte) & 0x20 ? '1' : '0'), \
  ((byte) & 0x10 ? '1' : '0'), \
  ((byte) & 0x08 ? '1' : '0'), \
  ((byte) & 0x04 ? '1' : '0'), \
  ((byte) & 0x02 ? '1' : '0'), \
  ((byte) & 0x01 ? '1' : '0') 

#endif // GLOBAL_DEBUG


#ifdef __cplusplus
} // extern C
#endif

#endif // GLOBAL_GLOBAL_PRINT_H
