#include <graphics/gui/scene_inspector.h>
#include <scene/components/transform.h>

static scene_t* gui_scene = NULL;

void gui_scene_inspector_set_scene(scene_t* scene) {
    gui_scene = scene;
}

void gui_transform(struct nk_context* nk_context, entity_t* entity) {
    if (nk_tree_push(nk_context, NK_TREE_NODE, "transform", NK_MAXIMIZED)) {
        transform_component_t* transform = (transform_component_t*)entity_get_component(entity, ENTITY_COMPONENT_TRANSFORM);

        nk_layout_row_dynamic(nk_context, 16, 4);

        nk_label(nk_context, "position:", NK_TEXT_ALIGN_LEFT);
        nk_property_float(nk_context, "#x", -FLT_MAX, &(transform->pos[0]), FLT_MAX, 1.0f, 0.01f);
        nk_property_float(nk_context, "#y", -FLT_MAX, &(transform->pos[1]), FLT_MAX, 1.0f, 0.01f);
        nk_property_float(nk_context, "#z", -FLT_MAX, &(transform->pos[2]), FLT_MAX, 1.0f, 0.01f);

        nk_label(nk_context, "rotation:", NK_TEXT_ALIGN_LEFT);
        nk_property_float(nk_context, "#x", -FLT_MAX, &(transform->rotation[0]), FLT_MAX, 1.0f, 0.01f);
        nk_property_float(nk_context, "#y", -FLT_MAX, &(transform->rotation[1]), FLT_MAX, 1.0f, 0.01f);
        nk_property_float(nk_context, "#z", -FLT_MAX, &(transform->rotation[2]), FLT_MAX, 1.0f, 0.01f);

        nk_label(nk_context, "scale:", NK_TEXT_ALIGN_LEFT);
        nk_property_float(nk_context, "#x", -FLT_MAX, &(transform->scale[0]), FLT_MAX, 1.0f, 0.01f);
        nk_property_float(nk_context, "#y", -FLT_MAX, &(transform->scale[1]), FLT_MAX, 1.0f, 0.01f);
        nk_property_float(nk_context, "#z", -FLT_MAX, &(transform->scale[2]), FLT_MAX, 1.0f, 0.01f);
        
        nk_tree_pop(nk_context);
    }
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

            if (nk_tree_push_id(nk_context, NK_TREE_TAB, "entity", NK_MAXIMIZED, i)) {
                if (entity_has_component(entity, ENTITY_COMPONENT_TRANSFORM)) {
                    gui_transform(nk_context, entity);
                }

                nk_tree_pop(nk_context);
            }
        }
    }
    nk_end(nk_context);
}
