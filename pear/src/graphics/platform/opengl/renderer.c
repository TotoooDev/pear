#ifdef PEAR_PLATFORM_OPENGL

#include <graphics/renderer.h>
#include <graphics/platform/opengl/shader.h>
#include <graphics/platform/opengl/mesh.h>
#include <graphics/platform/opengl/texture.h>
#include <graphics/platform/opengl/framebuffer.h>
#include <scene/types/camera_3d.h>
#include <event/event_dispatcher.h>
#include <core/app.h>
#include <core/log.h>
#include <GL/glew.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <graphics/platform/opengl/window.h>

#ifdef PEAR_DEBUG
    #define MAX_VERTEX_BUFFER 512 * 1024
    #define MAX_ELEMENT_BUFFER 128 * 1024

    #define PEAR_NUKLEAR
    #define NK_PRIVATE
    #define NK_INCLUDE_FIXED_TYPES
    #define NK_INCLUDE_STANDARD_IO
    #define NK_INCLUDE_DEFAULT_ALLOCATOR
    #define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
    #define NK_INCLUDE_FONT_BAKING
    #define NK_INCLUDE_DEFAULT_FONT
    #define NK_IMPLEMENTATION
    #include <graphics/vendor/nuklear.h>
    
    #define NK_GLFW_GL3_IMPLEMENTATION
    #include <graphics/vendor/nuklear_glfw3.h>
#endif

typedef struct Renderer {
    f32 fov;
    f32 aspect_ratio;
    f32 near;
    f32 far;

    f32 viewport_width;
    f32 viewport_height;
    f32 viewport_scale_x;
    f32 viewport_scale_y;
    f32 viewport_width_scaled;
    f32 viewport_height_scaled;

    Shader* shader_mesh;
    Shader* shader_screen;
    mat4 projection_matrix;
    mat4 view_matrix;

    Framebuffer* screen_framebuffer;
    Mesh* screen_mesh;

    #ifdef PEAR_NUKLEAR
        struct nk_glfw glfw;
        struct nk_context* context;
        struct nk_font_atlas* atlas;
    #endif
} Renderer;

static bool renderer_is_glew_init = false;

#ifdef PEAR_NUKLEAR
void renderer_nuklear_init(Renderer* renderer) {
    renderer->context = nk_glfw3_init(&(renderer->glfw), window_get_glfw(app_get_window()), NK_GLFW3_INSTALL_CALLBACKS);
    nk_glfw3_font_stash_begin(&(renderer->glfw), &(renderer->atlas));
    nk_glfw3_font_stash_end(&(renderer->glfw));
}

void renderer_nuklear_free(Renderer* renderer) {
    nk_font_atlas_clear(renderer->atlas);
    nk_free(renderer->context);
}

void renderer_nuklear_render(Renderer* renderer) {
    nk_glfw3_render(&(renderer->glfw), NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);
}

void renderer_nuklear_clear(Renderer* renderer) {
    nk_glfw3_new_frame(&(renderer->glfw));
}
#endif

void renderer_calculate_projection(Renderer* renderer) {
    glm_perspective(renderer->fov, renderer->aspect_ratio, renderer->near, renderer->far, renderer->projection_matrix);
}

void renderer_set_viewport(Renderer* renderer) {
    glViewport(0, 0, renderer->viewport_width_scaled, renderer->viewport_height_scaled);
}

