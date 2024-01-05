//
// Created by Bram on 13/07/2023.
//

#pragma once
#include <cstdint>
#include <ctime>

#if defined(_WIN32) || defined(_WIN64)
struct timeval_t {
    uint32_t tv_sec;
    uint32_t tv_usec;
};
int32_t gettimeofday(struct timeval_t* tp, [[maybe_unused]] struct timezone* tzp);
#else
    #include <sys/time.h>
using timeval_t = timeval;
#endif
