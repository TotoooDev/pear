#ifndef PEAR_CONFIG_H
#define PEAR_CONFIG_H

// platform
#ifdef PEAR_PLATFORM_LINUX
    #define PEAR_PLATFORM_NAME "linux"
#else
    #define PEAR_PLATFORM_NAME "unknown"
#endif

// graphics api
#ifdef PEAR_PLATFORM_OPENGL
    #define PEAR_RENDERER_API_NAME "opengl"
#else
    #define PEAR_RENDERER_API_NAME "unknown"
#endif

// debug mode
#ifdef PEAR_DEBUG
    #define PEAR_IS_DEBUG true
#else
    #define PEAR_IS_DEBUG false
#endif

#endif
