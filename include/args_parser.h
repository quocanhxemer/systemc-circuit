#ifndef ARGS_PARSER_H
#define ARGS_PARSER_H

#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#include <getopt.h>
#include <unistd.h>

#ifndef DEFAULTS
	#define DEFAULTS

	#ifndef DEFAULT_CYCLES_VALUE
	#define DEFAULT_CYCLES_VALUE 10
	#endif // !DEFAULT_CYCLES_VALUE

	#ifndef DEFAULT_CACHELINE_SIZE_VALUE
	#define DEFAULT_CACHELINE_SIZE_VALUE 4
	#endif // !DEFAULT_CACHELINE_SIZE_VALUE

	#ifndef DEFAULT_L1_LINES_VALUE
	#define DEFAULT_L1_LINES_VALUE 4
	#endif // !DEFAULT_L1_LINES_VALUE

	#ifndef DEFAULT_L2_LINES_VALUE
	#define DEFAULT_L2_LINES_VALUE 8
	#endif // !DEFAULT_L2_LINES_VALUE

	#ifndef DEFAULT_L1_LATENCY_VALUE
	#define DEFAULT_L1_LATENCY_VALUE 2
	#endif // !DEFAULT_L1_LATENCY_VALUE

	#ifndef DEFAULT_L2_LATENCY_VALUE
	#define DEFAULT_L2_LATENCY_VALUE 3
	#endif // !DEFAULT_L2_LATENCY_VALUE

	#ifndef DEFAULT_MEMORY_LATENCY_VALUE
	#define DEFAULT_MEMORY_LATENCY_VALUE 4
	#endif // !DEFAULT_MEMORY_LATENCY_VALUE
#endif // !DEFAULTS

struct arguments {
	int cycles;
	unsigned cacheLineSize;
	unsigned l1CacheLines;
	unsigned l2CacheLines;
	unsigned l1CacheLatency;
	unsigned l2CacheLatency;
	unsigned memoryLatency;
	const char* tracefile;
	const char* input_file;
};

struct arguments* parse_args(int argc, char **argv);

#endif //!ARGS_PARSER_H;