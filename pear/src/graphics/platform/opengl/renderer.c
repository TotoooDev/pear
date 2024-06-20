#ifdef PEAR_PLATFORM_OPENGL

#include <graphics/renderer.h>
#include <graphics/platform/opengl/shader.h>
#include <graphics/platform/opengl/mesh.h>
#include <graphics/platform/opengl/texture.h>
#include <scene/types/camera_3d.h>
#include <event/event_dispatcher.h>
#include <core/app.h>
#include <core/log.h>
#include <GL/glew.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Renderer {
    f32 fov;
    f32 aspect_ratio;
    f32 near;
    f32 far;

    Shader* shader;
    mat4 projection_matrix;
    mat4 view_matrix;
} Renderer;

static bool renderer_is_glew_init = false;

void renderer_calculate_projection(Renderer* renderer) {
    glm_perspective(renderer->fov, renderer->aspect_ratio, renderer->near, renderer->far, renderer->projection_matrix);
}

void renderer_on_event(EventType type, void* event, void* user_data) {
    if (type != EVENT_TYPE_WINDOW_RESIZED)
        return;

    Renderer* renderer = (Renderer*)user_data;
    WindowResizedEvent* resize_event = (WindowResizedEvent*)event;

    renderer->aspect_ratio = resize_event->width / (f32)resize_event->height;
    renderer_calculate_projection(renderer);
    glViewport(0, 0, resize_event->width, resize_event->height);
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
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            PEAR_INFO("opengl (%d): %s", id, message);
            break;
    }
}

void renderer_set_uniforms(Renderer* renderer, Material material) {
    shader_set_u32(renderer->shader, 0, "u_platform");
    shader_set_mat4(renderer->shader, renderer->projection_matrix, "u_projection");
    shader_set_mat4(renderer->shader, renderer->view_matrix, "u_view");

    if (material.albedo != NULL)
        shader_set_i32(renderer->shader, 0, "u_albedo");
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

void renderer_init_shaders(Renderer* renderer) {
    FILE* vertex_file = fopen("default.vert", "r");
    if (vertex_file == NULL) {
        PEAR_ERROR("failed to open file default.vert!");
        return;
    }

    char* vertex_buffer = NULL;
    size_t vertex_length;
    ssize_t vertex_bytes_read = getdelim(&vertex_buffer, &vertex_length, '\0', vertex_file);
    if (vertex_bytes_read == -1) {
        PEAR_ERROR("failed to read file default.vert!");
        return;
    }

    FILE* fragment_file = fopen("default.frag", "r");
    if (fragment_file == NULL) {
        PEAR_ERROR("failed to open file default.frag!");
        return;
    }

    char* fragment_buffer = NULL;
    size_t fragment_length;
    ssize_t fragment_bytes_read = getdelim(&fragment_buffer, &fragment_length, '\0', fragment_file);
    if (fragment_bytes_read == -1) {
        PEAR_ERROR("failed to read file default.frag!");
        return;
    }

    renderer->shader = shader_new(vertex_buffer, fragment_buffer);

    fclose(vertex_file);
    fclose(fragment_file);
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

    renderer_calculate_projection(renderer);
    glm_mat4_identity(renderer->view_matrix);

    event_subscribe(renderer_on_event, renderer);

    return renderer;
}

void renderer_delete(Renderer* renderer) {
    shader_delete(renderer->shader);
    free(renderer);
}

void renderer_clear(Renderer* renderer, f32 r, f32 g, f32 b, f32 a) {
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void renderer_draw_node_hierarchy(Renderer* renderer, Node* node) {
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
}

void renderer_draw_mesh3d(Renderer* renderer, Mesh3D* node) {
    Mesh* mesh = mesh3d_get_mesh(node);

    shader_use(renderer->shader);
    renderer_set_uniforms(renderer, mesh_get_material(mesh));

    mesh_use(mesh);
    glDrawElements(GL_TRIANGLES, mesh_get_num_indices(mesh), GL_UNSIGNED_INT, 0);
}

#endif