void renderer_on_event(EventType type, void* e, void* user_data) {
    Renderer* renderer = (Renderer*)user_data;

    if (type == EVENT_TYPE_WINDOW_RESIZED) {
        WindowResizedEvent* event = (WindowResizedEvent*)e;
        renderer->viewport_width = event->width;
        renderer->viewport_height = event->height;
        renderer->viewport_width_scaled = renderer->viewport_width * renderer->viewport_scale_x;
        renderer->viewport_height_scaled = renderer->viewport_height * renderer->viewport_scale_y;
        renderer->aspect_ratio = renderer->viewport_width / renderer->viewport_height;
        
        renderer_calculate_projection(renderer);
        renderer_set_viewport(renderer);

        framebuffer_delete(renderer->screen_framebuffer);
        TextureFormat formats[] = { TEXTURE_FORMAT_RGBA };
        renderer->screen_framebuffer = framebuffer_new(renderer->viewport_width_scaled, renderer->viewport_height_scaled, formats, 1, true);
        mesh_get_material(renderer->screen_mesh)->albedo = framebuffer_get_texture(renderer->screen_framebuffer, 0);
    }

    if (type == EVENT_TYPE_WINDOW_SCALE_CHANGED) {
        WindowScaleChanged* event = (WindowScaleChanged*)e;
        renderer->viewport_scale_x = event->scale_x;
        renderer->viewport_scale_y = event->scale_y;
        renderer->viewport_width_scaled = renderer->viewport_width * event->scale_x;
        renderer->viewport_height_scaled = renderer->viewport_height * event->scale_y;

        renderer_set_viewport(renderer);
    }
}

void renderer_debug_output(GLenum source, GLenum type, u32 id, GLenum severity, GLsizei length, const char *message, const void *userParam) {
    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH:
        case GL_DEBUG_SEVERITY_MEDIUM:
            PEAR_ERROR("opengl (%d): %s", id, message);
            break;
        case GL_DEBUG_SEVERITY_LOW:
            PEAR_WARN("opengl (%d): %s", id, message);
            break;
        // case GL_DEBUG_SEVERITY_NOTIFICATION:
            // PEAR_INFO("opengl (%d): %s", id, message);
            // break;
    }
}

void renderer_set_uniforms(Renderer* renderer, Material* material) {
    shader_set_u32(renderer->shader_mesh, 0, "u_platform");
    shader_set_mat4(renderer->shader_mesh, renderer->projection_matrix, "u_projection");
    shader_set_mat4(renderer->shader_mesh, renderer->view_matrix, "u_view");

    if (material->albedo != NULL)
        shader_set_i32(renderer->shader_mesh, 0, "u_albedo");
}

void renderer_init_debug_output() {
#ifdef PEAR_DEBUG
    i32 flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (!(flags & GL_CONTEXT_FLAG_DEBUG_BIT)) {
        PEAR_WARN("failed to initialize opengl debug output! good luck debugging the app >:)");
        return;
    }

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(renderer_debug_output, NULL);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
#endif
}

void renderer_init_shader(Shader** shader, const char* vertex_path, const char* fragment_path) {
    FILE* vertex_file = fopen(vertex_path, "r");
    if (vertex_file == NULL) {
        PEAR_ERROR("failed to open file %s!", vertex_path);
        return;
    }

    char* vertex_buffer = NULL;
    size_t vertex_length;
    ssize_t vertex_bytes_read = getdelim(&vertex_buffer, &vertex_length, '\0', vertex_file);
    if (vertex_bytes_read == -1) {
        PEAR_ERROR("failed to read file %s!", vertex_path);
        return;
    }

    FILE* fragment_file = fopen(fragment_path, "r");
    if (fragment_file == NULL) {
        PEAR_ERROR("failed to open file %s!", fragment_path);
        return;
    }

    char* fragment_buffer = NULL;
    size_t fragment_length;
    ssize_t fragment_bytes_read = getdelim(&fragment_buffer, &fragment_length, '\0', fragment_file);
    if (fragment_bytes_read == -1) {
        PEAR_ERROR("failed to read file %s!", fragment_path);
        return;
    }

    *shader = shader_new(vertex_buffer, fragment_buffer);

    fclose(vertex_file);
    fclose(fragment_file);
}

void renderer_init_shaders(Renderer* renderer) {
    renderer_init_shader(&(renderer->shader_mesh), "default.vert", "default.frag");
    renderer_init_shader(&(renderer->shader_screen), "framebuffer.vert", "framebuffer.frag");
}

