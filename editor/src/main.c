#include <core/app.h>
#include <editor.h>

int main(int argc, char* argv[]) {
    app_init();
    editor_initialize();

    app_run();
    
    editor_delete();

    return 0;
}
