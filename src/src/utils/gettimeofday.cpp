//
// Created by Bram on 13/07/2023.
//

#include "utils/gettimeofday.h"

#if defined(_WIN32) || defined(_WIN64)
    #define WIN32_LEAN_AND_MEAN
    #define NOMINMAX
    #include <Windows.h>

int32_t gettimeofday(struct timeval_t* tp, [[maybe_unused]] struct timezone* tzp) {
    // Note: some broken versions only have 8 trailing zero's, the correct epoch
    // has 9 trailing zero's This magic number is the number of 100 nanosecond
    // intervals since January 1, 1601 (UTC) until 00:00:00 January 1, 1970
    constexpr uint64_t epoch = ((uint64_t) 116444736000000000ULL);

    SYSTEMTIME system_time;
    GetSystemTime(&system_time);
    FILETIME file_time;
    SystemTimeToFileTime(&system_time, &file_time);

    uint64_t time = ((uint64_t) file_time.dwLowDateTime);
    time += ((uint64_t) file_time.dwHighDateTime) << 32;

    tp->tv_sec = (int32_t) ((time - epoch) / 10000000L);
    tp->tv_usec = (int32_t) (system_time.wMilliseconds * 1000);
    return 0;
}
#endif