void renderer_render_to_screen(Renderer* renderer) {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    shader_use(renderer->shader_screen);
    mesh_use(renderer->screen_mesh);
    glDisable(GL_DEPTH_TEST);
    texture_use(framebuffer_get_texture(renderer->screen_framebuffer, 0));
    glDrawElements(GL_TRIANGLES, mesh_get_num_indices(renderer->screen_mesh), GL_UNSIGNED_INT, NULL);
    glEnable(GL_DEPTH_TEST);
}

Renderer* renderer_new() {
    if (renderer_is_glew_init == false) {
        if (glewInit() != GLEW_OK) {
            PEAR_ERROR("failed to init glew!");
            return NULL;
        }

        renderer_init_debug_output();
        renderer_is_glew_init = true;
    }

    Renderer* renderer = (Renderer*)malloc(sizeof(Renderer));

    renderer_init_shaders(renderer);
    renderer->aspect_ratio = window_get_width(app_get_window()) / (f32)window_get_height(app_get_window());
    renderer->fov = glm_rad(45.0f);
    renderer->near = 0.01f;
    renderer->far = 100.0f;
    renderer->viewport_width = window_get_width(app_get_window());
    renderer->viewport_height = window_get_height(app_get_window());
    renderer->viewport_scale_x = window_get_scale_x(app_get_window());
    renderer->viewport_scale_y = window_get_scale_y(app_get_window());
    renderer->viewport_width_scaled = renderer->viewport_width * window_get_scale_x(app_get_window());
    renderer->viewport_height_scaled = renderer->viewport_height * window_get_scale_y(app_get_window());

    TextureFormat formats[] = { TEXTURE_FORMAT_RGBA };
    renderer->screen_framebuffer = framebuffer_new(renderer->viewport_width_scaled, renderer->viewport_height_scaled, formats, 1, true);

    vec2 positions[] = {
        {  1.0f,  1.0f },
        {  1.0f, -1.0f }, 
        { -1.0f, -1.0f }, 
        { -1.0f,  1.0f }
    };
    vec2 texture_coords[] = {
        { 1.0f, 1.0f },
        { 1.0f, 0.0f },
        { 0.0f, 0.0f },
        { 0.0f, 1.0f }
    };
    
    MeshInfo* mesh_info = meshinfo_new();
    meshinfo_add_attribute_vec2(mesh_info, true, positions, 4);
    meshinfo_add_attribute_vec2(mesh_info, true, texture_coords, 4);

    u32 indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    Material material = { .albedo = framebuffer_get_texture(renderer->screen_framebuffer, 0) };
    renderer->screen_mesh = mesh_new(mesh_info, material, indices, sizeof(indices));

    meshinfo_delete(mesh_info);

    renderer_set_viewport(renderer);
    renderer_calculate_projection(renderer);
    glm_mat4_identity(renderer->view_matrix);

    event_subscribe(renderer_on_event, renderer);

    #ifdef PEAR_NUKLEAR
        renderer_nuklear_init(renderer);
    #endif

    return renderer;
}

void renderer_delete(Renderer* renderer) {
    #ifdef PEAR_NUKLEAR
        renderer_nuklear_free(renderer);
    #endif
    framebuffer_delete(renderer->screen_framebuffer);
    shader_delete(renderer->shader_mesh);
    free(renderer);
}

