#ifndef GLOBAL_BUMP_ALLOC_H
#define GLOBAL_BUMP_ALLOC_H



#include "global.h"


#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
  u8* data;
  u32 size;
  u32 pos;

}bump_alloc_t;


// @DOC: initializes bump_allocator to specified size
//       ! needs to be free'd using bump_free()
//       reset for reusage using bump_reset()
#define bump_init(_alloc, _size) bump_init_dbg(_alloc, _size, __FILE__, __LINE__)
INLINE void bump_init_dbg(bump_alloc_t* alloc, u32 size, const char* _file, const int _line)
{
  TRACE();

  ERR_CHECK(alloc != NULL,       "alloc is null pointer\n\t->file. %s, line: %d\n", _file, _line);
  ERR_CHECK(alloc->data == NULL, "alloc->data isnt null pointer\n\t->file. %s, line: %d\n", _file, _line);
  ERR_CHECK(size > 0,            "size needs to be bigger than 0\n\t->file. %s, line: %d\n", _file, _line);

  MALLOC(alloc->data, size);
  alloc->size = size;
  alloc->pos  = 0;
}

// @DOC: returns pointer to memory in pre-allocated bump-memory
//       ! need to call bump_init() first
//       reset for reusage using bump_reset()
#define bump_alloc(_alloc, _size) bump_alloc_dbg(_alloc, _size, __FILE__, __LINE__)
INLINE void* bump_alloc_dbg(bump_alloc_t* alloc, u32 size, const char* _file, const int _line)
{
  TRACE();

  ERR_CHECK(alloc != NULL,       "alloc is null pointer\n\t->file. %s, line: %d\n", _file, _line);
  ERR_CHECK(alloc->data != NULL, "alloc->data is null pointer, call bump_init() first\n\t->file. %s, line: %d\n", _file, _line);

  if (alloc->pos + size < alloc->size)
  {
    // @UNSURE: set memory to 0
    alloc->pos += size;
    return &alloc->data[alloc->pos];
  }
  ERR("bump_alloc ran out of memory\n\t->file. %s, line: %d\n", _file, _line);
  return NULL;
}



// @DOC: reset the bump allocator for reusage
//       ! doesnt free just resets to be overwritten by next bump_alloc()
//       ! use bump_free() to actually free
#define bump_reset(_alloc) bump_reset_dbg(_alloc, __FILE__, __LINE__)
INLINE void bump_reset_dbg(bump_alloc_t* alloc, const char* _file, const int _line)
{
  TRACE();

  ERR_CHECK(alloc != NULL, "alloc is null pointer\n\t->file. %s, line: %d\n", _file, _line);
  alloc->pos = 0;
}



// @DOC: frees memory allocated in bump_init()
//       ! after calling this need ti call bump_init(), 
//         before calling bump_alloc() again
#define bump_free(_alloc) bump_free_dbg(_alloc, __FILE__, __LINE__)
INLINE void bump_free_dbg(bump_alloc_t* alloc, const char* _file, const int _line)
{
  TRACE();

  ERR_CHECK(alloc != NULL,       "alloc is null pointer\n\t->file. %s, line: %d\n", _file, _line);
  ERR_CHECK(alloc->data != NULL, "alloc->data is null pointer, call bump_init() first\n\t->file. %s, line: %d\n", _file, _line);

  FREE(alloc->data);
  alloc->data = NULL;
  alloc->size = 0;
  alloc->pos  = 0;
}

#ifdef __cplusplus
} // extern C
#endif

#endif  // GLOBAL_BUMP_ALLOC_H
