#include "cache.hpp"
#include "controller.hpp"
#include "memory.hpp"
#include <systemc>

using namespace sc_core;

int sc_main(int argc, char *argv[]) {
    sc_clock clk("clk", 1, SC_SEC);
    unsigned l1CacheLines = 4;
    unsigned l2CacheLines = 8;
    unsigned cacheLineSize = 4;
    unsigned l1CacheLatency = 40;
    unsigned l2CacheLatency = 100;
    unsigned memoryLatency = 500;
    unsigned memorySize = 100;

    CONTROLLER controller("controller", clk, l1CacheLines, l2CacheLines,
                          cacheLineSize, l1CacheLatency, l2CacheLatency,
                          memoryLatency, memorySize);

    sc_signal<uint32_t> address;
    sc_signal<uint32_t> input;
    sc_signal<int> we;

    sc_signal<bool> trigger;

    sc_signal<uint32_t> output;
    sc_signal<bool> done;

    controller.address.bind(address);
    controller.data_input.bind(input);
    controller.we.bind(we);
    controller.trigger.bind(trigger);
    controller.data_output.bind(output);
    controller.done.bind(done);

    sc_start(0, SC_SEC);
    address.write(0);
    input.write(0x01020304);
    we.write(1);
    trigger.write(!trigger.read());

    do {
        sc_start(1, SC_SEC);
    } while (!controller.done.read());

    printf("output = %x at %fs\n", output.read(), sc_time_stamp().to_seconds());

    address.write(4);
    we.write(1);
    input.write(0x05060708);
    trigger.write(!trigger.read());

    do {
        sc_start(1, SC_SEC);
    } while (!controller.done.read());

    printf("output = %x at %fs\n", output.read(), sc_time_stamp().to_seconds());

    address.write(2);
    we.write(0);
    trigger.write(!trigger.read());

    do {
        sc_start(1, SC_SEC);
    } while (!controller.done.read());

    printf("output = %x at %fs\n", output.read(), sc_time_stamp().to_seconds());
    // (expect 0x03040506)

    return 0;
}
