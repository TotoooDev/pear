#include <components/editor_billboard.h>

void editorbillboardcomponent_constructor(void* data, entity_t* entity) {
    editor_billboard_component_t* billboard = (editor_billboard_component_t*)data;

    billboard->texture = NULL;
    billboard->draw = true;
}

component_attachment_t editorbillboardcomponent_get_attachment() {
    return (component_attachment_t){
        .creator = editorbillboardcomponent_constructor,
        .destructor = NULL
    };
}