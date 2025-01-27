#include <script/system.h>
#include <scene/components/lua_script.h>
#include <scene/components/transform.h>
#include <scene/components/camera.h>
#include <graphics/camera.h>

void script_system(scene_t* scene, entity_t* entity, f32 timestep, void* user_data) {
    if (!scene_has_component(scene, entity, "lua_script")) {
        return;
    }

    lua_script_component_t* script = scene_get_component(scene, entity, "lua_script");
    if (script->script == NULL) {
        return;
    }

    if (scene_has_component(scene, entity, "transform")) {
        transform_component_t* transform = scene_get_component(scene, entity, "transform");
        script_begin_table(script->script, "transform");
            script_set_vec3(script->script, transform->pos, "pos");
            script_set_vec3(script->script, transform->rotation, "rotation");
            script_set_vec3(script->script, transform->scale, "scale");
        script_end_table(script->script);
    }
    else {
        script_set_nil(script->script, "transform");
    }

    if (scene_has_component(scene, entity, "camera")) {
        camera_component_t* camera = scene_get_component(scene, entity, "camera");
        script_begin_table(script->script, "camera");
            script_set_bool(script->script, camera->use, "use");

            if (scene_has_component(scene, entity, "transform")) {
                transform_component_t* transform = scene_get_component(scene, entity, "transform");
                vec3 front;
                vec3 right;
                vec3 up;

                camera_get_front(transform->rotation[0], transform->rotation[1], transform->rotation[2], front);
                camera_get_right(transform->rotation[0], transform->rotation[1], transform->rotation[2], right);
                camera_get_up(transform->rotation[0], transform->rotation[1], transform->rotation[2], up);

                script_set_vec3(script->script, front, "front");
                script_set_vec3(script->script, right, "right");
                script_set_vec3(script->script, up, "up");
            }
            else {
                script_set_nil(script->script, "front");
                script_set_nil(script->script, "right");
                script_set_nil(script->script, "up");
            }

        script_end_table(script->script);
    }
    else {
        script_set_nil(script->script, "camera");
    }

    if (!script->has_started) {
        script_on_start(script->script);
        script->has_started = true;
    }

    if (script->run) {
        script_on_update(script->script, timestep);
    }

    if (scene_has_component(scene, entity, "transform")) {
        transform_component_t* transform = scene_get_component(scene, entity, "transform");
        script_get_table(script->script, "transform");
            script_get_vec3(script->script, "pos", transform->pos);
            script_get_vec3(script->script, "rotation", transform->rotation);
            script_get_vec3(script->script, "scale", transform->scale);
        script_end_table_read(script->script);
    }

    if (scene_has_component(scene, entity, "camera")) {
        camera_component_t* camera = scene_get_component(scene, entity, "camera");
        script_get_table(script->script, "camera");
            camera->use = script_get_bool(script->script, "use");
        script_end_table_read(script->script);
    }
}
