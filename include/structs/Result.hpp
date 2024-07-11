#ifndef RESULT_HPP
#define RESULT_HPP

#include <stddef.h>

struct Result {
    size_t cycles;
    size_t misses;
    size_t hits;
    size_t primitiveGateCount;
};

#endif 
