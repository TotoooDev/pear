#include <scene/components/light.h>
#include <core/alloc.h>

light_component_t* lightcomponent_new() {
    light_component_t* light = (light_component_t*)PEAR_MALLOC(sizeof(light_component_t));

    light->cast = true;

    glm_vec3_copy((vec3){ 0.1f, 0.1f, 0.1f }, light->light.ambient);
    glm_vec3_copy((vec3){ 0.6f, 0.6f, 0.6f }, light->light.diffuse);
    glm_vec3_copy((vec3){ 1.0f, 1.0f, 1.0f }, light->light.specular);

    light->light.constant = 1.0f;
    light->light.linear = 0.14f;
    light->light.quadratic = 0.07f;

    light->light.cutoff = 12.5f;
    light->light.outer_cutoff = 17.5f;
    
    return light;
}
