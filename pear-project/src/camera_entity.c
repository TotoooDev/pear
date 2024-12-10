#include <camera_entity.h>

entity_t* cameraentity_create(scene_t* scene) {
    entity_t* entity = scene_add_entity(scene, "camera", ENTITY_COMPONENT_TRANSFORM, ENTITY_COMPONENT_CAMERA, ENTITY_COMPONENT_END);
    return entity;
}
