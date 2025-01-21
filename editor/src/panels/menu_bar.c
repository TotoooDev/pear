#ifdef PEAR_ENABLE_EDITOR

#include <panels/menu_bar.h>
#include <editor.h>
#include <graphics/editor/editor.h>
#include <event/event_dispatcher.h>
#include <loaders/scene.h>
#include <vendor/tinyfiledialogs/tinyfiledialogs.h>
#include <core/app.h>

void panel_menu_bar() {
    if (igBeginMainMenuBar()) {
        if (igBeginMenu("file", true)) {
            if (igMenuItem_Bool("load scene...", "", false, true)) {
                const char* filters[] = { "*.pear" };
                char* path = tinyfd_openFileDialog("choose scene", "", 1, filters, "pear scene files", 0);
                editor_set_scene(loader_load_scene(path));
            }

            igSeparator();

            if (igMenuItem_Bool("quit", "alt+f4", false, true)) {
                event_send(EVENT_TYPE_QUIT, NULL);
            }
            igEndMenu();
        }

        if (igBeginMenu("view", true)) {
            if (igMenuItem_Bool("disable imgui", "", false, true)) {
                app_disable_editor();
            }

            igEndMenu();
        }

        igEndMainMenuBar();
    }
}

#endif
