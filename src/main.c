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
        struct Request requests[],
        const char *tracefile
);


int main(int argc, char *argv[]) {
    struct arguments *args = parse_args(argc, argv);

    printf("<<-- Einstelungen -->> \n");
    printf("  cyclen: %d\n", args->cycles);
    printf("  cacheline_grosse: %d\n", args->cacheLineSize);
    printf("  l1_lines: %d\n", args->l1CacheLines);
    printf("  l2_lines: %d\n", args->l2CacheLines);
    printf("  l1_latenz: %d\n", args->l1CacheLatency);
    printf("  l2_latenz: %d\n", args->l2CacheLatency);
    printf("  memory_latenz: %d\n", args->memoryLatency);
    if (args->tracefile != NULL) {
        printf("  tracefile: %s.vcd\n", args->tracefile);
    }
    printf("  inputfile: %s\n\n", args->input_file);

    struct csv_file_data data = csv_parse_file(args->input_file);
    printf("<<-- Simulation -->> \n");
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

    printf("\n<<-- Statistiken -->> \n");

    printf("Zyklen gesamt: \t\t\t%zu\n\n", result.cycles);
    printf("Cache-Treffer: \t\t\t%zu\n", result.hits);
    printf("Cache-Fehler: \t\t\t%zu\n\n", result.misses);
    //Wir zählen nur read hits und read miss.
    double hit_rate = (double)result.hits / (result.hits + result.misses);
    printf("Trefferquote lesen: \t\t\t%.2f\n", hit_rate);
    printf("Fehlerquote lesen: \t\t\t%.2f\n\n", 1.0 - hit_rate);
    // Durchschnittliche cycles/instruction (Inclusiv auch Write)
    double avg = (double)result.cycles / data.lines;
    printf("Durchschnittliche Zyklen pro Speicherzugriff: \t%.2f Zyklen pro Anweisung \n", avg);
    printf("Anzahl der primitiven Gates: \t\t%zu\n\n", result.primitiveGateCount);

    free(args);
    return 0;
}