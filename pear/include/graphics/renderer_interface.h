#ifndef PEAR_GRAPHICS_RENDERER_INTERFACE_H_
#define PEAR_GRAPHICS_RENDERER_INTERFACE_H_

#include <scene/scene.h>
#include <core/types.h>

typedef struct renderer_t renderer_t;
typedef struct renderer_interface_t renderer_interface_t;

typedef void(*renderer_interface_draw_function_t)(renderer_interface_t*, renderer_t*);
typedef void(*renderer_interface_clear_function_t)(renderer_interface_t*, f32, f32, f32);
typedef void(*renderer_interface_delete_function_t)(renderer_interface_t*);

typedef struct renderer_interface_t {
    scene_system_t system;
    renderer_interface_draw_function_t draw_function;
    renderer_interface_clear_function_t clear_function;
    renderer_interface_delete_function_t delete_function;
    void* renderer;
} renderer_interface_t;

#endif
