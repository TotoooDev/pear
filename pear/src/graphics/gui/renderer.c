#include <graphics/gui/renderer.h>
#include <graphics/renderer.h>
#include <core/app.h>

static texture_t* gui_shadow_map = NULL;
static bool gui_is_wireframe = false;

void gui_renderer_set_shadow_map(texture_t* shadow_map) {
    gui_shadow_map = shadow_map;
}

void gui_renderer(struct nk_context* nk_context, void* user_data) {
    if (nk_begin(nk_context, "renderer", nk_rect(400, 400, 200, 200), gui_default_window_flags)) {
        if (nk_tree_push(nk_context, NK_TREE_TAB, "general info", NK_MINIMIZED)) {
            nk_layout_row_dynamic(nk_context, 16, 1);

            #ifdef PEAR_PLATFORM_OPENGL
                nk_label(nk_context, "graphics api: opengl", NK_TEXT_ALIGN_LEFT);
            #else
                nk_label(nk_context, "graphics api: unknown", NK_TEXT_ALIGN_LEFT);
            #endif

            if (nk_checkbox_label(nk_context, "wireframe", (nk_bool*)(&gui_is_wireframe))) {
                renderer_set_wireframe(app_get_renderer(), gui_is_wireframe);
            }

            nk_tree_pop(nk_context);
        }

        if (nk_tree_push(nk_context, NK_TREE_TAB, "shadow map", NK_MINIMIZED)) {
            nk_layout_row_static(nk_context, 256, 256, 1);
            nk_image(nk_context, gui_image_from_texture(gui_shadow_map));
            nk_layout_row_dynamic(nk_context, 16, 1);
            nk_tree_pop(nk_context);
        }
        nk_layout_row_dynamic(nk_context, 16, 1);
        
    }
    nk_end(nk_context);
}
