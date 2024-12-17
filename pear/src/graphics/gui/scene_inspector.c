#include <graphics/gui/scene_inspector.h>
#include <graphics/gui/component_inspector.h>
#include <scene/components/transform.h>
#include <scene/components/model.h>
#include <scene/components/camera.h>
#include <scene/components/light.h>
#include <scene/components/script.h>

static scene_t* gui_scene = NULL;

void gui_scene_inspector_set_scene(scene_t* scene) {
    gui_scene = scene;
}

void gui_scene_inspector(struct nk_context* nk_context, void* user_data) {
    if (nk_begin(nk_context, "scene inspector", nk_rect(10, 230, 300, 300), gui_default_window_flags)) {
        if (gui_scene == NULL) {
            nk_layout_row_dynamic(nk_context, 16, 1);
            nk_label(nk_context, "no scene provided!", NK_TEXT_ALIGN_LEFT);
            nk_end(nk_context);
            return;
        }

        array_t* entities = scene_get_entities(gui_scene);
        for (u32 i = 0; i < array_get_length(entities); i++) {
            entity_t* entity = array_get(entities, i);

            nk_layout_row_dynamic(nk_context, 16, 1);
            if (nk_button_label(nk_context, entity_get_name(entity))) {
                gui_component_inspector_set_entity(entity);
            }
        }
    }
    nk_end(nk_context);
}
