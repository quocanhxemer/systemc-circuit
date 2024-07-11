#ifndef API_H
#define API_H

#include <systemc>
#include <systemc.h>

#include <iostream>

#include <string>

#include "Result.hpp"
#include "Request.hpp"

#include "controller.hpp"

extern "C"
struct Result run_simulation(
    int cycles,
    unsigned l1CacheLines,
    unsigned l2CacheLines,
    unsigned cacheLineSize,
    unsigned l1CacheLatency,
    unsigned l2CacheLatency,
    unsigned memoryLatency,
    size_t numRequests,
    struct Request *requests,
    const char* tracefile = "tracefile"
);


#endif // !API_H