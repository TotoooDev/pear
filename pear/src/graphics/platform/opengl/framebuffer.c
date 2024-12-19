#ifdef PEAR_PLATFORM_OPENGL

#include <graphics/framebuffer.h>
#include <graphics/platform/opengl/framebuffer.h>
#include <graphics/platform/opengl/texture.h>
#include <graphics/texture.h>
#include <core/log.h>
#include <core/alloc.h>
#include <GL/glew.h>

typedef struct framebuffer_t {
    u32 num_textures;
    u32 id;
} framebuffer_t;

framebuffer_t* framebuffer_new() {
    framebuffer_t* framebuffer = (framebuffer_t*)PEAR_MALLOC(sizeof(framebuffer_t));
    
    framebuffer->num_textures = 0;

    glGenFramebuffers(1, &framebuffer->id);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer->id);
    
    return framebuffer;
}

void framebuffer_delete(framebuffer_t* framebuffer) {
    glDeleteFramebuffers(1, &framebuffer->id);
    PEAR_FREE(framebuffer);
}

void framebuffer_set_depth_only(framebuffer_t* framebuffer) {
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer->id);
    glReadBuffer(GL_NONE);
    glDrawBuffer(GL_NONE);
}

void framebuffer_add_texture(framebuffer_t* framebuffer, texture_t* texture) {
    if (texture_get_format(texture) == TEXTURE_FORMAT_DEPTH) {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture_get_id(texture), 0);
    }
    else if (texture_get_format(texture) == TEXTURE_FORMAT_STENCIL) {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, texture_get_id(texture), 0);
    }
    else {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + framebuffer->num_textures, GL_TEXTURE_2D, texture_get_id(texture), 0);
    }
    
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        PEAR_ERROR("framebuffer is not complete!");
    }

    framebuffer->num_textures++;
}

void framebuffer_use(framebuffer_t* framebuffer) {
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer->id);
}

void framebuffer_use_default() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

#endif
