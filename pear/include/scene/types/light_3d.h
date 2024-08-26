#ifndef PEAR_NODE_LIGHT_3D_H
#define PEAR_NODE_LIGHT_3D_H

#include <graphics/light.h>

typedef struct Light3DCreationInfo {
    Light light;
    vec3 pos;
    vec3 direction;
} Light3DCreationInfo;

typedef struct Light3D Light3D;

Light3D* light3d_new(Light3DCreationInfo* creation_info);
void light3d_delete(Light3D* light);

void light3d_set_pos(Light3D* light, vec3 pos);
void light3d_set_direction(Light3D* light, vec3 direction);

void light3d_get_pos(Light3D* light, vec3 dest);
void light3d_get_direction(Light3D* light, vec3 dest);

#endif
