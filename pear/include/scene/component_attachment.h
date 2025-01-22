#ifndef PEAR_SCENE_COMPONENT_ATTACHMENT_H_
#define PEAR_SCENE_COMPONENT_ATTACHMENT_H_

typedef struct entity_t entity_t;

typedef void(*component_creator_t)(void*, entity_t*);
typedef void(*component_destructor_t)(void*, entity_t*);

typedef struct component_attachment_t {
    component_creator_t creator;
    component_destructor_t destructor;
} component_attachment_t;

#endif
