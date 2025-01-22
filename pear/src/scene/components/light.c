#include <scene/components/light.h>

void lightcomponent_constructor(void* data, entity_t* entity) {
    light_component_t* light = (light_component_t*)data;
    
    light->cast = true;
    light->shadow_caster = true;
    light->light.type = LIGHT_TYPE_DIRECTIONAL;
    glm_vec3_fill(light->light.ambient, 0.2f);
    glm_vec3_fill(light->light.diffuse, 0.6f);
    glm_vec3_fill(light->light.specular, 1.0f);
    light->light.constant = 1.0f;
    light->light.linear = 0.22f;
    light->light.quadratic = 0.21f;
    light->light.cutoff = 12.5f;
    light->light.outer_cutoff = 17.5f;
}

component_attachment_t lightcomponent_get_attachment() {
    return (component_attachment_t){
        .creator = lightcomponent_constructor,
        .destructor = NULL
    };
}
