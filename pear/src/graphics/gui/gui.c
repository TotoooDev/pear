#include <graphics/gui/gui.h>
#include <graphics/gui/platform/opengl/gui.h>
#include <core/types.h>
#include <core/log.h>

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_KEYSTATE_BASED_INPUT
#include <graphics/vendor/nuklear.h>

#define GUI_NUM_MAX_FUNCTIONS 256

static struct nk_context* gui_context;

static gui_function_t gui_functions[GUI_NUM_MAX_FUNCTIONS];
static void* gui_user_datas[GUI_NUM_MAX_FUNCTIONS];
static u32 gui_num_functions = 0;

void gui_init() {
    gui_context = gui_platform_init();
}

void gui_free() {
    gui_platform_free();
    nk_free(gui_context);
}

void gui_add(gui_function_t function, void* user_data) {
    if (gui_num_functions >= GUI_NUM_MAX_FUNCTIONS) {
        PEAR_ERROR("unable to add gui! the gui array is full (%d functions max).", GUI_NUM_MAX_FUNCTIONS);
        return;
    }

    if (function == NULL) {
        PEAR_WARN("adding a gui with a null function pointer!");
    }

    gui_functions[gui_num_functions] = function;
    gui_user_datas[gui_num_functions] = user_data;

    gui_num_functions++;
}

void gui_clear() {
    gui_platform_clear();
}

void gui_render() {
    for (u32 i = 0; i < gui_num_functions; i++) {
        gui_function_t function = gui_functions[i];
        void* user_data = gui_user_datas[i];
        function(gui_context, user_data);
    }

    gui_platform_render();
}

struct nk_colorf gui_vec4_to_nk_color(vec4 color) {
    struct nk_colorf nk_color = {
        .r = color[0],
        .g = color[1],
        .b = color[2],
        .a = color[3]
    };
    return nk_color;
}

void gui_nk_color_to_vec4(struct nk_colorf nk_color, vec4 color) {
    color[0] = nk_color.r;
    color[1] = nk_color.g;
    color[2] = nk_color.b;
    color[3] = nk_color.a;
}

struct nk_colorf gui_vec3_to_nk_color(vec3 color) {
    struct nk_colorf nk_color = {
        .r = color[0],
        .g = color[1],
        .b = color[2],
        .a = 1.0f
    };
    return nk_color;
}

void gui_nk_color_to_vec3(struct nk_colorf nk_color, vec3 color) {
    color[0] = nk_color.r;
    color[1] = nk_color.g;
    color[2] = nk_color.b;
}
