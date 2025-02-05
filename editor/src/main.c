#include <graphics/renderer.h>
#include <core/app.h>
#include <editor.h>

#include <core/thread.h>

void thread_before() {
    printf("before thread!\n");
}

void* thread_func(void* arg) {
    printf("this is in a new thread!\n");
    thread_sleep(3000);
    return NULL;
}

void thread_after() {
    printf("after thread!\n");
}

int main(int argc, char* argv[]) {
    renderer_hint_enable_screen_renderer(false);
    app_init();
    editor_initialize();

    thread_t* thread = thread_new(thread_func, NULL, 0, thread_before, thread_after);
    thread_launch(thread);

    app_run();
    
    editor_delete();

    thread_join(thread);
    thread_delete(thread);

    return 0;
}
