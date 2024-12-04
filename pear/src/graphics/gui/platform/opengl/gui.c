#ifdef PEAR_PLATFORM_OPENGL

#include <core/app.h>

#include <GL/glew.h>
#include <graphics/platform/opengl/texture.h>
#include <graphics/platform/opengl/window.h>

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_KEYSTATE_BASED_INPUT
#define NK_IMPLEMENTATION
#define NK_GLFW_GL3_IMPLEMENTATION
#include <graphics/vendor/nuklear.h>
#include <graphics/vendor/nuklear_glfw3.h>

#define NK_MAX_VERTEX_BUFFER 512 * 1024
#define NK_MAX_ELEMENT_BUFFER 128 * 1024

static struct nk_glfw gui_glfw;
static struct nk_font_atlas* gui_atlas;

struct nk_context* gui_platform_init() {
    struct nk_context* context = nk_glfw3_init(&gui_glfw, window_get_glfw(app_get_window()), NK_GLFW3_INSTALL_CALLBACKS);
    nk_glfw3_font_stash_begin(&gui_glfw, &gui_atlas);
    nk_glfw3_font_stash_end(&gui_glfw);
    return context;
}

void gui_platform_free() {
    nk_font_atlas_clear(gui_atlas);
}

void gui_platform_clear() {
    nk_glfw3_new_frame(&gui_glfw);
}

void gui_platform_render() {
    nk_glfw3_render(&gui_glfw, NK_ANTI_ALIASING_ON, NK_MAX_VERTEX_BUFFER, NK_MAX_ELEMENT_BUFFER);
}

struct nk_image gui_image_from_texture(texture_t* texture) {
    return nk_image_id(texture_get_id(texture));
}

#endif
