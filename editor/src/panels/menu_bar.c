#ifdef PEAR_ENABLE_EDITOR

#include <panels/menu_bar.h>
#include <panels/scene_inspector.h>
#include <panels/entity_inspector.h>
#include <editor.h>
#include <graphics/editor/editor.h>
#include <event/event_dispatcher.h>
#include <event/keyboard.h>
#include <loaders/scene.h>
#include <vendor/tinyfiledialogs/tinyfiledialogs.h>
#include <core/app.h>

static bool panel_ctrl_pressed = false;
static bool panel_shift_pressed = false;

void panel_menu_bar_new() {
    app_set_scene(scene_new());
    editor_set_scene_path("");
}

void panel_menu_bar_save() {
    loader_write_scene(app_get_scene(), editor_get_scene_path(), editor_get_excluded_entities());
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
        editor_set_scene_path(path);
    }
}

void panel_menu_bar_on_event(event_type_t type, void* e, void* user_pointer) {
    if (type == EVENT_TYPE_KEY_DOWN) {
        key_down_event_t* event = (key_down_event_t*)e;
        if (event->key == PEAR_KEY_LEFT_CONTROL || event->key == PEAR_KEY_RIGHT_CONTROL) {
            panel_ctrl_pressed = true;
        }
        if (event->key == PEAR_KEY_LEFT_SHIFT || event->key == PEAR_KEY_RIGHT_SHIFT) {
            panel_shift_pressed = true;
        }

        if (panel_ctrl_pressed) {
            if (event->key == PEAR_KEY_N) {
                panel_menu_bar_new();
            }
            if (event->key == PEAR_KEY_S) {
                panel_menu_bar_save();
            }
            if (event->key == PEAR_KEY_O) {
                panel_menu_bar_open();
            }

            if (panel_shift_pressed) {
                if (event->key == PEAR_KEY_S) {
                    panel_menu_bar_save_as();
                }
            }
        }
    }

    if (type == EVENT_TYPE_KEY_UP) {
        key_up_event_t* event = (key_up_event_t*)e;
        if (event->key == PEAR_KEY_LEFT_CONTROL || event->key == PEAR_KEY_RIGHT_CONTROL) {
            panel_ctrl_pressed = false;
        }
        if (event->key == PEAR_KEY_LEFT_SHIFT || event->key == PEAR_KEY_RIGHT_SHIFT) {
            panel_shift_pressed = false;
        }
    }
}

void panel_menu_bar() {
    if (igBeginMainMenuBar()) {
        if (igBeginMenu("file", true)) {
            if (igMenuItem_Bool("new scene", "ctrl+n", false, true)) {
                panel_menu_bar_new();
            }

            igSeparator();

            if (igMenuItem_Bool("save scene", "ctrl+s", false, true)) {
                if (editor_get_scene_path()[0] == '\0') {
                    panel_menu_bar_save_as();
                }
                else {
                    panel_menu_bar_save();
                }
            }
            if (igMenuItem_Bool("save scene as...", "ctrl+shift+s", false, true)) {
                panel_menu_bar_save_as();
            }
            
            igSeparator();

            if (igMenuItem_Bool("open scene...", "ctrl+o", false, true)) {
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
