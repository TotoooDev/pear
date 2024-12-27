#ifdef PEAR_ENABLE_EDITOR

#include <graphics/editor/entity_inspector.h>
#include <scene/components/transform.h>
#include <scene/components/camera.h>
#include <scene/components/light.h>

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

void editor_light(entity_t* entity) {
    if (igTreeNode_Str("light")) {
        light_component_t* light = (light_component_t*)entity_get_component(entity, ENTITY_COMPONENT_LIGHT);

        igCombo_Str("type", (i32*)(&light->light.type), "directional\0point\0spot\0", 64);
        igCheckbox("cast", &light->cast);
        igCheckbox("shadow caster", &light->shadow_caster);

        igColorEdit3("ambient", light->light.ambient, ImGuiColorEditFlags_None);
        igColorEdit3("diffuse", light->light.diffuse, ImGuiColorEditFlags_None);
        igColorEdit3("specular", light->light.specular, ImGuiColorEditFlags_None);

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

        if (entity_has_component(editor_entity, ENTITY_COMPONENT_LIGHT)) {
            editor_light(editor_entity);
        }

        igEnd();
    }
}

#endif
