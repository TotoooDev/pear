#ifdef PEAR_PLATFORM_OPENGL

#include <graphics/texture.h>
#include <graphics/platform/opengl/texture.h>
#include <GL/glew.h>
#include <core/alloc.h>

typedef struct texture_t {
    u32 width;
    u32 height;
    texture_format_t format;
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

GLint texture_num_channels_to_opengl(u32 num_channels) {
    return texture_format_to_opengl(texture_num_channels_to_format(num_channels));
}

texture_t* texture_create(texture_wrapping_t wrapping, texture_filtering_t filtering) {
    texture_t* texture = (texture_t*)PEAR_MALLOC(sizeof(texture_t));

    glGenTextures(1, &(texture->id));
    glBindTexture(GL_TEXTURE_2D, texture->id);

    GLenum gl_wrapping = texture_wrapping_to_opengl(wrapping);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, gl_wrapping);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, gl_wrapping);

    GLenum gl_filtering = texture_filtering_to_opengl(filtering);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, gl_filtering);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, gl_filtering);

    return texture;
}

texture_t* texture_new(u32 width, u32 height, texture_wrapping_t wrapping, texture_filtering_t filtering, texture_format_t format) {
    texture_t* texture = texture_create(wrapping, filtering);
    texture->width = width;
    texture->height = height;
    texture->format = format;
    GLint gl_format = texture_format_to_opengl(format);
    glTexImage2D(GL_TEXTURE_2D, 0, gl_format, width, height, 0, gl_format, GL_UNSIGNED_BYTE, NULL);
    return texture;
}

texture_t* texture_new_from_image(image_t* image, texture_wrapping_t wrapping, texture_filtering_t filtering) {
    texture_t* texture = texture_create(wrapping, filtering);
    texture->width = image_get_width(image);
    texture->height = image_get_height(image);
    texture->format = texture_num_channels_to_format(image_get_num_channels(image));
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

texture_format_t texture_get_format(texture_t* texture) {
    return texture->format;
}

GLenum texture_format_to_opengl(texture_format_t format) {
    switch (format) {
    case TEXTURE_FORMAT_R:
        return GL_RED;

    case TEXTURE_FORMAT_RG:
        return GL_RG;

    case TEXTURE_FORMAT_RGB:
        return GL_RGB;

    case TEXTURE_FORMAT_DEPTH:
        return GL_DEPTH_COMPONENT;

    case TEXTURE_FORMAT_STENCIL:
        return GL_STENCIL_INDEX;

    default:
    case TEXTURE_FORMAT_RGBA:
        return GL_RGBA;
    }
}

GLenum texture_wrapping_to_opengl(texture_wrapping_t wrapping) {
    switch (wrapping) {
    case TEXTURE_WRAPPING_REPEAT:
        return GL_REPEAT;

    case TEXTURE_WRAPPING_REPEAT_MIRROR:
        return GL_MIRRORED_REPEAT;

    case TEXTURE_WRAPPING_CLAMP:
        return GL_CLAMP_TO_EDGE;

    default:
    case TEXTURE_WRAPPING_NONE:
        return GL_CLAMP_TO_BORDER;
    }
}

GLenum texture_filtering_to_opengl(texture_filtering_t filtering) {
    switch (filtering) {
    case TEXTURE_FILTERING_NEAREST:
        return GL_NEAREST;

    default:
    case TEXTURE_FILTERING_LINEAR:
        return GL_LINEAR;
    }
}

u32 texture_get_id(texture_t* texture) {
    return texture->id;
}

void texture_use(texture_t* texture, u32 texture_id) {
    glActiveTexture(GL_TEXTURE0 + texture_id);
    glBindTexture(GL_TEXTURE_2D, texture->id);
}

void texture_set_border_color(texture_t* texture, vec4 color) {
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);
}

#endif
