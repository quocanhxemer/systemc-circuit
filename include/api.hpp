#ifndef API
#define API

#include "Result.hpp"
#include "Request.hpp"

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


#endif // !API