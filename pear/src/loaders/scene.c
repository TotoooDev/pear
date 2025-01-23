#include <loaders/scene.h>
#include <loaders/model.h>
#include <loaders/skybox.h>
#include <loaders/vendor/cJSON.h>
#include <scene/components/transform.h>
#include <scene/components/model.h>
#include <scene/components/camera.h>
#include <scene/components/lua_script.h>
#include <scene/components/light.h>
#include <scene/components/skybox.h>
#include <util/filesystem.h>
#include <core/log.h>
#include <core/alloc.h>

void loader_parse_vec3(cJSON* json, vec3 dest) {
    dest[0] = cJSON_GetNumberValue(cJSON_GetObjectItem(json, "x"));
    dest[1] = cJSON_GetNumberValue(cJSON_GetObjectItem(json, "y"));
    dest[2] = cJSON_GetNumberValue(cJSON_GetObjectItem(json, "z"));
}

void loader_write_vec3(cJSON* json, vec3 vec) {
    cJSON_AddNumberToObject(json, "x", vec[0]);
    cJSON_AddNumberToObject(json, "y", vec[1]);
    cJSON_AddNumberToObject(json, "z", vec[2]);
}

scene_t* loader_load_scene(const char* filename) {
    char* json_data = filesystem_read_file(filename);
    cJSON* json = cJSON_Parse(json_data);
    PEAR_FREE(json_data);

    if (json == NULL) {
        const char* error = cJSON_GetErrorPtr();
        PEAR_ERROR("failed to parse scene file %s! %s", filename, error);
        return NULL;
    }

    scene_t* scene = scene_new();

    cJSON* json_scene = cJSON_GetObjectItem(json, "scene");
    cJSON* json_entities = cJSON_GetObjectItem(json_scene, "entities");
    cJSON* json_entity;
    cJSON_ArrayForEach(json_entity, json_entities) {
        char* name = cJSON_GetStringValue(cJSON_GetObjectItem(json_entity, "name"));
        entity_t* entity = scene_add_entity(scene, name);

        cJSON* json_components = cJSON_GetObjectItem(json_entity, "components");

        if (cJSON_HasObjectItem(json_components, "transform")) {
            transform_component_t* transform = scene_add_component(scene, entity, "transform");

            cJSON* json_transform = cJSON_GetObjectItem(json_components, "transform");
            cJSON* json_pos = cJSON_GetObjectItem(json_transform, "pos");
            cJSON* json_rotation = cJSON_GetObjectItem(json_transform, "rotation");
            cJSON* json_scale = cJSON_GetObjectItem(json_transform, "scale");

            loader_parse_vec3(json_pos, transform->pos);
            loader_parse_vec3(json_rotation, transform->rotation);
            loader_parse_vec3(json_scale, transform->scale);
        }

        if (cJSON_HasObjectItem(json_components, "model")) {
            model_component_t* model = scene_add_component(scene, entity, "model");

            cJSON* json_model = cJSON_GetObjectItem(json_components, "model");
            model->draw = cJSON_IsTrue(cJSON_GetObjectItem(json_model, "draw"));
            model->shadow_caster = cJSON_IsTrue(cJSON_GetObjectItem(json_model, "shadow_caster"));

            char* model_path = cJSON_GetStringValue(cJSON_GetObjectItem(json_model, "model_path"));
            model->model = loader_load_gltf(model_path);
        }

        if (cJSON_HasObjectItem(json_components, "camera")) {
            camera_component_t* camera = scene_add_component(scene, entity, "camera");

            cJSON* json_cam = cJSON_GetObjectItem(json_components, "camera");
            camera->use = cJSON_IsTrue(cJSON_GetObjectItem(json_cam, "use"));
        }

        if (cJSON_HasObjectItem(json_components, "lua_script")) {
            lua_script_component_t* script = scene_add_component(scene, entity, "lua_script");

            cJSON* json_script = cJSON_GetObjectItem(json_components, "lua_script");
            script->run = cJSON_IsTrue(cJSON_GetObjectItem(json_script, "run"));

            char* script_path = cJSON_GetStringValue(cJSON_GetObjectItem(json_script, "script_path"));
            script->script = script_new_from_file(script_path, entity);
        }

        if (cJSON_HasObjectItem(json_components, "light")) {
            light_component_t* light_comp = scene_add_component(scene, entity, "light");
            light_t light;

            cJSON* json_light = cJSON_GetObjectItem(json_components, "light");

            light.type = (u32)cJSON_GetNumberValue(cJSON_GetObjectItem(json_light, "type"));
            loader_parse_vec3(cJSON_GetObjectItem(json_light, "ambient"), light.ambient);
            loader_parse_vec3(cJSON_GetObjectItem(json_light, "diffuse"), light.diffuse);
            loader_parse_vec3(cJSON_GetObjectItem(json_light, "specular"), light.specular);
            light.constant = cJSON_GetNumberValue(cJSON_GetObjectItem(json_light, "constant"));
            light.linear = cJSON_GetNumberValue(cJSON_GetObjectItem(json_light, "linear"));
            light.quadratic = cJSON_GetNumberValue(cJSON_GetObjectItem(json_light, "quadratic"));
            light.cutoff = cJSON_GetNumberValue(cJSON_GetObjectItem(json_light, "cutoff"));
            light.outer_cutoff = cJSON_GetNumberValue(cJSON_GetObjectItem(json_light, "outer_cutoff"));

            light_comp->cast = cJSON_IsTrue(cJSON_GetObjectItem(json_light, "cast"));
            light_comp->shadow_caster = cJSON_IsTrue(cJSON_GetObjectItem(json_light, "shadow_caster"));
            light_comp->light = light;
        }

        if (cJSON_HasObjectItem(json_components, "skybox")) {
            skybox_component_t* skybox = scene_add_component(scene, entity, "skybox");

            cJSON* json_script = cJSON_GetObjectItem(json_components, "skybox");
            skybox->draw = cJSON_IsTrue(cJSON_GetObjectItem(json_script, "draw"));

            char* path = cJSON_GetStringValue(cJSON_GetObjectItem(json_script, "cubemap_path"));
            skybox->cubemap = loader_load_skybox(path);
            cubemap_set_path(skybox->cubemap, path);
        }
    }

    cJSON_Delete(json);

    return scene;
}

