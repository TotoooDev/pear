#include <scene/scene.h>
#include <graphics/editor/editor.h>
#include <graphics/editor/scene_inspector.h>
#include <core/app.h>

#include <entities.h> 

#include <scene/components/lua_script.h>
#include <script/script.h>
#include <core/log.h>

int main(int argc, char* argv[]) {
    app_init();

    scene_t* scene = app_get_scene();

    entity_t* entity = scene_add_entity(scene, "scripted entity", ENTITY_COMPONENT_LUA_SCRIPT, ENTITY_COMPONENT_END);
    lua_script_component_t* script = (lua_script_component_t*)entity_get_component(entity, ENTITY_COMPONENT_LUA_SCRIPT);
    script->script = script_new_from_file("scripts/script.lua");

    modelentity_create(scene);
    cameraentity_create(scene);
    lightentity_create(scene);
    skyboxentity_create(scene);

    #ifdef PEAR_ENABLE_EDITOR
        // editor_enable_demo(true);
        editor_set_scene(scene);
    #endif

    app_run();

    return 0;
}
