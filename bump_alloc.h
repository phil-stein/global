#ifndef GLOBAL_BUMP_ALLOC_H
#define GLOBAL_BUMP_ALLOC_H



// @NOTE: need to define BUMP_ALLOC_IMPLEMENTATION once before including 
//        #define BUMP_ALLOC_IMPLEMENTATION
//        #define BUMP_ALLOC_SIZE 1024
//        #include "bump_alloc.h"

#include "global.h"


#ifdef __cplusplus
extern "C"
{
#endif

#ifndef BUMP_ALLOC_SIZE
  #define BUMP_ALLOC_SIZE 512
#endif

typedef struct
{
  u8 data[BUMP_ALLOC_SIZE];
  u32 pos;

}bump_alloc_t;

#ifdef __cplusplus
} // extern C
#endif
#endif  // GLOBAL_BUMP_ALLOC_H

// --- implementation ---

// @DOC: need to define this once before including 
#ifdef BUMP_ALLOC_IMPLEMENTATION
#ifdef __cplusplus
extern "C"
{
#endif

bump_alloc_t alloc;

void* bump_alloc(u32 size)
{
  if (alloc.pos + size < BUMP_ALLOC_SIZE)
  {
    // @UNSURE: set memory to 0
    alloc.pos += size;
    return &alloc.data[pos];
  }
  return NULL;
}

void bump_free()
{
  alloc.pos = 0;
}

#ifdef __cplusplus
} // extern C
#endif

#endif  // STR_UTIL_IMPLEMENTATION
