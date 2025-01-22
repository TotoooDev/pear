#include <scene/components/camera.h>

component_attachment_t cameracomponent_get_attachment() {
    return (component_attachment_t){
        .creator = NULL,
        .destructor = NULL
    };
}
