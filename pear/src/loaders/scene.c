#include <loaders/scene.h>
#include <loaders/model.h>
#include <loaders/vendor/cJSON.h>
#include <scene/components/transform.h>
#include <scene/components/model.h>
#include <scene/components/camera.h>
#include <scene/components/lua_script.h>
#include <scene/components/light.h>
#include <util/filesystem.h>
#include <core/log.h>
#include <core/alloc.h>

void loader_parse_vec3(cJSON* json, vec3 dest) {
    f32 x = cJSON_GetNumberValue(cJSON_GetObjectItem(json, "x"));
    f32 y = cJSON_GetNumberValue(cJSON_GetObjectItem(json, "y"));
    f32 z = cJSON_GetNumberValue(cJSON_GetObjectItem(json, "z"));

    dest[0] = x;
    dest[1] = y;
    dest[2] = z;
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
        entity_t* entity = scene_add_entity(scene, name, ENTITY_COMPONENT_END);

        cJSON* json_components = cJSON_GetObjectItem(json_entity, "components");

        if (cJSON_HasObjectItem(json_components, "transform")) {
            transform_component_t* transform = entity_add_component(entity, ENTITY_COMPONENT_TRANSFORM);

            cJSON* json_transform = cJSON_GetObjectItem(json_components, "transform");
            cJSON* json_pos = cJSON_GetObjectItem(json_transform, "pos");
            cJSON* json_rotation = cJSON_GetObjectItem(json_transform, "rotation");
            cJSON* json_scale = cJSON_GetObjectItem(json_transform, "scale");

            loader_parse_vec3(json_pos, transform->pos);
            loader_parse_vec3(json_rotation, transform->rotation);
            loader_parse_vec3(json_scale, transform->scale);
        }

        if (cJSON_HasObjectItem(json_components, "model")) {
            model_component_t* model = entity_add_component(entity, ENTITY_COMPONENT_MODEL);

            cJSON* json_model = cJSON_GetObjectItem(json_components, "model");
            model->draw = cJSON_IsTrue(cJSON_GetObjectItem(json_model, "draw"));
            model->shadow_caster = cJSON_IsTrue(cJSON_GetObjectItem(json_model, "shadow_caster"));

            char* model_path = cJSON_GetStringValue(cJSON_GetObjectItem(json_model, "model_path"));
            model->model = loader_load_gltf(model_path);
        }

        if (cJSON_HasObjectItem(json_components, "camera")) {
            camera_component_t* camera = entity_add_component(entity, ENTITY_COMPONENT_CAMERA);

            cJSON* json_cam = cJSON_GetObjectItem(json_components, "camera");
            camera->use = cJSON_IsTrue(cJSON_GetObjectItem(json_cam, "use"));
        }

        if (cJSON_HasObjectItem(json_components, "lua_script")) {
            lua_script_component_t* script = entity_add_component(entity, ENTITY_COMPONENT_LUA_SCRIPT);

            cJSON* json_script = cJSON_GetObjectItem(json_components, "lua_script");
            script->run = cJSON_IsTrue(cJSON_GetObjectItem(json_script, "run"));

            char* script_path = cJSON_GetStringValue(cJSON_GetObjectItem(json_script, "script_path"));
            script->script = script_new_from_file(script_path, entity);
        }

        if (cJSON_HasObjectItem(json_components, "light")) {
            light_component_t* light_comp = entity_add_component(entity, ENTITY_COMPONENT_LIGHT);
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
    }

    cJSON_Delete(json);

    return scene;
}
