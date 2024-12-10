#include <graphics/gui/info.h>
#include <core/app.h>

void gui_info(struct nk_context* nk_context, void* user_data) {
    if (nk_begin(nk_context, "general info", nk_rect(10, 10, 300, 200), gui_default_window_flags)) {
        nk_layout_row_dynamic(nk_context, 16, 1);

        nk_label(nk_context, app_get_version_string(), NK_TEXT_ALIGN_LEFT);

        #ifdef PEAR_DEBUG
            nk_label(nk_context, "debug mode activated!", NK_TEXT_ALIGN_LEFT);
        #else
            nk_label(nk_context, "debug mode disabled", NK_TEXT_ALIGN_LEFT);
        #endif

        #ifdef PEAR_PLATFORM_OPENGL
            nk_label(nk_context, "graphics api: opengl", NK_TEXT_ALIGN_LEFT);
        #else
            nk_label(nk_context, "graphics api: unknown", NK_TEXT_ALIGN_LEFT);
        #endif

        nk_value_float(nk_context, "timestep", app_get_timestep());
        nk_value_float(nk_context, "fps", 1.0f / app_get_timestep() * 1000.0f);
    }
    nk_end(nk_context);
}
