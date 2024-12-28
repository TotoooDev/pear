#ifdef PEAR_ENABLE_EDITOR

#include <graphics/editor/entity_inspector.h>
#include <scene/components/transform.h>
#include <scene/components/camera.h>
#include <scene/components/light.h>
#include <scene/components/model.h>
#include <scene/components/script.h>
#include <scene/components/skybox.h>

#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include <graphics/editor/cimgui/cimgui.h>

static entity_t* editor_entity = NULL;

void editor_transform(entity_t* entity) {
    if (igTreeNodeEx_Str("transform", ImGuiTreeNodeFlags_DefaultOpen)) {
        transform_component_t* transform = (transform_component_t*)entity_get_component(entity, ENTITY_COMPONENT_TRANSFORM);
        igDragFloat3("position", transform->pos, 0.1f, -FLT_MAX, FLT_MAX, "%.3f", ImGuiSliderFlags_None);
        igDragFloat3("rotation", transform->rotation, 0.1f, -FLT_MAX, FLT_MAX, "%.3f", ImGuiSliderFlags_None);
        igDragFloat3("scale", transform->scale, 0.1f, -FLT_MAX, FLT_MAX, "%.3f", ImGuiSliderFlags_None);

        igTreePop();
    }
}

void editor_camera(entity_t* entity) {
    if (igTreeNodeEx_Str("camera", ImGuiTreeNodeFlags_DefaultOpen)) {
        camera_component_t* camera = (camera_component_t*)entity_get_component(entity, ENTITY_COMPONENT_CAMERA);
        igCheckbox("use", &camera->use);

        igTreePop();
    }
}

void editor_light(entity_t* entity) {
    if (igTreeNodeEx_Str("light", ImGuiTreeNodeFlags_DefaultOpen)) {
        light_component_t* light = (light_component_t*)entity_get_component(entity, ENTITY_COMPONENT_LIGHT);

        igCombo_Str("type", (i32*)(&light->light.type), "directional\0point\0spot\0", 64);
        igCheckbox("cast", &light->cast);
        igCheckbox("shadow caster", &light->shadow_caster);

        igColorEdit3("ambient", light->light.ambient, ImGuiColorEditFlags_None);
        igColorEdit3("diffuse", light->light.diffuse, ImGuiColorEditFlags_None);
        igColorEdit3("specular", light->light.specular, ImGuiColorEditFlags_None);

        if (light->light.type == LIGHT_TYPE_POINT || light->light.type == LIGHT_TYPE_SPOT) {
            igDragFloat("constant", &light->light.constant, 0.1f, 0.0f, FLT_MAX, "%.3f", ImGuiSliderFlags_None);
            igDragFloat("linear", &light->light.linear, 0.1f, 0.0f, FLT_MAX, "%.3f", ImGuiSliderFlags_None);
            igDragFloat("quadratic", &light->light.quadratic, 0.1f, 0.0f, FLT_MAX, "%.3f", ImGuiSliderFlags_None);
        }

        if (light->light.type == LIGHT_TYPE_SPOT) {
            igDragFloat("cutoff", &light->light.cutoff, 0.1f, 0.0f, FLT_MAX, "%.3f", ImGuiSliderFlags_None);
            igDragFloat("outer_cutoff", &light->light.outer_cutoff, 0.1f, 0.0f, FLT_MAX, "%.3f", ImGuiSliderFlags_None);
        }

        igTreePop();
    }
}

void editor_model(entity_t* entity) {
    if (igTreeNodeEx_Str("model", ImGuiTreeNodeFlags_DefaultOpen)) {
        model_component_t* model = (model_component_t*)entity_get_component(entity, ENTITY_COMPONENT_MODEL);

        igCheckbox("draw", &model->draw);
        igCheckbox("shadow caster", &model->shadow_caster);

        igTreePop();
    }
}

void editor_script(entity_t* entity) {
    if (igTreeNodeEx_Str("script", ImGuiTreeNodeFlags_DefaultOpen)) {
        script_component_t* script = (script_component_t*)entity_get_component(entity, ENTITY_COMPONENT_SCRIPT);

        igCheckbox("run", &script->run);
        if (igButton("restart script", (ImVec2){ 0.0f, 0.0f })) {
            script->has_started = false;
        }

        igTreePop();
    }
}

void editor_skybox(entity_t* entity) {
    if (igTreeNodeEx_Str("skybox", ImGuiTreeNodeFlags_DefaultOpen)) {
        skybox_component_t* skybox = (skybox_component_t*)entity_get_component(entity, ENTITY_COMPONENT_SKYBOX);

        igCheckbox("draw", &skybox->draw);

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
        igText(entity_get_name(editor_entity));

        if (entity_has_component(editor_entity, ENTITY_COMPONENT_TRANSFORM)) {
            editor_transform(editor_entity);
        }

        if (entity_has_component(editor_entity, ENTITY_COMPONENT_CAMERA)) {
            editor_camera(editor_entity);
        }

        if (entity_has_component(editor_entity, ENTITY_COMPONENT_LIGHT)) {
            editor_light(editor_entity);
        }

        if (entity_has_component(editor_entity, ENTITY_COMPONENT_MODEL)) {
            editor_model(editor_entity);
        }

        if (entity_has_component(editor_entity, ENTITY_COMPONENT_SCRIPT)) {
            editor_script(editor_entity);
        }

        if (entity_has_component(editor_entity, ENTITY_COMPONENT_SKYBOX)) {
            editor_skybox(editor_entity);
        }

        igEnd();
    }
}

#endif
