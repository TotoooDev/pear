#include <core/log.h>
#include <core/app.h>
#include <graphics/window.h>

int main(int argc, char* argv[]) {
    app_run();

    PEER_INFO("goodbye!");

    return 0;
}