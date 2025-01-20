#include <panels/viewport.h>
#include <graphics/renderer.h>
#include <graphics/editor/editor.h>
#include <core/app.h>

#ifdef PEAR_PLATFORM_OPENGL
#include <graphics/platform/opengl/texture.h>
#endif

void panel_viewport() {
    igPushStyleVar_Vec2(ImGuiStyleVar_WindowPadding, (ImVec2){ 0.0f, 0.0f });
    if (igBegin("viewport", NULL, ImGuiWindowFlags_None)) {
        ImVec2 size;
        igGetContentRegionAvail(&size);
        texture_t* texture = renderer_get_screen_texture(app_get_renderer());

        #ifdef PEAR_PLATFORM_OPENGL
            igImage(texture_get_id(texture), size, (ImVec2){ 0.0f, 0.0f }, (ImVec2){ 1.0f, 1.0f }, (ImVec4){ 1.0f, 1.0f, 1.0f, 1.0f }, (ImVec4){ 0.0f, 0.0f, 0.0f, 0.0f });
        #endif
    }
    igEnd();
    igPopStyleVar(1);
}
