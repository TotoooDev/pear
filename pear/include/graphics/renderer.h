#ifndef PEAR_GRAPHICS_RENDERER_H_
#define PEAR_GRAPHICS_RENDERER_H_

#include <graphics/renderer_interface.h>
#include <graphics/texture.h>
#include <scene/scene.h>
#include <core/types.h>

typedef struct renderer_t renderer_t;

// set this to false to enable off-screen rendering. you can retrieve the render in the screen_texture.
void renderer_hint_enable_screen_renderer(bool enable);
void renderer_system(scene_t* scene, entity_t* entity, f32 timestep, void* user_data);

renderer_t* renderer_new();
void renderer_delete(renderer_t* renderer);

void renderer_clear(renderer_t* renderer, f32 r, f32 g, f32 b);
void renderer_draw(renderer_t* renderer);

void renderer_add_renderer_interface_before(renderer_t* renderer, renderer_interface_t* interface);
void renderer_add_renderer_interface(renderer_t* renderer, renderer_interface_t* interface);
void renderer_add_renderer_interface_after(renderer_t* renderer, renderer_interface_t* interface);

u32 renderer_get_num_meshes(renderer_t* renderer);
u32 renderer_get_num_vertices(renderer_t* renderer);

texture_t* renderer_get_screen_texture(renderer_t* renderer);
texture_t* renderer_get_screen_depth_texture(renderer_t* renderer);
texture_t* renderer_get_shadow_map(renderer_t* renderer);
void renderer_get_viewport_size(renderer_t* renderer, f32* width, f32* height, f32* width_scaled, f32* height_scaled);
void renderer_get_view_matrix(renderer_t* renderer, mat4 dest);
void renderer_get_projection_matrix(renderer_t* renderer, mat4 dest);

void renderer_set_viewport_size(renderer_t* renderer, f32 width, f32 height, f32 width_scale, f32 height_scale);
void renderer_set_near(renderer_t* renderer, f32 near);
void renderer_set_far(renderer_t* renderer, f32 far);
void renderer_set_fov(renderer_t* renderer, f32 fov);
void renderer_enable_wireframe(renderer_t* renderer, bool active);

#endif
