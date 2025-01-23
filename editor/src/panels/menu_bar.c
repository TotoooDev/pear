#ifdef PEAR_ENABLE_EDITOR

#include <panels/menu_bar.h>
#include <panels/scene_inspector.h>
#include <panels/entity_inspector.h>
#include <graphics/editor/editor.h>
#include <event/event_dispatcher.h>
#include <loaders/scene.h>
#include <vendor/tinyfiledialogs/tinyfiledialogs.h>
#include <core/app.h>

array_t* panel_excluded_entities = NULL;

void panel_menu_bar_init() {
    panel_excluded_entities = array_new(5);
}

void panel_menu_bar_free() {
    array_delete(panel_excluded_entities);
}

void panel_menu_bar_exclude_entity(entity_t* entity) {
    array_add(panel_excluded_entities, entity);
}

void panel_menu_bar_clear_excluded_entities() {
    array_clear(panel_excluded_entities);
}

void panel_menu_bar() {
    if (igBeginMainMenuBar()) {
        if (igBeginMenu("file", true)) {
            if (igMenuItem_Bool("save scene", "", false, true)) {
                loader_write_scene(app_get_scene(), "scene_write.pear", panel_excluded_entities);
            }
            
            igSeparator();

            if (igMenuItem_Bool("load scene...", "", false, true)) {
                const char* filters[] = { "*.pear" };
                char* path = tinyfd_openFileDialog("choose scene", "", 1, filters, "pear scene files", 0);
                if (path != NULL) {
                    scene_t* scene = loader_load_scene(path);
                    app_set_scene(scene);
                    panel_scene_inspector_set_scene(scene);
                    panel_entity_inspector_set_scene(scene);
                    panel_entity_inspector_set_entity(NULL);
                }
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
