#include <scene/types/light_3d.h>
#include <stdlib.h>

typedef struct Light3D {
    Light light;
    vec3 pos;
    vec3 direction;
} Light3D;

Light3D* light3d_new(Light3DCreationInfo* creation_info) {
    Light3D* light = (Light3D*)malloc(sizeof(Light3D));
    
    light->light = creation_info->light;
    glm_vec3_copy(creation_info->pos, light->pos);
    glm_vec3_copy(creation_info->direction, light->direction);
    
    return light;
}

void light3d_delete(Light3D* light) {
    free(light);
}

void light3d_set_pos(Light3D* light, vec3 pos) {
    glm_vec3_copy(pos, light->pos);
}

void light3d_set_direction(Light3D* light, vec3 direction) {
    glm_vec3_copy(direction, light->direction);
}

void light3d_get_pos(Light3D* light, vec3 dest) {
    glm_vec3_copy(light->pos, dest);
}

void light3d_get_direction(Light3D* light, vec3 dest) {
    glm_vec3_copy(light->direction, dest);
}
