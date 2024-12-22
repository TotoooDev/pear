#ifdef PEAR_PLATFORM_OPENGL

#include <graphics/cubemap.h>
#include <graphics/platform/opengl/cubemap.h>

typedef struct cubemap_t {
    u32 id;
} cubemap_t;

cubemap_t* cubemap_new(u32 width, u32 height, texture_wrapping_t wrapping, texture_filtering_t filtering, texture_format_t format) {

}

cubemap_t* cubemap_new_from_images(image_t** images, texture_wrapping_t wrapping, texture_filtering_t filtering) {

}

void cubemap_delete(cubemap_t* cubemap) {

}

void cubemap_use(cubemap_t* cubemap) {

}

#endif
