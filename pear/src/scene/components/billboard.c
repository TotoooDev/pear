#include <scene/components/billboard.h>

void billboardcomponent_constructor(void* data, entity_t* entity) {
    billboard_component_t* billboard = (billboard_component_t*)data;

    billboard->texture = NULL;
    billboard->draw = true;
}

void billboardcomponent_destructor(void* data, entity_t* entity) {
    billboard_component_t* billboard = (billboard_component_t*)data;

    if (billboard->texture != NULL) {
        texture_delete(billboard->texture);
    }
}

component_attachment_t billboardcomponent_get_attachment() {
    return (component_attachment_t){
        .creator = billboardcomponent_constructor,
        .destructor = billboardcomponent_destructor
    };
}
