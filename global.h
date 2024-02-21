#ifndef GLOBAL_GLOBAL_H
#define GLOBAL_GLOBAL_H


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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#include "global_types.h"
#include "global_print.h"

#ifdef __cplusplus
extern "C" {
#endif


// @DOC: ifdef activates P... macros, ASSERT, ERR..., etc.
// #define GLOBAL_DEBUG
#ifdef GLOBAL_DEBUG

// -- trace --

// @NOTE: need to define 
//        TRACE_PRINT_LOCATION (-DTRACE_PRINT_LOCATION)
//        TRACE_LOG_PATH       (-DTRACE_LOG_PATH=\"trace.log\")
//        TRACE_LOG_MAX_LINES  (-DTRACE_LOG_MAX_LINES=20) 
//        ! globally

#ifdef TRACE_PRINT_LOCATION 

  // @DOC: how many chars max in line of trace log text
  #define TRACE_LOG_LINE_MAX 256
  // @DOC: how many lines in trace log text
  // #define TRACE_LOG_MAX_LINES 25
  
  // @DOC: file used for printig trace log to
  //       fopened in TRACE_INIT()
  extern FILE* __global_trace_file_pointer__;
  extern char  __global_trace_file_entries__[TRACE_LOG_MAX_LINES][TRACE_LOG_LINE_MAX];
  extern char* __global_trace_file_entry_ptrs__[TRACE_LOG_MAX_LINES];
  
  // @DOC:  calls init func defined in TRACE_REGISTER()
  #define TRACE_INIT_NAME  __global_trace_init__
  #define TRACE_INIT()     TRACE_INIT_NAME()
  
  // @DOC: defines function and extern FILE* used for writing trace log to file
  //       need to call TRACE_INIT() as well
  #define TRACE_REGISTER()                                                            \
    /* define extern FILE*, used for printting trace log to */                        \
    FILE* __global_trace_file_pointer__ = NULL;                                       \
    char  __global_trace_file_entries__[TRACE_LOG_MAX_LINES][TRACE_LOG_LINE_MAX];     \
    char* __global_trace_file_entry_ptrs__[TRACE_LOG_MAX_LINES];                      \
                                                                                      \
    void TRACE_INIT_NAME()                                                            \
    {                                                                                 \
      __global_trace_file_pointer__ = fopen(TRACE_LOG_PATH, "w+");                    \
      if(__global_trace_file_pointer__ == NULL)                                       \
      { ERR("couldnt open trace log file: %s\n", TRACE_LOG_PATH); }                   \
      /* rewind(__global_trace_file_pointer__);  / * set to beginnning of file */     \
      for (int i = 0; i < TRACE_LOG_MAX_LINES; ++i)                                   \
      { __global_trace_file_entry_ptrs__[i] =  __global_trace_file_entries__[i]; }    \
    }

  #define TRACE() __global_trace_func(__func__, __FILE__, __LINE__) 
  INLINE void __global_trace_func(const char* _func, const char* _file, const int _line) 
  {                                                                                     
    /* move lines around */                                                             
    __global_trace_file_entry_ptrs__[0] = __global_trace_file_entry_ptrs__[TRACE_LOG_MAX_LINES -1]; 
    
    for (int i = TRACE_LOG_MAX_LINES -1; i > 0; --i)                                            
    { 
      __global_trace_file_entry_ptrs__[i] = __global_trace_file_entry_ptrs__[i -1]; 
    }           
  
    sprintf(__global_trace_file_entry_ptrs__[0], 
        "%s, line: %d, file: %s\n",
        _func, _line, _file);                 
 
    // go to start of file, to overwrite
    // should work i think, but doesnt
    rewind(__global_trace_file_pointer__);  // set fprintf to beginnning of file 
    // fseek(__global_trace_file_pointer__, 0, SEEK_SET);
    // // super slow, but only this works
    // __global_trace_file_pointer__ = freopen(TRACE_LOG_PATH, "w", __global_trace_file_pointer__);
    // ASSERT(__global_trace_file_pointer__ != NULL);                                              
    
    // P_INT((int)ftell(__global_trace_file_pointer__));
   
    if (ftell(__global_trace_file_pointer__) != 0)
    {
      __global_trace_file_pointer__ = freopen(TRACE_LOG_PATH, "w", __global_trace_file_pointer__);
      ASSERT(__global_trace_file_pointer__ != NULL);                                              
    }

    for (int i = TRACE_LOG_MAX_LINES -1; i >= 0; --i)                                           
    { 
      // fprintf(__global_trace_file_pointer__, "%s\n", __global_trace_file_entry_ptrs__[i]); 
      fputs(__global_trace_file_entry_ptrs__[i], __global_trace_file_pointer__); 
      // fwrite(__global_trace_file_entry_ptrs__[i], sizeof(char), TRACE_LOG_LINE_MAX, __global_trace_file_pointer__); 
    }            
    // // fill rest of space
    
    // cop out, just putting bit of space between logs and garbage
    fputs("\n\n\n\n\n", __global_trace_file_pointer__); 
    
    // #define SPACE "          ""          ""          ""          ""          ""          ""          ""          ""          ""          ""          ""          ""          ""          ""          ""          ""          ""          ""          ""          "
    // fwrite(SPACE, sizeof(char), 200, __global_trace_file_pointer__); 
    // fwrite(SPACE, sizeof(char), 200, __global_trace_file_pointer__); 
    // fwrite(SPACE, sizeof(char), 200, __global_trace_file_pointer__); 
    // fwrite(SPACE, sizeof(char), 200, __global_trace_file_pointer__); 
    // fwrite(SPACE, sizeof(char), 200, __global_trace_file_pointer__); 
    // fwrite(SPACE, sizeof(char), 200, __global_trace_file_pointer__); 
    // fwrite(SPACE, sizeof(char), 200, __global_trace_file_pointer__); 
    // fwrite(SPACE, sizeof(char), 200, __global_trace_file_pointer__); 
    // fwrite(SPACE, sizeof(char), 200, __global_trace_file_pointer__); 
    // #undef SPACE

  }

#else   // TRACE_PRINT_LOCATION
  #define TRACE_INIT()   
  #define TRACE_REGISTER()
  #define TRACE()
#endif  // TRACE_PRINT_LOCATION

// -- func wrapper --

// @DOC: wrappers around standard functions to make the safer, can be compiled out

#define SPRINTF(max, ...)        ASSERT(sprintf(__VA_ARGS__) < (max))                   
#define STRCPY(dest, source)     ASSERT(strcpy(dest, source) != NULL)

#define MALLOC(ptr, size)        (ptr) = malloc(size);        ASSERT((ptr) != NULL)     
#define CALLOC(ptr, items, size) (ptr) = calloc(items, size); ASSERT((ptr) != NULL)     
#define REALLOC(ptr, size)       (ptr) = realloc(ptr, size);  ASSERT((ptr) != NULL)     
#define FREE(ptr)                ASSERT(ptr != NULL); free(ptr); ptr = NULL 

// stb_ds
#define ARRFREE(a)               arrfree((a)); (a) = NULL  
#define HMFREE(a)                hmfree((a));  (a) = NULL
#define SHFREE(a)                shfree((a));  (a) = NULL

// -- debug func --

// @UNSURE:
// @DOC: wraps a function with file and line args
//       void DBG(func(int arg))
//       ->
//       void func(int arg, const char* _file, const int _line)
//       this way it can be compiled out
#define DBG(func, ...)       (func)(__VA_ARGS__, const char* _file, const int _line)
// @DOC: file and line macro for defining DBG() funcs macros
//       void DBG(func(int arg))
//       #define func(arg)      func(arg DBG_F_L)
//       this way it can be compiled out
#define DBG_F_L              , __FILE__, __LINE__

#else // GLOBAL_DEBUG --------------------------------------------------------------------------------------

// @DOC: compile out all GLOBAL_DEBUG macros
// -- tracing --

#define TRACE()

// -- func wrapper --

#define SPRINTF(max, ...)     sprintf(__VA_ARGS__) 
#define STRCPY(dest, source)  strcpy(dest, source)
#define MALLOC(p, s)          (p) = malloc(s) 
#define CALLOC(p, s)          (p) = calloc(p, s)
#define REALLOC(p, s)         (p) = realloc(p, s)
#define FREE(n)               free(n); n = NULL
// @UNSURE: stb_ds
#define ARRFREE(a)         arrfree((a)); (a) = NULL  
#define HMFREE(a)          hmfree((a));  (a) = NULL
#define SHFREE(a)          shfree((a));  (a) = NULL


#define DBG(func, ...)      (func)(__VA_ARGS__) 
#define DBG_F_L              

#define TRACE_INIT()   
#define TRACE_REGISTER()
#define TRACE()

#endif // GLOBAL_DEBUG


#ifdef __cplusplus
} // extern C
#endif

// util headers in this repo
#include "io_util.h"    // needs IO_UTIL_IMPLEMENTATION    defined ONCE
#include "str_util.h"   // needs STR_UTIL_IMPLEMENTATION   defined ONCE
#include "bump_alloc.h" // needs BUMP_ALLOC_IMPLEMENTATION defined ONCE

#endif // GLOBAL_GLOBAL_H
