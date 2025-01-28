#include <scene/components/transform.h>

void transformcomponent_constructor(void* data, entity_t* entity) {
    transform_component_t* transform = (transform_component_t*)data;
    glm_vec3_zero(transform->pos);
    glm_vec3_zero(transform->rotation);
    glm_vec3_one(transform->scale);
}

component_attachment_t transformcomponent_get_attachment() {
    return (component_attachment_t){
        .creator = transformcomponent_constructor,
        .destructor = NULL
    };
}

void transformcomponent_get_model_matrix(transform_component_t* transform, mat4 dest) {
    glm_mat4_identity(dest);
    glm_translate(dest, transform->pos);
    glm_rotate(dest, glm_rad(transform->rotation[0]), (vec3){ 1.0f, 0.0f, 0.0f });
    glm_rotate(dest, glm_rad(transform->rotation[1]), (vec3){ 0.0f, 1.0f, 0.0f });
    glm_rotate(dest, glm_rad(transform->rotation[2]), (vec3){ 0.0f, 0.0f, 1.0f });
    glm_scale(dest, transform->scale);
}
