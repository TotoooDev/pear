#include <scene/scene.h>
#include <scene/components/transform.h>
#include <scene/components/camera.h>
#include <scene/components/script.h>
#include <scene/components/lua_script.h>
#include <graphics/camera.h>
#include <util/array.h>
#include <core/log.h>
#include <core/types.h>
#include <core/alloc.h>
#include <stdarg.h>

#ifdef PEAR_ENABLE_EDITOR
#include <scene/components/billboard.h>
#include <loaders/image.h>
#endif

#define SCENE_NUM_MAX_ENTITIES 512

typedef struct scene_t {
    array_t* entities;
} scene_t;

void scene_set_lua_script_values(entity_t* entity) {
    lua_script_component_t* script_comp = (lua_script_component_t*)entity_get_component(entity, ENTITY_COMPONENT_LUA_SCRIPT);
    script_t* script = script_comp->script;

    if (entity_has_component(entity, ENTITY_COMPONENT_TRANSFORM)) {
        transform_component_t* transform = (transform_component_t*)entity_get_component(entity, ENTITY_COMPONENT_TRANSFORM);
        script_begin_table(script, "transform");
            script_set_vec3(script, transform->pos, "pos");
            script_set_vec3(script, transform->rotation, "rotation");
            script_set_vec3(script, transform->scale, "scale");
        script_end_table(script);
    }

    if (entity_has_component(entity, ENTITY_COMPONENT_CAMERA)) {
        transform_component_t* transform = (transform_component_t*)entity_get_component(entity, ENTITY_COMPONENT_TRANSFORM);
        camera_component_t* camera = (camera_component_t*)entity_get_component(entity, ENTITY_COMPONENT_CAMERA);

        vec3 front;
        vec3 right;
        vec3 up;
        camera_get_front(transform->rotation[0], transform->rotation[1], transform->rotation[2], front);
        camera_get_right(transform->rotation[0], transform->rotation[1], transform->rotation[2], right);
        camera_get_up(transform->rotation[0], transform->rotation[1], transform->rotation[2], up);

        script_begin_table(script, "camera");
            script_set_bool(script, camera->use, "use");
            script_set_vec3(script, front, "front");
            script_set_vec3(script, right, "right");
            script_set_vec3(script, up, "up");
        script_end_table(script);
    }
}

void scene_set_entity_script_values(entity_t* entity) {
    lua_script_component_t* script_comp = (lua_script_component_t*)entity_get_component(entity, ENTITY_COMPONENT_LUA_SCRIPT);
    script_t* script = script_comp->script;

    if (entity_has_component(entity, ENTITY_COMPONENT_TRANSFORM)) {
        transform_component_t* transform = (transform_component_t*)entity_get_component(entity, ENTITY_COMPONENT_TRANSFORM);
        script_get_table(script, "transform");
            script_get_vec3(script, "pos", transform->pos);
            script_get_vec3(script, "rotation", transform->rotation);
            script_get_vec3(script, "scale", transform->scale);
        script_end_table_read(script);
    }

    if (entity_has_component(entity, ENTITY_COMPONENT_CAMERA)) {
        camera_component_t* camera = (camera_component_t*)entity_get_component(entity, ENTITY_COMPONENT_CAMERA);

        script_get_table(script, "camera");
            camera->use = script_get_bool(script, "use");
        script_end_table_read(script);
    }
}

void scene_update_script_component(scene_t* scene, entity_t* entity, f32 timestep) {
    script_component_t* script = (script_component_t*)entity_get_component(entity, ENTITY_COMPONENT_SCRIPT);
    
    if (!script->run) {
        return;
    }

    if (!script->has_started) {
        if (script->on_start != NULL) {
            script->on_start(entity, timestep);
        }
        script->has_started = true;
    }

    if (script->on_update != NULL) {
        script->on_update(entity, timestep);
    }
}

void scene_update_lua_script_component(scene_t* scene, entity_t* entity, f32 timestep) {
    lua_script_component_t* script = (lua_script_component_t*)entity_get_component(entity, ENTITY_COMPONENT_LUA_SCRIPT);
    
    if (!script->run || script->script == NULL) {
        return;
    }

    scene_set_lua_script_values(entity);
    
    if (!script->has_started) {
        script_on_start(script->script);
        script->has_started = true;
        scene_set_entity_script_values(entity);
    }

    script_on_update(script->script, timestep);
    scene_set_entity_script_values(entity);
}

scene_t* scene_new() {
    scene_t* scene = (scene_t*)PEAR_MALLOC(sizeof(scene_t));
    scene->entities = array_new(SCENE_NUM_MAX_ENTITIES);

    #ifdef PEAR_ENABLE_EDITOR
        scene_setup_editor_scene(scene);
    #endif

    return scene;
}

void scene_delete(scene_t* scene) {
    for (i32 i = array_get_length(scene->entities) - 1; i >= 0; i--) {
        entity_t* entity = array_get(scene->entities, i);
        scene_remove_entity(scene, entity);
    }

    array_delete(scene->entities);
    PEAR_FREE(scene);
}

entity_t* scene_add_entity(scene_t* scene, const char* name, ...) {
    va_list args;
    va_start(args, name);

    entity_t* entity = entity_new_from_va_list(name, args);
    
    va_end(args);

    #ifdef PEAR_ENABLE_EDITOR
        if (!entity_has_component(entity, ENTITY_COMPONENT_BILLBOARD)) {
            if (entity_has_component(entity, ENTITY_COMPONENT_CAMERA)) {

            }
            if (entity_has_component(entity, ENTITY_COMPONENT_LIGHT)) {
                image_t* image = loader_load_image("editor/light_bulb.png");
                texture_t* texture = texture_new_from_image(image, TEXTURE_WRAPPING_NONE, TEXTURE_FILTERING_LINEAR);
                image_delete(image);
                billboard_component_t* billboard = entity_add_component(entity, ENTITY_COMPONENT_BILLBOARD);
                billboard->texture = texture;
            }
        }
    #endif
    
    array_add(scene->entities, entity);
    return entity;
}

