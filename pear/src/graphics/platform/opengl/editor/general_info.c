#ifdef PEAR_ENABLE_EDITOR

#include <graphics/editor/general_info.h>
#include <core/app.h>

#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include <graphics/editor/cimgui/cimgui.h>

void editor_general_info(bool* show) {
    if (igBegin("general info", show, ImGuiWindowFlags_None)) {
        igText("pear %s", app_get_version_string());
        igText("timestep: %f", app_get_timestep());
        igText("%f fps", 1.0f / app_get_timestep() * 1000.0f);
        igEnd();
    }
}

#endif
