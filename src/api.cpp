#include "api.hpp"

#include "debugKit.hpp"

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
    const char *tracefile)
{
    struct Result result = { 0 };

    sc_clock clk("clk", 1, SC_SEC);

    CONTROLLER controller(
        "controller", clk,
        l1CacheLines, l2CacheLines, cacheLineSize,
        l1CacheLatency, l2CacheLatency, memoryLatency, 100
    );
    sc_trace_file* tf = sc_create_vcd_trace_file(tracefile);
    LOG("numRequests : " << numRequests);

    sc_signal<uint32_t> address, input_data;
    sc_signal<int> we;

    sc_signal<bool> trigger;

    sc_signal<uint32_t> output;
    sc_signal<bool> done;

    sc_trace(tf, output, "output");
    sc_trace(tf, done, "done");

    controller.address.bind(address);
    controller.data_input.bind(input_data);
    controller.we.bind(we);

    controller.trigger.bind(trigger);
    
    controller.data_output.bind(output);
    controller.done.bind(done);

    LOG("Starting simulation...");
    size_t i = 0;
    for (i = 0; i < numRequests; i++) {
        address.write(requests[i].addr);
        input_data.write(requests[i].data);
        we.write(requests[i].we);

        LOG("Request " << i << ": addr=" << requests[i].addr << ", data=" << requests[i].data << ", we=" << requests[i].we);
        LOG("Triggering...");
        trigger.write(!trigger.read());
        LOG("Waiting for response...");

        do {
            sc_start(1, SC_SEC);
        }
        while (!controller.done.read());
        controller.done.write(false);
        ILOG("Result: " << controller.data_output.read());

        LOG("finished cycle");
    }

    result.cycles = i;

    LOG("Total cycles: " << result.cycles);

    // result.hits = controller.l1.hits_count + controller.l2.hits_count;
    // result.hits = controller.l1.misses_count + controller.l2.misses_count;

    return result;
}

int sc_main(int argc, char* argv[]) {
    std::cerr << "You should use run_simulation instead of this.\n" <<
        "This is just a placeholder." << std::endl;
    return 0;
}