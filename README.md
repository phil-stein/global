# global
typedefs & macros i use in almost all projects <br>
for in terminal documentation see [term_docs](https://github.com/phil-stein/term_docs.git)


## global.h

__macros:__ <br>
```C
  NULL                    -> (void)*0  <br>
  GLOBAL_BOOL_TYPE        -> define before include or bool is int  <br>
  bool                    -> GLOBAL_BOOL_TYPE or int   <br>
  true                    -> 1  <br>
  false                   -> 0  <br>
  
  PF_...                  -> printf() helper  <br>
  P_...                   -> printf() helper with 'n' at the end   <br>
  P_LOC_...               -> printf() helper with 'n' file & line at the end  <br>
  
  PF_MODE(style, fg, bg)  -> set stdout   / printf() style, foreground-/background color  <br>
  PF_STYLE(style, color)  -> set stdout   / printf() style, foreground color  <br>
  PF_COLOR(color)         -> set stdout   / printf() foreground color  <br>
  PF_MODE_RESET()         -> reset stdout / printf() style, foreground-/background color  <br>
  PF_STYLE_RESET()        -> reset stdout / printf() style, foreground color  <br>
  
  INLINE                  -> inline functions: INLINE void func() <br>
  
  FLAG()                  -> x = FLAG(0), y = FLAG(1), for flag enums or variables <br>
  HAS_FLAG()              -> x = FLAG(0); bool b = HAS_FLAG(x, FLAG(0));  <br>
  REMOVE_FLAG()           -> x = FLAG(0) | FLAG(1); REMOVE_FLAG(x, FLAG(1));  <br>
  
  STR_VAR()               -> char* name = STR_VAL(variable);  <br>
  STR_BOOL()              -> bool b = true; char* state = STR_BOOL(b);  <br>
  ```
  
__typedefs:__ <br>
```c
  typedef char                s8; 
  typedef short               s16;
  typedef int                 s32;
  typedef long long           s64;

  typedef unsigned char       u8;
  typedef unsigned short      u16;
  typedef unsigned int        u32;
  typedef unsigned long	long  u64;

  typedef float               f32;
  typedef double              f64;

  typedef void (empty_callback)(void);
 ```
 ## str_util.h
 
 ## io_util.h
 
 
 
 
 
 
 
