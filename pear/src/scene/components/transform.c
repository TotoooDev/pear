#include <scene/components/transform.h>
#include <core/alloc.h>

transform_component_t* transformcomponent_new() {
    transform_component_t* transform = (transform_component_t*)PEAR_MALLOC(sizeof(transform_component_t));
    
    glm_vec3_zero(transform->pos);
    glm_vec3_zero(transform->rotation);
    glm_vec3_one(transform->scale);

    return transform;
}

void transformcomponent_get_model_matrix(transform_component_t* transform, mat4 dest) {
    glm_mat4_identity(dest);
    glm_translate(dest, transform->pos);
    glm_rotate(dest, transform->rotation[0], (vec3){ 1.0f, 0.0f, 0.0f });
    glm_rotate(dest, transform->rotation[1], (vec3){ 0.0f, 1.0f, 0.0f });
    glm_rotate(dest, transform->rotation[2], (vec3){ 0.0f, 0.0f, 1.0f });
    glm_scale(dest, transform->scale);
}
