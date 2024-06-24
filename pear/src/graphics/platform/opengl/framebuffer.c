#ifdef PEAR_PLATFORM_OPENGL

#include <graphics/framebuffer.h>
#include <graphics/platform/opengl/framebuffer.h>
#include <graphics/platform/opengl/texture.h>
#include <core/app.h>
#include <core/log.h>
#include <GL/glew.h>
#include <stdlib.h>

typedef struct Framebuffer {
    u32 id;
    i32 render_buffer;

    bool has_depth;
    Texture** textures;
    u32 num_textures;
} Framebuffer;

Framebuffer* framebuffer_new(u32 width, u32 height, TextureFormat* formats, u32 num_formats, bool has_depth) {
    if (num_formats == 0 && !has_depth) {
        PEAR_ERROR("no framebuffer attachments specified!");
        return NULL;
    }

    if (num_formats >= GL_MAX_COLOR_ATTACHMENTS)
        PEAR_WARN("to many framebuffer formats! %d formats, maximum is %d. disregarding the additionnal formats.", num_formats, GL_MAX_COLOR_ATTACHMENTS);

    Framebuffer* framebuffer = (Framebuffer*)malloc(sizeof(Framebuffer));

    framebuffer->render_buffer = -1;
    framebuffer->has_depth = has_depth;
    framebuffer->num_textures = num_formats;
    framebuffer->textures = (Texture**)malloc(sizeof(Texture*) * framebuffer->num_textures);

    glGenFramebuffers(1, &(framebuffer->id));
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer->id);

    for (u32 i = 0; i < framebuffer->num_textures; i++) {
        framebuffer->textures[i] = texture_new(width, height, TEXTURE_WRAPPING_NONE, TEXTURE_FILTERING_NEAREST, formats[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, texture_get_id(framebuffer->textures[i]), 0);
    }

    if (framebuffer->has_depth) {
        glGenRenderbuffers(1, &(framebuffer->render_buffer));
        glBindRenderbuffer(GL_RENDERBUFFER, framebuffer->render_buffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, framebuffer->render_buffer);
    }

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        PEAR_WARN("framebuffer is not complete!");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return framebuffer;
}

Framebuffer* framebuffer_new_for_screen(TextureFormat* formats, u32 num_formats, bool has_depth) {
    return framebuffer_new(window_get_width(app_get_window()), window_get_height(app_get_window()), formats, num_formats, has_depth);
}

void framebuffer_delete(Framebuffer* framebuffer) {
    for (u32 i = 0; i < framebuffer->num_textures; i++) {
        texture_delete(framebuffer->textures[i]);
    }
    free(framebuffer->textures);

    glDeleteFramebuffers(1, &(framebuffer->id));
    free(framebuffer);
}

Texture* framebuffer_get_texture(Framebuffer* framebuffer, u32 index) {
    if (index >= framebuffer->num_textures) {
        PEAR_ERROR("trying to access out of bounds framebuffer texture %d! only %d textures available.", index, framebuffer->num_textures);
        return NULL;
    }

    return framebuffer->textures[index];
}

Texture* framebuffer_get_depth_texture(Framebuffer* framebuffer) {
    if (!framebuffer->has_depth) {
        PEAR_WARN("trying to access inexistant depth texture!");
        return NULL;
    }
    
    return NULL;
}

u32 framebuffer_get_id(Framebuffer* framebuffer) {
    return framebuffer->id;
}

void framebuffer_use(Framebuffer* framebuffer) {
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer->id);
}

#endif
