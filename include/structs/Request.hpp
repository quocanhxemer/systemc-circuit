#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <stdint.h>

struct Request {
    uint32_t addr;
    uint32_t data;

    int we;
};

#endif 
