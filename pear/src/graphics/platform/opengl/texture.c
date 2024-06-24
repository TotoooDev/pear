#ifdef PEAR_PLATFORM_OPENGL

#include <graphics/texture.h>
#include <graphics/platform/opengl/texture.h>
#include <GL/glew.h>
#include <stdlib.h>

typedef struct Texture {
    u32 id;
} Texture;

Texture* texture_create(TextureWrapping wrapping, TextureFiltering filtering) {
    Texture* texture = (Texture*)malloc(sizeof(Texture));

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

Texture* texture_new(u32 width, u32 height, TextureWrapping wrapping, TextureFiltering filtering, TextureFormat format) {
    Texture* texture = texture_create(wrapping, filtering);

    GLenum gl_format;
    switch (format) {
    case TEXTURE_FORMAT_R:
        gl_format = GL_RED;
        break;

    case TEXTURE_FORMAT_RG:
        gl_format = GL_RG;
        break;

    case TEXTURE_FORMAT_RGB:
        gl_format = GL_RGB;
        break;

    default:
    case TEXTURE_FORMAT_RGBA:
        gl_format = GL_RGBA;
        break;
    }
    
    glTexImage2D(GL_TEXTURE_2D, 0, gl_format, width, height, 0, gl_format, GL_UNSIGNED_BYTE, NULL);

    return texture;
}

Texture* texture_new_from_image(Image* image, TextureWrapping wrapping, TextureFiltering filtering) {
    Texture* texture = texture_create(wrapping, filtering);

    GLenum gl_format;
    switch (image_get_num_channels(image)) {
    case 1:
        gl_format = GL_RED;
        break;

    case 2:
        gl_format = GL_RG;
        break;

    case 3:
        gl_format = GL_RGB;
        break;

    default:
    case 4:
        gl_format = GL_RGBA;
        break;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, gl_format, image_get_width(image), image_get_height(image), 0, gl_format, GL_UNSIGNED_BYTE, image_get_data(image));
    glGenerateMipmap(GL_TEXTURE_2D);

    return texture;
}

void texture_delete(Texture* texture) {
    glDeleteTextures(1, &(texture->id));
    free(texture);
}

u32 texture_get_id(Texture* texture) {
    return texture->id;
}

void texture_use(Texture* texture) {
    glBindTexture(GL_TEXTURE_2D, texture->id);
}

#endif
