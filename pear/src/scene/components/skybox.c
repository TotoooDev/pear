#include <scene/components/skybox.h>

void skyboxcomponent_constructor(void* data, entity_t* entity) {
    skybox_component_t* skybox = (skybox_component_t*)data;

    skybox->cubemap = NULL;
    skybox->draw = true;
}

void skyboxcomponent_destructor(void* data, entity_t* entity) {
    skybox_component_t* skybox = (skybox_component_t*)data;

    if (skybox->cubemap != NULL) {
        cubemap_delete(skybox->cubemap);
    }
}

component_attachment_t skyboxcomponent_get_attachment() {
    return (component_attachment_t){
        .creator = skyboxcomponent_constructor,
        .destructor = skyboxcomponent_destructor
    };
}