void loader_write_scene(scene_t* scene, const char* filename, array_t* excluded_entities) {
    cJSON* json = cJSON_CreateObject();
    cJSON* json_scene = cJSON_AddObjectToObject(json, "scene");
    cJSON* json_entities = cJSON_AddArrayToObject(json_scene, "entities");

    array_t* entities = scene_get_entities(scene);
    for (u32 i = 0; i < array_get_length(entities); i++) {
        entity_t* entity = array_get(entities, i);

        bool excluded = false;
        for (u32 j = 0; j < array_get_length(excluded_entities); j++) {
            if (array_get(excluded_entities, j) == entity) {
                excluded = true;
            }
        }
        if (excluded) {
            continue;
        }

        cJSON* json_entity = cJSON_CreateObject();
        cJSON_AddStringToObject(json_entity, "name", entity_get_name(entity));

        cJSON* json_components = cJSON_AddObjectToObject(json_entity, "components");

        if (scene_has_component(scene, entity, "transform")) {
            transform_component_t* transform = (transform_component_t*)scene_get_component(scene, entity, "transform");
            cJSON* json_transform = cJSON_AddObjectToObject(json_components, "transform");
            cJSON* json_pos = cJSON_AddObjectToObject(json_transform, "pos");
            cJSON* json_rotation = cJSON_AddObjectToObject(json_transform, "rotation");
            cJSON* json_scale = cJSON_AddObjectToObject(json_transform, "scale");

            loader_write_vec3(json_pos, transform->pos);
            loader_write_vec3(json_rotation, transform->rotation);
            loader_write_vec3(json_scale, transform->scale);
        }

        if (scene_has_component(scene, entity, "model")) {
            model_component_t* model = (model_component_t*)scene_get_component(scene, entity, "model");
            cJSON* json_model = cJSON_AddObjectToObject(json_components, "model");
            cJSON_AddStringToObject(json_model, "model_path", model_get_path(model->model));
            cJSON_AddBoolToObject(json_model, "draw", model->draw);
            cJSON_AddBoolToObject(json_model, "shadow_caster", model->shadow_caster);
        }

        if (scene_has_component(scene, entity, "camera")) {
            camera_component_t* camera = (camera_component_t*)scene_get_component(scene, entity, "camera");
            cJSON* json_camera = cJSON_AddObjectToObject(json_components, "camera");
            cJSON_AddBoolToObject(json_camera, "use", camera->use);
        }

        if (scene_has_component(scene, entity, "lua_script")) {
            lua_script_component_t* script = (lua_script_component_t*)scene_get_component(scene, entity, "lua_script");
            cJSON* json_script = cJSON_AddObjectToObject(json_components, "lua_script");
            cJSON_AddStringToObject(json_script, "script_path", script_get_path(script->script));
            cJSON_AddBoolToObject(json_script, "run", script->run);
        }

        if (scene_has_component(scene, entity, "light")) {
            light_component_t* light = (light_component_t*)scene_get_component(scene, entity, "light");
            cJSON* json_light = cJSON_AddObjectToObject(json_components, "light");

            cJSON_AddNumberToObject(json_light, "type", light->light.type);
            loader_write_vec3(cJSON_AddObjectToObject(json_light, "ambient"), light->light.ambient);
            loader_write_vec3(cJSON_AddObjectToObject(json_light, "diffuse"), light->light.diffuse);
            loader_write_vec3(cJSON_AddObjectToObject(json_light, "specular"), light->light.specular);
            cJSON_AddNumberToObject(json_light, "constant", light->light.constant);
            cJSON_AddNumberToObject(json_light, "linear", light->light.linear);
            cJSON_AddNumberToObject(json_light, "quadratic", light->light.quadratic);
            cJSON_AddNumberToObject(json_light, "cutoff", light->light.cutoff);
            cJSON_AddNumberToObject(json_light, "outer_cutoff", light->light.outer_cutoff);
            cJSON_AddBoolToObject(json_light, "cast", light->cast);
            cJSON_AddBoolToObject(json_light, "shadow_caster", light->shadow_caster);
        }

        if (scene_has_component(scene, entity, "skybox")) {
            skybox_component_t* skybox = (skybox_component_t*)scene_get_component(scene, entity, "skybox");
            cJSON* json_skybox = cJSON_AddObjectToObject(json_components, "skybox");
            cJSON_AddStringToObject(json_skybox, "cubemap_path", cubemap_get_path(skybox->cubemap));
            cJSON_AddBoolToObject(json_skybox, "draw", skybox->draw);
        }

        cJSON_AddItemToArray(json_entities, json_entity);
    }

    char* json_str = cJSON_PrintUnformatted(json);
    if (json_str == NULL) {
        const char* error = cJSON_GetErrorPtr();
        PEAR_ERROR("failed to write scene file %s! %s", filename, error);
        cJSON_Delete(json);
        return;
    }
    
    FILE* file = fopen(filename, "w");
    fprintf(file, json_str);
    fclose(file);

    cJSON_Delete(json);
}
