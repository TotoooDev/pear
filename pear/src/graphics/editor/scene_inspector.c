#ifdef PEAR_ENABLE_EDITOR

#include <graphics/editor/scene_inspector.h>
#include <graphics/editor/entity_inspector.h>

#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include <graphics/editor/vendor/cimgui/cimgui.h>

static scene_t* editor_scene = NULL;
static entity_t* editor_selected_entity = NULL;

void editor_set_scene(scene_t* scene) {
    editor_scene = scene;
}

void editor_scene_inspector(bool* show) {
    if (igBegin("scene inspector", show, ImGuiWindowFlags_None)) {
        if (editor_scene == NULL) {
            igText("no scene provided!");
            igEnd();
            return;
        }

        array_t* entities = scene_get_entities(editor_scene);
        igText("%d entities", array_get_length(entities));

        if (igButton("add entity", (ImVec2){ 0.0f, 0.0f })) {
            scene_add_entity(editor_scene, "new entity", ENTITY_COMPONENT_END);
        }
        igSameLine(0.0f, 8.0f);
        if (igButton("remove entity", (ImVec2){ 0.0f, 0.0f })) {
            scene_remove_entity(editor_scene, editor_selected_entity);
            editor_set_entity(NULL);
            editor_selected_entity = NULL;
        }

        igSeparator();
        for (u32 i = 0; i < array_get_length(entities); i++) {
            entity_t* entity = (entity_t*)array_get(entities, i);

            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_FramePadding;
            if (entity == editor_selected_entity) {
                flags |= ImGuiTreeNodeFlags_Selected;
            }
            
            if (igTreeNodeEx_Str(entity_get_name(entity), flags)) {
                if (igIsItemClicked(ImGuiMouseButton_Left)) {
                    editor_set_entity(entity);
                    editor_selected_entity = entity;
                }

                igTreePop();
            }
        }
    }
    igEnd();
}

#endif
