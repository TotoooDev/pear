#ifdef PEAR_ENABLE_EDITOR

#include <panels/scene_inspector.h>
#include <panels/entity_inspector.h>
#include <graphics/editor/editor.h>

static scene_t* panel_scene = NULL;
static entity_t* panel_selected_entity = NULL;

void panel_scene_inspector_set_scene(scene_t* scene) {
    panel_scene = scene;
}

void panel_scene_inspector() {
    if (igBegin("scene inspector", NULL, ImGuiWindowFlags_None)) {
        if (panel_scene == NULL) {
            igText("no scene provided!");
            igEnd();
            return;
        }

        array_t* entities = scene_get_entities(panel_scene);
        igText("%d entities", array_get_length(entities));

        if (igButton("add entity", (ImVec2){ 0.0f, 0.0f })) {
            scene_add_entity(panel_scene, "new entity");
        }
        igSameLine(0.0f, 8.0f);
        if (igButton("remove entity", (ImVec2){ 0.0f, 0.0f })) {
            scene_remove_entity(panel_scene, panel_selected_entity);
            panel_entity_inspector_set_entity(NULL);
            panel_selected_entity = NULL;
        }

        igSeparator();
        for (u32 i = 0; i < array_get_length(entities); i++) {
            entity_t* entity = (entity_t*)array_get(entities, i);

            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_FramePadding;
            if (entity == panel_selected_entity) {
                flags |= ImGuiTreeNodeFlags_Selected;
            }
            
            if (igTreeNodeEx_Str(entity_get_name(entity), flags)) {
                if (igIsItemClicked(ImGuiMouseButton_Left)) {
                    panel_entity_inspector_set_entity(entity);
                    panel_selected_entity = entity;
                }

                igTreePop();
            }
        }
    }
    igEnd();
}

#endif
