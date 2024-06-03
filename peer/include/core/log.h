#ifndef PEER_LOG_H
#define PEER_LOG_H

#include <core/types.h>

#ifdef PEER_DEBUG
    void peer_log(const char* message, ...);

    #define PEER_INFO(msg, ...) peer_log("[info] %s:%d %s(): "msg"\n", __FILE__, __LINE__, __func__, __VA_ARGS__)
    #define PEER_WARN(msg, ...) peer_log("[warn] %s:%d %s(): "msg"\n", __FILE__, __LINE__, __func__, __VA_ARGS__)
    #define PEER_ERROR(msg, ...) peer_log("[ERROR] %s:%d %s(): "msg"\n", __FILE__, __LINE__, __func__, __VA_ARGS__)
#else
    #define PEER_INFO(msg, ...)
    #define PEER_WARN(msg, ...)
    #define PEER_ERROR(msg, ...)
#endif

#endif