void scene_remove_entity(scene_t* scene, entity_t* entity) {
    if (entity_has_component(entity, ENTITY_COMPONENT_SCRIPT)) {
        script_component_t* script = (script_component_t*)entity_get_component(entity, ENTITY_COMPONENT_SCRIPT);
        if (script->run && script->on_end != NULL) {
            script->on_end(entity, 0.0f);
        }
    }

    if (entity_has_component(entity, ENTITY_COMPONENT_LUA_SCRIPT)) {
        lua_script_component_t* script = (lua_script_component_t*)entity_get_component(entity, ENTITY_COMPONENT_LUA_SCRIPT);
        if (script->run) {
            script_on_destroy(script->script);
        }
    }

    array_remove(scene->entities, entity);
    entity_delete(entity);
}

void scene_update(scene_t* scene, f32 timestep) {
    for (u32 i = 0; i < array_get_length(scene->entities); i++) {
        entity_t* entity = array_get(scene->entities, i);

        if (entity_has_component(entity, ENTITY_COMPONENT_SCRIPT)) {
            scene_update_script_component(scene, entity, timestep);
        }

        if (entity_has_component(entity, ENTITY_COMPONENT_LUA_SCRIPT)) {
            scene_update_lua_script_component(scene, entity, timestep);
        }
    }
}

array_t* scene_get_entities(scene_t* scene) {
    return scene->entities;
}

#ifdef PEAR_ENABLE_EDITOR
void scene_setup_editor_scene(scene_t* scene) {
    entity_t* editor_cam = scene_add_entity(
        scene,
        "editor camera",
        ENTITY_COMPONENT_TRANSFORM,
        ENTITY_COMPONENT_BILLBOARD,
        ENTITY_COMPONENT_CAMERA,
        ENTITY_COMPONENT_LUA_SCRIPT,
        ENTITY_COMPONENT_END
    );
    
    // bruh
    const char* cam_script_str =
"local move_camera = false \n"
"local speed = 0.01 \n"
"local keys = { forward = false, backward = false, left = false, right = false, up = false, down = false } \n"
"function on_update(timestep) \n"
"    if not move_camera then return end \n"
"    if keys.forward then pear.transform.pos = pear.transform.pos + pear.camera.front * speed * timestep end \n"
"    if keys.backward then pear.transform.pos = pear.transform.pos - pear.camera.front * speed * timestep end \n"
"    if keys.left then pear.transform.pos = pear.transform.pos - pear.camera.right * speed * timestep end \n"
"    if keys.right then pear.transform.pos = pear.transform.pos + pear.camera.right * speed * timestep end \n"
"    if keys.up then pear.transform.pos.y = pear.transform.pos.y + speed * timestep end \n"
"    if keys.down then pear.transform.pos.y = pear.transform.pos.y - speed * timestep end \n"
"    pear.transform.rotation.x = pear.transform.rotation.x + pear.mouse.relative.x \n"
"    pear.transform.rotation.y = pear.transform.rotation.y - pear.mouse.relative.y \n"
"    if pear.transform.rotation.y > 89.0 then \n"
"        pear.transform.rotation.y = 89.0 \n"
"    end \n"
"    if pear.transform.rotation.y < -89.0 then \n"
"        pear.transform.rotation.y = -89.0 \n"
"    end \n"
"end \n"
"function on_key_press(key) \n"
"    if key == \"PEAR_KEY_W\" then keys.forward = true end \n"
"    if key == \"PEAR_KEY_S\" then keys.backward = true end \n"
"    if key == \"PEAR_KEY_A\" then keys.left = true end \n"
"    if key == \"PEAR_KEY_D\" then keys.right = true end \n"
"    if key == \"PEAR_KEY_SPACE\" then keys.up = true end \n"
"    if key == \"PEAR_KEY_LEFT_CONTROL\" then keys.down = true end \n"
"end \n"
"function on_key_release(key) \n"
"    if key == \"PEAR_KEY_W\" then keys.forward = false end \n"
"    if key == \"PEAR_KEY_S\" then keys.backward = false end \n"
"    if key == \"PEAR_KEY_A\" then keys.left = false end \n"
"    if key == \"PEAR_KEY_D\" then keys.right = false end \n"
"    if key == \"PEAR_KEY_SPACE\" then keys.up = false end \n"
"    if key == \"PEAR_KEY_LEFT_CONTROL\" then keys.down = false end \n"
"    if key == \"PEAR_KEY_LEFT_ALT\" then \n"
"        move_camera = not move_camera \n"
"        pear.window.lock_mouse(move_camera) \n"
"    end \n"
"end \n"
"function on_editor() \n"
"    speed = pear.editor.drag_number(\"speed\", speed, 0.001, 0.0, math.huge) \n"
"    move_camera = pear.editor.checkbox(\"move camera\", move_camera) \n"
"end \n";
    script_t* cam_script = script_new(cam_script_str, editor_cam);
    lua_script_component_t* cam_script_comp = entity_get_component(editor_cam, ENTITY_COMPONENT_LUA_SCRIPT);
    cam_script_comp->script = cam_script;
    camera_component_t* cam_comp = entity_get_component(editor_cam, ENTITY_COMPONENT_CAMERA);
    cam_comp->use = true;
}

void scene_display_component_billboards(scene_t* scene, bool display) {

}
#endif
