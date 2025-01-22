#include <scene/components/model.h>

void modelcomponent_constructor(void* data, entity_t* entity) {
    model_component_t* model = (model_component_t*)data;

    model->model = NULL;
    model->draw = true;
    model->shadow_caster = true;
}

void modelcomponent_destructor(void* data, entity_t* entity) {
    model_component_t* model = (model_component_t*)data;

    if (model->model != NULL) {
        model_delete(model->model);
    }
}

component_attachment_t modelcomponent_get_attachment() {
    return (component_attachment_t){
        .creator = modelcomponent_constructor,
        .destructor = modelcomponent_destructor
    };
}
