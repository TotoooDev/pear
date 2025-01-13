#include <util/array.h>
#include <core/log.h>
#include <core/alloc.h>

typedef struct array_t {
    u32 size;
    u32 length;
    void** data;
} array_t;

array_t* array_new(u32 size) {
    array_t* array = (array_t*)PEAR_MALLOC(sizeof(array_t));

    array->size = size;
    array->length = 0;
    array->data = (void**)PEAR_MALLOC(sizeof(void*) * size);

    return array;
}

void array_delete(array_t* array) {
    PEAR_FREE(array->data);
    PEAR_FREE(array);
}

u32 array_add(array_t* array, void* data) {
    if (array->length >= array->size) {
        array->size = array->size + array->size / 2;
        array->data = PEAR_REALLOC(array->data, sizeof(void*) * array->size);
    }

    array->data[array->length] = data;
    array->length++;

    return array->length - 1;
}

void array_remove(array_t* array, void* data) {
    array_remove_index(array, array_get_index(array, data));
}

void array_remove_index(array_t* array, u32 index) {
    if (index >= array->length) {
        PEAR_ERROR("attempt to remove an item at index %d greater than the array length (%d elements)!", index, array->length);
        return;
    }

    for (u32 i = index; i < array->length - 1; i++) {
        array->data[i] = array->data[i + 1];
    }

    array->length--;
    array->data[array->length] = NULL;
}

void array_clear(array_t* array) {
    array->length = 0;
}

u32 array_get_index(array_t* array, void* data) {
    for (u32 i = 0; i < array->length; i++) {
        if (array->data[i] == data) {
            return i;
        }
    }

    return UINT32_MAX;
}

void** array_get_data(array_t* array) {
    return array->data;
}

void* array_get(array_t* array, u32 index) {
    return array->data[index];
}

u32 array_get_size(array_t* array) {
    return array->size;
}

u32 array_get_length(array_t* array) {
    return array->length;
}

