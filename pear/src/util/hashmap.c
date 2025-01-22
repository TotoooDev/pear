#include <util/hashmap.h>
#include <util/array.h>
#include <core/alloc.h>

typedef struct pair_t {
    u64 hash;
    void* value;
} pair_t;

typedef struct hashmap_t {
    array_t* pairs;
} hashmap_t;

u64 hashmap_hash_djb2(const char* str) {
    // from http://www.cse.yorku.ca/~oz/hash.html

    u64 hash = 5381;
    i32 c;

    while (c = *str++) {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }

    return hash;
}

hashmap_t* hashmap_new() {
    hashmap_t* hashmap = (hashmap_t*)PEAR_MALLOC(sizeof(hashmap_t));
    hashmap->pairs = array_new(5);
    return hashmap;
}

void hashmap_delete(hashmap_t* hashmap) {
    array_delete(hashmap->pairs);
    PEAR_FREE(hashmap);
}

void hashmap_set(hashmap_t* hashmap, const char* str, void* ptr) {
    u64 hash = hashmap_hash_djb2(str);
    for (u32 i = 0; i < array_get_length(hashmap->pairs); i++) {
        pair_t* pair = array_get(hashmap->pairs, i);
        if (pair->hash == hash) {
            pair->value = ptr;
        }
    }

    pair_t* pair = (pair_t*)PEAR_MALLOC(sizeof(pair_t));
    pair->hash = hash;
    pair->value = ptr;
    array_add(hashmap->pairs, pair);
}

void* hashmap_get(hashmap_t* hashmap, const char* str) {
    u64 hash = hashmap_hash_djb2(str);
    for (u32 i = 0; i < array_get_length(hashmap->pairs); i++) {
        pair_t* pair = array_get(hashmap->pairs, i);
        if (pair->hash == hash) {
            return pair->value;
        }
    }

    return NULL;
}

void hashmap_remove(hashmap_t* hashmap, const char* str) {
    u64 hash = hashmap_hash_djb2(str);
    for (u32 i = 0; i < array_get_length(hashmap->pairs); i++) {
        pair_t* pair = array_get(hashmap->pairs, i);
        if (pair->hash == hash) {
            array_remove_index(hashmap->pairs, i);
            return;
        }
    }
}

bool hashmap_exists(hashmap_t* hashmap, const char* str) {
    u64 hash = hashmap_hash_djb2(str);
    for (u32 i = 0; i < array_get_length(hashmap->pairs); i++) {
        pair_t* pair = array_get(hashmap->pairs, i);
        if (pair->hash == hash) {
            return true;
        }
    }

    return false;
}
