#ifndef RESULT
#define RESULT

#include <cstdio>

struct Result
{   
    size_t cycles;
    size_t misses;
    size_t hits;
    size_t primitiveGateCount;
} Result;

#endif // !RESULT