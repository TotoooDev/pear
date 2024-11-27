#ifdef PEAR_PLATFORM_LINUX

#include <core/timer.h>
#include <time.h>

static struct timespec first_time;

void timer_init() {
    clock_gettime(CLOCK_MONOTONIC, &first_time);
}

f32 timer_get_time_s() {
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    f32 seconds = (f32)(now.tv_sec - first_time.tv_sec);
    f32 nano_seconds = (f32)(now.tv_nsec - first_time.tv_nsec);
    f32 current = seconds + nano_seconds * 1e-9;
    return current;
}

f32 timer_get_time_ms() {
    return timer_get_time_s() * 1e3;
}

f32 timer_get_time_us() {
    return timer_get_time_ms() * 1e6;
}

#endif
