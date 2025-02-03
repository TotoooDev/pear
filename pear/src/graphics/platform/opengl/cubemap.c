#ifdef PEAR_PLATFORM_OPENGL

#include <graphics/cubemap.h>
#include <graphics/platform/opengl/cubemap.h>
#include <graphics/platform/opengl/texture.h>
#include <core/alloc.h>
#include <glad/glad.h>
#include <string.h>

typedef struct cubemap_t {
    u32 id;
    char path[1024];
} cubemap_t;

cubemap_t* cubemap_create(texture_wrapping_t wrapping, texture_filtering_t filtering) {
    cubemap_t* cubemap = (cubemap_t*)PEAR_MALLOC(sizeof(cubemap_t));

    glGenTextures(1, &cubemap->id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap->id);

    GLenum gl_wrapping = texture_wrapping_to_opengl(wrapping);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, gl_wrapping);	
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, gl_wrapping);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, gl_wrapping);

    GLenum gl_filtering = texture_filtering_to_opengl(filtering);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, gl_filtering);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, gl_filtering);

    return cubemap;
}

cubemap_t* cubemap_new(u32 width, u32 height, texture_wrapping_t wrapping, texture_filtering_t filtering, texture_format_t format) {
    cubemap_t* cubemap = cubemap_create(wrapping, filtering);
    
    for (u32 i = 0; i < 6; i++) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, texture_format_to_opengl(format), width, height, 0, texture_format_to_opengl(format), GL_UNSIGNED_BYTE, NULL);
    }
    
    return cubemap;
}

cubemap_t* cubemap_new_from_images(image_t** images, texture_wrapping_t wrapping, texture_filtering_t filtering) {
    cubemap_t* cubemap = cubemap_create(wrapping, filtering);
    
    for (u32 i = 0; i < 6; i++) {
        GLint gl_format = texture_num_channels_to_opengl(image_get_num_channels(images[i]));
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, gl_format, image_get_width(images[i]), image_get_height(images[i]), 0, gl_format, GL_UNSIGNED_BYTE, image_get_data(images[i]));
    }
    
    return cubemap;
}

void cubemap_delete(cubemap_t* cubemap) {
    glDeleteTextures(1, &cubemap->id);
    PEAR_FREE(cubemap);
}

void cubemap_use(cubemap_t* cubemap, u32 index) {
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap->id);
}

void cubemap_set_path(cubemap_t* cubemap, const char* path) {
    strncpy(cubemap->path, path, 1024);
}

char* cubemap_get_path(cubemap_t* cubemap) {
    return cubemap->path;
}

#endif
