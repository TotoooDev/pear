#ifdef PEAR_PLATFORM_LINUX

#include <core/thread.h>
#include <core/types.h>
#include <core/log.h>
#include <core/alloc.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

typedef struct thread_t {
    pthread_t id;
    thread_function_t function;
    void* arg;
    void* ret;
} thread_t;

void* thread_function(void* arg) {
    thread_t* thread = (thread_t*)arg;
    thread->ret = thread->function(thread->arg);
    return NULL;
}

thread_t* thread_new(thread_function_t function, void* arg, size_t arg_size) {
    thread_t* thread = (thread_t*)PEAR_MALLOC(sizeof(thread_t));
    thread->function = function;
    thread->ret = NULL;

    if (arg != NULL || arg_size != 0) {
        thread->arg = PEAR_MALLOC(arg_size);
        memcpy(thread->arg, arg, arg_size);
    }
    else {
        thread->arg = NULL;
    }

    return thread;
}

void thread_delete(thread_t* thread) {
    thread_join(thread);

    if (thread->arg != NULL) {
        PEAR_FREE(thread->arg);
    }

    PEAR_FREE(thread);
}

void thread_launch(thread_t* thread) {
    i32 ret = pthread_create(&thread->id, NULL, thread_function, thread);
    if (ret != 0) {
        PEAR_ERROR("failed to create thread!");
    }
}

void thread_join(thread_t* thread) {
    pthread_join(thread->id, NULL);
}

void thread_sleep(u32 time_ms) {
    usleep(time_ms * 1000);
}

void thread_set_function(thread_t* thread, thread_function_t function) {
    thread->function = function;
}

void thread_set_arg(thread_t* thread, void* arg, size_t arg_size) {
    if (thread->arg != NULL) {
        PEAR_FREE(thread->arg);
    }

    thread->arg = PEAR_MALLOC(arg_size);
    memcpy(thread->arg, arg, arg_size);
}

void* thread_get_return_value(thread_t* thread) {
    return thread->ret;
}

#endif
