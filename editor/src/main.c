#include <graphics/renderer.h>
#include <core/app.h>
#include <editor.h>

int main(int argc, char* argv[]) {
    renderer_hint_enable_screen_renderer(false);
    app_init();
    editor_initialize();

    app_run();
    
    editor_delete();

    return 0;
}
