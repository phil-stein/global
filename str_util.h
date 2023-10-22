#ifndef GLOBAL_STR_UTIL_H
#define GLOBAL_STR_UTIL_H

#ifdef __cplusplus
extern "C"
{
#endif


// @NOTE: need to define STR_UTIL_IMPLEMENTATION once before including 
//        #define STR_UTIL_IMPLEMENTATION
//        #include "str_util.h"

#include "global.h"

#include <string.h>
#include <math.h>   // abs()

// @DOC: find last occurance of str
char* str_util_find_last_of(char* str, char* identifier);
// @DOC: truncate str at pos
char* str_util_trunc(char* str, int pos);

#endif

// @DOC: need to define this once before including 
#ifdef STR_UTIL_IMPLEMENTATION

char* str_util_find_last_of(char* str, char* identifier)
{
    char* last = NULL;
    int n = strlen(identifier);
    while (*str)
    {
        // go through str and compare with identifier
        if (!memcmp(str++, identifier, n))
        {
            last = str - 1;
        }
    }
    return last;
}

char* str_util_trunc(char* str, int pos)
{
    int len = strlen(str);

    if (len > abs(pos)) 
    {
        if (pos > 0)
        {
            str = str + pos;
        }
        else
        {
            str[len + pos] = '\0';
        }
    }

    return str;
}

#ifdef __cplusplus
} // extern C
#endif

#endif
