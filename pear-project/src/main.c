#include <pear.h>
#include <scene/scene.h>
#include <scene/components/transform.h>
#include <scene/components/drawable.h>
#include <scene/components/camera.h>
#include <scene/components/script.h>
#include <graphics/window.h>
#include <graphics/gui/gui.h>
#include <graphics/gui/scene_inspector.h>
#include <core/app.h>
#include <core/timer.h>
#include <core/log.h>

void on_start(entity_t* entity, f32 timestep) {
    PEAR_INFO("i am created!");
}

void on_update(entity_t* entity, f32 timestep) {
    // PEAR_INFO("i am updated!");
}

void on_end(entity_t* entity, f32 timestep) {
    PEAR_INFO("i am deleted!");
}

void gui(struct nk_context* nk_context, void* user_data) {
    if (nk_begin(nk_context, "hello!", nk_rect(10, 10, 200, 200), gui_default_window_flags)) {
        nk_layout_row_dynamic(nk_context, 16, 1);
        nk_label(nk_context, "welcome to pear!", NK_TEXT_ALIGN_LEFT);
    }
    nk_end(nk_context);
}

int main(int argc, char* argv[]) {
    pear_print_hello();
    app_init();

    scene_t* scene = app_get_scene();
    entity_t* entity = scene_add_entity(scene, "entity", ENTITY_COMPONENT_SCRIPT, ENTITY_COMPONENT_DRAWABLE, ENTITY_COMPONENT_TRANSFORM, ENTITY_COMPONENT_END);
    
    script_component_t* script = (script_component_t*)entity_get_component(entity, ENTITY_COMPONENT_SCRIPT);
    script->on_start = on_start;
    script->on_update = on_update;
    script->on_end = on_end;

    drawable_component_t* drawable = (drawable_component_t*)entity_get_component(entity, ENTITY_COMPONENT_DRAWABLE);
    vec3 positions[] = {
        { 0.5f,  0.5f, 0.0f },
        { 0.5f, -0.5f, 0.0f },
        { -0.5f, -0.5f, 0.0f },
        { -0.5f,  0.5f, 0.0f }
    };
    vec3 colors[] = {
        { 1.0f, 0.0f, 0.0f },
        { 0.0f, 1.0f, 0.0f },
        { 0.0f, 0.0f, 1.0f },
        { 1.0f, 0.0f, 1.0f }
    };
    vec2 texture_coords[] = {
        { 1.0f, 1.0f },
        { 1.0f, 0.0f },
        { 0.0f, 0.0f },
        { 0.0f, 1.0f }
    };
    u32 indices[] = {
        0, 1, 3,
        1, 2, 3
    };
    mesh_info_t* mesh_info = meshinfo_new();
    meshinfo_add_indices(mesh_info, indices, 6);
    meshinfo_add_position(mesh_info, positions, 4);
    meshinfo_add_color(mesh_info, colors, 4);
    meshinfo_add_texture_coords(mesh_info, texture_coords, 4);

    bool success;
    pear_image_t pear_image = pear_image_load("wall.image", &success);
    image_t* image = image_new_from_pear_image(pear_image);

    drawable->texture = texture_new_from_image(image, TEXTURE_WRAPPING_NONE, TEXTURE_FILTERING_NEAREST);
    drawable->mesh = mesh_new(mesh_info);
    
    meshinfo_delete(mesh_info);
    image_delete(image);

    entity_t* camera = scene_add_entity(scene, "camera", ENTITY_COMPONENT_TRANSFORM, ENTITY_COMPONENT_CAMERA, ENTITY_COMPONENT_END);
    transform_component_t* transform = (transform_component_t*)entity_get_component(camera, ENTITY_COMPONENT_TRANSFORM);
    transform->pos[2] = 2.0f;
    transform->rotation[0] = -90.0f;

    gui_add(gui, NULL);
    gui_add(gui_scene_inspector, NULL);
    gui_scene_inspector_set_scene(scene);

    app_run();

    return 0;
}
