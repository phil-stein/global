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
#define P_LOCATION() PF_STYLE(PF_DIM, PF_WHITE); PF_STYLE(PF_ITALIC, PF_WHITE); _PF(" -> file: %s\n -> func: %s, line: %d\n", __FILE__, __func__, __LINE__); PF_STYLE_RESET()
// @DOC: print location on all P_ macros or not
//       PF_IF_LOC() used in P/PF macros
// #define PF_PRINT_LOCATION
#ifdef PF_PRINT_LOCATION
  #define PF_IF_LOC() P_LOCATION() 
#else
  #define PF_IF_LOC()
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
#ifdef __STDC_VERSION__
#define P_C_VERSION()                     \
  if (__STDC_VERSION__ >  201710L)        \
  { PF("-- using c23 --\n"); }            \
  else if (__STDC_VERSION__ >=  201710L)  \
  { PF("-- using c18 --\n"); }            \
  else if (__STDC_VERSION__ >= 201112L)   \
  { PF("-- using c11 --\n"); }            \
  else if (__STDC_VERSION__ >= 199901L)   \
  { PF("-- using c99 --\n"); }            \
  else                                    \
  { PF("-- using c89/c90 --\n"); }
#else  // __STDC_VERSION__
#define P_C_VERSION()   P_ERR("compiler doesnt define __STDC_VERSION__\n");
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

// #define P_ERR(...)	  
// #define ASSERT(c)     
// #define ERR(...)      
// #define ERR_CHECK(c, ...)   
// #define P_ERR_CHECK(c, ...) 

#ifdef __cplusplus
} // extern C
#endif

#endif // GLOBAL_GLOBAL_PRINT_H
