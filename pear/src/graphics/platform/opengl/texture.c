#ifdef PEAR_PLATFORM_OPENGL

#include <graphics/texture.h>
#include <graphics/platform/opengl/texture.h>
#include <GL/glew.h>
#include <core/alloc.h>

typedef struct texture_t {
    u32 width;
    u32 height;
    u32 id;
} texture_t;

texture_format_t texture_num_channels_to_format(u32 num_channels) {
    switch (num_channels) {
    case 1:
        return TEXTURE_FORMAT_R;

    case 2:
        return TEXTURE_FORMAT_RG;

    case 3:
        return TEXTURE_FORMAT_RGB;

    default:
    case 4:
        return TEXTURE_FORMAT_RGBA;
    }
}

GLint texture_format_to_opengl(texture_format_t format) {
    switch (format) {
    case TEXTURE_FORMAT_R:
        return GL_RED;

    case TEXTURE_FORMAT_RG:
        return GL_RG;

    case TEXTURE_FORMAT_RGB:
        return GL_RGB;

    default:
    case TEXTURE_FORMAT_RGBA:
        return GL_RGBA;
    }
}

GLint texture_num_channels_to_opengl(u32 num_channels) {
    return texture_format_to_opengl(texture_num_channels_to_format(num_channels));
}

texture_t* texture_create(texture_wrapping_t wrapping, texture_filtering_t filtering) {
    texture_t* texture = (texture_t*)PEAR_MALLOC(sizeof(texture_t));

    glGenTextures(1, &(texture->id));
    glBindTexture(GL_TEXTURE_2D, texture->id);

    GLenum gl_wrapping;
    switch (wrapping) {
    case TEXTURE_WRAPPING_REPEAT:
        gl_wrapping = GL_REPEAT;
        break;

    case TEXTURE_WRAPPING_REPEAT_MIRROR:
        gl_wrapping = GL_MIRRORED_REPEAT;
        break;

    case TEXTURE_WRAPPING_CLAMP:
        gl_wrapping = GL_CLAMP_TO_EDGE;
        break;

    case TEXTURE_WRAPPING_NONE:
        gl_wrapping = GL_CLAMP_TO_BORDER;
        break;
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, gl_wrapping);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, gl_wrapping);

    GLenum gl_filtering;
    switch (filtering) {
    case TEXTURE_FILTERING_NEAREST:
        gl_filtering = GL_NEAREST;
        break;

    case TEXTURE_FILTERING_LINEAR:
        gl_filtering = GL_LINEAR;
        break;
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, gl_filtering);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, gl_filtering);

    return texture;
}

texture_t* texture_new(u32 width, u32 height, texture_wrapping_t wrapping, texture_filtering_t filtering, texture_format_t format) {
    texture_t* texture = texture_create(wrapping, filtering);
    texture->width = width;
    texture->height = height;
    GLint gl_format = texture_format_to_opengl(format);
    glTexImage2D(GL_TEXTURE_2D, 0, gl_format, width, height, 0, gl_format, GL_UNSIGNED_BYTE, NULL);
    return texture;
}

texture_t* texture_new_depth(u32 width, u32 height, texture_wrapping_t wrapping, texture_filtering_t filtering) {
    texture_t* texture = texture_create(wrapping, filtering);
    texture->width = width;
    texture->height = height;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    return texture;
}

texture_t* texture_new_from_image(image_t* image, texture_wrapping_t wrapping, texture_filtering_t filtering) {
    texture_t* texture = texture_create(wrapping, filtering);
    GLint gl_format = texture_num_channels_to_opengl(image_get_num_channels(image));
    glTexImage2D(GL_TEXTURE_2D, 0, gl_format, image_get_width(image), image_get_height(image), 0, gl_format, GL_UNSIGNED_BYTE, image_get_data(image));
    glGenerateMipmap(GL_TEXTURE_2D);
    return texture;
}

void texture_delete(texture_t* texture) {
    glDeleteTextures(1, &(texture->id));
    free(texture);
}

u32 texture_get_width(texture_t* texture) {
    return texture->width;
}

u32 texture_get_height(texture_t* texture) {
    return texture->height;
}

u32 texture_get_id(texture_t* texture) {
    return texture->id;
}

void texture_use(texture_t* texture, u32 texture_id) {
    glActiveTexture(GL_TEXTURE0 + texture_id);
    glBindTexture(GL_TEXTURE_2D, texture->id);
}

#endif
