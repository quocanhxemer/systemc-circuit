#include <stdio.h>
#include <stddef.h>

#include "Result.hpp"

#include "args_parser.h"
#include "csv_reader.h"

extern struct Result run_simulation(
        int cycles,
        unsigned l1CacheLines,
        unsigned l2CacheLines,
        unsigned cacheLineSize,
        unsigned l1CacheLatency,
        unsigned l2CacheLatency,
        unsigned memoryLatency,
        size_t numRequests,
        struct Request *requests,
        const char *tracefile
);


int main(int argc, char *argv[]) {
    struct arguments* args = parse_args(argc, argv);
    printf("Einstellungen:\n");
    printf("  cycles: %d\n", args->cycles);
    printf("  cacheline_size: %d\n", args->cacheLineSize);
    printf("  l1_lines: %d\n", args->l1CacheLines);
    printf("  l2_lines: %d\n", args->l2CacheLines);
    printf("  l1_latency: %d\n", args->l1CacheLatency);
    printf("  l2_latency: %d\n", args->l2CacheLatency);
    printf("  memory_latency: %d\n", args->memoryLatency);
    if (args->tracefile != NULL) {
        printf("  tracefile: %s\n", args->tracefile);
    }
    printf("  inputfile: %s\n", args->input_file);

    struct csv_file_data data = csv_parse_file(args->input_file);

    struct Result result = run_simulation(
        args->cycles,
        args->l1CacheLines,
        args->l2CacheLines,
        args->cacheLineSize,
        args->l1CacheLatency,
        args->l2CacheLatency,
        args->memoryLatency,
        data.lines,
        data.data,
        args->tracefile
    );

    printf("Cycles: %zu\n", result.cycles);
    printf("Cache Hits: %zu\n", result.hits);
    printf("Cache Miss: %zu\n", result.misses);
    printf("Primitive Gate Count: %zu\n", result.primitiveGateCount);

    free(args);
    return 0;
}