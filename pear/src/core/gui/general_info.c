#include <core/gui/general_info.h>
#include <core/app.h>
#include <core/config.h>

#define GUI_NUM_FPS_STORED 64

static f32 gui_fps[GUI_NUM_FPS_STORED];

void gui_general_info(struct nk_context* nk_context, void* user_data) {
    if (nk_begin(nk_context, "general info", nk_rect(100, 50, 250, 250), gui_default_window_flags)) {
        if (nk_tree_push(nk_context, NK_TREE_TAB, "general info", NK_MAXIMIZED)) {
            nk_layout_row_dynamic(nk_context, 16, 1);
            nk_label(nk_context, "platform: " PEAR_PLATFORM_NAME, NK_TEXT_ALIGN_LEFT);
            nk_label(nk_context, "graphics api: " PEAR_RENDERER_API_NAME, NK_TEXT_ALIGN_LEFT);
            nk_labelf(nk_context, NK_TEXT_ALIGN_LEFT, "debug mode: %s", PEAR_IS_DEBUG == true ? "true" : "false");
            nk_tree_pop(nk_context);
        }
        
        if (nk_tree_push(nk_context, NK_TREE_TAB, "performance", NK_MAXIMIZED)) {
            nk_layout_row_dynamic(nk_context, 16, 1);
            nk_labelf(nk_context, NK_TEXT_ALIGN_LEFT, "timestep: %f", app_get_timestep());
            nk_labelf(nk_context, NK_TEXT_ALIGN_LEFT, "fps: %f", 1.0f / app_get_timestep() * 1000.0f);
            nk_layout_row_dynamic(nk_context, 64, 1);

            nk_tree_pop(nk_context);
        }
    }
    nk_end(nk_context);
}
