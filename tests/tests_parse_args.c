#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../include/args_parser.h"
//gcc -o tests src/args_parser.c tests_parse_args.c



struct arguments* parse_args(int argc, char **argv);
//Test valid input
void test_parse_args() {
    char *argv1[] = {"./program", "--cacheline-size", "128", "--l1-lines", "512", "--l2-lines", "1024", "--l1-latency", "2", "--l2-latency", "20", "--memory-latency", "200", "--tf=tracefile", "examples/example1.csv"};
    int argc1 = sizeof(argv1) / sizeof(argv1[0]);
    struct arguments *args1 = parse_args(argc1, argv1);
    assert(args1->cycles == DEFAULT_CYCLES_VALUE);
    assert(args1->cacheLineSize == 128);
    assert(args1->l1CacheLines == 512);
    assert(args1->l2CacheLines == 1024);
    assert(args1->l1CacheLatency == 2);
    assert(args1->l2CacheLatency == 20);
    assert(args1->memoryLatency == 200);
    assert(strcmp(args1->tracefile, "tracefile") == 0);
    assert(strcmp(args1->input_file, "../examples/example1.csv") == 0);
    free(args1);
}

int main() {
    test_parse_args();
    printf("All tests passed!\n");
    return 0;
}
