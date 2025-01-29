#ifdef PEAR_ENABLE_EDITOR

#include <panels/scene_inspector.h>
#include <panels/entity_inspector.h>
#include <editor.h>
#include <graphics/editor/editor.h>
#include <core/app.h>
#include <string.h>

void panel_scene_inspector() {
    if (igBegin("scene inspector", NULL, ImGuiWindowFlags_None)) {
        if (app_get_scene() == NULL) {
            igText("no scene provided!");
            igEnd();
            return;
        }

        array_t* entities = scene_get_entities(app_get_scene());
        igText("%d entities", array_get_length(entities));

        if (igButton("add entity", (ImVec2){ 0.0f, 0.0f })) {
            scene_add_entity(app_get_scene(), "new entity");
        }
        igSameLine(0.0f, 8.0f);
        if (igButton("remove entity", (ImVec2){ 0.0f, 0.0f })) {
            scene_remove_entity(app_get_scene(), editor_get_selected_entity());
            editor_set_selected_entity(NULL);
        }

        igSeparator();
        bool entity_selected = false;
        for (u32 i = 0; i < array_get_length(entities); i++) {
            entity_t* entity = (entity_t*)array_get(entities, i);

            bool excluded = false;
            array_t* excluded_entities = editor_get_excluded_entities();
            for (u32 j = 0; j < array_get_length(excluded_entities); j++) {
                if (array_get(excluded_entities, j) == entity) {
                    excluded = true;
                }
            }
            if (excluded) {
                continue;
            }

            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_FramePadding;
            if (entity == editor_get_selected_entity()) {
                flags |= ImGuiTreeNodeFlags_Selected;
            }
            
            bool ret;
            const char* entity_name = entity_get_name(entity);
            if (strlen(entity_name) == 0) {
                ret = igTreeNodeEx_Ptr(entity, flags, "unnamed entity %d", entity_get_id(entity));
            }
            else {
                ret = igTreeNodeEx_Str(entity_get_name(entity), flags);
            }

            if (ret) {
                if (igIsItemClicked(ImGuiMouseButton_Left)) {
                    editor_set_selected_entity(entity);
                    entity_selected = true;
                }
                else if (igIsMouseClicked_Bool(ImGuiMouseButton_Left, false) && igIsItemHovered(ImGuiHoveredFlags_None) && !entity_selected) {
                    editor_set_selected_entity(NULL);
                }

                igTreePop();
            }
        }
    }
    igEnd();
}

#endif
