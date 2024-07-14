#include "api.hpp"

#include "debugKit.hpp"

const int MEMORY_SIZE = 100;

struct Result run_simulation(int cycles, unsigned l1CacheLines,
                             unsigned l2CacheLines, unsigned cacheLineSize,
                             unsigned l1CacheLatency, unsigned l2CacheLatency,
                             unsigned memoryLatency, size_t numRequests,
                             struct Request *requests, const char *tracefile) {
    struct Result result = {0};

    sc_clock clk("clk", 1, SC_SEC);

    CONTROLLER controller("controller", clk, l1CacheLines, l2CacheLines,
                          cacheLineSize, l1CacheLatency, l2CacheLatency,
                          memoryLatency, MEMORY_SIZE);
    LOG("numRequests : " << numRequests);

    sc_signal<uint32_t> address, input_data;
    sc_signal<int> we;

    sc_signal<bool> trigger;

    sc_signal<uint32_t> output;
    sc_signal<bool> done;

    sc_trace_file *tf = nullptr;

    if (tracefile) {
        tf = sc_create_vcd_trace_file(tracefile);
        sc_trace(tf, output, "output");
        sc_trace(tf, done, "done");
    }

    controller.address.bind(address);
    controller.data_input.bind(input_data);
    controller.we.bind(we);

    controller.trigger.bind(trigger);

    controller.data_output.bind(output);
    controller.done.bind(done);

    LOG("Starting simulation...");
    size_t i = 0;
    int cycles_count = 0;
    for (i = 0; i < numRequests; i++) {
        address.write(requests[i].addr);
        input_data.write(requests[i].data);
        we.write(requests[i].we);

        LOG("Request " << i << ": addr=" << requests[i].addr << ", data="
                       << requests[i].data << ", we=" << requests[i].we);
        LOG("Triggering...");
        trigger.write(!trigger.read());
        LOG("Waiting for response...");

        do {
            sc_start(clk.period());
        } while (cycles_count++ < cycles && !controller.done.read());

        if (cycles_count > cycles) {
            break;
        }

        // Only count hits/misses status for read access
        if (!we.read()) {
            if (controller.hit) {
                result.hits++;
            } else {
                result.misses++;
            }
        }

        controller.done.write(false);
        ILOG("Result: " << controller.data_output.read());

        LOG("finished cycle");
    }

    result.cycles = cycles_count;

    result.primitiveGateCount = controller.gates_count;

    LOG("Total cycles: " << result.cycles);

    // result.hits = controller.l1.hits_count + controller.l2.hits_count;
    // result.hits = controller.l1.misses_count + controller.l2.misses_count;

    if (tf) {
        sc_close_vcd_trace_file(tf);
    }

    return result;
}

int sc_main(int argc, char *argv[]) {
    std::cerr << "You should use run_simulation instead of this.\n"
              << "This is just a placeholder." << std::endl;
    return 0;
}