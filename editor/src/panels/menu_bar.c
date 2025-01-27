#ifdef PEAR_ENABLE_EDITOR

#include <panels/menu_bar.h>
#include <panels/scene_inspector.h>
#include <panels/entity_inspector.h>
#include <editor.h>
#include <graphics/editor/editor.h>
#include <event/event_dispatcher.h>
#include <loaders/scene.h>
#include <vendor/tinyfiledialogs/tinyfiledialogs.h>
#include <core/app.h>

void panel_menu_bar_new() {
    app_set_scene(scene_new());
    editor_set_scene_path("");
}

void panel_menu_bar_save_as() {
    const char* filters[] = { "*.pear" };
    char* path = tinyfd_saveFileDialog("save scene as", ".", 1, filters, "pear scene file");
    if (path == NULL) {
        return;
    }

    loader_write_scene(app_get_scene(), path, editor_get_excluded_entities());
    editor_set_scene_path(path);
}

void panel_menu_bar_open() {
    const char* filters[] = { "*.pear" };
    char* path = tinyfd_openFileDialog("choose scene", "", 1, filters, "pear scene files", 0);
    if (path != NULL) {
        scene_t* scene = loader_load_scene(path);
        app_set_scene(scene);
    }

    editor_set_scene_path(path);
}

void panel_menu_bar() {
    if (igBeginMainMenuBar()) {
        if (igBeginMenu("file", true)) {
            if (igMenuItem_Bool("new scene", "", false, true)) {
                panel_menu_bar_new();
            }

            igSeparator();

            if (igMenuItem_Bool("save scene", "", false, true)) {
                if (editor_get_scene_path()[0] == '\0') {
                    panel_menu_bar_save_as();
                }
                else {
                    loader_write_scene(app_get_scene(), editor_get_scene_path(), editor_get_excluded_entities());
                }
            }
            if (igMenuItem_Bool("save scene as...", "", false, true)) {
                panel_menu_bar_save_as();
            }
            
            igSeparator();

            if (igMenuItem_Bool("open scene...", "", false, true)) {
                panel_menu_bar_open();
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
