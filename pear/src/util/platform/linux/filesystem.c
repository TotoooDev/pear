#ifdef PEAR_PLATFORM_LINUX

#include <core/log.h>
#include <stdio.h>
#include <sys/types.h>

char* fileystem_read_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        PEAR_ERROR("failed to open file %s!", filename);
        return NULL;
    }

    char* buffer = NULL;
    size_t length;
    ssize_t bytes_read = getdelim(&buffer, &length, '\0', file);
    if (bytes_read == -1) {
        PEAR_ERROR("failed to read file %s!", filename);
        return NULL;
    }

    fclose(file);

    return buffer;
}

#endif
