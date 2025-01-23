#ifndef PEAR_UTIL_FILESYSTEM_H_
#define PEAR_UTIL_FILESYSTEM_H_

#include <core/types.h>

char* filesystem_read_file(const char* filename);
bool filesystem_file_exists(const char* filename);

#endif
