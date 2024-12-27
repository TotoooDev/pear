#ifdef PEAR_ENABLE_EDITOR

#include <graphics/editor/entity_inspector.h>
#include <scene/components/transform.h>
#include <scene/components/camera.h>

#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include <graphics/editor/cimgui/cimgui.h>

static entity_t* editor_entity = NULL;

void editor_transform(entity_t* entity) {
    if (igTreeNode_Str("transform")) {
        transform_component_t* transform = (transform_component_t*)entity_get_component(entity, ENTITY_COMPONENT_TRANSFORM);
        igDragFloat3("position", transform->pos, 0.1f, -FLT_MAX, FLT_MAX, "%.3f", ImGuiSliderFlags_None);
        igDragFloat3("rotation", transform->rotation, 0.1f, -FLT_MAX, FLT_MAX, "%.3f", ImGuiSliderFlags_None);
        igDragFloat3("scale", transform->scale, 0.1f, -FLT_MAX, FLT_MAX, "%.3f", ImGuiSliderFlags_None);

        igTreePop();
    }
}

void editor_camera(entity_t* entity) {
    if (igTreeNode_Str("camera")) {
        camera_component_t* camera = (camera_component_t*)entity_get_component(entity, ENTITY_COMPONENT_CAMERA);
        igCheckbox("use", &camera->use);

        igTreePop();
    }
}

void editor_set_entity(entity_t* entity) {
    editor_entity = entity;
}

void editor_entity_inspector(bool* show) {
    if (editor_entity == NULL) {
        return;
    }

    if (igBegin("entity inspector", show, ImGuiWindowFlags_None)) {
        if (entity_has_component(editor_entity, ENTITY_COMPONENT_TRANSFORM)) {
            editor_transform(editor_entity);
        }

        if (entity_has_component(editor_entity, ENTITY_COMPONENT_CAMERA)) {
            editor_camera(editor_entity);
        }

        igEnd();
    }
}

#endif
