#ifdef PEAR_PLATFORM_LINUX

#include <string.h>

void fs_get_path(const char* path, char* valid_path) {
    strcpy(valid_path, path);
}

#endif
