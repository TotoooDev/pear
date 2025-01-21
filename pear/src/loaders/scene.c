#include <loaders/scene.h>
#include <loaders/vendor/cJSON.h>
#include <scene/components/transform.h>
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
    }

    return scene;
}
