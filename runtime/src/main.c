#include <core/app.h>
#include <loaders/scene.h>

int main(int argc, char* argv[]) {
    app_init();
    scene_t* scene = loader_load_scene("entry.pear");
    app_set_scene(scene);
    app_run();
    return 0;
}
