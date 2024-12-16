#include <core/alloc.h>
#include <core/log.h>
#include <stdlib.h>

void* pear_malloc(size_t size) {
    void* data = malloc(size);
    if (data == NULL) {
        PEAR_ERROR("failed to allocate %ld bytes of memory! something is terribly wrong...", size);
    }

    return data;
}

void pear_free(void* data) {
    free(data);
}

void* pear_realloc(void* data, size_t size) {
    return realloc(data, size);
}
