#ifndef REQUEST
#define REQUEST

#include <cstdint>

struct Request {
    uint32_t addr;
    uint32_t data;

    int we;
} Request;

#endif // !REQUEST