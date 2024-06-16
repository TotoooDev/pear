#ifndef PEAR_LOG_H
#define PEAR_LOG_H

#include <core/types.h>

#ifdef PEAR_DEBUG
    void pear_log(const char* message, ...);

    #define PEAR_INFO(msg, ...) pear_log("[info] %s:%d %s(): "msg"\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__)
    #define PEAR_WARN(msg, ...) pear_log("[warn] %s:%d %s(): "msg"\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__)
    #define PEAR_ERROR(msg, ...) pear_log("[ERROR] %s:%d %s(): "msg"\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__)
#else
    #define PEAR_INFO(msg, ...)
    #define PEAR_WARN(msg, ...)
    #define PEAR_ERROR(msg, ...)
#endif

#endif
