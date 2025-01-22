#ifndef PEAR_UTIL_HASHMAP_H_
#define PEAR_UTIL_HASHMAP_H_

#include <core/types.h>

typedef struct hashmap_t hashmap_t;

hashmap_t* hashmap_new();
void hashmap_delete(hashmap_t* hashmap);

void hashmap_set(hashmap_t* hashmap, const char* str, void* ptr);
void* hashmap_get(hashmap_t* hashmap, const char* str);
void hashmap_remove(hashmap_t* hashmap, const char* str);

bool hashmap_exists(hashmap_t* hashmap, const char* str);

#endif
