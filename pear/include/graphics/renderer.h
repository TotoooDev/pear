#ifndef PEAR_RENDERER_H
#define PEAR_RENDERER_H

#include <graphics/mesh.h>
#include <graphics/framebuffer.h>
#include <scene/node.h>
#include <scene/types/mesh_3d.h>
#include <core/types.h>

typedef struct Renderer Renderer;

Renderer* renderer_new();
void renderer_delete(Renderer* renderer);

void renderer_clear(Renderer* renderer, f32 r, f32 g, f32 b, f32 a);
void renderer_draw_node_hierarchy(Renderer* renderer, Node* node);
void renderer_draw_mesh3d(Renderer* renderer, Mesh3D* node, mat4 transform);

void renderer_set_fov(Renderer* renderer, f32 fov);
void renderer_set_target(Renderer* renderer, Framebuffer* framebuffer);
void renderer_set_target_window(Renderer* renderer);

#endif
