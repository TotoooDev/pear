#include <core/log.h>

int main(int argc, char* argv[]) {
    PEER_INFO("this is an info log 0x%x", 69);
    PEER_WARN("this is an warn log 0x%x", 69);
    PEER_ERROR("this is an error log 0x%x", 69);
    
    return 0;
}