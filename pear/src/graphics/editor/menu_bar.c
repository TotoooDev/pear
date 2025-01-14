#ifdef PEAR_ENABLE_EDITOR

#include <graphics/editor/menu_bar.h>
#include <graphics/editor/editor.h>
#include <event/event_dispatcher.h>
#include <core/app.h>

#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include <graphics/editor/vendor/cimgui/cimgui.h>

void editor_menu_bar(bool* show) {
    if (igBeginMainMenuBar()) {
        if (igBeginMenu("file", true)) {
            if (igMenuItem_Bool("quit", "alt+f4", false, true)) {
                event_send(EVENT_TYPE_QUIT, NULL);
            }
            igEndMenu();
        }

        if (igBeginMenu("view", true)) {
            static bool general_info_selected = true;
            if (igMenuItem_BoolPtr("general info", "", &general_info_selected, true)) {
                editor_enable_general_info(general_info_selected);
            }

            static bool scene_inspector_selected = true;
            if (igMenuItem_BoolPtr("scene inspector", "", &scene_inspector_selected, true)) {
                editor_enable_scene_inspector(scene_inspector_selected);
            }

            static bool entity_inspector_selected = true;
            if (igMenuItem_BoolPtr("entity inspector", "", &entity_inspector_selected, true)) {
                editor_enable_entity_inspector(entity_inspector_selected);
            }

            static bool renderer_inspector_selected = true;
            if (igMenuItem_BoolPtr("renderer_inspector", "", &renderer_inspector_selected, true)) {
                editor_enable_renderer_inspector(renderer_inspector_selected);
            }

            igSeparator();

            static bool demo_selected = false;
            if (igMenuItem_BoolPtr("imgui demo", "", &demo_selected, true)) {
                editor_enable_demo(demo_selected);
            }

            static bool menu_bar_selected = true;
            if (igMenuItem_BoolPtr("menu bar", "", &menu_bar_selected, true)) {
                editor_enable_menu_bar(menu_bar_selected);
            }

            igSeparator();

            if (igMenuItem_Bool("disable imgui", "", false, true)) {
                app_disable_editor();
            }

            igEndMenu();
        }

        igEndMainMenuBar();
    }
}

#endif
