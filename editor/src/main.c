#include <core/app.h>
#include <core/log.h>
#include <panels/menu_bar.h>
#include <panels/general_info.h>
#include <panels/scene_inspector.h>
#include <panels/entity_inspector.h>
#include <panels/viewport.h>
#include <scene/components/transform.h>
#include <scene/components/model.h>
// #include <scene/components/billboard.h>
#include <scene/components/camera.h>
// #include <scene/components/script.h>
// #include <scene/components/lua_script.h>
// #include <scene/components/light.h>
// #include <scene/components/skybox.h>
#include <graphics/editor/editor.h>
#include <string.h>

#include <loaders/model.h>

int main(int argc, char* argv[]) {
    app_init();

    scene_t* scene = app_get_scene();

    entity_t* entity = scene_add_entity(scene, "cool entity");
    transform_component_t* transform = scene_add_component(scene, entity, "transform");
    glm_vec3_fill(transform->scale, 50.0f);
    model_component_t* model = scene_add_component(scene, entity, "model");
    model->model = loader_load_gltf("Avocado.glb");

    entity_t* camera = scene_add_entity(scene, "camera");
    transform_component_t* camera_transform = scene_add_component(scene, camera, "transform");
    camera_transform->pos[0] = -5.0f;
    camera_component_t* camera_comp = scene_add_component(scene, camera, "camera");

    panel_scene_inspector_set_scene(scene);
    panel_entity_inspector_set_scene(scene);
    panel_entity_inspector_set_entity(camera);

    editor_add_function(panel_menu_bar, NULL);
    editor_add_function(panel_general_info, NULL);
    editor_add_function(panel_scene_inspector, NULL);
    editor_add_function(panel_entity_inspector, NULL);
    editor_add_function(panel_viewport, NULL);
    
    app_run();

    return 0;
}
