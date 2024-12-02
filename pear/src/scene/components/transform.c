#include <scene/components/transform.h>
#include <core/alloc.h>

transform_component_t* transformcomponent_create() {
    transform_component_t* comp = (transform_component_t*)PEAR_MALLOC(sizeof(transform_component_t));
    
    glm_vec3_zero(comp->pos);
    glm_vec3_zero(comp->rotation);
    glm_vec3_one(comp->scale);

    return comp;
}