void renderer_clear(Renderer* renderer, f32 r, f32 g, f32 b, f32 a) {
    #ifdef PEAR_NUKLEAR
        renderer_nuklear_clear(renderer);
    #endif

    renderer_set_target(renderer, renderer->screen_framebuffer);
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    #ifdef PEAR_NUKLEAR
        if (nk_begin(renderer->context, "Demo", nk_rect(50, 50, 230, 250),
            NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_SCALABLE|
            NK_WINDOW_MINIMIZABLE|NK_WINDOW_TITLE))
        {
            enum {EASY, HARD};
            static int op = EASY;
            static int property = 20;
            nk_layout_row_static(renderer->context, 30, 80, 1);
            if (nk_button_label(renderer->context, "button"))
                fprintf(stdout, "button pressed\n");

            nk_layout_row_dynamic(renderer->context, 30, 2);
            if (nk_option_label(renderer->context, "easy", op == EASY)) op = EASY;
            if (nk_option_label(renderer->context, "hard", op == HARD)) op = HARD;

            nk_layout_row_dynamic(renderer->context, 25, 1);
            nk_property_int(renderer->context, "Compression:", 0, &property, 100, 10, 1);

            nk_layout_row_dynamic(renderer->context, 20, 1);
            nk_label(renderer->context, "background:", NK_TEXT_LEFT);
            nk_layout_row_dynamic(renderer->context, 25, 1);
            struct nk_colorf bg;
            bg.r = 0.10f, bg.g = 0.18f, bg.b = 0.24f, bg.a = 1.0f;
            if (nk_combo_begin_color(renderer->context, nk_rgb_cf(bg), nk_vec2(nk_widget_width(renderer->context),400))) {
                nk_layout_row_dynamic(renderer->context, 120, 1);
                bg = nk_color_picker(renderer->context, bg, NK_RGBA);
                nk_layout_row_dynamic(renderer->context, 25, 1);
                bg.r = nk_propertyf(renderer->context, "#R:", 0, bg.r, 1.0f, 0.01f,0.005f);
                bg.g = nk_propertyf(renderer->context, "#G:", 0, bg.g, 1.0f, 0.01f,0.005f);
                bg.b = nk_propertyf(renderer->context, "#B:", 0, bg.b, 1.0f, 0.01f,0.005f);
                bg.a = nk_propertyf(renderer->context, "#A:", 0, bg.a, 1.0f, 0.01f,0.005f);
                nk_combo_end(renderer->context);
            }
        }
        nk_end(renderer->context);

        if (nk_begin(renderer->context, "cool window", nk_rect(50, 50, 250, 250), NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_SCALABLE|NK_WINDOW_MINIMIZABLE|NK_WINDOW_TITLE)) {
            nk_layout_row_dynamic(renderer->context, 0, 1);
            nk_label(renderer->context, "coucou", NK_TEXT_ALIGN_LEFT);
        }
        nk_end(renderer->context);

        renderer_nuklear_render(renderer);
    #endif
}

void renderer_draw_node_hierarchy(Renderer* renderer, Node* node) {
    renderer_set_target(renderer, renderer->screen_framebuffer);

    switch (node_get_type(node)) {
    case NODE_TYPE_MESH_3D:
        renderer_draw_mesh3d(renderer, node_get_data(node));
        break;

    case NODE_TYPE_CAMERA_3D:
        Camera3D* cam = (Camera3D*)node_get_data(node);
        camera3d_get_view_matrix(cam, renderer->view_matrix);
        break;

    case NODE_TYPE_CONTAINER:
    default:
        break;
    }

    if (!node_is_leaf(node)) {
        Node** sons = node_get_sons(node);
        for (u32 i = 0; i < node_get_num_sons(node); i++) {
            renderer_draw_node_hierarchy(renderer, sons[i]);
        }
    }

    renderer_render_to_screen(renderer);
}

void renderer_draw_mesh3d(Renderer* renderer, Mesh3D* node) {
    Mesh* mesh = mesh3d_get_mesh(node);

    shader_use(renderer->shader_mesh);
    renderer_set_uniforms(renderer, mesh_get_material(mesh));

    mesh_use(mesh);
    glDrawElements(GL_TRIANGLES, mesh_get_num_indices(mesh), GL_UNSIGNED_INT, 0);
}

void renderer_set_fov(Renderer* renderer, f32 fov) {
    renderer->fov = fov;
    renderer_calculate_projection(renderer);
}

void renderer_set_target(Renderer* renderer, Framebuffer* framebuffer) {
    framebuffer_use(framebuffer);
}

void renderer_set_target_window(Renderer* renderer) {
    framebuffer_use(renderer->screen_framebuffer);
}

#endif
