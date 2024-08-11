#ifndef PEAR_RENDERER_H
#define PEAR_RENDERER_H

#include <graphics/mesh.h>
#include <graphics/framebuffer.h>
#include <scene/node.h>
#include <scene/types/mesh_3d.h>
#include <core/types.h>

#ifdef PEAR_DEBUG
    #define PEAR_NUKLEAR
#endif

typedef struct Renderer Renderer;

Renderer* renderer_new();
void renderer_delete(Renderer* renderer);

void renderer_clear(Renderer* renderer, f32 r, f32 g, f32 b, f32 a);
void renderer_draw_node_hierarchy(Renderer* renderer, Node* node);
void renderer_draw_model3d(Renderer* renderer, Model3D* node);

void renderer_set_fov(Renderer* renderer, f32 fov);
void renderer_set_target(Renderer* renderer, Framebuffer* framebuffer);
void renderer_set_target_window(Renderer* renderer);

#ifdef PEAR_NUKLEAR

#define NK_PRIVATE
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#include <graphics/vendor/nuklear.h>

static const nk_flags renderer_nk_default_window_flags = NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE | NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE;

typedef void(*NuklearFunction)(Renderer* renderer, struct nk_context* nk_context, void* user_data);
void renderer_nk_add_gui(NuklearFunction function, void* user_data);

#endif

#endif
