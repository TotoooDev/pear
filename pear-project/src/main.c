#include <scene/scene.h>
#include <graphics/editor/editor.h>
#include <graphics/editor/scene_inspector.h>
#include <core/app.h>

#include <loaders/model.h>
#include <scene/components/model.h>

#include <entities.h> 

int main(int argc, char* argv[]) {
    app_init();

    scene_t* scene = app_get_scene();

    entity_t* entity = scene_add_entity(scene, "model", ENTITY_COMPONENT_TRANSFORM, ENTITY_COMPONENT_MODEL, ENTITY_COMPONENT_END);
    model_component_t* model_comp = entity_get_component(entity, ENTITY_COMPONENT_MODEL);
    model_comp->model = loader_load_gltf("Avocado.glb");

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
