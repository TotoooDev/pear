#ifndef PEAR_UTIL_FILESYSTEM_H_
#define PEAR_UTIL_FILESYSTEM_H_

char* filesystem_read_file(const char* filename);
void filesystem_read_file_buffer(const char* filename, void** buffer);

#endif
