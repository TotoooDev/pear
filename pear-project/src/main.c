#include <scene/scene.h>
#include <graphics/editor/editor.h>
#include <graphics/editor/scene_inspector.h>
#include <core/app.h>

#include <entities.h> 

#include <scene/components/billboard.h>
#include <loaders/image.h>

int main(int argc, char* argv[]) {
    app_init();

    scene_t* scene = app_get_scene();

    modelentity_create(scene);
    cameraentity_create(scene);
    lightentity_create(scene);
    skyboxentity_create(scene);

    entity_t* entity = scene_add_entity(scene, "billboard", ENTITY_COMPONENT_TRANSFORM, ENTITY_COMPONENT_BILLBOARD, ENTITY_COMPONENT_END);
    image_t* image = loader_load_image("light_bulb.jpg");
    texture_t* texture = texture_new_from_image(image, TEXTURE_WRAPPING_NONE, TEXTURE_FILTERING_LINEAR);
    billboard_component_t* billboard = (billboard_component_t*)entity_get_component(entity, ENTITY_COMPONENT_BILLBOARD);
    billboard->texture = texture;
    image_delete(image);

    #ifdef PEAR_ENABLE_EDITOR
        // editor_enable_demo(true);
        editor_set_scene(scene);
    #endif

    app_run();

    return 0;
}
