#ifndef PEER_RENDERER_H
#define PEER_RENDERER_H

#include <core/types.h>

typedef struct Renderer Renderer;

Renderer* renderer_new();
void renderer_delete(Renderer* renderer);

void renderer_clear(Renderer* renderer, f32 r, f32 g, f32 b, f32 a);

#endif
