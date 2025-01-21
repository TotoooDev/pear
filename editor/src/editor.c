#include <editor.h>
#include <panels/scene_inspector.h>
#include <panels/entity_inspector.h>
#include <scene/scene.h>

typedef struct editor_t {
    scene_t* scene;
} editor_t;

static editor_t editor;

void editor_set_scene(scene_t* scene) {
    if (editor.scene != NULL) {
        scene_delete(editor.scene);
    }

    editor.scene = scene;
    panel_scene_inspector_set_scene(editor.scene);
    panel_entity_inspector_set_entity(NULL);
}

scene_t* editor_get_scene() {
    return editor.scene;
}
