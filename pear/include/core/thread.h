#ifndef PEAR_THREAD_H_
#define PEAR_THREAD_H_

#include <core/types.h>

typedef struct thread_t thread_t;
typedef void*(*thread_function_t)(void*);

thread_t* thread_new(thread_function_t function, void* arg, size_t arg_size);
void thread_delete(thread_t* thread);

void thread_launch(thread_t* thread);
void thread_join(thread_t* thread);
void thread_sleep(u32 time_ms);

void thread_set_function(thread_t* thread, thread_function_t function);
void thread_set_arg(thread_t* thread, void* arg, size_t arg_size);

void* thread_get_return_value(thread_t* thread);

#endif
