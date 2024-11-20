#ifndef PEAR_LOG_H_
#define PEAR_LOG_H_

#include <stdio.h>

#ifdef PEAR_DEBUG
    #define PEAR_INFO(msg, ...) printf("[info] %s:%d %s(): "msg"\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__)
    #define PEAR_WARN(msg, ...) printf("[warn] %s:%d %s(): "msg"\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__)
    #define PEAR_ERROR(msg, ...) printf("[ERROR] %s:%d %s(): "msg"\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__)
#else
    #define PEAR_INFO(msg, ...)
    #define PEAR_WARN(msg, ...)
    #define PEAR_ERROR(msg, ...)
#endif

#endif
