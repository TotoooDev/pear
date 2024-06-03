#include <core/log.h>
#include <core/app.h>

int main(int argc, char* argv[]) {
    app_start();
    app_stop();

    PEER_INFO("goodbye!");

    return 0;
}