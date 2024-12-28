#ifdef PEAR_ENABLE_EDITOR

#include <graphics/editor/scene_inspector.h>
#include <graphics/editor/entity_inspector.h>

#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include <graphics/editor/cimgui/cimgui.h>

static scene_t* editor_scene = NULL;

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
        for (u32 i = 0; i < array_get_length(entities); i++) {
            entity_t* entity = (entity_t*)array_get(entities, i);
            
            if (igTreeNodeEx_Str(entity_get_name(entity), ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_FramePadding)) {
                if (igIsItemClicked(ImGuiMouseButton_Left)) {
                    editor_set_entity(entity);
                }

                igTreePop();
            }
        }

        igEnd();
    }
}

#endif
