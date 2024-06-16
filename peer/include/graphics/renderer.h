#ifndef PEER_RENDERER_H
#define PEER_RENDERER_H

#include <graphics/mesh.h>
#include <scene/types/mesh_3d.h>
#include <core/types.h>

typedef struct Renderer Renderer;

Renderer* renderer_new();
void renderer_delete(Renderer* renderer);

void renderer_clear(Renderer* renderer, f32 r, f32 g, f32 b, f32 a);
void renderer_draw_mesh(Renderer* renderer, Mesh* mesh, Mesh3D* node);

#endif
