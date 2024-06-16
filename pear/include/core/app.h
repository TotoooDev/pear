#ifndef PEAR_APPLICATION_H
#define PEAR_APPLICATION_H

#include <scene/node.h>
#include <core/types.h>

void app_init();
void app_stop();
void app_run();
bool app_is_running();
void app_set_root_node(Node* node);

#endif
