#include <panels/general_info.h>
#include <graphics/editor/editor.h>
#include <core/app.h>

void panel_general_info() {
    if (igBegin("general info", NULL, ImGuiWindowFlags_None)) {
        igText("%s", app_get_version_string());
        igSeparator();
        igText("timestep: %f", app_get_timestep());
        igText("%f fps", 1.0f / app_get_timestep() * 1000.0f);
    }
    igEnd();
}
