#ifndef PEAR_UTIL_ARRAY_H_
#define PEAR_UTIL_ARRAY_H_

#include <core/types.h>

typedef struct array_t array_t;

array_t* array_new(u32 size);
void array_delete(array_t* array);

u32 array_add(array_t* array, void* data);
void array_remove(array_t* array, void* data);
void array_remove_index(array_t* array, u32 index);
void array_clear(array_t* array);

void* array_get(array_t* array, u32 index);
u32 array_get_index(array_t* array, void* data);
void** array_get_data(array_t* array);

u32 array_get_size(array_t* array);
u32 array_get_length(array_t* array);

#endif
