#ifndef PEAR_ALLOC_H_
#define PEAR_ALLOC_H_

#include <stddef.h>

void* pear_malloc(size_t size);
void pear_free(void* data);
void* pear_realloc(void* data, size_t size);

#ifndef PEAR_MALLOC
    #define PEAR_MALLOC(size) pear_malloc(size)
#endif

#ifndef PEAR_FREE
    #define PEAR_FREE(data) pear_free(data)
#endif

#ifndef PEAR_REALLOC
    #define PEAR_REALLOC(data, size) pear_realloc(data, size)
#endif

#endif
