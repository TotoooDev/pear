#include <panels/entity_inspector.h>
#include <graphics/editor/editor.h>
#include <scene/components/transform.h>
#include <scene/components/camera.h>
#include <scene/components/light.h>
#include <scene/components/model.h>
#include <scene/components/lua_script.h>
#include <scene/components/skybox.h>
#include <loaders/model.h>
#include <vendor/tinyfiledialogs/tinyfiledialogs.h>
#include <string.h>

#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include <graphics/editor/vendor/cimgui/cimgui.h>

static scene_t* panel_scene = NULL;
static entity_t* panel_entity = NULL;

void panel_entity_inspector_component_combo() {
    const char* items[] = {
        "transform",
        "model",
        "billboard",
        "camera",
        "lua_script",
        "light",
        "skybox"
    };

    static i32 current_item = -1;
    igCombo_Str_arr("component", &current_item, items, 7, 64);
    if (igButton("add component", (ImVec2){ 0.0f, 0.0f }) && current_item > -1) {
        scene_add_component(panel_scene, panel_entity, items[current_item]);
        current_item = -1;
    }
    igSameLine(0.0f, 8.0f);
    if (igButton("remove component", (ImVec2){ 0.0f, 0.0f }) && current_item > -1) {
        scene_remove_component(panel_scene, panel_entity, items[current_item]);
        current_item = -1;
    }
}

void panel_entity_inspector_transform(entity_t* entity) {
    if (igTreeNodeEx_Str("transform", ImGuiTreeNodeFlags_DefaultOpen)) {
        transform_component_t* transform = (transform_component_t*)scene_get_component(panel_scene, entity, "transform");
        igDragFloat3("position", transform->pos, 0.1f, -FLT_MAX, FLT_MAX, "%.3f", ImGuiSliderFlags_None);
        igDragFloat3("rotation", transform->rotation, 0.1f, -FLT_MAX, FLT_MAX, "%.3f", ImGuiSliderFlags_None);
        igDragFloat3("scale", transform->scale, 0.1f, -FLT_MAX, FLT_MAX, "%.3f", ImGuiSliderFlags_None);

        igTreePop();
    }
}

void panel_entity_inspector_camera(entity_t* entity) {
    if (igTreeNodeEx_Str("camera", ImGuiTreeNodeFlags_DefaultOpen)) {
        camera_component_t* camera = (camera_component_t*)scene_get_component(panel_scene, entity, "camera");
        igCheckbox("use", &camera->use);

        igTreePop();
    }
}

void panel_entity_inspector_light(entity_t* entity) {
    if (igTreeNodeEx_Str("light", ImGuiTreeNodeFlags_DefaultOpen)) {
        light_component_t* light = (light_component_t*)scene_get_component(panel_scene, entity, "light");

        i32 current_item = light->light.type;
        const char* light_types[] = {
            "directional",
            "point",
            "spot"
        };

        igCombo_Str_arr("type", &current_item, light_types, 3, 64);
        if (current_item > -1) {
            light->light.type = (light_type_t)current_item;
        }

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

void panel_entity_inspector_model(entity_t* entity) {
    if (igTreeNodeEx_Str("model", ImGuiTreeNodeFlags_DefaultOpen)) {
        model_component_t* model = (model_component_t*)scene_get_component(panel_scene, entity, "model");

        if (model->model == NULL) {
            igText("no model loaded");
        }

        if (igButton("choose model (gltf only)", (ImVec2){ 0.0f, 0.0f })) {
            if (model->model != NULL) {
                model_delete(model->model);
            }

            const char* filters[] = { "*.gltf", "*.glb" };
            char* path = tinyfd_openFileDialog("choose model", "", 2, filters, "gltf files", 0);
            model->model = loader_load_gltf(path);
        }

        igSeparator();

        if (model->model != NULL) {
            igCheckbox("draw", &model->draw);
            igCheckbox("shadow caster", &model->shadow_caster);

            if (igTreeNode_Str("materials")) {
                material_t** materials = model_get_materials(model->model);
                for (u32 i = 0; i < model_get_num_materials(model->model); i++) {
                    if (igTreeNode_Ptr(&materials[i], materials[i]->name)) {
                        igCheckbox("use color", &materials[i]->use_color);
                        igColorEdit3("color", materials[i]->color, ImGuiColorEditFlags_None);

                        igTreePop();
                    }
                }

                igTreePop();
            }
        }

        igTreePop();
    }
}

void panel_entity_inspector_lua_script(entity_t* entity) {
    if (igTreeNodeEx_Str("lua script", ImGuiTreeNodeFlags_DefaultOpen)) {
        lua_script_component_t* script = (lua_script_component_t*)scene_get_component(panel_scene, entity, "lua_script");

        if (script->script == NULL) {
            igText("no script loaded");
        }

        if (igButton("choose lua script", (ImVec2){ 0.0f ,0.0f })) {
            if (script->script != NULL) {
                script_delete(script->script);
            }

            const char* filters[] = { "*.lua" };
            char* path = tinyfd_openFileDialog("choose lua script", "", 1, filters, "lua files", 0);
            script->script = script_new_from_file(path, entity);
        }

        igSeparator();

        if (script->script != NULL) {
            igCheckbox("run", &script->run);
            if (igButton("restart script", (ImVec2){ 0.0f, 0.0f })) {
                script->has_started = false;
            }

            if (script->run) {
                script_on_editor(script->script);
            }
        }

        igTreePop();
    }
}

void panel_entity_inspector_skybox(entity_t* entity) {
    if (igTreeNodeEx_Str("skybox", ImGuiTreeNodeFlags_DefaultOpen)) {
        skybox_component_t* skybox = (skybox_component_t*)scene_get_component(panel_scene, entity, "skybox");

        igCheckbox("draw", &skybox->draw);

        igTreePop();
    }
}

void panel_entity_inspector_set_entity(entity_t* entity) {
    panel_entity = entity;
}

void panel_entity_inspector_set_scene(scene_t* scene) {
    panel_scene = scene;
}

void panel_entity_inspector() {
    if (igBegin("entity inspector", NULL, ImGuiWindowFlags_None)) {
        if (panel_entity == NULL) {
            igEnd();
            return;
        }

        igInputText("name", entity_get_name(panel_entity), ENTITY_NAME_MAX_LENGTH, ImGuiInputTextFlags_EnterReturnsTrue, NULL, NULL);
        igText("id: %d", entity_get_id(panel_entity));

        igSeparator();
        
        panel_entity_inspector_component_combo();

        igSeparator();

        if (scene_has_component(panel_scene, panel_entity, "transform")) {
            panel_entity_inspector_transform(panel_entity);
        }

        if (scene_has_component(panel_scene, panel_entity, "camera")) {
            panel_entity_inspector_camera(panel_entity);
        }

        if (scene_has_component(panel_scene, panel_entity, "light")) {
            panel_entity_inspector_light(panel_entity);
        }

        if (scene_has_component(panel_scene, panel_entity, "model")) {
            panel_entity_inspector_model(panel_entity);
        }

        if (scene_has_component(panel_scene, panel_entity, "lua_script")) {
            panel_entity_inspector_lua_script(panel_entity);
        }

        if (scene_has_component(panel_scene, panel_entity, "skybox")) {
            panel_entity_inspector_skybox(panel_entity);
        }
    }
    igEnd();
}
